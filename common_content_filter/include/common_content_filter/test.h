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

#ifndef COMMON_CONTENT_FILTER__TEST_HPP_
#define COMMON_CONTENT_FILTER__TEST_HPP_

#include "common_content_filter/visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void common_content_filter_factory_t;
typedef void common_content_filter_t;

COMMON_CONTENT_FILTER_PUBLIC
void *
create_common_content_filter_factory();

COMMON_CONTENT_FILTER_PUBLIC
void
test_common_content_filter_factory(void *);

COMMON_CONTENT_FILTER_PUBLIC
void
test_func_c();

COMMON_CONTENT_FILTER_PUBLIC
common_content_filter_t *
create_common_content_filter();

COMMON_CONTENT_FILTER_PUBLIC
void
test_common_content_filter(void *);

#ifdef __cplusplus
}
#endif


#endif  // COMMON_CONTENT_FILTER__TEST_HPP_
