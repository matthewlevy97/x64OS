Kernel Memory Map:

0x0000000000000000 - 0x00007FFFFFFFFFFF		User Space
0xFFFFFF8000000000 - 0xFFFFFFFFFFFFFFFF		Kernel Space

0xFFFFFF8000100000 - 0xFFFFFF8FFFFFFFFF		Kernel.bin / physical memory mapped into here

0xFFFFFF9000000000 - 0xFFFFFF9001FFFFFF		Kernel stack          (32 MiB)   -------|
0xFFFFFF9002000000 - 0xFFFFFF9003FFFFFF		Kernel local heap     (32 MiB)          |--> Give entire P2 to storing process information (P4: 511, P3: 64)
0xFFFFFF9004000000 - 0xFFFFFF903FFFFFFF		Kernel Process local information -------|

0xFFFFFFA000000000 - 0xFFFFFFAFFFFFFFFF		Kernel global heap    (64 GiB)

