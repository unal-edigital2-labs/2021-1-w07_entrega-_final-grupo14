#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <uart.h>
#include <console.h>
#include <generated/csr.h>

#include "delay.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"


# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))

unsigned int matriz[6][7];
unsigned int color[3];


static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}
	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("RUNTIME>");
}

static void help(void)
{
	puts("Available commands:");
	puts("help                            - this command");
	puts("reboot                          - reboot CPU");
	puts("led                             - led test");
	puts("switch                          - switch test");
	puts("display                         - display test");
	puts("rgbled                          - rgb led test");
	puts("vga                             - vga test");
	puts("camara                          - camara test");
	puts("pwm			       - pwm test");
	puts("ultra			       - ultra test");
	puts("uart1			       - uart1 test");
	puts("motor			       - motor test");
	puts("infra			       - infra test");
	puts("camara			       - camara test");
	puts("dfp			       - dfp test");
}

static void reboot(void)
{
	ctrl_reset_write(1);
}


static void led_test(void)
{
	unsigned int i;
	printf("Test del los leds... se interrumpe con el botton 1\n");
	while(!(buttons_in_read()&1)) {

	for(i=1; i<65536; i=i*2) {
		leds_out_write(i);
		delay_ms(50);
	}
	for(i=32768;i>1; i=i/2) {
		leds_out_write(i);
		delay_ms(50);
	}
	}
	
}



static void rgbled_test(void)
{
	unsigned int T = 128;
	
	ledRGB_1_r_period_write(T);
	ledRGB_1_g_period_write(T);
	ledRGB_1_b_period_write(T);

	ledRGB_1_r_enable_write(1);
	ledRGB_1_g_enable_write(1);
	ledRGB_1_b_enable_write(1);

	
	ledRGB_2_r_period_write(T);
	ledRGB_2_g_period_write(T);
	ledRGB_2_b_period_write(T);
	
	
	ledRGB_2_r_enable_write(1);
	ledRGB_2_g_enable_write(1);
	ledRGB_2_b_enable_write(1);

	for (unsigned int j=0; j<100; j++){
		ledRGB_1_g_width_write(j);
		for (unsigned int i=0; i<100; i++){
			ledRGB_1_r_width_write(100-i);
			ledRGB_1_b_width_write(i);	
			delay_ms(20);
		}	
	}
	


}

static void pwm_test(void)
{
	unsigned int period = 2000000;
	unsigned int dutty = 0;
	unsigned int enable = 1;
	
	pwm_cntrl_enable_write(enable);
	printf("Test de pwm ... se interrumpe con el boton 1\n");
	
	delay_ms(10);


	dutty = 40000;
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(1000);
	
	dutty = 145000;
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(1000);
	
	dutty = 250000;
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(1000);
	
	
}


static void ultra_test(void)
{

	unsigned int orden = 0;
	unsigned int distancia = 0;
	unsigned int done = 0; 
	
	ultra_cntrl_orden_write(orden);
	printf("Test de ultrasonido ... se interrumpe con el boton 1\n");
	
	delay_ms(10);
	
	
	while(!(buttons_in_read()&1)){
	
	
	orden = 0;
	ultra_cntrl_orden_write(orden);
	
	delay_ms(1000);
	orden = 1;
	ultra_cntrl_orden_write(orden);
	delay_ms(1000);
	
	distancia = ultra_cntrl_d_read();
	printf("distancia : %d \n",2*distancia);
	done = ultra_cntrl_DONE_read();
	printf("done : %d \n",done);
	
	delay_ms(1000);
	
	}



}

static void enviarM(void){

	for(int i = 0; i < 6; i++) { 	
		for(int j = 0; j < 7; j++) {
			switch(matriz[i][j]){ 		//azul=1, verde=2, rojo=3
				case 1:
				uart1_write(65);
				delay_ms(10);
				break;
				case 2:
				uart1_write(86);
				delay_ms(10);
				break;
				case 3:
				uart1_write(82);
				delay_ms(10);
				break;
				default:
				uart1_write(matriz[i][j]);
				delay_ms(10);
				break;
			}
		}
		
		uart1_write(10);
		delay_ms(10);
    	}
	uart1_write(10);
	delay_ms(10);
	uart1_write(10);
	delay_ms(10);
}


static void uart1_test(void){
	printf("Test de la uart 1, debe poner en corto el pon RX Tx de la la UART1.\n");
	
	printf("se envia el caracter A por la uart 1  y al estar en loopback se recibe el caracter  y se retrasmite por la uart 0\n");
	printf("se interrumpe con el botton 1\n");
	unsigned int matriz[2][2]={{1,2},{3,4}};
	unsigned int dato = 0;
	while(!(buttons_in_read()&1)) {
	
	dato = uart1_read();
	if(dato == 97)
	printf("listo\n");
	else printf("no listo\n");
	delay_ms(500);
	}
}


void DFP_setup(void){

  //myDFPlayer.volume(25);
  // 7E FF 06 06 XX 00 19 XX EF 
  sendInfo(0x06,0x00,0x15);
   
  //myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  // 7E FF 06 07 XX 00 00 XX EF 
  sendInfo(0x07,0x00,0x00);
  
  //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  // 7E FF 06 09 XX 00 02 XX EF 
  sendInfo(0x09,0x00,0x02);

}

void sendInfo( unsigned int CMD, unsigned int param1, unsigned int param2){


  unsigned int checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + param1 + param2);
  unsigned int  Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
  param1, param2, HIGHBYTE(checksum), LOWBYTE(checksum), End_Byte};
  
    for(int i=0; i<10; i++){
    uart2_write(Command_line[i]);
    }
}

static void DFP_test(void){

  DFP_setup();
  
  
   //myDFPlayer.play(1);
   // 7E FF 06 03 XX 00 01 XX EF 
   //sendInfo(0x03,0x00,0x01);
   //delay_ms(2000);
   //myDFPlayer.stop();
   // 7E FF 06 16 XX 00 00 XX EF 
   //sendInfo(0x16,0x00,0x00);
   //myDFPlayer.play(2);
   // 7E FF 06 03 XX 00 02 XX EF 
   //sendInfo(0x03,0x00,0x02);
   //delay_ms(2000);

   sendInfo(0x16,0x00,0x00);
   //myDFPlayer.play(2);
   // 7E FF 06 03 XX 00 02 XX EF 
   sendInfo(0x03,0x00,0x03);
   delay_ms(2000);

   sendInfo(0x16,0x00,0x00);
   //myDFPlayer.play(2);
   // 7E FF 06 03 XX 00 02 XX EF 
   sendInfo(0x03,0x00,0x04);
   delay_ms(2000);

   sendInfo(0x16,0x00,0x00);
   //myDFPlayer.play(2);
   // 7E FF 06 03 XX 00 02 XX EF 
   sendInfo(0x03,0x00,0x05);
   delay_ms(2000);



}




static void motor_test(void){

	printf("Test de motor ... se interrumpe con el boton 1\n");
	
	unsigned int estado = 0x2;
	delay_ms(50);
	
	estado = 0x2;
	motor_cntrl_estado_write(estado);
	delay_ms(800);
	estado = 0x0;
	motor_cntrl_estado_write(estado);
	delay_ms(500);
		




}

static void infra_test(void){

	printf("Test de infra ... se interrumpe con el boton 1\n");

	while(!(buttons_in_read()&1)) {
		printf("Infrarrojo: %x \n",infra_cntrl_infras2_read());
		delay_ms(1000);
		}

}


int camara(void){

	unsigned int col = 0;
	unsigned int done = 0;
	unsigned int error = 0;
	
	printf("Test de camara ... se interrumpe con el boton 1\n");

		col = camara_cntrl_res_read(); 
		done = camara_cntrl_done_read();
		error = camara_cntrl_error_read();
		if(done){
			if(!error){
				switch (col){
	 				case 1: printf("Azul \n"); break;
	 				case 2: printf("Verde \n"); break;
	 				case 4: printf("Rojo \n"); break;
	 				case 7: printf("Ninguno \n"); break;
				}
			}
		}
	camara_cntrl_init_write(1);
	delay_ms(10);
	camara_cntrl_init_write(0);
	delay_ms(1000);
	
	return col;
	
}

static void camara_test(){
unsigned int c = camara();
delay_ms(10);
uart3_write(c);

}

int radar(void){

	unsigned int orden = 0;
	unsigned int distancia = 0;
	unsigned int limite_distancia = 30;
	unsigned int done = 0; 
	unsigned int period = 2000000;
	unsigned int dutty = 0;
	unsigned int enable = 1;
	unsigned int posicion =0;
        unsigned int cam=7;
	
	pwm_cntrl_enable_write(enable);
	orden = 0;
	ultra_cntrl_orden_write(orden);

	delay_ms(10);

	// Izquierda 
	dutty = 250000; // Servo gira a la izquierda
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(2000);
	orden = 1;
	ultra_cntrl_orden_write(orden);
	delay_ms(100);
	distancia = 2*ultra_cntrl_d_read(); // Se obtiene el valor de la distancia al objeto
	if (distancia<limite_distancia){
		delay_ms(1000);
		for(int i=0; i<4; i++)
		cam=camara();	
        	posicion += 0x1; 
		switch(cam){
			case 1: 
				color[0]=1;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x04);
			        delay_ms(2000);
				break; 
			case 2: 
                		color[0]=2;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x03);
			        delay_ms(2000);
			        
				break;
			case 4: 
				color[0]=3;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x05);
			        delay_ms(2000);
			        
				break;
		}
	} else {
		posicion+=0x0;
	}
	delay_ms(100);
	orden = 0;
	ultra_cntrl_orden_write(orden);


	/* Al frente */
	dutty = 145000; /* Servo gira al frente*/
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(1000);
	orden = 1;
	ultra_cntrl_orden_write(orden);
	delay_ms(100);
	distancia = 2*ultra_cntrl_d_read(); /* Se obtiene el valor de la distancia al objeto*/
	if (distancia<limite_distancia){
		delay_ms(1000);
		for(int i=0; i<4; i++)
		cam=camara();
        	posicion += 0x2;
		switch(cam){
			case 1: 
                		color[1]=1; 
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x04);
			        delay_ms(2000);
			        
				break; 
			case 2: 
				color[1]=2;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x03);
			        delay_ms(2000);
			        
				break;
			case 4: 
				sendInfo(0x16,0x00,0x00);
				color[1]=3; 
				sendInfo(0x03,0x00,0x05);
			        delay_ms(2000);
			        
				break;
	}} else {
		posicion+=0x0;
	}
	delay_ms(100);
	orden = 0;
	ultra_cntrl_orden_write(orden);


	/* Derecha */
	dutty = 40000; /* Servo gira a la derecha*/
	pwm_cntrl_period_write(period);
	pwm_cntrl_dutty_write(dutty);
	delay_ms(1000);
	orden = 1;
	ultra_cntrl_orden_write(orden);
	delay_ms(100);
	distancia = 2*ultra_cntrl_d_read(); /* Se obtiene el valor de la distancia al objeto*/
	if (distancia<limite_distancia){
		delay_ms(1000);
		for(int i=0; i<4; i++)
		cam=camara();
        	posicion +=0x4;
		switch(cam){
			case 1: 
				color[2]=1;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x04);
			        delay_ms(2000);
			        
				break; 
			case 2: 
				color[2]=2; 
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x03);
			        delay_ms(2000);
			        
				break;
			case 4: 
				color[2]=3;
				sendInfo(0x16,0x00,0x00);
				sendInfo(0x03,0x00,0x05);
			        delay_ms(2000);
			        
				break;
	}} else {
		posicion +=0x0;
	}
	delay_ms(100);
	orden = 0;
	ultra_cntrl_orden_write(orden);


	return posicion;

}

static void radar_test(){

	unsigned int r = radar();
	delay_ms(10);
	uart3_write(r);



}



static void avanzar(void){

	unsigned int estado = 0x0;
	unsigned int infra = 0;
	delay_ms(50);
	
	while(infra!=3){
	infra = infrarrojo();
		
		switch (infra){
			 case 0:
			      estado = 0x2;
			      motor_cntrl_estado_write(estado);
				delay_ms(50);
			      break;
			 case 2:
			      estado = 0x4;
			      motor_cntrl_estado_write(estado);
				delay_ms(50);
			      break;
			 case 1:
			      estado = 0x3;
			      motor_cntrl_estado_write(estado);
				delay_ms(50);
			      break;
			  default:
			      estado = 0x0;
			      motor_cntrl_estado_write(estado);
				delay_ms(50);
			      break;
      		}
	}
	
	if(infra==3){
		estado = 0x0;
		motor_cntrl_estado_write(estado);
		delay_ms(50);
	}
}


static void girarD(void){
	
	unsigned int tiempo = 900;
	unsigned int estado = 0x0;
	delay_ms(50);
	
	estado = 0x3;
	motor_cntrl_estado_write(estado);
	delay_ms(tiempo);
	estado = 0x0;
	motor_cntrl_estado_write(estado);
	delay_ms(50);
}

static void girarI(void){
	
	unsigned int tiempo = 900;
	unsigned int estado = 0x0;
	delay_ms(50);
	
	estado = 0x4;
	motor_cntrl_estado_write(estado);
	delay_ms(tiempo);
	estado = 0x0;
	motor_cntrl_estado_write(estado);
	delay_ms(50);
}


int infrarrojo(void){
   
	unsigned int entrada = infra_cntrl_infras2_read();
	unsigned int salida=0;
	
	if(entrada == 0x1f){
		salida = 0;
	        printf("Siga: %x \n",infra_cntrl_infras2_read());
	        //delay_ms(1000);
	}
	if(entrada == 0x17){
	        salida = 1;
	        printf("Derecha: %x \n",infra_cntrl_infras2_read());
	        //delay_ms(1000);
	}
	if(entrada == 0x1d){
	        salida = 2;
	        printf("izquierda: %x \n",infra_cntrl_infras2_read());
	        //delay_ms(1000);
	}
	if(entrada == 0x00){
	        salida = 3;
	        printf("Pare: %x \n",infra_cntrl_infras2_read());
		//delay_ms(1000);
	}
	
	return salida;

}




static void carro(void){
	
	unsigned int radar1=0x5;
	unsigned int posX=3;
	unsigned int posY=5;
	unsigned int dir=0; // 0 arriba, 1 derecha, 2 abajo
	
	DFP_setup(); // Se establecen los valores del DFP
	
	for(int i = 0; i < 6; i++) { 		// Rellenar la matriz de 0
		for(int j = 0; j < 7; j++) {
		matriz[i][j] = 80;
		}
    	}
	
	sendInfo(0x16,0x00,0x00);
	sendInfo(0x03,0x00,0x02);
        delay_ms(2000);
        
	
	 // De la camara azul:1  verde:2   rojo:4   nada:7
		
		//radar1 = radar();
		//delay_ms(2000);
		avanzar();
		delay_ms(2000);

		while(radar1!=0x7){
		
		radar1 = radar();
		
			switch (radar1){
			
			// Lados ocupado, frente vacio
            		case 0x5:
			     
					if(dir==0){
					      matriz[posY][posX-1]=color[0]; 	  
					      matriz[posY][posX+1]=color[2];
					      matriz[posY][posX]=43;
					      matriz[posY-1][posX]=43;
					      matriz[posY-2][posX]=43;
					      posY-=2;
					}		 	     
					if(dir==1){
					      matriz[posY+1][posX]=color[0];
					      matriz[posY-1][posX]=color[2];
					      matriz[posY][posX]=43;
					      matriz[posY][posX-1]=43;
					      matriz[posY][posX-2]=43;
					      posX-=2;
					}	
                			
				
					
				avanzar();
			      	delay_ms(2000);
			      	enviarM();	
            		break;
            		
			// Frente Izq ocupados, derecha vacio				
			case 0x3:
                		
			      	
				      	if(dir==1){
					      matriz[posY+1][posX]=color[0];
					      matriz[posY][posX-1]=color[1];
					      matriz[posY][posX]=43;
					      matriz[posY-1][posX]=43;
					      matriz[posY-2][posX]=43;
					      posY-=2;
				      	}
			      	dir-=1;	
                		
		        		
		        	girarD();
			      	delay_ms(5000);
			      	avanzar();
			      	delay_ms(2000);
			      	enviarM();
			break;
                  
			// Frente derecha ocupados, izquierda vacio								
			case 0x6: 
                		      
				      if(dir==0){
					      matriz[posY-1][posX]=color[1];
					      matriz[posY][posX+1]=color[2];
					      matriz[posY][posX]=43;
					      matriz[posY][posX-1]=43;
					      matriz[posY][posX-2]=43;
					      posX-=2;
				      }
				dir+=1;
				
				
				girarI();
			      	delay_ms(5000);
			      	avanzar();
			      	delay_ms(2000);
			     	enviarM();
			break;	
					
			default:
			    matriz[posY][posX-1]=color[0];
			    matriz[posY+1][posX]=color[1];
			    matriz[posY][posX+1]=color[2];
			    delay_ms(100);
			    enviarM();
			break;
      		}
      		
		}
	sendInfo(0x16,0x00,0x00);
	sendInfo(0x03,0x00,0x01);
   	delay_ms(2000);
   	
	

}



static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot();
	else if(strcmp(token, "led") == 0)
		led_test();
	else if(strcmp(token, "rgbled") == 0)
		rgbled_test();
	else if(strcmp(token, "pwm") == 0)
		pwm_test();
	else if(strcmp(token, "ultra") == 0)
	 	ultra_test();
	else if(strcmp(token, "uart1") == 0)
	 	uart1_test();
	else if(strcmp(token, "motor") == 0)
	 	motor_test();
	else if(strcmp(token, "infra") == 0)
	 	infra_test();
	else if(strcmp(token, "camara") == 0)
		camara();
	else if(strcmp(token, "dfp") == 0)
		DFP_test();
	else if(strcmp(token, "girari") == 0)
		girarI();
	else if(strcmp(token, "girard") == 0)
		girarD();
	else if(strcmp(token, "avanzar") == 0)
		avanzar();
	else if(strcmp(token, "enviar") == 0)
		enviarM();
		
	prompt();
}

int main(void)
{
	int func = 0;
	irq_setmask(0);
	irq_setie(1);
	uart_init();
	uart1_init();
	uart2_init();
	uart3_init();

	puts("\nSoC - RiscV project UNAL 2020-2-- CPU testing software  interrupt "__DATE__" "__TIME__"\n");
	help();
	prompt();

	while(1) {
	        func = uart3_read();
	        if(func == 112)
	        pwm_test();
	        if(func == 'i')
	        girarI();
	        if(func == 'd')
	        girarD();
	        if(func == 'l')
	        DFP_test();
	        if(func == 'a')
	        avanzar();
	        if(func == 'r')
	        radar_test();
	        if(func == 'c')
	        camara_test();
	        if(func == 's')
	        carro();
	        
	        delay_ms(10);
	}

	return 0;
}
