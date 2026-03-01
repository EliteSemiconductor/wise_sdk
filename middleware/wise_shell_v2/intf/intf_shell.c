#include "intf_shell.h"
#include "api/wise_uart_api.h"
#include <stdint.h>
#include <stdbool.h>

#define SHELL_UART_CH 0

static bool intf_shell_uart_read_char(char *ch)
{
    uint8_t tmp;
    if (wise_uart_read_char(SHELL_UART_CH, &tmp) == WISE_SUCCESS) {
        *ch = (char)tmp;
        return true;
    }
    return false;
}

static void intf_shell_uart_write_str(const char *s)
{
    while (*s) {
        wise_uart_write_char(SHELL_UART_CH, (uint8_t)*s++);
    }
}

__attribute__((weak)) void intf_shell_init(void)
{
    shell_config_t cfg = {
        .read_char = intf_shell_uart_read_char,
        .write_str = intf_shell_uart_write_str,
        .prompt    = "ESMT> ",
    };

    shell_init(&cfg);
}
__attribute__((weak)) void intf_shell_task(void)
{
    shell_task();
}
