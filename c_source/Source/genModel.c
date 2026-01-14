/* Automatically generated source file */
#include <float.h>
#include <tinyengine_function.h>
#include <sys/time.h>

#include "genModel.h"
#include "genInclude.h"
#define PRINT_LAYER 1

void start_time();
void print_time();
struct timeval stop, start;

void start_time()    {
    gettimeofday(&start, NULL);
}

void print_time()   {
    gettimeofday(&stop, NULL);
    printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
}

signed char* getInput() {
    return &buffer0[0];
}
signed char* getOutput() {
    return NNoutput;
}

void invoke_inf(){
    /* layer 0:QUANTIZE */
#ifdef PRINT_LAYER
    start_time();
    printf("=== OP 0, QUANTIZE ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    requantize_uint8_to_int8(&buffer0[0],784, 1073741824,1,127,-1,-128,127,&buffer0[784]);
    /* layer 1:CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 1, CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    convolve_s8_kernel3_stride2_padoffset1_oddch(&buffer0[784],28,28,1,(const q7_t*) weight0,bias0,shift0,multiplier0,-128,1,-128,127,&buffer0[6272],14,14,16,sbuf,-1);
    /* layer 2:DEPTHWISE_CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 2, DEPTHWISE_CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    depthwise_kernel3x3_stride1_pad1_padoffset0_inplace_CHW(&buffer0[6272],14,14,16,(const q7_t*) CHWweight1,offsetBias1,offsetRBias1,shift1,multiplier1,-128,128,-128,127,&buffer0[6272],14,14,16,sbuf,-128);
    /* layer 3:CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 3, CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    convolve_s8_kernel3_stride1_pad1(&buffer0[6272],14,14,16,(const q7_t*) weight2,bias2,shift2,multiplier2,-128,128,-128,127,&buffer0[0],14,14,32,sbuf,-128);
    /* layer 4:DEPTHWISE_CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 4, DEPTHWISE_CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    depthwise_kernel3x3_stride2_pad0_padoffset1_inplace_CHW(&buffer0[0],14,14,32,(const q7_t*) CHWweight3,offsetBias3,offsetRBias3,shift3,multiplier3,-128,128,-128,127,&buffer0[0],7,7,32,sbuf,-128);
    /* layer 5:CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 5, CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    convolve_1x1_s8(&buffer0[0],7,7,32,(const q7_t*) weight4,bias4,shift4,multiplier4,-128,128,-128,127,&buffer0[1568],7,7,64,sbuf);
    /* layer 6:REDUCE */
#ifdef PRINT_LAYER
    printf("=== OP 6, REDUCE ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    reduce_mean_axis_1_2_int8(&buffer0[1568],7,7,64, 1131728282,-1,-128, -128, -128,127, &buffer0[0]);
    /* layer 7:CONV_2D */
#ifdef PRINT_LAYER
    printf("=== OP 7, CONV_2D ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    convolve_1x1_s8(&buffer0[0],1,1,64,(const q7_t*) weight5,bias5,shift5,multiplier5,100,128,-128,127,&buffer0[64],1,1,36,sbuf);
    /* layer 8:SOFTMAX */
#ifdef PRINT_LAYER
    printf("=== OP 8, SOFTMAX ===\r\n"); fflush(stdout); 
    print_time();
    start_time();
#endif
    softmax_int8(&buffer0[64],36, 1398313856,26,-31, &buffer0[0]);
    print_time();
}
