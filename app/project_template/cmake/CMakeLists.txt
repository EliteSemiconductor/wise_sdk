# ============================================================================
# project_template ??CMake build for ER8130 / er8131p9.
#
# Toolchain-agnostic: select the compiler with --toolchain toolchains/arm-gcc.cmake
# (Arm GNU) or toolchains/armclang.cmake (ARM Compiler 6). The C sources already
# handle both via the __ARMCC_VERSION / __GNUC__ guards.
#
# All CMake lives in this folder; the SDK source tree is untouched and is
# referenced by path (WISE_ROOT). Radio comes from the prebuilt library
# (libWISERadioLib_er8131p9.a); only the weak-stub source is compiled.
#
#   cmake -S . -B build -G Ninja --toolchain toolchains/arm-gcc.cmake
#   cmake --build build
# ============================================================================
cmake_minimum_required(VERSION 3.20)

# Default optimization = -Os. CMake maps the MinSizeRel build type to -Os, so use
# it when the user hasn't selected a build type. Override per build with e.g.
#   -DCMAKE_BUILD_TYPE=Debug   (-O0 -g)   or   =Release (-O3) / =RelWithDebInfo.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Build type (default = -Os)" FORCE)
endif()

project(project_template C)

# cmake/ -> project_template/ -> app/ -> wise_sdk/
get_filename_component(WISE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
set(APP_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(PLATFORM er8131p9)

# ---------------------------------------------------------------------------
# Sources (radio = prebuilt lib, so only hal_drv_radio_weak.c is compiled ??
# it's the one radio_lib object NOT inside the .a).
# ---------------------------------------------------------------------------
file(GLOB CORE_SRC CONFIGURE_DEPENDS
    ${WISE_ROOT}/wise_core_v2/api/*.c
    ${WISE_ROOT}/wise_core_v2/core_utils/*.c
    ${WISE_ROOT}/wise_core_v2/platform/hal_intf/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/arm_core/cmsis/esmt/Source/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/drv/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/hdl/*.c
)
set(MIDDLEWARE_SRC
    ${WISE_ROOT}/middleware/retarget/retarget.c
    ${WISE_ROOT}/middleware/wise_system/wise_common.c
    ${WISE_ROOT}/middleware/wise_system/wise_event.c
    ${WISE_ROOT}/middleware/wise_system/wise_schlr.c
)
set(RADIO_GLUE ${WISE_ROOT}/wise_core_v2/radio_lib/hal_drv_radio_weak.c)

add_executable(${PROJECT_NAME}
    ${APP_DIR}/src/main.c
    ${CORE_SRC}
    ${MIDDLEWARE_SRC}
    ${RADIO_GLUE}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${APP_DIR}/config
    ${APP_DIR}/src
    ${WISE_ROOT}/boards
    ${WISE_ROOT}/middleware
    ${WISE_ROOT}/middleware/wise_system
    ${WISE_ROOT}/middleware/retarget
    ${WISE_ROOT}/wise_core_v2
    ${WISE_ROOT}/wise_core_v2/api
    ${WISE_ROOT}/wise_core_v2/core_utils
    ${WISE_ROOT}/wise_core_v2/platform/hal_intf
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/arm_core
    ${WISE_ROOT}/wise_core_v2/radio_lib
    ${WISE_ROOT}/wise_core_v2/radio_lib/${PLATFORM}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    __STACK_SIZE=0x1000
    __HEAP_SIZE=0x4000
    TARGET_APP
    "PLATFORM_COMPONENTS_FILE=\"components_template.h\""
)

target_compile_options(${PROJECT_NAME} PRIVATE -std=gnu11)

# Prebuilt radio library (GCC-built; native for GNU, links under armlink via the
# -fno-short-wchar + _impure_ptr handling already in the armclang setup).
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${WISE_ROOT}/wise_core_v2/prebuilt_libs/libWISERadioLib_${PLATFORM}.a
)

set(BIN "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.bin")

# ---------------------------------------------------------------------------
# Toolchain-specific: linker script, link options, and binary generation.
# ---------------------------------------------------------------------------
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(LINKER_SCRIPT "${APP_DIR}/ldscripts/default_9006.ld")
    target_link_options(${PROJECT_NAME} PRIVATE
        -T "${LINKER_SCRIPT}"
        -Wl,-Map=$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.map
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${PROJECT_NAME}>" "${BIN}"
        COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "objcopy -> ${PROJECT_NAME}.bin"
        VERBATIM
    )

elseif(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
    # ArmClang escalates some GCC-tolerated constructs to errors; match GCC.
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wno-error=implicit-function-declaration
        -Wno-error=int-conversion
        -Wno-error=incompatible-function-pointer-types
    )
    set(LINKER_SCRIPT "${APP_DIR}/keil/er8130_9006.sct")
    target_link_options(${PROJECT_NAME} PRIVATE
        --cpu=Cortex-M3 "--scatter=${LINKER_SCRIPT}" --map --info=summarysizes
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${FROMELF}" --bin --output "${BIN}" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "fromelf -> ${PROJECT_NAME}.bin"
        VERBATIM
    )

else()
    message(FATAL_ERROR "Unsupported compiler '${CMAKE_C_COMPILER_ID}' ??use the arm-gcc or armclang toolchain file.")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")
