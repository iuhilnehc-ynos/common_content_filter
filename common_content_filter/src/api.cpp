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

#include "common_content_filter/api.h"

#include "rmw/allocators.h"
#include "rmw/error_handling.h"
#include "rmw/rmw.h"

#include <rmw/subscription_content_filter_options.h>
#include <mutex>
#include <pegtl.hpp>

#include "DDSFilterFactory.hpp"
#include "Log.hpp"


namespace common_content_filter {

const int MAGIC = 0x434654;  // 'C','F','T'
const char* FILTER_CLASS_NAME = "DDSSQL";

using DDSFilterFactory = eprosima_common::fastdds::dds::DDSSQLFilter::DDSFilterFactory;
using IContentFilter = eprosima_common::fastdds::dds::IContentFilter;

DDSFilterFactory *
get_common_content_filter_factory() {
  static DDSFilterFactory content_filter_factory;
  return &content_filter_factory;
}

class ContentFilterWrapper {
public:
  ContentFilterWrapper()
  {
    // logDebug(DDSSQLFILTER, "ContentFilterWrapper ctor : " << this);
  }

  ~ContentFilterWrapper() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (filter_instance_) {
      DDSFilterFactory::ReturnCode_t ret = get_common_content_filter_factory()->delete_content_filter(
        FILTER_CLASS_NAME,
        filter_instance_);
      if (ret != DDSFilterFactory::RETCODE_OK) {
        logError(DDSSQLFILTER, "Failed to delete content filter: " << ret);
      }

      filter_instance_ = nullptr;
    }

    // logDebug(DDSSQLFILTER, "ContentFilterWrapper dtor : " << this);
  }

  bool evaluate(void * ros_data, bool serialized) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (serialized) {
      // TODO. deserialize the ros_data
    }

    bool ret = true;
    if (filter_instance_) {
      ret = filter_instance_->evaluate(ros_data);
    }
    // logDebug(DDSSQLFILTER, "evaluate ret: " << ret);
    return ret;
  }

  bool set_filter_expression(
    const rosidl_message_type_support_t * type_support,
    const std::string & filter_expression,
    const std::vector<std::string> & expression_parameters)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    static std::string tip = filter_instance_ ? "create" : "set";
    DDSFilterFactory::ReturnCode_t ret = get_common_content_filter_factory()->create_content_filter(
      FILTER_CLASS_NAME,    // deprecated
      "",                   // deprecated
      type_support,
      filter_expression.c_str(),
      expression_parameters,
      filter_instance_);
    if (ret != DDSFilterFactory::RETCODE_OK) {
      logError(DDSSQLFILTER, "failed to " << tip << " content filter, ret: " << ret);
      return false;
    }

    filter_expression_ = filter_expression;
    expression_parameters_ = expression_parameters;

    return true;
  }

  bool get_filter_expression(
    std::string & filter_expression,
    std::vector<std::string> & expression_parameters)
  {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!filter_instance_) {
      logError(DDSSQLFILTER, "content filter instance is not created");
      return false;
    }

    filter_expression = filter_expression_;
    expression_parameters = expression_parameters_;

    return true;
  }

  IContentFilter * filter_instance() {
    return filter_instance_;
  }

  bool is_enabled() {
    return filter_instance_ != nullptr;
  }

  int magic() {
    return magic_;
  }

private:
  const int magic_ = MAGIC;
  IContentFilter * filter_instance_ = nullptr;
  std::string filter_expression_;
  std::vector<std::string> expression_parameters_;
  std::mutex mutex_;
};

}  // namespace common_content_filter


#ifdef __cplusplus
extern "C"
{
#endif

void *
create_common_content_filter() {
  common_content_filter::ContentFilterWrapper * filter_instance_wrapper = nullptr;
  try {
    filter_instance_wrapper =
      new common_content_filter::ContentFilterWrapper();
  } catch (const std::runtime_error& e) {
    logInfo(DDSSQLFILTER, "Failed to create content filter: " << e.what());
  }

  return filter_instance_wrapper;
}

bool
is_common_content_filter_enabled(void * instance) {
  common_content_filter::ContentFilterWrapper * content_filter_wrapper =
    static_cast<common_content_filter::ContentFilterWrapper *>(instance);
  if (!content_filter_wrapper || content_filter_wrapper->magic() != common_content_filter::MAGIC ) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  return content_filter_wrapper->is_enabled();
}

bool
evaluate_common_content_filter(void * instance, void * ros_data, bool serialized) {

  common_content_filter::ContentFilterWrapper * content_filter_wrapper =
    static_cast<common_content_filter::ContentFilterWrapper *>(instance);
  if (!content_filter_wrapper || content_filter_wrapper->magic() != common_content_filter::MAGIC ) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  bool ret = false;
  try {
    ret = content_filter_wrapper->evaluate(ros_data, serialized);
  } catch (const std::runtime_error& e) {
    logError(DDSSQLFILTER, "Failed to evaluate: " << e.what());
  }

  return ret;
}

bool
set_common_content_filter(
  void * instance,
  const rosidl_message_type_support_t * type_support,
  const rmw_subscription_content_filter_options_t * options
) {

  common_content_filter::ContentFilterWrapper * content_filter_wrapper =
    static_cast<common_content_filter::ContentFilterWrapper *>(instance);
  if (!content_filter_wrapper || content_filter_wrapper->magic() != common_content_filter::MAGIC ) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  if (!type_support || !options) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  std::vector<std::string> expression_parameters;
  for (size_t i = 0; i < options->expression_parameters.size; ++i) {
    expression_parameters.push_back(options->expression_parameters.data[i]);
  }

  bool ret = false;
  try {
    ret = content_filter_wrapper->set_filter_expression(
      type_support, options->filter_expression, expression_parameters
    );
  } catch (const std::runtime_error& e) {
    logInfo(DDSSQLFILTER, "Failed to create content filter: " << e.what());
  }

  return ret;
}

bool
get_common_content_filter(
  void * instance,
  rcutils_allocator_t * allocator,
  rmw_subscription_content_filter_options_t * options
) {
  common_content_filter::ContentFilterWrapper * content_filter_wrapper =
    static_cast<common_content_filter::ContentFilterWrapper *>(instance);
  if (!content_filter_wrapper || content_filter_wrapper->magic() != common_content_filter::MAGIC ) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  if (!allocator || !options) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return false;
  }

  std::string filter_expression;
  std::vector<std::string> expression_parameters;
  bool ret = content_filter_wrapper->get_filter_expression(
    filter_expression,
    expression_parameters
  );
  if (!ret) {
    return false;
  }

  std::vector<const char *> string_array;
  for (size_t i = 0; i < expression_parameters.size(); ++i) {
    string_array.push_back(expression_parameters[i].c_str());
  }

  rmw_ret_t rmw_ret = rmw_subscription_content_filter_options_init(
    filter_expression.c_str(),
    string_array.size(),
    string_array.data(),
    allocator,
    options
  );

  if (rmw_ret != RMW_RET_OK) {
    logError(DDSSQLFILTER, rmw_get_error_string().str);
    return false;
  }

  return true;
}

void
destroy_common_content_filter(void * instance) {
  common_content_filter::ContentFilterWrapper * content_filter_wrapper =
    static_cast<common_content_filter::ContentFilterWrapper *>(instance);
  if (!content_filter_wrapper || content_filter_wrapper->magic() != common_content_filter::MAGIC ) {
    logError(DDSSQLFILTER, "Invalid arguments");
    return;
  }

  delete content_filter_wrapper;
}

#ifdef __cplusplus
}
#endif