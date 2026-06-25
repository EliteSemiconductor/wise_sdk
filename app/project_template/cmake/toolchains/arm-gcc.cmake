# ============================================================================
# CMake toolchain file — Arm GNU toolchain (arm-none-eabi-gcc), Cortex-M3 / ER8130.
# Use:  cmake -S app/project_template/cmake -B build/gcc \
#             --toolchain app/project_template/cmake/toolchains/arm-gcc.cmake
#
# Compiler is found on PATH by default. Override the location with
#   -DARM_GCC_BIN=<.../bin>   (e.g. the xPack GCC bundled with the Eclipse setup)
# or set the ARM_GCC_BIN environment variable.
# ============================================================================
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT ARM_GCC_BIN AND DEFINED ENV{ARM_GCC_BIN})
    set(ARM_GCC_BIN "$ENV{ARM_GCC_BIN}")
endif()
if(ARM_GCC_BIN)
    # tolerate a trailing slash/backslash in the supplied path
    string(REGEX REPLACE "[\\/]+$" "" ARM_GCC_BIN "${ARM_GCC_BIN}")
    set(_P "${ARM_GCC_BIN}/")
endif()
set(_PFX "${_P}arm-none-eabi-")

# When given a full path, CMake does NOT auto-append the host executable
# extension, so add it explicitly on Windows.
if(CMAKE_HOST_WIN32)
    set(_EXE ".exe")
endif()

set(CMAKE_C_COMPILER   "${_PFX}gcc${_EXE}")
set(CMAKE_ASM_COMPILER "${_PFX}gcc${_EXE}")
set(CMAKE_OBJCOPY      "${_PFX}objcopy${_EXE}" CACHE FILEPATH "")
set(CMAKE_SIZE         "${_PFX}size${_EXE}"    CACHE FILEPATH "")

# Bare-metal: the compiler check must not try to build a full executable.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(_CPU "-mcpu=cortex-m3 -mthumb")
set(CMAKE_C_FLAGS_INIT   "${_CPU} -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS_INIT "${_CPU}")
# newlib-nano + nosys (matches the Eclipse build); retarget.c provides the real
# _write/_sbrk that override the nosys stubs.
set(CMAKE_EXE_LINKER_FLAGS_INIT "${_CPU} --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
