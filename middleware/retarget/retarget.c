/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */
/* POSIX/newlib headers are only available (and only needed) for the GNU build;
 * they are included in the GNU branch below. The ARM Compiler library has no
 * <sys/stat.h> etc. */
#include "wise_uart_api.h"

int stdioUartPort = -1;

void retarget_set_port(int uartPort)
{
    stdioUartPort = uartPort;
}

static void SendChar(char c)
{
    if(stdioUartPort < 0)
        return;
        
    if (c == '\n') {
        wise_uart_write_char(stdioUartPort, '\r');
        wise_uart_write_char(stdioUartPort, '\n');
    } else {
        wise_uart_write_char(stdioUartPort, c);
    }
}


#if defined(__ARMCC_VERSION)
/* ============================================================================
 * ARM Compiler (AC5 armcc / AC6 armclang) C-library retargeting.
 * The ARM C library routes stdio through fputc()/__stdout rather than the
 * newlib _write/_sbrk syscalls below. Semihosting is disabled so the image
 * runs standalone without a debugger attached. The heap and stack are owned
 * by the scatter file's ARM_LIB_HEAP / ARM_LIB_STACK regions, so no _sbrk is
 * needed here.
 * (armclang also defines __GNUC__, so this __ARMCC_VERSION block must precede
 *  the GNU branch below.)
 * ========================================================================== */
#include <stdio.h>

#if (__ARMCC_VERSION >= 6000000)
/* AC6 / armclang: disable semihosting via a global symbol reference. */
__asm(".global __use_no_semihosting");
#else
/* AC5 / armcc */
#pragma import(__use_no_semihosting)
#endif

/* The AC6/armclang standard C library already provides __stdout / __stdin
 * (in stdio_streams.o), so defining them here triggers L6200E "multiply
 * defined". Only AC5's library needs the application to supply them. */
#if (__ARMCC_VERSION < 6000000)
struct __FILE {
    int handle;
};
FILE __stdout;
FILE __stdin;
#endif

int fputc(int ch, FILE *f)
{
    (void)f;
    SendChar((char)ch);
    return ch;
}

void _ttywrch(int ch)
{
    SendChar((char)ch);
}

void _sys_exit(int return_code)
{
    (void)return_code;
    while (1)
        ;
}

/* Required by some ARM C library builds when semihosting is disabled. */
char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;
    return NULL;
}

/* ----------------------------------------------------------------------------
 * Low-level device retarget. With __use_no_semihosting the C library still
 * references these _sys_* entry points for stdio stream setup, so they must be
 * supplied. Console output is funnelled to the UART; everything else is a stub.
 * (FILEHANDLE is 'int' in the ARM C library's rt_sys.h.)
 * -------------------------------------------------------------------------- */
int _sys_open(const char *name, int openmode)
{
    (void)name;
    (void)openmode;
    return 1; /* non-zero, non-(-1) dummy handle */
}

int _sys_close(int fh)
{
    (void)fh;
    return 0;
}

int _sys_write(int fh, const unsigned char *buf, unsigned len, int mode)
{
    (void)fh;
    (void)mode;
    for (unsigned i = 0; i < len; i++) {
        SendChar((char)buf[i]);
    }
    return 0; /* 0 = all bytes written */
}

int _sys_read(int fh, unsigned char *buf, unsigned len, int mode)
{
    (void)fh;
    (void)buf;
    (void)len;
    (void)mode;
    return -1; /* EOF */
}

int _sys_istty(int fh)
{
    (void)fh;
    return 1; /* treat the handle as an interactive device */
}

int _sys_seek(int fh, long pos)
{
    (void)fh;
    (void)pos;
    return -1;
}

int _sys_ensure(int fh)
{
    (void)fh;
    return 0;
}

long _sys_flen(int fh)
{
    (void)fh;
    return 0;
}

/* ----------------------------------------------------------------------------
 * newlib reentrancy pointer.
 * The prebuilt radio library (libWISERadioLib_*.a) is built with GCC/newlib and
 * references _impure_ptr (newlib's per-thread reentrancy structure, used for
 * errno/stdio). The ARM C library does not provide it, so supply a zero-init
 * backing store to satisfy the linker. The buffer is sized to cover newlib's
 * struct _reent; radio is disabled in this build, so it is not exercised at
 * runtime. (Type is void* — only the symbol name matters to the linker.)
 * -------------------------------------------------------------------------- */
static unsigned long _wise_impure_data[256]; /* 1 KB, zero-initialised */
void *_impure_ptr = _wise_impure_data;

#else /* !__ARMCC_VERSION : GNU newlib syscalls */

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>

int __io_putchar(int c)
{
    SendChar((char)c);
    return c;
}

int _write(int file, char *ptr, int len)
{
    (void)file;

    for (int i = 0; i < len; i++) {
        SendChar(ptr[i]);
    }

    return len;
}

// Placeholder for getpid function
pid_t _getpid(void)
{
    return 1; // return a dummy PID
}

// Placeholder for kill function
int _kill(pid_t pid, int sig)
{
    errno = EINVAL;
    return -1; // return error
}

// Placeholder for close function
int _close(int file)
{
    errno = EBADF;
    return -1;
}

// Placeholder for fstat function
__attribute__((used)) int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

// Placeholder for isatty function
__attribute__((used)) int _isatty(int file)
{
    return 1;
}

// Placeholder for lseek function
off_t _lseek(int file, off_t offset, int whence)
{
    errno = ESPIPE;
    return -1;
}

// Placeholder for read function
ssize_t _read(int file, void *ptr, size_t len)
{
    errno = EBADF;
    return -1;
}

//kevinyang, 20251121, override _sbrk to check __HeapLimit when malloc/calloc is called
extern char __HeapBase;
extern char __HeapLimit;
void * _sbrk(ptrdiff_t incr)
{
    static char *cur = NULL;
    char *prev;
    char *next;

    if (cur == NULL) {
        cur = &__HeapBase;
    }

    prev = cur;
    next = cur + incr;

    if (next > &__HeapLimit) {
        errno = ENOMEM;
        return (void *) -1;
    }

    cur = next;
    return prev;
}

#endif /* __ARMCC_VERSION */

