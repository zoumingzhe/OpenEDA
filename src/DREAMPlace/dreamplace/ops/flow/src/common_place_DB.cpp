/*************************************************************************
  > File Name: legal_detail_place_DB.cpp
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <string>
#include "flow/src/common_place_DB.h"
#include "utility/src/Msg.h"

DREAMPLACE_BEGIN_NAMESPACE

// init class static object
CommonPlaceDB* CommonPlaceDB::place_db_instance_ = nullptr;

// class commosnDB member functions
bool
CommonDB::__isDBLoaded() 
{
  /* Function: heck if DB is loaded correctly */
  if (nullptr == getPlTopCell()) {
    dreamplacePrint(kINFO, "Load DB failed w/o top cell\n");
    return false;
  } 
  if (nullptr == getFloorplan()) {
    dreamplacePrint(kINFO, "Load DB failed w/o floor plan\n");
    return false;
  } 
  if (nullptr == getTechLib()) {
    dreamplacePrint(kINFO, "Load DB failed w/o Tech library\n");
    return false;
  } 
  if (0 == getNumOfCells()) { 
    dreamplacePrint(kINFO, "Load DB failed w/o cells\n");
    return false;
  }
  if (0 == getNumOfInsts()) { 
    dreamplacePrint(kINFO, "Load DB failed w/o instances\n");
    return false;
  }
  return true;
}

void
CommonDB::__init()
{
  // from edi db
  if (!__isDBLoaded()) {
    dreamplacePrint(kINFO, "Deisgn is not loaded\n");
    return;
  }

  /* collect place blks as fixed insts
  PlConstraint blk = nullptr;
  forEachPlaceBlks(blk) {
    forEachRegionBox(blk) {
    } endForEachRegionBox
  } endEachPlaceBlks
  */

 // collect IO as fixed nodes
  PlPin* ioPin = nullptr;
  forEachIOPins(ioPin) {
    if (getPinNet(ioPin)) {
      PlTerm* term = getPinTerm(ioPin); 
      if (term && isTermHasPort(term)) {
        PlPort* port = getTermPortById(term, 0);
        if (port && isPortHasLGeometry(port)) {
          num_io_pins_++;
        }
      }
    }
  } endForEachIOPins

  num_nodes_ = getNumOfInsts() + num_io_pins_;
  init_x_.resize(num_nodes_);
  init_y_.resize(num_nodes_);
  node_size_x_.resize(num_nodes_);
  node_size_y_.resize(num_nodes_);
  // save inst name, orient for GP
  inst_names_.resize(getNumOfInsts());
  inst_orients_.resize(getNumOfInsts());

  /* TODO
  flat_region_boxes_ = new int[];
  flat_region_boxes_start = new int[];
  node2region_map_ = new int[];
  */

  // collect pin num for new memory
  num_nets_ =  getNumOfNets();
  if (num_nets_ > 0) { 
    PlNet* net = nullptr;
    forEachNets(net) {
      forEachNetPins(net) {
        num_pins_++;
      } endForEachNetPins
    } endForEachNets
  }

  if (num_pins_ > 0) {
    // init net to pin
    pin2net_map_.resize(num_pins_);
    flat_net2pin_map_.resize(num_pins_);
    flat_net2pin_start_map_.resize(num_nets_);
    // init inst(node) to pin
    pin2node_map_.resize(num_pins_);
    flat_node2pin_map_.resize(num_pins_);
    flat_node2pin_start_map_.resize(num_nodes_);
    // init pin offset
    pin_offset_x_.resize(num_pins_);
    pin_offset_y_.resize(num_pins_);
    net_mask_.resize(num_nets_);
    // save pin direction, net name, weight for GP
    pin_dirs_.resize(num_pins_);
    net_names_.resize(num_nets_);
    net_weights_.resize(num_nets_);
  }

  idx_to_insts_.resize(num_nodes_);

  int idx = 0;
  int pinIdx = 0;
  // moveable insts at the beginning
  std::unordered_map<PlPin*, PlInt> pin2Idx;
  PlInst* inst = nullptr;
  forEachInsts(inst) {
    if (isInstMoveable(inst)) {
      PlBox box = getInstBox(inst);
      init_x_[idx] = getBoxLLX(box);
      init_y_[idx] = getBoxLLY(box);
      node_size_x_[idx] = getBoxWidth(box);
      node_size_y_[idx] = getBoxHeight(box);
      inst_names_.push_back(inst->getName());
      inst_orients_.push_back(getInstOri(inst));
      idx_to_insts_.push_back(inst);
      // collect moveable inst pins
      if (getInstNumPins(inst) > 0) { 
        flat_node2pin_start_map_[idx] = pinIdx;
        forEachInstPins(inst) {
          pin_dirs_.push_back(getPinDirection(pin));
          pin2Idx.insert(std::make_pair(pin, pinIdx));
          flat_node2pin_map_[pinIdx] = pinIdx;
          pin2node_map_[pinIdx] = idx;
          PlBox pinBox;
          getPin1Box(pin, pinBox);
          pin_offset_x_[pinIdx] = getBoxLLX(pinBox);
          pin_offset_y_[pinIdx] = getBoxLLY(pinBox);
          pinIdx++;
        } endForEachInstPins
      }
      idx++;
    }
  } endForEachInsts
  num_movable_nodes_ = idx;
  num_movable_pins_ = pinIdx;

  // fixed insts after moveable instances
  forEachInsts(inst) {
    if (!isInstMoveable(inst)) {
      PlBox box = getInstBox(inst);
      init_x_[idx] = getBoxLLX(box);
      init_y_[idx] = getBoxLLY(box);
      node_size_x_[idx] = getBoxWidth(box);
      node_size_y_[idx] = getBoxHeight(box);
      inst_names_.push_back(getInstName(inst));
      inst_orients_.push_back(getInstOri(inst));
      idx_to_insts_.push_back(inst);
      // collect fixed inst pins
      if (getInstNumPins(inst) > 0) { 
        flat_node2pin_start_map_[idx] = pinIdx;
        forEachInstPins(inst) {
          pin_dirs_.push_back(getPinDirection(pin));
          pin2Idx.insert(std::make_pair(pin, pinIdx));
          flat_node2pin_map_[pinIdx] = pinIdx;
          pin2node_map_[pinIdx] = idx;
          PlBox pinBox;
          getPin1Box(pin, pinBox);
          pin_offset_x_[pinIdx] = getBoxLLX(pinBox);
          pin_offset_y_[pinIdx] = getBoxLLY(pinBox);
          pinIdx++;
        } endForEachInstPins
      }
      idx++;
    }
  } endForEachInsts

  // treat IOs as fixed insts as well
  forEachIOPins(ioPin) {
    if (getPinNet(ioPin)) {
      PlBox box;
      if (getPin1Box(ioPin, box)) {
        pin_dirs_.push_back(getPinDirection(ioPin));
        pin2Idx.insert(std::make_pair(ioPin, pinIdx));
        init_x_[idx] = getBoxLLX(box);
        init_y_[idx] = getBoxLLY(box);
        node_size_x_[idx] = getBoxWidth(box);
        node_size_y_[idx] = getBoxHeight(box);
        flat_node2pin_start_map_[idx] = pinIdx;
        flat_node2pin_map_[pinIdx] = pinIdx;
        pin2node_map_[pinIdx] = idx;
        pin_offset_x_[pinIdx] = 0;
        pin_offset_y_[pinIdx] = 0;
        pinIdx++;
        idx++;
      }
    }
  } endForEachIOPins

  // pins of same net to be abutted
  if (num_pins_ > 0) {
    pinIdx = 0;
    int netIdx = 0;
    PlNet* net = nullptr;
    forEachNets(net) {
      net_mask_[netIdx] = (isNetClock(net) ? false : true);
      flat_net2pin_start_map_[netIdx] = pinIdx;
      if (getNetPinArray(net) == nullptr) continue;
      forEachNetPins(net) {
        flat_net2pin_map_[pinIdx] = pin2Idx[pin];
        pin2net_map_[pin2Idx[pin]] = netIdx;
        pinIdx++; 
      } endForEachNetPins
      netIdx++;
    } endForEachNets
  }

  // collect fence num and fence box num for new memory
  node2fence_map_.resize(num_nodes_, std::numeric_limits<PlInt>::max());
  if(getNumOfGroups() > 0) {
    int numBox = 0;
    PlGroup* group = nullptr;
    forEachGruops(group) {
      PlConstraint* con = getRegion(group);
      if (con && isRegionFence(con)) {
        num_fences_++;
        forEachRegionBoxs(con) {
          numBox += 4;       // region has 4 locs
        } endForEachRegionBoxs
      }
    } endForEachGroups

    // inst(node) to fence
    if (numBox > 0) {
      // init fence
      flat_fence_boxes_.resize(numBox);
      flat_fence_boxes_start_.resize(num_fences_);
      std::unordered_map<PlConstraint*, int> regionId;
      idx = 0;
      int bIdx = 0;
      forEachGruops(group) {
        PlConstraint* con = getRegion(group);
        if (con && isRegionFence(con)) {
          flat_fence_boxes_start_[idx] = bIdx;
          regionId.insert(std::make_pair(con, idx));
          forEachRegionBoxs(con) {
            flat_fence_boxes_[bIdx++] = getBoxLLX(*box); 
            flat_fence_boxes_[bIdx++] = getBoxLLY(*box); 
            flat_fence_boxes_[bIdx++] = getBoxURX(*box); 
            flat_fence_boxes_[bIdx++] = getBoxURY(*box); 
          } endForEachRegionBoxs
          idx++;
        }
      } endForEachGroups
      idx = 0;
      forEachInsts(inst) {
        if (isInstMoveable(inst)) {
          PlConstraint* con = getInstRegion(inst);
          const auto& iter = regionId.find(con);
          if (iter != regionId.end()) {
            node2fence_map_[idx] = regionId[con];
          }
          idx++; 
        }
      } endForEachInsts
    }
  }

  // collect row boxes for GP
  PlRow* row = nullptr;
  forEachRows(row) {
    PlBox box = getRowBox(row);
    row_boxes_.push_back(box);
  } endForEachRows

  dreamplacePrint(kINFO, "Total %d instance%c, %d moveable instance%c, %d cell%c, %d net%c, %d  pin%c, %d io pin%c, %d io pin instance%c, %d moveable pin%c, %d row%c\n",
    num_nodes_, num_nodes_ > 1 ? 's' : ' ',
    num_movable_nodes_, num_movable_nodes_ > 1 ? 's' : ' ',
    getNumOfCells(), getNumOfCells() > 1 ? 's' : ' ',
    num_nets_, num_nets_ > 1 ? 's' : ' ',
    num_pins_, num_pins_ > 1 ? 's' : ' ',
    getNumOfIOPins(), getNumOfIOPins() > 1 ? 's' : ' ',
    num_io_pins_, num_io_pins_ > 1 ? 's' : ' ',
    num_movable_pins_, num_movable_pins_ > 1 ? 's' : ' ',
    getNumOfRows(), getNumOfRows() > 1 ? 's' : ' '
  );

  /* for debug
  for (int i = 0; i < num_movable_nodes_; ++i)
  {
    dreamplacePrint(kINFO, "inst %d, box: %d, %d, %d, %d, node_size_x %d, node_size_y %d\n",
        i, init_x_[i], init_y_[i], init_x_[i] + node_size_x_[i], init_y_[i]+node_size_y_[i],
        node_size_x_[i], node_size_y_[i] );

  }*/

  isCommonDBReady_ = true;
  dreamplacePrint(kINFO, "DB inilization is completed\n");
  return;
}


void
CommonDB::__free()
{
}

// class CommonPlaceDB member functions
void
CommonPlaceDB::updateDB2EDI()
{
  // update all moveable instances
  for (int i = 0; i < getNumMoveableNodes(); ++i) 
  { 
    PlInst* inst = getInstById(i);
    PlPoint loc(getCurXV().at(i), getCurYV().at(i));
    setInstLoc(inst, loc);
    setInstPStatus(inst, kPlStatus::kPlaced);
  } 
}

void
CommonPlaceDB::updateXY(const int* x, const int* y)
{
  for (int i=0; i < getNumMoveableNodes(); ++i)
  {
    cur_x_.at(i) = x[i];
    cur_y_.at(i) = y[i];
  }
}

void
CommonPlaceDB::summaryMovement()
{
  // summary all moveable instances
  PlUInt totalMove = 0;
  PlUInt maxMove = 0;
  PlUInt meanMove = 0;
  PlInt instNum = getNumMoveableNodes();
  if (instNum > 0) {
    for (int i = 0; i < instNum; ++i) 
    { 
      PlUInt detalX = std::abs(getInitXV()[i]- getCurXV()[i]);
      PlUInt detaly = std::abs(getInitYV()[i]- getCurYV()[i]);
      PlUInt move = detalX + detaly;
      totalMove += move;
      if (maxMove < move) {
        maxMove = move;
      }
    } 
    meanMove = static_cast<PlDouble>(totalMove) / instNum;
  }

  dreamplacePrint(kINFO, "Dreamplace movement summary: moved insts %d mean move %.3f, max move %.3f",
                  instNum, dbuToMicrons(meanMove), dbuToMicrons(maxMove));
}

void
CommonPlaceDB::__buildInternalDB()
{
  // create internal db for LP-DP
  if (!isCommonDBReady()) return;
  // copy core box
  if (!getParaBox().isInvalid()) {
    // box defined by user
    setArea(getParaBox());
  }  else if (false && !getRowBoxes().empty()) {
    // TODO, row boxes is not ready in openEDI DB
    // all rows bounding box
    PlBox rowBBox;
    for (auto box : getRowBoxes())
    {
      rowBBox.maxBox(box);
    }
    setArea(rowBBox);
  } else if (!getCoreBox().isInvalid()) {
    // core area box
    setArea(getCoreBox());
  } else {
    PlPolygon* poly = getCorePolygon();
    PlInt numPoly = getPolygonNumPoint(poly);
    if (numPoly >= 2) {   // area needs 2 points at least
      setArea(poly->getPoint(0).getX(), poly->getPoint(0).getY(),
             poly->getPoint(1).getX(), poly->getPoint(1).getY());
      for (int i = 2; i < numPoly; ++i) 
      {
        if (poly->getPoint(i).getX() < getAreaLLX()) {
          setAreaLLX(poly->getPoint(i).getX());
        } else if (poly->getPoint(i).getX() > getAreaURX()) {
          setAreaURX(poly->getPoint(i).getX());
        }
        if (poly->getPoint(i).getY() < getAreaLLY()) {
          setAreaLLY(poly->getPoint(i).getY());
        } else if (poly->getPoint(i).getY() > getAreaURY()) {
          setAreaURY(poly->getPoint(i).getY());
        }
      }
    }
  }
  // copy current locations
  cur_x_ = getInitXV();
  cur_y_ = getInitYV();
  // copy site  
  PlSite* site = getCoreSite();
  if (site) {
    setRowHight(getSiteH(site));
    setSiteWidth(getSiteW(site));
  }
  if (getParaNumBinsX() > 1) {
    setNumBinsX(getParaNumBinsX());
  }
  if (getParaNumBinsY() > 1) {
    setNumBinsY(getParaNumBinsY());
  }
}

void
CommonPlaceDB::__freeInternalDB() 
{ 
}

void 
CommonPlaceDB::initLegalizationDBGPU(LegalizationDB<int>& db)
{
}

void 
CommonPlaceDB::initLegalizationDBCPU(LegalizationDB<int>& db)
{
  hr_clock_rep total_time_start, total_time_stop;
  total_time_start = get_globaltime(); 

// DB for CPU flow
  db.xl = getAreaLLX();
  db.yl = getAreaLLY();
  db.xh = getAreaURX();
  db.yh = getAreaURY();
  db.site_width = site_width_;
  db.row_height = row_height_;
  db.bin_size_x = (db.xh-db.xl)/num_bins_x_;
  db.bin_size_y = (db.yh-db.yl)/num_bins_y_;
  db.num_bins_x = num_bins_x_;
  db.num_bins_y = num_bins_y_;
  db.num_sites_x = (db.xh-db.xl)/site_width_;
  db.num_sites_y = (db.yh-db.yl)/row_height_;
  db.num_nodes = getNumNodes(); 
  db.num_movable_nodes = getNumMoveableNodes();
  db.num_regions = getNumFences();

  db.init_x = getInitX();
  db.init_y = getInitY();
  db.node_size_x = getNodeSizeX();
  db.node_size_y = getNodeSizeY();
  db.flat_region_boxes = getFlatFenceBoxes();
  db.flat_region_boxes_start = getFlatFenceBoxesStart();
  db.node2fence_region_map = getNode2FenceMap();
  db.x = getCurX();
  db.y = getCurY();

  total_time_stop = get_globaltime();
  dreamplacePrint(kINFO, "Init Legalization DB time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
  return;
}

void 
CommonPlaceDB::initLegalizationDB(LegalizationDB<int>& db)
{
  if (isGPU()) {
#ifdef _CUDA_FOUND
    initLegalizationDBGPU(db);
#endif
  } else {
    initLegalizationDBCPU(db);
  }
}

// class CommonPlaceDB member functions
void CommonPlaceDB::initDetailedPlaceDB(DetailedPlaceDB<int>& db)
{
    hr_clock_rep total_time_start, total_time_stop;
    total_time_start = get_globaltime(); 

    db.xl = getAreaLLX();
    db.yl = getAreaLLY();
    db.xh = getAreaURX();
    db.yh = getAreaURY();
    db.site_width = site_width_;
    db.row_height = row_height_;
    db.bin_size_x = (db.xh-db.xl)/num_bins_x_;
    db.bin_size_y = (db.yh-db.yl)/num_bins_y_;
    db.num_bins_x = num_bins_x_;
    db.num_bins_y = num_bins_y_;
    db.num_sites_x = (db.xh-db.xl)/site_width_;
    db.num_sites_y = (db.yh-db.yl)/row_height_;
    db.num_nodes = getNumNodes(); 
    db.num_movable_nodes = getNumMoveableNodes();
    db.num_nets = getNumNets();
    db.num_pins = getNumPins();
    db.num_regions = getNumFences();

    if (!isGPU())
    {
        db.init_x = getInitX();
        db.init_y = getInitY();
        db.node_size_x = getNodeSizeX();
        db.node_size_y = getNodeSizeY();
        db.flat_region_boxes = getFlatFenceBoxes();
        db.flat_region_boxes_start = getFlatFenceBoxesStart();
        db.node2fence_region_map = getNode2FenceMap();
        db.x = getCurX();
        db.y = getCurY();
        db.flat_net2pin_map = getFlatNet2PinMap();
        db.flat_net2pin_start_map = getFlatNet2PinStartMap();
        db.pin2net_map = getPin2NetMap();
        db.flat_node2pin_map = getFlatNode2PinMap();
        db.flat_node2pin_start_map = getFlatNode2PinStartMap();
        db.pin2node_map = getFlatPin2NodeMap();
        db.pin_offset_x = getPinOffsetX();
        db.pin_offset_y = getPinOffsetY();
        db.net_mask = getNetMask();
    }
    else
    {
#ifdef _CUDA_FOUND
       initDetailedPlaceDBGPU(db);
#endif
    }

    total_time_stop = get_globaltime();
    dreamplacePrint(kINFO, "Init Detailed DB time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}

// class CommonPlaceDB member functions
void CommonPlaceDB::freeDetailedPlaceDB(DetailedPlaceDB<int>& db)
{
    hr_clock_rep total_time_start, total_time_stop;
    total_time_start = get_globaltime(); 
    if (isGPU())
    {
#ifdef _CUDA_FOUND
      freeDetailedPlaceDBGPU(db);
#endif
    }
    else
    {
        //nothing.
    }

    total_time_stop = get_globaltime();
    dreamplacePrint(kINFO, "free Detailed DB time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}

DREAMPLACE_END_NAMESPACE
