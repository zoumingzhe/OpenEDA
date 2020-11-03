/* @file  app_test.cpp
 * @date  <Aug 2020>
 * @brief <>
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "tcl/test_app.h"

#include <gperftools/profiler.h>

#include "db/core/cell.h"
#include "db/core/db.h"
#include "db/core/db_tcl_command.h"
#include "db/core/inst.h"
#include "db/core/object.h"
#include "db/util/array.h"
#include "db/util/box.h"
#include "tcl/test_object_tcl_cmd.h"
#include "util/util.h"

namespace open_edi {
namespace tcl {

using namespace std;
using namespace open_edi::db;
using namespace open_edi::util;

#define TEST_ALLOC_NUM 4096000

/// @brief createObject: used for performance comparison.
///
/// @tparam T
/// @param idx
/// @param type
///
/// @return
template <class T>
T *createObject(int idx, ObjectType type) {
    T *obj = nullptr;
    ObjectId id;

    assert(idx >= 0 && idx < TEST_ALLOC_NUM);

    obj = new T;
    obj->setId(idx);
    obj->setObjectType(type);

    return obj;
}

/// @brief testAppMem
///
/// @return
static int testAppMem() {
    MemPagePool *pool;
    uint64_t id;

    Inst *inst = nullptr, *t2 = nullptr;
    std::vector<Inst *> vecInst;

    Cell *tc = getTopCell();

    vecInst.resize(TEST_ALLOC_NUM, nullptr);

    for (int i = 0; i < TEST_ALLOC_NUM; i++) {
        // Cell pool allocation
        inst = tc->createObject<Inst>(kObjectTypeInst);
        vecInst[i] = inst;

        // System allocation, compare performance
        t2 = createObject<Inst>(i, kObjectTypeInst);
    }

    assert(MemPool::getObjectPtr<Inst>(inst->getId()) == inst);
    tc->getPool()->printUsage();

    // free allocated objects
    for (auto it : vecInst) {
        tc->deleteObject<Inst>(it);
    }

    // allocate again, expect to get from free list
    for (int i = 0; i < TEST_ALLOC_NUM; i++) {
        inst = tc->createObject<Inst>(kObjectTypeInst);
    }

    assert(MemPool::getObjectPtr<Inst>(inst->getId()) == inst);

    tc->getPool()->printUsage();

    return 0;
}

const int kTestArraySize = 65;
const int kTestArrayPerformanceSize = 409600;

/**
 * @brief test ArrayObject
 *
 */
void testArray() {
    ObjectId id;
    ArrayObject<int32_t> *int_array = nullptr;

    Cell *cell = getTopCell();

    // int32_t type
    int_array = cell->createObject<ArrayObject<int32_t>>(kObjectTypeArray);
    int_array->setPool(cell->getPool());
    int_array->reserve(kTestArraySize);

    // push
    for (int i = 0; i < kTestArraySize; i++) {
        int_array->pushBack(i);
    }

    // access
    for (int i = 0; i < kTestArraySize; i++) {
        assert(i == (*int_array)[i]);
    }

    // increase size
    for (int i = 0; i < kTestArraySize; i++) {
        int32_t v = i + kTestArraySize;
        int_array->pushBack(v);
    }

    // access
    for (int i = 0; i < kTestArraySize * 2; i++) {
        assert(i == (*int_array)[i]);
    }

    // set value
    for (int i = 0; i < kTestArraySize; i++) {
        (*int_array)[i] = i;
    }

    // verify
    for (int i = 0; i < kTestArraySize; i++) {
        assert(i == (*int_array)[i]);
    }
    {
    int i = 0;
    for (ArrayObject<int32_t>::iterator iter = int_array->begin(); 
            iter != int_array->end(); ++iter) {
        assert(i == (*iter));
        ++i;
    }
    }
    { //test adjustSize():
    int origin_size = int_array->getSize();
    int i = 0;
    int_array->adjustSize(kTestArraySize);
    for (ArrayObject<int32_t>::iterator iter = int_array->begin(); 
            iter != int_array->end(); iter++) {
        assert(i == (*iter));
        i++;
    }
    assert (i == kTestArraySize);
    int_array->adjustSize(origin_size);
    i = 0;
    for (ArrayObject<int32_t>::iterator iter = int_array->begin(); 
            iter != int_array->end(); iter++) {
        assert(i == (*iter));
        i++;
    }
    assert (i == origin_size);
    }

    // Box object
    Box init_box(0, 0, 100, 100), box_obj;
    ArrayObject<Box> *obj_array = nullptr;
    obj_array = cell->createObject<ArrayObject<Box>>(kObjectTypeArray);
    obj_array->setPool(cell->getPool());
    obj_array->reserve(kTestArraySize);

    // pushBack
    for (int i = 0; i < kTestArraySize; i++) {
        obj_array->pushBack(init_box);
    }

    // access, verify
    for (int i = 0; i < kTestArraySize; i++) {
        box_obj = (*obj_array)[i];
        assert(box_obj == init_box);
    }

    // increase size
    for (int i = 0; i < kTestArraySize; i++) {
        obj_array->pushBack(init_box);
    }

    // access, verify
    for (int i = 0; i < kTestArraySize * 2; i++) {
        box_obj = (*obj_array)[i];
        assert(box_obj == init_box);
    }

    // set value
    for (int i = 0; i < kTestArraySize; i++) {
        (*obj_array)[i] = Box(0, 0, 200, 200);
    }

    // access, verify
    for (int i = 0; i < kTestArraySize; i++) {
        box_obj = (*obj_array)[i];
        assert(box_obj == Box(0, 0, 200, 200));
    }

    // iterator
    int i = 0;
    ArrayObject<Box>::iterator iter;
    for(iter = obj_array->begin(); iter != obj_array->end(); iter++) {
        box_obj = *iter;
        if (i<kTestArraySize) {
            assert(box_obj == Box(0, 0, 200, 200));
        } else if (i >= kTestArraySize) {
            assert(box_obj == init_box);
        }
        i++;
    }

    // performance
    int_array = cell->createObject<ArrayObject<int32_t>>(kObjectTypeArray);
    int_array->setPool(cell->getPool());
    int_array->reserve(kTestArrayPerformanceSize);

    for (int i = 0; i < kTestArrayPerformanceSize; i++) {
        int_array->pushBack(i);
    }

    for (int i = 0; i < kTestArrayPerformanceSize; i++) {
        assert(i == (*int_array)[i]);
    }

    std::vector<int32_t> v_int;

    for (int i = 0; i < kTestArrayPerformanceSize; i++) {
        v_int.push_back(i);
    }

    for (int i = 0; i < kTestArrayPerformanceSize; i++) {
        assert(i == v_int[i]);
    }
}

/**
 * @brief main entry for various tests.
 *
 * @param cld
 * @param itp
 * @param argc
 * @param argv
 * @return int
 */
static int testApp(ClientData cld, Tcl_Interp *itp, int argc,
                   const char *argv[]) {
    ProfilerStart("test_j20.prof");

    testArray();
    testAppMem();
    utilTestMsg();
    
    ProfilerStop();

    return TCL_OK;
}

/**
 * @brief registerTestCommands
 *
 * @param itp
 */
void registerTestCommands(Tcl_Interp *itp) {
    Tcl_CreateCommand(itp, "do_test", testApp, NULL, NULL);
    //(temporarily) testing Tcl commands
    registerTestObjectCommand(itp);
}

}  // namespace tcl
}  // namespace open_edi
