// Copyright 2025 Michael Reilly <mreilly@mreilly.dev>
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the names of the copyright holders nor the names of the
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
#ifndef _CXSRANDLIB_H_
#define _CXSRANDLIB_H_

#include <stdint.h>

/**
 * @file cxsrandlib.h
 *
 * @brief This library implements a seeded xorshift pseudorandom number generator.
 */

/**
 * @brief Return a seeded pseudorandom unsigned 32 bit integer.
 *
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
uint32_t cxsrandlib_u32(uint32_t *state);

/**
 * @brief Return a seeded pseudorandom signed 32 bit integer in the range
 * [lo, bound).
 * 
 * Results returned are uniform except with regard to modulo bias. If bound is
 * less than or equal to lo, lo is returned and state is unmodified.
 *
 * @param[in] lo Inclusive lower bound
 * @param[in] bound Exclusive upper bound
 * @param[in] xorbits Transform result before bounds adjustment is applied
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
int32_t cxsrandlib_i32_range(int32_t lo, int32_t bound, uint32_t xorbits,
                             uint32_t *state);

/**
 * @brief Return a seeded pseudorandom unsigned 32 bit integer in the range
 * [lo, bound).
 * 
 * Results returned are uniform except with regard to modulo bias. If bound is
 * less than or equal to lo, lo is returned and state is unmodified.
 *
 * @param[in] lo Inclusive lower bound
 * @param[in] bound Exclusive upper bound
 * @param[in] xorbits Transform result before bounds adjustment is applied
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
uint32_t cxsrandlib_u32_range(uint32_t lo, uint32_t bound, uint32_t xorbits,
                              uint32_t *state);

/**
 * @brief Return a seeded pseudorandom double in the range [0, 1].
 * 
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
double cxsrandlib_double(uint32_t *state);

/**
 * @brief Output two seeded pseudorandom doubles according to the normal
 * distribution specified by mean and stddev using the Marsaglia polar method.
 *
 * This function has a cycle limited fallback to prevent theoretical infinite
 * recursion.
 *
 * @param[in] mean Mean of the normal distribution
 * @param[in] stddev Standard deviation of the normal distribution
 * @param[out] out Array in which to output two doubles
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
void cxsrandlib_normal2(double mean, double stddev, double out[2],
                        uint32_t *state);

/**
 * @brief Return a seeded pseudorandom double according to the normal distribution specified by mean and stddev using the Marsaglia polar method.
 *
 * This function operates by calling cxsrandlib_normal2 and discarding one of
 * the outputs.
 *
 * @param[in] mean Mean of the normal distribution
 * @param[in] stddev Standard deviation of the normal distribution
 * @param[in,out] state Seeded state (must be initialized non-zero)
 */
double cxsrandlib_normal(double mean, double stddev, uint32_t *state);

#endif
