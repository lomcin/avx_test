// To compile with gcc : gcc intrin.c -msse -mavx -o intrin
// To compile and run with time:  gcc intrin.c -msse -mavx -o intrin && (time ./intrin)
#include <immintrin.h>
#include <intrin.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int use_sse = 1;
    int use_avx = 1;
    int use_sequential = 1;
    int sse = __builtin_cpu_supports("sse");
    int avx = __builtin_cpu_supports("avx");
    if (use_sse)
        printf(sse ? "SSE available\n" : " SSE not available\n");
    if (use_avx)
        printf(avx ? "AVX available\n" : " AVX not available\n");
    int i, nx = 80000000;
    int k, iter = 10;
    float *a, *b, *c1, *c2;

    a = (float *)malloc(nx * sizeof(float));
    b = (float *)malloc(nx * sizeof(float));
    c1 = (float *)malloc(nx * sizeof(float));
    c2 = (float *)malloc(nx * sizeof(float));

    if (use_sequential && (use_sse || use_avx))
        for (i = 0; i < nx; i++)
        {
            a[i] = i;
            b[i] = 2 * i;
        }

    // without sse
    if (use_sequential)
        for (k = 0; k < iter; k++)
            for (i = 0; i < nx; i++)
                c1[i] = a[i] + b[i];

    // using sse
    if (sse && use_sse)
    {
        __m128 va, vb, vc;
        for (k = 0; k < iter; k++)
            for (i = 0; i < nx; i += 4)
            {
                va = __builtin_ia32_loadups(&a[i]);
                vb = __builtin_ia32_loadups(&b[i]);
                vc = __builtin_ia32_addps(va, vb);
                __builtin_ia32_storeups(&c2[i], vc);
            }
    }

    // result for SSE
    if (use_sequential && use_sse && sse)
    {
        float errsum;
        for (i = 0; i < nx; i++)
            errsum += c1[i] - c2[i];
        printf("SSE errsum=%f\n", errsum);
    }

    // using avx
    if (avx && use_avx)
    {
        __m256 va, vb, vc;
        for (k = 0; k < iter; k++)
            for (i = 0; i < nx; i += 4)
            {
                va = _mm256_loadu_ps(&a[i]);
                vb = _mm256_loadu_ps(&b[i]);
                vc = _mm256_add_ps(va, vb);
                _mm256_storeu_ps(&c2[i], vc);
            }
    }

    // result for AVX
    if (use_sequential && use_avx && avx)
    {
        float errsum;
        for (i = 0; i < nx; i++)
            errsum += c1[i] - c2[i];
        printf("AVX errsum=%f\n", errsum);
    }

    return 0;
}