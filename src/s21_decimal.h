#ifndef s21_decimal_h
#define s21_decimal_h

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "s21_int256.h"

#define SIGN_MASK 0x80000000
#define ZERO_SCALE 0

#define OK 0
#define ERROR 1

typedef struct {
  unsigned int bits[4];
} s21_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);              // <
int s21_is_less_or_equal(s21_decimal, s21_decimal);     // <=
int s21_is_greater(s21_decimal, s21_decimal);           // >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // >=
int s21_is_equal(s21_decimal, s21_decimal);             // ==
int s21_is_not_equal(s21_decimal, s21_decimal);         // !=

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result);

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result);

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

#endif /* s21_decimal_h */