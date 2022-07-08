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

#ifndef COMMON_CONTENT_FILTER__API_H_
#define COMMON_CONTENT_FILTER__API_H_

#include "common_content_filter/visibility_control.h"

#include <rcutils/allocator.h>
#include <rmw/subscription_content_filter_options.h>
#include <rosidl_runtime_c/message_type_support_struct.h>


#ifdef __cplusplus
extern "C"
{
#endif

/// Create a common content filter instance to filter data.
/**
 * \param[in] type_support Type support of the topic data being filtered
 * \return a valid address if success, or NULL on failure
 */
COMMON_CONTENT_FILTER_PUBLIC
void *
common_content_filter_create(const rosidl_message_type_support_t * type_support);

/// Check if the content filter instance is enabled.
/**
 * \param[in] instance The content filter instance
 * \return true if enabled, or false
 */
COMMON_CONTENT_FILTER_PUBLIC
bool
common_content_filter_is_enabled(void * instance);

/// Use the content filter instance to evalute the data.
/**
 * \param[in] instance The content filter instance
 * \param[in] data The ros2 payload
 * \param[in] serialized Indicate the ros2 payload is serialized or not
 * \return true if evaluate successfully, or false
 */
COMMON_CONTENT_FILTER_PUBLIC
bool
common_content_filter_evaluate(void * instance, void * data, bool serialized);

/// Set a common content filter instance with an options.
/**
 * \param[in] instance The content filter instance
 * \param[in] options the filter options
 * \return true if success, or false
 */
COMMON_CONTENT_FILTER_PUBLIC
bool
common_content_filter_set(
  void * instance,
  const rmw_subscription_content_filter_options_t * options
);

/// Get the options from a common content filter instance.
/**
 * \param[in] instance The content filter instance
 * \param[in] allocator Type support of the topic data being filtered
 * \param[in] options the filter options
 * \return true if success, or false
 */
COMMON_CONTENT_FILTER_PUBLIC
bool
common_content_filter_get(
  void * instance,
  rcutils_allocator_t * allocator,
  rmw_subscription_content_filter_options_t * options
);

/// Destroy the content filter instance.
/**
 * \param[in] instance The content filter instance
 */
COMMON_CONTENT_FILTER_PUBLIC
void
common_content_filter_destroy(void * instance);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_CONTENT_FILTER__API_H_
