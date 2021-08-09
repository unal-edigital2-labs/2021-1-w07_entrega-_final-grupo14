#ifndef __UART3_H
#define __UART3_H

#ifdef __cplusplus
extern "C" {
#endif

#define UART3_EV_TX	0x1
#define UART3_EV_RX	0x2

void uart3_init(void);
void uart3_isr(void);
void uart3_sync(void);

void uart3_write(char c);
char uart3_read(void);
int uart3_read_nonblock(void);

#ifdef __cplusplus
}
#endif

#endif
