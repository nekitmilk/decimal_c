#include "s21_decimal.h"
#include "utils.h"

static int non_valid_input(s21_decimal value, const s21_decimal* result);

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (non_valid_input(value, result)) return ERROR;

  int non_zero_flag = 0;

  while (get_scale(&value) != 0) {
    int remainder = div_by_10(&value);
    if (remainder != 0) {
      non_zero_flag = 1;
    }
  }

  if (non_zero_flag != 0 && is_negative(value)) {
    round_up(&value);
  }

  *result = value;
  return OK;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  if (non_valid_input(value, result)) return ERROR;

  int remainder = 0;
  int non_zero = 0;
  while (get_scale(&value) != 0) {
    non_zero |= remainder > 0;
    remainder = div_by_10(&value);
  }

  if (remainder == 5) {
    remainder += non_zero;
  }

  bankers_rounding(&value, remainder, ZERO_SCALE);

  *result = value;
  return OK;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  if (non_valid_input(value, result)) return ERROR;

  while (get_scale(&value) != 0) {
    div_by_10(&value);
  }

  *result = value;
  return OK;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  if (non_valid_input(value, result)) return ERROR;

  if (!is_value_equal_zero(value)) {
    value.bits[3] ^= SIGN_MASK;
    *result = value;
  }

  return OK;
}

static int non_valid_input(s21_decimal value, const s21_decimal* result) {
  if (result == NULL || is_invalid_scale(value) || is_invalid_bits_set(value)) {
    return ERROR;
  }
  return OK;
}