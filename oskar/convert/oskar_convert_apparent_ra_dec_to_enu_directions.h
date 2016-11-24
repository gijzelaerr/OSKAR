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


#ifndef OSKAR_CONVERT_APPARENT_RA_DEC_TO_ENU_DIRECTIONS_H_
#define OSKAR_CONVERT_APPARENT_RA_DEC_TO_ENU_DIRECTIONS_H_

/**
 * @file oskar_convert_apparent_ra_dec_to_enu_directions.h
 */

#include <oskar_global.h>
#include <mem/oskar_mem.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Equatorial to horizontal 3D direction cosines (single precision).
 *
 * @details
 * This function computes the x,y,z direction cosines in the horizontal
 * coordinate system for points specified in an equatorial frame.
 *
 * Points where z is negative are below the local horizon.
 *
 * @param[in]  n   The number of points to convert.
 * @param[in]  ra  Right Ascensions in radians.
 * @param[in]  dec Declinations in radians.
 * @param[in]  lst The current local sidereal time in radians.
 * @param[in]  lat The geodetic latitude of the origin of the horizon frame.
 * @param[out] x   x-direction-cosines in the horizontal system.
 * @param[out] y   y-direction-cosines in the horizontal system.
 * @param[out] z   z-direction-cosines in the horizontal system.
 */
OSKAR_EXPORT
void oskar_convert_apparent_ra_dec_to_enu_directions_f(int n,
        const float* ra, const float* dec, float lst, float lat, float* x,
        float* y, float* z);

/**
 * @brief
 * Equatorial to horizontal 3D direction cosines (double precision).
 *
 * @details
 * This function computes the x,y,z direction cosines in the horizontal
 * coordinate system for points specified in an equatorial frame.
 *
 * Points where z is negative are below the local horizon.
 *
 * @param[in]  n   The number of points to convert.
 * @param[in]  ra  Right Ascensions in radians.
 * @param[in]  dec Declinations in radians.
 * @param[in]  lst The current local sidereal time in radians.
 * @param[in]  lat The geodetic latitude of the origin of the horizon frame.
 * @param[out] x   x-direction-cosines in the horizontal system.
 * @param[out] y   y-direction-cosines in the horizontal system.
 * @param[out] z   z-direction-cosines in the horizontal system.
 */
OSKAR_EXPORT
void oskar_convert_apparent_ra_dec_to_enu_directions_d(int n,
        const double* ra, const double* dec, double lst, double lat, double* x,
        double* y, double* z);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_CONVERT_APPARENT_RA_DEC_TO_ENU_DIRECTIONS_H_ */
