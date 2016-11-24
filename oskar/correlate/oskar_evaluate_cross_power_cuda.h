/*
 * Copyright (c) 2014-2015, The University of Oxford
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

#ifndef OSKAR_EVALUATE_CROSS_POWER_CUDA_H_
#define OSKAR_EVALUATE_CROSS_POWER_CUDA_H_

/**
 * @file oskar_evaluate_cross_power_cuda.h
 */

#include <oskar_global.h>
#include <utility/oskar_vector_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * CUDA function to evaluate the cross-power product from all stations
 * (single precision).
 *
 * @details
 * This function evaluates the average cross-power product for the supplied
 * sources from all stations.
 *
 * The \p jones block is two dimensional, and the source dimension
 * is the fastest varying.
 *
 * Note that all pointers are device pointers, and must not be dereferenced
 * in host code.
 *
 * @param[in] num_sources    The number of sources in the input arrays.
 * @param[in] num_stations   The number of stations in the input arrays.
 * @param[in] d_jones        Pointer to Jones matrix block
 *                           (length \p num_sources * \p num_stations).
 * @param[out] d_out         Pointer to output average cross-power product
 *                           (length \p num_sources).
 */
OSKAR_EXPORT
void oskar_evaluate_cross_power_cuda_f(int num_sources,
        int num_stations, const float4c* d_jones, float4c* d_out);

/**
 * @brief
 * CUDA function to evaluate the cross-power product from all stations
 * (single precision).
 *
 * @details
 * This function evaluates the average cross-power product for the supplied
 * sources from all stations.
 *
 * The \p jones block is two dimensional, and the source dimension
 * is the fastest varying.
 *
 * Note that all pointers are device pointers, and must not be dereferenced
 * in host code.
 *
 * @param[in] num_sources    The number of sources in the input arrays.
 * @param[in] num_stations   The number of stations in the input arrays.
 * @param[in] d_jones        Pointer to Jones matrix block
 *                           (length \p num_sources * \p num_stations).
 * @param[out] d_out         Pointer to output average cross-power product
 *                           (length \p num_sources).
 */
OSKAR_EXPORT
void oskar_evaluate_cross_power_scalar_cuda_f(int num_sources,
        int num_stations, const float2* d_jones, float2* d_out);

/**
 * @brief
 * CUDA function to evaluate the cross-power product from all stations
 * (double precision).
 *
 * @details
 * This function evaluates the average cross-power product for the supplied
 * sources from all stations.
 *
 * The \p jones block is two dimensional, and the source dimension
 * is the fastest varying.
 *
 * Note that all pointers are device pointers, and must not be dereferenced
 * in host code.
 *
 * @param[in] num_sources    The number of sources in the input arrays.
 * @param[in] num_stations   The number of stations in the input arrays.
 * @param[in] d_jones        Pointer to Jones matrix block
 *                           (length \p num_sources * \p num_stations).
 * @param[out] d_out         Pointer to output average cross-power product
 *                           (length \p num_sources).
 */
OSKAR_EXPORT
void oskar_evaluate_cross_power_cuda_d(int num_sources,
        int num_stations, const double4c* d_jones, double4c* d_out);

/**
 * @brief
 * CUDA function to evaluate the cross-power product from all stations
 * (double precision).
 *
 * @details
 * This function evaluates the average cross-power product for the supplied
 * sources from all stations.
 *
 * The \p jones block is two dimensional, and the source dimension
 * is the fastest varying.
 *
 * Note that all pointers are device pointers, and must not be dereferenced
 * in host code.
 *
 * @param[in] num_sources    The number of sources in the input arrays.
 * @param[in] num_stations   The number of stations in the input arrays.
 * @param[in] d_jones        Pointer to Jones matrix block
 *                           (length \p num_sources * \p num_stations).
 * @param[out] d_out         Pointer to output average cross-power product
 *                           (length \p num_sources).
 */
OSKAR_EXPORT
void oskar_evaluate_cross_power_scalar_cuda_d(int num_sources,
        int num_stations, const double2* d_jones, double2* d_out);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_EVALUATE_CROSS_POWER_CUDA_H_ */
