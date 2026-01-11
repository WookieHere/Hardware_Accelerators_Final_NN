#include <stdio.h>

#include "arm_nnfunctions.h"
#include "tinyengine_function.h"

tinyengine_status softmax_int8(
		int8_t *input, const uint16_t size, 
		const int32_t input_multiplier, const int32_t input_left_shift,
		const int32_t diff_min, int8_t *output) {
	arm_softmax_s8(input, 1, size, 
		input_multiplier, input_left_shift, diff_min, output);
}
