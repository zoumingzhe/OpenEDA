/**
 * File              : object.cpp
 * Author            : Yibo Lin <yibolin@pku.edu.cn>
 * Date              : 04.26.2020
 * Last Modified Date: 04.26.2020
 * Last Modified By  : Yibo Lin <yibolin@pku.edu.cn>
 */

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "db/core/db.h"

EDI_BEGIN_NAMESPACE

namespace unitest {

// GTest class for fft module testing
class ObjectTest : public ::testing::Test {
 public:
  void testConstruct() {
    // test default construction
    Object obj1;
    
    ASSERT_EQ(obj1.getOwnerId(), 0);
    ASSERT_EQ(obj1.getId(), UNINIT_OBJECT_ID);

    // test construction
    Object obj2(nullptr, 0);
    ASSERT_EQ(obj2.getOwnerId(), 0);
    ASSERT_EQ(obj2.getId(), 0);

    Object obj3(&obj2, 1);
    ASSERT_EQ(obj3.getOwnerId(), obj2.getId());
    ASSERT_EQ(obj3.getId(), 1);
  }
};

TEST_F(ObjectTest, Construct) { testConstruct(); }

}  // namespace unitest

EDI_END_NAMESPACE
