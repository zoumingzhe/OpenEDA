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

// class commonDB member functions
void
CommonDB::__init()
{
  // from edi db
  return;
}
void
CommonDB::__free()
{
  // free edi db
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
        initDetailedPlaceDBGPU(db);
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
