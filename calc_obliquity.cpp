/***********************************************************
  黄道傾斜角計算

    DATE          AUTHOR          VERSION
    2020.10.30    mk-mode.com     1.00 新規作成

  Copyright(C) 2020 mk-mode.com All Rights Reserved.

  ----------------------------------------------------------
  引数 : JST（日本標準時）
           書式：最大23桁の数字
                 （先頭から、西暦年(4), 月(2), 日(2), 時(2), 分(2), 秒(2),
                             1秒未満(9)（小数点以下9桁（ナノ秒）まで））
                 無指定なら現在(システム日時)と判断。
***********************************************************/
#include "common.hpp"
#include "time.hpp"
#include "obliquity.hpp"

#include <cmath>
#include <cstdlib>   // for EXIT_XXXX
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char* argv[]) {
  // 定数
  static constexpr double kPi = atan(1.0) * 4.0;

  // 変数
  std::string     tm_str;  // time string
  unsigned int    s_tm;    // size of time string
  int             s_nsec;  // size of nsec string
  int             ret;     // return of functions
  struct timespec jst;     // JST
  struct timespec utc;     // UTC
  struct tm       t = {};  // for work
  double          jcn;     // Julian Century Number
  double          eps;     // 黄道傾斜角
  namespace       ns = calc_obliquity;

  try {
    // 日付取得
    if (argc > 1) {
      // コマンドライン引数より取得
      tm_str = argv[1];
      s_tm = tm_str.size();
      if (s_tm > 23) {
        std::cout << "[ERROR] Over 23-digits!" << std::endl;
        return EXIT_FAILURE;
      }
      s_nsec = s_tm - 14;
      std::istringstream is(tm_str);
      is >> std::get_time(&t, "%Y%m%d%H%M%S");
      jst.tv_sec  = mktime(&t);
      jst.tv_nsec = 0;
      if (s_tm > 14) {
        jst.tv_nsec = std::stod(
            tm_str.substr(14, s_nsec) + std::string(9 - s_nsec, '0'));
      }
    } else {
      // 現在日時の取得
      ret = std::timespec_get(&jst, TIME_UTC);
      if (ret != 1) {
        std::cout << "[ERROR] Could not get now time!" << std::endl;
        return EXIT_FAILURE;
      }
    }

    // JST -> UTC
    utc = ns::jst2utc(jst);

    // ユリウス世紀数計算
    ns::Time o_tm(utc);
    jcn = o_tm.calc_t();

    // 黄道傾斜角計算
    ns::Obliquity o_ob;
    eps = o_ob.calc_ob(jcn);

    // 結果出力
    std::cout << "JST: "
              << ns::gen_time_str(jst) << std::endl;
    std::cout << "UTC: "
              << ns::gen_time_str(utc) << std::endl;
    std::cout << " JD: "
              << std::fixed << std::setprecision(10) << o_tm.calc_jd()
              << " day" << std::endl;
    std::cout << "  T: "
              << std::fixed << std::setprecision(10) << jcn
              << " century (= Julian Century Number)" << std::endl;
    std::cout << "EPS:  "
              << std::fixed << std::setprecision(10) << eps
              << " rad." << std::endl;
    std::cout << "     "
              << std::fixed << std::setprecision(10) << eps * 180.0 / kPi
              << " deg." << std::endl;
  } catch (...) {
      std::cerr << "EXCEPTION!" << std::endl;
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

