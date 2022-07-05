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
 * @file DDSFilterExpression.hpp
 */

#ifndef COMMON_CONTENT_FILTER__DDSFILTEREXPRESSION_HPP_
#define COMMON_CONTENT_FILTER__DDSFILTEREXPRESSION_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "IContentFilter.hpp"
#include "DDSFilterCondition.hpp"
#include "DDSFilterField.hpp"
#include "DDSFilterParameter.hpp"

namespace eprosima_common
{
namespace fastdds
{
namespace dds
{
namespace DDSSQLFilter
{

/**
 * An IContentFilter that evaluates DDS-SQL filter expressions
 */
class DDSFilterExpression final : public IContentFilter
{

public:
  bool evaluate(
    const void * payload) const final;

  /**
   * Clear the information held by this object.
   */
  void clear();

  /// The root condition of the expression tree.
  std::unique_ptr<DDSFilterCondition> root;
  /// The fields referenced by this expression.
  std::map<std::string, std::shared_ptr<DDSFilterField>> fields;
  /// The parameters referenced by this expression.
  std::vector<std::shared_ptr<DDSFilterParameter>> parameters;
};

}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common

#endif  // COMMON_CONTENT_FILTER__DDSFILTEREXPRESSION_HPP_
