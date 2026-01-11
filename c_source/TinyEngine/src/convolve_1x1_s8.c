#include <stdio.h>

#include "arm_nnfunctions.h"
#include "arm_nnsupportfunctions.h"
#include "tinyengine_function.h"

#define DIM_KER_X (1U)
#define DIM_KER_Y (1U)

tinyengine_status convolve_1x1_s8(const int8_t *input, const uint16_t input_x,
		const uint16_t input_y, const uint16_t input_ch, const int8_t *kernel,
		const int32_t *bias, const int32_t *output_shift,
		const int32_t *output_mult, const int32_t out_offset,
		const int32_t input_offset, const int32_t out_activation_min,
		const int32_t out_activation_max, int8_t *output, const uint16_t output_x,
		const uint16_t output_y, const uint16_t output_ch, int16_t *runtime_buf) {


	(void) input_x;
	(void) input_y;

	int8_t *out = output;
	const int32_t num_elements = output_x * output_y;

	for (int32_t i_element = 0; i_element < num_elements; i_element++) {
		int8_t *src = &input[i_element * input_ch];
		int16_t *dst = runtime_buf;

		for (int32_t i_ch_in = 0; i_ch_in < input_ch; i_ch_in++) {
			dst[i_ch_in] = (int16_t)(src[i_ch_in] + input_offset);
		}
		out = mat_mult_kernel_s8_s16_one_column(kernel,
				runtime_buf, output_ch, output_shift, output_mult,
				(int8_t) out_offset, out_activation_min,
				out_activation_max, input_ch * DIM_KER_Y * DIM_KER_X,
				bias, out);
	}

	/* Return to application */
	return STATE_SUCCESS;
}
