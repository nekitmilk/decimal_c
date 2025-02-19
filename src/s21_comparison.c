#include "s21_decimal.h"
#include "utils.h"

static int compare_values(s21_decimal value_1, s21_decimal value_2);

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue = 1;

  if (is_negative(value_1) && !is_negative(value_2)) {
    result = 1;
    need_to_continue = 0;
  } else if (!is_negative(value_1) && is_negative(value_2)) {
    result = 0;
    need_to_continue = 0;
  }

  need_to_continue &= (equalize_scales(&value_1, &value_2) != 0);

  if (need_to_continue) {
    result = compare_values(value_1, value_2);
  }

  return result;
}

static int compare_values(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int is_values_equal = 0;

  if (value_1.bits[2] < value_2.bits[2]) {
    result = 1;
  } else if (value_1.bits[2] > value_2.bits[2]) {
    result = 0;
  } else if (value_1.bits[1] < value_2.bits[1]) {
    result = 1;
  } else if (value_1.bits[1] > value_2.bits[1]) {
    result = 0;
  } else if (value_1.bits[0] < value_2.bits[0]) {
    result = 1;
  } else if (value_1.bits[0] > value_2.bits[0]) {
    result = 0;
  } else {
    is_values_equal = 1;
  }

  if (is_negative(value_1) && is_negative(value_2)) invert_result(&result);
  if (is_values_equal) result = 0;

  return result;
}

int is_negative(s21_decimal value) { return (value.bits[3] >> 31) & 1; }

void invert_result(int *result) {
  if (*result == 0) {
    *result = 1;
  } else if (*result == 1) {
    *result = 0;
  }
}