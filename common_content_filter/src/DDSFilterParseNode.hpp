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
 * @file DDSFilterParseNode.hpp
 */

#ifndef COMMON_CONTENT_FILTER__DDSFILTERPARSENODE_HPP_
#define COMMON_CONTENT_FILTER__DDSFILTERPARSENODE_HPP_

#include <memory>
#include <vector>

#include <tao/pegtl/contrib/parse_tree.hpp>

#include "DDSFilterField.hpp"
#include "DDSFilterValue.hpp"

namespace eprosima_common
{
namespace fastdds
{
namespace dds
{
namespace DDSSQLFilter
{
namespace parser
{

using namespace tao::TAO_PEGTL_NAMESPACE;

struct ParseNode : parse_tree::basic_node<ParseNode>
{
  // When the node is a literal value, it will hold a pointer to it
  std::unique_ptr<DDSFilterValue> value;

  // When the node is a fieldname, it will hold the access path to the field, the data kind, and the type id
  std::vector<DDSFilterField::FieldAccessor> field_access_path;
  DDSFilterValue::ValueKind field_kind = DDSFilterValue::ValueKind::STRING;
  // ros2 primitive type id
  uint8_t type_id = 0;

  // When the node is a parameter, it will hold the parameter index
  size_t parameter_index = 0;

  const ParseNode & left() const
  {
    return *(children[0]);
  }

  const ParseNode & right() const
  {
    return *(children[1]);
  }

};

}  // namespace parser
}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common

#endif  // COMMON_CONTENT_FILTER__DDSFILTERPARSENODE_HPP_
