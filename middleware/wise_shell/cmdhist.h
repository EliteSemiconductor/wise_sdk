/*
 * Copyright (C) 2025 Elite Semiconductor Microelectronics Technology Inc
 * All rights reserved.
 *
 */

#ifndef __CMDHIST_H
#define __CMDHIST_H

void cmdhist_init();
void cmdhist_add(const char *buf);
int cmdhist_prev(int n, char *buf);
int cmdhist_next(int n, char *buf);

#endif //__CMDHIST_H
