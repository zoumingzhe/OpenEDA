/* @file  command.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#ifndef SRC_INFRA_COMMAND_H_
#define SRC_INFRA_COMMAND_H_
#include <string>
#include <vector>
#include <map>
#include "option.h"
#include "util/util.h"
#include "db/util/box.h"

namespace open_edi {
namespace infra {


using namespace open_edi::util;

class Command {
 public:
    Command() {
    }
    ~Command() {}
    void setName(const char * v) { name_ = v;}
    std::string getName() const { return name_;}
    void setIsPublic(CommandAccessType v) { is_public_ = v;}
    CommandAccessType getIsPublic() const { return is_public_;}
    void setDescription(const char * v) { description_ = v;}
    std::string getDescription() const { return description_;}
    void addOption(Option* v);
    int getOptionNum() const { return options_.size();}
    Option* getOption(int i) { return options_.at(i);}
    Option* getOption(const char * name);
    void addGroup(OptionGroup* v);
    int getGroupNum() const { return opt_groups_.size();}
    OptionGroup* getGroup(int i) { return opt_groups_.at(i);}
    Option* createOption(const char * name, OptionDataType type);
    bool isOptionSet(const char * name);
    bool getOptionValue(const char * name, bool& value);
    bool getOptionValue(const char * name, int& value);
    bool getOptionValue(const char * name, double& value);
    bool getOptionValue(const char * name, std::string& value);
    bool getOptionValue(const char * name, Point** value);
    bool getOptionValue(const char * name, db::Box** value);
    bool getOptionValue(const char * name, std::vector<std::string>** value);

    int preParse();
    int postParse();
    int setData();
    int parser(int argc, const char** argv);
    int parseValue(Option* opt, const char** argv);
    int parseNum(std::string& data, int& value);
    
    const std::map<std::string, Option*>* getOptionsByOrder() const {return &options_by_order_;}
    void setCallback(std::function<bool(Command*)> v) { cb_ = v;}
    Option* getNextPositionalOption();

 private:
    std::string name_;
    CommandAccessType is_public_;
    std::string description_;
    std::vector<Option*> options_;
    std::map<std::string, int> option_names_;  // use to map< option name, option vetor index>
    std::map<std::string, Option*> options_by_order_;
    std::vector<OptionGroup*> opt_groups_;

    int current_num_;   // internal use for parse data
    std::function<bool(Command*)> cb_;
};

}  // namespace infra 
}  // namespace open_edi
#endif  // SRC_INFRA_COMMAND_H_
