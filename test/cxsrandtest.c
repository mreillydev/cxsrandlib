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
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "cxsrandlib.h"

#define Epsilon 0.000001

int main(int argc, char **argv)
{
    uint32_t state = (uint32_t)time(NULL);

    if(argc < 2)
    {
        // i32 bounds tests
        assert(cxsrandlib_i32_range(-1, -2, 0, &state) == -1);
        assert(cxsrandlib_i32_range(-1, -1, 0, &state) == -1);
        assert(cxsrandlib_i32_range(-1, 0, 0, &state) == -1);
        assert(cxsrandlib_i32_range(0, -1, 0, &state) == 0);
        assert(cxsrandlib_i32_range(0, 0, 0, &state) == 0);
        assert(cxsrandlib_i32_range(0, 1, 0, &state) == 0);
        assert(cxsrandlib_i32_range(4, 3, 0, &state) == 4);
        assert(cxsrandlib_i32_range(4, 4, 0, &state) == 4);
        assert(cxsrandlib_i32_range(4, 5, 0, &state) == 4);
        assert((cxsrandlib_i32_range(0, 8, 0, &state)&~7) == 0);
        assert((cxsrandlib_i32_range(0, 256, 0, &state)&~255) == 0);
        assert(cxsrandlib_i32_range(INT32_MAX, INT32_MAX, 0, &state) == INT32_MAX);
        assert(cxsrandlib_i32_range(INT32_MAX, INT32_MIN, 0, &state) == INT32_MAX);
        assert(cxsrandlib_i32_range(INT32_MIN, INT32_MIN, 0, &state) == INT32_MIN);
        assert(cxsrandlib_i32_range(200, -5, 0, &state) == 200);
        assert(cxsrandlib_i32_range(-5, -7, 0, &state) == -5);

        // u32 bounds tests
        assert(cxsrandlib_u32_range(0, 0, 0, &state) == 0);
        assert(cxsrandlib_u32_range(0, 1, 0, &state) == 0);
        assert(cxsrandlib_u32_range(2, 0, 0, &state) == 2);
        assert(cxsrandlib_u32_range(2, 1, 0, &state) == 2);
        assert(cxsrandlib_u32_range(2, 2, 0, &state) == 2);
        assert(cxsrandlib_u32_range(2, 3, 0, &state) == 2);
        assert(cxsrandlib_u32_range(UINT32_MAX, 0, 0, &state) == UINT32_MAX);
        assert(cxsrandlib_u32_range(UINT32_MAX, UINT32_MAX, 0, &state) == UINT32_MAX);

        // i32 bounds tests (with xorbits)
        assert(cxsrandlib_i32_range(-1, -2, 98, &state) == -1);
        assert(cxsrandlib_i32_range(-1, -1, 12435, &state) == -1);
        assert(cxsrandlib_i32_range(-1, 0, 231, &state) == -1);
        assert(cxsrandlib_i32_range(0, -1, 0x18291fff, &state) == 0);
        assert(cxsrandlib_i32_range(0, 0, 37, &state) == 0);
        assert(cxsrandlib_i32_range(0, 1, 52, &state) == 0);
        assert(cxsrandlib_i32_range(4, 3, 89, &state) == 4);
        assert(cxsrandlib_i32_range(4, 4, 110, &state) == 4);
        assert(cxsrandlib_i32_range(4, 5, 2354, &state) == 4);
        assert((cxsrandlib_i32_range(0, 8, 0x7fffffff, &state)&~7) == 0);
        assert((cxsrandlib_i32_range(0, 256, 789, &state)&~255) == 0);
        assert(cxsrandlib_i32_range(INT32_MAX, INT32_MAX, 1, &state) == INT32_MAX);
        assert(cxsrandlib_i32_range(INT32_MAX, INT32_MIN, 22, &state) == INT32_MAX);
        assert(cxsrandlib_i32_range(INT32_MIN, INT32_MIN, 333, &state) == INT32_MIN);
        assert(cxsrandlib_i32_range(200, -5, 4444, &state) == 200);
        assert(cxsrandlib_i32_range(-5, -7, 5555, &state) == -5);

        // u32 bounds tests (with xorbits)
        assert(cxsrandlib_u32_range(0, 0, 3142199, &state) == 0);
        assert(cxsrandlib_u32_range(0, 1, 0xc578f, &state) == 0);
        assert(cxsrandlib_u32_range(2, 0, 235, &state) == 2);
        assert(cxsrandlib_u32_range(2, 1, 124, &state) == 2);
        assert(cxsrandlib_u32_range(2, 2, 0xffffffff, &state) == 2);
        assert(cxsrandlib_u32_range(2, 3, 1, &state) == 2);
        assert(cxsrandlib_u32_range(UINT32_MAX, 0, 88, &state) == UINT32_MAX);
        assert(cxsrandlib_u32_range(UINT32_MAX, UINT32_MAX, 7, &state) == UINT32_MAX);

        // double bounds testing
        for(int ix = 0; ix < 100; ix++)
        {
            double d = cxsrandlib_double(&state);
            assert(d >= 0.0 - Epsilon);
            assert(d <= 1.0 + Epsilon);
        }

        // Open up OS randomness source
        int fd = open("/dev/urandom", O_RDONLY);
        assert(fd >= 0);

        // i32 randomized testing
        for(int ix = 0; ix < 100; ix++)
        {
            int32_t lo = 0, bound = 0;
            uint32_t xorbits = 0;
            assert(read(fd, &lo, sizeof lo) == sizeof lo);
            assert(read(fd, &bound, sizeof bound) == sizeof bound);
            assert(read(fd, &xorbits, sizeof xorbits) == sizeof xorbits);
            int32_t i32 = cxsrandlib_i32_range(lo, bound, xorbits, &state);
            assert(i32 >= lo);
            if(bound > lo)
                assert(i32 < bound);
        }

        // u32 randomized testing
        for(int ix = 0; ix < 100; ix++)
        {
            uint32_t lo = 0, bound = 0, xorbits = 0;
            assert(read(fd, &lo, sizeof lo) == sizeof lo);
            assert(read(fd, &bound, sizeof bound) == sizeof bound);
            assert(read(fd, &xorbits, sizeof xorbits) == sizeof xorbits);
            uint32_t u32 = cxsrandlib_u32_range(lo, bound, xorbits, &state);
            assert(u32 >= lo);
            if(bound > lo)
                assert(u32 < bound);
        }

        // Close fd
        close(fd);
    }

    // Results generator for probability analysis
    else for(int ix = 0; ix < 100; ix++)
    {
        switch(argv[1][0])
        {
        case 'u': printf("%u,\n", cxsrandlib_u32(&state)); break;
        case 'r': printf("%d,\n", cxsrandlib_i32_range(0, 100, 0, &state)); break;
        case 'd': printf("%f,\n", cxsrandlib_double(&state)); break;
        case 'n': printf("%f,\n", cxsrandlib_normal(0.0, 1.0, &state)); break;
        }
    }

    // Done
    printf("cxsrandlibtest completed successfully\n");
    exit(0);
}
