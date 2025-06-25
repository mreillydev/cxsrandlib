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

#include <stdio.h>    
#include <math.h>

#include "cxsrandlib.h"

// Unsigned 32 bit integer xorshift PRNG
// If NULL arg, print error message and segfault
uint32_t cxsrandlib_u32(uint32_t *state)
{
    if(state == NULL)
        fprintf(stderr, "randib: NULL state passed to cxsrandlib_u32()\n");
    *state ^= ((*state) << 13);
    *state ^= ((*state) >> 17);
    *state ^= ((*state) <<  5);
    return *state;
}

// Signed 32 bit integer within [lo, bound)
int32_t cxsrandlib_i32_range(int32_t lo, int32_t bound, uint32_t xorbits,
                             uint32_t *state)
{
    if(lo >= bound-1 || bound == INT32_MIN)
        return lo; // avoid spurious randomness invocation
    int32_t result = (int32_t)(cxsrandlib_u32(state) ^ xorbits);
    if(result < 0)
        result = -result;   
    if(result < 0)
        result = 0;
    return lo + result % (bound - lo);
}

// Unsigned 32 bit integer within [lo, bound)
uint32_t cxsrandlib_u32_range(uint32_t lo, uint32_t bound, uint32_t xorbits,
                              uint32_t *state)
{
    if(lo >= bound-1 || bound == 0)
        return lo; // avoid spurious randomness invocation
    return lo + (cxsrandlib_u32(state) ^ xorbits) % (bound - lo);
}

// Call cxsrandlib_u32 and cast to a double within [0, 1].
double cxsrandlib_double(uint32_t *state)
{   
    return ((double)cxsrandlib_u32(state)) / UINT32_MAX;
}

// Implement the Marsaglia polar method, with a cycle limiting fallback to
// prevent theoretical infinite recursion.
void cxsrandlib_normal2(double mean, double stddev, double out[2],
                        uint32_t *state)
{
    double a = 0.0, b = 0.0, s = 0.0;
    for(int cycle = 0; cycle < 16; cycle++)
    {
        a = -1 + 2 * cxsrandlib_double(state);
        b = -1 + 2 * cxsrandlib_double(state);
        s = a * a + b * b;
        if(s > 0.00001 && s < 1.0)
        {
            out[0] = mean + stddev * a * sqrt((-2.0 * log(s)) / s);
            out[1] = mean + stddev * b * sqrt((-2.0 * log(s)) / s);
            return;
        }
    }
    out[0] = mean + stddev * a;
    out[1] = mean + stddev * b;
}

// Call cxsrandlib_normal2 and pseudorandomly pick a result to return.
double cxsrandlib_normal(double mean, double stddev, uint32_t *state)
{
    double data[2] = { 0.0, 0.0 };
    cxsrandlib_normal2(mean, stddev, data, state);
    return data[cxsrandlib_u32(state)%2];
}
