/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_nnsupportfunctions.h
 * Description:  Public header file of support functions for CMSIS NN Library
 *
 * $Date:        23. Nov 2021
 * $Revision:    V.6.0.0
 *
 * Target Processor:  Cortex-M CPUs
 * -------------------------------------------------------------------- */

#ifndef _ARM_NNSUPPORTFUNCTIONS_H_
#define _ARM_NNSUPPORTFUNCTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_MSC_VER ) 
#include <stdint.h>
#define __STATIC_FORCEINLINE static __forceinline
#define __STATIC_INLINE static __inline
#define __ALIGNED(x) __declspec(align(x))
#elif defined ( __APPLE_CC__ )
#include <stdint.h>
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
#elif defined (__GNUC_PYTHON__)
#include <stdint.h>
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
#elif defined (__GNUC__)
#include <stdint.h>
#define  __ALIGNED(x) __attribute__((aligned(x)))
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline)) 
#define __STATIC_INLINE static inline
#else
    #ifndef   __STATIC_FORCEINLINE
    #define __STATIC_FORCEINLINE                   __STATIC_INLINE
    #endif
#endif

typedef int8_t q7_t;
typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
#define NN_Q31_MAX ((q31_t)(0x7FFFFFFFL))
#define NN_Q7_MAX ((q7_t)(0x7F))
#define NN_Q31_MIN ((q31_t)(0x80000000L))
#define NN_Q7_MIN ((q7_t)(0x80))

#define LEFT_SHIFT(_shift) (_shift > 0 ? _shift : 0)
#define RIGHT_SHIFT(_shift) (_shift > 0 ? 0 : -_shift)
#define MASK_IF_ZERO(x) (x) == 0 ? ~0 : 0
#define MASK_IF_NON_ZERO(x) (x) != 0 ? ~0 : 0
#define SELECT_USING_MASK(mask, a, b) ((mask) & (a)) ^ (~(mask) & (b))

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define CLAMP(x, h, l) MAX(MIN((x), (h)), (l))
#define REDUCE_MULTIPLIER(_mult) ((_mult < 0x7FFF0000) ? ((_mult + (1 << 15)) >> 16) : 0x7FFF)

/**
 * @brief definition to pack four 8 bit values.
 */
#define PACK_Q7x4_32x1(v0, v1, v2, v3)                                                                                 \
    ((((int32_t)(v0) << 0) & (int32_t)0x000000FF) | (((int32_t)(v1) << 8) & (int32_t)0x0000FF00) |                     \
     (((int32_t)(v2) << 16) & (int32_t)0x00FF0000) | (((int32_t)(v3) << 24) & (int32_t)0xFF000000))


/**
 * @brief macro for adding rounding offset
 */
#ifndef ARM_NN_TRUNCATE
#define NN_ROUND(out_shift) ((0x1 << out_shift) >> 1)
#else
#define NN_ROUND(out_shift) 0
#endif

// Macros for shortening quantization functions' names and avoid long lines
#define MUL_SAT(a, b) arm_nn_doubling_high_mult((a), (b))
#define MUL_SAT_MVE(a, b) arm_doubling_high_mult_mve_32x4((a), (b))
#define MUL_POW2(a, b) arm_nn_mult_by_power_of_two((a), (b))

#define DIV_POW2(a, b) arm_nn_divide_by_power_of_two((a), (b))
#define DIV_POW2_MVE(a, b) arm_divide_by_power_of_two_mve((a), (b))

#define EXP_ON_NEG(x) arm_nn_exp_on_negative_values((x))
#define ONE_OVER1(x) arm_nn_one_over_one_plus_x_for_x_in_0_1((x))

struct arm_nn_double
{
    uint32_t low;
    int32_t high;
};

union arm_nn_long_long
{
    int64_t long_long;
    struct arm_nn_double word;
};

/**
  \brief   Count leading zeros
  \details Counts the number of leading zeros of a data value.
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
__STATIC_FORCEINLINE uint8_t __CLZ(uint32_t value)
{
  /* Even though __builtin_clz produces a CLZ instruction on ARM, formally
     __builtin_clz(0) is undefined behaviour, so handle this case specially.
     This guarantees ARM-compatible results if happening to compile on a non-ARM
     target, and ensures the compiler doesn't decide to activate any
     optimisations using the logic "value was passed to __builtin_clz, so it
     is non-zero".
     ARM GCC 7.3 and possibly earlier will optimise this test away, leaving a
     single CLZ instruction.
   */
  if (value == 0U)
  {
    return 32U;
  }
  return __builtin_clz(value);
}

/**
 * @brief           Saturating doubling high multiply. Result matches
 *                  NEON instruction VQRDMULH.
 * @param[in]       m1        Multiplicand. Range: {NN_Q31_MIN, NN_Q31_MAX}
 * @param[in]       m2        Multiplier. Range: {NN_Q31_MIN, NN_Q31_MAX}
 * @return          Result of multiplication.
 *
 */
__STATIC_FORCEINLINE q31_t arm_nn_doubling_high_mult(const q31_t m1, const q31_t m2)
{
    q31_t result = 0;
    // Rounding offset to add for a right shift of 31
    q63_t mult = 1 << 30;

    if ((m1 < 0) ^ (m2 < 0))
    {
        mult = 1 - mult;
    }
    // Gets resolved as a SMLAL instruction
    mult = mult + (q63_t)m1 * m2;

    // Utilize all of the upper 32 bits. This is the doubling step
    // as well.
    result = (int32_t)(mult / (1ll << 31));

    if ((m1 == m2) && (m1 == (int32_t)NN_Q31_MIN))
    {
        result = NN_Q31_MAX;
    }
    return result;
}

/**
 * @brief           Doubling high multiply without saturation. This is intended
 *                  for requantization where the scale is a positive integer
 *
 * @param[in]       m1        Multiplicand. Range: {NN_Q31_MIN, NN_Q31_MAX}
 * @param[in]       m2        Multiplier Range: {NN_Q31_MIN, NN_Q31_MAX}
 * @return          Result of multiplication.
 * @note            The result of this matches that of neon instruction
 *                  VQRDMULH for m1 in range {NN_Q31_MIN, NN_Q31_MAX} and m2 in
 *                  range {NN_Q31_MIN + 1, NN_Q31_MAX}. Saturation occurs when
 *                  m1 equals m2 equals NN_Q31_MIN and that is not handled by
 *                  this function.
 *
 */
__STATIC_FORCEINLINE q31_t arm_nn_doubling_high_mult_no_sat(const q31_t m1, const q31_t m2)
{
    q31_t result = 0;
    union arm_nn_long_long mult;

    // Rounding offset to add for a right shift of 31
    mult.word.low = 1 << 30;
    mult.word.high = 0;

    // Gets resolved as a SMLAL instruction
    mult.long_long = mult.long_long + (q63_t)m1 * m2;

    // Utilize all of the upper 32 bits. This is the doubling step
    // as well.
    result = (int32_t)(mult.long_long >> 31);

    return result;
}

/**
 * @brief           Rounding divide by power of two.
 * @param[in]       dividend - Dividend
 * @param[in]       exponent - Divisor = power(2, exponent)
 *                             Range: [0, 31]
 * @return          Rounded result of division. Midpoint is rounded away from zero.
 *
 */
__STATIC_FORCEINLINE q31_t arm_nn_divide_by_power_of_two(const q31_t dividend, const q31_t exponent)
{
    q31_t result = 0;
    const q31_t remainder_mask = (1 << exponent) - 1;
    int32_t remainder = remainder_mask & dividend;

    // Basic division
    result = dividend >> exponent;

    // Adjust 'result' for rounding (mid point away from zero)
    q31_t threshold = remainder_mask >> 1;
    if (result < 0)
    {
        threshold++;
    }
    if (remainder > threshold)
    {
        result++;
    }

    return result;
}

/**
 * @brief           Requantize a given value.
 * @param[in]       val         Value to be requantized
 * @param[in]       multiplier  multiplier. Range {NN_Q31_MIN + 1, Q32_MAX}
 * @param[in]       shift       left or right shift for 'val * multiplier'
 *
 * @return          Returns (val * multiplier)/(2 ^ shift)
 *
 */
__STATIC_FORCEINLINE q31_t arm_nn_requantize(const q31_t val, const q31_t multiplier, const q31_t shift)
{
#ifdef CMSIS_NN_USE_SINGLE_ROUNDING
    const int64_t total_shift = 31 - shift;
    const int64_t new_val = val * (int64_t)multiplier;

    int32_t result = new_val >> (total_shift - 1);
    result = (result + 1) >> 1;

    return result;
#else
    return arm_nn_divide_by_power_of_two(arm_nn_doubling_high_mult_no_sat(val * (1 << LEFT_SHIFT(shift)), multiplier),
                                         RIGHT_SHIFT(shift));
#endif
}


// @note The following functions are used only for softmax layer, scaled bits = 5 assumed

__STATIC_FORCEINLINE int32_t arm_nn_exp_on_negative_values(int32_t val)
{
    int32_t mask = 0;
    int32_t shift = 24;

    const int32_t val_mod_minus_quarter = (val & ((1 << shift) - 1)) - (1 << shift);
    const int32_t remainder = val_mod_minus_quarter - val;
    const int32_t x = (val_mod_minus_quarter << 5) + (1 << 28);
    const int32_t x2 = MUL_SAT(x, x);

    int32_t result = 1895147668 +
        MUL_SAT(1895147668, x + DIV_POW2(MUL_SAT(DIV_POW2(MUL_SAT(x2, x2), 2) + MUL_SAT(x2, x), 715827883) + x2, 1));

#define SELECT_IF_NON_ZERO(x)                                                                                          \
    {                                                                                                                  \
        mask = MASK_IF_NON_ZERO(remainder & (1 << shift++));                                                           \
        result = SELECT_USING_MASK(mask, MUL_SAT(result, x), result);                                                  \
    }

    SELECT_IF_NON_ZERO(1672461947)
    SELECT_IF_NON_ZERO(1302514674)
    SELECT_IF_NON_ZERO(790015084)
    SELECT_IF_NON_ZERO(290630308)
    SELECT_IF_NON_ZERO(39332535)
    SELECT_IF_NON_ZERO(720401)
    SELECT_IF_NON_ZERO(242)

#undef SELECT_IF_NON_ZERO

    mask = MASK_IF_ZERO(val);
    return SELECT_USING_MASK(mask, NN_Q31_MAX, result);
}

__STATIC_FORCEINLINE q31_t arm_nn_mult_by_power_of_two(const int32_t val, const int32_t exp)
{
    const int32_t thresh = ((1 << (31 - exp)) - 1);
    int32_t result = val << exp;
    result = SELECT_USING_MASK(MASK_IF_NON_ZERO(val > thresh), NN_Q31_MAX, result);
    result = SELECT_USING_MASK(MASK_IF_NON_ZERO(val < -thresh), NN_Q31_MIN, result);
    return result;
}

__STATIC_FORCEINLINE int32_t arm_nn_one_over_one_plus_x_for_x_in_0_1(int32_t val)
{
    const int64_t sum = (int64_t)val + (int64_t)NN_Q31_MAX;
    const int32_t half_denominator = (int32_t)((sum + (sum >= 0 ? 1 : -1)) / 2L);
    int32_t x = 1515870810 + MUL_SAT(half_denominator, -1010580540);

    const int32_t shift = (1 << 29);
    x += MUL_POW2(MUL_SAT(x, shift - MUL_SAT(half_denominator, x)), 2);
    x += MUL_POW2(MUL_SAT(x, shift - MUL_SAT(half_denominator, x)), 2);
    x += MUL_POW2(MUL_SAT(x, shift - MUL_SAT(half_denominator, x)), 2);

    return MUL_POW2(x, 1);
}


#ifdef __cplusplus
}
#endif

#endif
