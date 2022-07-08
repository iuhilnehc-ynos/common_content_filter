// Copyright 2022 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <gtest/gtest.h>

#include <test_msgs/msg/basic_types.h>
#include <osrf_testing_tools_cpp/scope_exit.hpp>

#include "common_content_filter/api.h"

class TestCommonContentFilterAPI : public ::testing::Test
{
protected:
  void SetUp()
  {
    type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(test_msgs, msg, BasicTypes);
    instance = common_content_filter_create(type_support);
    EXPECT_NE(instance, nullptr);
  }

  void TearDown()
  {
    common_content_filter_destroy(instance);
  }

protected:
  void set_options() {
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rmw_subscription_content_filter_options_t options =
      rmw_get_zero_initialized_content_filter_options();
    EXPECT_EQ(
      RMW_RET_OK,
      rmw_subscription_content_filter_options_init(
        filter_expression,
        expression_parameter_argc,
        expression_parameter_argv,
        &allocator,
        &options)
    );
    OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
    {
      rmw_subscription_content_filter_options_fini(&options, &allocator);
    });

    EXPECT_TRUE(common_content_filter_set(instance, &options));
  }

  void * instance;
  const rosidl_message_type_support_t * type_support;
  const char * filter_expression = "int32_value = %0";
  size_t expression_parameter_argc = 1;
  const char * expression_parameter_argv[1] = {"4"};
};

TEST_F(TestCommonContentFilterAPI, is_enabled) {
  EXPECT_FALSE(common_content_filter_is_enabled(nullptr));
  EXPECT_FALSE(common_content_filter_is_enabled(instance));
  set_options();
  EXPECT_TRUE(common_content_filter_is_enabled(instance));
}

TEST_F(TestCommonContentFilterAPI, evaluate) {
  EXPECT_FALSE(common_content_filter_evaluate(nullptr, nullptr, false));
  EXPECT_FALSE(common_content_filter_evaluate(instance, nullptr, false));

  test_msgs__msg__BasicTypes msg;
  test_msgs__msg__BasicTypes__init(&msg);
  msg.int32_value = 3;
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    test_msgs__msg__BasicTypes__fini(&msg);
  });

  // no filter setting, expect true
  EXPECT_TRUE(common_content_filter_evaluate(instance, &msg, false));
  // after setting filter with "int32_value = 4"
  set_options();
  // expect msg(int32_value = 3) return false
  EXPECT_FALSE(common_content_filter_evaluate(instance, &msg, false));
  // update msg with 4
  msg.int32_value = 4;
  EXPECT_TRUE(common_content_filter_evaluate(instance, &msg, false));
}

TEST_F(TestCommonContentFilterAPI, set) {
  EXPECT_FALSE(common_content_filter_set(nullptr, nullptr));
  EXPECT_FALSE(common_content_filter_set(instance, nullptr));
  set_options();
}

TEST_F(TestCommonContentFilterAPI, get) {
  EXPECT_FALSE(common_content_filter_get(nullptr, nullptr, nullptr));
  EXPECT_FALSE(common_content_filter_get(instance, nullptr, nullptr));
  set_options();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  EXPECT_FALSE(common_content_filter_get(instance, &allocator, nullptr));

  rmw_subscription_content_filter_options_t options =
    rmw_get_zero_initialized_content_filter_options();
  EXPECT_TRUE(common_content_filter_get(instance, &allocator, &options));
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    rmw_subscription_content_filter_options_fini(&options, &allocator);
  });

  EXPECT_STREQ(options.filter_expression, filter_expression);
  ASSERT_EQ(expression_parameter_argc, options.expression_parameters.size);
  for (size_t i = 0; i < expression_parameter_argc; ++i) {
    EXPECT_STREQ(options.expression_parameters.data[i], expression_parameter_argv[i]);
  }
}
