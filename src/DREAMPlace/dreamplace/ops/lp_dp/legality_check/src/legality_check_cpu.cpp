/**
 * @file   legality_check_cpu.cpp
 * @author Fly
 * @date   Nov 2020
 */
#include "lp_dp/legality_check/src/legality_check.h"
#include "lp_dp/legality_check/src/legality_check_cpu.h"

DREAMPLACE_BEGIN_NAMESPACE

template <typename T>
bool legalityCheckCPU(
     const T* x, const T* y, 
     const T* node_size_x, const T* node_size_y, 
     const T* flat_region_boxes, const int* flat_region_boxes_start, const int* node2fence_region_map, 
     T xl, T yl, T xh, T yh,
     T site_width, T row_height, 
     const int num_nodes, ///< movable and fixed cells 
     const int num_movable_nodes, 
     const int num_regions
     )
{
  bool legal_flag = true; 
  int num_rows = ceil((yh-yl)/row_height);
  dreamplaceAssert(num_rows > 0); 
  fflush(stdout);
  std::vector<std::vector<int> > row_nodes (num_rows);

  // check node within boundary 
  if (!boundaryCheck(x, y, node_size_x, node_size_y, xl, yl, xh, yh, num_movable_nodes)) {
    legal_flag = false; 
  }

  // check row and site alignment 
  if (!siteAlignmentCheck(
      x, y, site_width, row_height, 
      xl, yl, num_movable_nodes)) {
    legal_flag = false; 
  }

  if (!overlapCheck(
      node_size_x, node_size_y, 
      x, y, row_height, xl, yl, xh, yh, 
      num_nodes, num_movable_nodes)) {
    legal_flag = false; 
  }

  // check fence regions 
  if (!fenceRegionCheck(
      node_size_x, node_size_y, 
      flat_region_boxes, flat_region_boxes_start, node2fence_region_map, 
      x, y, num_movable_nodes, num_regions)) {
    legal_flag = false; 
  }
  return legal_flag;
}

/// @brief check legality 
int legalityCheckRun(CommonPlaceDB* db)
{
  // Call the cuda kernel launcher
  bool legal_flag = legalityCheckCPU<int>(
                    db->getCurX(), db->getCurY(),
                    db->getNodeSizeX(), db->getNodeSizeY(),
                    db->getFlatFenceBoxes(), db->getFlatFenceBoxesStart(), db->getNode2FenceMap(),
                    db->getAreaLLX(), db->getAreaLLY(), db->getAreaURX(), db->getAreaURY(),
                    db->getSiteWidth(), db->getRowHight(),
                    db->getNumNodes(),   //< movable and fixed cells 
                    db->getNumMoveableNodes(), db->getNumFences()
                    );

  return legal_flag; 
}

DREAMPLACE_END_NAMESPACE

