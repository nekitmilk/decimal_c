#include "test_runner.h"

#define ck_assert_dec_eq(src, dst)              \
  for (int i = 0; i < 4; i++) {                 \
    ck_assert_int_eq(src.bits[i], dst.bits[i]); \
  }

// ---------------------------------------
// Start tests for s21_from_decimal_to_int

START_TEST(test_from_decimal_to_int_invalid) {
  int dst1 = 0, *dst2 = NULL;
  int error_code1 = -100, error_code2 = -100;
  s21_decimal src1 = {{1234, 0x0, 0x0, 0xFFFFFFFF}},
              src2 = {{1234, 0x0, 0x0, 0x80000000}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, dst2);

  ck_assert_int_eq(error_code1, ERROR);
  ck_assert_int_eq(error_code2, ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_int_normal) {
  int dst1 = 0, dst2 = 0;
  int error_code1 = -100, error_code2 = -100;
  int expected_dst1 = 1234, expected_dst2 = -1234;
  s21_decimal src1 = {{1234, 0x0, 0x0, 0x0}},
              src2 = {{1234, 0x0, 0x0, 0x80000000}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, &dst2);

  ck_assert_int_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_int_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
}
END_TEST

START_TEST(test_from_decimal_to_int_zero) {
  int dst1 = -100, dst2 = -100;
  int error_code1 = -100, error_code2 = -100;
  int expected_dst = 0;
  s21_decimal src1 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal src2 = {{0x0, 0x0, 0x0, 0x80000000}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, &dst2);

  ck_assert_int_eq(dst1, expected_dst);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_int_eq(dst2, expected_dst);
  ck_assert_int_eq(error_code2, OK);
}
END_TEST

START_TEST(test_from_decimal_to_int_floating_point) {
  int dst1 = 0, dst2 = 0, dst3 = 0;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100;
  int expected_dst1 = 1234, expected_dst2 = 12, expected_dst3 = 0;
  s21_decimal src1 = {{123456, 0x0, 0x0, 2 << 16}},
              src2 = {{123456, 0x0, 0x0, 4 << 16}},
              src3 = {{123456, 0x0, 0x0, 10 << 16}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, &dst2);
  error_code3 = s21_from_decimal_to_int(src3, &dst3);

  ck_assert_int_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_int_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
  ck_assert_int_eq(dst3, expected_dst3);
  ck_assert_int_eq(error_code3, OK);
}
END_TEST

START_TEST(test_from_decimal_to_int_limit) {
  int dst1 = -100, dst2 = -100, dst3 = -100, dst4 = -100;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100,
      error_code4 = -100;
  int expected_dst1 = INT_MAX, expected_dst2 = -100, expected_dst3 = INT_MIN,
      expected_dst4 = -100;

  s21_decimal src1 = {{INT_MAX, 0x0, 0x0, 0x0}};
  s21_decimal src2 = {{0x0, 10, 0x0, 0x0}};
  s21_decimal src3 = {{(unsigned int)INT_MAX + 1, 0x0, 0x0, 0x80000000}};
  s21_decimal src4 = {{0x0, 10, 0x0, 0x80000000}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, &dst2);
  error_code3 = s21_from_decimal_to_int(src3, &dst3);
  error_code4 = s21_from_decimal_to_int(src4, &dst4);

  ck_assert_int_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_int_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, ERROR);
  ck_assert_int_eq(dst3, expected_dst3);
  ck_assert_int_eq(error_code3, OK);
  ck_assert_int_eq(dst4, expected_dst4);
  ck_assert_int_eq(error_code4, ERROR);
}
END_TEST

START_TEST(test_from_decimal_to_int_floating_point_extended) {
  int dst1 = 0, dst2 = 0, dst3 = 0, dst4 = 0, dst5 = 0, dst6 = 0;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100,
      error_code4 = -100, error_code5 = -100, error_code6 = -100;
  int expected_dst1 = 1099511627, expected_dst2 = 47223664, expected_dst3 = 10,
      expected_dst4 = -1099511627, expected_dst5 = -47223664,
      expected_dst6 = -10;
  s21_decimal src1 = {{0xffffffff, 0x000000ff, 0x0, 3 << 16}},
              src2 = {{0xffffffff, 0xffffffff, 0x000000ff, 14 << 16}},
              src3 = {{0x7fffffff, 0xfc5fc966, 0x0000003a, 20 << 16}},
              src4 = {{0xffffffff, 0x000000ff, 0x0, (3 << 16) | (1U << 31)}},
              src5 = {{0xffffffff, 0xffffffff, 0x000000ff,
                       (14 << 16) | (1U << 31)}},
              src6 = {{0x7fffffff, 0xfc5fc966, 0x0000003a,
                       (20 << 16) | (1U << 31)}};

  error_code1 = s21_from_decimal_to_int(src1, &dst1);
  error_code2 = s21_from_decimal_to_int(src2, &dst2);
  error_code3 = s21_from_decimal_to_int(src3, &dst3);
  error_code4 = s21_from_decimal_to_int(src4, &dst4);
  error_code5 = s21_from_decimal_to_int(src5, &dst5);
  error_code6 = s21_from_decimal_to_int(src6, &dst6);

  ck_assert_int_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_int_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
  ck_assert_int_eq(dst3, expected_dst3);
  ck_assert_int_eq(error_code3, OK);
  ck_assert_int_eq(dst4, expected_dst4);
  ck_assert_int_eq(error_code4, OK);
  ck_assert_int_eq(dst5, expected_dst5);
  ck_assert_int_eq(error_code5, OK);
  ck_assert_int_eq(dst6, expected_dst6);
  ck_assert_int_eq(error_code6, OK);
}
END_TEST

// End tests for s21_from_decimal_to_int
// -------------------------------------
// Start tests for s21_from_int_to_decimal

START_TEST(test_from_int_to_decimal_normal) {
  int src1 = 2676, src2 = -2676;
  int error_code1 = -100, error_code2 = -100;

  s21_decimal expected_dst1 = {{2676, 0x0, 0x0, 0x0}},
              expected_dst2 = {{2676, 0x0, 0x0, 0x80000000}};
  s21_decimal dst1 = {{0x0, 0x0, 0x0, 0x0}}, dst2 = {{0x0, 0x0, 0x0, 0x0}};

  error_code1 = s21_from_int_to_decimal(src1, &dst1);
  error_code2 = s21_from_int_to_decimal(src2, &dst2);

  ck_assert_dec_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_dec_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
}
END_TEST

START_TEST(test_from_int_to_decimal_limit) {
  int src1 = INT_MAX, src2 = INT_MIN;
  int error_code1 = -100, error_code2 = -100;

  s21_decimal expected_dst1 = {{INT_MAX, 0x0, 0x0, 0x0}},
              expected_dst2 = {
                  {(unsigned int)INT_MAX + 1, 0x0, 0x0, 0x80000000}};
  s21_decimal dst1 = {{0x0, 0x0, 0x0, 0x0}}, dst2 = {{0x0, 0x0, 0x0, 0x0}};

  error_code1 = s21_from_int_to_decimal(src1, &dst1);
  error_code2 = s21_from_int_to_decimal(src2, &dst2);

  ck_assert_dec_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_dec_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
}
END_TEST

START_TEST(test_from_int_to_decimal_invalid) {
  int src1 = 1;
  int error_code1 = -100;

  s21_decimal *dst1 = NULL;

  error_code1 = s21_from_int_to_decimal(src1, dst1);

  ck_assert_int_eq(error_code1, ERROR);
}
END_TEST

// End tests for s21_from_int_to_decimal
// -------------------------------------
// Start tests for s21_from_decimal_to_float

START_TEST(test_from_decimal_to_float_normal) {
  float dst1 = FLT_MAX, dst2 = FLT_MAX, dst3 = FLT_MAX, dst4 = FLT_MAX,
        dst5 = FLT_MAX, dst6 = FLT_MAX;
  float expected_dst1 = 97767731935837369139.20,
        expected_dst2 = -97767731935837369139.20,
        expected_dst3 = 30744572723174834176.f,
        expected_dst4 = -30744572723174834176.f, expected_dst5 = 0.15625,
        expected_dst6 = -0.15625;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100,
      error_code4 = -100, error_code5 = -100, error_code6 = -100;
  s21_decimal src1 = {{0x0, 0xFFFBDC00, 0x211, 0x20000}},
              src2 = {{0x0, 0xFFFBDC00, 0x211, 0x80020000}},
              src3 = {{0x0, 0xAAAAAA00, 0x1, 0x0}},
              src4 = {{0x0, 0xAAAAAA00, 0x1, 0x80000000}},
              src5 = {{0x3D09, 0x0, 0x0, 0x50000}},
              src6 = {{0x3D09, 0x0, 0x0, 0x80050000}};

  error_code1 = s21_from_decimal_to_float(src1, &dst1);
  error_code2 = s21_from_decimal_to_float(src2, &dst2);
  error_code3 = s21_from_decimal_to_float(src3, &dst3);
  error_code4 = s21_from_decimal_to_float(src4, &dst4);
  error_code5 = s21_from_decimal_to_float(src5, &dst5);
  error_code6 = s21_from_decimal_to_float(src6, &dst6);

  ck_assert_float_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_float_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
  ck_assert_float_eq(dst3, expected_dst3);
  ck_assert_int_eq(error_code3, OK);
  ck_assert_float_eq(dst4, expected_dst4);
  ck_assert_int_eq(error_code4, OK);
  ck_assert_float_eq(dst5, expected_dst5);
  ck_assert_int_eq(error_code5, OK);
  ck_assert_float_eq(dst6, expected_dst6);
  ck_assert_int_eq(error_code6, OK);
}
END_TEST

START_TEST(test_from_decimal_to_float_zero) {
  float dst1 = FLT_MAX, dst2 = FLT_MAX;
  float expected_dst1 = 0.f, expected_dst2 = -0.f;
  int error_code1 = -100, error_code2 = -100;
  s21_decimal src1 = {{0x0, 0x0, 0x0, 0x0}},
              src2 = {{0x0, 0x0, 0x0, 0x80000000}};

  error_code1 = s21_from_decimal_to_float(src1, &dst1);
  error_code2 = s21_from_decimal_to_float(src2, &dst2);

  ck_assert_float_eq(dst1, expected_dst1);
  ck_assert_int_eq(error_code1, OK);
  ck_assert_float_eq(dst2, expected_dst2);
  ck_assert_int_eq(error_code2, OK);
}
END_TEST

START_TEST(test_from_decimal_to_float_invalid) {
  float *dst1 = NULL, dst2 = FLT_MAX;
  int error_code1 = -100, error_code2 = -100;
  s21_decimal src1 = {{0x0, 0x0, 0x0, 0x0}},
              src2 = {{0x0, 0x0, 0x0, 0xFFFFFFFF}};

  error_code1 = s21_from_decimal_to_float(src1, dst1);
  error_code2 = s21_from_decimal_to_float(src2, &dst2);

  ck_assert_int_eq(error_code1, ERROR);
  ck_assert_int_eq(error_code2, ERROR);
}
END_TEST

// End tests for s21_from_decimal_to_float
// -------------------------------------
// Start tests for s21_from_float_to_decimal

START_TEST(test_from_float_to_decimal_normal) {
  float src1 = 0.0000000000000000000775886, src2 = -0.0000000000000000000775886,
        src3 = 773027200000000000000000000.f,
        src4 = -773027200000000000000000000.f,
        src5 = 0.0000000000000000000000000012,
        src6 = -0.0000000000000000000000000012, src7 = 241.244507, src8 = 3.0f;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100,
      error_code4 = -100, error_code5 = -100, error_code6 = -100,
      error_code7 = -100, error_code8 = -100;
  s21_decimal expected_dst1 = {{0xBD6CE, 0x0, 0x0, 0x190000}},
              expected_dst2 = {{0xBD6CE, 0x0, 0x0, 0x80190000}},
              expected_dst3 = {{0x66000000, 0x7B44D9C0, 0x27F6EE0, 0x0}},
              expected_dst4 = {{0x66000000, 0x7B44D9C0, 0x27F6EE0, 0x80000000}},
              expected_dst5 = {{0xC, 0x0, 0x0, 0x1C0000}},
              expected_dst6 = {{0xC, 0x0, 0x0, 0x801C0000}},
              expected_dst7 = {{0x24CF9D, 0x0, 0x0, 0x40000}},
              expected_dst8 = {{0x002dc6c0, 0x0, 0x0, 0x60000}};
  s21_decimal dst1 = {{0x0, 0x0, 0x0, 0x0}}, dst2 = {{0x0, 0x0, 0x0, 0x0}},
              dst3 = {{0x0, 0x0, 0x0, 0x0}}, dst4 = {{0x0, 0x0, 0x0, 0x0}},
              dst5 = {{0x0, 0x0, 0x0, 0x0}}, dst6 = {{0x0, 0x0, 0x0, 0x0}},
              dst7 = {{0x0, 0x0, 0x0, 0x0}}, dst8 = {{0x0, 0x0, 0x0, 0x0}};

  error_code1 = s21_from_float_to_decimal(src1, &dst1);
  error_code2 = s21_from_float_to_decimal(src2, &dst2);
  error_code3 = s21_from_float_to_decimal(src3, &dst3);
  error_code4 = s21_from_float_to_decimal(src4, &dst4);
  error_code5 = s21_from_float_to_decimal(src5, &dst5);
  error_code6 = s21_from_float_to_decimal(src6, &dst6);
  error_code7 = s21_from_float_to_decimal(src7, &dst7);
  error_code8 = s21_from_float_to_decimal(src8, &dst8);

  ck_assert_dec_eq(dst1, expected_dst1);
  ck_assert_float_eq(error_code1, OK);
  ck_assert_dec_eq(dst2, expected_dst2);
  ck_assert_float_eq(error_code2, OK);
  ck_assert_dec_eq(dst3, expected_dst3);
  ck_assert_float_eq(error_code3, OK);
  ck_assert_dec_eq(dst4, expected_dst4);
  ck_assert_float_eq(error_code4, OK);
  ck_assert_dec_eq(dst5, expected_dst5);
  ck_assert_float_eq(error_code5, OK);
  ck_assert_dec_eq(dst6, expected_dst6);
  ck_assert_float_eq(error_code6, OK);
  ck_assert_dec_eq(dst7, expected_dst7);
  ck_assert_float_eq(error_code7, OK);
  ck_assert_dec_eq(dst8, expected_dst8);
  ck_assert_float_eq(error_code8, OK);
}
END_TEST

START_TEST(test_from_float_to_decimal_invalid) {
  float src1 = INFINITY, src2 = -INFINITY, src3 = NAN, src4 = 1.e-30,
        src5 = 0.f;
  int error_code1 = -100, error_code2 = -100, error_code3 = -100,
      error_code4 = -100, error_code5 = -100;
  s21_decimal expected_dst4 = {{0x0, 0x0, 0x0, 0x0}};
  s21_decimal dst1 = {{0x0, 0x0, 0x0, 0x0}}, dst2 = {{0x0, 0x0, 0x0, 0x0}},
              dst3 = {{0x0, 0x0, 0x0, 0x0}}, dst4 = {{0x0, 0x0, 0x0, 0x0}},
              *dst5 = NULL;

  error_code1 = s21_from_float_to_decimal(src1, &dst1);
  error_code2 = s21_from_float_to_decimal(src2, &dst2);
  error_code3 = s21_from_float_to_decimal(src3, &dst3);
  error_code4 = s21_from_float_to_decimal(src4, &dst4);
  error_code5 = s21_from_float_to_decimal(src5, dst5);

  ck_assert_float_eq(error_code1, ERROR);
  ck_assert_float_eq(error_code2, ERROR);
  ck_assert_float_eq(error_code3, ERROR);
  ck_assert_dec_eq(dst4, expected_dst4);
  ck_assert_float_eq(error_code4, ERROR);
  ck_assert_float_eq(error_code5, ERROR);
}
END_TEST

// End tests for s21_from_float_to_decimal

Suite *convert_suite() {
  Suite *s;
  TCase *tc_from_decimal_to_int_normal, *tc_from_decimal_to_int_zero,
      *tc_from_decimal_to_int_floating_point, *tc_from_decimal_to_int_limit,
      *tc_from_decimal_to_int_floating_point_extended,
      *tc_from_decimal_to_int_invalid;

  TCase *tc_from_int_to_decimal_normal, *tc_from_int_to_decimal_limit,
      *tc_from_int_to_decimal_invalid;

  TCase *tc_from_decimal_to_float_normal, *tc_from_decimal_to_float_zero,
      *tc_from_decimal_to_float_invalid;

  TCase *tc_from_float_to_decimal_normal, *tc_from_float_to_decimal_invalid;

  s = suite_create("convert_tests");

  tc_from_decimal_to_int_normal =
      tcase_create("test_from_decimal_to_int_normal");
  tcase_add_test(tc_from_decimal_to_int_normal,
                 test_from_decimal_to_int_normal);
  suite_add_tcase(s, tc_from_decimal_to_int_normal);

  tc_from_decimal_to_int_zero = tcase_create("test_from_decimal_to_int_zero");
  tcase_add_test(tc_from_decimal_to_int_zero, test_from_decimal_to_int_zero);
  suite_add_tcase(s, tc_from_decimal_to_int_zero);

  tc_from_decimal_to_int_floating_point =
      tcase_create("test_from_decimal_to_int_floating_point");
  tcase_add_test(tc_from_decimal_to_int_floating_point,
                 test_from_decimal_to_int_floating_point);
  suite_add_tcase(s, tc_from_decimal_to_int_floating_point);

  tc_from_decimal_to_int_limit = tcase_create("test_from_decimal_to_int_limit");
  tcase_add_test(tc_from_decimal_to_int_limit, test_from_decimal_to_int_limit);
  suite_add_tcase(s, tc_from_decimal_to_int_limit);

  tc_from_decimal_to_int_floating_point_extended =
      tcase_create("test_from_decimal_to_int_floating_point_extended");
  tcase_add_test(tc_from_decimal_to_int_floating_point_extended,
                 test_from_decimal_to_int_floating_point_extended);
  suite_add_tcase(s, tc_from_decimal_to_int_floating_point_extended);

  tc_from_decimal_to_int_invalid =
      tcase_create("test_from_decimal_to_int_invalid");
  tcase_add_test(tc_from_decimal_to_int_invalid,
                 test_from_decimal_to_int_invalid);
  suite_add_tcase(s, tc_from_decimal_to_int_invalid);

  tc_from_int_to_decimal_normal =
      tcase_create("test_from_int_to_decimal_normal");
  tcase_add_test(tc_from_int_to_decimal_normal,
                 test_from_int_to_decimal_normal);
  suite_add_tcase(s, tc_from_int_to_decimal_normal);

  tc_from_int_to_decimal_limit = tcase_create("test_from_int_to_decimal_limit");
  tcase_add_test(tc_from_int_to_decimal_limit, test_from_int_to_decimal_limit);
  suite_add_tcase(s, tc_from_int_to_decimal_limit);

  tc_from_int_to_decimal_invalid =
      tcase_create("test_from_int_to_decimal_invalid");
  tcase_add_test(tc_from_int_to_decimal_invalid,
                 test_from_int_to_decimal_invalid);
  suite_add_tcase(s, tc_from_int_to_decimal_invalid);

  tc_from_decimal_to_float_normal =
      tcase_create("test_from_decimal_to_float_normal");
  tcase_add_test(tc_from_decimal_to_float_normal,
                 test_from_decimal_to_float_normal);
  suite_add_tcase(s, tc_from_decimal_to_float_normal);

  tc_from_decimal_to_float_zero =
      tcase_create("test_from_decimal_to_float_zero");
  tcase_add_test(tc_from_decimal_to_float_zero,
                 test_from_decimal_to_float_zero);
  suite_add_tcase(s, tc_from_decimal_to_float_zero);

  tc_from_decimal_to_float_invalid =
      tcase_create("test_from_decimal_to_float_invalid");
  tcase_add_test(tc_from_decimal_to_float_invalid,
                 test_from_decimal_to_float_invalid);
  suite_add_tcase(s, tc_from_decimal_to_float_invalid);

  tc_from_float_to_decimal_normal =
      tcase_create("test_from_float_to_decimal_normal");
  tcase_add_test(tc_from_float_to_decimal_normal,
                 test_from_float_to_decimal_normal);
  suite_add_tcase(s, tc_from_float_to_decimal_normal);

  tc_from_float_to_decimal_invalid =
      tcase_create("test_from_float_to_decimal_invalid");
  tcase_add_test(tc_from_float_to_decimal_invalid,
                 test_from_float_to_decimal_invalid);
  suite_add_tcase(s, tc_from_float_to_decimal_invalid);

  return s;
}