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
 * @file DDSFilterExpression.cpp
 */

#include "DDSFilterExpression.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DDSFilterCondition.hpp"
#include "DDSFilterField.hpp"
#include "DDSFilterParameter.hpp"

#include "Log.hpp"

namespace eprosima_common {
namespace fastdds {
namespace dds {
namespace DDSSQLFilter {

bool DDSFilterExpression::evaluate(
        const void * payload) const
{
    // logDebug(DDSSQLFILTER, "DDSFilterExpression::evaluate fields size:" << fields.size());

    root->reset();
    for (auto it = fields.begin();
            it != fields.end() && DDSFilterConditionState::UNDECIDED == root->get_state();
            ++it)
    {
        if (!it->second->set_value(payload))
        {
            return false;
        }
    }

    return DDSFilterConditionState::RESULT_TRUE == root->get_state();
}

void DDSFilterExpression::clear()
{
    parameters.clear();
    fields.clear();
    root.reset();
}


}  // namespace DDSSQLFilter
}  // namespace dds
}  // namespace fastdds
}  // namespace eprosima_common
