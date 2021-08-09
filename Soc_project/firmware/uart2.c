#include "uart2.h"
#include <irq.h>
#include <generated/csr.h>

void uart2_init(void)
{
	uart2_ev_pending_write(uart2_ev_pending_read());
	uart2_ev_enable_write(UART2_EV_TX | UART2_EV_RX);
//	irq_setmask(irq_getmask() | (1 << UART2_INTERRUPT));
}



void uart2_isr(void)
{

}

char uart2_read(void)
{
/*se bloquea si no hay lectura debe esperar dato de lo contrario el procesador se bloquea*/
	char c;
	while (uart2_rxempty_read());
	c = uart2_rxtx_read();
	uart2_ev_pending_write(UART2_EV_RX);
	return c;
}

int uart2_read_nonblock(void)
{
	return (uart2_rxempty_read() == 0);
}

void uart2_write(char c)
{
	while (uart2_txfull_read());
	uart2_rxtx_write(c);
	uart2_ev_pending_write(UART2_EV_TX);
}


void uart2_sync(void)
{
	while (uart2_txfull_read());
}
