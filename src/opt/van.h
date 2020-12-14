#ifndef VAN_H
#define VAN_H

#include <limits>
#include <math.h>
#include "optimize.h"

#define DEBUG_VAN 0
#if DEBUG_VAN
#include <iostream>
#endif

namespace open_edi {
namespace opt {

class Van {
  public:
    Van(double r0, double c0);
    ~Van();

    void optimization(const std::vector<Node *> &nodes_array,
                    const std::vector<Buffer> &buffers,
                    const std::vector<Buffer> &drivers,
                    uint64_t used_id);
    void getSolutions(std::vector<VanSizing *> &solutions);
    void setPinCapacitanceLimits(double cap_limits, bool overwrite);

  private:
    const double min_double_ = std::numeric_limits<double>::lowest();//largest negative value in magnitude
    const double max_double_ = std::numeric_limits<double>::max();//largest negative value in magnitude

    double r0_;           //wire unit resistance
    double c0_;           //wire unit capacitance
    VanSizing *solutions_;  //final solutions
    std::vector<Buffer> buffers_;
    BufferNode *location_head_;
    double pin_capacitance_limits_;
    uint64_t used_id_;

    void insertNonRedundantVanSizing(VanSizing *node, VanSizing *head);
    void mergeNonRedundantVanNode(VanNode *node, BufferNode *location, VanNode *list);
    void vanGinneken(const std::vector<Node *> &nodes_array);
    double distance(Node *from, Node *to);
    void insertNonRedundantVanNode(VanNode *node, VanNode *list);
    void addWire(double wire_length, Node *solution);
    void insertNonRedundantBuffer(VanNode *node, VanNode *list);
    void generateSolutionsByAddingBuffer(Buffer &buffer, VanNode *solutions, VanNode *generate_list);
    void addBuffer(Node *from, Node *to);
    void deleteBufferLocation(BufferNode *location);
    template<typename T> inline void deleteSolution(T *solution);
    void deleteSolutions(VanNode *solutions);
    void clearSolutions();
    #if DEBUG_VAN
    void printTree(Node *root);
    void printBuffer(BufferNode *root);
    #endif
};



}//namespace opt
}//namespace open_edi

#endif
