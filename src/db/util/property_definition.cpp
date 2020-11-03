/** 
 * @file  property_definition.cpp
 * @date  08/21/2020 04:33:40 PM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "db/core/db.h"
#include "util/util.h"
#include "db/util/symbol_table.h"
#include "db/util/property_definition.h"

namespace open_edi {
namespace db {

PropertyDefinition::PropertyDefinition() : prop_type_ (PropType::kUnknown), name_index_(-1), 
  data_type_(PropDataType::kUnknown), from_def_(false), has_range_(0), has_value_(0) {
  range_u_.int_range_.min = INT_MIN;
  range_u_.int_range_.max = INT_MAX;
  value_u_.string_value_ = -1;
}

PropertyDefinition::PropertyDefinition(Object* owner, UInt32 id) : BaseType(owner, id) {
  PropertyDefinition();
}

PropertyDefinition::~PropertyDefinition() {
  if (data_type_ == PropDataType::kString && value_u_.string_value_) {
      SymbolTable *symbol_table = getTopCell()->getSymbolTable();
      symbol_table->removeReference(value_u_.string_value_, this->getId());    
  }
}


PropertyDefinition::PropertyDefinition(PropertyDefinition const &rhs) { 
  copy(rhs); 
}

PropertyDefinition::PropertyDefinition(PropertyDefinition &&rhs) noexcept { 
  move(std::move(rhs)); 
}

PropertyDefinition &PropertyDefinition::operator=(PropertyDefinition const &rhs) {
  if (this != &rhs) {
    copy(rhs);
  }
  return *this;
}

PropertyDefinition &PropertyDefinition::operator=(PropertyDefinition &&rhs) noexcept {
  if (this != &rhs) {
    move(std::move(rhs));
  }
  return *this;
}

UInt32 PropertyDefinition::memory() const {
  UInt32 ret = this->BaseType::memory();
  ret += sizeof(prop_type_);
  ret += sizeof(name_index_);
  ret += sizeof(data_type_);
  ret += sizeof(from_def_);
  ret += sizeof(has_range_);
  ret += sizeof(has_value_);
  ret += sizeof(range_u_);
  ret += sizeof(value_u_);
  return ret;
}

void PropertyDefinition::copy(PropertyDefinition const &rhs) {
  this->BaseType::copy(rhs);
  prop_type_ = rhs.prop_type_;
  name_index_ = rhs.name_index_;
  data_type_ = rhs.data_type_;
  from_def_ = rhs.from_def_;
  has_range_ = rhs.has_range_;
  has_value_ = rhs.has_value_;
  range_u_ = rhs.range_u_;
  if (rhs.data_type_ == PropDataType::kString && rhs.value_u_.string_value_) {
    value_u_ = rhs.value_u_;
    getTopCell()->addSymbolReference(value_u_.string_value_, this->getId());    
  } else {
    value_u_ = rhs.value_u_;
  }
}

void PropertyDefinition::move(PropertyDefinition &&rhs) {
  this->BaseType::move(std::move(rhs));
  prop_type_ = std::move(rhs.prop_type_);
  name_index_ = std::move(rhs.name_index_);
  data_type_ = std::move(rhs.data_type_);
  from_def_ = std::move(rhs.from_def_);
  has_range_ = std::move(rhs.has_range_);
  has_value_ = std::move(rhs.has_value_);
  range_u_ = std::move(rhs.range_u_);
  value_u_ = std::move(rhs.value_u_);  
}

//Get
PropType PropertyDefinition::getPropType() const { 
  return prop_type_; 
}

const std::string & PropertyDefinition::getPropName() const {
  return getTopCell()->getSymbolByIndex(name_index_); 
}

PropDataType PropertyDefinition::getDataType() const { 
  return data_type_; 
}

bool PropertyDefinition::getFromDEF() const { 
  return from_def_; 
}

bool PropertyDefinition::getHasRange() const { 
  return has_range_; 
}

bool PropertyDefinition::getHasValue() const { 
  return has_value_; 
}

int PropertyDefinition::getIntRangeMin() const { 
  return range_u_.int_range_.min; 
}

int PropertyDefinition::getIntRangeMax() const { 
  return range_u_.int_range_.max; 
}

double PropertyDefinition::getRealRangeMin() const { 
  return range_u_.real_range_.min; 
}

double PropertyDefinition::getRealRangeMax() const { 
  return range_u_.real_range_.max; 
}

int PropertyDefinition::getIntValue() const { 
  return value_u_.int_value_; 
}

double PropertyDefinition::getRealValue() const { 
  return value_u_.real_value_; 
}

std::string PropertyDefinition::getStringValue() const {
  return getTopCell()->getSymbolByIndex(value_u_.string_value_);
}

//Set
void PropertyDefinition::setPropType(PropType v) { 
  prop_type_ = v; 
}

void PropertyDefinition::setPropName(const std::string & v) { 
  if (name_index_ >= 0) {
    SymbolTable *symbol_table = getTopCell()->getSymbolTable();
    symbol_table->removeReference(name_index_, this->getId());
  }

  name_index_ = getTopCell()->getOrCreateSymbol((std::string &)v);
  getTopCell()->addSymbolReference(name_index_, this->getId());  
}

void PropertyDefinition::setDataType(PropDataType v) { 
  data_type_ = v; 
}

void PropertyDefinition::setFromDEF(bool v) { 
  from_def_ = v; 
}

void PropertyDefinition::setHasRange(bool v) { 
  has_range_ = v; 
}

void PropertyDefinition::setIntRangeMin(int v) { 
  range_u_.int_range_.min = v; 
  has_range_ = true;
}

void PropertyDefinition::setIntRangeMax(int v) { 
  range_u_.int_range_.max = v;
  has_range_ = true;
}

void PropertyDefinition::setRealRangeMin(double v) { 
  range_u_.real_range_.min = v; 
  has_range_ = true;
}

void PropertyDefinition::setRealRangeMax(double v) { 
  range_u_.real_range_.max = v; 
  has_range_ = true;
}

//TODO: check range...
void PropertyDefinition::setIntValue(int v) { 
  value_u_.int_value_ = v; 
  has_value_ = true;
}

void PropertyDefinition::setRealValue(double v) { 
  value_u_.real_value_ = v; 
  has_value_ = true;
}

void PropertyDefinition::setStringValue(const std::string & v) { 
  if (data_type_ == PropDataType::kString) {
    if (value_u_.string_value_ >= 0) {
      SymbolTable *symbol_table = getTopCell()->getSymbolTable();
      symbol_table->removeReference(value_u_.string_value_, this->getId());
    }

    value_u_.string_value_ = getTopCell()->getOrCreateSymbol((std::string &)v);
    getTopCell()->addSymbolReference(value_u_.string_value_, this->getId());
    has_value_ = true;
  } 
}

void PropertyDefinition::printLEF(std::ofstream & ofs) const {
  ofs << "    " << toString(getPropType()) << " " << getPropName();
  
  switch (getDataType()) {
  case PropDataType::kInt:
    ofs << " INTEGER";
    if (getHasRange()) {
      ofs << " RANGE " << getIntRangeMin() << " " << getIntRangeMax();
    }
    if (getHasValue()) {
      ofs << " " << getIntValue();
    }    
    break;
  case PropDataType::kReal:
    ofs << " REAL";
    if (getHasRange()) {
      ofs << " RANGE " << getRealRangeMin() << " " << getRealRangeMax();
    } 
    if (getHasValue()) {
      ofs << " " << getRealValue();
    }    
    break;
  case PropDataType::kString:
    ofs << " STRING";
    if (getHasValue()) {
      ofs << " \"" << getStringValue().c_str() << "\"";
    }    
    break;
  }
  ofs << " ;\n";
}

void PropertyDefinition::printDEF(FILE *fp) const {
  fprintf(fp, "  %s %s", toString(getPropType()).c_str(), getPropName().c_str());
  
  switch (getDataType()) {
  case PropDataType::kInt:
    fprintf(fp, " INTEGER");
    if (getHasRange()) {
      fprintf(fp, " RANGE %d %d", getIntRangeMin(), getIntRangeMax());
    }
    if (getHasValue()) {
      fprintf(fp, " %d", getIntValue());
    }    
    break;
  case PropDataType::kReal:
    fprintf(fp, " REAL");
    if (getHasRange()) {
      fprintf(fp, " RANGE %g %g", getRealRangeMin(), getRealRangeMax());
    } 
    if (getHasValue()) {
      fprintf(fp, " %g", getRealValue());
    }    
    break;
  case PropDataType::kString:
    fprintf(fp, " STRING");
    if (getHasValue()) {
      fprintf(fp, " \"%s\"", getStringValue().c_str());
    }    
    break;
  }
  fprintf(fp, " ;\n");
}

OStreamBase &operator<<(OStreamBase &os, PropertyDefinition const &rhs) {
  os << DataTypeName(className(rhs)) << DataBegin("(");
  os << DataFieldName("prop_type_") << toString(rhs.prop_type_) << DataDelimiter();
  os << DataFieldName("name_index_") << rhs.name_index_ << DataDelimiter();
  os << DataFieldName("prop_name_") << rhs.getPropName() << DataDelimiter();
  os << DataFieldName("data_type_") << toString(rhs.data_type_) << DataDelimiter();
  os << DataFieldName("from_def_") << rhs.from_def_ << DataDelimiter();
  
  if (rhs.has_range_) {
    os << DataFieldName("has_range_") << "true" << DataDelimiter();
    switch (rhs.data_type_) {
    case PropDataType::kInt:
      os << DataFieldName("range_u_") << DataDelimiter("[") << rhs.getIntRangeMin() << DataDelimiter(" ") << rhs.getIntRangeMax() << DataDelimiter("]") << DataDelimiter();
      break;
    case PropDataType::kReal:
      os << DataFieldName("range_u_") << DataDelimiter("[") << rhs.getRealRangeMin() << DataDelimiter(" ") << rhs.getRealRangeMax() << DataDelimiter("]") << DataDelimiter();
      break;      
    default:
      break;
    }
  } else {
    os << DataFieldName("has_range_") << "false" << DataDelimiter();
  }

  if (rhs.has_value_) {
    switch (rhs.data_type_) {
    case PropDataType::kString:
      os << DataFieldName("value_u_") << rhs.getStringValue();
      break;
    case PropDataType::kInt:
      os << DataFieldName("value_u_") << rhs.getIntValue();
      break;
    case PropDataType::kReal:
      os << DataFieldName("value_u_") << rhs.getRealValue();
      break; 
    default:
      break; 
    }
  } else {
    os << DataFieldName("has_value_") << "false" << DataDelimiter();
  }
  os << DataEnd(")");
  return os;
}

#if 0
IStreamBase &operator>>(IStreamBase &is, PropertyDefinition &rhs) {
}
#endif

}  // namespace db 
}  // namespace open_edi


