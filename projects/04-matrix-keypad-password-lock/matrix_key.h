/*
 * File:   matrix_key.h
 * Brief:  4x4 matrix keypad driver interface.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#ifndef __MATRIX_KEY_H__
#define __MATRIX_KEY_H__

#include "public.h"

#define MATRIX_KEY_NONE        0U
#define MATRIX_KEY_DIGIT_MIN   1U
#define MATRIX_KEY_DIGIT_MAX   9U
#define MATRIX_KEY_ZERO        10U
#define MATRIX_KEY_CONFIRM     11U
#define MATRIX_KEY_DELETE      12U
#define MATRIX_KEY_INVALID_MIN 13U
#define MATRIX_KEY_MAX         16U

u8 MatrixKey(void);

#endif
