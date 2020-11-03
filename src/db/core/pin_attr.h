/**
 * @file   inst_term_attr.h
 * @date   Apr 2020
 * @brief  Attributes of instance
 */

#ifndef EDI_DB_DESIGN_INST_TERM_ATTR_H_
#define EDI_DB_DESIGN_INST_TERM_ATTR_H_

#include "db/core/object_attr.h"

namespace open_edi {
namespace db {

class Term;

/// @brief Attributes of instance.
class PinAttr : public ObjectAttr {
  public:
    using BaseType = ObjectAttr;

    /// @brief default constructor
    PinAttr();

    /// @brief copy constructor
    PinAttr(PinAttr const& rhs);

    /// @brief move constructor
    PinAttr(PinAttr&& rhs) noexcept;

    /// @brief copy assignment
    PinAttr& operator=(PinAttr const& rhs);

    /// @brief move constructor
    PinAttr& operator=(PinAttr&& rhs) noexcept;

    /// @brief getter for model terminal
    Term const* getTerm() const { return cell_term_; }

    /// @brief setter for model terminal
    void setTerm(Term const* v) { cell_term_ = v; }

    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    PinAttr* clone() const;

    /// @brief summarize memory usage of the object in bytes
    IndexType memory() const;

  protected:
    /// @brief copy object
    void copy(PinAttr const& rhs);
    /// @brief move object
    void move(PinAttr&& rhs);
    /// @brief overload output stream
    friend OStreamBase& operator<<(OStreamBase& os, PinAttr const& rhs);
    /// @brief overload input stream
    friend IStreamBase& operator>>(IStreamBase& is, PinAttr& rhs);

    Term const* cell_term_;  ///< model terminal in the model for the instance
};

}  // namespace db
}  // namespace open_edi

#endif
