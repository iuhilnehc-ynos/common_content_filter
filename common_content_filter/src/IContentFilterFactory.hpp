// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file IContentFilter.hpp
 */

#ifndef COMMON_CONTENT_FILTER__ICONTENTFILTERFACTORY_HPP_
#define COMMON_CONTENT_FILTER__ICONTENTFILTERFACTORY_HPP_

#include <rosidl_runtime_c/message_type_support_struct.h>

#include <string>
#include <vector>

#include "IContentFilter.hpp"


namespace eprosima_common
{
namespace fastdds
{
namespace dds
{

/**
 * The interface that a factory of IContentFilter objects should implement.
 */
struct IContentFilterFactory
{
  enum ReturnCode_t
  {
    RETCODE_OK,
    RETCODE_BAD_PARAMETER,
    RETCODE_UNSUPPORTED,
  };

  using ParameterSeq = std::vector<std::string>;

  /**
   * Create or update an IContentFilter instance.
   *
   * @param [in]       filter_class_name   Filter class name for which the factory is being called.
   *                                       Allows using the same factory for different filter classes.
   * @param [in]       type_name           Type name of the topic being filtered.
   * @param [in]       type_support        Type support of the topic data being filtered.
   * @param [in]       filter_expression   Content filter expression.
   *                                       May be nullptr when updating the parameters of a filter instance.
   * @param [in]       filter_parameters   Values to set for the filter parameters (%n on the filter expression).
   * @param [in, out]  filter_instance     When a filter is being created, it will be nullptr on input,
   *                                       and will have the pointer to the created filter instance on output.
   *                                       The caller takes ownership of the filter instance returned.
   *                                       When a filter is being updated, it will have a previously returned pointer
   *                                       on input. The method takes ownership of the filter instance during its
   *                                       execution, and can update the filter instance or even destroy it and
   *                                       create a new one.
   *                                       The caller takes ownership of the filter instance returned.
   *                                       It should always have a valid pointer upon return.
   *                                       The original state of the filter instance should be preserved when an
   *                                       error is returned.
   *
   * @return A return code indicating the result of the operation.
   */
  virtual ReturnCode_t create_content_filter(
    const char * filter_class_name,
    const char * type_name,
    const rosidl_message_type_support_t * type_support,
    const char * filter_expression,
    const ParameterSeq & filter_parameters,
    IContentFilter * & filter_instance) = 0;

  /**
   * Delete an IContentFilter instance.
   *
   * @param [in]  filter_class_name   Filter class name for which the factory is being called.
   *                                  Allows using the same factory for different filter classes.
   * @param [in]  filter_instance     A pointer to a filter instance previously returned by create_content_filter.
   *                                  The factory takes ownership of the filter instance, and can decide to destroy
   *                                  it or keep it for future use.
   *                                  In case of deletion, note this pointer must be downcasted to the derived class.
   *
   * @return A return code indicating the result of the operation.
   */
  virtual ReturnCode_t delete_content_filter(
    const char * filter_class_name,
    IContentFilter * filter_instance) = 0;
};

} // namespace dds
} // namespace fastdds
} // namespace eprosima_common

#endif  // COMMON_CONTENT_FILTER__ICONTENTFILTERFACTORY_HPP_
