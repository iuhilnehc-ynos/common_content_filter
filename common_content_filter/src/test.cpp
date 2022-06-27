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

#include "common_content_filter/test.h"

#include <cstdio> // for printf

namespace common_content_filter {

const int MAGIC = 0x333646;  // cft

void
test_func_cpp()
{
  printf("test_func_cpp\n");
}

class ContentFilterFactory {
public:
  ContentFilterFactory()
    : magic_(MAGIC) {};
  ~ContentFilterFactory() {};

  int magic() {
    return magic_;
  }
  void test() {
    printf("to call test in ContentFilterFactory : %p\n", (void*)this);
  }

private:
  int magic_;
};

class ContentFilter {
public:
  ContentFilter()
    : magic_(MAGIC) {};
  ~ContentFilter() {};

  int magic() {
    return magic_;
  }
  void test() {
    printf("to call test in ContentFilter : %p\n", (void*)this);
  }

private:
  int magic_;
};

}  // namespace common_content_filter


#ifdef __cplusplus
extern "C"
{
#endif

void
test_func_c() {
  return common_content_filter::test_func_cpp();
}


void *
create_common_content_filter_factory() {
  common_content_filter::ContentFilterFactory * content_filter_factory =
    new common_content_filter::ContentFilterFactory;
  return content_filter_factory;
}


void
test_common_content_filter_factory(void * instance) {
  common_content_filter::ContentFilterFactory * content_filter_factory =
    static_cast<common_content_filter::ContentFilterFactory *>(instance);
  if (!content_filter_factory || content_filter_factory->magic() != common_content_filter::MAGIC ) {
    printf("the instance is not valid\n");
    return;
  }

  content_filter_factory->test();
}


void *
create_common_content_filter() {
  common_content_filter::ContentFilter * content_filter = new common_content_filter::ContentFilter;
  return content_filter;
}

void
test_common_content_filter(void * instance) {
  common_content_filter::ContentFilter * content_filter =
    static_cast<common_content_filter::ContentFilter *>(instance);
  if (!content_filter || content_filter->magic() != common_content_filter::MAGIC ) {
    printf("the instance is not valid\n");
    return;
  }

  content_filter->test();
}

#ifdef __cplusplus
}
#endif