/*
 * Copyright (C) 2010-2022 Arm Limited or its affiliates.
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
 * Title:        arm_nnfunctions.h
 * Description:  Public header file for CMSIS NN Library
 *
 * $Date:        20 January 2022
 * $Revision:    V.7.4.0
 *
 * Target Processor:  Cortex-M CPUs
 * -------------------------------------------------------------------- */

/**
   \mainpage CMSIS NN Software Library
   *
   * Introduction
   * ------------
   *
   * This user manual describes the CMSIS NN software library,
   * a collection of efficient neural network kernels developed to maximize the
   * performance and minimize the memory footprint of neural networks on Cortex-M processor cores.
   *
   * The library is divided into a number of functions each covering a specific category:
   * - Convolution Functions
   * - Activation Functions
   * - Fully-connected Layer Functions
   * - SVDF Layer Functions
   * - Pooling Functions
   * - Softmax Functions
   * - Basic math Functions
   *
   * The library has separate functions for operating on different weight and activation data
   * types including 8-bit integers (q7_t) and 16-bit integers (q15_t). The descrition of the
   * kernels are included in the function description. The implementation details are also
   * described in this paper [1].
   *
   * Function Classification
   * --------
   * The functions can be classified into two segments
   * - Legacy functions supporting ARM's internal symmetric quantization(8 bits).
   * - Functions that support TensorFlow Lite framework with symmetric quantization(8 bits).
   *
   * The legacy functions can be identified with their suffix of _q7 or _q15 and are no new development is done there.
   * The article in [2] describes in detail how to run a network using the legacy functions.
   *
   * The functions supporting TensorFlow Lite framework is identified by the _s8 suffix and can be invoked from TFL
   * micro. The functions are bit exact to TensorFlow Lite. Refer to the TensorFlow's documentation in [3] on how to run
   * a TensorFlow Lite model using optimized CMSIS-NN kernels.
   *
   * Block Diagram
   * --------
   * \image html CMSIS-NN-OVERVIEW.PNG
   *
   * Examples
   * --------
   *
   * The library ships with a number of examples which demonstrate how to use the library functions.
   *
   * Pre-processor Macros
   * ------------
   *
   * Each library project have different pre-processor macros.
   *
   * - ARM_MATH_DSP:
   *
   * Define macro ARM_MATH_DSP, If the silicon supports DSP instructions(DSP extension).
   *
   * - ARM_MATH_MVEI:
   *
   * Define macro ARM_MATH_MVEI, If the silicon supports M-Profile Vector Extension.

   * - ARM_MATH_AUTOVECTORIZE
   *  Used in conjucture with ARM_MATH_MVEI to let the compiler auto vectorize for the functions that uses inline
   *  assembly. It does not affect functions that use C or intrinsics.
   * - ARM_MATH_BIG_ENDIAN:
   *
   * Define macro ARM_MATH_BIG_ENDIAN to build the library for big endian targets. This is supported only for the legacy
   * functions i.e, functions targetted at TensorFlow Lite do not support big endianness. By default library builds for
   * little endian targets.
   *
   * - ARM_NN_TRUNCATE:
   *
   * Define macro ARM_NN_TRUNCATE to use floor instead of round-to-the-nearest-int for the computation.
   *
   *
   * Copyright Notice
   * ------------
   *
   * Copyright (C) 2010-2019 Arm Limited. All rights reserved.
   *
   * [1] CMSIS-NN: Efficient Neural Network Kernels for Arm Cortex-M CPUs https://arxiv.org/abs/1801.06601
   *
   * [2] Converting a Neural Network for Arm Cortex-M with CMSIS-NN
   *
   https://developer.arm.com/solutions/machine-learning-on-arm/developer-material/how-to-guides/converting-a-neural-network-for-arm-cortex-m-with-cmsis-nn/single-page
   * [3] https://www.tensorflow.org/lite/microcontrollers/library
   *
   * [4] https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/NN#legacy-vs-tfl-micro-compliant-apis
   */

/**
 * @defgroup groupNN Neural Network Functions
 * A collection of functions to perform basic operations for neural network layers. Functions with a _s8 suffix support
 * TensorFlow Lite framework.
 */

#ifndef _ARM_NNFUNCTIONS_H
#define _ARM_NNFUNCTIONS_H

#include <stdint.h>

#define USE_INTRINSIC

//#define ARM_NN_TRUNCATE /* This config the rounding model to floor or round to the nearest int */

#ifdef __cplusplus
extern "C" {
#endif

void arm_softmax_s8(const int8_t *input,
                    const int32_t num_rows,
                    const int32_t row_size,
                    const int32_t mult,
                    const int32_t shift,
                    const int32_t diff_min,
                    int8_t *output);

/**
 * @brief U8 softmax function
 * @param[in]  input     Pointer to the input tensor
 * @param[in]  num_rows  Number of rows in the input tensor
 * @param[in]  row_size  Number of elements in each input row
 * @param[in]  mult      Input quantization multiplier
 * @param[in]  shift     Input quantization shift within the range [0, 31]
 * @param[in]  diff_min  Minimum difference with max in row. Used to check if
 *                       the quantized exponential operation can be performed
 * @param[out] output    Pointer to the output tensor
 *
 * @note Supported framework: TensorFlow Lite micro (bit-accurate)
 *
 */

#ifdef __cplusplus
}
#endif

#endif
