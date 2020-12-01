#include "IO.h"

using namespace std;

namespace open_edi {
namespace opt {

IO::IO() {
    used_id_ = 0;
}

IO::~IO() {
    nodes_.clear();
}

void IO::readBufferLibrary(const string file_name, vector<Buffer> &buffers) {
    buffers.clear();
    fin_.open(file_name);
    if ( fin_.is_open() ) {
        string name;
        double c,d,r;
        while(fin_ >> name >> c >> d >> r) {
            buffers.emplace_back(name=="inverter",c/1000,d,r,0);//convert to pF
        }
        #if DEBUG_IO
        for(uint16_t i=0;i<buffers.size();i++) {
            cout << name << i << ": (c,d,r)=(" << buffers[i].capacitance << "," << buffers[i].delay << "," << buffers[i].resistance << ")" << endl;
        }
        #endif
        fin_.close();
    }
    sort(buffers.begin(),buffers.end());
}

int IO::readInputTree(string file_name, vector<Buffer> &drivers) {
    destroyTree();
    fin_.open(file_name);
    if ( !fin_.is_open() ) {
        return -1;
    }
    string what;
    uint64_t num;
    fin_ >> what >> r0_;
    fin_ >> what >> c0_;
    c0_ /= 1000;//convert to pF
    #if DEBUG_IO
    cout << "(c,r) per unit:" << c0_ << "," << r0_ << endl;
    #endif
    //read source
    Node *node = new Node();
    node->id = 0;
    node->type = SOURCE;
    node->parent = NULL;
    fin_ >> what >> node->x >> node->y;
    node->solutions[0] = NULL;
    node->solutions[1] = NULL;
    nodes_[0] = node;
    //read driver
    double c,d,r;
    fin_ >> what >> num;
    for(int i=0;i<num;i++){
        fin_ >> what >> c >> d >> r;
        drivers.emplace_back(false,c/1000,d,r,0);
    }
    #if DEBUG_IO
    cout << what << "0:(x,y)=" << node->x << "," << node->y << "," << endl;
    #endif
    //read sink
    fin_ >> what >> num;
    for(int i=0;i<num;i++){
        VanNode *last = new VanNode();
        last->next = NULL;
        last->polarity = false;
        last->buffer_location = NULL;
        last->area = 0;
        node = new Node();
        node->type = SINK;
        node->parent = NULL;
        fin_ >> what >> node->id >> node->x >> node->y >> last->capacitance >> last->time;
        last->capacitance /= 1000;//convert to pF
        node->solutions[last->polarity] = last;
        node->solutions[!last->polarity] = NULL;
        nodes_[node->id] = node;
        used_id_ = max(used_id_,node->id);
        #if DEBUG_IO
        cout << what << node->id << ":(x,y,c,t)=" << node->x << "," << node->y << "," << last->capacitance << "," << last->time << endl;
        #endif
    }

    //read candidate
    fin_ >> what >> num;
    for(int i=0;i<num;i++){
        node = new Node();
        node->type = CANDIDATE;
        node->parent = NULL;
        fin_ >> what >> node->id >> node->x >> node->y;
        node->solutions[0] = NULL;
        node->solutions[1] = NULL;
        nodes_[node->id] = node;
        used_id_ = max(used_id_,node->id);
        #if DEBUG_IO
        cout << what << node->id << ":(x,y)=" << node->x << "," << node->y << endl;
        #endif
    }

    //read edge
    uint64_t id_from, id_to;
    while(fin_ >> what >> id_from >> id_to){
        #if DEBUG_IO
        cout << what << ":(from,to)=" << id_from << "," << id_to << endl;
        #endif
        Node *node_parent = nodes_[id_from];
        Node *node_child = nodes_[id_to];
        node_child->parent = node_parent;
        node_parent->children.push_back(node_child);
    }
    fin_.close();

    //generate sorted node array
    queue<Node *> nodes_queue;
    Node *front = NULL;
    nodes_queue.push(nodes_[0]);
    while(nodes_queue.size() > 0) {
	    front = nodes_queue.front();
        nodes_array.push_back(front);
	    nodes_queue.pop();
        for (int i = 0; i < front->children.size(); i++) {
            nodes_queue.push(front->children[i]);
        }
    }
    return 0;
}

void IO::destroyTree() {
    for(auto &o : nodes_) {
        delete o.second;
    }
    nodes_.clear();
    nodes_array.clear();
}

double IO::getR0() {
    return r0_;
}

double IO::getC0() {
    return c0_;
}

}//namespace opt
}//namespace open_edi