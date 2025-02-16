#include "utils.h"

int s21_mul_by_10(s21_decimal value, s21_decimal* result) {
  int is_overflowed = 0;
  unsigned long long temp = 0;
  unsigned int carry = 0;
  s21_decimal res;

  temp = (unsigned long long)value.bits[0] * 10;
  res.bits[0] = (unsigned int)(temp & 0xFFFFFFFF);  // применяем маску, чтобы
                                                    // извлечь младшие 32 бита
  carry = (unsigned int)(temp >> 32);

  temp = (unsigned long long)value.bits[1] * 10 + carry;
  res.bits[1] = (unsigned int)(temp & 0xFFFFFFFF);
  carry = (unsigned int)(temp >> 32);

  temp = (unsigned long long)value.bits[2] * 10 + carry;
  res.bits[2] = (unsigned int)(temp & 0xFFFFFFFF);
  carry = (unsigned int)(temp >> 32);

  if (carry > 0) {
    is_overflowed += 1;
  }

  if (!is_overflowed) {
    res.bits[3] = value.bits[3];
    *result = res;
  }
  return is_overflowed;
  // если возвращаем 1, это переполнение
}

int equalize_scales(s21_decimal* value_1, s21_decimal* value_2) {
  int all_good = 1;
  int scale_1 = (value_1->bits[3] >> 16) & 0xFF;
  int scale_2 = (value_2->bits[3] >> 16) & 0xFF;

  if (scale_1 < scale_2) {
    all_good = equalize(value_1, &scale_1, scale_2);
  } else if (scale_2 < scale_1) {
    all_good = equalize(value_2, &scale_2, scale_1);
  }

  return all_good;
}

int equalize(s21_decimal* value, int* scale_to_equalize, int target_scale) {
  int all_good = 1;
  s21_decimal temp;
  while (*scale_to_equalize != target_scale && all_good) {
    all_good = s21_mul_by_10(*value, &temp);
    // если не all_good, то переполнение
  }
  if (all_good) {
    *value = temp;
  }
  return all_good;
}