/*************************************************************************
    > File Name: macro_legalize.h
    > Author: Fei
    > Mail:
    > Created Time: Fri 25 Sep 2020 03:20:14 PM CDT
 ************************************************************************/

#ifndef _DREAMPLACE_OEM_MACRO_LEGALIZE_H
#define _DREAMPLACE_OEM_MACRO_LEGALIZE_H

#include "utility/src/Msg.h"
#include "flow/src/common_place_DB.h"
#include <vector>
#include <limits>

DREAMPLACE_BEGIN_NAMESPACE

/// @brief a wrapper class of required data for edi db
class MacroLegal
{
  public:
    MacroLegal()
    {
      db_ = CommonPlaceDB::getPlaceDBInstance();
    }
    ~MacroLegal()                            = default;
    MacroLegal(const MacroLegal&)            = delete;  // no copy
    MacroLegal &operator=(const MacroLegal&) = delete;  // no copy

    // common DB interface : get
    bool           isCommonDBReady()          const { return db_ && db_->isCommonDBReady();   }
    Coord*         getInitX()                 const { return db_->getInitX();                 }
    Coord*         getInitY()                 const { return db_->getInitX();                 }
    Coord*         getNodeSizeX()             const { return db_->getNodeSizeX();             }
    Coord*         getNodeSizeY()             const { return db_->getNodeSizeY();             }
    Coord*         getFlatRegionBoxes()       const { return db_->getFlatRegionBoxes();       }
    int*           getFlatRegionBoxesStart()  const { return db_->getFlatRegionBoxesStart();  }
    int*           getNode2RegionMap()        const { return db_->getNode2RegionMap();        }
    Coord*         getFlatFenceBoxes()        const { return db_->getFlatFenceBoxes();        }
    int*           getFlatFenceBoxesStart()   const { return db_->getFlatFenceBoxesStart();   }
    int*           getNode2FenceMap()         const { return db_->getNode2FenceMap();         }
    int*           getFlatNet2PinMap()        const { return db_->getFlatNet2PinMap();        } 
    int*           getFlatNet2PinStartMap()   const { return db_->getFlatNet2PinStartMap();   } 
    int*           getPin2NetMap()            const { return db_->getPin2NetMap();            } 
    int*           getFlatNode2PinMap()       const { return db_->getFlatNode2PinMap();       } 
    int*           getFlatNode2PinStartMap()  const { return db_->getFlatNode2PinStartMap();  } 
    int*           getFlatPin2NodeMap()       const { return db_->getFlatPin2NodeMap();       } 
    Coord*         getPinOffsetX()            const { return db_->getPinOffsetX();            } 
    Coord*         getPinOffsetY()            const { return db_->getPinOffsetY();            } 
    unsigned char* getNetMask()               const { return db_->getNetMask();               } 
    int            getNumNodes()              const { return db_->getNumNodes();              }
    int            getNumNets()               const { return db_->getNumNets();               }
    int            getNumPins()               const { return db_->getNumPins();               }
    int            getNumRegions()            const { return db_->getNumRegions();            }
    int            getNumFences()             const { return db_->getNumFences();             }

    // internal DB interface: get
    double   getNodeWeight(int id)            const { return db_->getNodeWeight(id);     }
    Coord*   getCurX()                        const { return db_->getCurX();             } 
    Coord*   getCurY()                        const { return db_->getCurY();             }
    Coord    getAreaLLX()                     const { return db_->getAreaLLX();          }
    Coord    getAreaLLY()                     const { return db_->getAreaLLY();          }
    Coord    getAreaURX()                     const { return db_->getAreaURX();          }
    Coord    getAreaURY()                     const { return db_->getAreaURY();          }
    Coord    getSiteWidth()                   const { return db_->getSiteWidth();        }
    Coord    getRowHight()                    const { return db_->getRowHight();         }
    Coord    getBinSizeX()                    const { return db_->getBinSizeX();         }
    Coord    getBinSizeY()                    const { return db_->getBinSizeY();         }
    int      getNumBinsX()                    const { return db_->getNumBinsX();         }
    int      getNumBinsY()                    const { return db_->getNumBinsY();         }
    int      getNumSitesX()                   const { return db_-> getNumSitesX();       }
    int      getNumSitesY()                   const { return db_->getNumSitesY();        }
    int      getNumMoveableNodes()            const { return db_->getNumMoveableNodes(); }
  
    // interface 
    void run();
  private:
    CommonPlaceDB* db_ = nullptr;
};

DREAMPLACE_END_NAMESPACE

#endif
