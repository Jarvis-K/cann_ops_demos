#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <acl/acl.h>

#define CHECK_RET(expr) \
    do { \
        aclError _ret = (expr); \
        if (_ret != ACL_ERROR_NONE) { \
            fprintf(stderr, "Error executing %s: %d\n", #expr, _ret); \
            return EXIT_FAILURE; \
        } \
    } while (0)

int main(int argc, char **argv) {
    const uint32_t num = 16; // number of elements
    float a[num];
    float b[num];
    float expected[num];
    for (uint32_t i = 0; i < num; ++i) {
        a[i] = (float)i;
        b[i] = (float)(2 * i);
        expected[i] = a[i] + b[i];
    }

    CHECK_RET(aclInit(nullptr));
    int deviceId = 0;
    aclrtContext context;
    CHECK_RET(aclrtCreateContext(&context, deviceId));
    CHECK_RET(aclrtSetCurrentContext(context));

    void *devA = nullptr;
    void *devB = nullptr;
    void *devOut = nullptr;
    size_t dataSize = num * sizeof(float);
    CHECK_RET(aclrtMalloc(&devA, dataSize, ACL_MEM_MALLOC_NORMAL_ONLY));
    CHECK_RET(aclrtMalloc(&devB, dataSize, ACL_MEM_MALLOC_NORMAL_ONLY));
    CHECK_RET(aclrtMalloc(&devOut, dataSize, ACL_MEM_MALLOC_NORMAL_ONLY));

    CHECK_RET(aclrtMemcpy(devA, dataSize, a, dataSize, ACL_MEMCPY_HOST_TO_DEVICE));
    CHECK_RET(aclrtMemcpy(devB, dataSize, b, dataSize, ACL_MEMCPY_HOST_TO_DEVICE));

    aclrtStream stream;
    CHECK_RET(aclrtCreateStream(&stream));

    const char *kernelFile = "kernel_meta/add_custom.o";
    const char *kernelName = "add_custom";

    size_t argsSize = sizeof(void*) * 3 + sizeof(uint32_t);
    void *args[] = { devOut, devA, devB, (void*)(uintptr_t)num };

    CHECK_RET(aclrtLaunchKernel(kernelName, 1, 1, 1, 0, stream, args, argsSize));
    CHECK_RET(aclrtSynchronizeStream(stream));

    float out[num];
    CHECK_RET(aclrtMemcpy(out, dataSize, devOut, dataSize, ACL_MEMCPY_DEVICE_TO_HOST));

    CHECK_RET(aclrtDestroyStream(stream));
    aclrtFree(devA);
    aclrtFree(devB);
    aclrtFree(devOut);
    CHECK_RET(aclrtDestroyContext(context));
    CHECK_RET(aclFinalize());

    for (uint32_t i = 0; i < num; ++i) {
        if (out[i] != expected[i]) {
            fprintf(stderr, "Mismatch at %u: %f vs %f\n", i, out[i], expected[i]);
            return EXIT_FAILURE;
        }
    }

    printf("Kernel output verified on CPU.\n");
    return EXIT_SUCCESS;
}
