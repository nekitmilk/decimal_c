#include "s21_decimal.h"
#include "s21_print_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error_code = 0;
  s21_decimal min, max;
  s21_from_int_to_decimal(INT_MAX, &max);
  s21_from_int_to_decimal(INT_MIN, &min);
  if (s21_is_greater(src, max) || s21_is_less(src, min)) {
    error_code = 1;
  } else {
    /*
    change for cases where bits[0] and bits[1] != 0
    
    option 1 --> use s21_round
    s21_floor(src, &src);
    
    option 2 --> use div (integer division)
    
    option 3 --> reinventing bicycle
    */
    int sign = get_sign(src);
    *dst = src.bits[0] / (int)(pow(10, src.bits[3] >> 16 & 0x00FF));
    *dst *= (sign) ? -1 : 1;
  }
  return error_code;
}

// Добавить проверку, что конвертация прошла успешно и в зависимости от этого
// делать код возврата
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error_code = 0;
  memset(dst, 0, sizeof(s21_decimal));  // Обнуляем все биты

  if (src < 0) {
    set_sign(dst, 1);
    src = -src;
  }

  dst->bits[0] = src;  // Младшие 32 бита
  set_scale(dst, 0);   // Без дробной части

  return error_code;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error_code = 0;
  
  s21_decimal sigma = {{1, 0x00000000, 0x00000000, 29 << 16}};
  s21_decimal sigma_negate;
  s21_decimal zero = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  s21_negate(sigma, &sigma_negate);
  
  memset(dst, 0, sizeof(s21_decimal));
  if (s21_is_greater(src, sigma_negate) && s21_is_less(src, sigma) &&
      !s21_is_equal(src, zero)) {
    *dst = 0;
    error_code = 1;
  } else {
    /*
    I hope my next 10 hours of work not usless

    s21_decimal whole, fractional;
    get_whole(src, &whole);
    get_fractional(src, &fractional);
    *dst = build_whole_float(whole) + build_fractional_float(fractional) *
            get_sign(src) ? -1 : 1;
    */

    *dst = (src.bits[2] / pow(10, src.bits[3] >> 16 & 0x00FF));
  }
  return error_code;
}

/*
Я абсолютно не уверен, когда-нибудь проверим
нужно подумать над округлением
*/
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error_code = 0;
  if (src == INFINITY || src == -INFINITY || src != src) {
    error_code = 1;
  } else {
    memset(dst, 0, sizeof(s21_decimal));
    int *bit = (int *)&src;
    int sign = *bit >> 31 & 1;
    int scale = (*bit >> 23) - 127;
    int mantissa =
        (scale == -127) ? (*bit && 0x7FFFFF) : (*bit && 0x7FFFFF) | 0x1000000;
    for (; (mantissa & 0x80000000) > 0; mantissa <<= 1) {
    }  // ?
    s21_from_int_to_decimal(mantissa * (sign ? -1 : 1), dst);
    if (src != (int)src) {
      s21_decimal sc;
      s21_from_int_to_decimal((int)pow(2, scale), &sc);
      s21_div(*dst, sc, dst);
    }
  }
  return error_code;
}