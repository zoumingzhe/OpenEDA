/*************************************************************************
  > File Name: legal_detail_place_DB.cpp
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
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
    dreamplacePrint(kINFO, "DB is not load\n");
    return;
  }

  num_nodes_ = getNumOfInsts();
  num_nets_ =  getNumOfNets();
  num_pins_ = getNumOfPins();
  num_regions_ = 0;  
  num_fences_  = 0;

  PlInt num_nodes_ = getNumOfInsts();
  init_x_ = new PlInt[num_nodes_];
  init_y_ = new PlInt[num_nodes_];
  node_size_x_ = new PlInt[num_nodes_];
  node_size_y_ = new PlInt[num_nodes_];
  //flat_region_boxes_ = new int[];
  //flat_region_boxes_start = new int[];
  //node2region_map_ = new int[];
  //flat_fence_boxes_ = new int[];
  //flat_fence_boxes_start = new int[];
  //node2fence_map_ = new int[];
  // net to pin
  pin2net_map_ = new PlInt[num_pins_];
  flat_net2pin_map_ = new PlInt[num_pins_];
  flat_net2pin_start_map_ = new PlInt[num_nets_];
  // inst(node) to pin
  pin2node_map_ = new PlInt[num_pins_];
  flat_node2pin_map_ = new PlInt[num_pins_];
  flat_node2pin_start_map_ = new PlInt[num_nodes_];

  pin_offset_x_ = new PlInt[num_pins_];
  pin_offset_y_ = new PlInt[num_pins_];

  net_mask_ = new unsigned char[num_nets_];
  idx_to_instId_.resize(num_nodes_);

  int idx = 0;
  int pinIdx = 0;
  // moveable insts at the beginning
  forEachInst() {
    if (isInstMoveable(inst)) {
      PlBox box = getInstBox(inst);
      init_x_[idx] = getBoxLLX(box);
      init_y_[idx] = getBoxLLY(box);
      node_size_x_[idx] = getBoxWidth(box);
      node_size_y_[idx] = getBoxHeight(box);
      idx_to_instId_.push_back(instId);
      // collect moveable inst pins
      if (0 == getInstNumPins(inst)) { 
        flat_node2pin_start_map_[idx] = pinIdx;
        forEachInstPin(inst) {
          flat_node2pin_map_[pinIdx] = pinId;
          pin2node_map_[pinId] = idx;
          pin_offset_x_[pinId] = getPinLocX(pin);
          pin_offset_y_[pinId] = getPinLocY(pin);
          pinIdx++;
        } endForEachInstPin
      }
      idx++;
    }
  } endForEachInst
  num_movable_nodes_ = idx;

  // fixed insts after moveable instances
  forEachInst() {
    if (!isInstMoveable(inst)) {
      PlBox box = getInstBox(inst);
      init_x_[idx] = getBoxLLX(box);
      init_y_[idx] = getBoxLLY(box);
      node_size_x_[idx] = getBoxWidth(box);
      node_size_y_[idx] = getBoxHeight(box);
      idx_to_instId_.push_back(instId);
      // collect fixed inst pins
      if (0 == getInstNumPins(inst)) { 
        flat_node2pin_start_map_[idx] = pinIdx;
        forEachInstPin(inst) {
          flat_node2pin_map_[pinIdx] = pinId;
          pin2node_map_[pinId] = idx;
          pin_offset_x_[pinId] = getPinLocX(pin);
          pin_offset_y_[pinId] = getPinLocY(pin);
          pinIdx++;
        } endForEachInstPin
      }
      idx++;
    }
  } endForEachInst

  pinIdx = 0;
  int netIdx = 0;
  dreamplacePrint(kINFO, "Total %d instances, %d moveable instance%c, %d cell%c, %d net%c, %d pin%c \n", 
  num_nodes_, num_movable_nodes_, num_movable_nodes_ > 1 ? 's' : ' ',
  getNumOfCells(), getNumOfCells() > 1 ? 's' : ' ',
  getNumOfNets(), getNumOfNets() > 1 ? 's' : ' ',
  getNumOfPins(), getNumOfPins() > 1 ? 's' : ' ');
  // sort pins of same net to be abutted
  forEachNet() {
    net_mask_[netIdx] = (isNetClock(net) ? false : true);
    flat_net2pin_start_map_[netIdx] = pinIdx;
    if (getNetPinArray(net) == nullptr) {
      continue;
    }
    net->print();
    forEachNetPin(net) {
      flat_net2pin_map_[pinIdx] = pinId;
      pin2net_map_[pinId] = netIdx;
      pinIdx++; 
    } endForEachNetPin
    netIdx++;
  } endForEachNet

  isCommonDBReady_ = true;
  dreamplacePrint(kINFO, "DB inilization is completed\n");
  return;
}

void
CommonDB::__free()
{
  // free edi db
  delete [] init_x_;
  init_x_ = nullptr;
  delete [] init_y_;
  init_y_ = nullptr;
  delete [] node_size_x_;
  node_size_x_ = nullptr;
  delete [] node_size_y_;
  node_size_y_ = nullptr;
  delete [] flat_net2pin_map_;
  flat_net2pin_map_ = nullptr;
  delete [] flat_net2pin_start_map_;
  flat_net2pin_start_map_ = nullptr;
  delete [] pin2net_map_;
  pin2net_map_ = nullptr;
  delete [] flat_node2pin_map_;
  flat_node2pin_map_ = nullptr;
  delete [] flat_node2pin_start_map_;
  flat_node2pin_start_map_ = nullptr;
  delete [] pin2node_map_;
  pin2node_map_ = nullptr;
  delete [] pin_offset_x_;
  pin_offset_x_ = nullptr;
  delete [] pin_offset_y_;
  pin_offset_y_ = nullptr;
  delete [] net_mask_;
  net_mask_ = nullptr;
  return;
}

// class CommonPlaceDB member functions
void
CommonPlaceDB::__buildInternalDB()
{
  // create internal db for LP-DP
  if (!isCommonDBReady()) return;
}


// class CommonPlaceDB member functions
void CommonPlaceDB::initDetailedPlaceDB(DetailedPlaceDB<int>& db, bool gpu=true)
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

    if (!gpu)
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
        //initDetailedPlaceDBGPU(db);
#endif
    }

    total_time_stop = get_globaltime();
    dreamplacePrint(kINFO, "Init Detailed DB time: %g ms\n", get_timer_period()*(total_time_stop-total_time_start));
    return;
}

// class CommonPlaceDB member functions
void CommonPlaceDB::freeDetailedPlaceDB(DetailedPlaceDB<int>& db, bool gpu=true)
{
    hr_clock_rep total_time_start, total_time_stop;
    total_time_start = get_globaltime(); 
    if (gpu)
    {
#ifdef _CUDA_FOUND
        //freeDetailedPlaceDBGPU(db);
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
