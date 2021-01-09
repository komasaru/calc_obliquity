#ifndef CALC_OBLIQUITY_COMMON_HPP_
#define CALC_OBLIQUITY_COMMON_HPP_

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace calc_obliquity {

// -------------------------------------
//   Functions
// -------------------------------------
struct timespec jst2utc(struct timespec);
std::string gen_time_str(struct timespec);

}  // namespace calc_obliquity

#endif

