#ifndef __UART2_H
#define __UART2_H

#ifdef __cplusplus
extern "C" {
#endif

#define UART2_EV_TX	0x1
#define UART2_EV_RX	0x2

void uart2_init(void);
void uart2_isr(void);
void uart2_sync(void);

void uart2_write(char c);
char uart2_read(void);
int uart2_read_nonblock(void);

#ifdef __cplusplus
}
#endif

#endif
