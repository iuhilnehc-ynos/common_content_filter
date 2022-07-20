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

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <rosidl_runtime_c/message_type_support_struct.h>

const rosidl_message_type_support_t *
get_type_support_introspection(
  const rosidl_message_type_support_t * type_support);

#endif  // UTILITIES_HPP_
