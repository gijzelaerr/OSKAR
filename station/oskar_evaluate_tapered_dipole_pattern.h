/*
 * Copyright (c) 2012, The University of Oxford
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

#ifndef OSKAR_EVALUATE_TAPERED_DIPOLE_PATTERN_H_
#define OSKAR_EVALUATE_TAPERED_DIPOLE_PATTERN_H_

/**
 * @file oskar_evaluate_tapered_dipole_pattern.h
 */

#include "oskar_global.h"
#include "utility/oskar_Mem.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Evaluates pattern of a perfect dipole at source positions.
 *
 * @details
 * This CUDA kernel evaluates the pattern of a perfect dipole antenna
 * at the supplied source positions.
 *
 * The output matrix is
 *
 * ( g_theta^a   g_phi^a )
 * ( g_theta^b   g_phi^b )
 *
 * where phi and theta are the angles measured from x to y and from xy to z,
 * respectively.
 *
 * The supplied theta and phi positions of the sources are the <b>modified</b>
 * source positions. They must be adjusted relative to a dipole with its axis
 * oriented along the x-direction.
 *
 * @param[out] pattern           Array of output Jones matrices per source.
 * @param[in] theta              Source position (modified) theta values in rad.
 * @param[in] phi                Source position (modified) phi values in rad.
 * @param[in] cos_power          Power of cosine taper (use 0 if none).
 * @param[in] gaussian_fwhm_rad  Gaussian FWHM of taper (use 0 if none).
 * @param[in] return_x_dipole    If true, return X dipole; else return Y dipole.
 */
OSKAR_EXPORT
int oskar_evaluate_tapered_dipole_pattern(oskar_Mem* pattern, int num_points,
        const oskar_Mem* theta, const oskar_Mem* phi, int cos_power,
        double gaussian_fwhm_rad, int return_x_dipole);

#ifdef __cplusplus
}
#endif

#endif /* OSKAR_EVALUATE_TAPERED_DIPOLE_PATTERN_H_ */
