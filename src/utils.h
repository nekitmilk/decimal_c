#ifndef _UTILS_H
#define _UTILS_H

#include "s21_decimal.h"

// Делит число типа Decimal на 10 и возвращает остаток от деления.
int div_by_10(s21_decimal *value);

// Приводит к общему масштабы двух чисел типа Decimal.
void equalize_scales(s21_decimal *value_1, s21_decimal *value_2);

// Проверяет знак числа типа Decimal. 1 - если число отрицательное, 0 - если
// положительное.
int is_negative(s21_decimal value);

int is_value_equal_zero(s21_decimal value);
void bankers_rounding(s21_decimal *value, unsigned int remainder,
                      int target_scale);

// void floor_rounding(s21_decimal* value, unsigned int remainder);

// Округлить в большую сторону
void round_up(s21_decimal *value);

// Проверяет, вызовет ли переполнение следующее умножение на 10 числа типа
// Decimal.
int is_next_mul_will_cause_an_overflow(s21_decimal value);

// Проверяет, есть ли ненулевые биты в 0-15 битах
int is_invalid_bits_set(s21_decimal value);

// Проверяет масштаб числа типа Decimal на корректность.
int is_invalid_scale(s21_decimal value);

// void tieshagr_bankers_rounding(s21_decimal* num, int target_scale);
int check_decimal(const s21_decimal *value);
int check_input_decimals(s21_decimal value_1, s21_decimal value_2,
                         const s21_decimal *result);

s21_decimal get_zero_dec();

int get_sign(s21_decimal d);
void set_sign(s21_decimal *d, int sign);
int get_scale(const s21_decimal *d);
void set_scale(s21_decimal *d, int scale);

int add_bits(s21_decimal *a, s21_decimal *b, s21_decimal *result);

int multiply_by_2(s21_decimal *num);
int multiply_by_8(s21_decimal *num);
int multiply_by_10(s21_decimal *num);

void convert_decimal_to_int256(s21_decimal dec, s21_int256 *int256);
int convert_int256_to_decimal(s21_int256 int256, s21_decimal *dec);

#endif
