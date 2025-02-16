#include "s21_decimal.h"
#include "utils.h"

static int compare_values(s21_decimal value_1, s21_decimal value_2);

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;
  int need_to_continue = 1;

  int is_value_1_negative = (value_1.bits[3] >> 31) & 1;
  int is_value_2_negative = (value_2.bits[3] >> 31) & 1;

  if (is_value_1_negative && !is_value_2_negative) {
    result = 1;
    need_to_continue = 0;
  } else if (!is_value_1_negative && is_value_2_negative) {
    result = 0;
    need_to_continue = 0;
  }

  need_to_continue &= equalize_scales(&value_1, &value_2);

  if (need_to_continue) result = compare_values(value_1, value_2);

  return result;
}

static int compare_values(s21_decimal value_1, s21_decimal value_2) {
  int result = -1;

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
  } else {
    result = 0;
  }

  return result;
}