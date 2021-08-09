#include "uart3.h"
#include <irq.h>
#include <generated/csr.h>

void uart3_init(void)
{
	uart3_ev_pending_write(uart3_ev_pending_read());
	uart3_ev_enable_write(UART3_EV_TX | UART3_EV_RX);
//	irq_setmask(irq_getmask() | (1 << UART3_INTERRUPT));
}



void uart3_isr(void)
{

}

char uart3_read(void)
{
/*se bloquea si no hay lectura debe esperar dato de lo contrario el procesador se bloquea*/
	char c;
	while (uart3_rxempty_read());
	c = uart3_rxtx_read();
	uart3_ev_pending_write(UART3_EV_RX);
	return c;
}

int uart3_read_nonblock(void)
{
	return (uart3_rxempty_read() == 0);
}

void uart3_write(char c)
{
	while (uart3_txfull_read());
	uart3_rxtx_write(c);
	uart3_ev_pending_write(UART3_EV_TX);
}


void uart3_sync(void)
{
	while (uart3_txfull_read());
}
