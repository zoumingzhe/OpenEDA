/* @file  stream.hpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef EDI_UTIL_STREAM_HPP_
#define EDI_UTIL_STREAM_HPP_

#include <fstream>
#include <sstream>
#include <vector>

#include "util/message.h"

namespace open_edi {

namespace db {
class Object;
};

namespace util {

/// forward declaration
using Object = open_edi::db::Object;

// class Object;
class Database;

/// @brief Base data indicator for input/output streams
struct DataIndicatorBase {
    std::string str_;
};

/// @brief Name of the data field
struct DataFieldName : public DataIndicatorBase {
    /// @brief constructor
    DataFieldName(std::string const& str) { str_ = str; }
};

/// @brief Name of the data type
struct DataTypeName : public DataIndicatorBase {
    /// @brief constructor
    DataTypeName(std::string const& str) { str_ = str; }
};

/// @brief Begin character of the data, e.g., ([{
struct DataBegin : public DataIndicatorBase {
    /// @brief constructor
    DataBegin(std::string const& str) { str_ = str; }
};

/// @brief End character of the data, e.g., }])
struct DataEnd : public DataIndicatorBase {
    /// @brief constructor
    DataEnd(std::string const& str) { str_ = str; }
};

/// @brief Delimiter of the data
struct DataDelimiter : public DataIndicatorBase {
    /// @brief constructor
    DataDelimiter(std::string const& str = ",") { str_ = str; }
};

/// @brief Base stream
class StreamBase {
  public:
    /// @brief constructor
    StreamBase(std::ios_base::openmode mode) : mode_(mode) {}

    /// @brief whether in binary mode
    virtual bool isBinary() const { return (mode_ & std::ios_base::binary); }

    /// @brief whether input
    virtual bool isInput() const { return (mode_ & std::ios_base::in); }

    /// @brief whether output
    virtual bool isOutput() const { return (mode_ & std::ios_base::out); }

  protected:
    std::ios_base::openmode mode_;  ///< mode
};

/// @brief Output to file as binary stream
class OStreamBase : public StreamBase {
  public:
    using BaseType = StreamBase;

    /// @brief default constructor
    OStreamBase() : BaseType(std::ios_base::out) {}

    /// @brief destructor
    virtual ~OStreamBase() {}

    /// @brief flush to output
    virtual void flush() = 0;

    /// @brief getter for the core stream
    virtual std::ostream& getStream() = 0;
};

/// @brief Input from file as binary stream.
/// It is necessary to have database in the context.
class IStreamBase : public StreamBase {
  public:
    using BaseType = StreamBase;

    /// @brief constructor
    IStreamBase(Database& db) : BaseType(std::ios_base::in), db_(db) {}

    /// @brief destructor
    virtual ~IStreamBase() {}

    /// @brief getter for the core stream
    virtual std::istream& getStream() = 0;

    /// @brief getter for database
    Database const& getDb() const { return db_; }

    /// @brief getter for database
    Database& getDb() { return db_; }

    /// @brief getter for current context
    Object* getContext() const {
        if (contexts_.empty()) {
            return nullptr;
        }
        return contexts_.back();
    }

    /// @brief add current context
    void addContext(Object* v) { contexts_.push_back(v); }

    /// @brief pop current context
    void popContext() { contexts_.pop_back(); }

  protected:
    Database& db_;  ///< database
    std::vector<Object*>
        contexts_;  ///< record contexts for objects to set owners
};

template <typename T = std::ofstream>
class OStream : public OStreamBase {
  public:
    using BaseType = OStreamBase;
    using StreamType = T;

    /// @brief default constructor
    OStream() : BaseType(), stream_() {}

    /// @brief constructor
    OStream(char const* filename, std::ios_base::openmode mode)
        : stream_(filename, mode) {
        mode_ = mode;
    }

    /// @brief destructor
    ~OStream() {}

    /// @brief open a file
    void open(char const* filename, std::ios_base::openmode mode) {
        stream_.open(filename, mode);
        mode_ = mode;
    }

    /// @brief close the file
    void close() { stream_.close(); }

    /// @brief return whether the file is open
    bool isOpen() const { return stream_.is_open(); }

    /// @brief flush to output
    void flush() { stream_.flush(); }

    /// @brief getter for the stream
    StreamType& getStream() { return stream_; }

  protected:
    StreamType stream_;  ///< kernel stream
};

template <typename T = std::ifstream>
class IStream : public IStreamBase {
  public:
    using BaseType = IStreamBase;
    using StreamType = T;

    /// @brief constructor
    IStream(Database& db) : BaseType(db), stream_() {}

    /// @brief constructor
    IStream(Database& db, char const* filename, std::ios_base::openmode mode)
        : BaseType(db), stream_(filename, mode) {
        mode_ = mode;
    }

    /// @brief destructor
    ~IStream() {}

    /// @brief open a file
    void open(char const* filename, std::ios_base::openmode mode) {
        stream_.open(filename, mode);
        mode_ = mode;
    }

    /// @brief close the file
    void close() { stream_.close(); }

    /// @brief return whether the file is open
    bool isOpen() const { return stream_.is_open(); }

    /// @brief return whether end of file
    bool eof() const { return stream_.eof(); }

    /// @brief getter for the stream
    StreamType& getStream() { return stream_; }

  protected:
    StreamType stream_;  ///< kernel stream
};

/// @brief template specialization for std::ostringstream
template <>
class OStream<std::ostringstream> : public OStreamBase {
  public:
    using BaseType = OStreamBase;
    using StreamType = std::ostringstream;

    /// @brief default constructor
    OStream() : BaseType(), stream_() {}

    /// @brief destructor
    ~OStream() {}

    /// @brief flush to output
    void flush() { stream_.flush(); }

    /// @brief getter for the stream
    StreamType& getStream() { return stream_; }

  protected:
    StreamType stream_;
};

/// @brief overload output stream for arithmetic types
template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, OStreamBase&>::type
operator<<(OStreamBase& os, T const& rhs) {
    if (os.isBinary()) {  // binary mode
        os.getStream().write(reinterpret_cast<char const*>(&rhs), sizeof(rhs));
    } else {  // ascii mode
        os.getStream() << rhs << " ";
    }
    return os;
}

/// @brief overload output stream for std::string
inline OStreamBase& operator<<(OStreamBase& os, std::string const& rhs) {
    if (os.isBinary()) {  // binary mode
        std::size_t size = rhs.size();
        os.getStream().write(reinterpret_cast<char const*>(&size),
                             sizeof(size));
        os.getStream().write(reinterpret_cast<char const*>(rhs.data()), size);
    } else {  // ascii mode
        os.getStream() << "\"" << rhs << "\""
                       << " ";
    }
    return os;
}

/// @brief overload output stream for data field indicator
template <typename T>
inline typename std::enable_if<std::is_base_of<DataIndicatorBase, T>::value,
                               OStreamBase&>::type
operator<<(OStreamBase& os, T const& rhs) {
    if (os.isBinary()) {  // binary mode
                          // simply skip
    } else {              // ascii mode
        if (!rhs.str_.empty()) {
            os.getStream() << rhs.str_ << " ";
        }
    }
    return os;
}

/// @brief overload output stream for data field indicator
inline OStreamBase& operator<<(OStreamBase& os, DataFieldName const& rhs) {
    if (os.isBinary()) {  // binary mode
                          // simply skip
    } else {              // ascii mode
        os.getStream() << "\"" << rhs.str_ << "\""
                       << " : ";
    }
    return os;
}

/// @brief overload input stream for arithmetic types
template <typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, IStreamBase&>::type
operator>>(IStreamBase& is, T& rhs) {
    if (is.isBinary()) {  // binary mode
        is.getStream().read(reinterpret_cast<char*>(&rhs), sizeof(rhs));
    } else {  // ascii mode
        is.getStream() >> rhs;
    }
    return is;
}

/// @brief overload input stream for std::string
inline IStreamBase& operator>>(IStreamBase& is, std::string& rhs) {
    rhs.clear();
    if (is.isBinary()) {  // binary mode
        std::size_t size;
        is.getStream().read(reinterpret_cast<char*>(&size), sizeof(size));
        if (size) {
            rhs.resize(size);
            is.getStream().read(&rhs[0], size);
        }
    } else {  // ascii mode
        is.getStream() >> rhs;
        if (rhs != "" && rhs != "\"\"") {
            ediAssertMsg(
                rhs.size() > 2U && rhs.front() == '\"' && rhs.back() == '\"',
                "invalid string format for IStreamBase %s", rhs.c_str());
            rhs = rhs.substr(1, rhs.size() - 2);
        } else {
            rhs = "";
        }
    }
    return is;
}

/// @brief overload input stream for data field indicator
template <typename T>
inline typename std::enable_if<
    std::is_base_of<DataIndicatorBase,
                    typename std::remove_reference<T>::type>::value,
    IStreamBase&>::type
operator>>(IStreamBase& is, T&& rhs) {
    if (is.isBinary()) {  // binary mode
                          // simply skip
    } else {              // ascii mode
        if (!rhs.str_.empty()) {
            std::string str;
            is.getStream() >> str;
            ediAssertMsg(str == rhs.str_,
                         "Data indicator mismtach, expected %s, got %s",
                         rhs.str_.c_str(), str.c_str());
        }
    }
    return is;
}

inline IStreamBase& operator>>(IStreamBase& is, DataFieldName&& rhs) {
    if (is.isBinary()) {  // binary mode
                          // simply skip
    } else {              // ascii mode
        is.getStream() >> rhs.str_;
        ediAssertMsg(rhs.str_.size() > 2U && rhs.str_.front() == '\"' &&
                         rhs.str_.back() == '\"',
                     "invalid string format for IStreamBase %s",
                     rhs.str_.c_str());
        rhs.str_ = rhs.str_.substr(1, rhs.str_.size() - 2);
        // swallow delimiter
        std::string delimiter;
        is.getStream() >> delimiter;
        ediAssertMsg(delimiter == ":",
                     "Delimiter after DataFieldName must be :, but got %s",
                     delimiter.c_str());
    }
    return is;
}

}  // namespace util
}  // namespace open_edi

#endif
