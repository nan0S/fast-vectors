#pragma once

#define CPP_ABOVE_17 __cplusplus > 201703L
#define UWR_FORCEINLINE inline __attribute__((__always_inline__))

#define LIKELY(x)       __builtin_expect((x), 1)
#define UNLIKELY(x)     __builtin_expect((x), 0)
