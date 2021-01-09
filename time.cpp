#include "time.hpp"

namespace calc_obliquity {

// 定数
static constexpr unsigned int kJstOffset = 9;        // JST offset from UTC
static constexpr unsigned int kSecHour   = 3600;     // Seconds in a hour
static constexpr unsigned int kJ2000     = 2451545;  // Julian Day of 2000-01-01 12:00:00
static constexpr double       kJy        = 365.25;   // 1 Julian Year

/*
 * @brief      Constructor
 *
 * @param[in]  UTC (timespec)
 */
Time::Time(struct timespec ts) {
  try {
    this->ts = ts;
    this->jd = 0.0;
    this->t  = 0.0;
  } catch (...) {
    throw;
  }
}

/*
 * @brief   JST (日本標準時) 計算
 *
 * @return  JST (timespec)
 */
struct timespec Time::calc_jst() {
  try {
    return utc2jst(ts);
  } catch (...) {
    throw;
  }
}

/*
 * @brief   JD (ユリウス日) 計算
 *
 * @return  JD (double)
 */
double Time::calc_jd() {
  try {
    jd = gc2jd(ts);
    return jd;
  } catch (...) {
    throw;
  }
}

/*
 * @brief   T (ユリウス世紀数) 計算
 *
 * @return  T (double)
 */
double Time::calc_t() {
  try {
    if (jd == 0.0) jd = gc2jd(ts);
    t = jd2t(jd);
    return t;
  } catch (...) {
    throw;
  }
}

// -------------------------------------
// 以下、 private functions
// -------------------------------------

/*
 * @brief      UTC (協定世界時) -> JST (日本標準時)
 *
 * @param[in]  UTC (timespec)
 * @return     JST (timespec)
 */
struct timespec Time::utc2jst(struct timespec ts) {
  struct timespec ts_jst;

  try {
    ts_jst.tv_sec  = ts.tv_sec + kJstOffset * kSecHour;
    ts_jst.tv_nsec = ts.tv_nsec;
  } catch (...) {
    throw;
  }

  return ts_jst;
}

/*
 * @brief      GC (グレゴリオ暦) -> JD (ユリウス日)
 *
 * @param[in]  GC (timespec)
 * @return     JD (double)
 */
double Time::gc2jd(struct timespec ts) {
  struct tm t;
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
  double jd;

  try {
    localtime_r(&ts.tv_sec, &t);
    year  = t.tm_year + 1900;
    month = t.tm_mon + 1;
    day   = t.tm_mday;
    hour  = t.tm_hour;
    min   = t.tm_min;
    sec   = t.tm_sec;
    // 1月,2月は前年の13月,14月とする
    if (month < 3) {
      --year;
      month += 12;
    }
    // 日付(整数)部分
    jd = static_cast<int>(365.25 * year)
       + static_cast<int>(year / 400.0)
       - static_cast<int>(year / 100.0)
       + static_cast<int>(30.59 * (month - 2))
       + day
       + 1721088.5;
    // 時間(小数)部分
    jd += (sec / 3600.0 + min / 60.0 + hour) / 24.0;
    // 時間(ナノ秒)部分
    jd += ts.tv_nsec / 1000000000.0 / 3600.0 / 24.0;
  } catch (...) {
    throw;
  }

  return jd;
}

/*
 * @brief      JD (ユリウス日) -> T (ユリウス世紀数)
 *
 * @param[in]  JD (double)
 * @return     T (double
 */
double Time::jd2t(double jd_a) {
  double t;

  try {
    t = (jd_a - kJ2000) / (kJy * 100);
  } catch (...) {
    throw;
  }

  return t;
}

}  // namespace calc_obliquity

