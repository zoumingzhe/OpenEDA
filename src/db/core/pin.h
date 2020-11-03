/**
 * @file   inst_term.h
 * @date   Apr 2020
 * @brief  Terminals of instances and nets in a netlist
 */

#ifndef EDI_DB_DESIGN_INST_TERM_H_
#define EDI_DB_DESIGN_INST_TERM_H_

#include "db/core/attr_object.h"
#include "db/core/pin_attr.h"
#include "db/util/array.h"
#include "util/enums.h"
#include "util/point.h"

namespace open_edi {
namespace db {

class Term;
class Inst;
class Net;

/// @brief represents the connection point
/// in a graph. An instance has multiple pins
/// and a net has multiple pins as well.
class Pin : public Object {
  public:
    /// @brief default constructor
    Pin();

    /// @brief constructor
    Pin(Object* owner, IndexType id);

    /// @brief copy constructor
    Pin(Pin const& rhs);

    /// @brief move constructor
    Pin(Pin&& rhs) noexcept;

    /// @brief copy assignment
    Pin& operator=(Pin const& rhs);

    /// @brief move assignment
    Pin& operator=(Pin&& rhs) noexcept;

    /// @brief getter for macro pin
    Term* getTerm() const;

    /// @brief setter for macro pin
    void setTerm(Term* v);

    /// @brief getter for instance
    Inst* getInst() const;

    /// @brief setter for instance
    void setInst(Inst* v);

    void setName(std::string name);
    std::string& getName() const;

    /// @brief getter for net
    Net* getNet() const;

    /// @brief setter for net
    void setNet(Net* net);
    void addnet(Net* net);

    bool getHasSpecial() const;
    void setHasSpecial(bool flag);

    bool getIsConnectNets() const;
    void addNet(Net *net);
    ArrayObject<ObjectId> *getNetArray() const;

  protected:
    /// @brief copy object
    void copy(Pin const& rhs);
    /// @brief move object
    void move(Pin&& rhs);
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, Pin const& rhs);
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, Pin& rhs);

  private:
    bool    has_special_     : 1;
    bool    is_connect_nets_ : 1;
    int32_t unused_          : 31;

    SymbolIndex name_index_;
    ObjectId term_;  /// macro pin
    ObjectId inst_;  ///< an instance
    union {
        ObjectId net_;
        ObjectId nets_;  ///< a pin cann connect multiple nets in Verilog
    };
};

}  // namespace db
}  // namespace open_edi

#endif
