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

#include "utility/oskar_binary_stream_write_std_metadata.h"
#include "utility/oskar_binary_stream_write.h"
#include "utility/oskar_BinaryTag.h"
#include "utility/oskar_Mem.h"
#include "utility/oskar_system_clock_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

int oskar_binary_stream_write_std_metadata(FILE* stream)
{
    int error;
    const char* str;
    size_t len;

    /* Write the system date and time. */
    str = oskar_system_clock_time(0, NULL);
    len = 1 + strlen(str);
    error = oskar_binary_stream_write(stream, OSKAR_CHAR,
            OSKAR_TAG_GROUP_METADATA, OSKAR_TAG_METADATA_DATE_TIME_STRING, 0,
            len, str);
    if (error) return error;

    /* Write the current working directory. */
    str = getenv("PWD");
    if (!str) str = getenv("CD");
    if (str)
    {
        len = 1 + strlen(str);
        error = oskar_binary_stream_write(stream, OSKAR_CHAR,
                OSKAR_TAG_GROUP_METADATA, OSKAR_TAG_METADATA_CWD, 0,
                len, str);
        if (error) return error;
    }

    /* Write the username. */
    str = getenv("USERNAME");
    if (str)
    {
        len = 1 + strlen(str);
        error = oskar_binary_stream_write(stream, OSKAR_CHAR,
                OSKAR_TAG_GROUP_METADATA, OSKAR_TAG_METADATA_USERNAME, 0,
                len, str);
        if (error) return error;
    }

    return OSKAR_SUCCESS;
}

#ifdef __cplusplus
}
#endif
