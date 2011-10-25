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

#include <cuda_runtime_api.h>
#include <stdlib.h>
#include "utility/oskar_mem_alloc.h"
#include "utility/oskar_mem_element_size.h"

int oskar_mem_alloc(oskar_Mem* mem)
{
    // Check that the structure exists.
    if (mem == NULL) return OSKAR_ERR_INVALID_ARGUMENT;

    // Get the meta-data.
    int n_elements = mem->private_n_elements;
    int location = mem->private_location;
    int type = mem->private_type;

    // Get the memory size.
    size_t element_size = oskar_mem_element_size(type);
    if (element_size == 0)
        return OSKAR_ERR_BAD_DATA_TYPE;
    size_t bytes = n_elements * element_size;

    // Check whether the memory should be on the host or the device.
    int err = 0;
    if (location == OSKAR_LOCATION_CPU)
    {
        // Allocate host memory.
        mem->data = malloc(bytes);
        if (mem->data == NULL)
            err = OSKAR_ERR_MEMORY_ALLOC_FAILURE;
    }
    else if (location == OSKAR_LOCATION_GPU)
    {
        // Allocate GPU memory.
        cudaMalloc(&mem->data, bytes);
        err = cudaPeekAtLastError();
    }
    else
    {
        return OSKAR_ERR_BAD_LOCATION;
    }
    return err;
}