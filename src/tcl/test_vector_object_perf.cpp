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

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

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

enum TestModeEnum { 
  kTestRandomAccess = 0,
  kTestIteration 
};

static bool parseVectorObjectPerfArgument(int argc, const char *argv[],
                                          bool *use_stdvector, int *size,
                                          int *iteration,
                                          TestModeEnum *test_mode);
static void vectorObjectPerfTest(int size);
static void stdVectorPerfTest(int size);

static bool parseVectorObjectPerfArgument(int argc, const char *argv[],
                                          bool *use_stdvector, int *size,
                                          int *iteration,
                                          TestModeEnum *test_mode) {
    *use_stdvector = (atoi(argv[1]) != 0) ? true : false;
    *size = atoi(argv[2]);
    if (argc >= 4) {
        *iteration = atoi(argv[3]);
    } else {
        *iteration = 4096;
    }

    if (argc >= 5) {
        *test_mode = static_cast<TestModeEnum>(atoi(argv[4]));
    }
    return true;
}

static VectorObjectMax *testVectorObjectPtr = nullptr;
// static VectorObject *testVectorObjectPtr = nullptr;
#define testVectorObject (*testVectorObjectPtr)
static std::vector<ObjectId> testSTDVector;
char testSTDPerfFile[128];
char testVOPerfFile[128];

static void initVectorObject(int size) {
    Inst *inst = nullptr;
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return;

    testVectorObjectPtr =
        VectorObjectMax::createDBVectorObjectVar(true /*is_header*/);
    for (int i = 0; i < size; ++i) {
        inst = current_top_cell->createObject<Inst>(kObjectTypeInst);

        testVectorObjectPtr->push_back(inst->getId());
    }
}

static void initSTDVector(int size) {
    Inst *inst = nullptr;
    Cell *current_top_cell = getTopCell();
    if (!current_top_cell) return;

    if (testSTDVector.size() > 0) {
        // testSTDVector.clear();
        std::vector<ObjectId>().swap(testSTDVector);
    }
    testSTDVector.reserve(size);
    for (int i = 0; i < size; ++i) {
        inst = current_top_cell->createObject<Inst>(kObjectTypeInst);
        testSTDVector.push_back(inst->getId());
    }
}

static void freeVectorObject() {
    if (testVectorObjectPtr == nullptr) {
        return;
    }

    int size = testVectorObjectPtr->totalSize();

    Cell *current_top_cell = getTopCell();

    for (int i = 0; i < size; ++i) {
        ObjectId inst_id = testVectorObjectPtr->pop_back();
        Inst *object_data = MemPool::getObjectPtr<Inst>(inst_id);
        current_top_cell->deleteObject<Inst>(object_data);
    }
}

static void freeSTDVector() {
    int index = testSTDVector.size();
    Cell *current_top_cell = getTopCell();

    while (--index > -1) {
        ObjectId inst_id = testSTDVector[index];
        testSTDVector.pop_back();
        Inst *object_data = MemPool::getObjectPtr<Inst>(inst_id);
        current_top_cell->deleteObject<Inst>(object_data);
    }
}

static void iterationVectorObject(int iteration, OStream<std::ofstream> &os) {
    uint8_t endl_c = '\n';

    while (--iteration) {
        int index = 0;
        for (VectorObjectMax::iterator iter = testVectorObjectPtr->begin();
             iter != testVectorObjectPtr->end(); ++iter) {
            ObjectId id = (*iter);
            ++index;
            if (index < 128) {  // in case the output file is too long...
                os << "vector Item#" << index << " " << id << endl_c;
            }
        }
    }
    // os << testVectorObject;
}

static void randomAccessVectorObject(int iteration,
                                     OStream<std::ofstream> &os) {
    int size = testVectorObject.totalSize();
    int max_index = size - 1;
    uint8_t endl_c = '\n';

    default_random_engine engine(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<> distribution(0, max_index);

    while (--iteration) {
        for (int i = 0; i < size; ++i) {
            int index = distribution(engine);
            ObjectId id = testVectorObject[index];
            if (i < 128) {  // in case the output file is too long...
                os << "vector Item#" << index << " " << id << endl_c;
            }
        }
    }
}

static void vectorObjectPerfTest(TestModeEnum test_mode, int size,
                                 int iteration) {
    initVectorObject(size);
    MemPagePool *pool = getTopCell()->getPool();
    pool->printUsage();

    uint8_t endl_c = '\n';
    OStream<std::ofstream> os("test_vector_object_perf.txt",
                              std::ios::out | std::ios::app);
    ProfilerStart(testVOPerfFile);

    switch (test_mode) {
        case kTestRandomAccess:
            randomAccessVectorObject(iteration, os);
            break;
        case kTestIteration:
            iterationVectorObject(iteration, os);
            break;
    }

    ProfilerStop();
    pool->printUsage();
    os.close();

    freeVectorObject();
    pool->printUsage();
}

static void iterationSTDVector(int iteration, OStream<std::ofstream> &os) {
    uint8_t endl_c = '\n';

    while (--iteration) {
        int index = 0;

        for (std::vector<ObjectId>::iterator iter = testSTDVector.begin();
             iter != testSTDVector.end(); ++iter) {
            ObjectId id = (*iter);
            ++index;
            if (index < 128) {  // in case the output file is too long...
                os << "stdvector Item#" << index << " " << id << endl_c;
            }
        }
    }
}

static void randomAccessSTDVector(int iteration, OStream<std::ofstream> &os) {
    int size = testSTDVector.size();
    int max_index = size - 1;
    uint8_t endl_c = '\n';

    default_random_engine engine(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<> distribution(0, max_index);
    while (--iteration) {
        for (int i = 0; i < size; ++i) {
            int index = distribution(engine);
            ObjectId id = testSTDVector[index];
            if (i < 128) {  // in case the output file is too long...
                os << "stdvector Item#" << index << " " << id << endl_c;
            }
        }
    }
}

static void stdVectorPerfTest(TestModeEnum test_mode, int size, int iteration) {
    MemPagePool *pool = getTopCell()->getPool();
    initSTDVector(size);
    pool->printUsage();

    OStream<std::ofstream> os("test_stdvector_perf.txt",
                              std::ios::out | std::ios::app);
    ProfilerStart(testSTDPerfFile);
    switch (test_mode) {
        case kTestRandomAccess:
            randomAccessSTDVector(iteration, os);
            break;
        case kTestIteration:
            iterationSTDVector(iteration, os);
            break;
    }
    ProfilerStop();
    pool->printUsage();
    os.close();

    freeSTDVector();
    pool->printUsage();
}

/************************************
 * main entry for test_vector_object.
 ************************************/
int vectorObjectPerfTest(ClientData cld, Tcl_Interp *itp, int argc,
                         const char *argv[]) {
    int size = 0;
    int iteration = 0;
    bool use_stdvector = false;
    TestModeEnum test_mode = kTestIteration;
    (void)parseVectorObjectPerfArgument(argc, argv, &use_stdvector, &size,
                                        &iteration, &test_mode);
    sprintf(testSTDPerfFile, "test_stdvector_%d.prof", getpid());
    sprintf(testVOPerfFile, "test_vector_object_%d.prof", getpid());

    if (use_stdvector) {
        stdVectorPerfTest(test_mode, size, iteration);
    } else {
        vectorObjectPerfTest(test_mode, size, iteration);
    }

    return TCL_OK;
}

}  // namespace tcl
}  // namespace open_edi
