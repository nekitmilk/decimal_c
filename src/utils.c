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
    ++smaller_scale;
  }

  while (smaller_scale != larger_scale) {
    div_by_10(value_with_larger_scale, bankers_rounding);
    --larger_scale;
  }
}

int mul_by_10(s21_decimal* value) {
  int is_value_overflowed = 0;
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

  if (carry != 0) {
    is_value_overflowed = 1;
    printf("OVERFLOW!\n");
  }

  res.bits[3] = value->bits[3];

  *value = res;
  set_scale(value, get_scale(value) + 1);
  return is_value_overflowed;
}

int is_next_mul_will_cause_an_overflow(s21_decimal value) {
  return value.bits[2] >= 0x19999999;

  // return mul_by_10(&value);
}

void div_by_10(s21_decimal* value,
               void (*rounding_function)(s21_decimal*, unsigned int)) {
  s21_decimal temp_result = {{0, 0, 0, 0}};
  unsigned int remainder = 0;

  for (int i = 2; i >= 0; --i) {
    unsigned long long current =
        ((unsigned long long)remainder << 32) | value->bits[i];
    temp_result.bits[i] = (unsigned int)(current / 10);
    remainder = (unsigned int)(current % 10);
  }
  // bankers_rounding(&temp_result, remainder);
  int scale = get_scale(value);
  if (scale > 0) --scale;
  temp_result.bits[3] = (value->bits[3] & SIGN_MASK) | scale << 16;
  rounding_function(&temp_result, remainder);
  *value = temp_result;
}

void bankers_rounding(s21_decimal* value, unsigned int remainder) {
  if (get_scale(value) == 0) {
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

void round_up(s21_decimal* value) {
  for (int i = 0, need_to_continue = 1; i < 3 && need_to_continue; ++i) {
    ++value->bits[i];
    if (value->bits[i] != 0) {
      need_to_continue = 0;
    }
  }
}

// у нечетных чисел последний бит 0
// Банковское округление до определенного знака
void tieshagr_bankers_rounding(s21_decimal *num, int target_scale) {
    int current_scale = get_scale(num);
    if (current_scale <= target_scale) return; // Округление не требуется

    // Вычисляем разницу в масштабах
    int scale_diff = current_scale - target_scale;

    // Вычисляем 10^scale_diff
    unsigned int divisor = 1;
    for (int i = 0; i < scale_diff; i++) {
        divisor *= 10;
    }

    // Получаем мантиссу
    unsigned int mantissa[3] = {num->bits[0], num->bits[1], num->bits[2]};

    // Вычисляем остаток
    unsigned int remainder = 0;
    for (int i = 2; i >= 0; i--) {
        unsigned long temp = ((unsigned long)remainder << 32) | mantissa[i];
        mantissa[i] = (unsigned int)(temp / divisor);
        remainder = (unsigned int)(temp % divisor);
    }

    // Проверяем, нужно ли округлять
    unsigned int half_divisor = divisor / 2;
    if (remainder > half_divisor || (remainder == half_divisor && (mantissa[0] & 1))) {
        // Округляем вверх
        unsigned int carry = 1;
        for (int i = 0; i < 3 && carry; i++) {
            unsigned long sum = (unsigned long)mantissa[i] + carry;
            mantissa[i] = (unsigned int)(sum & 0xFFFFFFFF);
            carry = (unsigned int)(sum >> 32);
        }
    }

    // Обновляем мантиссу и масштаб
    num->bits[0] = mantissa[0];
    num->bits[1] = mantissa[1];
    num->bits[2] = mantissa[2];
    set_scale(num, target_scale);
}

void tieshagr_bankers_rounding_v2(s21_decimal *num, int target_scale, int nechet) {
    int current_scale = get_scale(num);
    if (current_scale <= target_scale) return; // Округление не требуется

    // Вычисляем разницу в масштабах
    int scale_diff = current_scale - target_scale;

    // Вычисляем 10^scale_diff
    unsigned int divisor = 1;
    for (int i = 0; i < scale_diff; i++) {
        divisor *= 10;
    }

    // Получаем мантиссу
    unsigned int mantissa[3] = {num->bits[0], num->bits[1], num->bits[2]};

    // Вычисляем остаток
    unsigned int remainder = 0;
    for (int i = 2; i >= 0; i--) {
        unsigned long temp = ((unsigned long)remainder << 32) | mantissa[i];
        mantissa[i] = (unsigned int)(temp / divisor);
        remainder = (unsigned int)(temp % divisor);
    }

    // Проверяем, нужно ли округлять
    unsigned int half_divisor = divisor / 2;
    // if (remainder > half_divisor || (remainder == half_divisor && (mantissa[0] & 1)) || nechet) {
    if (remainder > half_divisor || ((remainder == half_divisor) && nechet)) {
        // Округляем вверх
        unsigned int carry = 1;
        for (int i = 0; i < 3 && carry; i++) {
            unsigned long sum = (unsigned long)mantissa[i] + carry;
            mantissa[i] = (unsigned int)(sum & 0xFFFFFFFF);
            carry = (unsigned int)(sum >> 32);
        }
    }
    

    // Обновляем мантиссу и масштаб
    num->bits[0] = mantissa[0];
    num->bits[1] = mantissa[1];
    num->bits[2] = mantissa[2];
    set_scale(num, target_scale);
}

int is_value_equal_zero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;


}