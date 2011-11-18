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

#include "oskar_global.h"
#include "interferometry/oskar_Visibilities.h"
#include "interferometry/oskar_visibilities_clear_contents.h"
#include "interferometry/oskar_visibilities_get_channel_amps.h"
#include "interferometry/oskar_visibilities_init.h"
#include "interferometry/oskar_visibilities_location.h"
#include "interferometry/oskar_visibilities_read.h"
#include "interferometry/oskar_visibilities_resize.h"
#include "interferometry/oskar_visibilities_write.h"
#include "utility/oskar_mem_init.h"

oskar_Visibilities::oskar_Visibilities(int amp_type, int location,
        int num_channels, int num_times, int num_baselines)
: num_channels(num_channels),
  num_times(num_times),
  num_baselines(num_baselines),
  freq_start_hz(0.0),
  freq_inc_hz(0.0),
  time_start_mjd_utc(0.0),
  time_inc_seconds(0.0)
{
    if (oskar_visibilities_init(this, amp_type, location, num_channels,
            num_times, num_baselines))
    {
        throw "error allocation visibility structure";
    }
}

// Copy constructor.
oskar_Visibilities::oskar_Visibilities(const oskar_Visibilities* other,
        int location)
: num_channels(other->num_channels),
  num_times(other->num_times),
  num_baselines(other->num_baselines),
  freq_start_hz(0.0),
  freq_inc_hz(0.0),
  time_start_mjd_utc(0.0),
  time_inc_seconds(0.0),
  uu_metres(&other->uu_metres, location),
  vv_metres(&other->vv_metres, location),
  ww_metres(&other->ww_metres, location),
  amplitude(&other->amplitude, location)
{
}

oskar_Visibilities::~oskar_Visibilities()
{
}

int oskar_Visibilities::clear_contents()
{
    return oskar_visibilities_clear_contents(this);
}

int oskar_Visibilities::write(const char* filename)
{
    return oskar_visibilities_write(filename, this);
}

oskar_Visibilities* oskar_Visibilities::read(const char* filename, int* status)
{
    return oskar_visibilities_read(filename, status);
}

int oskar_Visibilities::resize(int num_channels, int num_times, int num_baselines)
{
    return oskar_visibilities_resize(this, num_channels, num_times, num_baselines);
}

int oskar_Visibilities::init(int amp_type, int location, int num_channels,
        int num_times, int num_baselines)
{
    return oskar_visibilities_init(this, amp_type, location, num_channels,
            num_times, num_baselines);
}

int oskar_Visibilities::get_channel_amps(oskar_Mem* vis_amps, int channel)
{
    return oskar_visibilties_get_channel_amps(vis_amps, this, channel);
}

int oskar_Visibilities::location() const
{
    return oskar_visibilities_location(this);
}
