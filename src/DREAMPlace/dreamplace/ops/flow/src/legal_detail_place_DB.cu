
/*************************************************************************
    > File Name: legal_detail_place_DB.cu
    > Author: Chenzhong Luo
    > Date: Oct 2020
 ************************************************************************/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "flow/src/common_place_DB.h"
#include "utility/src/Msg.h"
#include "utility/src/utils.cuh"

DREAMPLACE_BEGIN_NAMESPACE

void CommonPlaceDB::initDetailedPlaceDBGPU(DetailedPlaceDB<int>& db)
//void initDetailedPlaceDBGPU(DetailedPlaceDB<int>& db)
{
    if (true)
    {
       int *tmp;
       allocateCopyCUDA(tmp, getInitX(), db.num_nodes);
       db.init_x = tmp;
       allocateCopyCUDA(tmp, getInitY(), db.num_nodes);
       db.init_y = tmp;
       allocateCopyCUDA(tmp, getNodeSizeX(), db.num_nodes);
       db.node_size_x = tmp;
       allocateCopyCUDA(tmp, getNodeSizeY(), db.num_nodes);
       db.node_size_y = tmp;
       allocateCopyCUDA(tmp, getFlatFenceBoxes(), db.num_regions*4); //refer to DetailedPlaceDB.h
       db.flat_region_boxes = tmp;
       allocateCopyCUDA(tmp, getFlatFenceBoxesStart(), db.num_regions+1); 
       db.flat_region_boxes_start = tmp;
       allocateCopyCUDA(tmp, getNode2FenceMap(), db.num_nodes);  //refer to PyPlaceDB.cpp
       db.node2fence_region_map = tmp;
       allocateCUDA(db.x, db.num_nodes, int);
       allocateCUDA(db.y, db.num_nodes, int);
       allocateCopyCUDA(tmp, getFlatNet2PinMap(), db.num_pins);  //refer to maximal_independent_set_cuda2cpu.cu
       db.flat_net2pin_map = tmp;
       allocateCopyCUDA(tmp, getFlatNet2PinStartMap(), db.num_nets+1);
       db.flat_net2pin_start_map = tmp;
       allocateCopyCUDA(tmp, getPin2NetMap(), db.num_pins);
       db.pin2net_map = tmp;
       allocateCopyCUDA(tmp, getFlatNode2PinMap(), db.num_pins);
       db.flat_node2pin_map = tmp;
       allocateCopyCUDA(tmp, getFlatNode2PinStartMap(), db.num_nodes+1);
       db.flat_node2pin_start_map = tmp;
       allocateCopyCUDA(tmp, getFlatPin2NodeMap(), db.num_pins);
       db.pin2node_map = tmp;
       allocateCopyCUDA(tmp, getPinOffsetX(), db.num_pins);
       db.pin_offset_x = tmp;
       allocateCopyCUDA(tmp, getPinOffsetY(), db.num_pins);
       db.pin_offset_y = tmp;
       unsigned char *tmpChar;
       allocateCopyCUDA(tmpChar, getNetMask(), db.num_nets);
       db.net_mask = tmpChar;
    }
}

void CommonPlaceDB::freeDetailedPlaceDBGPU(DetailedPlaceDB<int>& db)
//void freeDetailedPlaceDBGPU(DetailedPlaceDB<int>& db)
{
    if (true)
    {
       destroyCUDA(const_cast<int *>(db.init_x));
       destroyCUDA(const_cast<int *>(db.init_y));
       destroyCUDA(const_cast<int *>(db.node_size_x));
       destroyCUDA(const_cast<int *>(db.node_size_y));
       destroyCUDA(const_cast<int *>(db.flat_region_boxes));
       destroyCUDA(const_cast<int *>(db.flat_region_boxes_start));
       destroyCUDA(const_cast<int *>(db.node2fence_region_map));
       destroyCUDA(db.x);
       destroyCUDA(db.y);
       destroyCUDA(const_cast<int *>(db.flat_net2pin_map));
       destroyCUDA(const_cast<int *>(db.flat_net2pin_start_map));
       destroyCUDA(const_cast<int *>(db.pin2net_map));
       destroyCUDA(const_cast<int *>(db.flat_node2pin_map));
       destroyCUDA(const_cast<int *>(db.flat_node2pin_start_map));
       destroyCUDA(const_cast<int *>(db.pin2node_map));
       destroyCUDA(const_cast<int *>(db.pin_offset_x));
       destroyCUDA(const_cast<int *>(db.pin_offset_y));
       destroyCUDA(const_cast<unsigned char*>(db.net_mask));
    }
}

DREAMPLACE_END_NAMESPACE
