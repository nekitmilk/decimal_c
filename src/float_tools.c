#include "float_tools.h"

static int check_digit(float src, int i, int *count);

double get_mantissa(s21_decimal src) {
  double result = 0.0;
  for (int i = 0; i < 3; i++) {
    result += pow(2, 32 * i) * (double)src.bits[i];
  }
  return result;
}

float decimal_to_float(s21_decimal src) {
  double mantissa = get_mantissa(src);
  double value = (mantissa / pow(10, get_scale(&src)));
  value = get_sign(src) ? -value : value;
  return (float)value;
}

int get_count_digits_befor_point(float src) {
  int count = 0;
  for (; (long)(src / pow(10, count)); count++) {
  }
  return count;
}

int get_count_digits_after_point(float src) {
  int count = 1;
  for (int flag = 1; count < 64 && flag; count++) {
    if ((long)(src / pow(10, -count - 1)) / 10 % 10 != 0) {
      for (int i = 6, stop = 0; i > 0 && !stop; i--) {
        stop += check_digit(src, i, &count);
      }
      flag--;
    }
  }
  return count == 64 ? 0 : count;
}

int get_10_base_mantissa_from_float(float src, int digits_after_point) {
  int mantissa = 0;
  for (int i = 1000, flag = 1; i > -1000 && flag; i--) {
    if ((int)(src / pow(10, i)) > 0 && (int)(src / pow(10, i) < 10)) {
      mantissa = digits_after_point > 28 && (int)src == 0
                     ? (int)bround((double)src / pow(10, i - 7) /
                                   pow(10, digits_after_point - 27))
                     : (int)bround((double)src / pow(10, i - 7) / 10);
      flag--;
    }
  }
  return mantissa;
}

double bround(double value) {
  double f = floor(value + 0.5), c = ceil(value - 0.5), integer,
         fractional = modf(value, &integer);
  return ((int)integer % 2 == 1 && fractional == 0.5) ? f : c;
}

static int check_digit(float src, int i, int *count) {
  int stop = 0;
  if ((long)bround(src / pow(10, -*count - i + 1)) != 0) {
    *count += i - 1;
    stop++;
  }
  return stop;
}