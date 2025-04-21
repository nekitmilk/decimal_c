#include "s21_int256.h"

int get_sign_int256(s21_int256 d) { return (d.bits[7] >> 31) & 1; }

void set_sign_int256(s21_int256 *d, int sign) {
  d->bits[7] &= ~((unsigned)1 << 31);
  d->bits[7] |= (unsigned)(sign & 1) << 31;
}

int get_scale_int256(const s21_int256 *d) { return (d->bits[7] >> 16) & 0xFF; }

void set_scale_int256(s21_int256 *d, int scale) {
  d->bits[7] &= ~(0xFF << 16);
  d->bits[7] |= (scale & 0xFF) << 16;
}

int align_scales_int256(s21_int256 *a, s21_int256 *b) {
  int scale_a = get_scale_int256(a);
  int scale_b = get_scale_int256(b);

  int status = 0;

  while (scale_a < scale_b && !status) {
    s21_int256 temp = *a;
    status = multiply_by_10_int256(&temp);
    if (!status) {
      *a = temp;
      scale_a++;
    }
  }

  while (scale_b < scale_a && !status) {
    s21_int256 temp = *b;
    status = multiply_by_10_int256(&temp);
    if (!status) {
      *b = temp;
      scale_b++;
    }
  }

  set_scale_int256(a, scale_a);
  set_scale_int256(b, scale_b);

  return status;
}

int multiply_by_10_int256(s21_int256 *num) {
  int status = 0;

  s21_int256 temp2 = *num;
  s21_int256 temp8 = *num;

  if (left_shift_1_int256(&temp2) != 0) {
    status = 1;
  }

  if (multiply_by_8_int256(&temp8) != 0) {
    status = 1;
  }

  if (!status) {
    status = add_bits_int256(temp2, temp8, num);
  }

  return status;
}

int left_shift_1_int256(s21_int256 *num) {
  int in_my_mind = 0;

  for (int i = 0; i < 7; i++) {
    unsigned temp = num->bits[i];
    num->bits[i] = (num->bits[i] << 1) | in_my_mind;
    in_my_mind = (temp & 0x80000000) ? 1 : 0;
  }

  return in_my_mind;
}

// Функция побитового сдвига на произвольное колличество разрядов влево
void left_shift_int256(s21_int256 *num, int shift) {
  for (int i = shift; i > 0; i--) {
    left_shift_1_int256(num);
  }
}

// Функция побитового сдвига на произвольное колличество разрядов вправо
void right_shift_int256(s21_int256 *num, int shift) {
  for (int i = shift; i > 0; i--) {
    right_shift_1_int256(num);
  }
}

int right_shift_1_int256(s21_int256 *num) {
  int in_my_mind = 0;

  for (int i = 6; i >= 0; i--) {
    unsigned temp = num->bits[i];
    num->bits[i] = (num->bits[i] >> 1) | in_my_mind;
    in_my_mind = (temp & 1) ? 0x80000000 : 0;
  }
  return in_my_mind ? 1 : 0;
}

int multiply_by_2_int256(s21_int256 *num) { return left_shift_1_int256(num); }

int multiply_by_8_int256(s21_int256 *num) {
  int status = 0;
  for (int i = 0; i < 3 && !status; i++) {
    status = multiply_by_2_int256(num);
  }
  return status;
}

int add_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result) {
  int status = 0;

  int in_my_mind = 0;

  for (int i = 0; i < 7; i++) {
    unsigned long long part_sum = (unsigned long long)a.bits[i] +
                                  (unsigned long long)b.bits[i] +
                                  (unsigned long long)in_my_mind;
    result->bits[i] = (int)(part_sum & 0xFFFFFFFF);
    in_my_mind = (int)(part_sum >> 32);
  }

  if (in_my_mind != 0) {
    status = 1;
  }

  return status;
}

int sub_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result) {
  int credit = 0;

  for (int i = 0; i < 7; i++) {
    unsigned long long value_1 = (unsigned long long)a.bits[i];
    unsigned long long value_2 = (unsigned long long)b.bits[i];
    unsigned long long diff = 0;
    if (value_1 < (value_2 + credit)) {
      diff = (0x100000000 + value_1) - value_2 - credit;
      credit = 1;
    } else {
      diff = value_1 - value_2 - credit;
      credit = 0;
    }

    result->bits[i] = (int)(diff & 0xFFFFFFFF);
  }
  return credit;
}

int mul_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *result) {
  int overflow = 0;

  s21_int256 temp = a;
  for (int i = 0; i < 7 && !overflow; i++) {
    unsigned int current_int = b.bits[i];
    for (int shift = 0; shift < 32 && !overflow; shift++) {
      if ((current_int >> shift) & 1) {  // Проверяем нужно ли прибавлять что-то
        overflow = add_bits_int256(*result, temp, result);
      }
      multiply_by_2_int256(&temp);  // Сдвигаем temp
    }
  }

  return overflow;
}

int division_bits_int256(s21_int256 a, s21_int256 b, s21_int256 *div_result,
                         s21_int256 *mod_result) {
  int overflow = 0;
  s21_int256 quotient = get_zero_int256();
  s21_int256 remainder = get_zero_int256();
  if (equal_zero_int256(a)) {
    *div_result = get_zero_int256();
    *mod_result = get_zero_int256();
  } else if (compare_bits_int256(a, b) == 0) {
    *div_result = get_zero_int256();
    *mod_result = a;
  } else {
    int left_a = get_not_zero_bit_int256(a);
    int left_b = get_not_zero_bit_int256(b);
    int shift = left_a - left_b;
    s21_int256 shifted_devisor = b;
    left_shift_int256(&shifted_devisor, shift);
    s21_int256 dividend = a;
    while (shift >= 0) {
      int overmin = sub_bits_int256(dividend, shifted_devisor, &remainder);
      if (!overmin) {
        quotient.bits[0] |= 1;
        dividend = remainder;
      } else {
        remainder = dividend;
      }
      if (shift > 0) {
        left_shift_int256(&quotient, 1);
      }
      right_shift_int256(&shifted_devisor, 1);
      shift -= 1;
    }
    *div_result = quotient;
    *mod_result = remainder;
  }
  return overflow;
}

int compare_bits_int256(s21_int256 a, s21_int256 b) {
  int result = -1;
  for (int i = 0; i < 7; i++) {
    if (a.bits[i] > b.bits[i])
      result = 1;
    else if (a.bits[i] < b.bits[i])
      result = 0;
  }

  return result;
}

int equal_zero_int256(s21_int256 value) {
  int result = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
      value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0 &&
      value.bits[6] == 0) {
    result = 1;
  }

  return result;
}

s21_int256 get_zero_int256(void) {
  s21_int256 value = {0};
  return value;
}

s21_int256 get_one_int256(void) {
  s21_int256 value = {{1, 0, 0, 0, 0, 0, 0, 0}};
  return value;
}

int get_not_zero_bit_int256(s21_int256 value) {
  int result = -1;
  for (int i = 224; i >= 0; i--) {
    if (is_set_bit_int256(value, i)) {
      result = i;
      break;
    }
  }

  return result;
}

int is_set_bit_int256(s21_int256 value, int position) {
  int result = 0;

  int structure_position = position / 32;
  int int_position = position % 32;

  if (value.bits[structure_position] & (1 << int_position)) {
    result = 1;
  }

  return result;
}

void tieshagr_bankers_rounding_int256(s21_int256 *num, int target_scale) {
  int current_scale = get_scale_int256(num);
  if (current_scale > target_scale) {
    int sign = get_sign_int256(*num);
    int scale_diff = current_scale - target_scale;
    s21_int256 divisor = get_one_int256();
    for (int i = 0; i < scale_diff; i++) {
      multiply_by_10_int256(&divisor);
    }

    s21_int256 remainder = get_zero_int256();
    s21_int256 div_result = get_zero_int256();
    division_bits_int256(*num, divisor, &div_result, &remainder);

    s21_int256 half_divisor = divisor;
    right_shift_int256(&half_divisor, 1);
    if (compare_bits_int256(remainder, half_divisor) == 1 ||
        ((compare_bits_int256(remainder, half_divisor) == -1) &&
         (div_result.bits[0] & 1))) {
      add_bits_int256(div_result, get_one_int256(), &div_result);
    }

    *num = div_result;
    set_scale_int256(num, target_scale);
    set_sign_int256(num, sign);
  }
}
