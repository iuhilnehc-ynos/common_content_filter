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
 * @file identifiers.hpp
 *
 * Note: this is an implementation file, designed to be included inside the
 * DDSFilterExpressionParser.hpp file of the parent folder.
 */


struct CurrentIdentifierState
{
    // TODO. const rosidl_message_type_support_t * type_support,
    const rosidl_message_type_support_t * type_support;

    // ros2 primitive type
    const rosidl_message_type_support_t * current_type_support;
    uint8_t current_type;

    std::vector<DDSFilterField::FieldAccessor> access_path;
};


const rosidl_message_type_support_t *
get_type_support_introspection(
  const rosidl_message_type_support_t * type_supports)
{
  const rosidl_message_type_support_t * type_support =
    get_message_typesupport_handle(
    type_supports, rosidl_typesupport_introspection_c__identifier);
  if (nullptr == type_support) {
    rcutils_error_string_t prev_error_string = rcutils_get_error_string();
    rcutils_reset_error();

    type_support =
      get_message_typesupport_handle(
      type_supports,
      rosidl_typesupport_introspection_cpp::typesupport_identifier);
    if (nullptr == type_support) {
      rcutils_error_string_t error_string = rcutils_get_error_string();
      rcutils_reset_error();
      RMW_SET_ERROR_MSG_WITH_FORMAT_STRING(
        "Type support not from this implementation. Got:\n"
        "    %s\n"
        "    %s\n"
        "while fetching it",
        prev_error_string.str, error_string.str);
      return nullptr;
    }
  }

  return type_support;
}


template<typename MembersType>
void GetTypeIdentifier(
  CurrentIdentifierState& identifier_state,
  const std::string & parse_node_name,
  const MembersType * members, uint32_t index)
{
  const auto member = members->members_ + index;
  std::string name = member->name_;

  if (name == parse_node_name) {
    logError(DDSSQLFILTER, "find the parse_node_name: " << parse_node_name
      << " type_id:" << static_cast<int>(member->type_id_)
      << " member index:" << index
      << " member member->offset_:" << member->offset_
      << " member member->is_array_:" << member->is_array_
      << " member member->array_size_:" << member->array_size_
      << " member member->is_upper_bound_:" << member->is_upper_bound_
      );
    // some important information should be saved in the identifier_state
    identifier_state.current_type = member->type_id_;

    if (member->type_id_ == ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE) {

        // const rosidl_message_type_support_t * type_support_intro =
        //   get_type_support_introspection(member->members_);
        // const MembersType * sub_members =
        //   static_cast<const MembersType *>(type_support_intro->data);

      identifier_state.current_type_support = member->members_;
    }

    identifier_state.current_type_support = member->members_;

  }

  std::string type_name;
  switch (member->type_id_) {
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT:
      {
        type_name = "float";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE:
      {
        type_name = "double";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_LONG_DOUBLE:
      {
        type_name = "longdouble";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
      {
        type_name = "char";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WCHAR:
      {
        type_name = "wchar";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN:
      {
        type_name = "bool";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_OCTET:
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
      {
        type_name = "uint8_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
      {
        type_name = "int8_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
      {
        type_name = "uint16_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
      {
        type_name = "int16_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
      {
        type_name = "uint32_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
      {
        type_name = "int32_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
      {
        type_name = "uint64_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
      {
        type_name = "int64_t";
        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WSTRING:
      {
        uint32_t bound = member->string_upper_bound_ ?
          static_cast<uint32_t>(member->string_upper_bound_) : 255;
        bool wide =
          (member->type_id_ == ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING) ?
          false : true;

        break;
      }
    case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
      {
        const rosidl_message_type_support_t * type_support_intro =
          get_type_support_introspection(member->members_);
        const MembersType * sub_members =
          static_cast<const MembersType *>(type_support_intro->data);

      }
      break;
    default:
      break;
  }

  logError(DDSSQLFILTER, "GetTypeIdentifier name:" << name << " type_name:" << type_name);
}

template<typename MembersType>
inline bool
add_type_object(
  CurrentIdentifierState& identifier_state,
  std::unique_ptr< ParseNode >& n,
  const void * untype_members,
  size_t & member_index,
  size_t & array_index)
{
  const MembersType * members = static_cast<const MembersType *>(untype_members);
  if (!members) {
    return false;
  }

  const ParseNode& name_node = n->left();
  std::string parse_node_name = name_node.content();

  for (uint32_t i = 0; i < members->member_count_; ++i) {
    // GetTypeIdentifier(identifier_state, parse_node_name, members, i);

    const auto member = members->members_ + i;
    std::string name = member->name_;

    if (name == parse_node_name) {

      member_index = i;

      logError(DDSSQLFILTER, "find the parse_node_name: " << parse_node_name
        << " type_id:" << static_cast<int>(member->type_id_)
        << " member index:" << i
        << " member member->offset_:" << member->offset_
        << " member member->is_array_:" << member->is_array_
        << " member member->array_size_:" << member->array_size_
        << " member member->is_upper_bound_:" << member->is_upper_bound_
        );

        bool has_index = n->children.size() > 1;
        if (member->is_array_)
        {
            if (!has_index)
            {
                throw parse_error("field should have an index (i.e. [n])", n->left().end());
            }

            array_index = static_cast<size_t>(std::stoul(n->right().left().content()));



            if (member->array_size_ && !member->is_upper_bound_) {
              if (member->array_size_ <= array_index)
              {
                  throw parse_error("index is greater than maximum size", n->right().end());
              }
            }


        }
        else
        {
            if (has_index)
            {
                throw parse_error("field is not an array or sequence", n->right().begin());
            }
        }


      // some important information should be saved in the identifier_state
      identifier_state.current_type = member->type_id_;


      if (member->type_id_ == ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE) {

          // const rosidl_message_type_support_t * type_support_intro =
          //   get_type_support_introspection(member->members_);
          // const MembersType * sub_members =
          //   static_cast<const MembersType *>(type_support_intro->data);

        identifier_state.current_type_support = member->members_;
      }



      return true;
    }
  }
  throw parse_error("field not found", name_node.begin());

  return false;
}

bool test_type_support(
  CurrentIdentifierState& identifier_state,
  std::unique_ptr< ParseNode >& n,
  const rosidl_message_type_support_t * type_supports,
  size_t & member_index,
  size_t & array_index,
  const rosidl_message_type_support_t *& type_support_intro)
{
  type_support_intro =
    get_type_support_introspection(type_supports);
  if (!type_support_intro) {
    return false;
  }

  bool ret = false;
  logError(DDSSQLFILTER, "test_type_support: " << type_support_intro->typesupport_identifier);
  if (type_support_intro->typesupport_identifier ==
    rosidl_typesupport_introspection_c__identifier)
  {
    ret = add_type_object<rosidl_typesupport_introspection_c__MessageMembers>(
      identifier_state, n, type_support_intro->data,
      member_index, array_index);
  } else {
    ret = add_type_object<rosidl_typesupport_introspection_cpp::MessageMembers>(
      identifier_state, n, type_support_intro->data,
      member_index, array_index);
  }

  return ret;
}




struct identifier_processor
    : parse_tree::apply< identifier_processor >
{
    template<typename _TSize>
    static constexpr size_t process_bound(
            _TSize bound)
    {
        return 0 == bound ? std::numeric_limits<size_t>::max() : static_cast<size_t>(bound);
    }

    template<typename _BSeq>
    static size_t process_bounds(
            const _BSeq& bound_seq)
    {
        if (1 != bound_seq.size())
        {
            return 0;
        }

        return process_bound(bound_seq[0]);
    }

    // static bool type_should_be_indexed(
    //         CurrentIdentifierState& identifier_state,
    //         const TypeIdentifier*& out_type,
    //         size_t& max_size)
    // {
    //     max_size = 0;

    //     switch (identifier_state.current_type_support)
    //     {
    //         case TI_PLAIN_ARRAY_SMALL:
    //             out_type = ti.array_sdefn().element_identifier();
    //             max_size = process_bounds(ti.array_sdefn().array_bound_seq());
    //             return true;

    //         case TI_PLAIN_ARRAY_LARGE:
    //             out_type = ti.array_ldefn().element_identifier();
    //             max_size = process_bounds(ti.array_ldefn().array_bound_seq());
    //             return true;

    //         case TI_PLAIN_SEQUENCE_SMALL:
    //             out_type = ti.seq_sdefn().element_identifier();
    //             max_size = process_bound(ti.seq_sdefn().bound());
    //             return true;

    //         case TI_PLAIN_SEQUENCE_LARGE:
    //             out_type = ti.seq_ldefn().element_identifier();
    //             max_size = process_bound(ti.seq_ldefn().bound());
    //             return true;
    //     }

    //     out_type = &ti;
    //     return false;
    // }

    static void add_member_access(
            std::unique_ptr< ParseNode >& n,
            CurrentIdentifierState& identifier_state,
            const rosidl_message_type_support_t * type_support)
    {
        // if (TK_STRUCTURE != complete._d())
        // {
        //     throw parse_error("trying to access field on a non-struct type", n->begin());
        // }

        /*
            idl
            msg1
               string data

            msg2
               string data
               msg1   m1;

            msg2 topic message type, and use  `m1.data = 'test'`

        */

        // const ParseNode& name_node = n->left();
        // std::string name = name_node.content();

        // logError(DDSSQLFILTER, "identifiers add_member_access: " << name);



        size_t member_index = 0;
        // const CompleteStructMemberSeq& members = complete.struct_type().member_seq();
        // for (member_index = 0; member_index < members.size(); ++member_index)
        // {
        //     if (members[member_index].detail().name() == name)
        //     {
        //         break;
        //     }
        // }

        // if (member_index == members.size())
        // {
        //     throw parse_error("field not found", name_node.begin());
        // }

        // const TypeIdentifier& ti = members[member_index].common().member_type_id();
        // bool has_index = n->children.size() > 1;
        size_t max_size = 0;
        size_t array_index = std::numeric_limits<size_t>::max();

        bool is_array;
        size_t array_size;
        // try to get member_index, is_array,
        // TODO. just return the members

        const rosidl_message_type_support_t * type_support_intro;


        bool ret = test_type_support(identifier_state, n, type_support,
          member_index, array_index, type_support_intro);

        // if (is_array)
        // {
        //     if (!has_index)
        //     {
        //         throw parse_error("field should have an index (i.e. [n])", n->left().end());
        //     }

        //     array_index = static_cast<size_t>(std::stoul(n->right().left().content()));
        //     if (array_size <= array_index)
        //     {
        //         throw parse_error("index is greater than maximum size", n->right().end());
        //     }
        // }
        // else
        // {
        //     if (has_index)
        //     {
        //         throw parse_error("field is not an array or sequence", n->right().begin());
        //     }
        // }

        logError(DDSSQLFILTER, "DDSFilterField::FieldAccessor:"
          << member_index << " "
          << array_index << " ");
        identifier_state.access_path.emplace_back(DDSFilterField::FieldAccessor{ member_index, array_index, type_support_intro });
    }

    static DDSFilterValue::ValueKind get_value_kind(
            uint8_t type_id,
            const position& pos)
    {
      logError(DDSSQLFILTER, "get_value_kind type_id:" << static_cast<uint32_t>(type_id));
        switch (type_id)
        {
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN:
                return DDSFilterValue::ValueKind::BOOLEAN;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WCHAR:
                return DDSFilterValue::ValueKind::CHAR;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_WSTRING:
                return DDSFilterValue::ValueKind::STRING;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
                return DDSFilterValue::ValueKind::SIGNED_INTEGER;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_OCTET:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
                return DDSFilterValue::ValueKind::UNSIGNED_INTEGER;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT:
                return DDSFilterValue::ValueKind::FLOAT_FIELD;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE:
                return DDSFilterValue::ValueKind::DOUBLE_FIELD;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_LONG_DOUBLE:
                return DDSFilterValue::ValueKind::LONG_DOUBLE_FIELD;

            case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
                break;

        }


        // throw parse_error("type is not primitive", pos);
    }

    template< typename ... States >
    static void transform(
            std::unique_ptr< ParseNode >& n,
            CurrentIdentifierState& state,
            States&&... /*st*/)
    {
        logError(DDSSQLFILTER, "identifiers transform name:" << n->content());

        if (n->is<fieldname>())
        {
          logError(DDSSQLFILTER, "identifiers transform n->is<fieldname>()");

            // Set data for fieldname node
            n->field_kind = get_value_kind(state.current_type, n->end());
            n->field_access_path = state.access_path;
            n->type_id = state.current_type;

            // Reset parser state
            state.access_path.clear();
            state.current_type = 0;
            state.current_type_support = nullptr;

        }
        else
        {
            if (0 == state.current_type)
            {
              logError(DDSSQLFILTER, "identifiers transform 0 == state.current_type");

                add_member_access(n, state, state.type_support);
            }
            else
            {
              logError(DDSSQLFILTER, "identifiers transform 0 != state.current_type");

                // if (EK_COMPLETE != state.current_type->_d())
                // {
                //     throw parse_error("trying to access field on a non-complete type", n->begin());
                // }

                // const TypeObject* type_object = TypeObjectFactory::get_instance()->get_type_object(state.current_type);
                // if (nullptr == type_object)
                // {
                //     throw parse_error("could not find type object definition", n->begin());
                // }

                add_member_access(n, state, state.current_type_support);
            }
        }

        n->children.clear();
    }

};
