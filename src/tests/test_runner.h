#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <check.h>

#include "../s21_decimal.h"
#include "../utils.h"

Suite *comparison_suite(void);
Suite *convert_suite(void);
Suite *arithmetic_suite(void);
Suite *other_functions_suite(void);

#define TEST_ARITHMETIC_OK 0
#define TEST_ARITHMETIC_BIG 1
#define TEST_ARITHMETIC_SMALL 2
#define TEST_ARITHMETIC_ZERO_DIV 3

#endif