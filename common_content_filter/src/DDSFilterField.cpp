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
 * @file DDSFilterField.cpp
 */

#include "DDSFilterField.hpp"

#include <cassert>
#include <unordered_set>
#include <vector>

// #include <fastdds/rtps/common/SerializedPayload.h>
// #include <fastrtps/types/DynamicData.h>
// #include <fastrtps/types/TypeIdentifier.h>
// #include <fastrtps/types/TypesBase.h>

#include "rosidl_typesupport_introspection_cpp/field_types.hpp"

#include "DDSFilterPredicate.hpp"
#include "DDSFilterValue.hpp"

#include "Log.hpp"



namespace eprosima_common {
namespace fastdds {
namespace dds {
namespace DDSSQLFilter {


template<typename MembersType>
void
DDSFilterField::get_msg_data_address(
  const void * untype_members,
  FieldAccessor& accessor,
  const void *& data)
{
  const MembersType * members = static_cast<const MembersType *>(untype_members);
  if (!members) {
    throw std::runtime_error("The data in the type support introspection is invalid.");
  }

  const auto member = members->members_ + accessor.member_index;
  // logDebug(DDSSQLFILTER, "DDSFilterField::get_msg_data_address "
  //       << " accessor.member_index:" << accessor.member_index
  //       << " accessor.array_index:" << accessor.array_index
  //       << " member member->name:" << member->name_
  //       << " member member->type_id_:" << static_cast<int>(member->type_id_)
  //       << " member member->offset_:" << member->offset_
  //       << " member member->is_array_:" << member->is_array_
  //       << " member member->array_size_:" << member->array_size_
  //       << " member member->is_upper_bound_:" << member->is_upper_bound_
  //   );

  uint64_t addr = reinterpret_cast<uint64_t>(data);
  if (member->is_array_) {
    size_t array_size = member->array_size_;
    if (array_size == 0) {
      array_size = member->size_function(
        reinterpret_cast<void *>(addr + member->offset_));
    }

    if (accessor.array_index >= array_size) {
      throw std::runtime_error("The array index should be less than array size.");
    }

    data = member->get_function(
      reinterpret_cast<void *>(addr + member->offset_), accessor.array_index);
  } else {
    data = reinterpret_cast<void *>(addr + member->offset_);
  }
}

bool DDSFilterField::set_value(
        const void * data,
        size_t n)
{
    bool last_step = access_path_.size() - 1 == n;
    bool ret = false;
    const void * addr = data;
    bool is_c_type_support;

    // logDebug(
    //   DDSSQLFILTER,
    //   "DDSFilterField::set_value " << "access_path size:" << access_path_.size()
    // );

    const rosidl_message_type_support_t * type_support_introspection = access_path_[n].type_support_intro;
    if (type_support_introspection->typesupport_identifier ==
      rosidl_typesupport_introspection_c__identifier)
    {
      is_c_type_support = true;
      get_msg_data_address<rosidl_typesupport_introspection_c__MessageMembers>(
        type_support_introspection->data, access_path_[n], addr);

    } else {
      is_c_type_support = false;
      get_msg_data_address<rosidl_typesupport_introspection_cpp::MessageMembers>(
        type_support_introspection->data, access_path_[n], addr);
    }

    if (last_step)
    {
        ret = set_member(addr, is_c_type_support);
    }
    else
    {
        ret = set_value(addr, n + 1);
    }

    if (ret && last_step)
    {
        has_value_ = true;
        value_has_changed();

        // Inform parent predicates
        for (DDSFilterPredicate* parent : parents_)
        {
            parent->value_has_changed();
        }
    }

    return ret;
}

bool DDSFilterField::set_member(
        const void * data,
        bool is_c_type_support)
{
    bool ret = true;
    // logDebug(
    //   DDSSQLFILTER,
    //   "DDSFilterField::set_member " << "type_id_:" << (uint32_t)type_id_;
    // );

    try
    {
        switch (type_id_)
        {
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT:
                float_value = *(float*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE:
                float_value = *(double*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_LONG_DOUBLE:
                float_value = *(long double*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
                char_value = *(char*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN:
                boolean_value = *(bool*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_OCTET:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
                unsigned_integer_value = *(uint8_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
                signed_integer_value = *(int8_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
                unsigned_integer_value = *(uint16_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
                signed_integer_value = *(int16_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
                unsigned_integer_value = *(uint32_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
                signed_integer_value = *(int32_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
                unsigned_integer_value = *(uint64_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
                signed_integer_value = *(int64_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:

                if (is_c_type_support) {
                  strncpy(string_value, (char*)data, sizeof(string_value));
                } else {
                  auto string = reinterpret_cast<const std::string *>(data);
                  strncpy(string_value, (char*)string->c_str(), sizeof(string_value));
                }
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WCHAR:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WSTRING:
            default:
                ret = false;
                break;
        }
    }
    catch (...)
    {
        ret = false;
    }

    return ret;
}

}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common
