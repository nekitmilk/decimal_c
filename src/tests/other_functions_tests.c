#include "test_runner.h"

#define ADD_TEST_FLOOR_ROUNDING(testname) \
  tcase_add_test(tc_floor_rounding, testname);

// ROUND
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

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
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

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
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

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
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

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
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

  for (int i = 0; i < 3; ++i) {
    ck_assert_uint_eq(value.bits[i], expected_result.bits[i]);
  }
  ck_assert_int_eq(result_code, expected_result_code);
}
END_TEST

Suite* other_functions_suite(void) {
  Suite* s = suite_create("s21_decimal_other_functions_tests");

  // ROUND:
  TCase* tc_floor_rounding = tcase_create("Floor rounding tests");

  ADD_TEST_FLOOR_ROUNDING(test_floor_rounding_positive_number);
  ADD_TEST_FLOOR_ROUNDING(test_floor_rounding_negative_number);
  ADD_TEST_FLOOR_ROUNDING(test_floor_rounding_positive_integer);
  ADD_TEST_FLOOR_ROUNDING(test_floor_rounding_negative_integer);
  ADD_TEST_FLOOR_ROUNDING(test_floor_rounding_negative_zero_with_scale);
  // добавить тест с кодом возврата 1
  // придумать краевые случаи
  suite_add_tcase(s, tc_floor_rounding);

  return s;
}