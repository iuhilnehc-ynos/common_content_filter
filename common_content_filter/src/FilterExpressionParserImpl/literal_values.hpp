// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
// Copyright 2022 Sony Group Corporation
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
 * @file literal_values.hpp
 *
 * Note: this is an implementation file, designed to be included inside the
 * FilterExpressionParser.hpp file of the parent folder.
 */

#ifndef FILTEREXPRESSIONPARSERIMPL__LITERAL_VALUES_HPP_
#define FILTEREXPRESSIONPARSERIMPL__LITERAL_VALUES_HPP_

#include <memory>
struct literal_value_processor
  : parse_tree::apply<literal_value_processor>
{
  template<typename ... States>
  static void transform(
    std::unique_ptr<ParseNode> & n,
    States &&... /*st*/)
  {
    n->value.reset(new FilterValue());
    if (n->is<true_value>()) {
      n->value->kind = FilterValue::ValueKind::BOOLEAN;
      n->value->boolean_value = true;
    } else if (n->is<false_value>()) {
      n->value->kind = FilterValue::ValueKind::BOOLEAN;
      n->value->boolean_value = false;
    } else if (n->is<integer_value>() || n->is<hex_value>()) {
      if (n->m_begin.data[0] == '-') {
        n->value->kind = FilterValue::ValueKind::SIGNED_INTEGER;
        n->value->signed_integer_value = std::stoll(n->content(), nullptr, 0);
      } else {
        n->value->kind = FilterValue::ValueKind::UNSIGNED_INTEGER;
        n->value->unsigned_integer_value = std::stoull(n->content(), nullptr, 0);
      }
    } else if (n->is<float_value>()) {
      n->value->kind = FilterValue::ValueKind::FLOAT_CONST;
      n->value->float_value = std::stold(n->content());
    } else if (n->is<char_value>()) {
      n->value->kind = FilterValue::ValueKind::CHAR;
      n->value->char_value = n->m_begin.data[1];
    } else if (n->is<string_value>()) {
      const ParseNode & content_node = n->left();
      size_t str_len = content_node.m_end.byte - content_node.m_begin.byte;
      if (sizeof(n->value->string_value) < str_len) {
        throw parse_error("string constant has more than 255 characters", n->end());
      }
      n->value->kind = FilterValue::ValueKind::STRING;
      strncpy(n->value->string_value, content_node.m_begin.data, str_len);
    }

    n->children.clear();
  }
};

#endif  // FILTEREXPRESSIONPARSERIMPL__LITERAL_VALUES_HPP_
