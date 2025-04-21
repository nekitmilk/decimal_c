#ifndef s21_int256_h
#define s21_int256_h

#include <stdio.h>

typedef struct {
  unsigned int bits[8];
} s21_int256;

int get_sign_int256(s21_int256 d);
void set_sign_int256(s21_int256 *d, int sign);
int get_scale_int256(const s21_int256 *d);
void set_scale_int256(s21_int256 *d, int scale);
int align_scales_int256(s21_int256 *a, s21_int256 *b);

int multiply_by_10_int256(s21_int256 *num);
int multiply_by_2_int256(s21_int256 *num);
int multiply_by_8_int256(s21_int256 *num);

int left_shift_1_int256(s21_int256 *num);
int right_shift_1_int256(s21_int256 *num);

void left_shift_int256(s21_int256 *num, int shift);
void right_shift_int256(s21_int256 *num, int shift);

int add_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result);
int sub_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result);
int mul_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result);
int division_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *div_result,
                         s21_int256 *mod_result);

int compare_bits_int256(s21_int256 a, s21_int256 b);
int equal_zero_int256(s21_int256 value);

s21_int256 get_zero_int256(void);
s21_int256 get_one_int256(void);

int get_not_zero_bit_int256(s21_int256 value);
int is_set_bit_int256(s21_int256 value, int position);

void tieshagr_bankers_rounding_int256(s21_int256 *num, int target_scale);

#endif