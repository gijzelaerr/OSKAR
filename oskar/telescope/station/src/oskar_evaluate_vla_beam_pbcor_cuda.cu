/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "telescope/station/oskar_evaluate_vla_beam_pbcor_cuda.h"
#include "telescope/station/oskar_vla_pbcor_inline.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Kernel wrappers. ======================================================== */

/* Single precision. */
void oskar_evaluate_vla_beam_pbcor_cuda_f(float* beam, int num_sources,
        const float* l, const float* m, const float freq_ghz, const float p1,
        const float p2, const float p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_cudak_f
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}

void oskar_evaluate_vla_beam_pbcor_complex_cuda_f(float2* beam, int num_sources,
        const float* l, const float* m, const float freq_ghz, const float p1,
        const float p2, const float p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_complex_cudak_f
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}

void oskar_evaluate_vla_beam_pbcor_matrix_cuda_f(float4c* beam, int num_sources,
        const float* l, const float* m, const float freq_ghz, const float p1,
        const float p2, const float p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_matrix_cudak_f
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}


/* Double precision. */
void oskar_evaluate_vla_beam_pbcor_cuda_d(double* beam, int num_sources,
        const double* l, const double* m, const double freq_ghz,
        const double p1, const double p2, const double p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_cudak_d
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}

void oskar_evaluate_vla_beam_pbcor_complex_cuda_d(double2* beam,
        int num_sources, const double* l, const double* m,
        const double freq_ghz, const double p1, const double p2,
        const double p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_complex_cudak_d
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}

void oskar_evaluate_vla_beam_pbcor_matrix_cuda_d(double4c* beam,
        int num_sources, const double* l, const double* m,
        const double freq_ghz, const double p1, const double p2,
        const double p3)
{
    int num_blocks, num_threads = 256;
    num_blocks = (num_sources + num_threads - 1) / num_threads;
    oskar_evaluate_vla_beam_pbcor_matrix_cudak_d
    OSKAR_CUDAK_CONF(num_blocks, num_threads) (beam, num_sources,
            l, m, freq_ghz, p1, p2, p3);
}

#ifdef __cplusplus
}
#endif


/* Kernels. ================================================================ */

/* Single precision. */
__global__
void oskar_evaluate_vla_beam_pbcor_cudak_f(float* beam, int num_sources,
        const float* l, const float* m, const float freq_ghz, const float p1,
        const float p2, const float p3)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    beam[i] = oskar_vla_pbcor_inline_f(l[i], m[i], freq_ghz, p1, p2, p3);
}


__global__
void oskar_evaluate_vla_beam_pbcor_complex_cudak_f(float2* beam,
        int num_sources, const float* l, const float* m, const float freq_ghz,
        const float p1, const float p2, const float p3)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    beam[i].x = oskar_vla_pbcor_inline_f(l[i], m[i], freq_ghz, p1, p2, p3);
    beam[i].y = 0.0f;
}


__global__
void oskar_evaluate_vla_beam_pbcor_matrix_cudak_f(float4c* beam,
        int num_sources, const float* l, const float* m, const float freq_ghz,
        const float p1, const float p2, const float p3)
{
    float t;
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    t = oskar_vla_pbcor_inline_f(l[i], m[i], freq_ghz, p1, p2, p3);
    beam[i].a.x = t;
    beam[i].a.y = 0.0f;
    beam[i].b.x = 0.0f;
    beam[i].b.y = 0.0f;
    beam[i].c.x = 0.0f;
    beam[i].c.y = 0.0f;
    beam[i].d.x = t;
    beam[i].d.y = 0.0f;
}


/* Double precision. */
__global__
void oskar_evaluate_vla_beam_pbcor_cudak_d(double* beam, int num_sources,
        const double* l, const double* m, const double freq_ghz,
        const double p1, const double p2, const double p3)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    beam[i] = oskar_vla_pbcor_inline_d(l[i], m[i], freq_ghz, p1, p2, p3);
}


__global__
void oskar_evaluate_vla_beam_pbcor_complex_cudak_d(double2* beam,
        int num_sources, const double* l, const double* m,
        const double freq_ghz, const double p1, const double p2,
        const double p3)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    beam[i].x = oskar_vla_pbcor_inline_d(l[i], m[i], freq_ghz, p1, p2, p3);
    beam[i].y = 0.0;
}


__global__
void oskar_evaluate_vla_beam_pbcor_matrix_cudak_d(double4c* beam,
        int num_sources, const double* l, const double* m,
        const double freq_ghz, const double p1, const double p2,
        const double p3)
{
    double t;
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= num_sources) return;

    t = oskar_vla_pbcor_inline_d(l[i], m[i], freq_ghz, p1, p2, p3);
    beam[i].a.x = t;
    beam[i].a.y = 0.0;
    beam[i].b.x = 0.0;
    beam[i].b.y = 0.0;
    beam[i].c.x = 0.0;
    beam[i].c.y = 0.0;
    beam[i].d.x = t;
    beam[i].d.y = 0.0;
}

