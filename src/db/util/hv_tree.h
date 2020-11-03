/* @file  hv_tree.h
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
/**
 * @file   hv_tree.h
 * @date   July 2020
 */

#ifndef EDI_UTIL_HV_TREE_H_
#define EDI_UTIL_HV_TREE_H_

#include <iostream>
#include <vector>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

#include "util/namespace.h"
#include "db/util/box.h"
#include "db/core/object.h"

namespace open_edi {
namespace db {

#define HV_TREE_SPLIT_THRESHOLD 32

enum HVTreeCutDir {
  kUndefined, kHorizontal, kVertical
};

enum HVMid {
  kBelowMid,
  kOnMid,
  kAboveMid
};

enum HVTreeTraversal {
  kPreOrder, kInOrder, kPostOrder
};

// type T must inherit Object before using HVTree
template <typename T>
ObjectType getObjType(T *obj) {
  Object *local_obj = (reinterpret_cast<Object *>(obj));
  return local_obj->getObjectType();
}

// add more types and corresponding box APIs when using HVTree
template <typename T>
Box getObjBox(T *obj) {
  ObjectType obj_type = getObjType(obj);
  switch (obj_type) {
  case kObjectTypeWire:
    // Wire *wire = (reinterpret_cast<Wire *>(obj));
    // return wire->getBox();
  case kObjectTypeVia:
    // Via *via = (reinterpret_cast<Via *>(obj));
    // return via->getBBox();
  default:
    Box *box = (reinterpret_cast<Box *>(obj));
      return *box;
    break;
  }
  return Box(0, 0, 0, 0);
}

template <typename T>
class HVCutNode {
 public:
  HVCutNode<T>();
  ~HVCutNode<T>();

  // get & set
  bool getIsLeaf();
  void setIsLeaf(bool is_leaf);
  int getMid();
  void setMid(int mid);
  int getBoxNum();
  void setBoxNum(int num);
  HVTreeCutDir getDir();
  void setDir(HVTreeCutDir dir);
  void setThreshold(int threshold);
  int getThreshold();
  HVCutNode<T> *getLeft();
  void setLeft(HVCutNode<T> *left);
  HVCutNode<T> *getRight();
  void setRight(HVCutNode<T> *right);

  // main operation
  void insert(T *obj);
  void split(T *obj);
  bool remove(T *obj);
  void removeAll();
  void merge();
  int getDepth();
  void search(const Box &search_box, std::vector<T *> *search_result);
  int destroy();
  void traverse(HVTreeTraversal order = kPreOrder);

  // support functions
  HVMid checkMid(const Box &box);
  int calculateMid();

 private:
  bool is_leaf_;
  int mid_;
  int box_num_;
  Box bbox_;
  HVTreeCutDir dir_;
  int threshold_;
  std::vector<T *> boxes_;
  HVCutNode<T> *left_cut_;
  HVCutNode<T> *right_cut_;
};

template <typename T>
class HVTreeNode {
 public:
  HVTreeNode<T>();
  ~HVTreeNode<T>();

  // get & set
  bool getIsLeaf();
  void setIsLeaf(bool is_leaf);
  int getMid();
  void setMid(int mid);
  int getBoxNum();
  void setBoxNum(int num);
  HVTreeCutDir getDir();
  void setDir(HVTreeCutDir dir);
  void setThreshold(int threshold);
  int getThreshold();
  HVCutNode<T> *getCut();
  void setCut(HVCutNode<T> *cut);
  HVTreeNode<T> *getLeft();
  void setLeft(HVTreeNode<T> *left);
  HVTreeNode<T> *getRight();
  void setRight(HVTreeNode<T> *right);

  // main operations
  void insert(T *obj);
  void split(T *obj);
  bool remove(T *obj);
  void removeAll();
  void merge();
  int getDepth();
  void search(const Box &search_box, std::vector<T *> *search_result);
  void traverse(HVTreeTraversal order = kPreOrder);

  // support functions
  void setDefaultDir();
  HVMid checkMid(const Box &box);
  int calculateMid();
  HVTreeCutDir getOppositeDir();

 private:
  bool is_leaf_;
  int mid_;
  int box_num_;
  Box bbox_;
  HVTreeCutDir dir_;
  int threshold_;
  // may use union for cut_tree or box_list
  std::vector<T *> boxes_;
  HVCutNode<T> *cut_tree_;
  HVTreeNode<T> *left_tree_;
  HVTreeNode<T> *right_tree_;
};

// To be enhanced
// 1. template, t->getBox, filter
// 2. multi-thread(more efficient)
// 3. follow open-edi coding style
// 4. check duplicate (by type t->getType(), filter t->is_duplicate())
// 5. no need to do merge, store generic obj
// 6. cut dir, by routing direction
// 7. guide mid(preset or offset, or by design if it is PG)
// 8. iterator (query), whole tree iterator, query iterator
// 9. search return value (memory alloc and free), query
// 10. root rotate, reconstruct, update node information
template <typename T>
class HVTree {
 public:
  HVTree();
  ~HVTree();

  void setCutDir(HVTreeCutDir cut_dir);
  void setThreshold(int threshold);
  void insert(T *obj);
  void remove(T *obj);
  void removeAll();
  void traverse(HVTreeTraversal order = kPreOrder);
  void search(const Box &search_box, std::vector<T *> *search_result);
  // void strictSearch(Box &search_box, std::vector<T *> &search_result);
  // void firstSearch(Box &search_box, std::vector<T *> &search_result);

  // read/write lock
  void rdlock();
  void wrlock();
  void unlock();

 private:
  HVTreeNode<T> root_;
  pthread_rwlock_t hv_tree_rwlock_;
};

// HVCutNode

template <typename T>
HVCutNode<T>::HVCutNode() {
  is_leaf_ = true;
  mid_ = 0;
  box_num_ = 0;
  bbox_.setBox(0, 0, 0, 0);
  dir_ = kUndefined;
  threshold_ = HV_TREE_SPLIT_THRESHOLD;
  left_cut_ = 0;
  right_cut_ = 0;
}

template <typename T>
HVCutNode<T>::~HVCutNode() {
  removeAll();
}

template <typename T>
bool HVCutNode<T>::getIsLeaf() {
  return is_leaf_;
}

template <typename T>
void HVCutNode<T>::setIsLeaf(bool is_leaf) {
  is_leaf_ = is_leaf;
}

template <typename T>
int HVCutNode<T>::getMid() {
  return mid_;
}

template <typename T>
void HVCutNode<T>::setMid(int mid) {
  mid_ = mid;
}

template <typename T>
int HVCutNode<T>::getBoxNum() {
  return box_num_;
}

template <typename T>
void HVCutNode<T>::setBoxNum(int num) {
  box_num_ = num;
}

template <typename T>
HVTreeCutDir HVCutNode<T>::getDir() {
  return dir_;
}

template <typename T>
void HVCutNode<T>::setDir(HVTreeCutDir dir) {
  assert(dir != kUndefined);
  dir_ = dir;
}

template <typename T>
void HVCutNode<T>::setThreshold(int threshold) {
  threshold_ = threshold;
}

template <typename T>
int HVCutNode<T>::getThreshold() {
  return threshold_;
}

template <typename T>
HVCutNode<T> *HVCutNode<T>::getLeft() {
  return left_cut_;
}

template <typename T>
void HVCutNode<T>::setLeft(HVCutNode<T> *left) {
  left_cut_ = left;
}

template <typename T>
HVCutNode<T> *HVCutNode<T>::getRight() {
  return right_cut_;
}

template <typename T>
void HVCutNode<T>::setRight(HVCutNode<T> *right) {
  right_cut_ = right;
}

template <typename T>
void HVCutNode<T>::insert(T *obj) {
  Box box = getObjBox(obj);
  if (getIsLeaf()) {
    if (box_num_ < threshold_) {
      box_num_++;
      boxes_.push_back(obj);
      bbox_.maxBox(box);
    } else {
      split(obj);
    }
    return;
  }

  // if not leaf, insert into list or left/right cut tree
  HVMid mid_type = checkMid(box);
  switch (mid_type) {
  case kOnMid:
    box_num_++;
    boxes_.push_back(obj);
    bbox_.maxBox(box);
    break;
  case kBelowMid:
    if (!getLeft()) {
      // bad, split should balance left/right tree
#if HV_TREE_DEBUG
      // do some debug and try to do balance
#endif
      HVCutNode<T> *new_cut_node = new HVCutNode<T>;
      new_cut_node->setDir(getDir());
      new_cut_node->setThreshold(getThreshold());
      setLeft(new_cut_node);
    }
    getLeft()->insert(obj);
    break;
  case kAboveMid:
    if (!getRight()) {
      // bad, split should balance left/right tree
#if HV_TREE_DEBUG
      // do some debug and try to do balance
#endif
      HVCutNode<T> *new_cut_node = new HVCutNode<T>;
      new_cut_node->setDir(getDir());
      new_cut_node->setThreshold(getThreshold());
      setRight(new_cut_node);
    }
    getRight()->insert(obj);
    break;
  default:
    assert(0);
  }
}

template <typename T>
void HVCutNode<T>::split(T *obj) {
  assert(getIsLeaf());
  // calcluate mid and divide boxes into mid/left/right
  Box box = getObjBox(obj);
  bbox_.maxBox(box);
  boxes_.push_back(obj);
  int mid = calculateMid();
  assert(mid != 0);
  setMid(mid);
  std::vector<T *> left_boxes;
  std::vector<T *> mid_boxes;
  std::vector<T *> right_boxes;
  int box_num = static_cast<int>(boxes_.size());
  for (int i = 0; i < box_num; i++) {
    Box box_i = getObjBox(boxes_[i]);
    HVMid mid_type = checkMid(box_i);
    switch (mid_type) {
    case kOnMid:
      mid_boxes.push_back(boxes_[i]);
      break;
    case kBelowMid:
      left_boxes.push_back(boxes_[i]);
      break;
    case kAboveMid:
      right_boxes.push_back(boxes_[i]);
      break;
    default:
      assert(0);
    }
  }

  // not leaf node now, reset some members except mid and dir
  setIsLeaf(false);
  setBoxNum(0);
  bbox_.setBox(0, 0, 0, 0);
  std::vector<T *>().swap(boxes_);
  // add into mid if there are
  int mid_box_num = static_cast<int>(mid_boxes.size());
  for (int i = 0; i < mid_box_num; i++) {
    this->insert(mid_boxes[i]);
  }
  if (left_boxes.size() > 0) {
    HVCutNode<T> *new_cut_node = new HVCutNode<T>;
    new_cut_node->setDir(getDir());
    new_cut_node->setThreshold(getThreshold());
    // leaf node does not need to set mid until splitted
    int left_box_num = static_cast<int>(left_boxes.size());
    for (int i = 0; i < left_box_num; i++) {
      new_cut_node->insert(left_boxes[i]);
    }
    setLeft(new_cut_node);
  }
  if (right_boxes.size() > 0) {
    HVCutNode<T> *new_cut_node = new HVCutNode<T>;
    new_cut_node->setDir(getDir());
    new_cut_node->setThreshold(getThreshold());
    // leaf node does not need to set mid until splitted
    int right_box_num = static_cast<int>(right_boxes.size());
    for (int i = 0; i < right_box_num; i++) {
      new_cut_node->insert(right_boxes[i]);
    }
    setRight(new_cut_node);
  }
}

template <typename T>
bool HVCutNode<T>::remove(T *obj) {
  bool is_removed = false;
  unsigned int box_size = boxes_.size();
  if (box_size > 0) {
    unsigned int k = 0;
    for (unsigned int i = 0; i < box_size; i++) {
      if (boxes_[i] == obj) {
        is_removed = true;
        k = i;
        break;
      }
    }
    if (is_removed) {
      boxes_[k] = boxes_[box_size - 1];
      boxes_.pop_back();
      box_num_--;
      bbox_.setBox(0, 0, 0, 0);
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        bbox_.maxBox(box_i);
      }
    }
  }
  if (!is_removed && left_cut_) {
    is_removed = left_cut_->remove(obj);
  }
  if (!is_removed && right_cut_) {
    is_removed = right_cut_->remove(obj);
  }
  if (left_cut_ &&
      left_cut_->getIsLeaf() &&
      left_cut_->getBoxNum() <= 0) {
    delete left_cut_;
    setLeft(0);
  }
  if (right_cut_ &&
      right_cut_->getIsLeaf() &&
      right_cut_->getBoxNum() <= 0) {
    delete right_cut_;
    setRight(0);
  }
  if (!getLeft() && !getRight()) {
    setIsLeaf(true);
    setMid(0);
    // dir is not changed
  }

  return is_removed;
}

template <typename T>
void HVCutNode<T>::removeAll() {
  std::vector<T *>().swap(boxes_);
  // boxes_.swap(std::vector<Box>());
  if (getLeft()) {
    delete getLeft();
    setLeft(0);
  }
  if (getRight()) {
    delete getRight();
    setRight(0);
  }
}

template <typename T>
void HVCutNode<T>::traverse(HVTreeTraversal order) {
  if (getIsLeaf()) {
    printf("Cut Leaf : ");
    for (unsigned int i = 0; i < boxes_.size(); i++) {
      Box box_i = getObjBox(boxes_[i]);
      printf("(%d %d %d %d) ", box_i.getLLX(), box_i.getLLY(),
             box_i.getURX(), box_i.getURY());
    }
    printf("\n");
    return;
  }
  if (order == kPreOrder) {
    if (boxes_.size() > 0) {
      printf("Cut Mid : ");
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        printf("(%d %d %d %d) ", box_i.getLLX(), box_i.getLLY(),
               box_i.getURX(), box_i.getURY());
      }
      printf("\n");
    }
    if (left_cut_) {
      left_cut_->traverse(order);
    }
    if (right_cut_) {
      right_cut_->traverse(order);
    }
  } else if (order == kInOrder) {
    if (left_cut_) {
      left_cut_->traverse(order);
    }
    if (boxes_.size() > 0) {
      printf("Cut Mid : ");
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        printf("(%d %d %d %d) ", box_i.getLLX(), box_i.getLLY(),
               box_i.getURX(), box_i.getURY());
      }
      printf("\n");
    }
    if (right_cut_) {
      right_cut_->traverse(order);
    }
  } else {
    if (left_cut_) {
      left_cut_->traverse(order);
    }
    if (right_cut_) {
      right_cut_->traverse(order);
    }
    if (boxes_.size() > 0) {
      printf("Cut Mid : ");
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        printf("(%d %d %d %d) ", box_i.getLLX(), box_i.getLLY(),
               box_i.getURX(), box_i.getURY());
      }
      printf("\n");
    }
  }
}

template <typename T>
void HVCutNode<T>::search(const Box &search_box,
                          std::vector<T *> *search_result) {
  if (box_num_ > 0 && bbox_.isIntersect(search_box)) {
    for (unsigned int i = 0; i < boxes_.size(); i++) {
      Box box_i = getObjBox(boxes_[i]);
      if (search_box.isIntersect(box_i)) {
        search_result->push_back(boxes_[i]);
      }
    }
  }
  HVMid mid_type = checkMid(search_box);
  switch (mid_type) {
  case kOnMid:
    if (getLeft()) {
      getLeft()->search(search_box, search_result);
    }
    if (getRight()) {
      getRight()->search(search_box, search_result);
    }
    break;
  case kBelowMid:
    if (getLeft()) {
      getLeft()->search(search_box, search_result);
    }
    break;
  case kAboveMid:
    if (getRight()) {
      getRight()->search(search_box, search_result);
    }
    break;
  default:
    assert(0);
  }
}

// check if the box is on the cut(mid)
template <typename T>
HVMid HVCutNode<T>::checkMid(const Box &box) {
  switch (getDir()) {
  case kHorizontal:
    if (box.getLLY() <= getMid() && box.getURY() >= getMid()) {
      return kOnMid;
    } else if (box.getURY() < getMid()) {
      return kBelowMid;
    } else if (box.getLLY() > getMid()) {
      return kAboveMid;
    }
    break;
  case kVertical:
    if (box.getLLX() <= getMid() && box.getURX() >= getMid()) {
      return kOnMid;
    } else if (box.getURX() < getMid()) {
      return kBelowMid;
    } else if (box.getLLX() > getMid()) {
      return kAboveMid;
    }
    break;
  default:
    return kOnMid;
  }
  return kOnMid;
}

template <typename T>
// use bounding box's middle line as the cut mid
int HVCutNode<T>::calculateMid() {
  switch (getDir()) {
  case kHorizontal:
    return ((bbox_.getLLY() + bbox_.getURY()) / 2);
  case kVertical:
    return ((bbox_.getLLX() + bbox_.getURX()) / 2);
  default:
    return 0;
  }
  return 0;
}

// end HVCutNode

// HVTreeNode

template <typename T>
HVTreeNode<T>::HVTreeNode() {
  is_leaf_ = true;
  mid_ = 0;
  box_num_ = 0;
  bbox_.setBox(0, 0, 0, 0);
  dir_ = kUndefined;
  threshold_ = HV_TREE_SPLIT_THRESHOLD;
  cut_tree_ = 0;
  left_tree_ = 0;
  right_tree_ = 0;
}

template <typename T>
HVTreeNode<T>::~HVTreeNode() {
  removeAll();
}

template <typename T>
bool HVTreeNode<T>::getIsLeaf() {
  return is_leaf_;
}

template <typename T>
void HVTreeNode<T>::setIsLeaf(bool is_leaf) {
  is_leaf_ = is_leaf;
}

template <typename T>
int HVTreeNode<T>::getMid() {
  return mid_;
}

template <typename T>
void HVTreeNode<T>::setMid(int mid) {
  mid_ = mid;
}

template <typename T>
int HVTreeNode<T>::getBoxNum() {
  return box_num_;
}

template <typename T>
void HVTreeNode<T>::setBoxNum(int num) {
  box_num_ = num;
}

template <typename T>
HVTreeCutDir HVTreeNode<T>::getDir() {
  return dir_;
}

template <typename T>
void HVTreeNode<T>::setDir(HVTreeCutDir dir) {
  assert(dir != kUndefined);
  dir_ = dir;
}

template <typename T>
void HVTreeNode<T>::setThreshold(int threshold) {
  threshold_ = threshold;
}

template <typename T>
int HVTreeNode<T>::getThreshold() {
  return threshold_;
}

template <typename T>
HVCutNode<T> *HVTreeNode<T>::getCut() {
  return cut_tree_;
}

template <typename T>
void HVTreeNode<T>::setCut(HVCutNode<T> *cut_tree) {
  cut_tree_ = cut_tree;
}

template <typename T>
HVTreeNode<T> *HVTreeNode<T>::getLeft() {
  return left_tree_;
}

template <typename T>
void HVTreeNode<T>::setLeft(HVTreeNode<T> *left) {
  left_tree_ = left;
}

template <typename T>
HVTreeNode<T> *HVTreeNode<T>::getRight() {
  return right_tree_;
}

template <typename T>
void HVTreeNode<T>::setRight(HVTreeNode<T> *right) {
  right_tree_ = right;
}

template <typename T>
void HVTreeNode<T>::insert(T *obj) {
  Box box = getObjBox(obj);
  if (getIsLeaf()) {
    if (box_num_ < threshold_) {
      box_num_++;
      boxes_.push_back(obj);
      bbox_.maxBox(box);
    } else {
      split(obj);
    }
    return;
  }

  // if not leaf, insert into cut/left/right tree
  HVMid mid_type = checkMid(box);
  switch (mid_type) {
  case kOnMid:
    if (!getCut()) {
      HVCutNode<T> *new_cut_node = new HVCutNode<T>;
      new_cut_node->setDir(getOppositeDir());
      new_cut_node->setThreshold(getThreshold());
      setCut(new_cut_node);
    }
    getCut()->insert(obj);
    break;
  case kBelowMid:
    if (!getLeft()) {
      // bad, split should balance left/right tree
#if HV_TREE_DEBUG
      // do some debug and try to do balance
#endif
      HVTreeNode<T> *new_tree_node = new HVTreeNode<T>;
      new_tree_node->setDir(getOppositeDir());
      new_tree_node->setThreshold(getThreshold());
      setLeft(new_tree_node);
    }
    getLeft()->insert(obj);
    break;
  case kAboveMid:
    if (!getRight()) {
      // bad, split should balance left/right tree
#if HV_TREE_DEBUG
      // do some debug and try to do balance
#endif
      HVTreeNode<T> *new_tree_node = new HVTreeNode<T>;
      new_tree_node->setDir(getOppositeDir());
      new_tree_node->setThreshold(getThreshold());
      setRight(new_tree_node);
    }
    getRight()->insert(obj);
    break;
  default:
    assert(0);
  }
}

template <typename T>
void HVTreeNode<T>::split(T *obj) {
  assert(getIsLeaf());
  setDefaultDir();
  // calcluate mid and divide boxes into mid/left/right
  Box box = getObjBox(obj);
  bbox_.maxBox(box);
  boxes_.push_back(obj);
  int mid = calculateMid();
  assert(mid != 0);
  setMid(mid);
  std::vector<T *> left_boxes;
  std::vector<T *> mid_boxes;
  std::vector<T *> right_boxes;
  int box_num = static_cast<int>(boxes_.size());
  for (int i = 0; i < box_num; i++) {
    Box box_i = getObjBox(boxes_[i]);
    HVMid mid_type = checkMid(box_i);
    switch (mid_type) {
    case kOnMid:
      mid_boxes.push_back(boxes_[i]);
      break;
    case kBelowMid:
      left_boxes.push_back(boxes_[i]);
      break;
    case kAboveMid:
      right_boxes.push_back(boxes_[i]);
      break;
    default:
      assert(0);
    }
  }

  // not leaf node now, reset some members except mid and dir
  setIsLeaf(false);
  setBoxNum(0);
  bbox_.setBox(0, 0, 0, 0);
  std::vector<T *>().swap(boxes_);
  if (mid_boxes.size() > 0) {
    HVCutNode<T> *new_cut_node = new HVCutNode<T>;
    new_cut_node->setDir(getOppositeDir());
    new_cut_node->setThreshold(getThreshold());
    setCut(new_cut_node);
    int mid_box_num = static_cast<int>(mid_boxes.size());
    for (int i = 0; i < mid_box_num; i++) {
      new_cut_node->insert(mid_boxes[i]);
    }
  }
  if (left_boxes.size() > 0) {
    HVTreeNode<T> *new_tree_node = new HVTreeNode<T>;
    new_tree_node->setDir(getOppositeDir());
    new_tree_node->setThreshold(getThreshold());
    // leaf node does not need to set mid until splitted
    int left_box_num = static_cast<int>(left_boxes.size());
    for (int i = 0; i < left_box_num; i++) {
      new_tree_node->insert(left_boxes[i]);
    }
    setLeft(new_tree_node);
  }
  if (right_boxes.size() > 0) {
    HVTreeNode<T> *new_tree_node = new HVTreeNode<T>;
    new_tree_node->setDir(getOppositeDir());
    new_tree_node->setThreshold(getThreshold());
    // leaf node does not need to set mid until splitted
    int right_box_num = static_cast<int>(right_boxes.size());
    for (int i = 0; i < right_box_num; i++) {
      new_tree_node->insert(right_boxes[i]);
    }
    setRight(new_tree_node);
  }
}

template <typename T>
bool HVTreeNode<T>::remove(T *obj) {
  bool is_removed = false;
  if (getIsLeaf()) {
    unsigned int k = 0;
    unsigned int box_size = boxes_.size();
    for (unsigned int i = 0; i < box_size; i++) {
      if (boxes_[i] == obj) {
        is_removed = true;
        k = i;
        break;
      }
    }
    if (is_removed) {
      boxes_[k] = boxes_[box_size - 1];
      boxes_.pop_back();
      box_num_--;
      bbox_.setBox(0, 0, 0, 0);
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        bbox_.maxBox(box_i);
      }
    }
    return is_removed;
  }
  if (cut_tree_) {
    is_removed = cut_tree_->remove(obj);
  }
  if (!is_removed && left_tree_) {
    is_removed = left_tree_->remove(obj);
  }
  if (!is_removed && right_tree_) {
    is_removed = right_tree_->remove(obj);
  }
  if (cut_tree_ &&
      cut_tree_->getIsLeaf() &&
      cut_tree_->getBoxNum() <= 0) {
    delete cut_tree_;
    setCut(0);
  }
  if (left_tree_ &&
      left_tree_->getIsLeaf() &&
      left_tree_->getBoxNum() <= 0) {
    delete left_tree_;
    setLeft(0);
  }
  if (right_tree_ &&
      right_tree_->getIsLeaf() &&
      right_tree_->getBoxNum() <= 0) {
    delete right_tree_;
    setRight(0);
  }
  if (!getCut() && !getLeft() && !getRight()) {
    setIsLeaf(true);
    setMid(0);
    // dir is not changed
  }

  return is_removed;
}

template <typename T>
void HVTreeNode<T>::removeAll() {
  std::vector<T *>().swap(boxes_);
  // boxes_.swap(std::vector<Box>());
  if (getCut()) {
    delete getCut();
    setCut(0);
  }
  if (getLeft()) {
    delete getLeft();
    setLeft(0);
  }
  if (getRight()) {
    delete getRight();
    setRight(0);
  }
}

template <typename T>
void HVTreeNode<T>::traverse(HVTreeTraversal order) {
  if (getIsLeaf()) {
    printf("Tree Leaf : ");
    for (unsigned int i = 0; i < boxes_.size(); i++) {
      Box box_i = getObjBox(boxes_[i]);
      printf("(%d %d %d %d) ", box_i.getLLX(), box_i.getLLY(),
             box_i.getURX(), box_i.getURY());
    }
    printf("\n");
    return;
  }
  if (order == kPreOrder) {
    if (cut_tree_) {
      cut_tree_->traverse(order);
    }
    if (left_tree_) {
      left_tree_->traverse(order);
    }
    if (right_tree_) {
      right_tree_->traverse(order);
    }
  } else if (order == kInOrder) {
    if (left_tree_) {
      left_tree_->traverse(order);
    }
    if (cut_tree_) {
      cut_tree_->traverse(order);
    }
    if (right_tree_) {
      right_tree_->traverse(order);
    }
  } else {
    if (left_tree_) {
      left_tree_->traverse(order);
    }
    if (right_tree_) {
      right_tree_->traverse(order);
    }
    if (cut_tree_) {
      cut_tree_->traverse(order);
    }
  }
}

template <typename T>
void HVTreeNode<T>::search(const Box &search_box,
                           std::vector<T *> *search_result) {
  if (getIsLeaf()) {
    if (bbox_.isIntersect(search_box)) {
      for (unsigned int i = 0; i < boxes_.size(); i++) {
        Box box_i = getObjBox(boxes_[i]);
        if (search_box.isIntersect(box_i)) {
          search_result->push_back(boxes_[i]);
        }
      }
    }
    return;
  }
  // cut must be searched without checking search_box is on mid or not
  if (getCut()) {
    getCut()->search(search_box, search_result);
  }
  HVMid mid_type = checkMid(search_box);
  switch (mid_type) {
  case kOnMid:
    if (getLeft()) {
      getLeft()->search(search_box, search_result);
    }
    if (getRight()) {
      getRight()->search(search_box, search_result);
    }
    break;
  case kBelowMid:
    if (getLeft()) {
      getLeft()->search(search_box, search_result);
    }
    break;
  case kAboveMid:
    if (getRight()) {
      getRight()->search(search_box, search_result);
    }
    break;
  default:
    assert(0);
  }
}

template <typename T>
// check if the box is on the cut(mid)
HVMid HVTreeNode<T>::checkMid(const Box &box) {
  switch (getDir()) {
  case kHorizontal:
    if (box.getLLY() <= getMid() && box.getURY() >= getMid()) {
      return kOnMid;
    } else if (box.getURY() < getMid()) {
      return kBelowMid;
    } else if (box.getLLY() > getMid()) {
      return kAboveMid;
    }
    break;
  case kVertical:
    if (box.getLLX() <= getMid() && box.getURX() >= getMid()) {
      return kOnMid;
    } else if (box.getURX() < getMid()) {
      return kBelowMid;
    } else if (box.getLLX() > getMid()) {
      return kAboveMid;
    }
    break;
  default:
    return kOnMid;
  }
  return kOnMid;
}

template <typename T>
// use bounding box's middle line as the cut mid
int HVTreeNode<T>::calculateMid() {
  switch (getDir()) {
  case kHorizontal:
    return ((bbox_.getLLY() + bbox_.getURY()) / 2);
  case kVertical:
    return ((bbox_.getLLX() + bbox_.getURX()) / 2);
  default:
    return 0;
  }
  return 0;
}

template <typename T>
// cut/left/right tree dir is opposite to current tree dir
HVTreeCutDir HVTreeNode<T>::getOppositeDir() {
  assert(dir_ != kUndefined);
  switch (getDir()) {
  case kHorizontal:
    return kVertical;
  case kVertical:
    return kHorizontal;
  default:
    return kUndefined;
  }
  return kUndefined;
}

template <typename T>
void HVTreeNode<T>::setDefaultDir() {
  if (dir_ != kUndefined) {
    return;
  }
  int num_long_width = 0;
  int num_long_height = 0;
  for (unsigned int i = 0; i < boxes_.size(); i++) {
    Box box_i = getObjBox(boxes_[i]);
    if (box_i.getWidth() >= box_i.getHeight()) {
      num_long_width++;
    } else {
      num_long_height++;
    }
  }
  if (num_long_width >= num_long_height) {
    setDir(kVertical);
  } else {
    setDir(kHorizontal);
  }
}

// end HVTreeNode


// HVTree

template <typename T>
HVTree<T>::HVTree() {
  pthread_rwlock_init(&hv_tree_rwlock_, NULL);
}

template <typename T>
HVTree<T>::~HVTree() {
  pthread_rwlock_destroy(&hv_tree_rwlock_);
}


template <typename T>
void HVTree<T>::setCutDir(HVTreeCutDir cut_dir) {
  assert(cut_dir != kUndefined);
  root_.setDir(cut_dir);
}

template <typename T>
void HVTree<T>::setThreshold(int threshold) {
  root_.setThreshold(threshold);
}

template <typename T>
void HVTree<T>::insert(T *obj) {
  wrlock();
  root_.insert(obj);
  unlock();
}

// no need to check shreshold and merge after removing one obj
// but leaf needs to be set for removing node
template <typename T>
void HVTree<T>::remove(T *obj) {
  wrlock();
  root_.remove(obj);
  unlock();
}

template <typename T>
void HVTree<T>::removeAll() {
  wrlock();
  root_.removeAll();
  unlock();
}

template <typename T>
void HVTree<T>::traverse(HVTreeTraversal order) {
  rdlock();
  root_.traverse(order);
  unlock();
}

template <typename T>
void HVTree<T>::search(const Box &search_box,
                       std::vector<T *> *search_result) {
  rdlock();
  root_.search(search_box, search_result);
  unlock();
}

template <typename T>
void HVTree<T>::rdlock() {
  // check if it is single thread not to do lock
  pthread_rwlock_rdlock(&hv_tree_rwlock_);
}

template <typename T>
void HVTree<T>::wrlock() {
  // check if it is single thread not to do lock
  pthread_rwlock_wrlock(&hv_tree_rwlock_);
}

template <typename T>
void HVTree<T>::unlock() {
  // check if it is single thread not to do lock
  pthread_rwlock_unlock(&hv_tree_rwlock_);
}

// end HVTree

}  // namespace db 
}  // namespace open_edi

#endif  // EDI_UTIL_HV_TREE_H_
