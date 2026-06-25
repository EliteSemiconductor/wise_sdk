# ============================================================================
# WISEDemoApp — CMake build for ER8130 / subg_soc_9006.
#
# All 25 demos as individual CMake targets (the equivalent of the Eclipse build
# configs / Keil workspace projects). Select a compiler with a toolchain file:
#   --toolchain toolchains/arm-gcc.cmake   (Arm GNU)   ← default below
#   --toolchain toolchains/armclang.cmake  (ARM Compiler 6)
#
# All CMake lives in this folder; the SDK source tree is untouched (referenced
# by path). Radio = prebuilt libWISERadioLib_*.a (only the weak stub is built).
#
#   cmake -S . -B build -G Ninja --toolchain toolchains/arm-gcc.cmake \
#         -DARM_GCC_BIN="C:/.../gcc/bin"
#   cmake --build build                       # all demos
#   cmake --build build --target demo_cli     # just one
# ============================================================================
cmake_minimum_required(VERSION 3.20)

# Default optimization = -Os (CMake's MinSizeRel) unless a build type is given.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Build type (default = -Os)" FORCE)
endif()

project(WISEDemoApp C)

get_filename_component(WISE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../.." ABSOLUTE)
set(APP_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(SRC_DIR  "${APP_DIR}/src")
set(PLATFORM subg_soc_9006)
set(RADIO_LIB "${WISE_ROOT}/wise_core_v2/prebuilt_libs/libWISERadioLib_${PLATFORM}.a")
set(WMBUS_LIB "${WISE_ROOT}/protocol/prebuilt_libs/libWMbusDatalink.a")
set(LD_GNU    "${APP_DIR}/ldscripts/default_9006.ld")
set(SCT_ARM   "${APP_DIR}/keil/er8130_9006.sct")

# ---------------------------------------------------------------------------
# Common source set + includes. Compiled PER DEMO (not a shared lib) because
# PLATFORM_COMPONENTS_FILE changes the feature config of these very sources.
# ---------------------------------------------------------------------------
file(GLOB COMMON_SRC CONFIGURE_DEPENDS
    ${WISE_ROOT}/wise_core_v2/api/*.c
    ${WISE_ROOT}/wise_core_v2/core_utils/*.c
    ${WISE_ROOT}/wise_core_v2/platform/hal_intf/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/arm_core/cmsis/esmt/Source/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/drv/*.c
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/hdl/*.c
    ${WISE_ROOT}/middleware/wise_shell_v2/src/*.c
    ${WISE_ROOT}/middleware/wise_shell_v2/intf/*.c
)
list(APPEND COMMON_SRC
    ${WISE_ROOT}/middleware/retarget/retarget.c
    ${WISE_ROOT}/middleware/wise_system/wise_common.c
    ${WISE_ROOT}/middleware/wise_system/wise_event.c
    ${WISE_ROOT}/middleware/wise_system/wise_schlr.c
    ${WISE_ROOT}/wise_core_v2/radio_lib/hal_drv_radio_weak.c   # only radio src not in the .a
    ${SRC_DIR}/app_shell_commands.c
    ${SRC_DIR}/demo_app_common.c
)

set(COMMON_INC
    ${APP_DIR}/config ${SRC_DIR}
    ${WISE_ROOT}/boards
    ${WISE_ROOT}/middleware ${WISE_ROOT}/middleware/wise_system ${WISE_ROOT}/middleware/retarget
    ${WISE_ROOT}/middleware/wise_shell_v2 ${WISE_ROOT}/middleware/wise_shell_v2/src ${WISE_ROOT}/middleware/wise_shell_v2/intf
    ${WISE_ROOT}/wise_core_v2 ${WISE_ROOT}/wise_core_v2/api ${WISE_ROOT}/wise_core_v2/core_utils
    ${WISE_ROOT}/wise_core_v2/platform/hal_intf
    ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM} ${WISE_ROOT}/wise_core_v2/platform/${PLATFORM}/arm_core
    ${WISE_ROOT}/wise_core_v2/radio_lib ${WISE_ROOT}/wise_core_v2/radio_lib/${PLATFORM}
)
# every demo folder on the include path (demos include their own headers)
file(GLOB _demo_dirs LIST_DIRECTORIES true ${SRC_DIR}/demo_*)
foreach(_d ${_demo_dirs})
    if(IS_DIRECTORY ${_d})
        list(APPEND COMMON_INC ${_d})
    endif()
endforeach()

# ---------------------------------------------------------------------------
# mbedTLS — config-independent (uses config-esmt.h via build_info.h), so build
# it ONCE as a static lib and link it into the W-MBus demos. EXCLUDE_FROM_ALL =>
# only compiled when a W-MBus demo is actually built.
# ---------------------------------------------------------------------------
file(GLOB MBEDTLS_SRC CONFIGURE_DEPENDS ${WISE_ROOT}/third_party/mbedtls/library/*.c)
add_library(mbedtls STATIC EXCLUDE_FROM_ALL ${MBEDTLS_SRC})
target_include_directories(mbedtls PUBLIC
    ${WISE_ROOT}/third_party/mbedtls/include
    ${WISE_ROOT}/third_party/mbedtls/configs)

# ---------------------------------------------------------------------------
# Helpers.
# ---------------------------------------------------------------------------
function(wise_leniency tgt)
    # ArmClang escalates some GCC-tolerated constructs to errors; match GCC.
    if(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
        target_compile_options(${tgt} PRIVATE
            -Wno-error=implicit-function-declaration
            -Wno-error=int-conversion
            -Wno-error=incompatible-function-pointer-types)
    endif()
endfunction()
wise_leniency(mbedtls)

function(wise_link_and_bin tgt)
    set(_bin "$<TARGET_FILE_DIR:${tgt}>/${tgt}.bin")
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_link_options(${tgt} PRIVATE
            -T "${LD_GNU}" -Wl,-Map=$<TARGET_FILE_DIR:${tgt}>/${tgt}.map)
        set_target_properties(${tgt} PROPERTIES LINK_DEPENDS "${LD_GNU}")
        add_custom_command(TARGET ${tgt} POST_BUILD
            COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${tgt}>" "${_bin}"
            COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${tgt}>" VERBATIM)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
        target_link_options(${tgt} PRIVATE
            --cpu=Cortex-M3 "--scatter=${SCT_ARM}" --map --info=summarysizes)
        set_target_properties(${tgt} PROPERTIES LINK_DEPENDS "${SCT_ARM}")
        add_custom_command(TARGET ${tgt} POST_BUILD
            COMMAND "${FROMELF}" --bin --output "${_bin}" "$<TARGET_FILE:${tgt}>" VERBATIM)
    else()
        message(FATAL_ERROR "Use the arm-gcc or armclang toolchain file (got '${CMAKE_C_COMPILER_ID}').")
    endif()
endfunction()

# add_wise_demo(NAME CONFIG <hdr> STACK <0xNNN> SRCDIR <folder> TIER <simple|loader|timerhub|rtos|wmbus> [DEFINES ...])
function(add_wise_demo NAME)
    cmake_parse_arguments(D "" "CONFIG;STACK;SRCDIR;TIER" "DEFINES" ${ARGN})

    file(GLOB _demo_src CONFIGURE_DEPENDS ${SRC_DIR}/${D_SRCDIR}/*.c)
    set(_extra_src "")
    set(_extra_inc "")
    set(_extra_lib "")

    if(D_TIER STREQUAL "loader")
        # kermit pulls in the flash filesystem, which uses nvm.
        file(GLOB _s
            ${WISE_ROOT}/middleware/wise_kermit/*.c
            ${WISE_ROOT}/middleware/wise_flash_filesystem/*.c
            ${WISE_ROOT}/middleware/wise_nvm/*.c)
        list(APPEND _extra_src ${_s})
        list(APPEND _extra_inc
            ${WISE_ROOT}/middleware/wise_kermit
            ${WISE_ROOT}/middleware/wise_flash_filesystem
            ${WISE_ROOT}/middleware/wise_nvm)
    elseif(D_TIER STREQUAL "timerhub")
        file(GLOB _s ${WISE_ROOT}/middleware/wise_timer_hub/*.c)
        list(APPEND _extra_src ${_s})
        list(APPEND _extra_inc ${WISE_ROOT}/middleware/wise_timer_hub)
    elseif(D_TIER STREQUAL "rtos")
        file(GLOB _s ${WISE_ROOT}/third_party/FreeRTOS-Kernel/*.c)
        list(APPEND _extra_src ${_s}
            ${WISE_ROOT}/third_party/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c
            ${WISE_ROOT}/third_party/FreeRTOS-Kernel/portable/MemMang/heap_4.c
            ${WISE_ROOT}/third_party/FreeRTOS-Kernel/platform/tick_control.c)
        list(APPEND _extra_inc
            ${WISE_ROOT}/third_party/FreeRTOS-Kernel/include
            ${WISE_ROOT}/third_party/FreeRTOS-Kernel/portable/GCC/ARM_CM3)
    elseif(D_TIER STREQUAL "wmbus")
        file(GLOB _s
            ${WISE_ROOT}/middleware/wise_wmbus_crypto/*.c
            ${WISE_ROOT}/middleware/wise_kermit/*.c
            ${WISE_ROOT}/middleware/wise_flash_filesystem/*.c
            ${WISE_ROOT}/middleware/wise_nvm/*.c
            ${WISE_ROOT}/protocol/wmbus_datalink/*.c
            ${WISE_ROOT}/protocol/wmbus_datalink/platform_intf/*.c)
        list(APPEND _extra_src ${_s})
        list(APPEND _extra_inc
            ${WISE_ROOT}/middleware/wise_wmbus_crypto ${WISE_ROOT}/middleware/wise_kermit
            ${WISE_ROOT}/middleware/wise_flash_filesystem ${WISE_ROOT}/middleware/wise_nvm
            ${WISE_ROOT}/protocol/wmbus_datalink ${WISE_ROOT}/protocol/wmbus_datalink/platform_intf
            ${WISE_ROOT}/third_party/mbedtls/include ${WISE_ROOT}/third_party/mbedtls/configs)
        list(APPEND _extra_lib mbedtls "${WMBUS_LIB}")
    endif()

    add_executable(${NAME} ${COMMON_SRC} ${_demo_src} ${_extra_src})
    # Each demo's final artifacts (.elf/.axf, .bin, .map) go in their own folder.
    set_target_properties(${NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out/${NAME}")
    target_include_directories(${NAME} PRIVATE ${COMMON_INC} ${_extra_inc})
    target_compile_definitions(${NAME} PRIVATE
        __STACK_SIZE=${D_STACK} __HEAP_SIZE=0x4000 TARGET_APP
        "PLATFORM_COMPONENTS_FILE=\"${D_CONFIG}\""
        ${D_DEFINES})
    target_compile_options(${NAME} PRIVATE -std=gnu11)
    target_link_libraries(${NAME} PRIVATE "${RADIO_LIB}" ${_extra_lib})
    wise_leniency(${NAME})
    wise_link_and_bin(${NAME})
endfunction()

# ---------------------------------------------------------------------------
# The 25 demos (= Eclipse build configs).  DEMO_APP_NAME is now defined in each
# demo_xxx_main.c, so it is NOT passed here.
# ---------------------------------------------------------------------------
add_wise_demo(demo_cli         CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_cli        TIER simple)
add_wise_demo(demo_gpio        CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_gpio       TIER simple)
add_wise_demo(demo_spi         CONFIG components_demo_simple.h    STACK 0x800  SRCDIR demo_spi        TIER simple)
add_wise_demo(demo_i2c         CONFIG components_demo_simple.h    STACK 0x800  SRCDIR demo_i2c        TIER simple)
add_wise_demo(demo_pwm         CONFIG components_demo_simple.h    STACK 0x800  SRCDIR demo_pwm        TIER simple)
add_wise_demo(demo_pwmslow     CONFIG components_demo_simple.h    STACK 0x800  SRCDIR demo_pwmslow    TIER simple)
add_wise_demo(demo_rtc         CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_rtc        TIER simple)
add_wise_demo(demo_wutmr       CONFIG components_demo_wutmr.h     STACK 0x800  SRCDIR demo_wutmr      TIER simple)
add_wise_demo(demo_gptmr       CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_gptmr      TIER simple)
add_wise_demo(demo_wdt         CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_wdt        TIER simple)
add_wise_demo(demo_crypto      CONFIG components_demo_crypto.h    STACK 0x800  SRCDIR demo_crypto     TIER simple)
add_wise_demo(demo_trng        CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_trng       TIER simple)
add_wise_demo(demo_radio_trx   CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_radio_trx  TIER simple)
add_wise_demo(demo_power_mode  CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_power_mode TIER simple)
add_wise_demo(demo_template    CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_template   TIER simple)
add_wise_demo(demo_boot_loader CONFIG components_demo_loader.h    STACK 0x800  SRCDIR demo_boot_loader TIER loader)
add_wise_demo(demo_nfc         CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_nfc        TIER simple)
add_wise_demo(demo_flash       CONFIG components_demo_app.h       STACK 0x800  SRCDIR demo_flash      TIER simple)
add_wise_demo(demo_timer_hub   CONFIG components_demo_timer_hub.h STACK 0x800  SRCDIR demo_timer_hub  TIER timerhub)
add_wise_demo(demo_freeRTOS    CONFIG components_demo_rtos.h      STACK 0x800  SRCDIR demo_freeRTOS   TIER rtos)
add_wise_demo(demo_wmbus_phy_meter       CONFIG components_demo_wmbus.h STACK 0x800  SRCDIR demo_wmbus      TIER wmbus DEFINES WISE_WMBUS_SW_HELPER WISE_LOG_LEVEL=2 WMBUS_DEMO_PHY_METER)
add_wise_demo(demo_wmbus_phy_other       CONFIG components_demo_wmbus.h STACK 0x800  SRCDIR demo_wmbus      TIER wmbus DEFINES WISE_WMBUS_SW_HELPER WISE_LOG_LEVEL=2 WMBUS_DEMO_PHY_OTHER)
add_wise_demo(demo_wmbus_link_meter      CONFIG components_demo_wmbus.h STACK 0x1000 SRCDIR demo_wmbus_link TIER wmbus DEFINES WISE_WMBUS_SW_HELPER WISE_LOG_LEVEL=2 WMBUS_DEMO_LINK_METER)
add_wise_demo(demo_wmbus_link_other      CONFIG components_demo_wmbus.h STACK 0x800  SRCDIR demo_wmbus_link TIER wmbus DEFINES WISE_WMBUS_SW_HELPER WISE_LOG_LEVEL=2 WMBUS_DEMO_LINK_OTHER)
add_wise_demo(demo_wmbus_phy_meter_sleep CONFIG components_demo_wmbus.h STACK 0x800  SRCDIR demo_wmbus      TIER wmbus DEFINES WISE_WMBUS_SW_HELPER WISE_LOG_LEVEL=2 WMBUS_DEMO_PHY_METER_PD)
