#include "test_runner.h"

#define ASSERT ck_assert_uint_eq(expected_result, result);

#define ADD_TEST_IS_LESS(testname) tcase_add_test(tc_is_less, testname)

#define ADD_TEST_IS_LESS_OR_EQUAL(testname) \
  tcase_add_test(tc_is_less_or_equal, testname)

#define ADD_TEST_IS_GREATER(testname) tcase_add_test(tc_is_greater, testname)

#define ADD_TEST_IS_GREATER_OR_EQUAL(testname) \
  tcase_add_test(tc_is_greater_or_equal, testname)

#define ADD_TEST_EDGE_CASES(testname) tcase_add_test(tc_edge_cases, testname)

#define ADD_TEST_IS_EQUAL(testname) tcase_add_test(tc_is_equal, testname)

#define ADD_TEST_IS_NOT_EQUAL(testname) \
  tcase_add_test(tc_is_not_equal, testname)

// IS LESS
START_TEST(test_is_less_both_positive_no_scales) {
  s21_decimal value_1;
  // 12345678987654321
  value_1.bits[2] = 0x00000000;
  value_1.bits[1] = 0x002BDC54;
  value_1.bits[0] = 0x6291F4B1;

  value_1.bits[3] = 0x00000000;

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_one_negative_no_scales) {
  s21_decimal value_1;
  // -12345678987654321
  value_1.bits[2] = 0x00000000;  // старшие 32 бита
  value_1.bits[1] = 0x002BDC54;  // средние 32 бита
  value_1.bits[0] = 0x6291F4B1;  // младшие 32 бита
  value_1.bits[3] = SIGN_MASK;   // отрицательное число

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_high_bits_equal_but_mid_bits_not) {
  s21_decimal value_1 = {{0, 0xBBBBBBBB, 0x2ED234F5, 0}};
  s21_decimal value_2 = {{0, 0xAAAAAAAA, 0x2ED234F5, 0}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_both_positive_with_scales) {
  s21_decimal value_1 = {
      {0xF974E2D2, 0x1B959112, 0, 18 << 16}};  // 1.987654321234567890
  s21_decimal value_2 = {
      {0xFB985F15, 0x1D7808B2, 0, 19 << 16}};  // 0.2123456789123456789

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_with_barely_equal_values) {
  s21_decimal value_1 = {
      {0xCD76F24E, 0x1E839582, 0, 0}};  // 2198765432112345678
  s21_decimal value_2 = {
      {0x4276A677, 0xEB666718, 0x0000000B, 2 << 16}};  // 2198765432112345677.99

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_both_negative) {
  s21_decimal value_1;
  // -12345678987654321
  value_1.bits[2] = 0x00000000;
  value_1.bits[1] = 0x002BDC54;
  value_1.bits[0] = 0x6291F4B1;
  value_1.bits[3] = SIGN_MASK;

  //  -12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, SIGN_MASK}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_both_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_both_positive_and_one_number_has_scale) {
  // 123456789876.54321
  s21_decimal value_1;
  value_1.bits[2] = 0x00000000;
  value_1.bits[1] = 0x002BDC54;
  value_1.bits[0] = 0x6291F4B1;
  value_1.bits[3] = 5 << 16;

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_both_equal_but_one_of_them_with_a_scale) {
  // 123456789123.45678
  s21_decimal value_1 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 9 << 16}};

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(
    test_is_less_both_equal_but_one_of_them_with_a_scale_and_other_one_is_negative) {
  // 123456789123.45678
  s21_decimal value_1 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 9 << 16}};

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, SIGN_MASK}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_with_two_values_with_scale) {
  //  1234567.89
  s21_decimal value_1 = {{0, 0, 0x075BCD15, 2 << 16}};

  // 1234567.9
  s21_decimal value_2 = {{0, 0, 0x00BC614F, 1 << 16}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_int_eq(expected_result, result);

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ck_assert_int_eq(expected_result, result);
}
END_TEST

// IS_LESS_OR_EQUAL
START_TEST(test_is_less_or_equal_both_positive_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 1;
  int result = s21_is_less_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_or_equal_both_negative_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, SIGN_MASK}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, SIGN_MASK}};

  int expected_result = 1;
  int result = s21_is_less_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_or_equal_one_negative_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, SIGN_MASK}};

  int expected_result = 0;
  int result = s21_is_less_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_less_or_equal_one_negative) {
  s21_decimal value_1;
  // -12345678987654321
  value_1.bits[2] = 0x00000000;
  value_1.bits[1] = 0x002BDC54;
  value_1.bits[0] = 0x6291F4B1;
  value_1.bits[3] = SIGN_MASK;

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_less_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

// IS GREATER
START_TEST(test_is_greater_both_positive) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFA, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF, 0}};

  int expected_result = 0;
  int result = s21_is_greater(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_greater(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_greater_one_negative) {
  // -12345678987654321
  s21_decimal value_1 = {{0x6291F4B1, 0x002BDC54, 0x00000000, SIGN_MASK}};

  // 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 0;
  int result = s21_is_greater(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_greater(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_greater_both_negative) {
  s21_decimal value_1 = {{0, 0xFFFFFFFF, 0xFFFF, SIGN_MASK}};
  s21_decimal value_2 = {{0, 0xFFFFFFFA, 0xFFFF, SIGN_MASK}};

  int expected_result = 0;
  int result = s21_is_greater(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_greater(value_2, value_1);
  ASSERT;
}
END_TEST

// IS GREATER OR EQUAL
START_TEST(test_is_greater_or_equal_both_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 1;
  int result = s21_is_greater_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_greater_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_greater_or_equal_both_positive) {
  s21_decimal value_1 = {{0xFFFFFFFB, 0xFFFFFFFF, 0xFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFF, 0}};

  int expected_result = 0;
  int result = s21_is_greater_or_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_greater_or_equal(value_2, value_1);
  ASSERT;
}
END_TEST

// IS EQUAL
START_TEST(test_is_equal_both_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 1;
  int result = s21_is_equal(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_equal_with_non_equal_integers) {
  s21_decimal value_1 = {{0xFAFABAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 0;
  int result = s21_is_equal(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_equal(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_is_equal_with__numbers_with_scales) {
  s21_decimal value_1 = {{999999, 0, 0, 1 << 16}};
  s21_decimal value_2 = {{100000, 0, 0, 0}};

  int expected_result = 0;
  int result = s21_is_equal(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_equal(value_2, value_1);
  ASSERT;
}
END_TEST

// IS NOT EQUAL
START_TEST(test_is_not_equal_both_equal_integers) {
  s21_decimal value_1 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};
  s21_decimal value_2 = {{0xFAFAFAFA, 0xFAFAFAFA, 0xFAFAFAFA, 0}};

  int expected_result = 0;
  int result = s21_is_not_equal(value_1, value_2);
  ASSERT;

  expected_result = 0;
  result = s21_is_not_equal(value_2, value_1);
  ASSERT;
}
END_TEST

// EDGE CASES
START_TEST(test_edge_cases_with_overflow_risk) {
  // максимальное число
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  // макс. число, с 5 знаками после запятой
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 5 << 16}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_edge_cases_with_overflow_risk_2) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFF, 28 << 16}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ASSERT;

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ASSERT;
}
END_TEST

START_TEST(test_edge_cases_with_overflow_risk_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0x19999999, 0}};
  s21_decimal value_2 = {{0, 0, 0, 2 << 16}};

  int result = s21_is_greater(value_1, value_2);
  int expected_result = 1;
  ASSERT;

  result = s21_is_greater(value_2, value_1);
  expected_result = 0;
  ASSERT;
}
END_TEST

START_TEST(test_edge_cases_both_zeros_but_one_of_them_with_a_negative_sign) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, SIGN_MASK}};
  int result = s21_is_greater(value_1, value_2);
  int expected_result = 0;
  ASSERT;

  result = s21_is_greater(value_2, value_1);
  expected_result = 0;
  ASSERT;
}
END_TEST

START_TEST(test_edge_cases_banker_rounding_max_overflow) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 3 << 16}};

  s21_decimal value_2 = {{5, 0, 0, 0}};

  int result = s21_is_greater(value_1, value_2);
  int expected_result = 1;
  ASSERT;

  result = s21_is_greater(value_2, value_1);
  expected_result = 0;
  ASSERT;
}
END_TEST

Suite* comparison_suite(void) {
  Suite* s = suite_create("s21_decimal_comparison_tests");

  // IS LESS:
  TCase* tc_is_less = tcase_create("Is less tests");

  ADD_TEST_IS_LESS(test_is_less_both_positive_no_scales);
  ADD_TEST_IS_LESS(test_is_less_one_negative_no_scales);
  ADD_TEST_IS_LESS(test_is_less_high_bits_equal_but_mid_bits_not);
  ADD_TEST_IS_LESS(test_both_positive_with_scales);
  ADD_TEST_IS_LESS(test_is_less_with_barely_equal_values);
  ADD_TEST_IS_LESS(test_is_less_both_negative);
  ADD_TEST_IS_LESS(test_is_less_both_equal_integers);
  ADD_TEST_IS_LESS(test_is_less_both_positive_and_one_number_has_scale);
  ADD_TEST_IS_LESS(test_is_less_both_equal_but_one_of_them_with_a_scale);
  ADD_TEST_IS_LESS(
      test_is_less_both_equal_but_one_of_them_with_a_scale_and_other_one_is_negative);
  ADD_TEST_IS_LESS(test_is_less_with_two_values_with_scale);

  suite_add_tcase(s, tc_is_less);

  // IS LESS OR EQUAL:
  TCase* tc_is_less_or_equal = tcase_create("Is less or equal tests");

  ADD_TEST_IS_LESS_OR_EQUAL(test_is_less_or_equal_both_positive_equal_integers);
  ADD_TEST_IS_LESS_OR_EQUAL(test_is_less_or_equal_both_negative_equal_integers);
  ADD_TEST_IS_LESS_OR_EQUAL(test_is_less_or_equal_one_negative_equal_integers);
  ADD_TEST_IS_LESS_OR_EQUAL(test_is_less_or_equal_one_negative);

  suite_add_tcase(s, tc_is_less_or_equal);

  // IS GREATER:
  TCase* tc_is_greater = tcase_create("Is greater tests");

  ADD_TEST_IS_GREATER(test_is_greater_both_positive);
  ADD_TEST_IS_GREATER(test_is_greater_one_negative);
  ADD_TEST_IS_GREATER(test_is_greater_both_negative);

  suite_add_tcase(s, tc_is_greater);

  // IS GREATER OR EQUAL:
  TCase* tc_is_greater_or_equal = tcase_create("Is greater or equal tests");

  ADD_TEST_IS_GREATER_OR_EQUAL(test_is_greater_or_equal_both_equal_integers);
  ADD_TEST_IS_GREATER_OR_EQUAL(test_is_greater_or_equal_both_positive);

  suite_add_tcase(s, tc_is_greater_or_equal);

  // IS EQUAL:
  TCase* tc_is_equal = tcase_create("Is equal tests");

  ADD_TEST_IS_EQUAL(test_is_equal_both_equal_integers);
  ADD_TEST_IS_EQUAL(test_is_equal_with_non_equal_integers);
  ADD_TEST_IS_EQUAL(test_is_equal_with__numbers_with_scales);

  suite_add_tcase(s, tc_is_equal);

  // IS NOT EQUAL:
  TCase* tc_is_not_equal = tcase_create("Is not equal tests");

  ADD_TEST_IS_NOT_EQUAL(test_is_not_equal_both_equal_integers);

  suite_add_tcase(s, tc_is_not_equal);

  // EDGE CASES:
  TCase* tc_edge_cases = tcase_create("Edge cases");

  ADD_TEST_EDGE_CASES(test_edge_cases_with_overflow_risk);
  ADD_TEST_EDGE_CASES(test_edge_cases_with_overflow_risk_2);
  ADD_TEST_EDGE_CASES(test_edge_cases_with_overflow_risk_3);
  ADD_TEST_EDGE_CASES(
      test_edge_cases_both_zeros_but_one_of_them_with_a_negative_sign);
  ADD_TEST_EDGE_CASES(test_edge_cases_banker_rounding_max_overflow);

  suite_add_tcase(s, tc_edge_cases);

  return s;
}