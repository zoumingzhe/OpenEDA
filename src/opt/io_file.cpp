#include "io_file.h"

using namespace std;

namespace open_edi {
namespace opt {

IOFILE::IOFILE() {
    used_id_ = 0;
}

IOFILE::~IOFILE() {
    destroyTree();
}

void IOFILE::readBufferLibrary(const string file_name, vector<Buffer> &buffers) {
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

int IOFILE::readInputTree(string file_name, vector<Buffer> &drivers) {
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
    node->r0 = r0_;
    node->c0 = c0_;
    node->c_down = 0.0;
    node->c_edge = 0.0;
    node->r_edge = 0.0;
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
        node->r0 = r0_;
        node->c0 = c0_; 
        node->type = SINK;
        node->parent = NULL;
        fin_ >> what >> node->id >> node->x >> node->y >> last->capacitance >> last->time;
        last->capacitance /= 1000;//convert to pF
        node->c_down = last->capacitance;
        node->c_edge = 0.0;
        node->r_edge = 0.0;
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
        node->r0 = r0_;
        node->c0 = c0_;
        node->c_down = 0.0;
        node->c_edge = 0.0;
        node->r_edge = 0.0;
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
        node_child->c_edge = c0_ * (fabs(node_parent->x - node_child->x) + fabs(node_parent->y - node_child->y));
        node_child->r_edge = r0_ * (fabs(node_parent->x - node_child->x) + fabs(node_parent->y - node_child->y));
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

void IOFILE::destroyTree() {
    for (int i = nodes_array.size() - 1; i >= 0; i--) {
        Node *cur_node = nodes_array[i], *temp, *seg;
        if (cur_node->parent && nodes_.find(cur_node->parent->id) == nodes_.end()) {       
            seg = cur_node->parent;
            while (seg && nodes_.find(seg->id) == nodes_.end()) {
                temp = seg;
                seg = seg->parent;
                delete temp;
            }
        }
    }
    for(auto &node : nodes_array) {
        if(node->type == SINK){
            if(node->solutions[0]){
                delete node->solutions[0];
            }
            if(node->solutions[1]){
                delete node->solutions[1];
            }
        }
        delete node;
    }
    nodes_.clear();
    nodes_array.clear();
}

double IOFILE::getR0() {
    return r0_;
}

double IOFILE::getC0() {
    return c0_;
}

void IOFILE::getTreeCopy(vector<Node *> &array) {
    unordered_map<uint64_t, Node *> nodes_copy;
    for(auto &node : nodes_){
        Node *node_src = node.second;
        Node *node_dst = new Node();
        node_dst->id = node_src->id;
        node_dst->x = node_src->x;
        node_dst->y = node_src->y;
        node_dst->type = node_src->type;
        node_dst->r0 = node_src->r0 ;
        node_dst->c0 = node_src->c0 ;
        node_dst->c_down = node_src->c_down ;
        node_dst->c_edge = node_src->c_edge ;
        node_dst->r_edge = node_src->r_edge ;
        VanNode *solution_src = node_src->solutions[0];
        if(solution_src){
            VanNode *solution_dst = new VanNode();
            solution_dst->area = solution_src->area;
            solution_dst->buffer_location = solution_src->buffer_location;
            solution_dst->capacitance = solution_src->capacitance;
            solution_dst->next = solution_src->next;
            solution_dst->polarity = solution_src->polarity;
            solution_dst->time = solution_src->time;
            node_dst->solutions[0] = solution_dst;
        }else{
            node_dst->solutions[0] = NULL;
        }
        solution_src = node_src->solutions[1];
        if(solution_src){
            VanNode *solution_dst = new VanNode();
            solution_dst->area = solution_src->area;
            solution_dst->buffer_location = solution_src->buffer_location;
            solution_dst->capacitance = solution_src->capacitance;
            solution_dst->next = solution_src->next;
            solution_dst->polarity = solution_src->polarity;
            solution_dst->time = solution_src->time;
            node_dst->solutions[1] = solution_dst;
        }else{
            node_dst->solutions[1] = NULL;
        }
        nodes_copy[node.first] = node_dst;
    }
    queue<Node *> nodes_queue;
    Node *front = NULL;
    nodes_queue.push(nodes_[0]);
    while(nodes_queue.size() > 0) {
	front = nodes_queue.front();
        Node *front_copy = nodes_copy[front->id];
        if(front->parent){
            front_copy->parent = nodes_copy[front->parent->id];
        }else{
            front_copy->parent = NULL;
        }
        array.push_back(front_copy);
	nodes_queue.pop();
        for (int i = 0; i < front->children.size(); i++) {
            nodes_queue.push(front->children[i]);
        }
    }
}

}//namespace opt
}//namespace open_edi
