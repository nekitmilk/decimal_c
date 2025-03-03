#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H



#include <check.h>

#include "../s21_decimal.h"

Suite *comparison_suite(void);
Suite *convert_suite(void);
Suite *arithmetic_suite(void);
Suite* other_functions_suite(void);
// сюда дописываем свои suite функции


// Доп тесты, удалить
#define TEST_ARITHMETIC_OK 0
#define TEST_ARITHMETIC_BIG 1
#define TEST_ARITHMETIC_SMALL 2
#define TEST_ARITHMETIC_ZERO_DIV 3

Suite *add_suite0(void);
Suite *add_suite1(void);
Suite *add_suite2(void);
Suite *add_suite3(void);
Suite *add_suite4(void);
Suite *add_suite5(void);
Suite *add_suite6(void);
Suite *add_suite7(void);
Suite *add_suite8(void);
Suite *add_suite9(void);

#endif