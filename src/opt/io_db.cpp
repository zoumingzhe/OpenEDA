#include "io_db.h"

using namespace std;
using namespace open_edi::db;

namespace open_edi {
namespace opt {

IODB::IODB() {
    //
}

IODB::~IODB() {
    destroyTree();
}

void IODB::readBufferLibrary(const string file_name, vector<Buffer> &buffers) {
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

int IODB::readInputTree(string file_name, vector<Buffer> &drivers) {
    destroyTree();
    // read db
    queue<Cell *> cells;
    Cell *top_cell = getTopCell();
    if(top_cell){
        cells.push(top_cell);
    }
    while(!cells.empty()){
        top_cell = cells.front();
        cells.pop();
        // get all instances
        ArrayObject<ObjectId> *v_inst = top_cell->getInstanceArray();
        if(v_inst){
            ArrayObject<ObjectId> inst_ids = *v_inst;
            for(int64_t j=0;j<inst_ids.getSize();j++){
                ObjectId inst_id = inst_ids[j];
                Inst *inst = Object::addr<Inst>(inst_id);
                string name = inst->getName();
                if(name[0]=='\\'){
                    name.erase(0, 1);
                }
                if(name.back()==' '){
                    name.pop_back();
                }
                insts_[name] = inst;
            }
        }
        // get all cells
        ArrayObject<ObjectId> *v_cell = top_cell->getCellArray();
        if(v_cell){
            ArrayObject<ObjectId> cell_ids = *v_cell;
            for(int64_t i=0;i<cell_ids.getSize();i++){
                ObjectId cell_id = cell_ids[i];
                Cell *cell = Object::addr<Cell>(cell_id);
                cells.push(cell);
            }
        }
    }

    // read file
    fin_.open(file_name);
    if ( !fin_.is_open() ) {
        return -1;
    }
    string what;
    uint64_t level;
    r0_ = 0.02;
    c0_ = 200;
    c0_ /= 1000;//convert to pF
    drivers.emplace_back(false,1,10,200,0);
    #if DEBUG_IO
    cout << "(c,r) per unit:" << c0_ << "," << r0_ << endl;
    #endif
    set<db::Net *> nets;
    while(fin_ >> what){
        if(what == "level"){
            fin_ >> level;
            if(level>0){
                nets_level_.emplace_back(nets);
            }
            nets.clear();
        }else{
            vector<string> splits;
            size_t pos = 0;
            string delimiter = "/";
            string token;
            while ((pos = what.find(delimiter)) != std::string::npos) {
                token = what.substr(0, pos);
                splits.emplace_back(token);
                what.erase(0, pos + delimiter.length());
            }
            if(what.length()>0){
                splits.emplace_back(what);
            }
            size_t path_num = splits.size();
            if(path_num<2){
                continue;
            }
            string pin_name = splits[path_num-1];
            string inst_name = splits[path_num-2];
            int64_t i=path_num-3;
            while(i>-1){
                if(splits[i].back()=='\\'){
                    inst_name = splits[i]+"/"+inst_name;
                }else{
                    break;
                }
                i--;
            }
            inst_name.erase(remove(inst_name.begin(), inst_name.end(), '\\'), inst_name.end());
            Inst *inst = insts_[inst_name];
            if(inst){
                Pin *pin = inst->getPin(pin_name);
                if(pin){
                    Net *net = pin->getNet();
                    Node *node = new Node();
                    node->id = pin->getId();
                    node->x = (uint64_t)net%500;
                    node->y = (uint64_t)pin%500;
                    node->type = CANDIDATE;//SINK
                    node->solutions[1] = NULL;
                    VanNode *solution = new VanNode();
                    solution->area = 0;
                    solution->buffer_location = NULL;
                    solution->capacitance = (uint64_t)node%100;
                    solution->next = NULL;
                    solution->polarity = false;
                    solution->time = 0;
                    node->solutions[0] = solution;
                    nets.insert(net);
                    net_nodes_[net].emplace_back(node);
                }else{
                    continue;
                }
            }else{
                cout << "cannot find inst:" << inst_name << endl;
            }
        }
    }
    nets_level_.emplace_back(nets);
    fin_.close();

    // connect parent
    for(auto &net : net_nodes_) {
        vector<Node *> array = net.second;
        Node *p = array.back();
        for(auto &node : array){
            node->parent = p;
        }
        p->parent = NULL;
        p->type = SOURCE;
    }
    return 0;
}

void IODB::destroyTree() {
    for(auto &net : net_nodes_) {
        for(auto & node : net.second){
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
    }
    insts_.clear();
    net_nodes_.clear();
    nets_level_.clear();
}

double IODB::getR0() {
    return r0_;
}

double IODB::getC0() {
    return c0_;
}

size_t IODB::getTotalLevel() {
    return nets_level_.size();
}

void IODB::getNodesByLevel(uint32_t level, vector<vector<Node *>> &nodes_array) {
    nodes_array.clear();
    if(level>=nets_level_.size()){
        return;
    }
    set<db::Net *> nets = nets_level_[level];
    for(auto &net : nets){
        nodes_array.emplace_back(net_nodes_[net]);
    }
}

void IODB::commitBufferToDB(db::Pin *root_pin, BufferNode *b_tree_root) {
    if (b_tree_root == NULL || root_pin == NULL) {
        return;
    }
    std::unordered_map<BufferNode *, Inst *> b_node_inst_map;
    /* Step 1: delete net rooted at root pin */
    Net *original_net = root_pin->getNet();
    Object::deleteObject<Net>(original_net);
    /* Step 2: create instances of buffers and their pins */
    Inst *root_inst = root_pin->getInst();    
    ObjectId owner_id = root_inst->getOwnerId();
    Cell *owner_cell = NULL;
    if (owner_id) { 
        owner_cell = Object::addr<Cell>(owner_id);
    }
    else {
        // print warning message
        return;
    }
    queue<BufferNode *> b_node_queue;
    BufferNode *temp = NULL;
    b_node_queue.push(b_tree_root);
    while(b_node_queue.size() > 0) {
        temp = b_node_queue.front();
        b_node_queue.pop();
        if (temp->left) {
            b_node_queue.push(temp->left);
        }
        if (temp->right) {
            b_node_queue.push(temp->right);
        }
        if (temp->bufferType != -1) {
            std::string buffer_name = "BUFFER_OPT_TEST_" + std::to_string(temp->id_at);
            Inst *b_inst = owner_cell->createInstance(buffer_name);
            b_node_inst_map[temp] = b_inst;
            b_inst->setMaster("BUFFD0BWP12T"); // to do: get buffer cell name from bufferType
            Cell *b_cell = b_inst->getMaster();
            ArrayObject<ObjectId> *b_term_array_ptr = b_cell->getTermArray();
            if (!b_term_array_ptr) {
                // print error message
                return;
            }
            else {
                std::string at_pin_name = "PIN_BUFFER_AT_" + std::to_string(temp->id_at);
                std::string to_pin_name = "PIN_BUFFER_TO_" + std::to_string(temp->id_to);
                Pin *at_pin = b_inst->createInstancePin(at_pin_name);
                Pin *to_pin = b_inst->createInstancePin(to_pin_name);
                ArrayObject<ObjectId> b_term_array = *b_term_array_ptr;
                for(int64_t i = 0; i < b_term_array.getSize(); i++) {
                    ObjectId term_id = b_term_array[i];
                    Term *b_term = Object::addr<Term>(term_id);
                    SignalDirection term_dir = b_term->getDirection();
                    if (term_dir == SignalDirection::kInput) {
                        at_pin->setTerm(b_term);
                    }
                    else if (term_dir == SignalDirection::kOutput) {
                        to_pin->setTerm(b_term);
                    }
                    else {
                        continue;
                    }
                }
            }
        }
    }
    /* Step 3: create connectivity between buffers by adding nets */
    temp = NULL;
    b_node_queue.push(b_tree_root);
    while(b_node_queue.size() > 0) {
        temp = b_node_queue.front();
        b_node_queue.pop();
        if (temp->left) {
            b_node_queue.push(temp->left);
        }
        if (temp->right) {
            b_node_queue.push(temp->right);
        }
        if (temp->bufferType != -1) {
            Inst *b_inst = b_node_inst_map[temp];
            std::string net_name = "NET_VG_" + b_inst->getName() + "_" + std::to_string(temp->id_to); 
            std::string to_pin_name = "PIN_BUFFER_TO_" + std::to_string(temp->id_to);
            Net *net = owner_cell->createNet(net_name);
            Pin *to_pin = b_inst->getPin(to_pin_name);
            to_pin->setNet(net);
            net->addPin(to_pin);
            if (temp->left) {
                __connect_b_tree_with_upstream_net(temp->left, b_node_inst_map, net);
            }
            if (temp->right) {
                __connect_b_tree_with_upstream_net(temp->right, b_node_inst_map, net);
            }
        }
    }
}

void IODB::__connect_b_tree_with_upstream_net(BufferNode *buffer_node,
                                              std::unordered_map<BufferNode *, Inst *> b_node_inst_map,
                                              Net *net) {
    // the caller should make sure buffer_node != NULL
    queue<BufferNode *> b_node_queue;
    BufferNode *temp = NULL;
    if (buffer_node->bufferType != -1) {
        Inst *b_inst = b_node_inst_map[buffer_node];
        std::string at_pin_name = "PIN_BUFFER_AT_" + std::to_string(buffer_node->id_at);
        Pin *at_pin = b_inst->getPin(at_pin_name);
        at_pin->setNet(net);
        net->addPin(at_pin);
    }
    else {
        b_node_queue.push(buffer_node);
        while (b_node_queue.size() > 0) {
            temp = b_node_queue.front();
            b_node_queue.pop();
            // b_node_queue only contains dummy buffer nodes
            if (temp->left && temp->left->bufferType == -1) {
                b_node_queue.push(temp->left);
            }
            if (temp->right && temp->right->bufferType == -1) {
                b_node_queue.push(temp->right);
            }
            // process left branch
            if (temp->left && temp->left->bufferType != -1) {
                Inst *b_inst_left = b_node_inst_map[temp->left];
                std::string at_pin_left_name = "PIN_BUFFER_AT_" + std::to_string(temp->left->id_at);
                Pin *at_pin_left = b_inst_left->getPin(at_pin_left_name);
                at_pin_left->setNet(net);
                net->addPin(at_pin_left);
            }
            // process right branch
            if (temp->right && temp->right->bufferType != -1) {
                Inst *b_inst_right = b_node_inst_map[temp->right];
                std::string at_pin_right_name = "PIN_BUFFER_AT_" + std::to_string(temp->right->id_at);
                Pin *at_pin_right = b_inst_right->getPin(at_pin_right_name);
                at_pin_right->setNet(net);
                net->addPin(at_pin_right);
            }
        }
    }
}

}//namespace opt
}//namespace open_edi
