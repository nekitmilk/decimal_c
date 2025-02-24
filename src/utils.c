#include "utils.h"

void equalize_scales(s21_decimal* value_1, s21_decimal* value_2) {
  int scale_1 = get_scale(value_1);
  int scale_2 = get_scale(value_2);

  if (scale_1 < scale_2) {
    equalize(value_1, value_2, scale_1, scale_2);
  } else if (scale_2 < scale_1) {
    equalize(value_2, value_1, scale_2, scale_1);
  }
}

void equalize(s21_decimal* value_with_smaller_scale,
              s21_decimal* value_with_larger_scale, int smaller_scale,
              int larger_scale) {
  while ((smaller_scale != larger_scale) &&
         !is_next_mul_will_cause_an_overflow(*value_with_smaller_scale)) {
    mul_by_10(value_with_smaller_scale);
    ++(smaller_scale);
  }

  while (smaller_scale != larger_scale) {
    div_by_10(value_with_larger_scale);
    --larger_scale;
  }
}

int is_next_mul_will_cause_an_overflow(s21_decimal value) {
  return value.bits[2] > 0x19999999;
}

void mul_by_10(s21_decimal* value) {
  unsigned long long temp = 0;
  unsigned int carry = 0;
  s21_decimal res;

  temp = (unsigned long long)value->bits[0] * 10;
  res.bits[0] = (unsigned int)(temp & 0xFFFFFFFF);  // применяем маску, чтобы
                                                    // извлечь младшие 32 бита
  carry = (unsigned int)(temp >> 32);

  temp = (unsigned long long)value->bits[1] * 10 + carry;
  res.bits[1] = (unsigned int)(temp & 0xFFFFFFFF);
  carry = (unsigned int)(temp >> 32);

  temp = (unsigned long long)value->bits[2] * 10 + carry;
  res.bits[2] = (unsigned int)(temp & 0xFFFFFFFF);
  carry = (unsigned int)(temp >> 32);

  res.bits[3] = value->bits[3];

  *value = res;
  set_scale(value, get_scale(value) + 1);
}

void div_by_10(s21_decimal* value) {
  s21_decimal temp_result = {{0, 0, 0, 0}};
  unsigned int remainder = 0;

  for (int i = 2; i >= 0; --i) {
    unsigned long long current =
        ((unsigned long long)remainder << 32) | value->bits[i];
    temp_result.bits[i] = (unsigned int)current / 10;
    remainder = (unsigned int)current % 10;
  }
  bankers_rounding(&temp_result, remainder);
  int scale = get_scale(value);
  if (scale > 0) --scale;
  temp_result.bits[3] = (value->bits[3] & SIGN_MASK) | scale << 16;
  *value = temp_result;
}

void bankers_rounding(s21_decimal* value, unsigned int remainder) {
  if (remainder > 5) {
    round_up(value);

  } else if (remainder == 5) {
    if (value->bits[0] % 2 != 0) {
      round_up(value);
    }
  }
}

void round_up(s21_decimal* value) {
  for (int i = 0, need_to_continue = 1; i < 3 && need_to_continue; ++i) {
    ++value->bits[i];
    if (value->bits[i] != 0) {
      need_to_continue = 0;
    }
  }
}
