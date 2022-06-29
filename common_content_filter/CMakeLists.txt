cmake_minimum_required(VERSION 3.8)
project(common_content_filter)


if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rcutils REQUIRED)
find_package(rcpputils REQUIRED)
find_package(rmw REQUIRED)
find_package(rosidl_typesupport_introspection_c REQUIRED)
find_package(rosidl_typesupport_introspection_cpp REQUIRED)

add_library(${PROJECT_NAME}_library SHARED
  src/DDSFilterCompoundCondition.cpp
  src/DDSFilterExpression.cpp
  src/DDSFilterExpressionParser.cpp
  src/DDSFilterFactory.cpp
  src/DDSFilterField.cpp
  src/DDSFilterParameter.cpp
  src/DDSFilterPredicate.cpp
  src/DDSFilterValue.cpp
  src/test.cpp
  )

set_target_properties(${PROJECT_NAME}_library
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
# target_link_libraries(${PROJECT_NAME}_library PUBLIC
#   rcutils::rcutils
#   rmw::rmw
#   )
# target_link_libraries(${PROJECT_NAME}_library PRIVATE
#   rcpputils::rcpputils)
target_include_directories(${PROJECT_NAME}_library
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
  PRIVATE
  ${PROJECT_SOURCE_DIR}/thirdparty/taocpp-pegtl
  )

ament_target_dependencies(${PROJECT_NAME}_library
  "rcpputils"
  "rcutils"
  "rmw"
  "rosidl_typesupport_introspection_c"
  "rosidl_typesupport_introspection_cpp"
)
# # Wait for all rosidl generators to finish before building this library
# add_dependencies(${PROJECT_NAME}_library
#   ${PROJECT_NAME})

# Depend on the generated C++ messages
# rosidl_get_typesupport_target(cpp_typesupport_target "${PROJECT_NAME}" "rosidl_typesupport_cpp")
# target_link_libraries(${PROJECT_NAME}_library PUBLIC
#   ${cpp_typesupport_target})

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(${PROJECT_NAME}_library
  PRIVATE "COMMON_CONTENT_FILTER_BUILDING_LIBRARY")

install(
  TARGETS ${PROJECT_NAME}_library EXPORT ${PROJECT_NAME}_library
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export old-style CMake variables
ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME}_library)

# Export modern CMake targets
ament_export_targets(${PROJECT_NAME}_library)

ament_export_dependencies(ament_cmake_core)
ament_export_dependencies(rcpputils)
ament_export_dependencies(rcutils)
ament_export_dependencies(rmw)
ament_export_dependencies(rosidl_typesupport_introspection_c)
ament_export_dependencies(rosidl_typesupport_introspection_cpp)

install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
