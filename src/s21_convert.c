#include "float_tools.h"
#include "s21_decimal.h"
#include "utils.h"

#define SIGMA                               \
  {                                         \
    { 1, 0x00000000, 0x00000000, 29 << 16 } \
  }
#define MAX_FLOAT 79228162514264337593543950335.f
#define MIN_FLOAT -79228162514264337593543950335.f
#define SIGNIFICANT_DIGITS 7

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error_code = 0;
  s21_decimal min, max;
  s21_from_int_to_decimal(INT_MAX, &max);
  s21_from_int_to_decimal(INT_MIN, &min);
  if (dst == NULL || check_decimal(&src)) {
    error_code = 1;
  }
  if (!error_code) {
    s21_truncate(src, &src);
  }
  if (s21_is_greater(src, max) || s21_is_less(src, min)) {
    error_code = 1;
  }
  if (!error_code) {
    int sign = get_sign(src);
    unsigned int mantissa = (unsigned int)get_mantissa(src);

    if (mantissa == 0x80000000) {
      *dst = 0x80000000;
    } else {
      *dst = (int)mantissa;
      *dst *= (sign) ? -1 : 1;
    }
  }
  return error_code;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error_code = 0;
  if (dst == NULL) {
    error_code = 1;
  } else {
    *dst = get_zero_dec();
    if (src < 0) {
      set_sign(dst, 1);
      if (src != INT_MIN) {
        src = -src;
      }
    }
    dst->bits[0] = src;
    set_scale(dst, 0);
  }
  return error_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error_code = 0;
  if (dst == NULL || check_decimal(&src)) {
    error_code = 1;
  } else {
    *dst = decimal_to_float(src);
  }
  return error_code;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error_code = 0;
  if (src == INFINITY || src == -INFINITY || src != src || src <= MIN_FLOAT ||
      src >= MAX_FLOAT || dst == NULL) {
    error_code = 1;
  } else if (fabs(src) > 0 && fabs(src) < 1e-28) {
    error_code = 1;
    *dst = get_zero_dec();
  } else {
    *dst = get_zero_dec();
    float_bits fb;
    fb.f = src;
    // int *bit = (int *)&src;
    int sign = (fb.u >> 31) & 1;
    src = !sign ? src : -src;

    int digits_befor_point = get_count_digits_befor_point(src);
    int digits_after_point = get_count_digits_after_point(src);
    int mantissa = get_10_base_mantissa_from_float(src, digits_after_point);

    int shift = 0;
    for (; mantissa % 10 == 0 && mantissa != 0 && digits_befor_point == 0;
         mantissa /= 10) {
      shift++;
    }
    s21_from_int_to_decimal(mantissa, dst);
    // в целой части >= 7 значащих знаков
    if (digits_befor_point >= SIGNIFICANT_DIGITS) {
      digits_befor_point += shift;
      for (int i = digits_befor_point - SIGNIFICANT_DIGITS; i > 0; i--) {
        multiply_by_10(dst);
      }
    }
    // в целой части < 7 значащих знаков, в оставшихся допустимых значащих
    // знаках дробной части есть знаки
    else if ((int)src != 0 &&
             mantissa / (int)pow(10, SIGNIFICANT_DIGITS - digits_befor_point) !=
                 0) {
      set_scale(dst, SIGNIFICANT_DIGITS - digits_befor_point - shift);
    }
    // в целой части нет значащих знаков
    else if ((int)src == 0) {
      set_scale(dst,
                (digits_after_point > 28 ? 28 : digits_after_point) - shift);
    }
    set_sign(dst, sign);
  }
  return error_code;
}