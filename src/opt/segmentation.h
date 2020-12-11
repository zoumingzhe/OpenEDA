#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "io.h"
#include <iostream>

namespace open_edi {
namespace opt {

class Segmentation {
  public:
    Segmentation();
    ~Segmentation();
    void cleanData();

    std::vector<Node *> getWireSegmentationRet();
    int computeWireSegmentation(std::vector<Node *> nodes_array_raw,
		                int max_node_id,
				const std::vector<Buffer> &buffer_libs,
				const std::vector<Buffer> &driver_libs);

  private:
    double distance(Node *from, Node *to);
    double computeK(double r, 
		    double c, 
		    double l, 
		    double rb, 
		    double cb, 
		    double kb, 
		    double r_source, 
		    double c_down);
    int max_id;
    std::vector<Node *> nodes_array_seg;
};

} // namespace opt
} // namespace open_edi

#endif
