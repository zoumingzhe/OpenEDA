/* @file  message.h
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
 * @file   message.h
 * @date   Apr 2020
 */

#ifndef EDI_UTIL_MESSAGE_H_
#define EDI_UTIL_MESSAGE_H_

#include <boost/algorithm/string.hpp>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "util/namespace.h"

namespace open_edi {
namespace util {

/// @brief Message type for print functions
enum MessageType { kNone = 0, kDebug, kInfo, kWarn, kError, kAssert };

/// class for print and record message
class Message {
  public:
    /// @brief default constructor
    Message();
    /// @brief destructor
    ~Message() {
        is_suppress = false;
        // fclose(log_file_);
    }
    /// @brief issue message
    int issueMsg(const char* category, int id, int m, ...);
    template <typename... Args>
    inline int issueMsg(const char* category, int id, MessageType m,
                        Args... args) {
        return issueMsg(category, id, (int)m, args...);
    }
    int issueMsg(MessageType m, const char* format, ...);
    /// @brief report sumary og messages
    void reportMsg();
    /// @brief get message type by id
    const char* getMsgType(const char* msg_id);
    /// @brief nomarl info message
    int info(const char* format, ...);
    /// @brief register message by file
    void registerMsgFile(const char* file_name);
    /// @brief get the max massage display number
    inline int getLimit() { return message_limit_; }
    /// @brief set the max massage display number
    inline void setLimit(int limit_num) { message_limit_ = limit_num; }

    void testMsg();

  private:
    typedef std::map<std::string, int> msgRecordMap;
    typedef std::unordered_map<int, std::string> msgCategoryMap;
    typedef std::map<std::string, msgCategoryMap> msgRegisterMap;

    void creatLogFile__(const char* file_name = NULL);
    void updateMsgTable__(const char* msg_id, MessageType m);
    int reportMsgMap__(msgRecordMap report_map);
    const char* getMsgId__(const char* category, int id);
    int mgsCountById__(const std::string& msg_id);
    std::string getMsgFormatByMsgId__(const char* msg_id);
    const char* getCategoryByFile__(const char* file);
    std::vector<std::string> parseMsgLine__(const char* msgLine);
    int printPrefix__(MessageType m, char* prefix);
    int vPrintSteream__(MessageType m, FILE* stream, const char* format,
                        va_list args);
    bool isMsgDetailLine__(const char* msgLine);
    std::string getMsgBody__(const std::string& category, int id);
    std::string getMsgFormat__(const std::string& category, int id);
    const char* getMsgDetail__(const std::string& category, int id);
    bool isFileAccess__(const std::string file_name);

    msgRecordMap warn_msg_map_;        // warning Msg counter map
    msgRecordMap error_msg_map_;       // error Msg counter map
    int message_limit_ = 100;          // num of message output
    msgRegisterMap register_msg_map_;  // map for store the msg info
    FILE* log_file_;                   // log file pointer
    bool is_suppress;                  // suppress status
};

/// @brief print to screen (stdout)
int ediPrint(MessageType m, const char* format, ...);
/// @brief print to stream
int ediPrintStream(MessageType m, FILE* stream, const char* format, ...);
/// @brief core function to print formatted data from variable argument list
int ediVPrintStream(MessageType m, FILE* stream, const char* format,
                    va_list args);
/// @brief format to a buffer
int ediSPrint(MessageType m, char* buf, const char* format, ...);
/// @brief core function to format a buffer
int ediVSPrint(MessageType m, char* buf, const char* format, va_list args);
/// @brief format prefix
int ediSPrintPrefix(MessageType m, char* buf);

/// @brief print for assertion
void ediPrintAssertMsg(const char* expr, const char* fileName, unsigned lineNum,
                       const char* funcName, const char* format, ...);
void ediPrintAssertMsg(const char* expr, const char* fileName, unsigned lineNum,
                       const char* funcName);
/// @brief test Message class
int utilTestMsg();

#define ediAssertMsg(condition, args...)                                    \
    do {                                                                    \
        if (!(condition)) {                                                 \
            ::open_edi::util::ediPrintAssertMsg(                            \
                #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__, args); \
            abort();                                                        \
        }                                                                   \
    } while (false)
#define ediAssert(condition)                                          \
    do {                                                              \
        if (!(condition)) {                                           \
            ::open_edi::util::ediPrintAssertMsg(                      \
                #condition, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
            abort();                                                  \
        }                                                             \
    } while (false)

extern Message* message;

}  // namespace util
}  // namespace open_edi

#endif  // EDI_UTIL_MESSAGE_H_
