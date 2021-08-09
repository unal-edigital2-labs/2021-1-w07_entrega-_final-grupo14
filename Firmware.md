# Firmware  :man_technologist: 
Para el desarrollo del firmware nos fijaremos en archivo main.c en el cual se encuentran las funciones del funcionamiento de nuestro robot. Estas seran explicadas a continuación teniendo en cuenta que primero creamos dos variables globales que entraran en varias funciones. El primero una matriz que nos servira de mapa según la trayectoria y lo que detecte nuestro robot, y la segunda un arreglo "color" de 3 posiciones que nos diga si detecto azul, verde o rojo.

    unsigned int matriz[6][5];
    unsigned int color[3];


## :heavy_check_mark: infrarrojo();
  Para la función infrarrojo primero se crean dos variables, al primera para obtener el valor que nos da el periferico y la segunda como una salida de la función que se utilizara mas adelante dentro de otras funciones.
  
    unsigned int entrada = infra_cntrl_infras2_read();
    unsigned int salida=0;
A partir de lo que obtenemos de la entrada hacemos una serie de comparaciones para poder entrar en ciertas condiciones, es decir que del modulo .v del infrarrojo se obtienen 5 bits, si el infrarrojo detecta cinta negra manda un 0 por el sensor que lo capto, es decir si ningun sensor detecto cinta negra excepto el de la mitad obtendremos 11011 que en hexadecimal seria 1B, a partir de esto generamos 4 casos, el ya mencionado que es para que el robot siga avanzado por su camino, 17 y 1D para saber si se desvio de su camino dado que un sensor de los lados detecto la cinta negra y por ultimo 00 que significa que todos los sensores detectaron cinta negra y de tal manera se sabe que se debe detener.

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

## :heavy_check_mark: sendInfo(CMD, p1, p2); y DFP_setup();
  Para la función sendInfo() se crean dos variables, la segunda que es un arreglo de 10 posiciones nos envia cada uno de los pares de bytes que necesita el DFP player para funcionar, esto se profundiza un poco mas dentro de la explicación del periferico. Esta función tiene 3 entradas con lo que se puede modificar el comando que se va a utilizar y los dos parametros, finalmente se envia este arreglo por medio de un for para ser transmitido por una uart al periferico.
   
    unsigned int checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + param1 + param2);
    unsigned int  Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    param1, param2, HIGHBYTE(checksum), LOWBYTE(checksum), End_Byte};
  
    for(int i=0; i<10; i++){
    uart2_write(Command_line[i]);
    }

La función DFP_setup depende solamente de 3 llamados a la función sendInfo, en la cual le damos unos valores de entrada al iniciar el robot, darle un volumen al parlante de 25 (que se encuentra entre el rango de 0 a 30), este se logra con el comando 0x06. Que el DFP player reprodusca los audios sin ningun filtro, es decir normal con el comando 0x07. Y que use obtenga los audios de una memoria SD con el comando 0x09.

    //myDFPlayer.volume(25);
    // 7E FF 06 06 XX 00 19 XX EF 
    sendInfo(0x06,0x00,0x15);

    //myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    // 7E FF 06 07 XX 00 00 XX EF 
    sendInfo(0x07,0x00,0x00);

    //myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    // 7E FF 06 09 XX 00 02 XX EF 
    sendInfo(0x09,0x00,0x02);
 
## :heavy_check_mark: camara();
Para la funcion de la camara se obtienen los valores que arroja el driver de la camara que son color, done y error. A partir de estos se genera una serie de if que nos dice si ya termino de tomar la imagen y no hay un error entra en unas condiciones según sea el valor que obtuvimos de la variable color. si es 001 significa que es azul, 010 verde, 100 rojo y 111 si no detecta la predominancia de alguno de estos colores. 

## :heavy_check_mark: radar(); 
Para la funcion radar se le introducen dos funciones que anteriormente mencionamos, sendInfo() y camara(). Primero creamos unas variables para poder tomas los valores de distacion que obtenemos del ultrasonido y a su vez la posición del motor paso a paso por el PWM.

    unsigned int orden = 0;
    unsigned int distancia = 0;
    unsigned int limite_distancia = 10;
    unsigned int done = 0; 
    unsigned int period = 2000000;
    unsigned int dutty = 0;
    unsigned int enable = 1;
    unsigned int posicion =0;
    unsigned int cam=7;
Primero declaramos el funcionamiento cuando el radar esta mirando hacia la izquierda, de damos un valor a dutty para que el el motor apunte el ultrasonido a la izquierda, le damos el valor de 1 a orden para que se active el ultrasonido y obtenemos el valor de distacia gracias al modulo de ultrasonido.

    /* Izquierda */
    dutty = 40000; /* Servo gira a la izquierda*/
    pwm_cntrl_period_write(period);
    pwm_cntrl_dutty_write(dutty);
    delay_ms(1000);
    orden = 1;
    ultra_cntrl_orden_write(orden);
    delay_ms(100);
    distancia = 2*ultra_cntrl_d_read(); /* Se obtiene el valor de la distancia al objeto*/
Este valor de distacia lo comparamos con una distancia limite que nosotros proponemos, es decir un valor maximo para decir si detecto una pared o no. Luego a l valor de posición se le sumo 0x1 esto se explicara a profundidad mas adelante pero la idea es que esta observando a la izquierda. LLamamos a la funcion camara, esta detectara la imagen y pasamos a un switch el cual encontramos diferentes casos segun el color que obtuvimos por la camara. segun el color detectado se le da un valor al arreglo color, la posicion color[0] es cuando el radar esta observando la izquierda, color[1] cuando el radar obserba al frente y color[2] cuando ve a la derecha. según esto se le asigna 1 si es azul, 2 si es verde y 3 si es rojo.

Despues de hacer la asigancion al arreglo color se reproducen los respectivos audios para saber que color detecto, y depues de 2000 milisegundos (2 segundos) se vuelve a usar la funcion sendInfo pero con el comando de parar con el fin de que el audio que se esta reproduciendo no entre en un bucle. Por ultimo en caso de que la distacia detectada a la pared sea mas grande de lo establecido saldra del if, le sumara al avslor de posición 0x0 y el valor de ornde sera 0 para que el ultrasonido detenga su proceso.

    if (distancia<limite_distancia){
        cam=camara();
        posicion += 0x1; 
        switch(cam){
          case 1: 
            color[0]=1;
            sendInfo(0x03,0x00,0x04);
                  delay_ms(2000);
                  sendInfo(0x16,0x00,0x00);
            break; 
          case 2: 
                    color[0]=2;
            sendInfo(0x03,0x00,0x03);
                  delay_ms(2000);
                  sendInfo(0x16,0x00,0x00);
            break;
          case 4: 
            color[0]=3;
            sendInfo(0x03,0x00,0x05);
                  delay_ms(2000);
                  sendInfo(0x16,0x00,0x00);
            break;
        }
      } else {
        posicion+=0x0;
      }
      delay_ms(100);
      orden = 0;
      ultra_cntrl_orden_write(orden);
 
A contnuacion explicaremos el funcionamiento de la variable "posicion"  para esto como se observa en la siguiente tabla se encuentra los 3 casos segun a donde este mirando el radar.

| Posición | Binario | Hexadecimal |
| ------------- | ------------- | ------------- |
| Izquierda | 001 |  0x1 |
| Frente | 010 |  0x2 |
| Derecha | 100 |  0x4  |
      
Si el robot detecto dos paredes al mirar a sus lados con el radar entonces la variable posición nos dira que paredes estan ocupadas con la suma de estos valores dentro de los if que nos dicen si detecto pared. Es decir si detecto paredes a la izquierda y derecha significa que posicion tendria el valor de 0x5, si detecto 3 paredes posicion seria 0x7. Esto sera de gran importancia para el desarrollo de la función principal carro() en donde su principla papel sera servirnos como guia para el dibujo de la matriz.

## :heavy_check_mark: enviarM();

## :heavy_check_mark:  avanzar();

## :heavy_check_mark: girarD();

## :heavy_check_mark: girarI();

## :heavy_check_mark:  carro();

