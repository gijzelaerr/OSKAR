/*
 * Copyright (c) 2011-2014, The University of Oxford
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

#include <oskar_correlate_functions_inline.h>
#include <oskar_correlate_point_cuda.h>
#include <oskar_sinc.h>
#include <oskar_add_inline.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Kernel wrappers. ======================================================== */

/* Single precision. */
void oskar_correlate_point_cuda_f(int num_sources,
        int num_stations, const float4c* d_jones,
        const float* d_source_I, const float* d_source_Q,
        const float* d_source_U, const float* d_source_V,
        const float* d_source_l, const float* d_source_m,
        const float* d_station_u, const float* d_station_v,
        float inv_wavelength, float frac_bandwidth, float4c* d_vis)
{
    dim3 num_threads(128, 1);
    dim3 num_blocks(num_stations, num_stations);
    size_t shared_mem = num_threads.x * sizeof(float4c);
    oskar_correlate_point_cudak_f
    OSKAR_CUDAK_CONF(num_blocks, num_threads, shared_mem)
    (num_sources, num_stations, d_jones, d_source_I, d_source_Q, d_source_U,
            d_source_V, d_source_l, d_source_m, d_station_u,
            d_station_v, inv_wavelength, frac_bandwidth, d_vis);
}

/* Double precision. */
void oskar_correlate_point_cuda_d(int num_sources,
        int num_stations, const double4c* d_jones,
        const double* d_source_I, const double* d_source_Q,
        const double* d_source_U, const double* d_source_V,
        const double* d_source_l, const double* d_source_m,
        const double* d_station_u, const double* d_station_v,
        double inv_wavelength, double frac_bandwidth, double4c* d_vis)
{
    dim3 num_threads(128, 1);
    dim3 num_blocks(num_stations, num_stations);
    size_t shared_mem = num_threads.x * sizeof(double4c);
    oskar_correlate_point_cudak_d
    OSKAR_CUDAK_CONF(num_blocks, num_threads, shared_mem)
    (num_sources, num_stations, d_jones, d_source_I, d_source_Q, d_source_U,
            d_source_V, d_source_l, d_source_m, d_station_u,
            d_station_v, inv_wavelength, frac_bandwidth, d_vis);
}

#ifdef __cplusplus
}
#endif


/* Kernels. ================================================================ */

/* Indices into the visibility/baseline matrix. */
#define SP blockIdx.x /* Column index. */
#define SQ blockIdx.y /* Row index. */

extern __shared__ float4c  smem_f[];
extern __shared__ double4c smem_d[];

/* Single precision. */
__global__
void oskar_correlate_point_cudak_f(const int num_sources,
        const int num_stations, const float4c* restrict jones,
        const float* restrict source_I, const float* restrict source_Q,
        const float* restrict source_U, const float* restrict source_V,
        const float* restrict source_l, const float* restrict source_m,
        const float* restrict station_u, const float* restrict station_v,
        const float inv_wavelength, const float frac_bandwidth,
        float4c* restrict vis)
{
    __shared__ float uu, vv;
    float4c sum;
    int i;

    /* Return immediately if in the wrong half of the visibility matrix. */
    if (SQ >= SP) return;

    /* Get common baseline values per thread block. */
    if (threadIdx.x == 0)
    {
        oskar_evaluate_modified_baseline_inline_f(station_u[SP],
                station_u[SQ], station_v[SP], station_v[SQ], inv_wavelength,
                frac_bandwidth, &uu, &vv);
    }
    __syncthreads();

    /* Get pointers to source vectors for both stations. */
    const float4c* restrict station_p = &jones[num_sources * SP];
    const float4c* restrict station_q = &jones[num_sources * SQ];

    /* Each thread loops over a subset of the sources. */
    oskar_clear_complex_matrix_f(&sum); /* Partial sum per thread. */
    for (i = threadIdx.x; i < num_sources; i += blockDim.x)
    {
        /* Compute bandwidth-smearing term. */
        float rb = oskar_sinc_f(uu * source_l[i] + vv * source_m[i]);

        /* Accumulate baseline visibility response for source. */
        oskar_accumulate_baseline_visibility_for_source_inline_f(&sum, i,
                source_I, source_Q, source_U, source_V,
                station_p, station_q, rb);
    }

    /* Store partial sum for the thread in shared memory and synchronise. */
    smem_f[threadIdx.x] = sum;
    __syncthreads();

    /* Accumulate contents of shared memory. */
    if (threadIdx.x == 0)
    {
        /* Sum over all sources for this baseline. */
        for (i = 1; i < blockDim.x; ++i)
        {
            oskar_add_complex_matrix_in_place_f(&sum, &smem_f[i]);
        }

        /* Add result of this thread block to the baseline visibility. */
        i = oskar_evaluate_baseline_index_inline(num_stations, SP, SQ);
        oskar_add_complex_matrix_in_place_f(&vis[i], &sum);
    }
}

/* Double precision. */
__global__
void oskar_correlate_point_cudak_d(const int num_sources,
        const int num_stations, const double4c* restrict jones,
        const double* restrict source_I, const double* restrict source_Q,
        const double* restrict source_U, const double* restrict source_V,
        const double* restrict source_l, const double* restrict source_m,
        const double* restrict station_u, const double* restrict station_v,
        const double inv_wavelength, const double frac_bandwidth,
        double4c* restrict vis)
{
    __shared__ double uu, vv;
    double4c sum;
    int i;

    /* Return immediately if in the wrong half of the visibility matrix. */
    if (SQ >= SP) return;

    /* Get common baseline values per thread block. */
    if (threadIdx.x == 0)
    {
        oskar_evaluate_modified_baseline_inline_d(station_u[SP],
                station_u[SQ], station_v[SP], station_v[SQ], inv_wavelength,
                frac_bandwidth, &uu, &vv);
    }
    __syncthreads();

    /* Get pointers to source vectors for both stations. */
    const double4c* restrict station_p = &jones[num_sources * SP];
    const double4c* restrict station_q = &jones[num_sources * SQ];

    /* Each thread loops over a subset of the sources. */
    oskar_clear_complex_matrix_d(&sum); /* Partial sum per thread. */
    for (i = threadIdx.x; i < num_sources; i += blockDim.x)
    {
        /* Compute bandwidth-smearing term. */
        double rb = oskar_sinc_d(uu * source_l[i] + vv * source_m[i]);

        /* Accumulate baseline visibility response for source. */
        oskar_accumulate_baseline_visibility_for_source_inline_d(&sum, i,
                source_I, source_Q, source_U, source_V,
                station_p, station_q, rb);
    }

    /* Store partial sum for the thread in shared memory and synchronise. */
    smem_d[threadIdx.x] = sum;
    __syncthreads();

    /* Accumulate contents of shared memory. */
    if (threadIdx.x == 0)
    {
        /* Sum over all sources for this baseline. */
        for (i = 1; i < blockDim.x; ++i)
        {
            oskar_add_complex_matrix_in_place_d(&sum, &smem_d[i]);
        }

        /* Add result of this thread block to the baseline visibility. */
        i = oskar_evaluate_baseline_index_inline(num_stations, SP, SQ);
        oskar_add_complex_matrix_in_place_d(&vis[i], &sum);
    }
}
