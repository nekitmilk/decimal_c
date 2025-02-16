#ifndef _UTILS_H
#define _UTILS_H

#include "s21_decimal.h"

int s21_mul_by_10(s21_decimal value, s21_decimal* result);
int equalize_scales(s21_decimal* value_1, s21_decimal* value_2);
int equalize(s21_decimal* value, int* scale_to_equalize, int target_scale);

#endif
