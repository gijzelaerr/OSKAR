/*
 * Copyright (c) 2013-2014, The University of Oxford
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

#include "apps/lib/oskar_TelescopeLoadElementPattern.h"
#include "apps/lib/oskar_Dir.h"
#include <oskar_log.h>
#include <oskar_file_exists.h>

using std::map;
using std::string;

const string oskar_TelescopeLoadElementPattern::spline_name_x =
        "spline_data_cache_x.bin";
const string oskar_TelescopeLoadElementPattern::spline_name_y =
        "spline_data_cache_y.bin";

oskar_TelescopeLoadElementPattern::oskar_TelescopeLoadElementPattern(
        const oskar_Settings* settings, oskar_Log* log)
{
    settings_ = settings;
    log_ = log;
}

oskar_TelescopeLoadElementPattern::~oskar_TelescopeLoadElementPattern()
{
}

void oskar_TelescopeLoadElementPattern::load(oskar_Telescope* telescope,
        const oskar_Dir& cwd, int num_subdirs, map<string, string>& filemap,
        int* status)
{
    update_map(filemap, cwd);

    // Load element pattern data for stations only at the deepest level!
    if (num_subdirs == 0)
    {
        int num_stations = oskar_telescope_num_stations(telescope);
        for (int i = 0; i < num_stations; ++i)
        {
            oskar_Station* s = oskar_telescope_station(telescope, i);
            oskar_station_resize_element_types(s, 1, status);
            load_element_patterns(log_, settings_, s, filemap, status);
        }
    }
}

void oskar_TelescopeLoadElementPattern::load(oskar_Station* station,
        const oskar_Dir& cwd, int num_subdirs, int /*depth*/,
        map<string, string>& filemap, int* status)
{
    update_map(filemap, cwd);

    // Load element pattern data for stations only at the deepest level!
    if (num_subdirs == 0)
    {
        oskar_station_resize_element_types(station, 1, status);
        load_element_patterns(log_, settings_, station, filemap, status);
    }
}

string oskar_TelescopeLoadElementPattern::name() const
{
    return string("element pattern loader");
}

void oskar_TelescopeLoadElementPattern::load_element_patterns(oskar_Log* log,
        const oskar_Settings* settings, oskar_Station* station,
        map<string, string>& filemap, int* status)
{
    // Check if safe to proceed.
    if (*status) return;

    // Return if element patterns are disabled.
    if (!settings->telescope.aperture_array.element_pattern.
            enable_numerical_patterns)
        return;

    // Get current absolute paths for spline data.
    string spline_path_x, spline_path_y;
    if (filemap.count(spline_name_x))
        spline_path_x = filemap.at(spline_name_x);
    if (filemap.count(spline_name_y))
        spline_path_y = filemap.at(spline_name_y);

    // Generate a unique key for a pair of element files at this location
    // (be they ASCII data or cached coefficients).
    // Return if no files have been found.
    string files;
    files.append(spline_path_x);
    files.append(spline_path_y);
    if (files.length() == 0)
        return;

    // Copy element pattern data and then return if this file combination
    // has already been loaded.
    if (models.count(files))
    {
        // Copy the element pattern data.
        oskar_element_copy(oskar_station_element(station, 0),
                models.at(files), status);
        return;
    }

    // Load cached spline coefficients for X dipole, if present.
    if (spline_path_x.length() > 0 &&
            oskar_file_exists(spline_path_x.c_str()))
    {
        oskar_log_message(log, 0, "Loading spline data (X): %s",
                spline_path_x.c_str());
        oskar_log_message(log, 0, "");
        oskar_element_read(oskar_station_element(station, 0), 1,
                spline_path_x.c_str(), status);
    }

    // Load cached spline coefficients for Y dipole, if present.
    if (spline_path_y.length() > 0 &&
            oskar_file_exists(spline_path_y.c_str()))
    {
        oskar_log_message(log, 0, "Loading spline data (Y): %s",
                spline_path_y.c_str());
        oskar_log_message(log, 0, "");
        oskar_element_read(oskar_station_element(station, 0), 2,
                spline_path_y.c_str(), status);
    }

    // Store pointer to the element model for these files.
    models[files] = oskar_station_element(station, 0);
}

void oskar_TelescopeLoadElementPattern::update_map(map<string, string>& filemap,
        const oskar_Dir& cwd)
{
    // Update the dictionary of element files for the current directory.
    // The presence of spline coefficients is sufficient to override ones
    // from a higher level.
    if (cwd.exists(spline_name_x))
    {
        filemap[spline_name_x] = cwd.absoluteFilePath(spline_name_x);
    }
    if (cwd.exists(spline_name_y))
    {
        filemap[spline_name_y] = cwd.absoluteFilePath(spline_name_y);
    }
}
