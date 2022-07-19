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
 * @file FilterConditionState.hpp
 */

#ifndef COMMON_CONTENT_FILTER__FILTERCONDITIONSTATE_HPP_
#define COMMON_CONTENT_FILTER__FILTERCONDITIONSTATE_HPP_

namespace common_content_filter
{
namespace SQLFilter
{

/**
 * Possible states of a FilterCondition.
 */
enum class FilterConditionState : char
{
  /// Initial state of the FilterCondition, indicating there is no result.
  UNDECIDED,

  /// State indicating that the FilterCondition evaluates to @c false.
  RESULT_FALSE,

  /// State indicating that the FilterCondition evaluates to @c true.
  RESULT_TRUE
};

}  // namespace SQLFilter
}  // namespace common_content_filter

#endif  // COMMON_CONTENT_FILTER__FILTERCONDITIONSTATE_HPP_
