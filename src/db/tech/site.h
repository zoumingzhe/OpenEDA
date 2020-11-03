/* @file  site.h
 * @date  08/12/2020 06:33:49 PM CST
 * @brief Class Site.
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_DB_TECH_SITE_H_
#define EDI_DB_TECH_SITE_H_

#include "db/core/object.h"
#include "db/tech/type_def.h"
#include "util/util.h"

namespace open_edi {
namespace db {

using Orient = open_edi::util::Orient;

class SitePatternPair : public Object {
  public:
    /// @brief default constructor
    SitePatternPair() : name_index_(-1), orientation_(Orient::kUnknown) {}
    /// @brief copy constructor
    SitePatternPair(SitePatternPair const &rhs);
    SitePatternPair(const char *name, Orient orientation);
    SitePatternPair(std::string name, Orient orientation);

    // Get
    SymbolIndex getNameIndex() const;
    const char *getName() const;
    Orient getOrientation() const;
    // Set
    void setName(const char *v);
    void setOrientation(Orient v);
    void printLEF(std::ofstream &ofs) const;

  protected:
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, SitePatternPair const &rhs);
    /// @brief overload input stream
    // friend IStreamBase& operator>>(IStreamBase &is, SitePatternPair &rhs);
  private:
    // DATA
    SymbolIndex name_index_; /**< name */
    Orient orientation_;
};

class Site : public Object {
  public:
    using BaseType = Object;

    /// @brief default constructor
    Site();

    /// @brief constructor
    Site(Object *owner, UInt32 id) : BaseType(owner, id) { Site(); }

    /// @brief copy constructor
    Site(Site const &rhs);

    ~Site();

    /// @brief move constructor
    Site(Site &&rhs) noexcept;

    /// @brief copy assignment
    Site &operator=(Site const &rhs);

    /// @brief move constructor
    Site &operator=(Site &&rhs) noexcept;
    /// @brief clone current object for extensibility.
    /// Users need to manage the pointer to avoid memory leakage.
    /// @return the pointer to the new object
    Site *clone() const {return nullptr;}

    /// @brief summarize memory usage of the object in bytes
    UInt32 memory() const;

    /// @brief Get APIs
    SymbolIndex getNameIndex() const;
    const char *getName() const;
    SiteClass getClass() const;
    Symmetry getSymmetry() const;
    Orient getOrientation() const;
    bool getIsDefault() const;
    UInt32 getWidth() const;
    UInt32 getHeight() const;
    uint64_t numSitePatterns() const;
    ObjectId getSitePatternsId() const;

    /// @brief Set APIs
    void setName(const char *v);
    void setClass(SiteClass v);
    void setSymmetry(Symmetry v);
    void setOrientation(Orient v);
    void setIsDefault(bool v);
    void setWidth(UInt32 v);
    void setHeight(UInt32 v);
    void setPatternSize(UInt32 v);
    void addSitePattern(ObjectId obj_id);

    void setClass(const char *v);
    void setSymmetry(const char *v);
    void setOrientation(const char *v);

    /// @brief Print
    void printLEF(std::ofstream &ofs) const;

  protected:
    /// @brief copy object
    void copy(Site const &rhs);
    /// @brief move object
    void move(Site &&rhs);
    /// @brief overload output stream
    friend OStreamBase &operator<<(OStreamBase &os, Site const &rhs);
    /// @brief overload input stream
    friend IStreamBase &operator>>(IStreamBase &is, Site &rhs) {return is;}

  private:
    // DATA
    SymbolIndex name_index_; /**< name */
    SiteClass class_;
    Symmetry symmetry_;
    Orient orientation_;
    bool is_default_;
    UInt32 width_;
    UInt32 height_;
    ObjectId site_patterns_;
};

}  // namespace db
}  // namespace open_edi

#endif
