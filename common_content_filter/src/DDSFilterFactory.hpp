// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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
 * @file DDSFilterFactory.hpp
 */

#ifndef COMMON_CONTENT_FILTER__DDSFILTERFACTORY_HPP_
#define COMMON_CONTENT_FILTER__DDSFILTERFACTORY_HPP_

#include "IContentFilterFactory.hpp"

#include "IContentFilter.hpp"

#include "DDSFilterEmptyExpression.hpp"
#include "DDSFilterExpression.hpp"
#include "ObjectPool.hpp"

namespace eprosima_common
{
namespace fastdds
{
namespace dds
{
namespace DDSSQLFilter
{

/**
 * An IContentFilterFactory that processes DDS-SQL filter expressions.
 */
class DDSFilterFactory final : public IContentFilterFactory
{

public:
  ~DDSFilterFactory();

  ReturnCode_t create_content_filter(
    const rosidl_message_type_support_t * type_support,
    const char * filter_expression,
    const ParameterSeq & filter_parameters,
    IContentFilter * & filter_instance) override;

  ReturnCode_t delete_content_filter(
    IContentFilter * filter_instance) override;

private:
  /**
   * Retrieve a DDSFilterExpression from the pool.
   *
   * @return A pointer to an empty DDSFilterExpression.
   */
  DDSFilterExpression * get_expression()
  {
    return expression_pool_.get(
      []
      {
        return new DDSFilterExpression();
      });
  }

  /**
   * Generic method to perform processing of an AST node resulting from the parsing of a DDS-SQL filter expression.
   * Provides a generic mechanism for methods that perform post-processing of the generated AST tree, so they could
   * have access to the private fields of DDSFilterFactory.
   *
   * @return return code indicating the conversion result.
   */
  template<typename _Parser, typename _ParserNode, typename _State, typename _Output>
  ReturnCode_t convert_tree(
    _State & state,
    _Output & parse_output,
    const _ParserNode & node);

  /// Empty expressions content filter
  DDSFilterEmptyExpression empty_expression_;
  /// Pool of DDSFilterExpression objects
  ObjectPool<DDSFilterExpression *> expression_pool_;

};

}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common

#endif  // COMMON_CONTENT_FILTER__DDSFILTERFACTORY_HPP_
