/* @file  option.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_INFRA_OPTION_H_
#define SRC_INFRA_OPTION_H_
#include  <string>
#include <vector>
#include "util/message.h"

namespace open_edi {
namespace infra {

using namespace open_edi::util;

enum CommandStatus {
    kSuccess = 0,
    kInvalidOption = 1,
    kOptionNoValue = 2,
    kNoValueOption = 3,
    kOptionOneValue = 4,
    kDataNotSupport = 5,
    kGetOptionFail = 6,
    kCommandNoDefaultOpt = 7,
    kOptionStartIncorrect = 8,
    kOptionMandatory = 9,
    kOptionDendencyNotSet = 10,
    kOptionOutOfRange = 11,
    kOptionsNeedEnumStrings = 12,
    kOptionIncorrectPara = 13,
    kUserLogicError = 14,
    kGroupGetOptionFail = 15,
    kGroupDependencyError = 16,
    kGroupExclusiveError = 17,
    kOptionDataTypeError = 18,
};

enum OptionDataType {
    kNone = 0,
    kBool = 1,
    kInt = 2,
    kDouble = 3,
    kString = 4,
    kEnum = 5,
    kPoint = 6,
    kRect = 7,
    kIntList = 8,
    kDoubleList = 9,
    kStringList = 10,
};

enum CommandAccessType {
    kPublic = 0,
    kPrivate = 1,
    kObsoleting = 2,
    kObsoleted = 3,
};

class Option {
 public:
    Option();
    Option(const char* name, OptionDataType type, bool is_required, const char* description);
    Option(const char* name, OptionDataType type, bool is_required, std::vector<std::string>* v, const char* description);
    Option(const char* name, OptionDataType type, bool is_required, bool v, const char* description);
    Option(const char* name, OptionDataType type, bool is_required, int v, const char* description, int min = 0, int max = 0);
    Option(const char* name, OptionDataType type, bool is_required, double v, const char* description, double min = 0, double max = 0);
    ~Option() {}
    void setName(const char * v);
    std::string getName() const { return name_;}
    void setDescription(const char * v) { description_ = v;}
    std::string getDescription() const { return description_;}
    void setAlias(Option * v) { alias_ = v;}
    const Option* getAlias() const {return alias_;}
    void setIsRequired(bool v) { is_required_ = v;}
    bool isRequired() const { return is_required_;}
    void setIsPublic(CommandAccessType v) { is_public_ = v;}
    CommandAccessType isPublic() const { return is_public_;}
    void setIsSet(bool v) { is_set_ = v;}
    bool isSet() const { return is_set_;}
    void setTempIsSet(bool v) { temp_is_set_ = v;}
    bool tempIsSet() const { return temp_is_set_;}
    void setIsUserSet(bool v) { is_user_set_ = v;}
    bool isUserSet() const { return is_user_set_;}
    void setType(OptionDataType v) { type_ = v;}
    OptionDataType getType() const { return type_;}
    void setDependency(Option* v) { dependency_ = v;}
    Option* getDependency() const { return dependency_;}
    void setExclude(Option* v) {exclude_ = v;}
    Option* getExclude() const { return exclude_;}
    void setNext(Option* v) {next_ = v;}
    Option* getNext() const { return next_;}

    void setValueIntMax(int v) { value_int_max_ = v;}
    int getValueIntMax() const { return value_int_max_;}
    void setValueIntMin(int v) { value_int_min_ = v;}
    int getValueIntMin() const { return value_int_min_;}
    void setValueDoubleMax(int v) { value_double_max_ = v;}
    int getValueDoubleMax() const { return value_double_max_;}
    void setValueDoubleMin(int v) { value_double_min_ = v;}
    int getValueDoubleMin() const { return value_double_min_;}
    void setMaximumValueNum(int v) { maximum_value_num_ = v;}
    int getMaximumValueNum() const { return maximum_value_num_;}
    void setEnums(std::vector<std::string>* v) { enums_ = v;}
    std::vector<std::string>* getEnums() const { return enums_;}

    void setTempData(void* v) {temp_value_ = v;}
    void* getTempData() { return temp_value_;}
    void setData(void* v) {value_ = v;}
    void* getData() { return value_;}
    void setDefaultData(void* v) {default_value_ = v;}
    void* getDefaultData() {return temp_value_;}
    int checkRule();
    Option& operator+(Option& rhs) {
        rhs.setNext(this);
        return rhs;
    }

 private:
    void initPara(const char* name, OptionDataType type, bool is_required, const char* description);
    std::string name_;
    std::string description_;
    Option* alias_;
    bool is_required_;
    CommandAccessType is_public_;
    bool is_set_;
    bool is_user_set_;  // previous set by user   is set by user
    bool temp_is_set_;
    OptionDataType type_;
    Option* dependency_;
    Option* exclude_;
    Option* next_;
    std::vector<std::string>* enums_;

    int value_int_max_;
    int value_int_min_;
    double value_double_max_;
    double value_double_min_;
    int maximum_value_num_;

    void* value_;
    void* temp_value_;
    void* default_value_;
};

enum OptionRelation {
    kDependency = 0,
    kExclusive = 1,
};

class OptionGroup {
 public:
    OptionGroup() {}
    OptionGroup(const char* name1, const char* name2, OptionRelation r);
    ~OptionGroup() {}
    void setOpt1Name(const char * v) { opt1_name_ = v;}
    std::string getOpt1Name() const { return opt1_name_;}
    void setOpt1Ptr(Option* v) { opt1_ptr_ = v;}
    Option* getOpt1Ptr() const { return opt1_ptr_;}
    void setOpt2Name(const char * v) { opt2_name_ = v;}
    std::string getOpt2Name() const { return opt2_name_;}
    void setOpt2Ptr(Option* v) { opt2_ptr_ = v;}
    Option* getOpt2Ptr() const { return opt2_ptr_;}
    void setRelation(OptionRelation v) { relation_ = v;}
    OptionRelation getRelation() const { return relation_;}
    void setNext(OptionGroup* v) {next_ = v;}
    OptionGroup* getNext() const { return next_;}
    int checkRule();

    OptionGroup& operator+(OptionGroup& rhs) {
        rhs.setNext(this);
        return rhs;
    }

 private:
    std::string opt1_name_;
    Option* opt1_ptr_;
    std::string opt2_name_;
    Option* opt2_ptr_;
    OptionRelation relation_;
    OptionGroup* next_;
};

/*template<typename T>
class Option : public OptionBase {
 public:
    Option() {}
    ~Option() {}
    virtual void setData(void *v) { value_ = (T *)v; }
    virtual void *getData() { return (void *)value_; }
    virtual void setTempData(void *v) { temp_value_ = (T *)v; }
    virtual void *getTempData() { return (void *)temp_value_; }
    virtual void setDefaultData(void* v) { default_value_ = (T *)v;}
    virtual void* getDefaultData() const { return (void* )default_value_;}

 private:
    T* value_;
    T* temp_value_;
    T* default_value_;
};*/

}  // namespace infra
}  // namespace open_edi

#endif  // SRC_INFRA_OPTION_H_

