/** 
 * @file  graph.h
 * @date  11/13/2020 11:17:24 AM CST
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include <iostream>
#include <string>
#include <stack>
#include "util/util.h"
#include "util/enums.h"
#include "db/core/object.h"
#include "db/core/cell.h"
#include "db/core/net.h"
#include "db/core/term.h"
#include "db/core/pin.h"
#include "db/core/db.h"

namespace open_edi {
namespace db {

/// @brief class GraphArc 
///
/// @tparam N
/// @tparam E
template<typename P, typename E>
class GraphArc {
  public:
    /// @brief GraphArc 
    GraphArc() {
        from_ = nullptr;
        to_ = nullptr;
        edge_ = nullptr;
    }
    /// @brief GraphArc 
    GraphArc(P from, E e, P to) {
        from_ = from;
        edge_ = e;
        to_ = to;
    }
    /// @brief ~GraphArc 
    ~GraphArc() = default;

    /// @brief setFrom 
    ///
    /// @param n
    void setFrom(P *n) {from_ = n;}
    /// @brief setTo 
    ///
    /// @param n
    void setTo(P *n) {to_ = n;}
    /// @brief setEdge 
    ///
    /// @param e
    void setEdge(E *e) {edge_ = e;}
    /// @brief getFrom 
    ///
    /// @return 
    P *getFrom() {return from_;}
    /// @brief getTo 
    ///
    /// @return 
    P *getTo() {return to_;}
    /// @brief getEdge 
    ///
    /// @return 
    E *getEdge() {return edge_;}

  private:
    P *from_;
    P *to_;
    E *edge_;
};

/// @brief class Graph
///
/// @tparam N
/// @tparam E
template<typename N, typename E>
class Graph {
  public:
    Graph() {;}
    ~Graph() {;}

    class iterator {
      public:
        /**
         * @brief Construct a new iterator object
         * 
         */
        iterator() {
            __initializeAttrs();
        }
        
        /// @brief iterator 
        ///
        /// @param cell
        iterator(Cell *cell) {
            __initializeAttrs();
            cell_ = cell;
        }

        /// @brief iterator 
        ///
        /// @param cell
        /// @param E
        iterator(Cell* cell, Net* e) {
            __initializeAttrs();
            cell_ = cell;
            edge_  = e;
            is_per_net_ = 1;
            is_all_ = 0;
        }

        /// @brief ~iterator 
        ~iterator() {
            __initializeAttrs();
            // clear mark flags: instances, nets, pins...
            if (cell_ != nullptr) {
                Inst *inst = nullptr;
                Pin *pin = nullptr;
                ArrayObject<ObjectId> *arr_ptr = nullptr, *arr_ptr1 = nullptr;
                ArrayObject<ObjectId>::iterator iter, iter1;

                arr_ptr = Object::addr<ArrayObject<ObjectId> >(cell_->getInstances());
                for (iter = arr_ptr->begin(); iter != arr_ptr->end(); iter++) {
                    inst = Object::addr<Inst>(*iter);
                    if (nullptr == inst || !inst->getIsValid()) continue;
                    inst->setIsMarked(0); //
                    arr_ptr1 = inst->getPinArray();
                    if (arr_ptr1 == nullptr) continue;
                    for (iter1 = arr_ptr1->begin(); iter1 != arr_ptr1->end(); iter1++) {
                        pin = Object::addr<Pin>(*iter);
                        if (nullptr == pin || !pin->getIsValid()) continue;
                        pin->setIsMarked(0);
                    }
                }
            }
            cell_ = nullptr;
            edge_ = nullptr;
        }

        /// @brief setLevel 
        ///
        /// @param level
        void setLevel(int32_t level) {
            level_ = level;
            is_level_limited_ = 1;
        }
        /// @brief setFromTo 
        ///
        /// @param from
        /// @param to
        void setFromTo(Pin *from, Pin *to) {
            from_ = from;
            to_   = to;
            is_p2p_ = 1;
            is_level_limited_ = 0;
            is_all_ = 0;
        }

        // begin() -- initialize edge stack (from given node stack or top)
        // ++()/++(int) -- iterate edges, build next level node stack in the meanwhile, 
        // when edge stack is empty, build next level edge stack.
        // If the Arc stack has still value, just pop value.
        // *() -- return Arcs from Arc stack.
        // end() -- once none instance is found for all edages in edge_stack_.

        /// @brief begin(), iterate from source to sink.
        ///
        /// @return 
        void begin() {
            if (is_all_) {
                __setupEdgeStack();
            }
            (*this)++;
        }
        /**
         * @brief beginReverse(), iterate from sink to source.
         * 
         */
        void beginReverse() {
            is_reverse_ = 1;

            if (is_all_) {
                __setupEdgeStack();
            }
        }
        
        /// @brief &end 
        ///
        /// @return 
        bool end() {
            return (node_stack_.empty() && arc_stack_.empty() && arc_.empty());
        }

        /// @brief operator++ 
        ///
        /// @return 
        iterator& operator++(int) {
            GraphArc<Pin,Net> *gr = nullptr;

            // 1. If arc_stack_ is not empty, just pop
            if (!arc_.empty()) {
                gr = arc_.top();
                arc_.pop();
                res_arc_.setFrom(gr->getFrom());
                res_arc_.setTo(gr->getTo());
                res_arc_.setEdge(gr->getEdge());
                is_data_ready_ = 1;
                delete gr;
            }
            if (!arc_.empty()) return *this;

            // data in arc_ stack is empty, move to next edge
            // 2. If arc_stack_ is not empty, pop & iterate edges to fill data in arc_ stack
            is_data_ready_ = 0;
            if (!arc_stack_.empty()) {
                gr = nullptr;
                gr = arc_stack_.top();
                arc_stack_.pop();
                while (gr) {
                    // 3. Fill node_stack_ while doEdge(), and push arcs into arc_
                    if (__doOneEdge(gr) || arc_stack_.empty()) {
                        break;
                    }
                    delete gr;
                    gr = nullptr;
                    gr = arc_stack_.top();
                    arc_stack_.pop();
                }
            }
            if (gr != nullptr) delete gr;

            if (arc_stack_.empty() && !node_stack_.empty()) {
                // no arc found for existing arcs, move to next level
                __setupEdgeStack();
            }
            return *this;
        }
        
        /// @brief operator 
        ///
        /// @return 
        GraphArc<Pin,Net> *const operator*() {
            if (is_data_ready_) return &res_arc_;
            return nullptr;
        }

        /**
         * @brief ready() data in iterator is ready to access.
         * 
         * @return true 
         * @return false 
         */
        bool ready() {return is_data_ready_;}
        /**
         * @brief Get current level
         * 
         * @return int32_t 
         */
        int32_t getLevel() {return level_;}
    
      private:

        bool __isSource(Pin *pin) {
            SignalDirection dir = SignalDirection::kOutput;
            assert(pin != nullptr);
            if (pin->getIsPrimary()) {
                dir = SignalDirection::kInput;
            }
            if (pin->getDirection() == dir || 
                pin->getDirection() == SignalDirection::kInout ||
                pin->getDirection() == SignalDirection::kOutputTristate) {
                return true;
            }
            return false;
        }

        bool __isSink(Pin *pin) {
            SignalDirection dir = SignalDirection::kInput;
            assert(pin != nullptr);
            if (pin->getIsPrimary()) {
                dir = SignalDirection::kOutput;
            }
            if (pin->getDirection() == dir || 
                pin->getDirection() == SignalDirection::kInout ||
                pin->getDirection() == SignalDirection::kOutputTristate) {
                return true;
            }
            return false;
        }

        bool __doOneEdge(GraphArc<Pin,Net>* gr) {
            Inst *inst = nullptr;
            ArrayObject<ObjectId>* arr_ptr = nullptr;
            GraphArc<Pin,Net>* arc = nullptr;
            Pin *pin = nullptr;
            bool status = false;

            assert(gr != nullptr);

            arr_ptr = gr->getEdge()->getPinArray();
            if (nullptr == arr_ptr) return status; 

            // iterate net pins: input, inout, tri-state then others
            // for each pin, mark instance
            for (iter_ = arr_ptr->begin(); iter_ != arr_ptr->end(); iter_++) {
                pin = nullptr;
                pin = Object::addr<Pin>(*iter_);
                if (pin && !pin->getIsMarked()) {
                    arc = nullptr;
                    if (is_reverse_) {
                        if (__isSource(pin)) {
                            arc = new GraphArc<Pin,Net>();
                            assert(arc != nullptr);
                            arc->setFrom(pin);
                            arc->setTo(gr->getTo());
                        }
                    } else if (__isSink(pin)) {
                        arc = new GraphArc<Pin,Net>();
                        assert(arc != nullptr);
                        arc->setFrom(gr->getFrom());
                        arc->setTo(pin);
                    }
                    if (nullptr != arc) {
                        arc->setEdge(gr->getEdge());
                        arc_.push(arc);
                        // TODO: 
                        //if there's tri-state pin, it might be re-visisted, but mark it for now.
                        pin->setIsMarked(1);

                        inst = nullptr;
                        inst = pin->getInst();
                        if (nullptr != inst && !inst->getIsMarked()) {
                            //assert(inst->getIsMarked() != 1); // loop
                            node_stack_.push(inst);
                            // TODO: 
                            //if there's tri-state pin, it might be re-visisted, but mark it for now.
                            inst->setIsMarked(1);
                        }
                        status = true;
                    }
                }
            }
            return status;
        }

        bool __setupEdgesByArray(ArrayObject<ObjectId>* arr_ptr) {
            Pin *pin = nullptr;
            Net *net = nullptr;
            GraphArc<Pin,Net> *arc = nullptr;
            
            for (iter_ = arr_ptr->begin(); iter_ != arr_ptr->end(); iter_++) {
                pin = nullptr;
                pin = Object::addr<Pin>(*iter_);
                if (pin && !pin->getIsMarked()) {
                    net = nullptr;
                    pin->setIsMarked(1);
                    net = pin->getNet();
                    // no need to mark nets
                    // can be re-entried
                    if (nullptr != net) {
                        arc = nullptr;
                        if (is_reverse_) {
                            if (__isSink(pin)) {
                                arc = new GraphArc<Pin,Net>();
                                assert(arc != nullptr);
                                arc->setTo(pin);
                            }
                        } else if (__isSource(pin)) {
                            arc = new GraphArc<Pin,Net>();
                            assert(arc != nullptr);
                            arc->setFrom(pin);
                        }
                        if (nullptr == arc) {
                            //util::getMsgMgr->issueMessage();
                            continue;
                        }
                        arc->setEdge(net);
                        arc_stack_.push(arc);
                    }
                }
            }
            return true;
        }
        /// @brief setupEdgeStack 
        bool __setupEdgeStack() {
            Inst *inst = nullptr;
            ArrayObject<ObjectId> *arr_ptr = nullptr;

            if (node_stack_.empty() && cell_ != nullptr) { // start from top cell
                arr_ptr = Object::addr<ArrayObject<ObjectId>>(cell_->getIOPins());
                return (__setupEdgesByArray(arr_ptr));
            } else {
                while (!node_stack_.empty()) {
                    inst = nullptr;
                    inst = node_stack_.top();
                    node_stack_.pop();
                    arr_ptr = Object::addr<ArrayObject<ObjectId>>(inst->getPins());
                    if (!__setupEdgesByArray(arr_ptr)) continue;
                }
            }
            
            level_++;
            return true;
        }

        /// @brief initializeIterator 
        void __initializeAttrs() {
            cell_  = nullptr;
            edge_   = nullptr;
            from_  = nullptr;
            to_    = nullptr;
            level_ = 0;
        
            is_all_ = 1;
            is_reverse_ = 0;
            is_level_limited_ = 0;
            is_data_ready_ = 0;
            is_per_net_ = 0;
            is_p2p_ = 0;
        }

      private:
        Bits32 is_all_:1;
        Bits32 is_reverse_:1; // from sink to source
        Bits32 is_per_net_:1; // for single E
        Bits32 is_p2p_:1; // point to point (pin)
        Bits32 is_level_limited_:1;
        Bits32 is_data_ready_:1;
        int32_t level_;
        Cell *cell_;
        Net  *edge_;
        Pin  *from_;
        Pin  *to_;
        
        ArrayObject<ObjectId>::iterator iter_;
        std::stack<GraphArc<Pin,Net>*> arc_stack_;
        std::stack<GraphArc<Pin,Net>*> arc_;
        std::stack<Inst*> node_stack_;
        GraphArc<Pin,Net> res_arc_;
    };

  private:
    N *nodes_;
    E *edges_;
};

} // namespace db
} // namespace open_edi
