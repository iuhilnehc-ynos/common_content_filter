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
 * @file DDSFilterField.hpp
 */

#ifndef COMMON_CONTENT_FILTER__DDSFILTERFIELD_HPP_
#define COMMON_CONTENT_FILTER__DDSFILTERFIELD_HPP_

#include <cassert>
#include <unordered_set>
#include <vector>

#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"

#include "DDSFilterPredicate.hpp"
#include "DDSFilterValue.hpp"

namespace eprosima_common {
namespace fastdds {
namespace dds {
namespace DDSSQLFilter {

/**
 * A DDSFilterValue for fieldname-based expression values.
 */
class DDSFilterField final : public DDSFilterValue
{

public:

    /**
     * An element on the access path to the final field.
     */
    struct FieldAccessor final
    {
        /// Index of the member to access
        size_t member_index;

        /// Element index for array / sequence members
        size_t array_index;

        /// Type support introspection information for current field
        const rosidl_message_type_support_t * type_support_introspection;
    };

    /**
     * Construct a DDSFilterField.
     *
     * @param[in]  type_id       TypeIdentifier representing the primitive data type of the fieldname.
     * @param[in]  access_path   Access path to the field.
     * @param[in]  data_kind     Kind of data the field represents.
     */
    DDSFilterField(
            uint8_t type_id,
            const std::vector<FieldAccessor>& access_path,
            ValueKind data_kind)
        : DDSFilterValue(data_kind)
        , access_path_(access_path)
        , type_id_(type_id)
    {
    }

    virtual ~DDSFilterField() = default;

    /**
     * This method is used by a DDSFilterPredicate to check if this DDSFilterField can be used.
     *
     * @return whether this DDSFilterField has a value that can be used on a predicate.
     */
    inline bool has_value() const noexcept final
    {
        return has_value_;
    }

    /**
     * Instruct this value to reset.
     */
    inline void reset() noexcept final
    {
        has_value_ = false;
    }


    /**
     * Perform the deserialization of the field represented by this DDSFilterField.
     * Will notify the predicates where this DDSFilterField is being used.
     *
     * @param[in]  data  The dynamic representation of the payload being filtered.
     *
     * @return Whether the deserialization process succeeded.
     *
     * @post Method @c has_value returns true.
     */
    inline bool set_value(
            const void * data_value)
    {
        return set_value(data_value, (size_t)0);
    }

    /**
     * Perform the deserialization of a specific step of the access path.
     *
     * @param[in]  data  The dynamic representation of the step being processed.
     * @param[in]  n     The step on the access path being processed.
     *
     * @return Whether the deserialization process succeeded.
     *
     * @post Method @c has_value returns true.
     */
    bool set_value(
            const void * data_value,
            size_t n);

protected:

    inline void add_parent(
            DDSFilterPredicate* parent) final
    {
        assert(nullptr != parent);
        parents_.emplace(parent);
    }

private:

    template<typename MembersType>
    bool
    get_msg_data_address(
      const void * untype_members,
      FieldAccessor& accessor,
      const void *& data);

    bool set_member(
            const void * data_value,
            bool is_c_type_support);

    bool has_value_ = false;
    std::vector<FieldAccessor> access_path_;
    uint8_t type_id_ = 0;
    std::unordered_set<DDSFilterPredicate*> parents_;
};

}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common

#endif  // COMMON_CONTENT_FILTER__DDSFILTERFIELD_HPP_
