/* ----------------------------------------------------------------------
 * Project: Tiny Training Engine, MCUNetV3
 * Title:   tinyengine_function.h
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *  - Chuang Gan, ganchuang@csail.mit.edu
 *
 * Target ISA:  ARMv7E-M 	// Shiming: removed armv7e-specific insts
 * -------------------------------------------------------------------- */

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t q7_t;
typedef uint8_t q8_t;
typedef int16_t q15_t;
typedef uint16_t q16_t;
typedef int32_t q31_t;
typedef uint32_t q32_t;

typedef enum {
    STATE_SUCCESS = 0,    /* No error */
    PARAM_NO_SUPPORT = 1, /* Unsupported parameters */
} tinyengine_status;

#define TN_MAX(A, B) ((A) > (B) ? (A) : (B))
#define TN_MIN(A, B) ((A) < (B) ? (A) : (B))

// bit assignment and check
#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1ULL << (b)))
#define BIT_FLIP(a, b) ((a) ^= (1ULL << (b)))
#define BIT_CHECK(a, b) (!!((a) & (1ULL << (b))))  // '!!' to make sure this returns 0 or 1

#define BITMASK_SET(x, mask) ((x) |= (mask))
#define BITMASK_CLEAR(x, mask) ((x) &= (~(mask)))
#define BITMASK_FLIP(x, mask) ((x) ^= (mask))
#define BITMASK_CHECK_ALL(x, mask) (!(~(x) & (mask)))
#define BITMASK_CHECK_ANY(x, mask) ((x) & (mask))

tinyengine_status convolve_1x1_s8(const q7_t *input, const uint16_t input_x, const uint16_t input_y,
                                  const uint16_t input_ch, const q7_t *kernel, const int32_t *bias,
                                  const int32_t *output_shift, const int32_t *output_mult, const int32_t out_offset,
                                  const int32_t input_offset, const int32_t out_activation_min,
                                  const int32_t out_activation_max, q7_t *output, const uint16_t output_x,
                                  const uint16_t output_y, const uint16_t output_ch, q15_t *runtime_buf);

int8_t *mat_mult_kernel_s8_s16_one_column(const int8_t *input_a,
							const int16_t *input_b,
							const uint16_t output_ch,
							const int32_t *out_shift,
							const int32_t *out_mult,
							const int32_t out_offset,
							const int16_t activation_min,
							const int16_t activation_max,
							const uint16_t num_col_a,
							const int32_t *const output_bias,
							int8_t *out_0);

tinyengine_status convolve_s8_kernel3_stride1_pad1(
		const int8_t *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const int8_t *kernel, const int32_t *bias,
		const int32_t *output_shift, const int32_t *output_mult,
		const int32_t output_offset, const int32_t input_offset,
		const int32_t output_activation_min,
		const int32_t output_activation_max, int8_t *output,
		const uint16_t output_x, const uint16_t output_y,
		const uint16_t output_ch, int16_t *runtime_buf, int8_t pad_value
);

tinyengine_status convolve_s8_kernel3_stride2_padoffset1_oddch(
		const int8_t *input, const uint16_t input_x, const uint16_t input_y,
		const uint16_t input_ch, const int8_t *kernel, const int32_t *bias,
		const int32_t *output_shift, const int32_t *output_mult,
		const int32_t output_offset, const int32_t input_offset,
		const int32_t output_activation_min,
		const int32_t output_activation_max, int8_t *output,
		const uint16_t output_x, const uint16_t output_y,
		const uint16_t output_ch, int16_t *runtime_buf, int8_t pad_value
);

tinyengine_status requantize_uint8_to_int8(
		const char *input, const uint16_t size,
		const int32_t effective_scale_multiplier, 
		const int32_t effective_scale_shift,
		const int32_t input_zeropoint, const int32_t output_zeropoint,
		const int32_t min_val, const int32_t max_val,
		int8_t *output);

tinyengine_status softmax_int8(
		int8_t *input, const uint16_t size, 
		const int32_t input_multiplier, const int32_t input_left_shift,
		const int32_t diff_min, int8_t *output);

tinyengine_status reduce_mean_axis_1_2_int8(
		const int8_t *input, const uint16_t input_h, 
		const uint16_t input_w, const uint16_t input_c,
		const int32_t multiplier, const int32_t shift,
		const int32_t input_zeropoint,
		const int32_t output_zeropoint,
		const int32_t min_val, const int32_t max_val,
		int8_t *output);
