/**
 * File              : graphviz_writer.h
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 06.19.2020
 * Last Modified Date: 06.19.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#ifndef EDI_IO_GRAPHVIZ_WRITER_H_
#define EDI_IO_GRAPHVIZ_WRITER_H_

#include "util/util.h"
#include <fstream>

EDI_BEGIN_NAMESPACE

class Model; 
class Design; 
class Database; 

class GraphvizWriter {
  public:
    /// @brief Constructor 
    GraphvizWriter(Database const& db); 

    /// @brief Top API for write files 
    bool operator()(std::string const& filename) const {
      return run(filename);
    }

    /// @brief Top API for write files 
    bool run(std::string const& filename) const; 

  protected:
    /* remove-design
    /// @brief write a design 
    void writeDesign(std::ofstream& os, Design const* design) const; 
    */
    /// @brief insert escape character for string as a name 
    std::string processName(std::string const& str) const;

    Database const& db_; ///< database 
};

EDI_END_NAMESPACE

#endif
