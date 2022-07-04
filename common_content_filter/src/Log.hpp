// license

#ifndef COMMON_CONTENT_FILTER__LOG_HPP_
#define COMMON_CONTENT_FILTER__LOG_HPP_

// TODO. use rcutil log instead
#include <iostream>

#define common_content_filter_log_(cat, msg)                                                                              \
    {                                                                                                   \
            std::stringstream log_ss_tmp__;                                                     \
            log_ss_tmp__ << msg;                                                                \
            std::cout << "[" << #cat << "] " << log_ss_tmp__.str() << std::endl;                 \
    }

#define logDebug common_content_filter_log_
#define logInfo common_content_filter_log_
#define logWarning common_content_filter_log_
#define logError common_content_filter_log_

#endif  // COMMON_CONTENT_FILTER__LOG_HPP_