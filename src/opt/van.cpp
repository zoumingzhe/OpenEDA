#include "van.h"

using namespace std;

namespace open_edi {
namespace opt {

Van::Van(double r0, double c0) : r0_(r0), c0_(c0) {
    solutions_ = nullptr;
    pin_capacitance_limits_ = max_double_;
}

Van::~Van() {
    clearSolutions();
}

void Van::getSolutions(vector<VanSizing *> &solutions) {
    solutions.clear();
    VanSizing *solution = solutions_;
    while(solution->next) {//exclude tail
        if(solution->capacitance < pin_capacitance_limits_){
            solutions.push_back(solution);
            #if DEBUG_VAN
                cout << "final solution(t,c,a)=(" << solution->time << "," << solution->capacitance << "," << solution->area << ")" << endl;
            #endif
        }
        solution = solution->next;
    }
}

void Van::clearSolutions() {
    #if DEBUG_VAN
    cout << "try clear solutions" << endl;
    #endif
    //delete final solutions
    VanSizing *solution = solutions_;
    VanSizing *next = nullptr;
    while(solution) {
        next = solution->next;
        deleteSolution(solution);
        solution = next;
    }
    solutions_ = nullptr;
}

void Van::setPinCapacitanceLimits(double cap_limits, bool overwrite) {
    if (overwrite) {
        pin_capacitance_limits_ = cap_limits;
    } else {
        pin_capacitance_limits_ = min(pin_capacitance_limits_,cap_limits);
    }
}

void Van::insertNonRedundantVanSizing(VanSizing *node, VanSizing *head) {
    //decrease time order
    #if DEBUG_VAN
        cout << "try solution(t,c,a)=(" << node->time << "," << node->capacitance << "," << node->area << ")" << endl;
    #endif
    VanSizing *pred = head;
    VanSizing *curr = pred->next;
    VanSizing *x;
    while (curr != nullptr) {
        if (node->time > curr->time) {//insert here
            x = new VanSizing(node);
            pred->next = x;
            x->next = curr;
            pred = x;
            break;
        } else {
            if (node->capacitance>=curr->capacitance && node->area>=curr->area) {//dominated
                return;
            } else {//check next
                pred = curr;
                curr = curr->next;
            }
        }
    }
    //check redundant
    while (curr != nullptr) {
        if (node->capacitance<=curr->capacitance && node->area<=curr->area) {//dominate other
            pred->next = curr->next;
            deleteSolution(curr);
            curr = pred->next;
        } else {
            pred = curr;
            curr = curr->next;
        }
    }
}

void Van::optimization(const vector<Node *> &nodes_array,
                     const vector<Buffer> &buffers,
                     const vector<Buffer> &drivers) {
    clearSolutions();
    buffers_ = buffers;
    vanGinneken(nodes_array);
    //clear negative polarity solutions
    deleteSolutions(nodes_array[0]->solutions[1]);
    nodes_array[0]->solutions[1] = nullptr;
    //add driver
    VanSizing dummy_head;
    dummy_head.time = max_double_;
    dummy_head.capacitance = max_double_;
    dummy_head.area = max_double_;
    VanSizing *dummy_tail = new VanSizing();
    dummy_tail->time = min_double_;
    dummy_tail->capacitance = min_double_;
    dummy_tail->area = min_double_;
    dummy_tail->buffer_location = nullptr;
    dummy_head.next = dummy_tail;
    dummy_tail->next = nullptr;
    VanSizing potential;
    for(uint64_t i=0;i<drivers.size();i++) {
        Buffer driver = drivers[i];
        potential.driver_id = i;
        VanNode *solution = nodes_array[0]->solutions[0];
        while(solution != nullptr) {//update solutions
            potential.time = solution->time-driver.resistance*solution->capacitance-driver.delay;
            potential.area = solution->area + driver.area;
            potential.capacitance = solution->capacitance;
            potential.buffer_location = solution->buffer_location;
            insertNonRedundantVanSizing(&potential,&dummy_head);//decrease time order
            solution = solution->next;
        }
    }
    solutions_ = dummy_head.next;
    //clear node solutions
    VanNode *solution = nodes_array[0]->solutions[0];
    while(solution){
        VanNode *next = solution->next;
        deleteSolution(solution);
        solution = next;
    }
    nodes_array[0]->solutions[0] = nullptr;
}

void Van::mergeNonRedundantVanNode(VanNode *node, BufferNode *location, VanNode *list) {
    //increase time order
    VanNode *pred = list;
    VanNode *curr = pred->next;
    VanNode *mark_pred = pred;
    VanNode *mark_curr = curr;
    bool dominate = false;
    while(curr != nullptr) {
        if ( node->time > curr->time ) {
            if (node->capacitance<=curr->capacitance && node->area<=curr->area) {//dominate curr
                dominate = true;
                pred->next = curr->next;
                deleteSolution(curr);
                curr = pred->next;
            } else {//check next
                pred = curr;
                curr = curr->next;
            }
        } else {//try insert here
            mark_pred = pred;
            mark_curr = curr;
            if (!dominate) {//check dominated
                while (curr != nullptr) {
                    if (node->capacitance>=curr->capacitance && node->area>=curr->area) {//dominated
                        return;
                    } else {
                        pred = curr;
                        curr = curr->next;
                    }
                }
            }
            curr = mark_curr;
            break;
        }
    }
    BufferNode *p = new BufferNode();
    p->count = 1;
    p->left = node->buffer_location;
    p->right = location;
    if(p->right){
        p->right->count++;
    }
    p->bufferType = buffers_.size();//no buffer, ignore direction
    node->buffer_location = p;
    if (curr == nullptr) {//insert last
        pred->next = node;
        node->next = curr;
    } else {
        mark_pred->next = node;
        node->next = mark_curr;
    }
}
void Van::vanGinneken(const vector<Node *> &nodes_array) {
    #if DEBUG_VAN
    cout << "start van Ginneken" << endl;
    #endif
    for (uint64_t i=nodes_array.size()-1;i>0;i--) {//i>0, exclude source node
        Node *child_node = nodes_array[i];
        Node *parent_node = child_node->parent;
        double wire_length = distance(parent_node,child_node);
        addWire(wire_length, child_node);
        if(parent_node->type==CANDIDATE) {
            addBuffer(parent_node, child_node);
        }
        if( (uint64_t)parent_node->solutions[0] | (uint64_t)parent_node->solutions[1] ) {
            //merge two solutions (increase time order)
            #if DEBUG_VAN
            cout << "merge two solutions" << endl;
            #endif
            VanNode dummy_head;
            dummy_head.time = min_double_;
            dummy_head.capacitance = min_double_;
            dummy_head.area = min_double_;
            for (int polarity=0;polarity<2;polarity++) {
                dummy_head.next = nullptr;
                VanNode *solution_parent = parent_node->solutions[polarity];
                VanNode *solution_child = child_node->solutions[polarity];
                VanNode *small_time;
                BufferNode *location;
                while ( solution_parent && solution_child ) {
                    double time_parent = solution_parent->time;
                    double time_child = solution_child->time;
                    if (time_parent < time_child) {
                        //seperate solution1
                        small_time = solution_parent;
                        solution_parent = solution_parent->next;
                        //update it
                        small_time->time = time_parent;
                        small_time->capacitance += solution_child->capacitance;
                        small_time->area += solution_child->area;
                        location = solution_child->buffer_location;
                    } else {
                        //seperate solution2
                        small_time = solution_child;
                        solution_child = solution_child->next;
                        //update it
                        small_time->time = time_child;
                        small_time->capacitance += solution_parent->capacitance;
                        small_time->area += solution_parent->area;
                        location = solution_parent->buffer_location;
                    }
                    mergeNonRedundantVanNode(small_time,location,&dummy_head);
                }
                parent_node->solutions[polarity] = dummy_head.next;
                child_node->solutions[polarity] = nullptr;
                deleteSolutions(solution_parent);
                deleteSolutions(solution_child);
            }
            #if DEBUG_VAN
            VanNode *s = child_node->solutions[0];
            while(s){
                cout << "solution(t,c,a)=(" << s->time << "," << s->capacitance << "," << s->area << ")" << endl;
                s = s->next;
            }
            #endif
        } else {
            parent_node->solutions[0] = child_node->solutions[0];
            parent_node->solutions[1] = child_node->solutions[1];
            child_node->solutions[0] = nullptr;
            child_node->solutions[1] = nullptr;
        }
    }
}

double Van::distance(Node *from, Node *to) {
    return (fabs(from->x-to->x)+fabs(from->y-to->y));
}

void Van::insertNonRedundantVanNode(VanNode *node, VanNode *list) {
    //increase time order
    VanNode *pred = list;
    VanNode *curr = pred->next;
    VanNode *mark_pred = pred;
    VanNode *mark_curr = curr;
    bool dominate = false;
    while(curr != nullptr) {
        if ( node->time > curr->time ) {
            if (node->capacitance<=curr->capacitance && node->area<=curr->area) {//dominate curr
                dominate = true;
                pred->next = curr->next;
                deleteSolution(curr);
                curr = pred->next;
            } else {
                pred = curr;
                curr = curr->next;
            }
        } else {//try insert here
            mark_pred = pred;
            mark_curr = curr;
            if(!dominate) {
                while(curr != nullptr){
                    if (node->capacitance>=curr->capacitance && node->area>=curr->area) {//dominated
                        deleteSolution(node);
                        return;
                    } else {//check next
                        pred = curr;
                        curr = curr->next;
                    }
                }
            }
            curr = mark_curr;
            break;
        }
    }
    if (curr == nullptr) {//insert last
        pred->next = node;
        node->next = curr;
    } else {
        mark_pred->next = node;
        node->next = mark_curr;
    }
}
void Van::addWire(double wire_length, Node *node) {
    #if DEBUG_VAN
    cout << "addWire with length " << wire_length << " at node " << node->id << endl;
    #endif
    //add wire && update solution
    VanNode dummy_head;
    dummy_head.time = min_double_;
    dummy_head.capacitance = min_double_;
    dummy_head.area = min_double_;
    for (int polarity=0;polarity<2;polarity++) {
        dummy_head.next = nullptr;
        VanNode *origin = node->solutions[polarity];
        while( origin != nullptr ) {//resort by updated time
            double r = wire_length * r0_;
            double c = wire_length * c0_;
            origin->time = origin->time - 0.5*r*c - r*origin->capacitance;
            origin->capacitance += c;
            VanNode *single_node = origin;
            origin = origin->next;
            insertNonRedundantVanNode(single_node,&dummy_head);//increase time order
        }
        node->solutions[polarity] = dummy_head.next;
    }
    #if DEBUG_VAN
    VanNode *s = node->solutions[0];
    while(s){
        cout << "solution(t,c,a)=(" << s->time << "," << s->capacitance << "," << s->area << ")" << endl;
        s = s->next;
    }
    #endif
}

void Van::insertNonRedundantBuffer(VanNode *node, VanNode *list) {
    //increase time order
    VanNode *pred = list;
    VanNode *curr = pred->next;
    VanNode *mark_pred = pred;
    VanNode *mark_curr = curr;
    bool dominate = false;
    while(curr != nullptr) {
        if (node->time > curr->time) {
            if (node->area <= curr->area) {//dominate curr
                dominate = true;
                pred->next = curr->next;
                deleteSolution(curr);
                curr = pred->next;
            } else {
                pred = curr;
                curr = curr->next;
            }
        } else {//try insert here
            mark_pred = pred;
            mark_curr = curr;
            if(!dominate){//check dominated
                while(curr != nullptr){
                    if (node->area>=curr->area) {//dominated
                        return;
                    } else {//check next
                        pred = curr;
                        curr = curr->next;
                    }
                }
            }
            curr = mark_curr;
            break;
        }
    }
    VanNode *solution = new VanNode();
    solution->area = node->area;
    solution->capacitance = node->capacitance;
    solution->time = node->time;
    BufferNode *p = new BufferNode();
    p->count = 1;
    p->left = node->buffer_location;
    if(p->left){
        p->left->count++;
    }
    p->right = nullptr;
    solution->buffer_location = p;
    if (curr == nullptr) {//insert last
        pred->next = solution;
        solution->next = curr;
    } else {
        mark_pred->next = solution;
        solution->next = mark_curr;
    }
}
void Van::generateSolutionsByAddingBuffer(Buffer &buffer, VanNode *solutions, VanNode *generate_list) {
    VanNode potential;
    VanNode *solution = solutions;
    while(solution) {
        #if DEBUG_VAN
        cout << "solution(t,c,a)=(" << solution->time << "," << solution->capacitance << "," << solution->area << ")" << endl;
        cout << "with buffer(r,d,a)=(" << buffer.resistance << "," << buffer.delay << "," << buffer.area << ")" << endl;
        #endif
        potential.time = solution->time - buffer.delay - buffer.resistance*solution->capacitance;
        potential.capacitance = buffer.capacitance;
        potential.area = solution->area + buffer.area;
        potential.buffer_location = solution->buffer_location;
        #if DEBUG_VAN
        cout << "generate(t,c,a)=(" << potential.time << "," << potential.capacitance << "," << potential.area << ")" << endl;
        #endif
        insertNonRedundantBuffer(&potential,generate_list);
        solution = solution->next;
    }
}
void Van::addBuffer(Node *from, Node *to) {
    #if DEBUG_VAN
    cout << "add buffer from " << from->id  << " to " << to->id << endl;
    #endif
    VanNode *z[2][buffers_.size()];
    //add buffer && check different buffer types
    #if DEBUG_VAN
    cout << "generate buffer solutions" << endl;
    #endif
    VanNode dummy_head;
    dummy_head.time = min_double_;
    dummy_head.capacitance = min_double_;
    dummy_head.area = min_double_;
    for (int polarity=0;polarity<2;polarity++) {
        for(uint64_t i=0;i<buffers_.size();i++) {
            Buffer buffer = buffers_[i];
            bool add_buffer_polarity = polarity^buffer.inverter;
            dummy_head.next = nullptr;
            generateSolutionsByAddingBuffer(buffer, to->solutions[polarity], &dummy_head);
            z[add_buffer_polarity][i] = dummy_head.next;
        }
    }
    // add answer to list
    #if DEBUG_VAN
    cout << "insert buffer solutions" << endl;
    #endif
    for (int polarity=0;polarity<2;polarity++) {
        dummy_head.next = to->solutions[polarity];
        for (uint64_t i=0;i<buffers_.size();i++) {
            VanNode *node = z[polarity][i];
            while(node != nullptr) {
                node->polarity = polarity;
                VanNode *single_node = node;
                node = node->next;
                BufferNode *location = single_node->buffer_location;
                location->bufferType = i;
                location->id_at = from->id;
                location->id_to = to->id;
                insertNonRedundantVanNode(single_node,&dummy_head);
            }
        }
        to->solutions[polarity] = dummy_head.next;
    }
    #if DEBUG_VAN
    VanNode *s = to->solutions[0];
    while(s){
        cout << "solution(t,c,a)=(" << s->time << "," << s->capacitance << "," << s->area << ")" << endl;
        s = s->next;
    }
    #endif
}

void Van::deleteBufferLocation(BufferNode *location) {
    if(location){
        location->count--;
        location_head_ = location;
        BufferNode *curr = location;
        curr->next = nullptr;
        do {
            if(location_head_->count==0){
                if(location_head_->left){
                    location_head_->left->count--;
                    curr->next = location_head_->left;
                    curr = curr->next;
                }
                if(location_head_->right){
                    location_head_->right->count--;
                    curr->next = location_head_->right;
                    curr = curr->next;
                }
                curr->next = nullptr;
                BufferNode *head_node = location_head_->next;
                delete location_head_;
                location_head_ = head_node;
            }else{
                location_head_ = location_head_->next;
            }
        } while(location_head_);
    }
}

template<typename T> void Van::deleteSolution(T *solution) {
    deleteBufferLocation(solution->buffer_location);
    delete solution;
}

void Van::deleteSolutions(VanNode *solutions) {
    VanNode *solution = solutions;
    VanNode *next = nullptr;
    while(solution) {
        next = solution->next;
        deleteSolution(solution);
        solution = next;
    }
}

#if DEBUG_VAN
void Van::printBuffer(BufferNode *root) {
    if ( root == nullptr) {
        return;
    }
    if ( root->bufferType > -1 ) {
        cout << "buffer " << root->bufferType << " at id:from " << root->id_at << " to " << root->id_to << endl;
    }
    printBuffer(root->left);
    printBuffer(root->right);
}
#endif

}//namespace opt
}//namespace open_edi