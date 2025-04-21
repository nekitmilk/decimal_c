#include "test_runner.h"

void check_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void check_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check);
void check_sub(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void check_sub_fail(s21_decimal decimal1, s21_decimal decimal2, int check);
void check_mul(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void check_mul_fail(s21_decimal decimal1, s21_decimal decimal2, int code_check);
void check_div(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check);
void check_div_fail(s21_decimal decimal1, s21_decimal decimal2, int code_check);

void ck_assert_dec_eq(s21_decimal result, s21_decimal expected_result) {
  ck_assert_int_eq(result.bits[0], expected_result.bits[0]);
  ck_assert_int_eq(result.bits[1], expected_result.bits[1]);
  ck_assert_int_eq(result.bits[2], expected_result.bits[2]);
  ck_assert_int_eq(result.bits[3], expected_result.bits[3]);
}

START_TEST(add_normal_1) {
  // Нормальная ситуация 123 + 17 = 140
  s21_decimal value_1 = {{0x7b, 0, 0, 0}};  // 123
  s21_decimal value_2 = {{0x11, 0, 0, 0}};  // 17
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x8C, 0, 0, 0}};  // 140
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_normal_1_1) {
  // Нормальная ситуация 123 + 17 = 140
  s21_decimal value_1 = {{0x7b, 0, 0, 0}};  // 123
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{0x11, 0, 0, 0}};  // 17
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x8C, 0, 0, 0}};  // 140
  set_sign(&expected_1, 1);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_normal_2) {
  // Нормальная ситуация 123 000.. + 17 000.. = 140 000..
  s21_decimal value_1 = {{0, 0x7b, 0, 0}};
  s21_decimal value_2 = {{0, 0x11, 0, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0, 0x8C, 0, 0}};
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_normal_3) {
  // Нормальная ситуация 123 000.. + 17 000.. = 140 000..
  s21_decimal value_1 = {{0, 0, 0x7b, 0}};
  s21_decimal value_2 = {{0, 0, 0x11, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0, 0, 0x8C, 0}};
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_normal_4) {
  s21_decimal value_1 = {{0x49A635DF, 0, 0, 0}};
  set_scale(&value_1, 7);
  s21_decimal value_2 = {{0x2D8F5E21, 0, 0, 0}};
  set_scale(&value_2, 7);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x77359400, 0, 0, 0}};
  set_scale(&expected_1, 7);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_normal_5) {
  // 37 BFA10CB7 C85DC2DA
  s21_decimal value_1 = {{0x9F84C2C7, 0x1589C8F, 0, 0}};  // 96999532659,000007
  set_scale(&value_1, 6);
  s21_decimal value_2 = {
      {0x9665456A, 0x2A3D6270, 0x3, 0}};  // 5838392938,0000056682
  set_scale(&value_2, 10);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  // 37 BFA10CB7 C85DC418
  s21_decimal expected_1 = {
      {0xC85DC2DA, 0xBFA10CB7, 0x37, 0}};  // 102 837 925 597,0000127000
  set_scale(&expected_1, 10);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_transfering_1) {
  // Нормальная ситуация 123 000.. + 17 000.. = 140 000..
  s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}};  // 4 294 967 295
  s21_decimal value_2 = {{0xD0FD5, 0, 0, 0}};     // 856 021
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x000D0FD4, 1, 0, 0}};  // 4 295 823 316
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_transfering_2) {
  // Нормальная ситуация 123 000.. + 17 000.. = 140 000..
  s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}};  // 4 294 967 295
  s21_decimal value_2 = {{0, 0xD0FD5, 0, 0}};     // 856 021
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0, 0x000D0FD4, 1, 0}};  // 4 295 823 316
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_transfering_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal value_2 = {{0x1, 0, 0, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0, 0x1, 0, 0}};
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_1) {
  // Нормальная ситуация 123 + (-23) = 100
  s21_decimal value_1 = {{0x7b, 0, 0, 0}};  // 123
  s21_decimal value_2 = {{0x17, 0, 0, 0}};  // -23
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x64, 0, 0, 0}};  // 140
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_2) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0, 0, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0, 0xFFFFFFFF, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x0, 0xFFFFFFFF, 0, 0}};
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_4) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0x0, 0xFFFFFFFF, 0, 0}};
  set_sign(&expected_1, 1);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_5) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0x6, 0, 0, 0}};
  set_sign(&value_2, 1);
  set_scale(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // set_sign(&expected_1, 1);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_6) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0x4, 0, 0, 0}};
  set_sign(&value_2, 1);
  set_scale(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // set_sign(&expected_1, 1);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_diff_sign_7) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0x5, 0, 0, 0}};
  set_sign(&value_2, 1);
  set_scale(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  s21_decimal expected_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // set_sign(&expected_1, 1);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overflow_1) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0x1, 0, 0, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 1;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overflow_2) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0, 0xFABFF, 0, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 1;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overflow_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0, 0, 0xFABFF, 0}};
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 1;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

// Большое число - ооочень маленькое, что должно быть?
// Все, что не влезает в мантису должно быть округлено
START_TEST(add_overflow_4) {
  s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}};  // 18446744069414584320
  s21_decimal value_2 = {{0x1, 0, 0, 0}};         // 1 * 10^-27
  set_scale(&value_2, 27);
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};  // 18446744069414584319,999...

  // 3b9ac9ff c4653600 00000000
  s21_decimal expected_1 = {
      {0, 0xc4653600, 0x3b9ac9ff, 0}};  // 18446744069414584320,00...
  set_scale(&expected_1, 9);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overflow_5) {
  s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}};  // 18446744069414584320
  s21_decimal value_2 = {{0x1, 0, 0, 0}};         // 1 * 10^-27
  set_scale(&value_2, 27);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};  // 18446744069414584320 + 1 * 10^-27

  s21_decimal expected_1 = {
      {0, 0xc4653600, 0x3b9ac9ff, 0}};  // 18446744069414584320,00...
  set_scale(&expected_1, 9);
  int expected_err = 0;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_dec_eq(result_1, expected_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_dec_eq(result_2, expected_1);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overmin_1) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{0x1, 0, 0, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 2;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overmin_2) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{0xFFFFF, 0, 0, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 2;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overmin_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{0, 0xFABFFF, 0, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 2;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(add_overmin_4) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  set_sign(&value_1, 1);
  s21_decimal value_2 = {{0, 0xFABFFF, 0xFABFFF, 0}};
  set_sign(&value_2, 1);
  s21_decimal result_1 = {{0}};
  s21_decimal result_2 = {{0}};

  int expected_err = 2;

  int err_1 = s21_add(value_1, value_2, &result_1);
  ck_assert_int_eq(err_1, expected_err);

  int err_2 = s21_add(value_2, value_1, &result_2);
  ck_assert_int_eq(err_2, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_1) {
  // Максимальное значение мантиссы: 79,228,162,514,264,337,593,543,950,335
  s21_decimal value_1 = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Максимальное значение
  s21_decimal value_2 = {{0x1, 0, 0, 0x00010000}};  // 0.1 (scale = 1)
  s21_decimal result = {{0}};

  // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,335 (округление
  // вниз)
  s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int expected_err = 0;

  int err = s21_add(value_1, value_2, &result);
  ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_2) {
  // Максимальное значение мантиссы: 79,228,162,514,264,337,593,543,950,335
  s21_decimal value_1 = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Максимальное значение
  s21_decimal value_2 = {{0x6, 0, 0, 0x80010000}};  // -0.6 (scale = 1)
  s21_decimal result = {{0}};

  // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,334 (округление
  // вниз)
  s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int expected_err = 0;

  int err = s21_add(value_1, value_2, &result);
  ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_3) {
  // Максимальное значение мантиссы: 79,228,162,514,264,337,593,543,950,334
  s21_decimal value_1 = {
      {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Максимальное значение - 1
  s21_decimal value_2 = {{0x5, 0, 0, 0x00010000}};  // 0.5 (scale = 1)
  s21_decimal result = {{0}};

  // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,334 (округление
  // вниз, так как мантисса четная)
  s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  int expected_err = 0;

  int err = s21_add(value_1, value_2, &result);
  ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_4) {
  // Минимальное значение мантиссы: -79,228,162,514,264,337,593,543,950,335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                          0x80000000}};  // Минимальное значение
  s21_decimal value_2 = {{0x6, 0, 0, 0x80010000}};  // -0.6 (scale = 1)
  s21_decimal result = {{0}};

  // Ожидаемый результат: -79,228,162,514,264,337,593,543,950,334 (округление
  // вниз) s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF,
  // 0x80000000}};
  int expected_err = 2;

  int err = s21_add(value_1, value_2, &result);
  // ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_6) {
  // Обычное число: 123.45
  s21_decimal value_1 = {{12345, 0, 0, 0x00020000}};  // 123.45 (scale = 2)
  s21_decimal value_2 = {
      {6, 0, 0, 0x00180000}};  // 0.0000000000000000000000000006 (scale = 24)
  s21_decimal result = {{0}};
  s21_decimal expected = {{0xb0400006, 0xe8dfebfd, 0x00661d8d, 0x00180000}};
  int expected_err = 0;

  int err = s21_add(value_1, value_2, &result);
  ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_7) {
  // Отрицательное обычное число: -123.45
  s21_decimal value_1 = {{12345, 0, 0, 0x80020000}};  // -123.45 (scale = 2)
  s21_decimal value_2 = {
      {6, 0, 0, 0x00180000}};  // 0.0000000000000000000000000006 (scale = 24)
  s21_decimal result = {{0}};

  s21_decimal expected = {{0xb03ffffa, 0xe8dfebfd, 0x00661d8d,
                           0x80180000}};  // -123.4499999999999999999999999994
  int expected_err = 0;

  int err = s21_add(value_1, value_2, &result);
  ck_assert_dec_eq(result, expected);
  ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(test_add1) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 79228162514264337593543950334
  s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  s21_decimal result = {{0}};
  int err = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(err, check);
}

START_TEST(test_add2) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x801C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add3) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.0000000000000000000000000010
  s21_decimal decimal2 = {{0xA, 0x0, 0x0, 0x801C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add4) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -56124981125209321576924.010631
  s21_decimal decimal2 = {{0xF555887, 0x553F1641, 0xB5597F0B, 0x80060000}};
  // 79228106389283212384222373411
  s21_decimal check = {{0x3AC4E623, 0x756BD584, 0xFFFFF41D, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add5) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -0.5
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80010000}};
  // overflow
  int check = TEST_ARITHMETIC_SMALL;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add6) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 0.5
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x10000}};
  // -79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add7) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -1.9999999999999999999999999000
  s21_decimal decimal2 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}};
  // overflow
  int check = TEST_ARITHMETIC_SMALL;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add8) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -26409387.704754779197847983445
  s21_decimal decimal2 = {{0x1B755555, 0x2CE411B0, 0x55555560, 0x80150000}};
  // overflow
  int check = TEST_ARITHMETIC_SMALL;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add9) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -0.0000000000000000000000000000
  s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x801C0000}};
  // -79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add10) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.4999999999999999999999999999
  s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add11) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.4999999999999999999999999999
  s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // 79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add12) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 1.9999999999999999999999999000
  s21_decimal decimal2 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add13) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -1.9999999999999999999999999000
  s21_decimal decimal2 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}};
  // 79228162514264337593543950332
  s21_decimal check = {{0xFFFFFFFC, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_add(decimal1, decimal2, check);
}

START_TEST(test_add14) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 26409387.704754779197847983445
  s21_decimal decimal2 = {{0x1B755555, 0x2CE411B0, 0x55555560, 0x150000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_add15) {
  // 79228162514264337593543950334
  s21_decimal decimal1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7.9228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_add_fail(decimal1, decimal2, check);
}

START_TEST(test_sub1) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0
  s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub2) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub3) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 79228162514264337593543950334
  s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 1
  s21_decimal check = {{0x1, 0x0, 0x0, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub4) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7922816251426433759354395033.5
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // 71305346262837903834189555302
  s21_decimal check = {{0x66666666, 0x66666666, 0xE6666666, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub5) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7922816251426433759354395035
  s21_decimal decimal2 = {{0x9999999B, 0x99999999, 0x19999999, 0x0}};
  // 71305346262837903834189555300
  s21_decimal check = {{0x66666664, 0x66666666, 0xE6666666, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub6) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.5
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x10000}};
  // 79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub7) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.4999999999999999999999999999
  s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub8) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.4999999999999999999999999999
  s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub9) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.5000000000000000000000000001
  s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub10) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.5000000000000000000000000001
  s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub11) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 1.9999999999999999999999999000
  s21_decimal decimal2 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}};
  // 79228162514264337593543950333
  s21_decimal check = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub12) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -1.9999999999999999999999999000
  s21_decimal decimal2 = {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub13) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 1.0000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // 79228162514264337593543950334
  s21_decimal check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub14) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -1.0000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801C0000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub15) {
  // -0.4999999999999999999999999999
  s21_decimal decimal1 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // 3.6336660283201536
  s21_decimal decimal2 = {{0x811800, 0x811800, 0x0, 0x100000}};
  // -4.1336660283201535999999999999
  s21_decimal check = {{0x117FFFFF, 0x1DFB7C4D, 0x8590E24C, 0x801C0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub16) {
  // -0.4999999999999999999999999999
  s21_decimal decimal1 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // -3.6336660283201536
  s21_decimal decimal2 = {{0x811800, 0x811800, 0x0, 0x80100000}};
  // 3.1336660283201536000000000001
  s21_decimal check = {{0x1800001, 0xDFD679EC, 0x654113ED, 0x1C0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub17) {
  // 0.5000000000000000000000000001
  s21_decimal decimal1 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // -79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // overflow
  int check = TEST_ARITHMETIC_BIG;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub18) {
  // 0.5000000000000000000000000001
  s21_decimal decimal1 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 79228162514264337593543950334
  s21_decimal decimal2 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -79228162514264337593543950333
  s21_decimal check = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub19) {
  // 0.5000000000000000000000000001
  s21_decimal decimal1 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 7922816251426433759354395033
  s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};
  // -7922816251426433759354395032.5
  s21_decimal check = {{0xFFFFFFF5, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub20) {
  // 0.5000000000000000000000000001
  s21_decimal decimal1 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // -7922816251426433759354395033
  s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x80000000}};
  // 7922816251426433759354395033.5
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub21) {
  // 0.5000000000000000000000000001
  s21_decimal decimal1 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // 7922816251426433759354395033.5
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // -7922816251426433759354395033.0
  s21_decimal check = {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub22) {
  // 26409387
  s21_decimal decimal1 = {{0x192F9AB, 0x0, 0x0, 0x0}};
  // 26409387498605864508043122005
  s21_decimal decimal2 = {{0x55555555, 0x0, 0x55555555, 0x0}};
  // -26409387498605864508016712618
  s21_decimal check = {{0x53C25BAA, 0x0, 0x55555555, 0x80000000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub23) {
  // 26409387
  s21_decimal decimal1 = {{0x192F9AB, 0x0, 0x0, 0x0}};
  // -26409387498605864508043122005
  s21_decimal decimal2 = {{0x55555555, 0x0, 0x55555555, 0x80000000}};
  // 26409387498605864508069531392
  s21_decimal check = {{0x56E84F00, 0x0, 0x55555555, 0x0}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub24) {
  // -0.0000000000000000000000000010
  s21_decimal decimal1 = {{0xA, 0x0, 0x0, 0x801C0000}};
  // -0.4999999999999999999999999999
  s21_decimal decimal2 = {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // 0.4999999999999999999999999989
  s21_decimal check = {{0x87FFFFF5, 0x1F128130, 0x1027E72F, 0x1C0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub25) {
  // -0.0000000000000000000000000010
  s21_decimal decimal1 = {{0xA, 0x0, 0x0, 0x801C0000}};
  // 0.5000000000000000000000000001
  s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}};
  // -0.5000000000000000000000000011
  s21_decimal check = {{0x8800000B, 0x1F128130, 0x1027E72F, 0x801C0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub26) {
  // -0.0000000000000000000000000010
  s21_decimal decimal1 = {{0xA, 0x0, 0x0, 0x801C0000}};
  // -0.5000000000000000000000000001
  s21_decimal decimal2 = {{0x88000001, 0x1F128130, 0x1027E72F, 0x801C0000}};
  // 0.4999999999999999999999999991
  s21_decimal check = {{0x87FFFFF7, 0x1F128130, 0x1027E72F, 0x1C0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub27) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 7922816251426433759354395033
  s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x0}};
  // overflow
  int check = TEST_ARITHMETIC_SMALL;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub28) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -7922816251426433759354395033
  s21_decimal decimal2 = {{0x99999999, 0x99999999, 0x19999999, 0x80000000}};
  // -71305346262837903834189555302
  s21_decimal check = {{0x66666666, 0x66666666, 0xE6666666, 0x80000000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub29) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 7922816251426433759354395033.5
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // overflow
  int check = TEST_ARITHMETIC_SMALL;

  check_sub_fail(decimal1, decimal2, check);
}

START_TEST(test_sub30) {
  // 646.33673839575124685661598885
  s21_decimal decimal1 = {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x1A0000}};
  // -0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x801C0000}};
  // 646.33673839575124685661598885
  s21_decimal check = {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x1A0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_sub31) {
  // 646.33673839575124685661598885
  s21_decimal decimal1 = {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x1A0000}};
  // 0.0000000000000000000000000010
  s21_decimal decimal2 = {{0xA, 0x0, 0x0, 0x1C0000}};
  // 646.33673839575124685661598885
  s21_decimal check = {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x1A0000}};

  check_sub(decimal1, decimal2, check);
}

START_TEST(test_mul1) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 7.8228162514264337593543950335
  s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0x1C0000}};
  // 61.978735728724164262422454727
  s21_decimal check = {{0xE07921C7, 0x3FD7ABE1, 0xC8439BCC, 0x1B0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul2) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // -0.8228162514264337593543950335
  s21_decimal decimal2 = {{0x4F63FFFF, 0xBB0D25CF, 0x1A962D2F, 0x801C0000}};
  // -6.5190219687391279469416894926
  s21_decimal check = {{0x23B23CAE, 0xEC60363A, 0xD2A415FA, 0x801C0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul3) {
  // -7.000000000000025
  s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
  // -0.00000000000005
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800E0000}};
  // 0.0000000000003500000000000012
  s21_decimal check = {{0x40B6C00C, 0xC6F3B, 0x0, 0x1C0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul4) {
  // -7922816251426.4337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80100000}};
  // 78228162514264.337593543950335
  s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0xF0000}};
  // -619787357287241642624224547.27
  s21_decimal check = {{0xE07921C7, 0x3FD7ABE1, 0xC8439BCC, 0x80020000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul5) {
  // -79228162514264.337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800F0000}};
  // 782281625142643.37593543950335
  s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0xE0000}};
  // -61978735728724164262422454727
  s21_decimal check = {{0xE07921C7, 0x3FD7ABE1, 0xC8439BCC, 0x80000000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul6) {
  // 79228162514264.1
  s21_decimal decimal1 = {{0x70D42571, 0x2D093, 0x0, 0x10000}};
  // 78228162514264.5
  s21_decimal decimal2 = {{0x22618575, 0x2C77B, 0x0, 0x10000}};
  // 6197873572872410706900970254.4
  s21_decimal check = {{0x7F6C2E90, 0x3FD777DD, 0xC8439BCC, 0x10000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul7) {
  // 79228162514265.1
  s21_decimal decimal1 = {{0x70D4257B, 0x2D093, 0x0, 0x10000}};
  // 78228162514264.5
  s21_decimal decimal2 = {{0x22618575, 0x2C77B, 0x0, 0x10000}};
  // 6197873572872488935063484519.0
  s21_decimal check = {{0xA1CDB406, 0x3FDA3F58, 0xC8439BCC, 0x10000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul8) {
  // 5.0
  s21_decimal decimal1 = {{0x32, 0x0, 0x0, 0x10000}};
  // 4.5
  s21_decimal decimal2 = {{0x2D, 0x0, 0x0, 0x10000}};
  // 22.50
  s21_decimal check = {{0x8CA, 0x0, 0x0, 0x20000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul9) {
  // 79228162.5
  s21_decimal decimal1 = {{0x2F394219, 0x0, 0x0, 0x10000}};
  // 78228162.5555555555555
  s21_decimal decimal2 = {{0xB20798E3, 0x6856A1BC, 0x2A, 0xD0000}};
  // 6197873575027970.8333289317688
  s21_decimal check = {{0x34C52538, 0x6AFC5902, 0xC8439BCD, 0xD0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul10) {
  // -7.000000000000025
  s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
  // -2.00000000000005
  s21_decimal decimal2 = {{0x20F48005, 0xB5E6, 0x0, 0x800E0000}};
  // 14.000000000000400000000000001
  s21_decimal check = {{0xF1E90001, 0xBD686F20, 0x2D3C8750, 0x1B0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul11) {
  // -7.000000000000025
  s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
  // -0.0000000000005
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800D0000}};
  // 0.0000000000035000000000000125
  s21_decimal check = {{0x8723807D, 0x7C5850, 0x0, 0x1C0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul12) {
  // -7.000000000000025
  s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
  // -0.000000000000005
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800F0000}};
  // 0.0000000000000350000000000001
  s21_decimal check = {{0xB9ABE001, 0x13E52, 0x0, 0x1C0000}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul13) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 0.00000
  s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x50000}};
  // 0
  s21_decimal check = {{0x0, 0x0, 0x0, 0x0}};

  check_mul(decimal1, decimal2, check);
}

START_TEST(test_mul14) {
  // 7922816251426433759354395032.8
  s21_decimal decimal1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // 10.000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1B0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_BIG;

  check_mul_fail(decimal1, decimal2, code_check);
}

START_TEST(test_mul15) {
  // 7922816251426433759354395032.8
  s21_decimal decimal1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};
  // -10.000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801B0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_SMALL;

  check_mul_fail(decimal1, decimal2, code_check);
}

START_TEST(test_mul16) {
  // -7922816251426433759354395032.7
  s21_decimal decimal1 = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
  // 10.000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1B0000}};
  // -79228162514264337593543950335
  s21_decimal decimal_check = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul17) {
  // -7922816251426433759354395032.7
  s21_decimal decimal1 = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};
  // -10.000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801B0000}};
  // 79228162514264337593543950335
  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_mul(decimal1, decimal2, decimal_check);
}

START_TEST(test_mul18) {
  // -0.0000000000000025
  s21_decimal decimal1 = {{0x19, 0x0, 0x0, 0x80100000}};
  // -0.0000000000000005
  s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x80100000}};
  // too small
  // 0.0000000000000000000000000000
  int code_check = TEST_ARITHMETIC_SMALL;

  check_mul_fail(decimal1, decimal2, code_check);
}

START_TEST(test_mul19) {
  // 792281625142643.37593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE0000}};
  // -782281625142643.37593543950335
  s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0x800E0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_SMALL;

  // check_mul(decimal1, decimal2, decimal_check);
  check_mul_fail(decimal1, decimal2, code_check);
}

START_TEST(test_mul20) {
  // 792281625142643.37593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE0000}};
  // 782281625142643.37593543950335
  s21_decimal decimal2 = {{0x17FFFFFF, 0x602F7FC3, 0xFCC4D1C3, 0xE0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_BIG;

  check_mul_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div1) {
  // 1.2640938749860586450804312205
  s21_decimal decimal1 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 2
  s21_decimal decimal2 = {{0x2, 0x0, 0x0, 0x0}};
  // 0.6320469374930293225402156102
  s21_decimal check = {{0x8C444446, 0x5F128130, 0x146C2B73, 0x1C0000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div2) {
  // 2
  s21_decimal decimal1 = {{0x2, 0x0, 0x0, 0x0}};
  // 1.2640938749860586450804312205
  s21_decimal decimal2 = {{0x1888888D, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 1.5821609767882606564463392905
  s21_decimal check = {{0x8B80B889, 0x20B8279E, 0x331F5430, 0x1C0000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div3) {
  // 1431655765
  s21_decimal decimal1 = {{0x55555555, 0x0, 0x0, 0x0}};
  // 12.640938749860586450805
  s21_decimal decimal2 = {{0x2EB3EF75, 0x4444026F, 0x2AD, 0x150000}};
  // 113255494.17884722765619814067
  s21_decimal check = {{0x6D7C5EB3, 0x2C521AF0, 0x24984660, 0x140000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div4) {
  // 1
  s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x0}};
  // 500
  s21_decimal decimal2 = {{0x1F4, 0x0, 0x0, 0x0}};
  // 0.002
  s21_decimal check = {{0x2, 0x0, 0x0, 0x30000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div5) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 7.9228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 10000000000000000000000000000
  s21_decimal check = {{0x10000000, 0x3E250261, 0x204FCE5E, 0x0}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div6) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.0000000000000000000000000001
  s21_decimal check = {{0x1, 0x0, 0x0, 0x1C0000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div7) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -26409387504754779197847983445
  s21_decimal decimal2 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // -3
  s21_decimal check = {{0x3, 0x0, 0x0, 0x80000000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div8) {
  // -26409387504754779197847983445
  s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // 156064767525876035022225408
  s21_decimal decimal2 = {{0x0, 0x0, 0x811800, 0x0}};
  // -169.22068909868474138626644073
  s21_decimal check = {{0x8C085869, 0xC05E68BD, 0x36AD9B79, 0x801A0000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div9) {
  // -26409387504754779197847983445
  s21_decimal decimal1 = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};
  // -429496729.5
  s21_decimal decimal2 = {{0xFFFFFFFF, 0x0, 0x0, 0x80010000}};
  // 61489146926681729710
  s21_decimal check = {{0xAAAAAAAE, 0x55555558, 0x3, 0x0}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div10) {
  // -79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 1.0000000000000000000000000001
  s21_decimal decimal2 = {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // -79228162514264337593543950327
  s21_decimal check = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div11) {
  // 1.2640938749860586450804312207
  s21_decimal decimal1 = {{0x1888888F, 0xBE250261, 0x28D856E6, 0x1C0000}};
  // 2.0000000000000000000000000000
  s21_decimal decimal2 = {{0x20000000, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}};
  // 0.6320469374930293225402156104
  s21_decimal check = {{0x8C444448, 0x5F128130, 0x146C2B73, 0x1C0000}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div12) {
  // 3.6336660283201536
  s21_decimal decimal1 = {{0x811800, 0x811800, 0x0, 0x100000}};
  // 0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // 36336660283201536000000000000
  s21_decimal check = {{0x89800000, 0xFEE8FB1C, 0x7568FB1C, 0x0}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div13) {
  // 7.9228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  // 0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_div(decimal1, decimal2, check);
}

START_TEST(test_div14) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0
  s21_decimal decimal2 = {{0x0, 0x0, 0x0, 0x80000000}};
  // division by zero
  int code_check = TEST_ARITHMETIC_ZERO_DIV;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div15) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_BIG;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div16) {
  // 79228162514264337593543950335
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.0000000000000000000000000001
  s21_decimal decimal2 = {{0x1, 0x0, 0x0, 0x801C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_SMALL;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div17) {
  // 0.0000000000000000000000000001
  s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x1C0000}};
  // -79228162514264337593543950335
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // -0
  int code_check = TEST_ARITHMETIC_SMALL;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div18) {
  // 0.0000000074505801528346182749
  s21_decimal decimal1 = {{0xCAD4845D, 0x9F9C7BD, 0x4, 0x1C0000}};
  // 1152921504606846976
  s21_decimal decimal2 = {{0x0, 0x10000000, 0x0, 0x0}};
  // 0.0000000000000000000000000065
  s21_decimal decimal_check = {{0x41, 0x0, 0x0, 0x1C0000}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div19) {
  // 0.0000000000000000000542101054
  s21_decimal decimal1 = {{0x204FCE3E, 0x0, 0x0, 0x1C0000}};
  // 8388608
  s21_decimal decimal2 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000000000065
  s21_decimal decimal_check = {{0x41, 0x0, 0x0, 0x1C0000}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div20) {
  // 8388608
  s21_decimal decimal1 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000542101054
  s21_decimal decimal2 = {{0x204FCE3E, 0x0, 0x0, 0x1C0000}};
  // 154742514114351823414827745.38
  s21_decimal decimal_check = {{0x6306800A, 0xE4ABBB05, 0x32000031, 0x20000}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div21) {
  // 0.0000000000000000000020971522
  s21_decimal decimal1 = {{0x1400002, 0x0, 0x0, 0x1C0000}};
  // 8388608
  s21_decimal decimal2 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000000000003
  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x1C0000}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div22) {
  // 8388608
  s21_decimal decimal1 = {{0x800000, 0x0, 0x0, 0x0}};
  // 0.0000000000000000000020971522
  s21_decimal decimal2 = {{0x1400002, 0x0, 0x0, 0x1C0000}};
  // 3999999618530309817284601470.5
  s21_decimal decimal_check = {{0x695280F1, 0x2D062C6F, 0x813F38AA, 0x10000}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div23) {
  // 1
  s21_decimal decimal1 = {{0x1, 0x0, 0x0, 0x0}};
  // 39614081257132168796771975168
  s21_decimal decimal2 = {{0x0, 0x0, 0x80000000, 0x0}};
  // 0
  int code_check = TEST_ARITHMETIC_SMALL;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div24) {
  // 79228162514264337593543950320
  s21_decimal decimal1 = {{0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_BIG;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div25) {
  // -79228162514264337593543950320
  s21_decimal decimal1 = {{0xFFFFFFF0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // overflow
  int code_check = TEST_ARITHMETIC_SMALL;

  check_div_fail(decimal1, decimal2, code_check);
}

START_TEST(test_div26) {
  // 79228162514264337593543950319
  s21_decimal decimal1 = {{0xFFFFFFEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // 0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  // 79228162514264337593543950335
  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  check_div(decimal1, decimal2, decimal_check);
}

START_TEST(test_div27) {
  // 79228162514264337593543950319
  s21_decimal decimal1 = {{0xFFFFFFEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  // -0.9999999999999999999999999998
  s21_decimal decimal2 = {{0xFFFFFFE, 0x3E250261, 0x204FCE5E, 0x801C0000}};
  // -79228162514264337593543950335
  s21_decimal decimal_check = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  check_div(decimal1, decimal2, decimal_check);
}

Suite *arithmetic_suite() {
  Suite *s;
  TCase *tc_add_normal, *tc_add_transfering, *tc_add_diff_sign,
      *tc_add_overflow, *tc_add_overmin, *tc_mantis_boundary_rounding,
      *tc_add_core, *tc_sub_core, *tc_mul_core, *tc_div_core;

  s = suite_create("arithmetic_tests");

  tc_add_normal = tcase_create("test_add_normal");
  tcase_add_test(tc_add_normal, add_normal_1);
  tcase_add_test(tc_add_normal, add_normal_1_1);
  tcase_add_test(tc_add_normal, add_normal_2);
  tcase_add_test(tc_add_normal, add_normal_3);
  tcase_add_test(tc_add_normal, add_normal_4);
  tcase_add_test(tc_add_normal, add_normal_5);
  suite_add_tcase(s, tc_add_normal);

  tc_add_transfering = tcase_create("test_add_transfering");
  tcase_add_test(tc_add_transfering, add_transfering_1);
  tcase_add_test(tc_add_transfering, add_transfering_2);
  tcase_add_test(tc_add_transfering, add_transfering_3);
  suite_add_tcase(s, tc_add_transfering);

  tc_add_diff_sign = tcase_create("test_add_diff_sign");
  tcase_add_test(tc_add_diff_sign, add_diff_sign_1);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_2);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_3);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_4);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_5);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_6);
  tcase_add_test(tc_add_diff_sign, add_diff_sign_7);
  suite_add_tcase(s, tc_add_diff_sign);

  tc_add_overflow = tcase_create("test_add_overflow");
  tcase_add_test(tc_add_overflow, add_overflow_1);
  tcase_add_test(tc_add_overflow, add_overflow_2);
  tcase_add_test(tc_add_overflow, add_overflow_3);
  tcase_add_test(tc_add_overflow, add_overflow_4);
  tcase_add_test(tc_add_overflow, add_overflow_5);
  suite_add_tcase(s, tc_add_overflow);

  tc_add_overmin = tcase_create("test_add_overmin");
  tcase_add_test(tc_add_overmin, add_overmin_1);
  tcase_add_test(tc_add_overmin, add_overmin_2);
  tcase_add_test(tc_add_overmin, add_overmin_3);
  tcase_add_test(tc_add_overmin, add_overmin_4);
  suite_add_tcase(s, tc_add_overmin);

  tc_mantis_boundary_rounding =
      tcase_create("test_add_mantis_boundary_rounding");
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_1);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_2);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_3);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_4);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_6);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_7);
  suite_add_tcase(s, tc_mantis_boundary_rounding);

  tc_add_core = tcase_create("test_add_core");
  tcase_add_test(tc_add_core, test_add1);
  tcase_add_test(tc_add_core, test_add2);
  tcase_add_test(tc_add_core, test_add3);
  tcase_add_test(tc_add_core, test_add4);
  tcase_add_test(tc_add_core, test_add5);
  tcase_add_test(tc_add_core, test_add6);
  tcase_add_test(tc_add_core, test_add7);
  tcase_add_test(tc_add_core, test_add8);
  tcase_add_test(tc_add_core, test_add9);
  tcase_add_test(tc_add_core, test_add10);
  tcase_add_test(tc_add_core, test_add11);
  tcase_add_test(tc_add_core, test_add12);
  tcase_add_test(tc_add_core, test_add13);
  tcase_add_test(tc_add_core, test_add14);
  tcase_add_test(tc_add_core, test_add15);
  suite_add_tcase(s, tc_add_core);

  tc_sub_core = tcase_create("test_sub_core");
  tcase_add_test(tc_sub_core, test_sub1);
  tcase_add_test(tc_sub_core, test_sub2);
  tcase_add_test(tc_sub_core, test_sub3);
  tcase_add_test(tc_sub_core, test_sub4);
  tcase_add_test(tc_sub_core, test_sub5);
  tcase_add_test(tc_sub_core, test_sub6);
  tcase_add_test(tc_sub_core, test_sub7);
  tcase_add_test(tc_sub_core, test_sub8);
  tcase_add_test(tc_sub_core, test_sub9);
  tcase_add_test(tc_sub_core, test_sub10);
  tcase_add_test(tc_sub_core, test_sub11);
  tcase_add_test(tc_sub_core, test_sub12);
  tcase_add_test(tc_sub_core, test_sub13);
  tcase_add_test(tc_sub_core, test_sub14);
  tcase_add_test(tc_sub_core, test_sub15);
  tcase_add_test(tc_sub_core, test_sub16);
  tcase_add_test(tc_sub_core, test_sub17);
  tcase_add_test(tc_sub_core, test_sub18);
  tcase_add_test(tc_sub_core, test_sub19);
  tcase_add_test(tc_sub_core, test_sub20);
  tcase_add_test(tc_sub_core, test_sub21);
  tcase_add_test(tc_sub_core, test_sub22);
  tcase_add_test(tc_sub_core, test_sub23);
  tcase_add_test(tc_sub_core, test_sub24);
  tcase_add_test(tc_sub_core, test_sub25);
  tcase_add_test(tc_sub_core, test_sub26);
  tcase_add_test(tc_sub_core, test_sub27);
  tcase_add_test(tc_sub_core, test_sub28);
  tcase_add_test(tc_sub_core, test_sub29);
  tcase_add_test(tc_sub_core, test_sub30);
  tcase_add_test(tc_sub_core, test_sub31);
  suite_add_tcase(s, tc_sub_core);

  tc_mul_core = tcase_create("test_mul_core");
  tcase_add_test(tc_mul_core, test_mul1);
  tcase_add_test(tc_mul_core, test_mul2);
  tcase_add_test(tc_mul_core, test_mul3);
  tcase_add_test(tc_mul_core, test_mul4);
  tcase_add_test(tc_mul_core, test_mul5);
  tcase_add_test(tc_mul_core, test_mul6);
  tcase_add_test(tc_mul_core, test_mul7);
  tcase_add_test(tc_mul_core, test_mul8);
  tcase_add_test(tc_mul_core, test_mul9);
  tcase_add_test(tc_mul_core, test_mul10);
  tcase_add_test(tc_mul_core, test_mul11);
  tcase_add_test(tc_mul_core, test_mul12);
  tcase_add_test(tc_mul_core, test_mul13);
  tcase_add_test(tc_mul_core, test_mul14);
  tcase_add_test(tc_mul_core, test_mul15);
  tcase_add_test(tc_mul_core, test_mul16);
  tcase_add_test(tc_mul_core, test_mul17);
  tcase_add_test(tc_mul_core, test_mul18);
  tcase_add_test(tc_mul_core, test_mul19);
  tcase_add_test(tc_mul_core, test_mul20);
  suite_add_tcase(s, tc_mul_core);

  tc_div_core = tcase_create("test_div_core");
  tcase_add_test(tc_div_core, test_div1);
  tcase_add_test(tc_div_core, test_div2);
  tcase_add_test(tc_div_core, test_div3);
  tcase_add_test(tc_div_core, test_div4);
  tcase_add_test(tc_div_core, test_div5);
  tcase_add_test(tc_div_core, test_div6);
  tcase_add_test(tc_div_core, test_div7);
  tcase_add_test(tc_div_core, test_div8);
  tcase_add_test(tc_div_core, test_div9);
  tcase_add_test(tc_div_core, test_div10);
  tcase_add_test(tc_div_core, test_div11);
  tcase_add_test(tc_div_core, test_div12);
  tcase_add_test(tc_div_core, test_div13);
  tcase_add_test(tc_div_core, test_div14);
  tcase_add_test(tc_div_core, test_div15);
  tcase_add_test(tc_div_core, test_div16);
  tcase_add_test(tc_div_core, test_div17);
  tcase_add_test(tc_div_core, test_div18);
  tcase_add_test(tc_div_core, test_div19);
  tcase_add_test(tc_div_core, test_div20);
  tcase_add_test(tc_div_core, test_div21);
  tcase_add_test(tc_div_core, test_div22);
  tcase_add_test(tc_div_core, test_div23);
  tcase_add_test(tc_div_core, test_div24);
  tcase_add_test(tc_div_core, test_div25);
  tcase_add_test(tc_div_core, test_div26);
  tcase_add_test(tc_div_core, test_div27);
  suite_add_tcase(s, tc_div_core);

  return s;
}

void check_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
  s21_decimal result;
  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(code, TEST_ARITHMETIC_OK);
}

void check_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
  s21_decimal result;
  int code = s21_add(decimal1, decimal2, &result);

  ck_assert_int_eq(code, check);
}

void check_sub(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
  s21_decimal result;
  int code = s21_sub(decimal1, decimal2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(code, TEST_ARITHMETIC_OK);
}

void check_sub_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
  s21_decimal result;
  int code = s21_sub(decimal1, decimal2, &result);
  ck_assert_int_eq(code, check);
}

void check_mul(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
  s21_decimal result;
  int code = s21_mul(decimal1, decimal2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(code, TEST_ARITHMETIC_OK);
}

void check_mul_fail(s21_decimal decimal1, s21_decimal decimal2,
                    int code_check) {
  s21_decimal result;
  int code = s21_mul(decimal1, decimal2, &result);
  ck_assert_int_eq(code, code_check);
}

void check_div(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(code, TEST_ARITHMETIC_OK);
}

void check_div_fail(s21_decimal decimal1, s21_decimal decimal2,
                    int code_check) {
  s21_decimal result;
  int code = s21_div(decimal1, decimal2, &result);
  ck_assert_int_eq(code, code_check);
}