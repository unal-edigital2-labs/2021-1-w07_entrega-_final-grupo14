# Radar 📡
El funcionamiento de este periférico se basa en que, por medio de un sensor de ultrasonido HC-SR04, se mide la distancia a la pared del laberinto que se encuentre apuntando el sensor, y usando un motor paso a paso se varía la dirección en la cual apunta el ultrasonido con el fin de obtener las distancias existentes entre el robot y las paredes que se encuentran a su derecha, su izquierda y el frente, y de tal forma determinar las posiciones donde se encuentran las paredes del laberinto para que el robot pueda mapearlas posteriormente y de igual tomar la decisión sobre hacia que dirección debe avanzar.

Para el desarrollo de los drivers correspondientes a este periférico nos guiamos por el trabajo realizado por el Grupo 2 del semestre 2020-II ([Ultrasonido](https://github.com/unal-edigital2/w07_entrega-_final-grupo02/tree/main/Hardware/Modulos/ultrasonido)), en donde para determinar la distancia se utilizan principalmente dos módulos (*contador.v* y *genpulsos.v*) junto a otros módulos auxiliares que cumplen la función de divisores de frecuencia para hacer relojes o se encargan de que se cumpla la máquina de estados. 

### Módulo Contador (*contador.v*)
Las funciones de este módulo son activar la generación del pulso que le indica al ultrasonido que debe iniciar a operar, y calcular la distancia entre el ultrasonido y la pared midiendo el ancho de la señal *echo*, la cual es la salida del ultrasonido y proporcional a la distancia. Para que la proporción entre el ancho de la señal y la distancia sea 1 a 1 se utiliza un divisor de frecuencia para generar un reloj cuyos ciclos son equivalentes al tiempo que se demora la señal emitida por el ultrasonido en recorrer 1 cm. El diagrama de bloque que representa el funcionamiento del módulo es el siguiente:

![Screenshot](/Imagenes/Contador.PNG)

El código utilizado para realizar este proceso es el siguiente:

      always@(posedge CLKOUT)
        begin
          logico=(count0[7]||count0[6]||count0[5]||count0[4]||count0[3]||count0[2]||count0[1]||count0[0]);
          if(reset)
          begin
            count0=0;
            calculate=0;
            pulse=0;
          end
          //	Da la orden de mandar un pulso
          else
          begin
            if(ENABLE)
            begin
              pulse=1'b1;
            end
            //
            //	Cuenta el rango que tiene el pulso del ECHO del sensor
            //
            if(ECHO)
            begin
              count0=count0+1;
            end
            if(!ECHO && logico)
            begin
              calculate = 1;
            end
          end
        end
        assign count = count0;

Podemos observar que la variable *count0* aumenta 1 por cada ciclo del reloj *CLKOUT* mientras que la señal de entrada *echo* se encuentra en 1, de tal forma cuando el pulso *echo* vuelve a tener valor de 0, se almacena en la variable *count* el valor de la distancia entre el ultrasonido y la pared más cercana. De igual forma la variable *calculate* toma valor de 1 cuando se termina de realizar el proceso de medición, actuando como un Done. 

### Módulo Generador de Pulsos (*genpulsos.v*)

La función de este módulo es crear una señal PWM de ancho de 10 microsegundos en alto cuando el módulo *contador.v* se lo indica. Esta señal corresponde a el *trigg* del ultrasonido, por lo tanto despues de recibir esta señal este empieza a operar y emite las ondas que se reflejan en las parades del laberinto con el fin de medir la distancia. El diagrama de bloque que representa el funcionamiento del módulo es el siguiente:

![Screenshot](/Imagenes/PWM_ultra.PNG)

El código utilizado para realizar este proceso es el siguiente:

            assign trigg=(Doit && ~NoDoit);
            always@(posedge CLKOUT1)
                  begin
                        if(reset)
                        begin
                              Doit<=0;
                              NoDoit<=0;
                        end
                        else
                        begin
                              if(pulse)
                              begin
                                    Doit<=1'b1;
                              end
                              if(pulse && Doit)
                              begin
                                    NoDoit<=1'b1;
                              end
                        end
                  end
El reloj *CLKOUT1* tiene periodo de 10 microsegundos, por lo tanto cuando la señal de entrada *pulse* tiene valor alto, en el primer ciclo del reloj las señales *Doit* y *NoDoit* tendran un valor distinto y por ende la señal de salida *trigg* sera alta durante este periodo, pero en el siguiente ciclo del reloj las señales seran iguales y la señal *trigg* tendra valor bajo, generando de tal forma un pulso de 10 microsegundos que corresponde a la entrada del ultrasonido.  

