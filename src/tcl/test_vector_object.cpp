/* @file  test_vector_object.cpp
 * @date  <date>
 * @brief <Descriptions>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
///@file test_vector_object.cpp : copied from appTest.cpp; replace std::vector
///with VectorObject.
//
#include <gperftools/profiler.h>

#include "db/core/db.h"
#include "db/core/inst.h"
#include "db/util/vector_object_var.h"
#include "tcl/test_object_tcl_cmd.h"
#include "util/util.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::util;
using namespace open_edi::db;

//#define size 4096000
bool parseVectorObjectArgument(int argc, const char *argv[], int *size) {
    *size = atoi(argv[1]);
    return true;
}

/************************************
 * main entry for test_vector_object.
 ************************************/
int vectorObjectTest(ClientData cld, Tcl_Interp *itp, int argc,
                     const char *argv[]) {
    MemPagePool *pool;
    uint64_t id;
    Inst *inst = nullptr;
    int size = 0;

    VectorObjectMax *vecInst0;
    VectorObjectMax *vecInst;
    Cell *current_top_cell = getTopCell();

    if (!current_top_cell) return 0;
    pool = current_top_cell->getPool();
    (void)parseVectorObjectArgument(argc, argv, &size);
    ProfilerStart("test_vector_object.prof");

    std::vector<ObjectId> vecInstId(size);
    // vecInst0 =
    // current_top_cell->createObject<VectorObjectMax>(kObjectTypeVector);
    // vecInst0->setHeaderId(vecInst0->getId());
    vecInst0 = VectorObjectMax::createDBVectorObjectVar(true /*is_header*/);
    // ObjectId vecInst0Id = vecInst0->getId();
    // vecInst =
    // current_top_cell->createObject<VectorObjectMax>(kObjectTypeVector);
    // vecInst->setHeaderId(vecInst->getId());

    vecInst = VectorObjectMax::createDBVectorObjectVar(true /*is_header*/);
    for (int i = 0; i < size; i++) {
        inst = current_top_cell->createObject<Inst>(kObjectTypeInst);
        vecInst->push_back(inst->getId());
        vecInstId[i] = inst->getId();
        Inst *inst0 = current_top_cell->createObject<Inst>(kObjectTypeInst);
        vecInst0->push_back(inst0->getId());
    }

    assert(MemPool::getObjectPtr<Inst>(inst->getId()) == inst);

    pool->printUsage();
    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_vector_object.txt",
                              std::ios::out | std::ios::app);
    // testing iterator:
    for (VectorObjectMax::iterator iter = vecInst->begin();
         iter != vecInst->end(); ++iter) {
        ObjectId id = (*iter);
        os << id << endl_c;
    }

    os << *vecInst << endl_c;
    vecInst->removeObjectIdFromVector(inst->getId());
    vecInst->removeObjectIdFromVector(vecInstId[0]);
    os << *vecInst << endl_c;
    // free allocated objects
    int total_size = vecInst0->totalSize();
    for (int i = 0; i < total_size; i++) {
        ObjectId object_id = vecInst0->pop_back();
        if (object_id) {
            Inst *object_data = MemPool::getObjectPtr<Inst>(object_id);
            current_top_cell->deleteObject<Inst>(object_data);
        }

        object_id = vecInst->pop_back();
        if (object_id) {  // in case object_id isn't invalid.
            Inst *object_data = MemPool::getObjectPtr<Inst>(object_id);
            current_top_cell->deleteObject<Inst>(object_data);
        }
    }
    // expecting empty vector....
    os << *vecInst0 << endl_c;
    os << *vecInst << endl_c;
    os.close();

    VectorObjectMax::deleteDBVectorObjectVar(vecInst0->getId());
    VectorObjectMax::deleteDBVectorObjectVar(vecInst->getId());
    pool->printUsage();

    // allocate from free list
    for (int i = 0; i < size; i++) {
        inst = current_top_cell->createObject<Inst>(kObjectTypeInst);
    }

    assert(MemPool::getObjectPtr<Inst>(inst->getId()) == inst);

    pool->printUsage();
    ProfilerStop();

    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
