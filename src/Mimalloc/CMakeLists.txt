project("mimalloc")

# mimalloc replaces the process allocator: hooked at runtime on Windows, overridden at link time elsewhere.
# On Android this works because the benchmarks are executables (see docs/development/android.md):
# the executable comes first in bionic's global symbol lookup. Never link this into a library the app dlopen()s.

add_library(mimalloc OBJECT static.c)
target_include_directories(mimalloc PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
if (WIN32)
    target_link_libraries(mimalloc PUBLIC detours)
endif ()

# No transparent huge pages, on every platform (docs/development/known-issues.md, "Transparent huge pages"). mimalloc's
# default is 0 on Android but 2 on Linux, where whether memory gets 2 MiB pages then depends on each machine's
# /sys/kernel/mm/transparent_hugepage/enabled. With 0, mimalloc disables THP for the whole benchmark process
# (prctl(PR_SET_THP_DISABLE)), including memory not allocated through mimalloc. Windows uses no large pages by default.
target_compile_definitions(mimalloc PRIVATE MI_DEFAULT_ALLOW_THP=0)

# Ignore the MIMALLOC_* environment variables, so a machine's environment cannot change the allocator's behaviour
# (e.g. MIMALLOC_ALLOW_THP, MIMALLOC_ALLOW_LARGE_OS_PAGES, MIMALLOC_PURGE_DELAY). Turn on only for diagnostics
# (MIMALLOC_SHOW_STATS, MIMALLOC_VERBOSE, ...); never for builds whose results are published.
option(ELPIDA_MIMALLOC_ALLOW_ENVIRONMENT "Let mimalloc read its MIMALLOC_* environment variables (diagnostics only)" OFF)
if (NOT ELPIDA_MIMALLOC_ALLOW_ENVIRONMENT)
    target_compile_definitions(mimalloc PRIVATE MI_NO_GETENV=1)
endif ()

# Android: mimalloc defaults to pthread_getspecific() for its per-thread heap, ~60% slower malloc+free than the
# __thread model Linux uses (docs/development/android.md, "TLS"). Use __thread when the target has ELF TLS (API >= 29; the
# benchmarks are API 29 static executables, so this also holds on older devices). Below 29 __thread would be emulated
# TLS, whose __emutls_get_address() calls malloc and recurses into mimalloc until the stack overflows.
# TARGET_ENV is e.g. android29, or androideabi29 for armv7a
if (ANDROID AND TARGET_ENV MATCHES "^android(eabi)?([0-9]+)$" AND CMAKE_MATCH_2 GREATER_EQUAL 29)
    target_compile_definitions(mimalloc PRIVATE MI_TLS_MODEL_LOCAL=1)
endif ()
