# add_custom kernel for Ascend 910B

This example demonstrates how to implement a simple custom operator using the
[CANN](https://www.hiascend.com/software/cann) Ascend-C API. The operator performs an
addition of two tensors on the Ascend 910B device.

## Files

- `add_custom.c`       - Kernel implementation using the Ascend-C API.
- `run_add_custom.cpp` - Host side example that invokes the kernel and
  verifies the result on the CPU.
- `README.md`          - This document.

## Build

Make sure the Ascend environment variables are set and the Ascend-C compiler is
available. Then you can build the kernel with:

```bash
ascend-c add_custom.c
```

The compiler will generate the kernel meta files under `kernel_meta/`.

To build the host example, compile `run_add_custom.cpp` with `g++` linking
against the Ascend runtime libraries. A typical command looks like:

```bash
g++ run_add_custom.cpp -o run_add_custom \
    -I$ASCEND_HOME/ascend-toolkit/latest/include \
    -L$ASCEND_HOME/ascend-toolkit/latest/lib64 -lacl_rt
```

Run the binary after compiling the kernel to verify its result:

```bash
./run_add_custom
```
