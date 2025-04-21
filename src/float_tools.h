#ifndef FLOAT_TOOLS_H
#define FLOAT_TOOLS_H

#include <math.h>

#include "s21_decimal.h"
#include "utils.h"

typedef union {
  float f;
  unsigned int u;
} float_bits;

double get_mantissa(s21_decimal src);
float decimal_to_float(s21_decimal src);
// src >= 0
int get_count_digits_befor_point(float src);
// src >= 0
int get_count_digits_after_point(float src);
int get_10_base_mantissa_from_float(float src, int digits_after_point);
double bround(double dValue);

#endif