/*
 * Copyright (c) 2011, The University of Oxford
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

#include "ms/oskar_ms_create_vis1.h"
#include "ms/MsCreate.h"

void oskar_ms_create_vis1(const char* name, double mjd, double exposure,
        double interval, double ra, double dec, int na, const double* ax,
        const double* ay, const double* az, int nv, const double* u,
        const double* v, const double* w, const double* vis, const int* ant1,
        const int* ant2, double freq)
{
    // Create the MsCreate object, passing it the filename.
    oskar::MsCreate ms(name, mjd, exposure, interval);

    // Add antenna positions.
    ms.addAntennas(na, ax, ay, az);

    // Add the Right Ascension & Declination of field centre.
    ms.addField(ra, dec);

    // Add polarisation.
    ms.addPolarisation(1);

    // Add frequency band.
    ms.addBand(0, 1, freq, 1.0);

    // Add visibilities.
    ms.addVisibilities(1, nv, u, v, w, vis, ant1, ant2);
}
