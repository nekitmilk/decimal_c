#ifndef s21_decimal_h
#define s21_decimal_h

#include <stdio.h>
#include <string.h>

#define SIGN_MASK 0x80000000

typedef struct {
  unsigned int bits[4];
} s21_decimal;

int get_sign(s21_decimal d);
void set_sign(s21_decimal *d, int sign);
int get_scale(const s21_decimal *d);
void set_scale(s21_decimal *d, int scale);

void align_scales(s21_decimal *a, s21_decimal *b);
int multiply_by_10(s21_decimal *num);
int add_bits(s21_decimal *a, s21_decimal *b, s21_decimal *result);
int sub_bits(s21_decimal *a, s21_decimal *b, s21_decimal *result);
int multiply_by_2(s21_decimal *num);
int multiply_by_8(s21_decimal *num);
int compare_bits(s21_decimal a, s21_decimal b);

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

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif /* s21_decimal_h */