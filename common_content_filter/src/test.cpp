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

#include "common_content_filter/test.h"

#include "rmw/subscription_content_filter_options.h"

#include <cstdio> // for printf
#include <cstdlib> // for abort

#include <pegtl.hpp>

#include "DDSFilterGrammar.hpp"
#include "DDSFilterExpressionParser.hpp"
#include "DDSFilterFactory.hpp"
#include "Log.hpp"


// #include "rosidl_typesupport_introspection_c/identifier.h"
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"



using namespace eprosima_common::fastdds::dds;
using namespace eprosima_common::fastdds::dds::DDSSQLFilter;
using namespace eprosima_common::fastdds::dds::DDSSQLFilter::parser;


namespace common_content_filter {

const int MAGIC = 0x333644;  // cft

void
test_func_cpp()
{
  printf("test_func_cpp\n");

  struct sign s;
  printf("test_func_cpp, %p\n", (void*)&s);
  logError(DDSSQLFILTER, "ERROR " << MAGIC);

  std::string filter_expression = "id > 0";
  // std::string filter_expression = "id > 0 or a.b = 'def' and name=%0";
  // std::string filter_expression = "id=%0";
  // std::string filter_expression = "name[0]=%0";

  logInfo(DDSSQLFILTER, "filter_expression: " << filter_expression);

  // std::unique_ptr<ParseNode> node = parse_filter_expression(filter_expression.c_str(), nullptr);

  DDSFilterFactory factory;
  IContentFilter * filter_instance;

  DDSFilterFactory::ReturnCode_t ret = factory.create_content_filter(
            "filter_class_name",
            "type_name",
            nullptr,
            filter_expression.c_str(),
            {"'test'"},
            filter_instance);

  logInfo(DDSSQLFILTER, "factory.create_content_filter ret: " << ret);



  ret = factory.delete_content_filter(
              "filter_class_name",
              filter_instance);

  logInfo(DDSSQLFILTER, "factory.delete_content_filter ret: " << ret);

}

// class ContentFilterFactory {
// public:
//   ContentFilterFactory()
//     : magic_(MAGIC)
//   {
//     printf("ContentFilterFactory ctor : %p\n", (void*)this);
//   }
//   ~ContentFilterFactory() {
//     printf("ContentFilterFactory dtor : %p\n", (void*)this);
//   }

//   int magic() {
//     return magic_;
//   }
//   void test() {
//     printf("to call test in ContentFilterFactory : %p\n", (void*)this);
//   }

// private:
//   int magic_;
// };

class ContentFilter {
public:
  ContentFilter()
    : magic_(MAGIC)
  {
    printf("ContentFilter ctor : %p\n", (void*)this);
  };
  ~ContentFilter() {
    printf("ContentFilter dtor : %p\n", (void*)this);
  };

  int magic() {
    return magic_;
  }
  void test() {
    printf("to call test in ContentFilter : %p\n", (void*)this);
  }

private:
  int magic_;
};

}  // namespace common_content_filter


#ifdef __cplusplus
extern "C"
{
#endif

void
test_func_c() {
  return common_content_filter::test_func_cpp();
}


// TODO. deprecated, factory not exported outside. use a static instance in this library.
eprosima_common::fastdds::dds::DDSSQLFilter::DDSFilterFactory *
get_common_content_filter_factory() {
  // TODO. only one factory or one context one factory
  static eprosima_common::fastdds::dds::DDSSQLFilter::DDSFilterFactory content_filter_factory;
  return &content_filter_factory;
}

void *
create_common_content_filter(
  const rosidl_message_type_support_t * type_support,
  rmw_subscription_content_filter_options_t * options
) {

  if (!type_support || !options) {
    return NULL;
  }




  logInfo(DDSSQLFILTER, "create_common_content_filter options: " << options->filter_expression);

  IContentFilter * filter_instance;

  std::vector<std::string> expression_parameters;
  for (size_t i = 0; i < options->expression_parameters.size; ++i) {
    expression_parameters.push_back(options->expression_parameters.data[i]);
  }

  DDSFilterFactory::ReturnCode_t ret = get_common_content_filter_factory()->create_content_filter(
            "DDSSQL",    // deprecated
            "type_name", // deprecated
            type_support,
            options->filter_expression,
            expression_parameters,
            filter_instance);

  logInfo(DDSSQLFILTER, "factory.create_content_filter ret: " << ret);

  return filter_instance;
}

bool
common_content_filter_evaluate(void * instance, void * ros_data, bool serialized) {
  eprosima_common::fastdds::dds::IContentFilter * content_filter =
    static_cast<eprosima_common::fastdds::dds::IContentFilter *>(instance);
  if (serialized) {
    // TODO. deserialize the ros_data
  }

  bool ret = content_filter->evaluate(ros_data);
  return ret;
}

void
destroy_common_content_filter(void * instance) {
  eprosima_common::fastdds::dds::IContentFilter * content_filter =
    static_cast<eprosima_common::fastdds::dds::IContentFilter *>(instance);
  // if (!content_filter || content_filter->magic() != common_content_filter::MAGIC ) {
  //   printf("the instance is invalid\n");
  //   return;
  // }
  // delete content_filter;

  DDSFilterFactory::ReturnCode_t ret = get_common_content_filter_factory()->delete_content_filter(
            "DDSSQL",
            content_filter);

  logInfo(DDSSQLFILTER, "factory.delete_content_filter ret: " << ret);

}

#ifdef __cplusplus
}
#endif