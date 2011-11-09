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


#include "utility/oskar_get_error_string.h"
#include <cuda_runtime_api.h>

const char* oskar_get_error_string(const int error)
{
    // If the error code is positive, get the CUDA error string as OSKAR
    // error codes are negative.
    if (error > 0)
        return cudaGetErrorString((cudaError_t)error);

    // Return a string describing the OSKAR error code.
    switch(error)
    {
        case OSKAR_ERR_EOF:
            return "end of file";
            break;
        case OSKAR_ERR_FILE_IO:
            return "file I/O error";
            break;
        case OSKAR_ERR_INVALID_ARGUMENT:
            return "invalid argument";
            break;
        case OSKAR_ERR_MEMORY_ALLOC_FAILURE:
            return "memory allocation failure";
            break;
        case OSKAR_ERR_MEMORY_NOT_ALLOCATED:
            return "memory not allocated";
            break;
        case OSKAR_ERR_TYPE_MISMATCH:
            return "data type mismatch";
            break;
        case OSKAR_ERR_DIMENSION_MISMATCH:
            return "data dimension mismatch";
            break;
        case OSKAR_ERR_BAD_LOCATION:
            return "unsupported pointer location";
            break;
        case OSKAR_ERR_BAD_DATA_TYPE:
            return "unsupported data type";
            break;
        case OSKAR_ERR_BAD_JONES_TYPE:
            return "unsupported data type for Jones matrix";
            break;
        case OSKAR_ERR_OUT_OF_RANGE:
            return "memory location out of range";
            break;
        case OSKAR_ERR_VERSION_MISMATCH:
            return "incompatible OSKAR version";
            break;
        case OSKAR_ERR_BAD_UNITS:
            return "invalid units";
        default:
            break;
    };
    return "unknown error.";
}

