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
                nets_level_.push_back(nets);
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
                    if(net_level_.find(net)==net_level_.end()){
                        nets.insert(net);
                        net_level_[net] = level;
                    }
                    net_nodes_[net].push_back(node);
                }else{
                    continue;
                }
            }else{
                cout << "cannot find inst:" << inst_name << endl;
            }
        }
    }
    if(nets.size()>0){
        nets_level_.push_back(nets);
    }
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
        delete p->solutions[0];
        p->solutions[0] = NULL;
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
        nodes_array.push_back(net_nodes_[net]);
    }
}

void IODB::createInstNodes(unordered_map<uint64_t,InstNode *> &inst_nodes,uint32_t level) {
    set<Net *> nets = nets_level_[level];
    for(auto &net : nets){
        for(auto &node : net_nodes_[net]){
            if(node->type == CANDIDATE) {//SINK
                Pin *pin = Object::addr<Pin>(node->id);
                if(pin==NULL){
                    cout << "error pin:" << node->id << endl;
                }
                uint64_t inst_id = pin->getInst()->getId();
                InstNode *inst_node = inst_nodes[inst_id];
                if(!inst_node){
                    inst_node = new InstNode();
                    inst_node->delay = 0;
                    inst_nodes[inst_id] = inst_node;
                }
                PinNode *pin_node = inst_node->in_pins[node->id];
                if(!pin_node){
                    pin_node = new PinNode();
                    inst_node->in_pins[node->id] = pin_node;
                }
            }else if(node->type == SOURCE) {
                Pin *pin = Object::addr<Pin>(node->id);
                uint64_t inst_id = pin->getInst()->getId();
                InstNode *inst_node = inst_nodes[inst_id];
                if(!inst_node){
                    inst_node = new InstNode();
                    inst_node->delay = 0;
                    inst_nodes[inst_id] = inst_node;
                }
                PinNode *pin_node = inst_node->out_pins[node->id];
                if(!pin_node){
                    pin_node = new PinNode();
                    inst_node->out_pins[node->id] = pin_node;
                }
            }
        }
    }
}

void IODB::printBufferSolution(BufferNode *root, ofstream &fout) {
    queue<BufferNode *> nodes;
    if(root)
        nodes.push(root);
    while(nodes.size()>0){
        BufferNode *tmp = nodes.front();
        nodes.pop();
        if (tmp->bufferType != (uint32_t)-1) {
            fout << "start " << tmp->id_at << " end " << tmp->id_to << " buffertype " << tmp->bufferType << endl;
        }
        if(tmp->left){
            nodes.push(tmp->left);
        }
        if(tmp->right){
            nodes.push(tmp->right);
        }
    }
}

void IODB::writeResult(string output_file,unordered_map<uint64_t,InstNode *> &inst_nodes) {
    ofstream fout(output_file,fstream::out);
    if ( !fout ) {
        cout << "open file '" << output_file << "' failed!" << endl;
        return;
    }
    fout << "final result:" << endl;
    set<db::Net *> nets = nets_level_.back();
    for(auto &net : nets){
        vector<Node *> nodes = net_nodes_[net];
        Node *node = nodes.back();
        Pin *pin = Object::addr<Pin>(node->id);
        uint64_t inst_id = pin->getInst()->getId();
        InstNode *inst_node = inst_nodes[inst_id];
        PinNode *pin_node = inst_node->out_pins[node->id];
        size_t num_solutions = 0;
        VanResult *solution = pin_node->solutions;
        while(solution){
            num_solutions++;
            solution = solution->next;
        }
        fout << "net " << net->getName() << " has " << num_solutions << " solutions." << endl;
        if(num_solutions>0){
            fout << "best solution:" << endl;
            printBufferSolution(pin_node->solutions->buffer_location,fout);
        }
    }
    for(int64_t i=0;i<nets_level_.size()-1;i++){
        set<db::Net *> nets = nets_level_[i];
        for(auto &net : nets){
            vector<Node *> nodes = net_nodes_[net];
            Node *node = nodes.back();
            Pin *pin = Object::addr<Pin>(node->id);
            uint64_t inst_id = pin->getInst()->getId();
            InstNode *inst_node = inst_nodes[inst_id];
            PinNode *pin_node = inst_node->out_pins[node->id];
            VanResult *s = pin_node->best_solution;
            if(s){
                fout << "net " << net->getName() << " has best solution:" << endl;
                printBufferSolution(s->buffer_location,fout);
            }
        }
    }
    fout.close();
    
    for(auto &nodes : net_nodes_){
        for(auto &node : nodes.second){
            delete node;
        }
    }
}

}//namespace opt
}//namespace open_edi
