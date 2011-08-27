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

#include "interferometry/test/CudaCorrelatorTest.h"
#include "interferometry/cudak/oskar_cudak_correlator.h"
#include <cmath>
#include <cstdlib>
#include <vector>

#define TIMER_ENABLE 1
#include "utility/timer.h"

// Register the test class.
CPPUNIT_TEST_SUITE_REGISTRATION(CudaCorrelatorTest);

/**
 * @details
 * Sets up the context before running each test method.
 */
void CudaCorrelatorTest::setUp()
{
}

/**
 * @details
 * Clean up routine called after each test is run.
 */
void CudaCorrelatorTest::tearDown()
{
}

/**
 * @details
 * Tests correlator kernel.
 */
void CudaCorrelatorTest::test_method()
{
    int ns = 100;
    int na = 25;
    int nb = na * (na - 1) / 2;
    float lambda_bandwidth = 0.0f;
    std::vector<float> h_I(ns);
    std::vector<float> h_Q(ns);
    std::vector<float> h_U(ns);
    std::vector<float> h_V(ns);
    std::vector<float> h_l(ns);
    std::vector<float> h_m(ns);
    std::vector<float> h_u(na);
    std::vector<float> h_v(na);
    std::vector<float4c> h_vis(nb);
    std::vector<float4c> h_jones(ns * na);

    for (int s = 0; s < ns; ++s)
    {
        h_I[0] = 2.0 * (s+1);
        h_Q[0] = 0.5 * (s+1);
        h_U[0] = 0.3 * (s+1);
        h_V[0] = 0.1 * (s+1);
    }

    for (int a = 0; a < na; ++a)
    {
        for (int s = 0; s < ns; ++s)
        {
            int i = s + a * ns;
            h_jones[i].a = make_float2(float(8*i + 0), float(8*i + 1));
            h_jones[i].b = make_float2(float(8*i + 2), float(8*i + 3));
            h_jones[i].c = make_float2(float(8*i + 4), float(8*i + 5));
            h_jones[i].d = make_float2(float(8*i + 6), float(8*i + 7));
        }
    }

    // Allocate device memory.
    float *d_I, *d_Q, *d_U, *d_V, *d_l, *d_m, *d_u, *d_v;
    float4c *d_jones, *d_vis;
    cudaMalloc((void**)&d_I, ns * sizeof(float));
    cudaMalloc((void**)&d_Q, ns * sizeof(float));
    cudaMalloc((void**)&d_U, ns * sizeof(float));
    cudaMalloc((void**)&d_V, ns * sizeof(float));
    cudaMalloc((void**)&d_l, ns * sizeof(float));
    cudaMalloc((void**)&d_m, ns * sizeof(float));
    cudaMalloc((void**)&d_u, na * sizeof(float));
    cudaMalloc((void**)&d_v, na * sizeof(float));
    cudaMalloc((void**)&d_jones, ns * na * sizeof(float4c));
    cudaMalloc((void**)&d_vis, nb * sizeof(float4c));
    cudaMemcpy(d_I, &h_I[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_Q, &h_Q[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_U, &h_U[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_V, &h_V[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_l, &h_l[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_m, &h_m[0], ns * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_u, &h_u[0], na * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_v, &h_v[0], na * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_jones, &h_jones[0], ns * na * sizeof(float4c),
            cudaMemcpyHostToDevice);

    // Call the correlator kernel.
    dim3 vThd(256, 1); // Antennas, antennas.
    dim3 vBlk(na, na);
    size_t vsMem = vThd.x * sizeof(float4c);
    TIMER_START
    oskar_cudak_correlator_f <<<vBlk, vThd, vsMem>>> (ns, na, d_jones,
            d_I, d_Q, d_U, d_V, d_u, d_v, d_l, d_m, lambda_bandwidth, d_vis);
    cudaDeviceSynchronize();
    TIMER_STOP("Finished correlator kernel, %d sources", ns)
    int err = cudaPeekAtLastError();
    if (err)
    {
        printf("CUDA Error, code %d\n", err);
        CPPUNIT_FAIL("CUDA Error");
    }

    // Copy memory back to host.
    cudaMemcpy(&h_vis[0], d_vis, nb * sizeof(float4c), cudaMemcpyDeviceToHost);

    // Free device memory.
    cudaFree(d_I);
    cudaFree(d_Q);
    cudaFree(d_U);
    cudaFree(d_V);
    cudaFree(d_l);
    cudaFree(d_m);
    cudaFree(d_u);
    cudaFree(d_v);
    cudaFree(d_jones);
    cudaFree(d_vis);

    // Check contents of memory.

}