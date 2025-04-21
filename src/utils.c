#include "utils.h"

static void equalize(s21_decimal *value_with_smaller_scale,
                     s21_decimal *value_with_larger_scale);

void equalize_scales(s21_decimal *value_1, s21_decimal *value_2) {
  if (get_scale(value_1) < get_scale(value_2)) {
    equalize(value_1, value_2);
  } else if (get_scale(value_2) < get_scale(value_1)) {
    equalize(value_2, value_1);
  }
}

static void equalize(s21_decimal *value_with_smaller_scale,
                     s21_decimal *value_with_larger_scale) {
  int smaller_scale = get_scale(value_with_smaller_scale);
  int larger_scale = get_scale(value_with_larger_scale);

  while ((smaller_scale != larger_scale) &&
         !is_next_mul_will_cause_an_overflow(*value_with_smaller_scale)) {
    multiply_by_10(value_with_smaller_scale);
    ++smaller_scale;
    set_scale(value_with_smaller_scale, smaller_scale);
  }

  while (smaller_scale != larger_scale) {
    int remainder = div_by_10(value_with_larger_scale);
    --larger_scale;
    bankers_rounding(value_with_larger_scale, remainder, smaller_scale);
  }
}

int div_by_10(s21_decimal *value) {
  s21_decimal temp_result = {{0, 0, 0, 0}};
  unsigned int remainder = 0;

  for (int i = 2; i >= 0; --i) {
    unsigned long long current =
        ((unsigned long long)remainder << 32) | value->bits[i];
    temp_result.bits[i] = (unsigned int)(current / 10);
    remainder = (unsigned int)(current % 10);
  }

  int scale = get_scale(value) - 1;
  temp_result.bits[3] = (value->bits[3] & SIGN_MASK) | scale << 16;
  *value = temp_result;

  return remainder;
}

void bankers_rounding(s21_decimal *value, unsigned int remainder,
                      int target_scale) {
  if (get_scale(value) == target_scale) {
    if (remainder > 5) {
      round_up(value);

    } else if (remainder == 5) {
      int is_even = !(value->bits[0] & 1);
      if (!is_even) {
        round_up(value);
      }
    }
  }
}

void round_up(s21_decimal *value) {
  for (int i = 0, need_to_continue = 1; i < 3 && need_to_continue; ++i) {
    ++value->bits[i];
    if (value->bits[i] != 0) {
      need_to_continue = 0;
    }
  }
}

int is_value_equal_zero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int check_decimal(const s21_decimal *value) {
  int status = 0;

  if (is_invalid_scale(*value)) {
    status = 1;
  } else if (is_invalid_bits_set(*value)) {
    status = 1;
  }

  return status;
}

int check_input_decimals(s21_decimal value_1, s21_decimal value_2,
                         const s21_decimal *result) {
  int status = 0;

  if (result == NULL || check_decimal(&value_1) || check_decimal(&value_2)) {
    status = 1;
  }

  return status;
}

s21_decimal get_zero_dec() {
  s21_decimal zero = {0};
  return zero;
}

int is_next_mul_will_cause_an_overflow(s21_decimal value) {
  return value.bits[2] >= 0x19999999;
}

int is_invalid_bits_set(s21_decimal value) {
  return (value.bits[3] & 0x7F00FFFF) != 0;
}

int is_invalid_scale(s21_decimal value) { return (get_scale(&value) > 28); }

int get_sign(s21_decimal d) { return (d.bits[3] >> 31) & 1; }

void set_sign(s21_decimal *d, int sign) {
  d->bits[3] &= ~(1U << 31);
  d->bits[3] |= (sign & 1U) << 31;
}

int get_scale(const s21_decimal *d) { return (d->bits[3] >> 16) & 0xFF; }

void set_scale(s21_decimal *d, int scale) {
  d->bits[3] &= ~(0xFF << 16);
  d->bits[3] |= (scale & 0xFF) << 16;
}

int multiply_by_10(s21_decimal *num) {
  int status = 0;

  s21_decimal temp2 = *num;
  s21_decimal temp8 = *num;

  if (multiply_by_2(&temp2) != 0) {
    status = 1;
  }

  if (multiply_by_8(&temp8) != 0) {
    status = 1;
  }

  if (!status) {
    add_bits(&temp2, &temp8, num);
  }

  return status;
}

int multiply_by_2(s21_decimal *num) {
  int status = 0;

  int in_my_mind = 0;
  if (num->bits[0] & 0x80000000) {
    in_my_mind = 1;
  }

  num->bits[0] <<= 1;

  for (int i = 1; i < 3; i++) {
    if (in_my_mind) {
      in_my_mind = 0;
      if (num->bits[i] & 0x80000000) {
        in_my_mind = 1;
      }
      num->bits[i] <<= 1;
      num->bits[i] |= 1;
    } else {
      if (num->bits[i] & 0x80000000) {
        in_my_mind = 1;
      }
      num->bits[i] <<= 1;
    }
  }

  status = in_my_mind;

  return status;
}

int multiply_by_8(s21_decimal *num) {
  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = multiply_by_2(num);
  }
  return status;
}

int add_bits(s21_decimal *a, s21_decimal *b, s21_decimal *result) {
  int status = 0;

  int in_my_mind = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long part_sum = (unsigned long long)a->bits[i] +
                                  (unsigned long long)b->bits[i] +
                                  (unsigned long long)in_my_mind;
    result->bits[i] = (int)(part_sum & 0xFFFFFFFF);
    in_my_mind = (int)(part_sum >> 32);
  }

  if (in_my_mind != 0) {
    status = 1;
  }

  return status;
}

void convert_decimal_to_int256(s21_decimal dec, s21_int256 *int256) {
  int256->bits[0] = dec.bits[0];
  int256->bits[1] = dec.bits[1];
  int256->bits[2] = dec.bits[2];
  int256->bits[3] = 0;
  int256->bits[4] = 0;
  int256->bits[5] = 0;
  int256->bits[6] = 0;
  int256->bits[7] = dec.bits[3];
}

int convert_int256_to_decimal(s21_int256 int256, s21_decimal *dec) {
  int status = 0;

  int scale_int256 = get_scale_int256(&int256);
  s21_int256 temp = int256;

  if (temp.bits[3] || temp.bits[4] || temp.bits[5] || temp.bits[6] ||
      scale_int256 > 28) {
    int stop_rounding = 0;
    while (scale_int256 > 0 && !stop_rounding) {
      tieshagr_bankers_rounding_int256(&temp, scale_int256 - 1);
      if (temp.bits[3] || temp.bits[4] || temp.bits[5] || temp.bits[6] ||
          scale_int256 > 29) {
        temp = int256;
        scale_int256--;
      } else {
        stop_rounding = 1;
      }
    }
    if (temp.bits[3] || temp.bits[4] || temp.bits[5] || temp.bits[6]) {
      status = 1;
    }
  }

  if (!status) {
    dec->bits[0] = temp.bits[0];
    dec->bits[1] = temp.bits[1];
    dec->bits[2] = temp.bits[2];
    dec->bits[3] = temp.bits[7];
    if (is_value_equal_zero(*dec) && !equal_zero_int256(int256)) {
      status = 2;
    }
  }
  return status;
}
