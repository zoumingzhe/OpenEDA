#include <cmath>

#include "segmentation.h"
#include "io.h"


#define DEBUG_SEG 0

#define MAX_SEG_NUM_PER_WIRE 0

namespace open_edi {
namespace opt {

Segmentation::Segmentation() {
    nodes_array_seg.clear();
    max_id = -1;
}

Segmentation::~Segmentation() {
    nodes_array_seg.clear();
    max_id = -1;
}

std::vector<Node *> Segmentation::getWireSegmentationRet() {
    return nodes_array_seg;
}

double Segmentation::distance(Node *a, Node *b) {
    return (fabs(a->x - b->x) + fabs(a->y - b->y));
}

double Segmentation::computeK(double r,
		              double c,
			      double l,
			      double rb,
			      double cb,
			      double kb,
			      double r_source,
			      double c_down) {
    double numerator = 2.0 * pow(r * c * l + r * (cb - c_down) - c * (rb - r_source), 2);
    double denominator = r * c * (rb * cb + kb);
    return (-0.5 + sqrt(1.0 + numerator / denominator));
}

int Segmentation::computeWireSegmentation(std::vector<Node *> nodes_array_input,
		                          int max_node_id,
					  const std::vector<Buffer> &buffer_libs,
					  const std::vector<Buffer> &driver_libs) {
    max_id = std::max(max_id, max_node_id);
    if (nodes_array_input.size() == 0) {
        return 1;
    }
#if DEBUG_SEG
    Node * temp;
    int parent, child;
    for (int i = 0; i < nodes_array_input.size(); i++) {
        temp = nodes_array_input[i];
	if (!temp->parent || temp->parent->id < 0) {
	    parent = -1;
	}
	else {
            parent = temp->parent->id;
	}
	if (temp->children.size() > 0) {
	    child = temp->children[0]->id;
	}
	else {
	    child = -1;
	}
	std::cout<< "## raw: id,x,y,type,parent,child,r0,c0,cDown,cEdge,rEdge:  " << temp->id << " , " << temp->x << " , " << temp->y << " , " << temp->type << " , " << parent << " , " << child << " , " << temp->r0 << " , " << temp->c0 << " , " << temp->c_down << " , " << temp->c_edge << " , " << temp->r_edge << std::endl; 
    }
#endif

    nodes_array_seg.swap(nodes_array_input);
    Node *child_node, *parent_node;
    int cur_max_node_id = max_node_id;
    /* for each node, compute c_down */
    for (int i = nodes_array_seg.size() - 1; i > 0; i--) {
	child_node = nodes_array_seg[i];
	parent_node = child_node->parent;
	parent_node->c_down += (child_node->c_down + child_node->c_edge);
    }
    /* for each wire, do segmentation and update to node's seg_node_array */
    double r = 0.0, c = 0.0, l = 0.0, rb = 0.0, cb = 0.0, kb = 0.0, r_source = 0.0, c_down = 0.0;
    double r_edge = 0.0, c_edge = 0.0;
    int j = 0, h = 0, k = 0, k1 = 0, k2 = 0;
    Node *cur_node, *pre_node;
    for (int i = nodes_array_seg.size() - 1; i > 0; i--) {
        child_node = nodes_array_seg[i];
	parent_node = child_node->parent;
        l = distance(parent_node, child_node);
	if (l < 1e-6) {
	    continue;
	}
	r = child_node->r_edge / l;
	c = child_node->c_edge / l;
	c_down = child_node->c_down;
	k = 0;
	for (j = 0; j < buffer_libs.size(); j++) {
	    rb = buffer_libs[j].resistance;
	    cb = buffer_libs[j].capacitance;
	    kb = buffer_libs[j].delay;
	    for (h = 0; h < driver_libs.size(); h++) {
		r_source = driver_libs[h].resistance;
                k1 = computeK(r, c, l, rb, cb, kb, r_source, c_down);
                k2 = computeK(r, c, l, rb, cb, kb, r_source, 0.0);
	        k = floor(std::max(k, (std::max(k1, k2))));
#if DEBUG_SEG
		std::cout<< "## k1, k2, k:  " << k1 << " , " << k2 << " , " << k << std::endl;
#endif
	    }
	}
	k = std::min(MAX_SEG_NUM_PER_WIRE, k);
	if (k == 0) {
	    continue;
	}
	// create segmentation nodes
	cur_node = child_node;
	pre_node = child_node;
	c_edge = child_node->c_edge / (k + 1);
	r_edge = child_node->r_edge / (k + 1);
	child_node->c_edge = c_edge;
	child_node->r_edge = r_edge;
	for (int i = 1; i <= k; i++) {
	    cur_node->parent = new Node();
	    pre_node = cur_node;
	    cur_node = cur_node->parent;
	    max_id++;
	    cur_node->id = max_id;
	    cur_node->x = child_node->x - i * (child_node->x - parent_node->x) / (k + 1);
	    cur_node->y = child_node->y + i * (parent_node->y - child_node->y) / (k + 1);
	    cur_node->type = CANDIDATE;
	    cur_node->children.push_back(pre_node);
	    cur_node->r0 = r;
	    cur_node->c0 = c;
	    cur_node->c_down = pre_node->c_down + c_edge;
	    cur_node->c_edge = c_edge;
	    cur_node->r_edge = r_edge;
	}
	cur_node->parent = parent_node;
	parent_node->children.clear();
	parent_node->children.push_back(cur_node);
    }
    
#if DEBUG_SEG
    for (int i = 0; i < nodes_array_seg.size(); i++) {
        temp = nodes_array_seg[i];
	if (!temp->parent || temp->parent->id < 0) {
	    parent = -1;
	}
	else {
            parent = temp->parent->id;
	}
	if (temp->children.size() > 0) {
	    child = temp->children[0]->id;
	}
	else {
	    child = -1;
	}
	std::cout<< "## seg: id,x,y,type,parent,child,r0,c0,cDown,cEdge,rEdge:  " << temp->id << " , " << temp->x << " , " << temp->y << " , " << temp->type << " , " << parent << " , " << child << " , " << temp->r0 << " , " << temp->c0 << " , " << temp->c_down << " , " << temp->c_edge << " , " << temp->r_edge << std::endl; 
    }
#endif
    return 0;
}

} // namespace opt
} // namespace open_edi
