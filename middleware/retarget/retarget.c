/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>

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

