# add_custom kernel for Ascend 910B

This example demonstrates how to implement a simple custom operator using the
[CANN](https://www.hiascend.com/software/cann) Ascend-C API. The operator performs an
addition of two tensors on the Ascend 910B device.

## Files

- `add_custom.c`  - Implementation of the kernel using the Ascend-C API.
- `README.md`      - This document.

## Build

Make sure the Ascend environment variables are set and the Ascend-C compiler is
available. Then you can build the kernel with:

```bash
ascend-c add_custom.c
```

The compiler will generate the kernel meta files under `kernel_meta/`.
