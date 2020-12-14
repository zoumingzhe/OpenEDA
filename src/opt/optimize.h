#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <vector>
#include <stdint.h>

namespace open_edi {
namespace opt {

struct Buffer {
    bool inverter;
    double capacitance;
    double delay;
    double resistance;
    double area;
    Buffer(bool inverter_, double cap_, double delay_, double res_, double a_)
     : inverter(inverter_), capacitance(cap_), delay(delay_), resistance(res_), area(a_) {}
    bool operator < (const Buffer &buffer) const {
        return capacitance > buffer.capacitance;
    }
};

struct BufferNode {//trace buffer location
    BufferNode *left;
    BufferNode *right;
    BufferNode *next;
    uint32_t count;             //reference count
    uint32_t bufferType;        //bigger than total buffer numbers means no buffer
    uint64_t id_at;
    uint64_t id_to;
};

struct VanNode {//solution node
    VanNode *next;
    bool polarity;      //positive or negative
    double time;        //required arrival time
    double capacitance; //downstream
    double area;        //downstream
    BufferNode *buffer_location;
};

struct VanSizing {//final solutions
    VanSizing *next;
    double time;
    double capacitance;
    double area;
    uint64_t driver_id;
    BufferNode *buffer_location;
    VanSizing() {}
    VanSizing(VanSizing *s) {
        time = s->time;
        capacitance = s->capacitance;
        area = s->area;
        driver_id = s->driver_id;
        buffer_location = s->buffer_location;
        if(buffer_location){
            buffer_location->count++;
        }
    }
};

enum NodeType {SOURCE, INTERNAL, CANDIDATE, SINK};

struct Node {
    uint64_t id;                //node id
    double x;                   //x coordinate
    double y;                   //y coordinate
    NodeType type;              //see NodeType
    VanNode *solutions[2];      //positive & negative polarity
    Node *parent;
    std::vector<Node *> children;
    double r0;                  //wire unit resistance
    double c0;                  //wire unit capacitance
    double c_down;              // downstream capacitance rooted at this node
    double c_edge;              // capacitance of edge from this node to it's parent
    double r_edge;              // resistance of edge from this node to it's parent
};

}//namespace opt
}//namespace open_edi
#endif