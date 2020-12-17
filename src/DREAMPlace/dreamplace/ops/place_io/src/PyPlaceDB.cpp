/**
 * @file   PyPlaceDB.cpp
 * @author Yibo Lin
 * @date   Apr 2020
 * @brief  Placement database for python 
 */

#include "PyPlaceDB.h"
#include <boost/polygon/polygon.hpp>
#ifdef _CMAKE_PLACE
#include "flow/src/common_place_DB.h"
#endif

DREAMPLACE_BEGIN_NAMESPACE

bool readLef(PlaceDB& db)
{
	// read lef 
    std::vector<std::string> const& vLefInput = db.userParam().vLefInput;
    for (std::vector<std::string>::const_iterator it = vLefInput.begin(), ite = vLefInput.end(); it != ite; ++it)
    {
        std::string const& filename = *it;
        dreamplacePrint(kINFO, "reading %s\n", filename.c_str());
        bool flag = LefParser::read(db, filename);
        if (!flag) 
        {
            dreamplacePrint(kERROR, "LEF file parsing failed: %s\n", filename.c_str());
            return false;
        }
    }

	return true;
}

bool readDef(PlaceDB& db)
{
	// read def 
    std::string const& defInput = db.userParam().defInput;
    if (!defInput.empty())
    {
        std::string const& filename = defInput;
        dreamplacePrint(kINFO, "reading %s\n", filename.c_str());
        // a pre-reading phase to grep number of components, nets, and pins 
        prereadDef(db, filename);
        bool flag = DefParser::read(db, filename);
        if (!flag) 
        {
            dreamplacePrint(kERROR, "DEF file parsing failed: %s\n", filename.c_str());
            return false;
        }
    }
	else dreamplacePrint(kWARN, "no DEF file specified\n");

	return true;
}

void prereadDef(PlaceDB& db, std::string const& filename)
{
    std::ifstream inFile (filename.c_str());
    if (!inFile.good())
        return;

    // need to extract following information 
    unsigned numRows = 0;
    unsigned numNodes = 0;
    unsigned numIOPin = 0;
    unsigned numNets = 0;
    unsigned numBlockages = 0;

    std::string line;
    std::string token;
    while (!inFile.eof())
    {
        std::getline(inFile, line);
        if (line.compare(0, 3, "ROW") == 0) // a line starts with keyword "ROW"
            ++numRows;
        else if (line.compare(0, 10, "COMPONENTS") == 0)
        {
            std::istringstream iss (line);
            iss >> token >> numNodes >> token;
        }
        else if (line.compare(0, 4, "PINS") == 0)
        {
            std::istringstream iss (line);
            iss >> token >> numIOPin >> token;
        }
        else if (line.compare(0, 4, "NETS") == 0)
        {
            std::istringstream iss (line);
            iss >> token >> numNets >> token;
        }
        else if (line.compare(0, 9, "BLOCKAGES") == 0)
        {
            std::istringstream iss (line);
            iss >> token >> numBlockages >> token;
        }
    }

    dreamplacePrint(kINFO, "detect %u rows, %u components, %u IO pins, %u nets, %u blockages\n", numRows, numNodes, numIOPin, numNets, numBlockages);
    db.prepare(numRows, numNodes, numIOPin, numNets, numBlockages);

    inFile.close();
}

bool readVerilog(PlaceDB& db)
{
    // read verilog 
    std::string const& verilogInput = db.userParam().verilogInput;
    if (!verilogInput.empty())
    {
        std::string const& filename = verilogInput;
        dreamplacePrint(kINFO, "reading %s\n", filename.c_str());
        bool flag = VerilogParser::read(db, filename);
        if (!flag)
        {
            dreamplacePrint(kERROR, "Verilog file parsing failed: %s\n", filename.c_str());
            return false;
        }
    }
    else dreamplacePrint(kWARN, "no Verilog file specified\n");

    return true;
}

bool readBookshelf(PlaceDB& db)
{
    // read bookshelf 
    std::string const& bookshelfAuxInput = db.userParam().bookshelfAuxInput;
    if (!bookshelfAuxInput.empty())
    {
        std::string const& filename = bookshelfAuxInput;
        dreamplacePrint(kINFO, "reading %s\n", filename.c_str());
        bool flag = BookshelfParser::read(db, filename);
        if (!flag)
        {
            dreamplacePrint(kERROR, "Bookshelf file parsing failed: %s\n", filename.c_str());
            return false;
        }
    }
    else dreamplacePrint(kWARN, "no Bookshelf file specified\n");

    // read additional .pl file 
    std::string const& bookshelfPlInput = db.userParam().bookshelfPlInput;
    if (!bookshelfPlInput.empty())
    {
        std::string const& filename = bookshelfPlInput;
        dreamplacePrint(kINFO, "reading %s\n", filename.c_str());
        bool flag = BookshelfParser::readPl(db, filename);
        if (!flag)
        {
            dreamplacePrint(kERROR, "Bookshelf additional .pl file parsing failed: %s\n", filename.c_str());
            return false;
        }
    }
    else dreamplacePrint(kWARN, "no additional Bookshelf .pl file specified\n");

    return true;
}

#ifdef _CMAKE_PLACE
void 
PyPlaceDB::set(unsigned long db_ptr)
{
    /* new set function for place_design flow */
    namespace gtl = boost::polygon; 
    using namespace gtl::operators;
    typedef gtl::polygon_90_set_data<PlaceDB::coordinate_type> PolygonSet; 

    dreamplacePrint(kINFO, "Preparing python DB.\n");
    CommonPlaceDB *db = (CommonPlaceDB*)db_ptr;
    if (nullptr == db) {
      dreamplacePrint(kERROR, "Not find place common DB.\n");
      return;
    }

    // record original node to new node mapping 
    std::vector<std::vector<PlaceDB::index_type> > mNode2NewNodes (db->getNumNodes()); 

    num_nodes = db->getNumNodes();
    num_terminal_NIs = db->getNumIOPins();
    num_terminals = num_nodes - num_terminal_NIs - db->getNumMoveableNodes();
    int realNodeNum = db->getNumNodes() - num_terminal_NIs;
    for (int i = 0; i < realNodeNum; ++i) 
    {
      //pybind11::dict node_name2id_map; ///< node name to id map, cell name 
      node_name2id_map[pybind11::str(db->getInstNameById(i))] = i; 
      //pybind11::list node_orient; ///< 1D array, cell orientation 
      node_orient.append(pybind11::str(toString(db->getInstOriById(i)))); 
    }
    //pybind11::list node_names; ///< 1D array, cell name
    node_names = pybind11::list(pybind11::cast(db->getInstNames())); 
    //node_orient = pybind11::list(pybind11::cast(db->getInstOrients())); 

    node_x = pybind11::list(pybind11::cast(db->getInitXV()));
    node_y = pybind11::list(pybind11::cast(db->getInitYV()));
    node_size_x = pybind11::list(pybind11::cast(db->getNodeSizeXV()));
    node_size_y = pybind11::list(pybind11::cast(db->getNodeSizeYV()));

    //pybind11::list node2orig_node_map?
    for (int i = 0; i < db->getNumPins(); ++i) {
      //pybind11::list pin_direct; ///< 1D array, pin direction IO 
      pin_direct.append(toString(db->getPinDirById(i))); 
    }
    pin_offset_x = pybind11::list(pybind11::cast(db->getPinOffsetXV()));
    pin_offset_y = pybind11::list(pybind11::cast(db->getPinOffsetYV()));

    //pybind11::list net2pin_map; ///< array of 1D array, each row stores pin id? 
    flat_net2pin_map = pybind11::list(pybind11::cast(db->getFlatNet2PinMapV()));
    flat_net2pin_start_map = pybind11::list(pybind11::cast(db->getFlatNet2PinStartMapV()));
    //pybind11::list net_weights; ///< net weight 
    if (db->getNumNets() > 0) {
      //pybind11::list net_names; ///< net name 
      net_names = pybind11::list(pybind11::cast(db->getNetNames()));
      net_weights = pybind11::list(pybind11::cast(db->getNetWeights()));
      for (int i = 0; i < db->getNumNets(); ++i)
      {
        //pybind11::dict net_name2id_map; ///< net name to id map
        net_name2id_map[pybind11::str(db->getNetNameById(i))] = i; 
      }
    }
    //pybind11::list node2pin_map; ///< array of 1D array, contains pin id of each node?
    flat_node2pin_map = pybind11::list(pybind11::cast(db->getFlatNode2PinMapV()));
    flat_node2pin_start_map = pybind11::list(pybind11::cast(db->getFlatNode2PinStartMapV()));
    pin2node_map = pybind11::list(pybind11::cast(db->getFlatPin2NodeMapV()));
    pin2net_map = pybind11::list(pybind11::cast(db->getPin2NetMapV()));

    // collect all row boxes
    for (auto box : db->getRowBoxes()) 
    {
      pybind11::tuple row = pybind11::make_tuple(getBoxLLX(box), getBoxLLY(box), getBoxURX(box), getBoxURY(box)); 
      rows.append(row); 
    }

    //pybind11::list regions; ///< array of 1D array, each region contains rectangles? 
    flat_region_boxes = pybind11::list(pybind11::cast(db->getFlatFenceBoxesV()));
    flat_region_boxes_start = pybind11::list(pybind11::cast(db->getFlatFenceBoxesStartV()));
    node2fence_region_map = pybind11::list(pybind11::cast(db->getNode2FenceMapV()));

    //TODO for routing info
    //unsigned int num_routing_grids_x; ///< number of routing grids in x 
    //unsigned int num_routing_grids_y; ///< number of routing grids in y 
    //int routing_grid_xl; ///< routing grid region may be different from placement region 
    //int routing_grid_yl; 
    //int routing_grid_xh; 
    //int routing_grid_yh;
    //pybind11::list unit_horizontal_capacities; ///< number of horizontal tracks of layers per unit distance 
    //pybind11::list unit_vertical_capacities; /// number of vertical tracks of layers per unit distance 
    //pybind11::list initial_horizontal_demand_map; ///< initial routing demand from fixed cells, indexed by (layer, grid x, grid y) 
    //pybind11::list initial_vertical_demand_map; ///< initial routing demand from fixed cells, indexed by (layer, grid x, grid y)   
    //
    xl = db->getAreaLLX();
    yl = db->getAreaLLY();
    xh = db->getAreaURX();
    yh = db->getAreaURY();
    row_height = db->getRowHight();
    site_width = db->getSiteWidth();

    // collect boxes for fixed cells and put in a polygon set to remove overlap later 
    std::vector<gtl::rectangle_data<PlaceDB::coordinate_type>> fixed_boxes; 
    double total_fixed_node_area = 0; // compute total area of fixed cells, which is an upper bound  
    if (db->getNumNodes() > 0 && 
        db->getNumNodes() > db->getNumMoveableNodes()) {
      for (int i = db->getNumMoveableNodes(); i < realNodeNum; ++i)
      {
        total_fixed_node_area += (db->getNodeSizeXV().at(i) * db->getNodeSizeYV().at(i));
        fixed_boxes.emplace_back(db->getInitXV().at(i),
                                 db->getInitYV().at(i),
                                 db->getInitXV().at(i) + db->getNodeSizeXV().at(i),
                                 db->getInitYV().at(i) + db->getNodeSizeYV().at(i));
      }
    }

    PolygonSet ps (gtl::HORIZONTAL, fixed_boxes.begin(), fixed_boxes.end());
    // critical to make sure only overlap with the die area is computed 
    ps &= gtl::rectangle_data<PlaceDB::coordinate_type>(xl, yl, xh, yh);
    double total_fixed_node_overlap_area = gtl::area(ps);
    total_space_area = getBoxArea(db->getArea()) -
                       std::min(total_fixed_node_overlap_area, total_fixed_node_area); 
    num_movable_pins = db->getNumMoveablePins(); 
    dreamplacePrint(kINFO, "Completed python DB.\n");
    
}
#endif

void PyPlaceDB::set(PlaceDB const& db) 
{
    namespace gtl = boost::polygon; 
    using namespace gtl::operators;
    typedef gtl::polygon_90_set_data<PlaceDB::coordinate_type> PolygonSet; 

    num_terminal_NIs = db.numIOPin();  // IO pins 
    double total_fixed_node_area = 0; // compute total area of fixed cells, which is an upper bound  
    // collect boxes for fixed cells and put in a polygon set to remove overlap later 
    std::vector<gtl::rectangle_data<PlaceDB::coordinate_type>> fixed_boxes; 
    // record original node to new node mapping 
    std::vector<std::vector<PlaceDB::index_type> > mNode2NewNodes (db.nodes().size()); 

    // add a node to a bin 
    auto addNode2Bin = [&](Box<PlaceDB::coordinate_type> const& box) {
        fixed_boxes.emplace_back(box.xl(), box.yl(), box.xh(), box.yh());
    };
    // general add a node 
    auto addNode = [&](Node const& node, 
            std::string const& name, Orient orient, 
            Box<PlaceDB::coordinate_type> const& box, bool dist2map) {
        // this id may be different from node id 
        int id = node_names.size(); 
        node_name2id_map[pybind11::str(name)] = id; 
        node_names.append(pybind11::str(name)); 
        node_x.append(box.xl()); 
        node_y.append(box.yl()); 
        node_orient.append(pybind11::str(std::string(orient))); 
        node_size_x.append(box.width()); 
        node_size_y.append(box.height()); 
        // map new node to original index 
        node2orig_node_map.append(node.id()); 
        // record original node to new node mapping 
        mNode2NewNodes.at(node.id()).push_back(id); 
        if (dist2map)
        {
            //dreamplacePrint(kDEBUG, "node %s\n", db.nodeName(node).c_str());
            addNode2Bin(box);
        }
    };
    // add obstruction boxes for fixed nodes 
    // initialize node shapes from obstruction 
    // I do not differentiate obstruction boxes at different layers
    // At least, this is true for DAC/ICCAD 2012 benchmarks 
    auto addObsBoxes = [&](Node const& node, std::vector<Box<PlaceDB::coordinate_type> > const& vBox, bool dist2map){
        Box<PlaceDB::coordinate_type> bbox; 
        for (PlaceDB::index_type i = 0; i < vBox.size(); ++i)
        {
            auto box = vBox[i]; 
            box.set(box.xl() + node.xl(), 
                    box.yl() + node.yl(), 
                    box.xh() + node.xl(), 
                    box.yh() + node.yl()
                   );
            char buf[128]; 
            dreamplaceSPrint(kNONE, buf, "%s.DREAMPlace.Shape%u", db.nodeName(node).c_str(), i); 
            addNode(node, std::string(buf), Orient(node.orient()), box, dist2map);
            bbox.encompass(box); 
        }
        // compute the upper bound of fixed cell area 
        if (dist2map)
        {
            total_fixed_node_area += bbox.area();
        }
    };

    num_terminals = 0; // regard only fixed macros as macros, placement blockages are ignored  
    for (unsigned int i = 0; i < db.nodes().size(); ++i)
    {
        Node const& node = db.node(i); 
        Macro const& macro = db.macro(db.macroId(node));
        if (node.status() != PlaceStatusEnum::FIXED || i >= db.nodes().size() - num_terminal_NIs)
        {
            addNode(node, db.nodeName(node), Orient(node.orient()), node, false); 
        }
        else if (macro.className() != "DREAMPlace.PlaceBlockage") // fixed cells are special cases, skip placement blockages (looks like ISPD2015 benchmarks do not process placement blockages) 
        {
            Macro const& macro = db.macro(db.macroId(node));

            if (!macro.obs().empty())
            {
                MacroObs::ObsConstIterator foundObs = macro.obs().obsMap().find("Bookshelf.Shape"); 
                // add obstruction boxes for fixed nodes 
                // initialize node shapes from obstruction 
                // I do not differentiate obstruction boxes at different layers
                // At least, this is true for DAC/ICCAD 2012 benchmarks 

                // put all boxes into a polygon set to remove overlaps 
                // this can make the placement engine more robust 
                PolygonSet ps; 
                if (foundObs != macro.obs().end()) // for BOOKSHELF
                {
                    for (auto const& box : foundObs->second)
                    {
                        ps.insert(gtl::rectangle_data<PlaceDB::coordinate_type>(box.xl(), box.yl(), box.xh(), box.yh())); 
                    }
                }
                else 
                {
                    for (auto it = macro.obs().begin(), ite = macro.obs().end(); it != ite; ++it)
                    {
                        for (auto const& box : it->second)
                        {
                            ps.insert(gtl::rectangle_data<PlaceDB::coordinate_type>(box.xl(), box.yl(), box.xh(), box.yh())); 
                        }
                    }

                    // I do not know whether we should add the bounding box of this fixed cell as well 
                    ps.insert(gtl::rectangle_data<PlaceDB::coordinate_type>(0, 0, node.width(), node.height()));
                }

                // Get unique boxes without overlap for each fixed cell
                // However, there may still be overlapping between fixed cells. 
                // We cannot eliminate these because we want to keep the mapping from boxes to cells. 
                std::vector<gtl::rectangle_data<PlaceDB::coordinate_type>> vRect; 
                ps.get_rectangles(vRect); 
                std::vector<Box<PlaceDB::coordinate_type>> vBox; 
                vBox.reserve(vRect.size()); 
                for (auto const& rect : vRect) 
                {
                    vBox.emplace_back(gtl::xl(rect), gtl::yl(rect), gtl::xh(rect), gtl::yh(rect));
                }
                addObsBoxes(node, vBox, true); 
                num_terminals += vBox.size(); 
            }
            else 
            {
                addNode(node, db.nodeName(node), Orient(node.orient()), node, true); 
                num_terminals += 1; 
                // compute upper bound of total fixed cell area 
                total_fixed_node_area += node.area();
            }
        }
    }
    // we only know num_nodes when all fixed cells with shapes are expanded 
    dreamplacePrint(kDEBUG, "num_terminals %d, numFixed %u, numPlaceBlockages %u, num_terminal_NIs %d\n", 
        num_terminals, db.numFixed(), db.numPlaceBlockages(), num_terminal_NIs);
    num_nodes = db.nodes().size() + num_terminals - db.numFixed() - db.numPlaceBlockages(); 
    dreamplaceAssertMsg(num_nodes == node_x.size(), 
        "%u != %lu, db.nodes().size = %lu, num_terminals = %d, numFixed = %u, numPlaceBlockages = %u, num_terminal_NIs = %d", 
        num_nodes, node_x.size(), db.nodes().size(), num_terminals, db.numFixed(), db.numPlaceBlockages(), num_terminal_NIs);

    // this is different from simply summing up the area of all fixed nodes 
    double total_fixed_node_overlap_area = 0;
    // compute total area uniquely 
    {
        PolygonSet ps (gtl::HORIZONTAL, fixed_boxes.begin(), fixed_boxes.end());
        // critical to make sure only overlap with the die area is computed 
        ps &= gtl::rectangle_data<PlaceDB::coordinate_type>(db.rowXL(), db.rowYL(), db.rowXH(), db.rowYH());
        total_fixed_node_overlap_area = gtl::area(ps);
    }
    // the total overlap area should not exceed the upper bound; 
    // current estimation may exceed if there are many overlapping fixed cells or boxes 
    total_space_area = db.rowBbox().area() - std::min(total_fixed_node_overlap_area, total_fixed_node_area); 
    dreamplacePrint(kDEBUG, "fixed area overlap: %g total: %g, space area = %g\n", total_fixed_node_overlap_area, total_fixed_node_area, total_space_area);

    // construct node2pin_map and flat_node2pin_map
    int count = 0; 
    for (unsigned int i = 0; i < mNode2NewNodes.size(); ++i)
    {
        Node const& node = db.node(i);
        for (unsigned int j = 0; j < mNode2NewNodes.at(i).size(); ++j)
        {
            pybind11::list pins; 
            if (j == 0) // for fixed macros with multiple boxes, put all pins to the first one 
            {
                for (auto pin_id : node.pins())
                {
                    pins.append(pin_id); 
                    flat_node2pin_map.append(pin_id); 
                }
            }
            node2pin_map.append(pins); 
            flat_node2pin_start_map.append(count); 
            if (j == 0) // for fixed macros with multiple boxes, put all pins to the first one 
            {
                count += node.pins().size(); 
            }
        }
    }
    flat_node2pin_start_map.append(count); 

    num_movable_pins = 0; 
    for (unsigned int i = 0, ie = db.pins().size(); i < ie; ++i)
    {
        Pin const& pin = db.pin(i); 
        Node const& node = db.getNode(pin); 
        pin_direct.append(std::string(pin.direct())); 
        // for fixed macros with multiple boxes, put all pins to the first one 
        PlaceDB::index_type new_node_id = mNode2NewNodes.at(node.id()).at(0); 
        Pin::point_type pin_pos (node.pinPos(pin));
        pin_offset_x.append(pin_pos.x() - node_x[new_node_id].cast<PlaceDB::coordinate_type>()); 
        pin_offset_y.append(pin_pos.y() - node_y[new_node_id].cast<PlaceDB::coordinate_type>()); 
        pin2node_map.append(new_node_id); 
        pin2net_map.append(db.getNet(pin).id()); 

        if (node.status() != PlaceStatusEnum::FIXED /*&& node.status() != PlaceStatusEnum::DUMMY_FIXED*/)
        {
            num_movable_pins += 1; 
        }
    }
    count = 0; 
    for (unsigned int i = 0, ie = db.nets().size(); i < ie; ++i)
    {
        Net const& net = db.net(i); 
        net_weights.append(net.weight());
        net_name2id_map[pybind11::str(db.netName(net))] = net.id(); 
        net_names.append(pybind11::str(db.netName(net))); 
        pybind11::list pins; 
        for (std::vector<Net::index_type>::const_iterator it = net.pins().begin(), ite = net.pins().end(); it != ite; ++it)
        {
            pins.append(*it);
        }
        net2pin_map.append(pins); 

        for (std::vector<Net::index_type>::const_iterator it = net.pins().begin(), ite = net.pins().end(); it != ite; ++it)
        {
            flat_net2pin_map.append(*it); 
        }
        flat_net2pin_start_map.append(count); 
        count += net.pins().size(); 
    }
    flat_net2pin_start_map.append(count); 

    for (std::vector<Row>::const_iterator it = db.rows().begin(), ite = db.rows().end(); it != ite; ++it)
    {
        pybind11::tuple row = pybind11::make_tuple(it->xl(), it->yl(), it->xh(), it->yh()); 
        rows.append(row); 
    }

    // initialize regions 
    count = 0; 
    for (std::vector<Region>::const_iterator it = db.regions().begin(), ite = db.regions().end(); it != ite; ++it)
    {
        Region const& region = *it; 
        pybind11::list boxes; 
        for (std::vector<Region::box_type>::const_iterator itb = region.boxes().begin(), itbe = region.boxes().end(); itb != itbe; ++itb)
        {
            pybind11::tuple box = pybind11::make_tuple(itb->xl(), itb->yl(), itb->xh(), itb->yh());
            boxes.append(box);
            flat_region_boxes.append(box);
        }
        regions.append(boxes);
        flat_region_boxes_start.append(count); 
        count += region.boxes().size();
    }
    flat_region_boxes_start.append(count); 

    // I assume one cell only belongs to one FENCE region 
    std::vector<int> vNode2FenceRegion (db.numMovable() + db.numFixed(), std::numeric_limits<int>::max()); 
    for (std::vector<Group>::const_iterator it = db.groups().begin(), ite = db.groups().end(); it != ite; ++it)
    {
        Group const& group = *it; 
        Region const& region = db.region(group.region());
        if (region.type() == RegionTypeEnum::FENCE)
        {
            for (std::vector<Group::index_type>::const_iterator itn = group.nodes().begin(), itne = group.nodes().end(); itn != itne; ++itn)
            {
                Group::index_type node_id = *itn; 
                if (db.node(node_id).status() != PlaceStatusEnum::FIXED) // ignore fixed cells 
                {
                    vNode2FenceRegion.at(node_id) = region.id();
                }
            }
        }
    }
    for (std::vector<int>::const_iterator it = vNode2FenceRegion.begin(), ite = vNode2FenceRegion.end(); it != ite; ++it)
    {
        node2fence_region_map.append(*it); 
    }

    xl = db.rowXL(); 
    yl = db.rowYL(); 
    xh = db.rowXH(); 
    yh = db.rowYH(); 

    row_height = db.rowHeight(); 
    site_width = db.siteWidth(); 

    // routing information initialized 
    num_routing_grids_x = 0; 
    num_routing_grids_y = 0; 
    routing_grid_xl = xl; 
    routing_grid_yl = yl; 
    routing_grid_xh = xh; 
    routing_grid_yh = yh; 
    if (!db.routingCapacity(PlanarDirectEnum::HORIZONTAL).empty())
    {
        num_routing_grids_x = db.numRoutingGrids(kX); 
        num_routing_grids_y = db.numRoutingGrids(kY);
        routing_grid_xl = db.routingGridOrigin(kX);
        routing_grid_yl = db.routingGridOrigin(kY); 
        routing_grid_xh = routing_grid_xl + num_routing_grids_x * db.routingTileSize(kX); 
        routing_grid_yh = routing_grid_yl + num_routing_grids_y * db.routingTileSize(kY); 
        for (PlaceDB::index_type layer = 0; layer < db.numRoutingLayers(); ++layer)
        {
            unit_horizontal_capacities.append((double)db.numRoutingTracks(PlanarDirectEnum::HORIZONTAL, layer) / db.routingTileSize(kY)); 
            unit_vertical_capacities.append((double)db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer) / db.routingTileSize(kX));
        }
        // this is slightly different from db.routingGridOrigin 
        // to be consistent with global placement 
        double routing_grid_size_x = db.routingTileSize(kX); 
        double routing_grid_size_y = db.routingTileSize(kY);
        double routing_grid_area = routing_grid_size_x * routing_grid_size_y;
        std::vector<int> initial_horizontal_routing_map (db.numRoutingLayers() * num_routing_grids_x * num_routing_grids_y, 0); 
        std::vector<int> initial_vertical_routing_map (initial_horizontal_routing_map.size(), 0); 
        for (FixedNodeConstIterator it = db.fixedNodeBegin(); it.inRange(); ++it)
        {
            Node const& node = *it; 
            Macro const& macro = db.macro(db.macroId(node)); 

            for (MacroObs::ObsConstIterator ito = macro.obs().begin(); ito != macro.obs().end(); ++ito)
            {
                if (ito->first != "Bookshelf.Shape") // skip dummy layer for BOOKSHELF 
                {
                    std::string const& layerName = ito->first; 
                    PlaceDB::index_type layer = db.getLayer(layerName); 
                    for (auto const& obs_box : ito->second)
                    {
                        // convert to absolute box 
                        MacroObs::box_type box (node.xl() + obs_box.xl(), node.yl() + obs_box.yl(), 
                                node.xl() + obs_box.xh(), node.yl() + obs_box.yh());
                        PlaceDB::index_type grid_index_xl = std::max(int((box.xl() - db.routingGridOrigin(kX)) / routing_grid_size_x), 0); 
                        PlaceDB::index_type grid_index_yl = std::max(int((box.yl() - db.routingGridOrigin(kY)) / routing_grid_size_y), 0);
                        PlaceDB::index_type grid_index_xh = std::min(unsigned((box.xh() - db.routingGridOrigin(kX)) / routing_grid_size_x) + 1, num_routing_grids_x); 
                        PlaceDB::index_type grid_index_yh = std::min(unsigned((box.yh() - db.routingGridOrigin(kY)) / routing_grid_size_y) + 1, num_routing_grids_y); 
                        for (PlaceDB::index_type k = grid_index_xl; k < grid_index_xh; ++k)
                        {
                            PlaceDB::coordinate_type grid_xl = db.routingGridOrigin(kX) + k * routing_grid_size_x; 
                            PlaceDB::coordinate_type grid_xh = grid_xl + routing_grid_size_x;
                            for (PlaceDB::index_type h = grid_index_yl; h < grid_index_yh; ++h)
                            {
                                PlaceDB::coordinate_type grid_yl = db.routingGridOrigin(kY) + h * routing_grid_size_y; 
                                PlaceDB::coordinate_type grid_yh = grid_yl + routing_grid_size_y; 
                                MacroObs::box_type grid_box (grid_xl, grid_yl, grid_xh, grid_yh); 
                                PlaceDB::index_type index = layer * num_routing_grids_x * num_routing_grids_y + (k * num_routing_grids_y + h);
                                double intersect_ratio = intersectArea(box, grid_box) / routing_grid_area;
                                dreamplaceAssert(intersect_ratio <= 1);
                                initial_horizontal_routing_map[index] += ceil(intersect_ratio * db.numRoutingTracks(PlanarDirectEnum::HORIZONTAL, layer)); 
                                initial_vertical_routing_map[index] += ceil(intersect_ratio * db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer)); 
                                if (layer == 2)
                                {
                                    dreamplaceAssert(db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer) == 0);
                                    dreamplaceAssertMsg(initial_vertical_routing_map[index] == 0, "intersect_ratio %g, initial_vertical_routing_map[%u] = %d, capacity %u, product %g", 
                                            intersect_ratio, index, initial_vertical_routing_map[index], 
                                            db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer), 
                                            intersect_ratio * db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer)
                                            );
                                }
                            }
                        }
                    }
                }
            }
        }
        // clamp maximum for overlapping fixed cells 
        for (PlaceDB::index_type layer = 0; layer < db.numRoutingLayers(); ++layer)
        {
            for (int i = 0, ie = num_routing_grids_x * num_routing_grids_y; i < ie; ++i)
            {
                auto& hvalue = initial_horizontal_routing_map[layer * ie + i];
                hvalue = std::min(hvalue, (int)db.numRoutingTracks(PlanarDirectEnum::HORIZONTAL, layer));

                auto& vvalue = initial_vertical_routing_map[layer * ie + i];
                vvalue = std::min(vvalue, (int)db.numRoutingTracks(PlanarDirectEnum::VERTICAL, layer));
            }
        }
        for (auto item : initial_horizontal_routing_map)
        {
            initial_horizontal_demand_map.append(item); 
        }
        for (auto item : initial_vertical_routing_map)
        {
            initial_vertical_demand_map.append(item); 
        }
    }
}

DREAMPLACE_END_NAMESPACE
