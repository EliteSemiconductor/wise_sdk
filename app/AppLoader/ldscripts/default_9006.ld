/* ============================================================
 * 
 * HARDWARE CONSTRAINT: DMA can only access first 48KB of RAM
 *
 * ============================================================ */

GROUP(libgcc.a libc.a libm.a libnosys.a)

/* ============================================================
 * Hardware constraints 
 * ============================================================ */
__DMA_REGION_LIMIT = 0x2000C000;  /* 48 KB absolute limit */
__RAM_BASE         = 0x20000000;
__RAM_SIZE         = 0x00010000;  /* 64KB */

/* ============================================================
 * Memory Map
 * ============================================================ */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x2E000  /* 184 KB */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = __RAM_SIZE
}

/* ============================================================
 * Size configuration
 * ============================================================ */
PROVIDE(__STACK_SIZE = DEFINED(__STACK_SIZE) ? __STACK_SIZE : 0x1000);
PROVIDE(__HEAP_SIZE  = DEFINED(__HEAP_SIZE)  ? __HEAP_SIZE  : 0x4000);
__stack_size__ = __STACK_SIZE;
__heap_size__  = __HEAP_SIZE;

ENTRY(Reset_Handler)

/* ============================================================
 * Section Allocation
 * ============================================================ */
SECTIONS
{
    /* ===================== FLASH ===================== */
    .text :
    {
        KEEP(*(.vectors))
        __Vectors_End = .;
        __Vectors_Size = __Vectors_End - __Vectors;

        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /* ===================== RAM ===================== */
    . = ALIGN(4);
    __etext = .;

    /* Initialized data - copied from FLASH to RAM */
    .data : AT (__etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM

    /* BSS - zero initialized */
    .bss :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    /* ===================== DMA Pool ===================== */
    /* 
     * DMA Pool with initialization support
     * - Initialized data: copied from FLASH (like .data)
     * - Uninitialized data: zero-filled (like .bss)
     */
    
    /* Calculate LMA for initialized DMA data */
    __dma_data_lma__ = __etext + (__data_end__ - __data_start__);
    
    /* DMA Pool - Initialized data (copied from FLASH) */
    .dma_data : AT (__dma_data_lma__)
    {
        . = ALIGN(4);
        __dma_data_start__ = .;
        *(.dma_data*)
        . = ALIGN(4);
        __dma_data_end__ = .;
    } > RAM

    /* DMA Pool - Zero initialized (like .bss) */
    .dma_bss (NOLOAD) :
    {
        . = ALIGN(4);
        __dma_bss_start__ = .;
        *(.dma_pool*)
        *(.dma_buffer*)
        . = ALIGN(4);
        __dma_bss_end__ = .;
    } > RAM

    /* DMA pool symbols */
    __dma_pool_start__ = __dma_data_start__;
    __dma_pool_end__   = __dma_bss_end__;
    __dma_size__       = __dma_pool_end__ - __dma_pool_start__;
    __dma_region_end__ = .;

    /* RAM TEXT allocation */
    __RAM_TEXT_FROM__ = __dma_data_lma__ + (__dma_data_end__ - __dma_data_start__);
    .ram_text : AT (__RAM_TEXT_FROM__)
    {
        . = ALIGN(4);
        __RAM_TEXT_START__ = .;
        *(.ram_text*)
        . = ALIGN(16);
        __RAM_TEXT_END__ = .;
    } > RAM

    /* ===================== Heap & Stack ===================== */
    .heap (COPY):
    {
        . = ALIGN(8);
        __HeapBase = .;
        __end__ = .;
        end = __end__;
        KEEP(*(.heap*))
        . = ALIGN(8);
        __HeapLimit = .;
    } > RAM

    .stack_dummy (COPY):
    {
        . = ALIGN(8);
        KEEP(*(.stack*))
        . = ALIGN(8);
    } > RAM

    /* ============================================================
     * Memory Statistics
     * ============================================================ */
    
    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    __StackLimit = __StackTop - SIZEOF(.stack_dummy);
    PROVIDE(__stack = __StackTop);
    
    __data_size__     = __data_end__ - __data_start__;
    __bss_size__      = __bss_end__ - __bss_start__;
    __dma_data_size__ = __dma_data_end__ - __dma_data_start__;
    __dma_bss_size__  = __dma_bss_end__ - __dma_bss_start__;
    __RAM_TEXT_SIZE__ = __RAM_TEXT_END__ - __RAM_TEXT_START__;
    
    __used_static__   = __dma_region_end__ - __data_start__;
    __used_heap__     = __HEAP_SIZE;
    __used_stack__    = __STACK_SIZE;
    __used_total__    = __used_static__ + __used_heap__ + __used_stack__;
    __ram_free__      = __RAM_SIZE - __used_total__;

    /* ============================================================
     * Assertions
     * ============================================================ */
    
    ASSERT(__StackLimit >= __HeapLimit, 
           "FATAL: Stack overlaps heap")
    
    ASSERT(__dma_region_end__ <= __DMA_REGION_LIMIT,
           "FATAL: DMA region exceeds 48KB hardware limit")
    
    ASSERT(__dma_region_end__ <= __HeapBase,
           "FATAL: DMA region overlaps heap")
    
    ASSERT(__used_total__ <= __RAM_SIZE,
           "FATAL: Total memory exceeds RAM size")
    
    ASSERT((__dma_pool_start__ & 0x3) == 0,
           "FATAL: DMA pool not 4-byte aligned")
    
    ASSERT((__StackTop & 0x7) == 0,
           "FATAL: Stack top not 8-byte aligned")
}
