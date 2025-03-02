#include "s21_decimal.h"
#include "utils.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  if (result == NULL) return 1;

  while (get_scale(&value) != 0 && !is_value_equal_zero(value)) {
    div_by_10(&value, floor_rounding);
  }
  *result = value;
  return 0;
}
// код ошибки : 0 OK, 1 ошибка вычисления

void floor_rounding(s21_decimal* value, unsigned int remainder) {
  if (get_scale(value) == 0 || is_value_equal_zero(*value)) {
    if (is_negative(*value) && remainder > 0) {
      round_up(value);
    }
  }
}