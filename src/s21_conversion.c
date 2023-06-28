#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = OK;
  if (abs(src) > s21_MAXDEC) {
    status = CONVERR;
  }
  s21_clear_decimal(dst);
  if (src < 0) {
    src = -1 * src;
    s21_set_sign(dst);
  }
  dst->bits[0] = src;
  return status;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = OK;
  if (fabs(src) < 2e-28) {
    s21_clear_decimal(dst);
    status = CONVERR;
  }
  if (fabs(src) > s21_MAXDEC) {
    status = CONVERR;
  }
  if (dst) {
    s21_clear_decimal(dst);
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src = -(src);
    }
    long int new = (long int)src;
    long int exp = 0;
    while (src - ((float)new / (long int)(pow(10, exp))) != 0) {
      exp++;
      new = src *(long int)(pow(10, exp));
    }
    s21_from_int_to_decimal(new, dst);
    if (sign) {
      s21_set_sign(dst);
    }
    dst->bits[3] += exp << 16;
  } else {
    status = CONVERR;
  }
  return status;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int status = OK;
  if (dst) {
    if (s21_get_sign(src) == 1) {
      *dst = -1 * (int)(src.bits[0] / pow(10, s21_get_exp(src)));
    } else {
      *dst = (int)(src.bits[0] / pow(10, s21_get_exp(src)));
    }
  } else {
    status = CONVERR;
  }
  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int status = OK;
  *dst = 0;
  if (dst == NULL) {
    status = 1;
  } else {
    int minus = 1;
    long double result = 0, two = 1;
    int exp = s21_get_exp(src);
    if (s21_get_sign(src) == 1) minus = -1;
    for (int k = 0; k < 96; k++) {
      if (s21_getBit(src, k)) {
        result += two;
      }
      two *= 2;
    }
    for (int k = 0; k < exp; k++) {
      result /= 10;
    }
    *dst = minus * result;
  }
  return status;
}