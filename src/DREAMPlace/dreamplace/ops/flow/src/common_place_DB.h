/*************************************************************************
  > File Name: legal_detail_place_DB.h
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/

#ifndef _DREAMPLACE_OEM_COMMONPLACEDB_H
#define _DREAMPLACE_OEM_COMMONPLACEDB_H

#include <vector>
#include <limits>
#include "db.h"
#include "utility/src/Msg.h"
#include "utility/src/DetailedPlaceDB.h"

DREAMPLACE_BEGIN_NAMESPACE

typedef PlInt Coord;
struct plLoc
{
  Coord x;
  Coord y;
};

// LP and DP flow steps
const int kNone           = 0;
const int kMacroLegalize  = 1;
const int kGreedLegalize  = 1 << 1;
const int kAbacusLegalize = 1 << 2;
const int kKReorder1      = 1 << 3;
const int kIndependentSM  = 1 << 4;
const int kGlobalSwap     = 1 << 5;
const int kKReorder2      = 1 << 6;
const int kLegalityCheck  = 1 << 7;

/// @brief a wrapper class of box
class CBox
{
  public:
    CBox() 
    {
      invalidate();
    }
    CBox(Coord llx, Coord lly, Coord urx, Coord ury) 
    {
      ll.x = llx;
      ll.y = lly;
      ur.x = urx;
      ur.y = ury;
    }
    ~CBox() = default;

    // invalidate the box
    void invalidate()
    {
      ll.x = std::numeric_limits<Coord>::max(); 
      ll.y = std::numeric_limits<Coord>::max(); 
      ur.x = std::numeric_limits<Coord>::lowest();
      ur.y = std::numeric_limits<Coord>::lowest();
    }
    // is box invalid
    bool invalid() 
    {
      return (ll.x >= ur.x || ll.y >= ur.y ) ? true : false;
    }

  public:
    plLoc ll;
    plLoc ur;
};

/// @brief a wrapper class of required data for user input paramaters
class Para 
{
  public:
    Para()
    {}
    Para(CBox areaBox, int bins_x, int bins_y) : box_(areaBox),
    num_bins_x_(bins_x), num_bins_y_(bins_y)
    {}
    Para(int bins_x, int bins_y, int flows, bool saveDB, bool gpu) : 
    num_bins_x_(bins_x), num_bins_y_(bins_y), flow_steps_(flows), 
    save_db_(saveDB), gpu_(gpu)
    {}
    Para(CBox areaBox, int bins_x, int bins_y, int flows, bool saveDB) : 
    box_(areaBox), num_bins_x_(bins_x), num_bins_y_(bins_y), 
    flow_steps_(flows), save_db_(saveDB) 
    {}
    ~Para() = default;
    CBox    getBox() 		const { return box_; }
    int    getNumBinsX()        const { return num_bins_y_; }
    int    getNumBinsY()        const { return num_bins_y_; }
    int    getFlowSteps()       const { return flow_steps_; }
    bool   isDBSaved()          const { return save_db_;}
    bool   isGPU()              const { return gpu_;        }
    int    getNumThreads()      const { return num_threads_;}
  private:
    CBox    box_;
    int    num_bins_x_     = 1;
    int    num_bins_y_     = 1;
    int    flow_steps_     = 0xFF;   // default is to run LP and DP full flow
    int    num_threads_    = 8;
    bool   save_db_        = false;  // option to save intermediate DBs
    bool   gpu_            = true;
};

typedef std::vector<plLoc> LocVec;
/// @brief a wrapper class of required data for edi db
class CommonDB 
{
  public:
    CommonDB()
    {
      __init();
    }
    CommonDB(const CommonDB&)            = delete; // no copy
    CommonDB &operator=(const CommonDB&) = delete; // no copy
    ~CommonDB()
    {
      __free();
    }
    bool           isCommonDBReady()          const { return isCommonDBReady_;         }
    Coord*         getInitX()                 const { return init_x_;                  }
    Coord*         getInitY()                 const { return init_y_;                  }
    Coord*         getNodeSizeX()             const { return node_size_x_;             }
    Coord*         getNodeSizeY()             const { return node_size_y_;             }
    Coord*         getFlatRegionBoxes()       const { return flat_region_boxes_;       }
    int*           getFlatRegionBoxesStart()  const { return flat_region_boxes_start_; }
    int*           getNode2RegionMap()        const { return node2region_map_;         }
    Coord*         getFlatFenceBoxes()        const { return flat_fence_boxes_;        }
    int*           getFlatFenceBoxesStart()   const { return flat_fence_boxes_start_;  }
    int*           getNode2FenceMap()         const { return node2fence_map_;          }
    int*           getFlatNet2PinMap()        const { return flat_net2pin_map_;        } 
    int*           getFlatNet2PinStartMap()   const { return flat_net2pin_start_map_;  } 
    int*           getPin2NetMap()            const { return pin2net_map_;             } 
    int*           getFlatNode2PinMap()       const { return flat_node2pin_map_;       } 
    int*           getFlatNode2PinStartMap()  const { return flat_node2pin_start_map_; } 
    int*           getFlatPin2NodeMap()       const { return pin2node_map_;            } 
    Coord*         getPinOffsetX()            const { return pin_offset_x_;            } 
    Coord*         getPinOffsetY()            const { return pin_offset_y_;            } 
    unsigned char* getNetMask()               const { return net_mask_;                } 
    int            getNumNodes()              const { return num_nodes_;               }
    int            getNumNets()               const { return num_nets_;                }
    int            getNumPins()               const { return num_pins_;                }
    int            getNumRegions()            const { return num_regions_;             }
    int            getNumFences()             const { return num_fences_;              }
    int            getNumMoveableNodes()      const { return num_movable_nodes_;       }
  private:
    bool __isDBLoaded();
    void __init();
    void __free();
  private:
    // for both LP and DP
    Coord*         init_x_                  = nullptr;  // init loc X of nodes
    Coord*         init_y_                  = nullptr;  // init loc Y of nodes
    Coord*         node_size_x_             = nullptr;  // size X of nodes
    Coord*         node_size_y_             = nullptr;  // size Y of nodes
    Coord*         flat_region_boxes_       = nullptr;  // < number of region boxes x 4
    int*           flat_region_boxes_start_ = nullptr;  // < number of regions + 1 
    int*           node2region_map_         = nullptr;  // < length of number of movable cells 
    Coord*         flat_fence_boxes_        = nullptr;  // < number of fence boxes x 4
    int*           flat_fence_boxes_start_  = nullptr;  // < number of fences + 1 
    int*           node2fence_map_          = nullptr;  // < length of number of movable cells 
    // For DP only
    int*           flat_net2pin_map_        = nullptr; 
    int*           flat_net2pin_start_map_  = nullptr; 
    int*           pin2net_map_             = nullptr; 
    int*           flat_node2pin_map_       = nullptr; 
    int*           flat_node2pin_start_map_ = nullptr; 
    int*           pin2node_map_            = nullptr; 
    Coord*         pin_offset_x_            = nullptr; 
    Coord*         pin_offset_y_            = nullptr; 
    unsigned char* net_mask_                = nullptr; 
    bool           isCommonDBReady_         = false; 
    // for both LP and DP
    int            num_nodes_               = 0;        // number of nodes
    int            num_nets_                = 0;        // number of nets
    int            num_pins_                = 0;        // number of pins
    int            num_regions_             = 0;        // number of regions for region_boxes and region_boxes_start
    int            num_fences_              = 0;        // number of fences for fence_boxes and fence_boxes_start
    int            num_movable_nodes_       = 0;        // num of movebale nodes 
    std::vector<PlObjId> idx_to_instId_;                // get db inst id by place id

};

/// @brief a wrapper class of required data for i,j grid 
class Grid
{
  public:
    Grid() 
    {
      __buildGrid();
    }
    ~Grid()                      = default;  // default 
    Grid(const Grid&)            = delete;   // no copy
    Grid &operator=(const Grid&) = delete;   // no copy

    // APIs for i,j grid
    int       getNumSiteCol()     const { return num_site_col_; }
    int       getNumSiteRow()     const { return num_siet_row_; }

  private:
    void __buildGrid();
  private:
    CBox box_;
    Coord site_width_         = 0;                  // site width
    Coord row_height_         = 0;                  // row height  
    int num_site_col_         = 0;                  // num of sites in x
    int num_siet_row_         = 0;                  // num of row in y
};

/// @brief a wrapper class of required data for LP-DP 
class CommonPlaceDB
{
  private:
    CommonPlaceDB()
    {
      __buildInternalDB();
    }
    CommonPlaceDB(Para& para) : para_(para)
    {
      __buildInternalDB();
    }
    ~CommonPlaceDB()                                 = default;
    CommonPlaceDB(const CommonPlaceDB&)            = delete; // no copy
    CommonPlaceDB &operator=(const CommonPlaceDB&) = delete; // no copy
    void __buildInternalDB();
  public:
    static CommonPlaceDB* getPlaceDBInstance()
    {
      if (place_db_instance_ == nullptr) {
        place_db_instance_ = new CommonPlaceDB();
      }
      return place_db_instance_;
    }
    static CommonPlaceDB* getPlaceDBInstance(Para& para)
    {
      if (place_db_instance_ == nullptr) {
        place_db_instance_ = new CommonPlaceDB(para);
      }
      return place_db_instance_;
    }
    static void freePlaceDBInstance() 
    {
      if (place_db_instance_) {
        delete place_db_instance_;
        place_db_instance_ = nullptr;
      }
    }
    // user setting interface
    const Para&    getPara()                  const { return para_;                          }
    // common DB interface : get
    bool           isCommonDBReady()          const { return db_.isCommonDBReady();          }
    Coord*         getInitX()                 const { return db_.getInitX();                 }
    Coord*         getInitY()                 const { return db_.getInitX();                 }
    Coord*         getNodeSizeX()             const { return db_.getNodeSizeX();             }
    Coord*         getNodeSizeY()             const { return db_.getNodeSizeY();             }
    Coord*         getFlatRegionBoxes()       const { return db_.getFlatRegionBoxes();       }
    int*           getFlatRegionBoxesStart()  const { return db_.getFlatRegionBoxesStart();  }
    int*           getNode2RegionMap()        const { return db_.getNode2RegionMap(); }
    Coord*         getFlatFenceBoxes()        const { return db_.getFlatFenceBoxes();        }
    int*           getFlatFenceBoxesStart()   const { return db_.getFlatFenceBoxesStart();   }
    int*           getNode2FenceMap()         const { return db_.getNode2FenceMap();  }
    int*           getFlatNet2PinMap()        const { return db_.getFlatNet2PinMap();        } 
    int*           getFlatNet2PinStartMap()   const { return db_.getFlatNet2PinStartMap();   } 
    int*           getPin2NetMap()            const { return db_.getPin2NetMap();            } 
    int*           getFlatNode2PinMap()       const { return db_.getFlatNode2PinMap();       } 
    int*           getFlatNode2PinStartMap()  const { return db_.getFlatNode2PinStartMap();  } 
    int*           getFlatPin2NodeMap()       const { return db_.getFlatPin2NodeMap();       } 
    Coord*         getPinOffsetX()            const { return db_.getPinOffsetX();            } 
    Coord*         getPinOffsetY()            const { return db_.getPinOffsetY();            } 
    unsigned char* getNetMask()               const { return db_.getNetMask();               } 
    int            getNumNodes()              const { return db_.getNumNodes();              }
    int            getNumNets()               const { return db_.getNumNets();               }
    int            getNumPins()               const { return db_.getNumPins();               }
    int            getNumRegions()            const { return db_.getNumRegions();            }
    int            getNumFences()             const { return db_.getNumFences();             }
    int            getNumMoveableNodes()      const { return db_.getNumMoveableNodes();      }

    // internal DB interface: get
    double   getNodeWeight(int id)            const { return node_weights_.at(id); }
    Coord    getNodeCurX(int id)              const { return cur_loc_.at(id).x;    } 
    Coord    getNodeCurY(int id)              const { return cur_loc_.at(id).y;    }
    Coord    getAreaLLX()                     const { return box_.ll.x;            }
    Coord    getAreaLLY()                     const { return box_.ll.y;            }
    Coord    getAreaURX()                     const { return box_.ur.x;            }
    Coord    getAreaURY()                     const { return box_.ur.x;            }
    Coord*   getCurX()                        const { return cur_x_;               }
    Coord*   getCurY()                        const { return cur_y_;               }
    Coord    getSiteWidth()                   const { return site_width_;          }
    Coord    getRowHight()                    const { return row_height_;          }
    Coord    getBinSizeX()                    const { return bin_size_x_;          }
    Coord    getBinSizeY()                    const { return bin_size_y_;          }
    int      getNumBinsX()                    const { return num_bins_x_;          }
    int      getNumBinsY()                    const { return num_bins_y_;          }
    int      getNumSitesX()                   const { return num_sites_x_;         }
    int      getNumSitesY()                   const { return num_sites_y_;         }
  
    // internal DB interface: set
    void     setNodeWeight(const int id, const double w) { node_weights_[id] = w;             }
    void     setNodeCurX(const int id, const Coord x)    { cur_loc_[id].x = x;                } 
    void     setNodeCurY(const int id, const Coord y)    { cur_loc_[id].y = y;                }
    void     setAreaLLX(const Coord llx)                 { box_.ll.x = llx;                   }
    void     setAreaLLY(const Coord lly)                 { box_.ll.y = lly;                   }
    void     setAreaURX(const Coord urx)                 { box_.ur.x = urx;                   }
    void     setAreaURY(const Coord ury)                 { box_.ur.y = ury;                   }
    void     setSiteWidth(const Coord site_w)            { site_width_ = site_w;              }
    void     setRowHight(const Coord row_h)              { row_height_ = row_h;               }
    void     setBinSizeX(const Coord bin_size_x)         { bin_size_x_ = bin_size_x;          }
    void     setBinSizeY(const Coord bin_size_y)         { bin_size_y_ = bin_size_y;          }
    void     setNumBinsX(const int num_bin_x)            { num_bins_x_ = num_bin_x;           }
    void     setNumBinsY(const int num_bin_y)            { num_bins_y_ = num_bin_y;           }
    void     setNumSitesX(const int num_sites_x)         { num_sites_x_ = num_sites_x;        }
    void     setNumSitesY(const int num_sites_y)         { num_sites_y_ = num_sites_y;        }

    //  detailedDB interface
    void     initDetailedPlaceDB(DetailedPlaceDB<int>& db, bool gpu);
    void     freeDetailedPlaceDB(DetailedPlaceDB<int>& db, bool gpu);

#ifdef _CUDA_FOUND
  private:
    void     initDetailedPlaceDBGPU(DetailedPlaceDB<int>& db);
    void     freeDetailedPlaceDBGPU(DetailedPlaceDB<int>& db);
#endif

  private:
    Para                para_;                             // user setting
    CommonDB            db_;                               // edi db
    LocVec              cur_loc_;                          // node current location X, Y
    std::vector<double> node_weights_;                     // weight of nodes, for LP only
    CBox                 box_;                              // box of die area xl, yl, xh, yh
    Coord*              cur_x_              = nullptr;     // init loc X of nodes
    Coord*              cur_y_              = nullptr;     // init loc Y of nodes
    // build i,j grid in the future
    Coord               site_width_         = 0;           // site width
    Coord               row_height_         = 0;           // row height  
    Coord               bin_size_x_         = 0;           // bin size x
    Coord               bin_size_y_         = 0;           // bin size y
    int                 num_bins_x_         = 0;           // num of bins in x
    int                 num_bins_y_         = 0;           // num of bins in y
    int                 num_sites_x_        = 0;           // num of sites in x
    int                 num_sites_y_        = 0;           // num of sites in y

    static CommonPlaceDB* place_db_instance_;            // static object
};

DREAMPLACE_END_NAMESPACE

#endif
