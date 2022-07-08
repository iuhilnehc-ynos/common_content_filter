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
#include <tuple>
#include <vector>

#include <rmw/rmw.h>
#include <rmw/serialized_message.h>
#include <test_msgs/msg/basic_types.h>

#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <test_content_filter_msgs/msg/complex.hpp>
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
  void set_options()
  {
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
  EXPECT_FALSE(common_content_filter_evaluate(nullptr, nullptr, true));
  EXPECT_FALSE(common_content_filter_evaluate(instance, nullptr, true));

  test_msgs__msg__BasicTypes msg;
  test_msgs__msg__BasicTypes__init(&msg);
  msg.int32_value = 3;
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    test_msgs__msg__BasicTypes__fini(&msg);
  });

  // test serialized message
  rmw_serialized_message_t serialized_message =
    rmw_get_zero_initialized_serialized_message();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  EXPECT_EQ(
    RMW_RET_OK,
    rmw_serialized_message_init(&serialized_message, 1U, &allocator)
  );
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    rmw_serialized_message_fini(&serialized_message);
  });

  EXPECT_EQ(
    RMW_RET_OK,
    rmw_serialize(&msg, type_support, &serialized_message)
  );

  // no filter setting, expect true
  EXPECT_TRUE(common_content_filter_evaluate(instance, &msg, false));
  EXPECT_TRUE(common_content_filter_evaluate(instance, &serialized_message, true));
  // after setting filter with "int32_value = 4"
  set_options();
  // expect msg(int32_value = 3) return false
  EXPECT_FALSE(common_content_filter_evaluate(instance, &msg, false));
  EXPECT_FALSE(common_content_filter_evaluate(instance, &serialized_message, true));
  // update msg with 4
  msg.int32_value = 4;
  EXPECT_EQ(
    RMW_RET_OK,
    rmw_serialize(&msg, type_support, &serialized_message)
  );

  EXPECT_TRUE(common_content_filter_evaluate(instance, &msg, false));
  EXPECT_TRUE(common_content_filter_evaluate(instance, &serialized_message, true));
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

class TestComplexMsgCommonContentFilterAPI : public ::testing::Test
{
protected:
  void SetUp()
  {
    type_support =
      rosidl_typesupport_cpp::get_message_type_support_handle<
      test_content_filter_msgs::msg::Complex>();

    instance = common_content_filter_create(type_support);
    EXPECT_NE(instance, nullptr);
  }

  void TearDown()
  {
    common_content_filter_destroy(instance);
  }

protected:
  void set_options(
    const char * filter_expression,
    size_t expression_parameter_argc,
    const char * expression_parameter_argv[])
  {
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
};

TEST_F(TestComplexMsgCommonContentFilterAPI, get) {
  // default is zero
  test_msgs::msg::BasicTypes basic_types_data_zero;

  // set member data with one
  test_msgs::msg::BasicTypes basic_types_data_one;
  basic_types_data_one.bool_value = true;
  basic_types_data_one.byte_value = 1;
  basic_types_data_one.char_value = 1;
  basic_types_data_one.float32_value = 1.0f;
  basic_types_data_one.float64_value = 1.0;
  basic_types_data_one.int8_value = 1;
  basic_types_data_one.uint8_value = 1;
  basic_types_data_one.int16_value = 1;
  basic_types_data_one.uint16_value = 1;
  basic_types_data_one.int32_value = 1l;
  basic_types_data_one.uint32_value = 1ul;
  basic_types_data_one.int64_value = 1ll;
  basic_types_data_one.uint64_value = 1ull;

  test_content_filter_msgs::msg::Basic basic_zero_one;
  basic_zero_one.names = {
    "basic_zero_one_first_name",            // data.basic_array[0].names[0]
    "basic_zero_one_second_name"            // data.basic_array[0].names[1]
  };
  basic_zero_one.basic_types = {
    basic_types_data_zero,                  // data.basic_array[0].basic_types[0] is 0
    basic_types_data_one                    // data.basic_array[0].basic_types[1] is 1
  };

  test_content_filter_msgs::msg::Basic basic_one_zero;
  basic_one_zero.names = {
    "basic_one_zero_first_name",            // data.basic_array[1].names[0]
    "basic_one_zero_second_name"            // data.basic_array[1].names[1]
  };
  basic_one_zero.basic_types = {
    basic_types_data_one,                   // data.basic_array[1].basic_types[0] is 1
    basic_types_data_zero                   // data.basic_array[1].basic_types[1] is 0
  };

  test_content_filter_msgs::msg::Complex msg;
  msg.data.basic_array = {
    basic_zero_one,
    basic_one_zero
  };
  msg.data.names = {
    "intermedia_first_name",                // data.names[0]
    "intermedia_second_name"                // data.names[1]
  };

  msg.name = "complex_name";                // name

  EXPECT_TRUE(common_content_filter_evaluate(instance, &msg, false));

  struct Info
  {
    const char * filter_expression;
    std::vector<const char *> expression_parameter;
    bool expectation;
  };

  std::vector<Info> expectation = {
    // name with string or string array
    {"name=%0", {"'complex_name'"}, true},
    {"name=%0", {"'not_complex_name'"}, false},

    {"data.names[0]=%0", {"'intermedia_first_name'"}, true},
    {"data.names[0]=%0", {"'intermedia_second_name'"}, false},
    {"data.names[1]=%0", {"'intermedia_first_name'"}, false},
    {"data.names[1]=%0", {"'intermedia_second_name'"}, true},

    {"data.basic_array[0].names[0]=%0", {"'basic_zero_one_first_name'"}, true},
    {"data.basic_array[0].names[0]=%0", {"'basic_zero_one_second_name'"}, false},
    {"data.basic_array[0].names[1]=%0", {"'basic_zero_one_first_name'"}, false},
    {"data.basic_array[0].names[1]=%0", {"'basic_zero_one_second_name'"}, true},

    {"data.basic_array[1].names[0]=%0", {"'basic_one_zero_first_name'"}, true},
    {"data.basic_array[1].names[0]=%0", {"'basic_one_zero_second_name'"}, false},
    {"data.basic_array[1].names[1]=%0", {"'basic_one_zero_first_name'"}, false},
    {"data.basic_array[1].names[1]=%0", {"'basic_one_zero_second_name'"}, true},

    {"name=%0 and data.names[0]=%1", {"'complex_name'", "'intermedia_first_name'"}, true},
    {"name=%0 and data.names[0]=%1", {"'not_complex_name'", "'intermedia_first_name'"}, false},
    {"name=%0 or data.names[0]=%1", {"'complex_name'", "'intermedia_first_name'"}, true},
    {"name=%0 or data.names[0]=%1", {"'not_complex_name'", "'intermedia_first_name'"}, true},
    {"name=%0 or data.names[0]=%1", {"'complex_name'", "'intermedia_second_name'"}, true},

    // basic types array
    // 0 0
    {"data.basic_array[0].basic_types[0].bool_value=%0", {"false"}, true},
    {"data.basic_array[0].basic_types[0].byte_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].char_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].float32_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].float64_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].int8_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].uint8_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].int16_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].uint16_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].int32_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].uint32_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].int64_value=%0", {"0"}, true},
    {"data.basic_array[0].basic_types[0].uint64_value=%0", {"0"}, true},

    {"data.basic_array[0].basic_types[0].bool_value=%0", {"true"}, false},
    {"data.basic_array[0].basic_types[0].byte_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].char_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].float32_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].float64_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].int8_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].uint8_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].int16_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].uint16_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].int32_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].uint32_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].int64_value=%0", {"1"}, false},
    {"data.basic_array[0].basic_types[0].uint64_value=%0", {"1"}, false},

    // 0 1
    {"data.basic_array[0].basic_types[1].bool_value=%0", {"false"}, false},
    {"data.basic_array[0].basic_types[1].byte_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].char_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].float32_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].float64_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].int8_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].uint8_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].int16_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].uint16_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].int32_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].uint32_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].int64_value=%0", {"0"}, false},
    {"data.basic_array[0].basic_types[1].uint64_value=%0", {"0"}, false},

    {"data.basic_array[0].basic_types[1].bool_value=%0", {"true"}, true},
    {"data.basic_array[0].basic_types[1].byte_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].char_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].float32_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].float64_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].int8_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].uint8_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].int16_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].uint16_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].int32_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].uint32_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].int64_value=%0", {"1"}, true},
    {"data.basic_array[0].basic_types[1].uint64_value=%0", {"1"}, true},

    // [1][0]
    {"data.basic_array[1].basic_types[0].bool_value=%0", {"false"}, false},
    {"data.basic_array[1].basic_types[0].byte_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].char_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].float32_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].float64_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].int8_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].uint8_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].int16_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].uint16_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].int32_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].uint32_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].int64_value=%0", {"0"}, false},
    {"data.basic_array[1].basic_types[0].uint64_value=%0", {"0"}, false},

    {"data.basic_array[1].basic_types[0].bool_value=%0", {"true"}, true},
    {"data.basic_array[1].basic_types[0].byte_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].char_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].float32_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].float64_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].int8_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].uint8_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].int16_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].uint16_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].int32_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].uint32_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].int64_value=%0", {"1"}, true},
    {"data.basic_array[1].basic_types[0].uint64_value=%0", {"1"}, true},

    // [1][1]
    {"data.basic_array[1].basic_types[1].bool_value=%0", {"false"}, true},
    {"data.basic_array[1].basic_types[1].byte_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].char_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].float32_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].float64_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].int8_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].uint8_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].int16_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].uint16_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].int32_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].uint32_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].int64_value=%0", {"0"}, true},
    {"data.basic_array[1].basic_types[1].uint64_value=%0", {"0"}, true},

    {"data.basic_array[1].basic_types[1].bool_value=%0", {"true"}, false},
    {"data.basic_array[1].basic_types[1].byte_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].char_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].float32_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].float64_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].int8_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].uint8_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].int16_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].uint16_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].int32_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].uint32_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].int64_value=%0", {"1"}, false},
    {"data.basic_array[1].basic_types[1].uint64_value=%0", {"1"}, false},
  };

  for (auto & item : expectation) {
    set_options(
      item.filter_expression,
      item.expression_parameter.size(),
      item.expression_parameter.data());
    EXPECT_EQ(common_content_filter_evaluate(instance, &msg, false), item.expectation);
  }
}
