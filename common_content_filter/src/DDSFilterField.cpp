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
bool
DDSFilterField::test_member(
  const void * untype_members,
  FieldAccessor& accessor,
  const void *& data)
{
  const MembersType * members = static_cast<const MembersType *>(untype_members);
  if (!members) {
    return false;
  }

  const auto member = members->members_ + accessor.member_index;

  logError(DDSSQLFILTER, "DDSFilterField::test_member "
      << " accessor.member_index:" << accessor.member_index
      << " accessor.array_index:" << accessor.array_index

        << " member member->name:" << member->name_
        << " member member->type_id_:" << static_cast<int>(member->type_id_)
        << " member member->offset_:" << member->offset_
        << " member member->is_array_:" << member->is_array_
        << " member member->array_size_:" << member->array_size_
        << " member member->is_upper_bound_:" << member->is_upper_bound_
    );

  // data = data+member->offset_
  if (member->is_array_) {
    data = member->get_function((void *)(data + member->offset_), accessor.array_index);
  } else {
    data = data + member->offset_;
  }

  return true;
}



bool DDSFilterField::set_value(
        const void * data,
        size_t n)
{
    // using namespace eprosima_common::fastrtps::types;

    // uint32_t index = static_cast<uint32_t>(access_path_[n].member_index);
    // auto member_id = data.get_member_id_at_index(index);
    bool last_step = access_path_.size() - 1 == n;
    void * addr;

    logError(DDSSQLFILTER, "DDSFilterField::set_value "
      << "access_path_.size():" << access_path_.size()
    );

    // for (auto & access_path : access_path_) {
    //   logError(DDSSQLFILTER, "DDSFilterField::set_value "
    //         << "      access_path.member_index:" << access_path.member_index
    //         << "      access_path.array_index:" << access_path.array_index
    //       );
    // }
    bool ret = false;

    const void * data_updated = data;
    const rosidl_message_type_support_t * type_support_intro = access_path_[n].type_support_intro;
    if (type_support_intro->typesupport_identifier ==
      rosidl_typesupport_introspection_c__identifier)
    {
      ret = test_member<rosidl_typesupport_introspection_c__MessageMembers>(
        type_support_intro->data, access_path_[n], data_updated);
    } else {
      ret = test_member<rosidl_typesupport_introspection_cpp::MessageMembers>(
        type_support_intro->data, access_path_[n], data_updated);
    }

    // if (access_path_[n].array_index < std::numeric_limits<size_t>::max())
    // {
    //     size_t array_index = access_path_[n].array_index;
    //     // member

    // }
    // else
    // {
        if (last_step)
        {
            ret = set_member(data_updated);
        }
        else
        {
            ret = set_value(data_updated, n + 1);
        }
    // }

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
        const void * data)
{
    // using namespace eprosima_common::fastrtps::types;

    bool ret = true;

    // just a test, content_filtering_subscriber use float msg
    // float_value = *(float*)data;

    logError(DDSSQLFILTER, "DDSFilterField::set_member "
      << "type_id_:" << type_id_
    );


    try
    {
        switch (type_id_)
        {
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN:
                // boolean_value = data->get_bool_value(member_id);
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WCHAR:
                // char_value = data->get_char8_value(member_id);
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WSTRING:
                // string_value = data->get_string_value(member_id);
                break;

            // case TK_INT16:
            //     signed_integer_value = data->get_int16_value(member_id);
            //     break;

            // case TK_INT32:
            //     signed_integer_value = data->get_int32_value(member_id);
            //     break;

            // case TK_INT64:
            //     signed_integer_value = data->get_int64_value(member_id);
            //     break;

            // case TK_BYTE:
            //     unsigned_integer_value = data->get_uint8_value(member_id);
            //     break;

            // case TK_UINT16:
            //     unsigned_integer_value = data->get_uint16_value(member_id);
            //     break;

            // case TK_UINT32:
            //     unsigned_integer_value = data->get_uint32_value(member_id);
            //     break;

            // case TK_UINT64:
            //     unsigned_integer_value = data->get_uint64_value(member_id);
            //     break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
                unsigned_integer_value = *(int8_t*)data;
                break;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT:
                float_value = *(float*)data;
                break;

            // case TK_FLOAT64:
            //     float_value = data->get_float64_value(member_id);
            //     break;

            // case TK_FLOAT128:
            //     float_value = data->get_float128_value(member_id);
            //     break;

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
