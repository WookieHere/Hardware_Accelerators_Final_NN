#include <stdint.h>
#include <stdio.h>
#include <string.h>  // For memcpy

#include "emnist_byclass_testset_0_999/input_9.h"
#include "Source/genModel.c" // Just include the whole c file...

#define NN_OUTPUT_TYPE int8_t
#define NN_OUTPUT_TYPE_MIN -128
#define NN_OUTPUT_SIZE 36
// INPUT_NUM will be printed. If you change input_x.h, remember to update this
#define INPUT_NUM 9
#define INPUT_SIZE_BYTE 784
#define MODEL_NAME "st_mnist_v1_28_tfs_int8_emnist_byclass"

int main(int argc, char* argv[]) {
    printf("Starting %s, input %d\n", MODEL_NAME, INPUT_NUM);
    fflush(stdout);

    // Obtain a pointer to the model's input tensor
    char* input = getInput();

    char* output = getOutput();

    memcpy(input, input_data, INPUT_SIZE_BYTE);

    //printf("Starting\n");
    //fflush(stdout);
    invoke_inf();

    NN_OUTPUT_TYPE max_out = NN_OUTPUT_TYPE_MIN;
    uint8_t max_idx = 0;
    for (int i = 0; i < NN_OUTPUT_SIZE; ++i) {
        if (((NN_OUTPUT_TYPE *)(output))[i] > max_out) {
            max_out = ((NN_OUTPUT_TYPE *)(output))[i];
            max_idx = i;
        }
        printf("Output %d %f\n", ((NN_OUTPUT_TYPE *)(output))[i], ((NN_OUTPUT_TYPE *)(output))[i]);
        fflush(stdout);
    }
    printf("Label: %d, Max: %d\n", label, max_idx);
    fflush(stdout);
}
