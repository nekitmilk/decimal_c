#include "test_runner.h"

void ck_assert_dec_eq (s21_decimal result, s21_decimal expected_result) {
    ck_assert_int_eq(result.bits[0], expected_result.bits[0]);
    ck_assert_int_eq(result.bits[1], expected_result.bits[1]);
    ck_assert_int_eq(result.bits[2], expected_result.bits[2]);
    ck_assert_int_eq(result.bits[3], expected_result.bits[3]);
}

START_TEST(add_normal_1) {
    // Нормальная ситуация 123 + 17 = 140
    s21_decimal value_1 = {{0x7b, 0, 0, 0}}; // 123
    s21_decimal value_2 = {{0x11, 0, 0, 0}}; // 17
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

    s21_decimal expected_1 = {{0x8C, 0, 0, 0}}; // 140
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
    s21_decimal value_1 = {{0x7b, 0, 0, 0}}; // 123
    set_sign(&value_1, 1);
    s21_decimal value_2 = {{0x11, 0, 0, 0}}; // 17
    set_sign(&value_2, 1);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

    s21_decimal expected_1 = {{0x8C, 0, 0, 0}}; // 140
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
    s21_decimal value_1 = {{0x9F84C2C7, 0x1589C8F, 0, 0}}; // 96999532659,000007
    set_scale(&value_1, 6);
    s21_decimal value_2 = {{0x9665456A, 0x2A3D6270, 0x3, 0}}; // 5838392938,0000056682
    set_scale(&value_2, 10);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

// 37 BFA10CB7 C85DC418
    s21_decimal expected_1 = {{0xC85DC2DA, 0xBFA10CB7, 0x37, 0}}; // 102 837 925 597,0000127000
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
    s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}}; // 4 294 967 295
    s21_decimal value_2 = {{0xD0FD5, 0, 0, 0}}; // 856 021
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

    s21_decimal expected_1 = {{0x000D0FD4, 1, 0, 0}}; // 4 295 823 316
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
    s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}}; // 4 294 967 295
    s21_decimal value_2 = {{0, 0xD0FD5, 0, 0}}; // 856 021
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

    s21_decimal expected_1 = {{0, 0x000D0FD4, 1, 0}}; // 4 295 823 316
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
    s21_decimal value_1 = {{0x7b, 0, 0, 0}}; // 123
    s21_decimal value_2 = {{0x17, 0, 0, 0}}; // -23
    set_sign(&value_2, 1);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}};

    s21_decimal expected_1 = {{0x64, 0, 0, 0}}; // 140
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
    s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}}; // 18446744069414584320
    s21_decimal value_2 = {{0x1, 0, 0, 0}}; // 1 * 10^-27
    set_scale(&value_2, 27);
    set_sign(&value_2, 1);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}}; // 18446744069414584319,999...

// 3b9ac9ff c4653600 00000000
    s21_decimal expected_1 = {{0, 0xc4653600, 0x3b9ac9ff, 0}}; // 18446744069414584320,00...
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
    s21_decimal value_1 = {{0, 0xFFFFFFFF, 0, 0}}; // 18446744069414584320
    s21_decimal value_2 = {{0x1, 0, 0, 0}}; // 1 * 10^-27
    set_scale(&value_2, 27);
    s21_decimal result_1 = {{0}};
    s21_decimal result_2 = {{0}}; // 18446744069414584320 + 1 * 10^-27

    s21_decimal expected_1 = {{0, 0xc4653600, 0x3b9ac9ff, 0}}; // 18446744069414584320,00...
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
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}; // Максимальное значение
    s21_decimal value_2 = {{0x1, 0, 0, 0x00010000}}; // 0.1 (scale = 1)
    s21_decimal result = {{0}};

    // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,335 (округление вниз)
    s21_decimal expected = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int expected_err = 0;

    int err = s21_add(value_1, value_2, &result);
    ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_2) {
    // Максимальное значение мантиссы: 79,228,162,514,264,337,593,543,950,335
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}; // Максимальное значение
    s21_decimal value_2 = {{0x6, 0, 0, 0x80010000}}; // -0.6 (scale = 1)
    s21_decimal result = {{0}};

    // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,334 (округление вниз)
    s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int expected_err = 0;

    int err = s21_add(value_1, value_2, &result);
    ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_3) {
    // Максимальное значение мантиссы: 79,228,162,514,264,337,593,543,950,334
    s21_decimal value_1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}}; // Максимальное значение - 1
    s21_decimal value_2 = {{0x5, 0, 0, 0x00010000}}; // 0.5 (scale = 1)
    s21_decimal result = {{0}};

    // Ожидаемый результат: 79,228,162,514,264,337,593,543,950,334 (округление вниз, так как мантисса четная)
    s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    int expected_err = 0;

    int err = s21_add(value_1, value_2, &result);
    ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_4) {
    // Минимальное значение мантиссы: -79,228,162,514,264,337,593,543,950,335
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}; // Минимальное значение
    s21_decimal value_2 = {{0x6, 0, 0, 0x80010000}}; // -0.6 (scale = 1)
    s21_decimal result = {{0}};

    // Ожидаемый результат: -79,228,162,514,264,337,593,543,950,334 (округление вниз)
    // s21_decimal expected = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
    int expected_err = 2;

    int err = s21_add(value_1, value_2, &result);
    // ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_5) {
    // Максимальное значение мантиссы: 7,9,228,162,514,264,337,593,543,950,335
    s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}}; // Максимальное значение, scale = 28
    s21_decimal value_2 = {{0x6, 0, 0, 0x001C0000}}; // 0.0000000000000000000000000006 (scale = 28)
    s21_decimal result = {{0}};

    int expected_err = 1;

    int err = s21_add(value_1, value_2, &result);
    //ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

START_TEST(mantis_boundary_rounding_6) {
    // Обычное число: 123.45
    s21_decimal value_1 = {{12345, 0, 0, 0x00020000}}; // 123.45 (scale = 2)
    s21_decimal value_2 = {{6, 0, 0, 0x00180000}}; // 0.0000000000000000000000000006 (scale = 24)
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
    s21_decimal value_1 = {{12345, 0, 0, 0x80020000}}; // -123.45 (scale = 2)
    s21_decimal value_2 = {{6, 0, 0, 0x00180000}}; // 0.0000000000000000000000000006 (scale = 24)
    s21_decimal result = {{0}};

    s21_decimal expected = {{0xb03ffffa, 0xe8dfebfd, 0x00661d8d, 0x80180000}}; // -123.4499999999999999999999999994
    int expected_err = 0;

    int err = s21_add(value_1, value_2, &result);
    ck_assert_dec_eq(result, expected);
    ck_assert_int_eq(err, expected_err);
}
END_TEST

// Здесь должен быть блок тестов на банковское округление
// ОБЯЗАТЕЛЬНО тесты, в которых будет задействовано банк округл с двумя положительными числами

// Тесты вычитания двух одинаковых чисел

// Тест число с очень большим масштабом + число: 10,000...0 + 187654 = 187664

Suite *arithmetic_suite() {
  Suite *s;
  TCase *tc_add_normal, *tc_add_transfering, *tc_add_diff_sign, *tc_add_overflow, *tc_add_overmin, *tc_mantis_boundary_rounding;

  s = suite_create("add_tests");

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

  tc_mantis_boundary_rounding = tcase_create("test_add_mantis_boundary_rounding");
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_1);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_2);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_3);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_4);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_5);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_6);
  tcase_add_test(tc_mantis_boundary_rounding, mantis_boundary_rounding_7);
  suite_add_tcase(s, tc_mantis_boundary_rounding);

  return s;
}