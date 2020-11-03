/* @file  message.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/**
 * @file   message.cpp
 * @date   Apr 2020
 */

#include "util/message.h"

namespace open_edi {
namespace util {

Message* message;

int ediPrint(MessageType m, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = ediVPrintStream(m, stdout, format, args);
    va_end(args);

    return ret;
}

int ediPrintStream(MessageType m, FILE* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = ediVPrintStream(m, stream, format, args);
    va_end(args);

    return ret;
}

int ediVPrintStream(MessageType m, FILE* stream, const char* format,
                    va_list args) {
    // print prefix
    char prefix[16];
    ediSPrintPrefix(m, prefix);
    fprintf(stream, "%s", prefix);

    // print message
    int ret = vfprintf(stream, format, args);

    return ret;
}

int ediSPrint(MessageType m, char* buf, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = ediVSPrint(m, buf, format, args);
    va_end(args);

    return ret;
}

int ediVSPrint(MessageType m, char* buf, const char* format, va_list args) {
    // print prefix
    char prefix[16];
    ediSPrintPrefix(m, prefix);
    sprintf(buf, "%s", prefix);

    // print message
    int ret = vsprintf(buf + strlen(prefix), format, args);

    return ret;
}

int ediSPrintPrefix(MessageType m, char* prefix) {
    switch (m) {
        case kNone:
            return sprintf(prefix, "%c", '\0');
        case kInfo:
            return sprintf(prefix, "[INFO   ] ");
        case kWarn:
            return sprintf(prefix, "[WARNING] ");
        case kError:
            return sprintf(prefix, "[ERROR  ] ");
        case kDebug:
            return sprintf(prefix, "[DEBUG  ] ");
        case kAssert:
            return sprintf(prefix, "[ASSERT ] ");
        default:
            ediAssertMsg(0, "unknown message type");
    }
    return 0;
}

void ediPrintAssertMsg(const char* expr, const char* fileName, unsigned lineNum,
                       const char* funcName, const char* format, ...) {
    // construct message
    char buf[1024];
    va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    // print message
    ediPrintStream(kAssert, stderr, "%s:%u: %s: Assertion `%s' failed: %s\n",
                   fileName, lineNum, funcName, expr, buf);
}

void ediPrintAssertMsg(const char* expr, const char* file_name,
                       unsigned lineNum, const char* funcName) {
    // print message
    ediPrintStream(kAssert, stderr, "%s:%u: %s: Assertion `%s' failed\n",
                   file_name, lineNum, funcName, expr);
}

Message::Message() {
    log_file_ = NULL;  // log file pointer
    // creatLogFile__();
    is_suppress = false;
    message_limit_ = 20;
}

int Message::info(const char* format, ...) {
    char msg[2048];
    va_list args;
    va_start(args, format);

    vsprintf(msg, format, args);
    int ret = fprintf(stdout, "%s", msg);
    if (log_file_) ret = fprintf(log_file_, "%s", msg);

    va_end(args);

    return ret;
}

int Message::printPrefix__(MessageType m, char* prefix) {
    if (m == kWarn) snprintf(prefix, sizeof(prefix), "WARNING");
    if (m == kError) snprintf(prefix, sizeof(prefix), "ERROR  ");
    if (m == kDebug) snprintf(prefix, sizeof(prefix), "DEBUG  ");
    return 0;
}

int Message::vPrintSteream__(MessageType m, FILE* stream, const char* format,
                             va_list args) {
    // print prefix
    char prefix[16];
    char msg[2048];
    printPrefix__(m, prefix);
    fprintf(stream, "%s", prefix);
    if (log_file_) fprintf(log_file_, "%s", prefix);

    // print message
    vsprintf(msg, format, args);
    int ret = fprintf(stream, "%s", msg);
    if (log_file_) ret = fprintf(log_file_, "%s", msg);

    return ret;
}

bool Message::isMsgDetailLine__(const char* msg_line) {
    if (!msg_line) return false;

    std::string msg = msg_line;
    size_t pos = std::string::npos;

    pos = msg.find("\t");
    if (pos != std::string::npos) {
        return true;
    }
    return false;
}

std::vector<std::string> Message::parseMsgLine__(const char* msg_line) {
    std::vector<std::string> msg_v;
    std::string idStr, msg_format_s, msg_detail_s;
    std::string msgStr = msg_line;
    size_t pos = std::string::npos;
    bool isDetailLine = false;

    if (!msg_line) return msg_v;

    // process detail line
    pos = msgStr.find("\t");

    if (pos != std::string::npos) {
        msg_detail_s = msgStr.substr(pos);
        msg_v.push_back(msg_detail_s);
        return msg_v;
    }

    // process format line
    pos = msgStr.find(" ");
    if (pos != std::string::npos) {
        idStr = msgStr.substr(0, pos);
        msg_v.push_back(idStr);
        msg_format_s = msgStr.substr(pos + 1);
        msg_v.push_back(msg_format_s);
    }

    return msg_v;
}

// Api for getting category
const char* Message::getCategoryByFile__(const char* file) {
    std::string msg_category, file_name_s;
    std::string fileStr = file;
    size_t pos = std::string::npos;

    // spcial parser the relative path.
    pos = fileStr.find_last_of("/");
    if (pos != std::string::npos) {
        file_name_s = fileStr.substr(pos + 1);
        pos = std::string::npos;
    } else {
        file_name_s = fileStr;
    }

    pos = file_name_s.find(".");
    if (pos != std::string::npos) {
        msg_category = file_name_s.substr(0, pos);
        boost::algorithm::to_upper(msg_category);
    }

    return msg_category.c_str();
}

// Api for register message for *.msg file
void Message::registerMsgFile(const char* file_name) {
    std::ifstream f;
    std::vector<std::string> msg_v;
    std::string msg_line_s, msg_format_s, msg_detail_s, msg_body_s,
        msg_category_s;
    msgCategoryMap msgHash;
    bool is_format_line = false;  // two message line can't be the same type
    int id = -1;
    bool is_message_file = false;

    is_message_file = boost::algorithm::ends_with(file_name, ".msg");
    if (!is_message_file) {
        info(
            "The register file is not message file, please check your file "
            "name.");
        return;
    }

    f.open(file_name, std::ifstream::in);
    if (!f.good()) {
        info("cannot open message file %s for read\n", file_name);
        return;
    }

    while (!f.eof()) {
        id = -1;
        // get id and message format
        getline(f, msg_line_s);
        if (msg_line_s.empty() || msg_line_s.find("//") != msg_line_s.npos)
            continue;
        msg_v = parseMsgLine__(msg_line_s.c_str());
        if (msg_v.size() == 2 && !is_format_line) {
            is_format_line = true;
            id = stoi(msg_v[0]);
            msg_format_s = msg_v[1];
        } else {
            info("wrong message format");
            return;
        }

        // get the detail
        if (!f.eof()) getline(f, msg_line_s);
        if (msg_line_s.empty()) continue;
        msg_v = parseMsgLine__(msg_line_s.c_str());
        if (msg_v.size() == 1 && is_format_line) {
            is_format_line = false;
            msg_detail_s = msg_v[0];
        } else {
            info("wrong message format");
            return;
        }

        msg_body_s = msg_format_s + msg_detail_s;
        msgHash.insert(std::pair<int, std::string>(id, msg_body_s));
    }

    // get the category
    msg_category_s = getCategoryByFile__(file_name);
    register_msg_map_.insert(
        std::pair<std::string, msgCategoryMap>(msg_category_s, msgHash));
    for (auto it = register_msg_map_.begin(); it != register_msg_map_.end();
         it++) {
        msgHash = it->second;
        for (auto it = msgHash.begin(); it != msgHash.end(); it++) {
        }
    }
    f.close();
    return;
}

// Api for getting msg body from hash map
std::string Message::getMsgBody__(const std::string& category, int id) {
    msgRegisterMap::iterator it;
    msgCategoryMap::iterator it2;
    msgCategoryMap msgMap_cat;
    std::string msg_body;

    it = register_msg_map_.find(category);
    if (it != register_msg_map_.end()) {
        msgMap_cat = it->second;
    } else {
        info("Cannot find the message %s-%d\n", category.c_str(), id);
        return "";
    }

    it2 = msgMap_cat.find(id);
    if (it2 != msgMap_cat.end()) {
        msg_body = it2->second;
    } else {
        info("Cannot find the message %s-%d.\n", category.c_str(), id);
        return "";
    }

    return msg_body;
}

// Api for get message format
std::string Message::getMsgFormat__(const std::string& category, int id) {
    std::string msg_body = getMsgBody__(category, id);
    std::string format("");
    size_t pos = std::string::npos;

    pos = msg_body.find("\t");
    if (pos != std::string::npos) {
        format = msg_body.substr(0, pos);
    }

    return format;
}

// Api for getting msg format from hash map by msg_id
std::string Message::getMsgFormatByMsgId__(const char* msg_id) {
    size_t pos = std::string::npos;
    std::string msg_id_s = msg_id;
    pos = msg_id_s.find("-");
    int id = -1;
    std::string format;
    if (pos != std::string::npos) {
        std::string category = msg_id_s.substr(0, pos);
        std::string idStr = msg_id_s.substr(pos + 1);
        int id = stoi(idStr);
        format = getMsgFormat__(category, id);
    }

    return format;
}

// Api for getting msg detail from hash map
const char* Message::getMsgDetail__(const std::string& category, int id) {
    size_t pos = std::string::npos;
    std::string msgStr = getMsgBody__(category, id);
    const char* msgDetail;
    std::string msg_detail_s;

    pos = msgStr.find("\t");
    if (pos != std::string::npos) {
        msg_detail_s = msgStr.substr(pos);
        msgDetail = msg_detail_s.c_str();
        return msgDetail;
    } else {
        return NULL;
    }
}

// Api for message counting
int Message::mgsCountById__(const std::string& msg_id) {
    int total = 0;
    std::string msgId_s = msg_id;
    msgRecordMap::iterator iter_warn;
    msgRecordMap::iterator iter_error;
    iter_warn = warn_msg_map_.find(msg_id);
    iter_error = error_msg_map_.find(msg_id);
    int nWarn = 0;
    int nErr = 0;

    if (iter_warn != warn_msg_map_.end()) nWarn = iter_warn->second;
    if (iter_error != error_msg_map_.end()) nErr = iter_error->second;

    total = nWarn + nErr;

    return total;
}

// Api for get full msg id
const char* Message::getMsgId__(const char* category, int id) {
    std::string cat = category;
    std::string msg_id = cat + std::to_string(id);

    return msg_id.c_str();
}
// get message type by id
const char* Message::getMsgType(const char* msg_id) {
    msgRecordMap::iterator iter;
    std::string msgId_s = msg_id;
    iter = warn_msg_map_.find(msgId_s);
    if (iter != warn_msg_map_.end()) return "WARN";
    iter = error_msg_map_.find(msgId_s);
    if (iter != error_msg_map_.end()) return "ERROR";

    return NULL;
}

// report warning message
int Message::reportMsgMap__(msgRecordMap report_map) {
    msgRecordMap::iterator iter;
    for (iter = report_map.begin(); iter != report_map.end(); iter++) {
        std::string msg_id = iter->first;
        std::string format = getMsgFormatByMsgId__(msg_id.c_str());
        info("%10s\t%8s\t%40s\t%7d\n", msg_id.c_str(),
             getMsgType(msg_id.c_str()), format.c_str(), iter->second);
    }

    return 0;
}

// API for report message
void Message::reportMsg() {
    msgRecordMap::iterator iter;
    // print tittle
    info("%62s\n",
         "***************************Report "
         "Message******************************");
    info("%12s\t%8s\t%40s\t%10s\n", "messageID", "type", "message format",
         "total number");
    reportMsgMap__(warn_msg_map_);
    reportMsgMap__(error_msg_map_);

    return;
}

// update message map
void Message::updateMsgTable__(const char* msg_id, MessageType m) {
    msgRecordMap::iterator iter_warn;
    msgRecordMap::iterator iter_error;

    iter_warn = warn_msg_map_.find(msg_id);
    iter_error = error_msg_map_.find(msg_id);
    if (m == kWarn) {
        if (iter_warn != warn_msg_map_.end()) {
            iter_warn->second++;
        } else {
            warn_msg_map_.insert(std::pair<const char*, int>(msg_id, 1));
        }
    }
    if (m == kError) {
        if (iter_error != error_msg_map_.end()) {
            iter_error->second++;
        } else {
            error_msg_map_.insert(std::pair<const char*, int>(msg_id, 1));
        }
    }

    return;
}

// Api for issue messsge
int Message::issueMsg(const char* category, int id, int m, ...) {
    std::string format = getMsgFormat__(category, id);
    std::string msg_id = std::string(category) + "-" + std::to_string(id);
    int ret = 0;
    int total = 0;
    va_list args;

    va_start(args, m);
    if (format.size()) {
        updateMsgTable__(msg_id.c_str(), (MessageType)m);
        total = mgsCountById__(msg_id);

        format = format.substr(1, format.size() - 2);
        format = " <" + msg_id + ">: " + format;
        format = boost::algorithm::replace_all_copy(format, "\\n", "\n");

        if (total <= message_limit_) {
            ret = vPrintSteream__((MessageType)m, stdout, format.c_str(), args);
        }
    }
    va_end(args);

    return ret;
}

// Api for issue messsge
int Message::issueMsg(MessageType m, const char* format, ...) {
    int ret = 0;
    std::string format_s = "\t: " + (std::string)format;
    va_list args;

    va_start(args, format);
    ret = vPrintSteream__(m, stdout, format_s.c_str(), args);
    va_end(args);

    return ret;
}

bool Message::isFileAccess__(const std::string file_name) {
    if (access(file_name.c_str(), F_OK) == -1) {
        return true;
    } else {
        return false;
    }
}
void Message::creatLogFile__(const char* file) {
    int i = 0;
    std::string file_name = "openedi.log";
    while (1) {
        if (i != 0) file_name = "openedi.log" + std::to_string(i);
        if (isFileAccess__(file_name)) {
            log_file_ = fopen(file_name.c_str(), "w+");
            break;
        } else {
            i++;
        }
    }
}

void Message::testMsg() {
    info("<Message testing>: test normal message info\n");
    info("<Message testing>: test message info include %s\n", "string");
    info("<Message testing>: register testing\n");
    info("<Message testing>: issue testing\n");
    issueMsg("UTIL", 1, kWarn, "test");
    issueMsg("UTIL", 2, kError, "");
    issueMsg("UTIL", 3, kError, "");
    issueMsg("UTIL", 3, kError, "");
    reportMsg();
}

// api for pybind11 for testing the message function
int utilTestMsg() {
    message->issueMsg("UTIL", 1, kWarn, "out class testing1");
    message->testMsg();
    message->issueMsg("UTIL", 1, kWarn, "out class testing2");
    message->issueMsg("UTIL", 1, kDebug, "out class testing2");
    message->issueMsg("UTIL", 4, kWarn, "out class testing1");
    message->issueMsg(kDebug, "Debug Info \n");
    message->issueMsg(kDebug, "Debug Info by %s\n", "niiceda");
    message->info("test over\n");
    return 0;
}

}  // namespace util
}  // namespace open_edi
