#include "test_runner.h"

#define COMPARE                                                \
  for (int i = 0; i < 3; ++i) {                                \
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]); \
  }                                                            \
  ck_assert_int_eq(result_code, expected_result_code)

#define ADD_TEST_S21_FLOOR(testname) tcase_add_test(tc_floor_rounding, testname)

#define ADD_TEST_S21_ROUND(testname) tcase_add_test(tc_round, testname)

#define ADD_TEST_S21_TRUNCATE(testname) tcase_add_test(tc_truncate, testname)

#define ADD_TEST_S21_NEGATE(testname) tcase_add_test(tc_negate, testname)

// S21_FLOOR
START_TEST(test_floor_rounding_positive_number) {
  s21_decimal value;
  // 99999999999999999999.5
  value.bits[2] = 0x00000036;
  value.bits[1] = 0x35C9ADC5;
  value.bits[0] = 0xDE9FFFFB;

  value.bits[3] = 1 << 16;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 99999999999999999999
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x630FFFFF;

  expected_result.bits[3] = 0;

  COMPARE;
}
END_TEST

START_TEST(test_floor_rounding_negative_number) {
  s21_decimal value;
  // -99999999999999999999.5
  value.bits[2] = 0x00000036;
  value.bits[1] = 0x35C9ADC5;
  value.bits[0] = 0xDE9FFFFB;

  value.bits[3] = 1 << 16 | SIGN_MASK;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // -100000000000000000000
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x63100000;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}
END_TEST

START_TEST(test_floor_rounding_positive_integer) {
  s21_decimal value;
  // 99999999999999999999
  value.bits[2] = 0x00000005;
  value.bits[1] = 0x6BC75E2D;
  value.bits[0] = 0x630FFFFF;

  value.bits[3] = 0;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 99999999999999999999
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x630FFFFF;

  expected_result.bits[3] = 0;

  COMPARE;
}
END_TEST

START_TEST(test_floor_rounding_negative_integer) {
  s21_decimal value;
  // 99999999999999999999
  value.bits[2] = 0x00000005;
  value.bits[1] = 0x6BC75E2D;
  value.bits[0] = 0x630FFFFF;

  value.bits[3] = SIGN_MASK;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 99999999999999999999
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x630FFFFF;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}
END_TEST

START_TEST(test_floor_rounding_negative_zero_with_scale) {
  s21_decimal value;
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0;

  value.bits[3] = 2 << 16 | SIGN_MASK;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0;

  expected_result.bits[3] = 2 << 16 | SIGN_MASK;

  COMPARE;
}
END_TEST

START_TEST(test_s21_floor_rounding_negative_number_with_scale_v2) {
  s21_decimal value;
  // -12.34
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0x000004D2;

  value.bits[3] = 2 << 16 | SIGN_MASK;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // -13
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x0000000D;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}

START_TEST(test_s21_floor_rounding_negative_number_with_scale_v3) {
  s21_decimal value;
  // -999999.011
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0x3B9AC623;

  value.bits[3] = 3 << 16 | SIGN_MASK;

  int result_code = s21_floor(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // -1000000
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x000F4240;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}

START_TEST(test_s21_floor_unacceptable_pointer) {
  s21_decimal value;
  // -999999.011
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0x3B9AC623;

  value.bits[3] = 3 << 16 | SIGN_MASK;

  int expected_result_code = 1;
  int result_code = s21_floor(value, NULL);
  ck_assert_int_eq(expected_result_code, result_code);
}
// S21_ROUND:

START_TEST(test_s21_round_basic_case_1) {
  s21_decimal value;
  // 99999999999999999999.5
  value.bits[2] = 0x00000036;
  value.bits[1] = 0x35C9ADC5;
  value.bits[0] = 0xDE9FFFFB;

  value.bits[3] = 1 << 16;

  int result_code = s21_round(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 100000000000000000000
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x63100000;
  expected_result.bits[3] = 0;

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
}
END_TEST

START_TEST(test_s21_round_basic_case_2) {
  s21_decimal value;
  // -99999999999999999999.5
  value.bits[2] = 0x00000036;
  value.bits[1] = 0x35C9ADC5;
  value.bits[0] = 0xDE9FFFFB;

  value.bits[3] = 1 << 16 | SIGN_MASK;

  int result_code = s21_round(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // -100000000000000000000
  expected_result.bits[2] = 0x00000005;
  expected_result.bits[1] = 0x6BC75E2D;
  expected_result.bits[0] = 0x63100000;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}
END_TEST

START_TEST(test_s21_round_basic_case_3) {
  s21_decimal value;
  // 9999999999999999998.5
  value.bits[2] = 0x00000005;
  value.bits[1] = 0x6BC75E2D;
  value.bits[0] = 0x630FFFF1;

  value.bits[3] = 1 << 16;

  int result_code = s21_round(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 99999999999999999998
  expected_result.bits[2] = 0x00000000;
  expected_result.bits[1] = 0x8AC72304;
  expected_result.bits[0] = 0x89E7FFFE;

  expected_result.bits[3] = 0;

  COMPARE;
}

START_TEST(test_s21_round_basic_case_4) {
  s21_decimal value;
  // 999999.999
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0x3B9AC9FF;

  value.bits[3] = 3 << 16;

  int result_code = s21_round(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // 1000000
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x000F4240;

  expected_result.bits[3] = 0;

  COMPARE;
}

START_TEST(test_s21_round_basic_case_5) {
  s21_decimal value;
  // -12.34
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0x000004D2;

  value.bits[3] = 2 << 16 | SIGN_MASK;

  int result_code = s21_round(value, &value);
  int expected_result_code = 0;

  s21_decimal expected_result;
  // -12
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x0000000C;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}

START_TEST(test_s21_truncate_basic_case_1) {
  s21_decimal value;
  // 1298287672.357
  value.bits[2] = 0;
  value.bits[1] = 0x12E;
  value.bits[0] = 0x47F9BC25;

  value.bits[3] = 3 << 16;

  int expected_result_code = 0;
  int result_code = s21_truncate(value, &value);

  s21_decimal expected_result;
  // 1298287672
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x4D624C38;

  expected_result.bits[3] = 0;

  COMPARE;
}

START_TEST(test_s21_truncate_basic_case_2) {
  s21_decimal value;
  // -1298287672.357
  value.bits[2] = 0;
  value.bits[1] = 0x12E;
  value.bits[0] = 0x47F9BC25;

  value.bits[3] = 3 << 16 | SIGN_MASK;

  int expected_result_code = 0;
  int result_code = s21_truncate(value, &value);

  s21_decimal expected_result;
  // -1298287672
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x4D624C38;

  expected_result.bits[3] = SIGN_MASK;

  COMPARE;
}

START_TEST(test_s21_truncate_basic_case_3) {
  s21_decimal value;
  // 0.0001
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 1;

  value.bits[3] = 4 << 16;

  int expected_result_code = 0;
  int result_code = s21_truncate(value, &value);

  s21_decimal expected_result;
  // 0
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0;

  expected_result.bits[3] = 0;
  COMPARE;
}

START_TEST(test_s21_truncate_large_scale_value) {
  s21_decimal value;
  // 792.28162514264337593543950335
  value.bits[2] = 0xFFFFFFFF;
  value.bits[1] = 0xFFFFFFFF;
  value.bits[0] = 0xFFFFFFFF;

  value.bits[3] = 26 << 16;

  int expected_result_code = 0;
  int result_code = s21_truncate(value, &value);

  s21_decimal expected_result;
  // 792
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0x00000318;

  expected_result.bits[3] = 0;

  COMPARE;
}
// S21_NEGATE:
START_TEST(test_s21_negate_basic_test_1) {
  s21_decimal value;
  // 235867827412365232.349
  value.bits[2] = 0x0000000C;
  value.bits[1] = 0xC952DB59;
  value.bits[0] = 0xA92C48DD;

  value.bits[3] = 3 << 16;

  int expected_result_code = 0;
  int result_code = s21_negate(value, &value);

  s21_decimal expected_result;
  // -235867827412365232.349
  expected_result.bits[2] = 0x0000000C;
  expected_result.bits[1] = 0xC952DB59;
  expected_result.bits[0] = 0xA92C48DD;

  expected_result.bits[3] = 3 << 16 | SIGN_MASK;

  COMPARE;
}

START_TEST(test_s21_negate_basic_test_2) {
  s21_decimal value;
  // -235867827412365232.349
  value.bits[2] = 0x0000000C;
  value.bits[1] = 0xC952DB59;
  value.bits[0] = 0xA92C48DD;

  value.bits[3] = 3 << 16 | SIGN_MASK;

  int expected_result_code = 0;
  int result_code = s21_negate(value, &value);

  s21_decimal expected_result;
  // 235867827412365232.349
  expected_result.bits[2] = 0x0000000C;
  expected_result.bits[1] = 0xC952DB59;
  expected_result.bits[0] = 0xA92C48DD;

  expected_result.bits[3] = 3 << 16;

  COMPARE;
}

START_TEST(test_s21_negate_zero_value) {
  s21_decimal value;
  // 0test_s21_floor_unacceptable_pointer
  value.bits[2] = 0;
  value.bits[1] = 0;
  value.bits[0] = 0;

  value.bits[3] = 0;

  int expected_result_code = 0;
  int result_code = s21_negate(value, &value);

  s21_decimal expected_result;
  // 0
  expected_result.bits[2] = 0;
  expected_result.bits[1] = 0;
  expected_result.bits[0] = 0;

  expected_result.bits[3] = 0;

  COMPARE;
}

Suite* other_functions_suite(void) {
  Suite* s = suite_create("s21_decimal_other_functions_tests");

  // S21_FLOOR:
  TCase* tc_floor_rounding = tcase_create("s21_floor tests");

  ADD_TEST_S21_FLOOR(test_floor_rounding_positive_number);
  ADD_TEST_S21_FLOOR(test_floor_rounding_negative_number);
  ADD_TEST_S21_FLOOR(test_s21_floor_rounding_negative_number_with_scale_v2);
  ADD_TEST_S21_FLOOR(test_s21_floor_rounding_negative_number_with_scale_v3);
  ADD_TEST_S21_FLOOR(test_floor_rounding_positive_integer);
  ADD_TEST_S21_FLOOR(test_floor_rounding_negative_integer);
  ADD_TEST_S21_FLOOR(test_floor_rounding_negative_zero_with_scale);
  ADD_TEST_S21_FLOOR(test_s21_floor_unacceptable_pointer);
  // добавить тест с кодом возврата 1
  // придумать краевые случаи
  suite_add_tcase(s, tc_floor_rounding);

  // S21_ROUND:
  TCase* tc_round = tcase_create("s21_round tests");

  ADD_TEST_S21_ROUND(test_s21_round_basic_case_1);
  ADD_TEST_S21_ROUND(test_s21_round_basic_case_2);
  ADD_TEST_S21_ROUND(test_s21_round_basic_case_3);
  ADD_TEST_S21_ROUND(test_s21_round_basic_case_4);
  ADD_TEST_S21_ROUND(test_s21_round_basic_case_5);

  suite_add_tcase(s, tc_round);

  // S21_TRUNCATE:
  TCase* tc_truncate = tcase_create("s21_truncate tests");

  ADD_TEST_S21_TRUNCATE(test_s21_truncate_basic_case_1);
  ADD_TEST_S21_TRUNCATE(test_s21_truncate_basic_case_2);
  ADD_TEST_S21_TRUNCATE(test_s21_truncate_basic_case_3);
  ADD_TEST_S21_TRUNCATE(test_s21_truncate_large_scale_value);

  suite_add_tcase(s, tc_truncate);

  // S21_NEGATE:
  TCase* tc_negate = tcase_create("s21_negate tests");

  ADD_TEST_S21_NEGATE(test_s21_negate_basic_test_1);
  ADD_TEST_S21_NEGATE(test_s21_negate_basic_test_2);
  ADD_TEST_S21_NEGATE(test_s21_negate_zero_value);

  suite_add_tcase(s, tc_negate);

  return s;
}