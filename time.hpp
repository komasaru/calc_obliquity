#ifndef CALC_OBLIQUITY_TIME_HPP_
#define CALC_OBLIQUITY_TIME_HPP_

#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace calc_obliquity {

class Time {
  struct timespec ts;                        // timespec of UTC
  double jd;                                 // JD (ユリウス日)
  double t;                                  // T (ユリウス世紀数)

public:
  Time(struct timespec);                     // コンストラクタ
  struct timespec calc_jst();                // 計算: JST  (日本標準時)
  double calc_jd();                          // 計算: JD   (ユリウス日)
  double calc_t();                           // 計算: T    (ユリウス世紀数)

private:
  struct timespec utc2jst(struct timespec);  // UTC -> JST
  double gc2jd(struct timespec);             // GC  -> JD
  double jd2t(double);                       // JD  -> T
};

}  // namespace calc_obliquity

#endif

