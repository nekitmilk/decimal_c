#include "test_runner.h"
#define ADD_IS_LESS_TEST(testname) tcase_add_test(tc_is_less, testname)

START_TEST(test_is_less_both_positive) {
  // val 1 = 12345678987654321, оно же 0x2BDC546291F4B1.
  // в одном знаке 4 бит, знаков 14, значит имеем дело с 56 бит.
  // дополняем нулями до 96 бит, значит добавляем (96 - 56)/ 4 = 10 нулей
  // - 0x00000000002BDC546291F4B1
  s21_decimal value_1;
  value_1.bits[2] = 0x00000000;  // старшие 32 бита
  value_1.bits[1] = 0x002BDC54;  // средние 32 бита
  value_1.bits[0] = 0x6291F4B1;  // младшие 32 бита
  value_1.bits[3] = 0x00000000;  // нет ни показателя степени, ни знака

  // val 2 = 12345678912345678, оно же 0x2BDC545E14D64E
  // 0x00000000002BDC545E14D64E

  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(test_is_less_one_negative) {
  s21_decimal value_1;
  // число -12345678987654321
  value_1.bits[2] = 0x00000000;  // старшие 32 бита
  value_1.bits[1] = 0x002BDC54;  // средние 32 бита
  value_1.bits[0] = 0x6291F4B1;  // младшие 32 бита
  value_1.bits[3] = SIGN_MASK;   // отрицательное число

  // число 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(test_is_less_both_negative) {
  s21_decimal value_1;
  // число -12345678987654321
  value_1.bits[2] = 0x00000000;  // старшие 32 бита
  value_1.bits[1] = 0x002BDC54;  // средние 32 бита
  value_1.bits[0] = 0x6291F4B1;  // младшие 32 бита
  value_1.bits[3] = SIGN_MASK;   // отрицательное число

  // число -12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, SIGN_MASK}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(test_is_less_both_positive_and_one_number_has_scale) {
  // val 1 = 123456789876.54321
  s21_decimal value_1;
  value_1.bits[2] = 0x00000000;  // старшие 32 бита
  value_1.bits[1] = 0x002BDC54;  // средние 32 бита
  value_1.bits[0] = 0x6291F4B1;  // младшие 32 бита
  value_1.bits[3] = 5 << 16;     // 5 знаков после запятой

  // val 2 = 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(test_is_less_both_equal_but_one_of_them_with_a_scale) {
  // val 2 = 123456789123.45678
  s21_decimal value_1 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 9 << 16}};

  // val 2 = 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 0x00000000}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 0;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(
    test_is_less_both_equal_but_one_of_them_with_a_scale_and_other_one_is_negative) {
  // val 2 = 123456789123.45678
  s21_decimal value_1 = {{0x5E14D64E, 0x002BDC54, 0x00000000, 9 << 16}};

  // val 2 = 12345678912345678
  s21_decimal value_2 = {{0x5E14D64E, 0x002BDC54, 0x00000000, SIGN_MASK}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ck_assert_uint_eq(expected_result, result);

  expected_result = 1;
  result = s21_is_less(value_2, value_1);
  ck_assert_uint_eq(expected_result, result);
}
END_TEST

START_TEST(test_is_less_with_two_values_with_scale) {
  //  1234567.89 == 75BCD15, scale = 2
  s21_decimal value_1 = {{0, 0, 0x075BCD15, 2 << 16}};

  // 1234567.9 == BC614F, scale = 1
  s21_decimal value_2 = {{0, 0, 0x00BC614F, 1 << 16}};

  int expected_result = 1;
  int result = s21_is_less(value_1, value_2);
  ck_assert_int_eq(expected_result, result);
}

START_TEST(test_edge_cases_with_overflow_risk) {
  // максимальное число
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  // макс. число, с 5 знаками после запятой
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 5 << 16}};

  int expected_result = 0;
  int result = s21_is_less(value_1, value_2);
  ck_assert_int_eq(expected_result, result);
}
END_TEST

Suite* comparison_suite(void) {
  // создаём набор тестов, в данном случае - тесты для сравнения
  Suite* s = suite_create("s21_decimal_comparison_tests");

  // создаём набор тестовых случаев - здесь тестируем сравнение "меньше"
  // (is_less)
  TCase* tc_is_less = tcase_create("Is less tests");
  TCase* tc_edge_cases = tcase_create("Edge cases");

  // добавляем наши тесты в набор тестовых случаев. в набор is_less (арг.1)
  // добавляем тест (арг. 2)

  // ТЕСТЫ "МЕНЬШЕ"
  ADD_IS_LESS_TEST(test_is_less_both_positive);
  ADD_IS_LESS_TEST(test_is_less_one_negative);
  ADD_IS_LESS_TEST(test_is_less_both_negative);
  ADD_IS_LESS_TEST(test_is_less_both_positive_and_one_number_has_scale);
  ADD_IS_LESS_TEST(test_is_less_both_equal_but_one_of_them_with_a_scale);
  ADD_IS_LESS_TEST(
      test_is_less_both_equal_but_one_of_them_with_a_scale_and_other_one_is_negative);
  ADD_IS_LESS_TEST(test_is_less_with_two_values_with_scale);
  ////////////////////////////////////////////////////////////

  // ТЕСТЫ НА КРАЕВЫЕ СЛУЧАИ
  tcase_add_test(tc_edge_cases, test_edge_cases_with_overflow_risk);

  ////////////////////////////////////////////////////////////

  // в общий набор тестов (s) добавляем набор случаев (tc_is_less)
  suite_add_tcase(s, tc_is_less);
  suite_add_tcase(s, tc_edge_cases);

  return s;
}