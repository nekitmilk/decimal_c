#include "s21_decimal.h"
#include "utils.h"
// #include "s21_print_decimal.h"

// 0 - OK; 1 - too big or infinity; 2 - too small or -infinity
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  memset(result, 0, sizeof(s21_decimal));
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);

  int scale1 = get_scale(&value_1);
  int scale2 = get_scale(&value_2);
  // Выравнивание масштабов
  int err_align = align_scales(&value_1, &value_2);

  if (err_align)
  {
    int result_scale = scale1 > scale2? get_scale(&value_2): get_scale(&value_1);
    scale1 > scale2 ? 
      tieshagr_bankers_rounding_v2(&value_1, result_scale, value_2.bits[0] << 31 | 0) :
      tieshagr_bankers_rounding_v2(&value_2, result_scale, value_1.bits[0] << 31 | 0);

    // printf("%d\n", value_2.bits[0] << 31 | 0);
    // printf("val2: %.8x %.8x %.8x %x", value_2.bits[2], value_2.bits[1], value_2.bits[0], value_2.bits[3]);    
  }
  int overflow = 0;

  // обработка, когда одно число положительное, другое отрицательное
  if (sign1 != sign2) {
    if (compare_bits(value_1, value_2)) {
      overflow = sub_bits(&value_1, &value_2, result);
      set_sign(result, sign1);
    } else {
      overflow = sub_bits(&value_2, &value_1, result);
      set_sign(result, sign2);
    }
  } else {
    overflow = add_bits(&value_1, &value_2, result);
    set_sign(result, sign1);
  }

  if (overflow) {
    if (get_sign(*result)) {
      status = 2;
    }
    else {
      status = 1;
    }
    
    // status = overflow;
    memset(result, 0, sizeof(s21_decimal));
  } else {
    // Установка общего масштаба и знака
    set_scale(result, get_scale(&value_1));
  }

  return status;
}

// Вроде работает, нужны тесты
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  s21_decimal negative_value_2 = {0};
  s21_negate(value_2, &negative_value_2);

  status = s21_add(value_1, negative_value_2, result);

  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;

  memset(result, 0, sizeof(s21_decimal));
  int sign1 = get_sign(value_1);
  int sign2 = get_sign(value_2);

  int scale_1 = get_scale(&value_1);
  int scale_2 = get_scale(&value_2);

  s21_decimal zero = {0};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal sub_result = value_2;

  if (sign2) {
    set_sign(&sub_result, 0);
  }
  set_scale(&sub_result, 0);

  while (compare_bits(sub_result, zero) != -1) {
    s21_sub(sub_result, one, &sub_result);
    status = s21_add(*result, value_1, result);
  }

  set_scale(result, scale_1 + scale_2);
  if (sign1 > sign2) {
    set_sign(result, sign1);
  } else {
    set_sign(result, sign2);
  }

  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  memset(result, 0, sizeof(s21_decimal));

  s21_decimal div_result = {0};  // хранит целую часть
  s21_decimal fract_result = {0};  // Хранит число после запятой
  s21_decimal mod_result = {0};  // хранит остаток от деления

  // s21_decimal zero = {0};
  s21_decimal ten = {{10, 0, 0, 0}};

    if (is_zero(value_2)) {
        status = 3;
    }
    else {
        _div(value_1, value_2, &div_result); // Сохранили в div_result целочисленное деление
        mod(value_1, value_2, &mod_result); // Сохраняем в mod_result остаток от деления
        
        int result_scale = 0;

        while (!is_zero(mod_result) && result_scale < 28)
        {
            s21_decimal temp_div = {0};
            result_scale += 1;
            printf("res_scale: %d\n",result_scale);

            s21_mul(mod_result, ten, &mod_result);
            _div(mod_result, value_2, &temp_div);
            mod(mod_result, value_2, &mod_result);
            s21_mul(fract_result, ten, &fract_result);
            s21_add(fract_result, temp_div, &fract_result);
        }
        
        set_scale(&fract_result, result_scale);
        s21_add(div_result, fract_result, result);
        
    }

    set_scale(&fract_result, result_scale);
    s21_add(div_result, fract_result, result);
  }

  return status;
}  // Добавить обработку слишком больших или сликом малых чисел в результате
// Добавить округление

// Функция выполняет целочисленное деление
int _div(s21_decimal value_1, s21_decimal value_2, s21_decimal *div_result) {
    int status = 0;
    s21_decimal one = {{1, 0, 0, 0}};

  while (s21_is_less_or_equal(value_2, value_1)) {
    status = s21_sub(value_1, value_2, &value_1);
    status = s21_add(*div_result, one, div_result);
  }  // целочисленное деление

  return status;
}  // работает, но следует добавить какой-то ускоритель

// Идея ускорителя в том, чтобы степенями двойки находить близкий результат
// int div_acelerator(s21_decimal value_1, s21_decimal value_2, s21_decimal
// *div_result) {
//   s21_decimal two = {2, 0, 0, 0};

//   while (s21_is_less_or_equal(value_2, value_1)) {
//     s21_mul(value_2, two, &value_2);
//   }
// }

int mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *mod_result) {
  int status = 0;

  while (s21_is_less_or_equal(value_2, value_1)) {
    status = s21_sub(value_1, value_2, &value_1);
  }  // поиск остатка от деления

  if (!status) {
    *mod_result = value_1;
  }

  return status;
}  // работает

// Проверяет является ли число нулем
// Если это ноль, то возвращает 1, иначе 0
int is_zero(s21_decimal num) {
  return (num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0);
}

// Нужны тесты (Аня)
// Используется в s21_sub
int s21_negate(s21_decimal value, s21_decimal *result) {
  int status = 0;

  memset(result, 0, sizeof(s21_decimal));

  int sign = get_sign(value);
  int scale = get_scale(&value);
  set_scale(result, scale);

  if (sign) {
    set_sign(result, 0);
  } else {
    set_sign(result, 1);
  }

  for (int i = 0; i < 3; i++) {
    result->bits[i] = value.bits[i];
  }

  return status;
}

int get_sign(s21_decimal d) {
  return (d.bits[3] >> 31) & 1;  // Бит 31: 0 (+) или 1 (-)
}

void set_sign(s21_decimal *d, int sign) {
  d->bits[3] &= ~(1 << 31);        // Обнуляем бит 31
  d->bits[3] |= (sign & 1) << 31;  // Устанавливаем новый знак
}

// Получить степень из bits[3]
int get_scale(const s21_decimal *d) {
  return (d->bits[3] >> 16) & 0xFF;  // Биты 16-23 (маска 0xFF)
}

void set_scale(s21_decimal *d, int scale) {
  d->bits[3] &= ~(0xFF << 16);         // Обнуляем биты 16-23
  d->bits[3] |= (scale & 0xFF) << 16;  // Устанавливаем scale
}

// Выравнивает запятую в обоих числах
// Могут быть проблемы с потерей точности, нужно ли с этим что-то делать?
// Очень нужно что-то с этим сделать
// Функция старается максимально выровнять масштаб
// Если у нее не получается до конца это сделать, то она останавливается и возвращает 1
// добавленный масштаб
int align_scales(s21_decimal *a, s21_decimal *b) {
  int scale_a = get_scale(a);
  int scale_b = get_scale(b);

  int status = 0;

  while (scale_a < scale_b && !status) {
    s21_decimal temp = *a;
    status = multiply_by_10(&temp);
    if (!status) {
      *a = temp;
      scale_a++;
    }
    
  }

  while (scale_b < scale_a && !status) {
    s21_decimal temp = *b;
    status = multiply_by_10(&temp);
    if (!status) {
      *b = temp;
      scale_b++;
    }
  }

  set_scale(a, scale_a);
  set_scale(b, scale_b);

  return status;
}

// В чем идея? Чтобы побитово умножить число на 10, нам нужно сначала
// сделать сдвиг на 1 бит (умножение на 2), это же число сдвинуть на 3 бит
// (умножение на 8) теперь мы можем суммировать и получить умножение на 10 num *
// 2 + num * 8 = num * (2 + 8) Оно рабатает, нужен рефакторинг status 0 - OK: 1
// - NOT OK
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

  // Добавить установку масштаба - 1?

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

// Трижды сдвигает по биту, что эвивалентно умножению на 8
// Вроде работает
int multiply_by_8(s21_decimal *num) {
  int status = 0;
  for (int i = 0; i < 3; i++) {
    status = multiply_by_2(num);
  }
  return status;
}

// Складывает целые части decimal
// Вроде работает
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

// Вычитает целые части decimal
// Вроде работает, но нужны прям тесты
int sub_bits(s21_decimal *a, s21_decimal *b, s21_decimal *result) {
  int credit = 0;

  for (int i = 0; i < 3; i++) {
    unsigned long long value_1 = (unsigned long long)a->bits[i];
    unsigned long long value_2 = (unsigned long long)b->bits[i];
    unsigned long long diff = 0;
    if (value_1 < (value_2 + credit)) {
      diff = (0x100000000 + value_1) - value_2 - credit;
      credit = 1;
    } else if (value_1 >= (value_2 + credit)) {
      diff = value_1 - value_2 - credit;
      credit = 0;
    }

    result->bits[i] = (int)(diff & 0xFFFFFFFF);
  }
  return credit;
}

// Сравнение абсолютных значенией
// 1 - a > b; 0 - a < b; -1 - a = b
// Вроде работает
int compare_bits(s21_decimal a, s21_decimal b) {
  int result = -1;
  for (int i = 0; i < 3; i++) {
    if (a.bits[i] > b.bits[i])
      result = 1;
    else if (a.bits[i] < b.bits[i])
      result = 0;
    // printf("%d\n", result);
  }

  return result;
}