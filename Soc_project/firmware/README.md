# Firmware  :man_technologist: 
Para el desarrollo del firmware nos fijaremos en archivo main.c en el cual se encuentran las funciones del funcionamiento de nuestro robot. Estas seran explicadas a continuación teniendo en cuenta que primero creamos dos variables globales que entraran en varias funciones. El primero una matriz que nos servira de mapa según la trayectoria y lo que detecte nuestro robot, y la segunda un arreglo "color" de 3 posiciones que nos diga si detecto azul, verde o rojo.
```C
    unsigned int matriz[6][7];
    unsigned int color[3];
```

## :heavy_check_mark: infrarrojo()
  Para la función infrarrojo primero se crean dos variables, al primera para obtener el valor que nos da el periferico y la segunda como una salida de la función que se utilizara mas adelante dentro de otras funciones.
 ```C
    unsigned int entrada = infra_cntrl_infras2_read();
    unsigned int salida=0;
 ```
A partir de lo que obtenemos de la entrada hacemos una serie de comparaciones para poder entrar en ciertas condiciones, es decir que del modulo .v del infrarrojo se obtienen 5 bits, si el infrarrojo detecta cinta negra manda un 0 por el sensor que lo capto, es decir si ningun sensor detecto cinta negra excepto el de la mitad obtendremos 11011 que en hexadecimal seria 1B, a partir de esto generamos 4 casos, el ya mencionado que es para que el robot siga avanzado por su camino, 17 y 1D para saber si se desvio de su camino dado que un sensor de los lados detecto la cinta negra y por ultimo 00 que significa que todos los sensores detectaron cinta negra y de tal manera se sabe que se debe detener.
```C
    if(entrada == 0x1b){
      salida = 0;
            printf("Siga: %x \n",infra_cntrl_infras2_read());
    }
    if(entrada == 0x17){
            salida = 1;
            printf("Derecha: %x \n",infra_cntrl_infras2_read());
    }
    if(entrada == 0x1d){
            salida = 2;
            printf("izquierda: %x \n",infra_cntrl_infras2_read());
    }
    if(entrada == 0x00){
            salida = 3;
            printf("Pare: %x \n",infra_cntrl_infras2_read());
    }
    return salida;
```

## :heavy_check_mark: sendInfo(CMD, p1, p2) y DFP_setup()
  Para la función sendInfo() se crean dos variables, la segunda que es un arreglo de 10 posiciones nos envia cada uno de los pares de bytes que necesita el DFP player para funcionar, esto se profundiza un poco mas dentro de la explicación del periferico. Esta función tiene 3 entradas con lo que se puede modificar el comando que se va a utilizar y los dos parametros, finalmente se envia este arreglo por medio de un for para ser transmitido por una uart al periferico.
  
  ```C 
    unsigned int checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + param1 + param2);
    unsigned int  Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    param1, param2, HIGHBYTE(checksum), LOWBYTE(checksum), End_Byte};
  
    for(int i=0; i<10; i++){
    uart2_write(Command_line[i]);
    }
 ```

La función DFP_setup depende solamente de 3 llamados a la función sendInfo, en la cual le damos unos valores de entrada al iniciar el robot, darle un volumen al parlante de 25 (que se encuentra entre el rango de 0 a 30), este se logra con el comando 0x06. Que el DFP player reprodusca los audios sin ningun filtro, es decir normal con el comando 0x07. Y que use obtenga los audios de una memoria SD con el comando 0x09.
```C
    //myDFPlayer.volume(25);
    // 7E FF 06 06 XX 00 19 XX EF 
    sendInfo(0x06,0x00,0x15);

    //myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    // 7E FF 06 07 XX 00 00 XX EF 
    sendInfo(0x07,0x00,0x00);

    //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    // 7E FF 06 09 XX 00 02 XX EF 
    sendInfo(0x09,0x00,0x02);
 ```
## :heavy_check_mark: camara()
Para la funcion de la camara se obtienen los valores que arroja el driver de la camara que son color, done y error. A partir de estos se genera una serie de if que nos dice si ya termino de tomar la imagen y no hay un error entra en unas condiciones según sea el valor que obtuvimos de la variable color. si es 001 significa que es azul, 010 verde, 100 rojo y 111 si no detecta la predominancia de alguno de estos colores. 

```C
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
```

## :heavy_check_mark: radar()
Para la funcion radar se le introducen dos funciones que anteriormente mencionamos, sendInfo() y camara(). Primero creamos unas variables para poder tomas los valores de distacion que obtenemos del ultrasonido y a su vez la posición del motor paso a paso por el PWM.
```C
    unsigned int orden = 0;
    unsigned int distancia = 0;
    unsigned int limite_distancia = 10;
    unsigned int done = 0; 
    unsigned int period = 2000000;
    unsigned int dutty = 0;
    unsigned int enable = 1;
    unsigned int posicion =0;
    unsigned int cam=7;
 ```
Primero declaramos el funcionamiento cuando el radar esta mirando hacia la izquierda, de damos un valor a dutty para que el el motor apunte el ultrasonido a la izquierda, le damos el valor de 1 a orden para que se active el ultrasonido y obtenemos el valor de distacia gracias al modulo de ultrasonido.
```C
    /* Izquierda */
    dutty = 40000; /* Servo gira a la izquierda*/
    pwm_cntrl_period_write(period);
    pwm_cntrl_dutty_write(dutty);
    delay_ms(1000);
    orden = 1;
    ultra_cntrl_orden_write(orden);
    delay_ms(100);
    distancia = 2*ultra_cntrl_d_read(); /* Se obtiene el valor de la distancia al objeto*/
 ```
Este valor de distacia lo comparamos con una distancia limite que nosotros proponemos, es decir un valor maximo para decir si detecto una pared o no. Luego al valor de *posicion* se le sumo 0x1 esto se explicara a profundidad mas adelante pero la idea es que esta observando a la izquierda. LLamamos a la funcion camara, esta detectara la imagen y pasamos a un switch el cual encontramos diferentes casos segun el color que obtuvimos por la camara. segun el color detectado se le da un valor al arreglo color, la posicion *color[0]* es cuando el radar esta observando la izquierda, *color[1]* cuando el radar obserba al frente y *color[2]* cuando ve a la derecha. según esto se le asigna 1 si es azul, 2 si es verde y 3 si es rojo.

Despues de hacer la asigancion al arreglo color se reproducen los respectivos audios para saber que color detecto, y depues de 2000 milisegundos (2 segundos) se vuelve a usar la funcion **sendInfo()** pero con el comando de parar con el fin de que el audio que se esta reproduciendo no entre en un bucle. Por ultimo en caso de que la distacia detectada a la pared sea mas grande de lo establecido saldra del *if*, le sumara al valor de *posicion* 0x0 y el valor de ornde sera 0 para que el ultrasonido detenga su proceso.
```C
    if (distancia<limite_distancia){
		delay_ms(1000);
		for(int i=0; i<7; i++)
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

 ```
A contnuacion explicaremos el funcionamiento de la variable *posicion* para esto como se observa en la siguiente tabla se encuentra los 3 casos segun a donde este mirando el radar.

| Posición | Binario | Hexadecimal |
| ------------- | ------------- | ------------- |
| Izquierda | 001 |  0x1 |
| Frente | 010 |  0x2 |
| Derecha | 100 |  0x4  |
      
Si el robot detecto dos paredes al mirar a sus lados con el radar entonces la variable *posición* nos dira que paredes estan ocupadas con la suma de estos valores dentro de los if que nos dicen si detecto pared. Es decir si detecto paredes a la izquierda y derecha significa que *posicion* tendria el valor de 0x5, si detecto 3 paredes *posicion* seria 0x7. Por ultimo, la funcion **radar()** nos retorna este valor de *posicion*, esto sera de gran importancia para el desarrollo de la función principal **carro()** en donde su principal papel sera servirnos como guia para el dibujo de la matriz.

## :heavy_check_mark: enviarM()
Se creo una función para que envie la matriz y llegue por bluetooth a un celular con la aplicación de "Bluetooth termianl HC-05" en esta se le envia cada posición de la matriz por medio de dos **for** que nos permitan ir por cada posicion de las filas y luego cambiar de fila al terminar. A partir de eso simplemente si el valor de la matriz es un 1 enviara por bluetooth una A, si es 2 una V y ses 3 una R haciendo referencia al color de la pared que observo a sus constados. Se finaliza el primer **for** con un salto de linea para que se observe el cambio de fila.
```C
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
  ```
    
## :heavy_check_mark:  avanzar()
Para la función de **avanzar()** del robot se le asigna un valor a la variable estado que nos dira como deben girar los motores al igual que la variable infras, este ultimo tomara su valor según lo que se obtenga del periferico y de la función **infrarrojo()**. A partir de lo anterior si la función de los infrarrojos se obtiene un valor diferente de 3 (que significa que todos los sensores observaron cinta negra) se entrara a un while en donde usara todo el tiempo la función de infrarrojos para saber su valor, luego se activaran los motores modificando el valor de la variable *estado* para que el carro avance de frente. Finalmente cuando los infrarrojos detecten una cinta negra en todos sus sensores, el carro se detendra.

 ```C
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
 
 ```




## :heavy_check_mark: girarD() y girarI()
Estas dos funciones se encargan del movimiento del robot, si gira hacia la izquierda o hacia la derecha. A contnuación solo mostraremos el codigo de **girarD()** ya que ambos codigos son similares. Podemos observar que se crean dos variables, la primera *tiempo* para decirle a los motores cuanto tiempo deben girar para poner el robot a 90° de su posición original y la segunda variable *estado* que le diga al modulo de **Motor** cuales motores debe girar y en que dirección. En este caso 0x3 significa que el motor de la izquierda gire hacia adelante y el de la derecha hacia atras para que haga el giro el robot, luego de hacer el giro detiene ambos motores con el estados 0x0, para el caso de **girarI()** el estado seria 0x4 para hacer el giro.
```C
	unsigned int tiempo = 900;
		unsigned int estado = 0x0;
		delay_ms(50);

		estado = 0x3;
		motor_cntrl_estado_write(estado);
		delay_ms(tiempo);
		estado = 0x0;
		motor_cntrl_estado_write(estado);
		delay_ms(50);
```
## :heavy_check_mark:  carro()
Ahora mostraremos nuestra función principal, la cual se encarga de que el robot pueda andar por el laberinto, identificar las paredes con su color y rellenar la matriz, para esto primero se crean diferentes variable spara luego hacer el llamado de las funciones, se le asigna una posicion inicial al robot en la matriz y se le asiga un valor a *dir* que hace referencia a la dirección, es decir que toma un valor diferente si mira hacia el norte, este u oeste. 
```C
        unsigned int radar1=0x5;
	unsigned int posX=3;
	unsigned int posY=5;
	unsigned int dir=0; /* 0 arriba, 1 derecha, 2 abajo*/
 ```       
Ahora se usa la funcón de **DFP_setup()** para darle los valores iniciales al DFP paleyer mini, luego se rellena la matriz del mapa con "80" que en ASCII es una P, a partir de dos funciones *for*. Despues de esto se usa la función **sendInfo()** para reproducir un audio y dar inicio al recorrido del robot.

 ```C	
	DFP_setup(); /* Se establecen los valores del DFP*/
	for(int i = 0; i < 6; i++) { 		/* Rellenar la matriz de 0 */
		for(int j = 0; j < 5; j++) {
		matriz[i][j] = 80;
		}
    	}
        sendInfo(0x03,0x00,0x02);
        delay_ms(2000);
        sendInfo(0x16,0x00,0x00);
``` 	

El proceso de recorrido comienza con el carro avanzando al primer cuadrante, luego se ejecuta un while comparando si el valor de radar es diferente a 0x7 que significa que observo todas las paredes ocupadas, al entrar dentro del while ejecutara todo el tiempo la función **radar()** y comparara su valor hasta que llegue al final de laberinto.

```C	
	 avanzar();
	 delay_ms(2000);
        
	 while(radar1!=0x7){
		radar1 = radar();
```	

A partir de lo que toma el valor de la función **radar()**  se entra en un switch en donde tendremos 3 casos, 0x5 si los lados estan ocupados y alfrente libre, 0x3 cuando la izquierda y al frente estan ocupados mientras que la derecha esta libre, y 0x6 derecha y frente ocupados mientras que la izquierda esta libre. A partir de esto entra en un *if* para saber la dirección que posee el robot. Ahora se tomara el arreglo de *color[]* y se le asiganara a la matriz los valores a las paredes el color que halla observado la camara dentro de la funcíon **radar()**. A su vez, le dara un valor de "43" que en ASCII es un "+" a la posición donde se encuentre el robot y por donde haya pasado. Actualiza su posición y avanza a la siguiente posición.


```C
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
```
Esto mismo pasa con los demas casos que toma el radar con su retorno de *posicion* y segun con que dirección se encuentre el robot, al final de cada uno de estos *case* se usa la función **enviarM()** para que el usuario obserbe que a visto el robot dentro del laberinto.  :ok_hand: :robot:


## :heavy_check_mark: main()

Por ultimo mostramos la función **main()** en donde se usa un while el cual siempre se ejecuta, a partir de la aplicacion de celular con la que conectamos por bluetooth al robot le enviamos una letra para que haga una función, es decir si queremos que el robot inicie el recorrido de laberinto con la función **carro()** simplemente se escribe un "s" por la aplicación y el robot comenzara su camino.
```C
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

```
