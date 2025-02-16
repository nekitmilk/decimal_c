#include "test_runner.h"

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

Suite* comparison_suite(void) {
  // создаём набор тестов, в данном случае - тесты для сравнения
  Suite* s = suite_create("s21_decimal_comparison_tests");

  // создаём набор тестовых случаев - здесь тестируем сравнение "меньше"
  // (is_less)
  TCase* tc_is_less = tcase_create("Is_less tests");

  // добавляем наши тесты в набор тестовых случаев. в набор is_less (арг.1)
  // добавляем тест (арг. 2)
  tcase_add_test(tc_is_less, test_is_less_both_positive);

  // добавляем в общий набор тестов (s) добавляем набор случаев (tc_is_less)
  suite_add_tcase(s, tc_is_less); 

  return s;
}