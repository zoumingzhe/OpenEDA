/* @file  option.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include  <string>
#include <vector>
#include "option.h"
#include "db/util/box.h"

namespace open_edi {
namespace infra {

Option::Option() {
    is_set_ = false;
    value_int_max_ = 0;
    value_int_min_ = 0;
    value_double_max_ = 0;
    value_double_min_ = 0;
    next_ = nullptr;
}

Option::Option(const char* name, OptionDataType type, bool is_required, const char* description) {
    // message->info("option constructor\n");
    initPara(name, type, is_required, description);

    switch (type) {
    case OptionDataType::kNone:
        break;
    case OptionDataType::kBool:
    {
        bool *a = new bool();
        setData(a);
        bool *temp = new bool();
        setTempData(temp);
        break;
    }
    case OptionDataType::kInt:
    {
        int *a = new int();
        setData(a);
        int *temp = new int();
        setTempData(temp);
        break;
    }
    case OptionDataType::kDouble:
    {
        double *a = new double();
        setData(a);
        double *temp = new double();
        setTempData(temp);
        break;
    }
    case OptionDataType::kString:
    {
        std::string *a = new std::string();
        setData(a);
        std::string *temp = new std::string();
        setTempData(temp);
        break;
    }
    case OptionDataType::kEnum:
    {
        message->issueMsg("INFRA", kOptionsNeedEnumStrings, kError, name);
        break;
    }
    case OptionDataType::kPoint:
    {
        //message->info("in option point\n");
        Point* a = new Point();
        setData(a);
        Point *temp = new Point();
        setTempData(temp);
        break;
    }
    case OptionDataType::kRect:
    {
        db::Box* a = new db::Box();
        setData(a);
        db::Box *temp = new db::Box();
        setTempData(temp);
        break;
    }
    case OptionDataType::kIntList:
    {
        std::vector<int> *a = new std::vector<int>();
        setData(a);
        std::vector<int> *temp = new std::vector<int>();
        setTempData(temp);
        break;
    }
    case OptionDataType::kDoubleList:
    {
        std::vector<double> *a = new std::vector<double>();
        setData(a);
        std::vector<double> *temp = new std::vector<double>();
        setTempData(temp);
        break;
    }
    case OptionDataType::kStringList:
    {
        std::vector<std::string> *a = new std::vector<std::string>();
        setData(a);
        std::vector<std::string> *temp = new std::vector<std::string>();
        setTempData(temp);
        break;
    }
    default:
    {
        break;
    }
    }
}

Option::Option(const char* name, OptionDataType type, bool is_required, std::vector<std::string>* v, const char* description) {
    if (type != OptionDataType::kEnum) {
        message->issueMsg("INFRA", kOptionIncorrectPara, kError, name);
    }
    initPara(name, type, is_required, description);
    int *a = new int();
    setData(a);
    int *temp = new int();
    setTempData(temp);
    setEnums(v);
}

Option::Option(const char* name, OptionDataType type, bool is_required, const char* v, const char* description) {
    if (type != OptionDataType::kEnum) {
        message->issueMsg("INFRA", kOptionIncorrectPara, kError, name);
    }
    initPara(name, type, is_required, description);
    int *a = new int();
    setData(a);
    int *temp = new int();
    setTempData(temp);
    std::vector<std::string>* enums = new std::vector<std::string>();
    std::string enum_string = v;
    std::string::size_type pos = enum_string.find(" ");
    while (pos != std::string::npos) {
        std::string sub_string = enum_string.substr(0, pos);
        enums->push_back(sub_string);
        enum_string = enum_string.substr(pos + 1);
        pos = enum_string.find(" ");
        if (pos == std::string::npos) {
            enums->push_back(enum_string);
        }
    }
    setEnums(enums);
}

Option::Option(const char* name, OptionDataType type, bool is_required, bool v, const char* description) {
    if (type != OptionDataType::kBool) {
        message->issueMsg("INFRA", kOptionIncorrectPara, kError, name);
        // return;
    }
    initPara(name, type, is_required, description);
    bool *a = new bool();
    setData(a);
    bool *temp = new bool();
    setTempData(temp);
}

Option::Option(const char* name, OptionDataType type, bool is_required, int v, const char* description, int min, int max) {
    if (type != OptionDataType::kInt) {
        message->issueMsg("INFRA", kOptionIncorrectPara, kError, name);
    }
    initPara(name, type, is_required, description);
    // ob->setAlias(alias);
    int *a = new int();
    setData(a);
    int *temp = new int();
    setTempData(temp);
    setValueIntMin(min);
    setValueIntMax(max);
}

Option::Option(const char* name, OptionDataType type, bool is_required, double v, const char* description, double min, double max) {
    if (type != OptionDataType::kDouble) {
        message->issueMsg("INFRA", kOptionIncorrectPara, kError, name);
    }
    initPara(name, type, is_required, description);
    double *a = new double();
    setData(a);
    double *temp = new double();
    setTempData(temp);
    setValueDoubleMin(min);
    setValueDoubleMax(max);
}

void Option::initPara(const char* name, OptionDataType type, bool is_required, const char* description) {
    setName(name);
    setType(type);
    setDescription(description);
    setIsRequired(is_required);
}


void Option::setName( const char * v) {
    /*if (v[0] != '-') {
        message->issueMsg("INFRA", kOptionStartIncorrect, kError, v);
        return;
    }*/
    name_ = v;
}

int Option::checkRule() {
    // mandatory should be set
    if (isRequired()) {
        if (isSet() == false) {
            message->issueMsg("INFRA", kOptionMandatory, kError, getName().c_str());
        }
    }
    // dependency should be set
    // if (getDependency()) {
    //     if (getDependency()->isSet() == false) {
    //         message->issueMsg("INFRA", kOptionDendencyNotSet, kError, getName().c_str(), getDependency()->getName().c_str());
    //     }
    // }

    if (getType() == OptionDataType::kInt) {
        if (getValueIntMax() != getValueIntMin()) {
            int value = *((int*)getTempData());
            if (value < getValueIntMin() || value > getValueIntMax()) {
                message->issueMsg("INFRA", kOptionOutOfRange, kError, getName().c_str(), value, getValueIntMin(), getValueIntMax());
                return kOptionOutOfRange;
            }
        }
    }
    if (getType() == OptionDataType::kDouble) {
        if (getValueDoubleMax() != getValueDoubleMin()) {
            double value = *((double*)getTempData());
            if (value < getValueDoubleMin() || value > getValueDoubleMax()) {
                message->issueMsg("INFRA", kOptionOutOfRange, kError, getName().c_str(), value, getValueDoubleMin(), getValueDoubleMax());
                return kOptionOutOfRange;
            }
        }
    }
    return kSuccess;
}

OptionGroup::OptionGroup(const char* name1, const char* name2, OptionRelation r) {
    setOpt1Name(name1);
    setOpt2Name(name2);
    setRelation(r);
    opt1_ptr_ = nullptr;
    opt2_ptr_ = nullptr;
    next_ = nullptr;
}

OptionGroup::OptionGroup() {
    opt1_ptr_ = nullptr;
    opt2_ptr_ = nullptr;
    next_ = nullptr;
}

int OptionGroup::checkRule() {
    if (relation_ == OptionRelation::kDependency) {
        if (opt1_ptr_->isSet() ^ opt2_ptr_->isSet()) {
            message->issueMsg("INFRA", kGroupDependencyError, kError, opt1_name_.c_str(), opt2_name_.c_str());
            return kGroupDependencyError;
        }
    } else if (relation_ == OptionRelation::kExclusive) {
        if (opt1_ptr_->isSet() && opt2_ptr_->isSet()) {
            message->issueMsg("INFRA", kGroupExclusiveError, kError, opt1_name_.c_str(), opt2_name_.c_str());
            return kGroupExclusiveError;
        }
    }

    return kSuccess;
}

}  // namespace infra
}  // namespace open_edi
