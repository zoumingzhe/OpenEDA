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
//#include <string>
#include "utility/src/Msg.h"
#include "utility/src/DetailedPlaceDB.h"
#include "utility/src/LegalizationDB.h"
#include "db.h"

DREAMPLACE_BEGIN_NAMESPACE

typedef PlInt Coord;
struct plLoc
{
  Coord x;
  Coord y;
};

typedef std::vector<Coord> LocV;
typedef std::vector<PlInt> IntV;
typedef std::vector<unsigned char> CharV;
typedef std::vector<double> DoubleV;
typedef std::vector<String> StringV;
typedef std::vector<PlBox> BoxV;
typedef std::vector<PlOrient> OrientV;
typedef std::vector<PlDirection> DirectionV;
typedef std::vector<PlInst*> InstV;
// GP, LP and DP flow steps
const int kNone           = 0;
const int kMacroLegalize  = 1;
const int kGreedLegalize  = 1 << 1;
const int kAbacusLegalize = 1 << 2;
const int kKReorder1      = 1 << 3;
const int kIndependentSM  = 1 << 4;
const int kGlobalSwap     = 1 << 5;
const int kKReorder2      = 1 << 6;
const int kLegalityCheck  = 1 << 7;
const int kGlobalplace    = 1 << 8;

/// @brief a wrapper class of required data for user input paramaters
class Para 
{
  public:
    Para()
    {}
    Para(PlBox areaBox, int bins_x, int bins_y) : box_(areaBox),
    num_bins_x_(bins_x), num_bins_y_(bins_y)
    {}
    Para(int bins_x, int bins_y, int flows, bool saveDB, bool gpu) : 
    num_bins_x_(bins_x), num_bins_y_(bins_y), flow_steps_(flows), 
    save_db_(saveDB), gpu_(gpu)
    {}
    Para(PlBox areaBox, int bins_x, int bins_y, int flows, bool saveDB) : 
    box_(areaBox), num_bins_x_(bins_x), num_bins_y_(bins_y), 
    flow_steps_(flows), save_db_(saveDB) 
    {}
    Para(int bins_x, int bins_y, int flows, bool saveDB, bool gpu, std::string file) :
    num_bins_x_(bins_x), num_bins_y_(bins_y), flow_steps_(flows),
    save_db_(saveDB), gpu_(gpu), json_file_(file)
    {}
    ~Para() = default;
    PlBox  getBox() 		const { return box_; }
    int    getNumBinsX()        const { return num_bins_y_; }
    int    getNumBinsY()        const { return num_bins_y_; }
    int    getFlowSteps()       const { return flow_steps_; }
    bool   isDBSaved()          const { return save_db_;}
    bool   isGPU()              const { return gpu_;        }
    int    getNumThreads()      const { return num_threads_;}
    std::string getJsonFile()   const { return json_file_;  }
  private:
    PlBox    box_;
    int    num_bins_x_     = 1;
    int    num_bins_y_     = 1;
    int    flow_steps_     = 0xFF;   // default is to run LP and DP full flow
    int    num_threads_    = 8;
    bool   save_db_        = false;  // option to save intermediate DBs
    bool   gpu_            = true;
    std::string json_file_ = nullptr;
};

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
    // save as vector
    const LocV&    getInitX()                 const { return init_x_;                  }
    const LocV&    getInitY()                 const { return init_y_;                  }
    const LocV&    getNodeSizeX()             const { return node_size_x_;             }
    const LocV&    getNodeSizeY()             const { return node_size_y_;             }
    const LocV&    getFlatRegionBoxes()       const { return flat_region_boxes_;       }
    const IntV&    getFlatRegionBoxesStart()  const { return flat_region_boxes_start_; }
    const IntV&    getNode2RegionMap()        const { return node2region_map_;         }
    const LocV&    getFlatFenceBoxes()        const { return flat_fence_boxes_;        }
    const IntV&    getFlatFenceBoxesStart()   const { return flat_fence_boxes_start_;  }
    const IntV&    getNode2FenceMap()         const { return node2fence_map_;          }
    const IntV&    getFlatNet2PinMap()        const { return flat_net2pin_map_;        } 
    const IntV&    getFlatNet2PinStartMap()   const { return flat_net2pin_start_map_;  } 
    const IntV&    getPin2NetMap()            const { return pin2net_map_;             } 
    const IntV&    getFlatNode2PinMap()       const { return flat_node2pin_map_;       } 
    const IntV&    getFlatNode2PinStartMap()  const { return flat_node2pin_start_map_; } 
    const IntV&    getFlatPin2NodeMap()       const { return pin2node_map_;            } 
    const LocV&    getPinOffsetX()            const { return pin_offset_x_;            } 
    const LocV&    getPinOffsetY()            const { return pin_offset_y_;            } 
    const CharV&   getNetMask()               const { return net_mask_;                } 

    const StringV&    getInstNames()          const { return inst_names_;              }
    const OrientV&    getInstOrients()        const { return inst_orients_;            }
    const DirectionV& getPinDirections()      const { return pin_dirs_;                }
    const StringV&    getNetNames()           const { return net_names_;               }
    const DoubleV&    getNetWeights()         const { return net_weights_;             }
    const BoxV&       getRowBoxes()           const { return row_boxes_;               }

    int            getNumNodes()              const { return num_nodes_;               }
    int            getNumNets()               const { return num_nets_;                }
    int            getNumPins()               const { return num_pins_;                }
    int            getNumRegions()            const { return num_regions_;             }
    int            getNumFences()             const { return num_fences_;              }
    int            getNumMoveableNodes()      const { return num_movable_nodes_;       }
    PlInst*        getInstById(int i)         const { return idx_to_insts_.at(i);      }
    PlInt          getNumIOPins()             const { return num_io_pins_;             }
    int            getNumMoveablePins()       const { return num_movable_pins_;        }

    void           setInitX(int i, Coord x)         { init_x_[i] = x;                  }
    void           setInitY(int i, Coord y)         { init_y_[i] = y;                  }
  private:
    bool __isDBLoaded();
    void __init();
    void __free();
  private:
    // for both LP and DP
    LocV           init_x_;                   // init loc X of nodes
    LocV           init_y_;                   // init loc Y of nodes
    LocV           node_size_x_;              // size X of nodes
    LocV           node_size_y_;              // size Y of nodes
    LocV           flat_region_boxes_;        // < number of region boxes x 4
    IntV           flat_region_boxes_start_;  // < number of regions + 1 
    IntV           node2region_map_;          // < length of number of movable cells 
    LocV           flat_fence_boxes_;         // < number of fence boxes x 4
    IntV           flat_fence_boxes_start_;   // < number of fences + 1 
    IntV           node2fence_map_;           // < length of number of movable cells 
    // For DP onl  y
    IntV           flat_net2pin_map_       ; 
    IntV           flat_net2pin_start_map_ ; 
    IntV           pin2net_map_            ; 
    IntV           flat_node2pin_map_      ; 
    IntV           flat_node2pin_start_map_; 
    IntV           pin2node_map_           ; 
    LocV           pin_offset_x_           ; 
    LocV           pin_offset_y_           ; 
    CharV          net_mask_               ; 
    bool           isCommonDBReady_        ; 
    // for both LP and DP
    int            num_nodes_               = 0;        // number of nodes
    int            num_nets_                = 0;        // number of nets
    int            num_pins_                = 0;        // number of pins
    int            num_io_pins_             = 0;        // number of io pins
    int            num_regions_             = 0;        // number of regions for region_boxes and region_boxes_start
    int            num_fences_              = 0;        // number of fences for fence_boxes and fence_boxes_start
    int            num_movable_nodes_       = 0;        // num of movebale nodes 
    int            num_movable_pins_        = 0;        // num of movebale pins 
    InstV          idx_to_insts_;                // get db inst by place id
    StringV        inst_names_;                  // save inst name
    OrientV        inst_orients_;                // save inst orient
    DirectionV     pin_dirs_;                    // save pin direction
    StringV        net_names_;                   // save net name
    DoubleV        net_weights_;                 // save net weight
    BoxV           row_boxes_;                   // save row box

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
    PlBox box_;
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
    ~CommonPlaceDB()
    {
      __freeInternalDB();  
    } 
    CommonPlaceDB(const CommonPlaceDB&)            = delete; // no copy
    CommonPlaceDB &operator=(const CommonPlaceDB&) = delete; // no copy
    void __buildInternalDB();
    void __freeInternalDB();
  public:
    static CommonPlaceDB* getPlaceDBInstance()
    {
      if (place_db_instance_ == nullptr) {
        dreamplacePrint(kWARN, "Not find place common DB.\n");
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
    const PlBox    getParaBox()               const { return getPara().getBox();             }
    const int      getParaNumBinsX()          const { return getPara().getNumBinsX();        }
    const int      getParaNumBinsY()          const { return getPara().getNumBinsY();        }
    const bool     isGPU()                    const { return getPara().isGPU();              }
    const int      numThreads()               const { return getPara().getNumThreads();      }
    const int      getParaFlowSteps()         const { return getPara().getFlowSteps();       }
    // common DB interface : get
    bool                 isCommonDBReady()          const { return db_.isCommonDBReady();          }
    const LocV&          getInitXV()                const { return db_.getInitX();                 }
    const LocV&          getInitYV()                const { return db_.getInitX();                 }
    const LocV&          getNodeSizeXV()            const { return db_.getNodeSizeX();             }
    const LocV&          getNodeSizeYV()            const { return db_.getNodeSizeY();             }
    const LocV&          getFlatRegionBoxesV()      const { return db_.getFlatRegionBoxes();       }
    const IntV&          getFlatRegionBoxesStartV() const { return db_.getFlatRegionBoxesStart();  }
    const IntV&          getNode2RegionMapV()       const { return db_.getNode2RegionMap();        }
    const LocV&          getFlatFenceBoxesV()       const { return db_.getFlatFenceBoxes();        }
    const IntV&          getFlatFenceBoxesStartV()  const { return db_.getFlatFenceBoxesStart();   }
    const IntV&          getNode2FenceMapV()        const { return db_.getNode2FenceMap();         }
    const IntV&          getFlatNet2PinMapV()       const { return db_.getFlatNet2PinMap();        } 
    const IntV&          getFlatNet2PinStartMapV()  const { return db_.getFlatNet2PinStartMap();   } 
    const IntV&          getPin2NetMapV()           const { return db_.getPin2NetMap();            } 
    const IntV&          getFlatNode2PinMapV()      const { return db_.getFlatNode2PinMap();       } 
    const IntV&          getFlatNode2PinStartMapV() const { return db_.getFlatNode2PinStartMap();  } 
    const IntV&          getFlatPin2NodeMapV()      const { return db_.getFlatPin2NodeMap();       } 
    const LocV&          getPinOffsetXV()           const { return db_.getPinOffsetX();            } 
    const LocV&          getPinOffsetYV()           const { return db_.getPinOffsetY();            } 
    const CharV&         getNetMaskV()              const { return db_.getNetMask();               } 
    const StringV&       getInstNames()             const { return db_.getInstNames();             }
    const OrientV&       getInstOrients()           const { return db_.getInstOrients();           }
    const DirectionV&    getPinDirections()         const { return db_.getPinDirections();         }
    const StringV&       getNetNames()              const { return db_.getNetNames();              }
    const DoubleV&       getNetWeights()            const { return db_.getNetWeights();            }
    const BoxV&          getRowBoxes()              const { return db_.getRowBoxes();              }
    const String         getInstNameById(PlInt id)  const { return db_.getInstNames().at(id);      }
    const String         getNetNameById(PlInt id)   const { return db_.getNetNames().at(id);       }
    // common DB interface : get ptr array
    const Coord*         getInitX()                 const { return db_.getInitX().data();                 }
    const Coord*         getInitY()                 const { return db_.getInitX().data();                 }
    const Coord*         getNodeSizeX()             const { return db_.getNodeSizeX().data();             }
    const Coord*         getNodeSizeY()             const { return db_.getNodeSizeY().data();             }
    const Coord*         getFlatRegionBoxes()       const { return db_.getFlatRegionBoxes().data();       }
    const int*           getFlatRegionBoxesStart()  const { return db_.getFlatRegionBoxesStart().data();  }
    const int*           getNode2RegionMap()        const { return db_.getNode2RegionMap().data();        }
    const Coord*         getFlatFenceBoxes()        const { return db_.getFlatFenceBoxes().data();        }
    const int*           getFlatFenceBoxesStart()   const { return db_.getFlatFenceBoxesStart().data();   }
    const int*           getNode2FenceMap()         const { return db_.getNode2FenceMap().data();         }
    const int*           getFlatNet2PinMap()        const { return db_.getFlatNet2PinMap().data();        } 
    const int*           getFlatNet2PinStartMap()   const { return db_.getFlatNet2PinStartMap().data();   } 
    const int*           getPin2NetMap()            const { return db_.getPin2NetMap().data();            } 
    const int*           getFlatNode2PinMap()       const { return db_.getFlatNode2PinMap().data();       } 
    const int*           getFlatNode2PinStartMap()  const { return db_.getFlatNode2PinStartMap().data();  } 
    const int*           getFlatPin2NodeMap()       const { return db_.getFlatPin2NodeMap().data();       } 
    const Coord*         getPinOffsetX()            const { return db_.getPinOffsetX().data();            } 
    const Coord*         getPinOffsetY()            const { return db_.getPinOffsetY().data();            } 
    const unsigned char* getNetMask()               const { return db_.getNetMask().data();               } 

    int            getNumNodes()              const { return db_.getNumNodes();              }
    int            getNumNets()               const { return db_.getNumNets();               }
    int            getNumPins()               const { return db_.getNumPins();               }
    int            getNumRegions()            const { return db_.getNumRegions();            }
    int            getNumFences()             const { return db_.getNumFences();             }
    int            getNumMoveableNodes()      const { return db_.getNumMoveableNodes();      }
    PlInt          getNumIOPins()             const { return db_.getNumIOPins();             }
    PlInst*        getInstById(int i)         const { return db_.getInstById(i);             }
    int            getNumMoveablePins()       const { return db_.getNumPins();               }

    // internal DB interface: get
    double       getNodeWeight(int id)            const { return node_weights_.at(id); }
    PlBox        getArea()                        const { return box_;                 }
    Coord        getAreaLLX()                     const { return box_.getLLX();        }
    Coord        getAreaLLY()                     const { return box_.getLLY();        }
    Coord        getAreaURX()                     const { return box_.getURX();        }
    Coord        getAreaURY()                     const { return box_.getURY();        }
    const LocV&  getCurXV()                       const { return cur_x_;               }
    const LocV&  getCurYV()                       const { return cur_y_;               }
    Coord*       getCurX()                              { return cur_x_.data();        }
    Coord*       getCurY()                              { return cur_y_.data();        }
    Coord        getSiteWidth()                   const { return site_width_;          }
    Coord        getRowHight()                    const { return row_height_;          }
    Coord        getBinSizeX()                    const { return bin_size_x_;          }
    Coord        getBinSizeY()                    const { return bin_size_y_;          }
    int          getNumBinsX()                    const { return num_bins_x_;          }
    int          getNumBinsY()                    const { return num_bins_y_;          }
    int          getNumSitesX()                   const { return num_sites_x_;         }
    int          getNumSitesY()                   const { return num_sites_y_;         }

    // internal DB interface: set
    void     setNodeWeight(const int id, const double w) { node_weights_[id] = w;             }
    void     setArea(PlBox box)                          { box_ = box;                        }
    void     setArea(PlInt lx, PlInt ly, PlInt ux, PlInt uy)  { setBox(box_, lx, ly, ux, uy); }
    void     setAreaLLX(const Coord llx)                 { box_.setLLX(llx);                  }
    void     setAreaLLY(const Coord lly)                 { box_.setLLY(lly);                  }
    void     setAreaURX(const Coord urx)                 { box_.setURX(urx);                  }
    void     setAreaURY(const Coord ury)                 { box_.setURY(ury);                  }
    void     setSiteWidth(const Coord site_w)            { site_width_ = site_w;              }
    void     setRowHight(const Coord row_h)              { row_height_ = row_h;               }
    void     setBinSizeX(const Coord bin_size_x)         { bin_size_x_ = bin_size_x;          }
    void     setBinSizeY(const Coord bin_size_y)         { bin_size_y_ = bin_size_y;          }
    void     setNumBinsX(const int num_bin_x)            { num_bins_x_ = num_bin_x;           }
    void     setNumBinsY(const int num_bin_y)            { num_bins_y_ = num_bin_y;           }
    void     setNumSitesX(const int num_sites_x)         { num_sites_x_ = num_sites_x;        }
    void     setNumSitesY(const int num_sites_y)         { num_sites_y_ = num_sites_y;        }

__attribute__ ((visibility("default")))
    void     updateXY(const int* x, const int* y);
    //  detailedDB interface
    void     initDetailedPlaceDB(DetailedPlaceDB<int>& db);
    void     freeDetailedPlaceDB(DetailedPlaceDB<int>& db);

    void     updateDB2EDI();
    void     summaryMovement();
    void     initLegalizationDBGPU(LegalizationDB<int>& db);
    void     initLegalizationDBCPU(LegalizationDB<int>& db);
    void     initLegalizationDB(LegalizationDB<int>& db);
#ifdef _CUDA_FOUND
    void     updateXYGPU(const int* x, const int* y);
  private:
    void     initDetailedPlaceDBGPU(DetailedPlaceDB<int>& db);
    void     freeDetailedPlaceDBGPU(DetailedPlaceDB<int>& db);
#endif

  private:
    Para                para_;                             // user setting
    CommonDB            db_;                               // edi db
    DoubleV             node_weights_;                     // weight of nodes, for LP only
    PlBox               box_;                              // box of die area xl, yl, xh, yh
    LocV                cur_x_;                            // init loc X of nodes
    LocV                cur_y_;                            // init loc Y of nodes
    // build i,j grid in the future
    Coord               site_width_         = 0;           // site width
    Coord               row_height_         = 0;           // row height  
    Coord               bin_size_x_         = 0;           // bin size x
    Coord               bin_size_y_         = 0;           // bin size y
    int                 num_bins_x_         = 1;           // num of bins in x
    int                 num_bins_y_         = 1;           // num of bins in y
    int                 num_sites_x_        = 0;           // num of sites in x
    int                 num_sites_y_        = 0;           // num of sites in y

    static CommonPlaceDB* place_db_instance_;            // static object
};

DREAMPLACE_END_NAMESPACE

#endif
