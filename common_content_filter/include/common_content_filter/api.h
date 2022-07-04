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

#ifndef COMMON_CONTENT_FILTER__API_HPP_
#define COMMON_CONTENT_FILTER__API_HPP_

#include "common_content_filter/visibility_control.h"

#include <rmw/subscription_content_filter_options.h>
#include <rosidl_runtime_c/message_type_support_struct.h>


#ifdef __cplusplus
extern "C"
{
#endif

COMMON_CONTENT_FILTER_PUBLIC
void *
create_common_content_filter(
  const rosidl_message_type_support_t * type_support,
  rmw_subscription_content_filter_options_t * options
);

COMMON_CONTENT_FILTER_PUBLIC
bool
common_content_filter_evaluate(void * instance, void * ros_data, bool serialized);

COMMON_CONTENT_FILTER_PUBLIC
void
destroy_common_content_filter(void *);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_CONTENT_FILTER__API_HPP_
