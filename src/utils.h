#ifndef _UTILS_H
#define _UTILS_H

#include "s21_decimal.h"

// int
// void mul_by_10(s21_decimal value, s21_decimal* result);
int mul_by_10(s21_decimal* value);

// void div_by_10(s21_decimal value, s21_decimal* result);
void div_by_10(s21_decimal* value,
               void (*rounding_function)(s21_decimal*, unsigned int));

void equalize_scales(s21_decimal* value_1, s21_decimal* value_2);

// int equalize(s21_decimal* value, int* scale_to_equalize, int target_scale);
void equalize(s21_decimal* value_with_smaller_scale,
              s21_decimal* value_with_larger_scale, int smaller_scale,
              int larger_scale);

int is_negative(s21_decimal value);
int is_value_equal_zero(s21_decimal value);
void bankers_rounding(s21_decimal* value, unsigned int remainder);
void floor_rounding(s21_decimal* value, unsigned int remainder);
void round_up(s21_decimal* value);
int is_next_mul_will_cause_an_overflow(s21_decimal value);
// int get_scale(s21_decimal value);
#endif
