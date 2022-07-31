// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
// Copyright 2022 Sony Corporation
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
 * @file FilterParameter.hpp
 */

#ifndef FILTERPARAMETER_HPP_
#define FILTERPARAMETER_HPP_

#include "FilterValue.hpp"

namespace common_content_filter
{
namespace SQLFilter
{

/**
 * A FilterValue for expression parameters (i.e. %nn).
 */
class FilterParameter final : public FilterValue
{
public:
  virtual ~FilterParameter() = default;

  /**
   * Sets the value of this FilterParameter given from an input string.
   *
   * @param[in] parameter  The string from which to set the value.
   *
   * @return whether the parsing of the string correspond to a valid literal value.
   */
  bool set_value(
    const char * parameter);
};

}  // namespace SQLFilter
}  // namespace common_content_filter

#endif  // FILTERPARAMETER_HPP_
