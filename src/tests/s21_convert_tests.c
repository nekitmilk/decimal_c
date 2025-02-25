#include "test_runner.h"

// Start tests for s21_from_decimal_to_int

START_TEST(test_from_decimal_to_int_normal) {
    int dst1 = 0, dst2 = 0;
    int error_code1 = -100, error_code2 = -100;
    int expected_dst1 = 1234, expected_dst2 = -1234;
    int expected_error_code1 = 0, expected_error_code2 = 0;
    s21_decimal src1 = {{0x00000000, 0x00000000, 1234, 0x00000000}};
    s21_decimal src2 = {{0x00000000, 0x00000000, 1234, 0x80000000}};


    error_code1 = s21_from_decimal_to_int(src1, &dst1);
    error_code2 = s21_from_decimal_to_int(src2, &dst2);

    ck_assert_int_eq(dst1, expected_dst1);
    ck_assert_int_eq(error_code1, expected_error_code1);
    ck_assert_int_eq(dst2, expected_dst2);
    ck_assert_int_eq(error_code2, expected_error_code2);
}
END_TEST

START_TEST(test_from_decimal_to_int_zero) {
    int dst1 = -100, dst2 = -100;
    int error_code1 = -100, error_code2 = -100;
    int expected_dst = 0;
    int expected_error_code = 0;
    s21_decimal src1 = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
    s21_decimal src2 = {{0x00000000, 0x00000000, 0x00000000, 0x80000000}};


    error_code1 = s21_from_decimal_to_int(src1, &dst1);
    error_code2 = s21_from_decimal_to_int(src2, &dst2);

    ck_assert_int_eq(dst1, expected_dst);
    ck_assert_int_eq(error_code1, expected_error_code);
    ck_assert_int_eq(dst2, expected_dst);
    ck_assert_int_eq(error_code2, expected_error_code);
}
END_TEST
// Add tests with not empty bits[0] and bits[1]
START_TEST(test_from_decimal_to_int_floating_point) {
    int dst1 = 0, dst2 = 0, dst3 = 0;
    int error_code1 = -100, error_code2 = -100, error_code3 = -100;
    int expected_dst1 = 1234, expected_dst2 = 12, expected_dst3 = 0;
    int expected_error_code = 0;
    s21_decimal src1 = {{0x00000000, 0x00000000, 123456, 2 << 16}};
    s21_decimal src2 = {{0x00000000, 0x00000000, 123456, 4 << 16}};
    s21_decimal src3 = {{0x00000000, 0x00000000, 123456, 10 << 16}};


    error_code1 = s21_from_decimal_to_int(src1, &dst1);
    error_code2 = s21_from_decimal_to_int(src2, &dst2);
    error_code3 = s21_from_decimal_to_int(src3, &dst3);

    ck_assert_int_eq(dst1, expected_dst1);
    ck_assert_int_eq(error_code1, expected_error_code);
    ck_assert_int_eq(dst2, expected_dst2);
    ck_assert_int_eq(error_code2, expected_error_code);
    ck_assert_int_eq(dst3, expected_dst3);
    ck_assert_int_eq(error_code3, expected_error_code);
}
END_TEST

START_TEST(test_from_decimal_to_int_limit) {
    int dst1 = -100, dst2 = -100, dst3 = -100, dst4 = -100;
    int error_code1 = -100, error_code2 = -100, error_code3 = -100, error_code4 = -100;
    int expected_dst1 = INT_MAX, expected_dst2 = -100, expected_dst3 = INT_MIN, expected_dst4 = -100;
    int expected_error_code1 = 0, expected_error_code2 = 1, expected_error_code3 = 0, expected_error_code4 = 1;
    s21_decimal src1 = {{0x00000000, 0x00000000, INT_MAX, 0x00000000}};
    s21_decimal src2 = {{0x00000000, 10, 0x00000000, 0x00000000}};
    s21_decimal src3 = {{0x00000000, 0x00000000, 0x80000000, 0x80000000}};
    s21_decimal src4 = {{0x00000000, 10, 0x00000000, 0x80000000}};


    error_code1 = s21_from_decimal_to_int(src1, &dst1);
    error_code2 = s21_from_decimal_to_int(src2, &dst2);
    error_code3 = s21_from_decimal_to_int(src3, &dst3);
    error_code4 = s21_from_decimal_to_int(src4, &dst4);

    ck_assert_int_eq(dst1, expected_dst1);
    ck_assert_int_eq(error_code1, expected_error_code1);
    ck_assert_int_eq(dst2, expected_dst2);
    ck_assert_int_eq(error_code2, expected_error_code2);
    ck_assert_int_eq(dst3, expected_dst3);
    ck_assert_int_eq(error_code3, expected_error_code3);
    ck_assert_int_eq(dst4, expected_dst4);
    ck_assert_int_eq(error_code4, expected_error_code4);
}
END_TEST

// End tests for s21_from_decimal_to_int

Suite *convert_suite() {
  Suite *s;
  TCase *tc_from_decimal_to_int_normal, *tc_from_decimal_to_int_zero,
        *tc_from_decimal_to_int_floating_point, *tc_from_decimal_to_int_limit;

  s = suite_create("convert_tests");

  tc_from_decimal_to_int_normal = tcase_create("test_from_decimal_to_int_normal");
  tcase_add_test(tc_from_decimal_to_int_normal, test_from_decimal_to_int_normal);
  suite_add_tcase(s, tc_from_decimal_to_int_normal);

  tc_from_decimal_to_int_zero = tcase_create("test_from_decimal_to_int_zero");
  tcase_add_test(tc_from_decimal_to_int_zero, test_from_decimal_to_int_zero);
  suite_add_tcase(s, tc_from_decimal_to_int_zero);
  
  tc_from_decimal_to_int_floating_point = tcase_create("test_from_decimal_to_int_floating_point");
  tcase_add_test(tc_from_decimal_to_int_floating_point, test_from_decimal_to_int_floating_point);
  suite_add_tcase(s, tc_from_decimal_to_int_floating_point);

  tc_from_decimal_to_int_limit = tcase_create("test_from_decimal_to_int_limit");
  tcase_add_test(tc_from_decimal_to_int_limit, test_from_decimal_to_int_limit);
  suite_add_tcase(s, tc_from_decimal_to_int_limit);

  return s;
}
