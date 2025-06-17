#include "kernel_operator.h"

#ifdef __cplusplus
extern "C" {
#endif

// Simple vector add kernel for Ascend 910B written in Ascend-C.
__aicore__ inline void add_compute(global_ptr half *out, global_ptr const half *a,
                                    global_ptr const half *b, uint32_t num)
{
    for (uint32_t i = 0; i < num; ++i) {
        out[i] = a[i] + b[i];
    }
}

__aicore__ __global__ void add_custom(global_ptr half *out,
                                      global_ptr const half *a,
                                      global_ptr const half *b,
                                      uint32_t num)
{
    add_compute(out, a, b, num);
}

#ifdef __cplusplus
}
#endif
