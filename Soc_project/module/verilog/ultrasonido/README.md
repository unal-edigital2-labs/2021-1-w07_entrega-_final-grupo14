# Radar 📡
El funcionamiento de este periférico se basa en que, por medio de un sensor de ultrasonido HC-SR04, se mide la distancia a la pared del laberinto que se encuentre apuntando el sensor, y usando un servomotor se varía la dirección en la cual apunta el ultrasonido con el fin de obtener las distancias existentes entre el robot y las paredes que se encuentran a su derecha, su izquierda y el frente, y de tal forma determinar las posiciones donde se encuentran las paredes del laberinto para que el robot pueda mapearlas posteriormente y de igual tomar la decisión sobre hacia que dirección debe avanzar.

## Ultrasonido

Para el desarrollo de los drivers correspondientes a este periférico nos guiamos por el trabajo realizado por el Grupo 2 del semestre 2020-II ([Ultrasonido](https://github.com/unal-edigital2/w07_entrega-_final-grupo02/tree/main/Hardware/Modulos/ultrasonido)), en donde para determinar la distancia se utilizan principalmente dos módulos (*contador.v* y *genpulsos.v*) junto a otros módulos auxiliares que cumplen la función de divisores de frecuencia para hacer relojes o se encargan de que se cumpla la máquina de estados. El diagrama que describe la conexión entre los drivers de este periférico es el siguiente:

![Screenshot](/Imagenes/ultra.png)

Y las ubicaciones de los registros en el mapa de memoria (*Soc_MemoryMap.csv*) son las siguientes:

<p align="center">
  <img src="/Imagenes/mem_ultra.PNG" align="center">
</p>


A continuación se hace la descripción del funcionamiento de los módulos principales correspondientes al ultrasonido.

### Módulo Contador (*contador.v*)
Las funciones de este módulo son activar la generación del pulso que le indica al ultrasonido que debe iniciar a operar, y calcular la distancia entre el ultrasonido y la pared midiendo el ancho de la señal *echo*, la cual es la salida del ultrasonido y proporcional a la distancia. Para que la proporción entre el ancho de la señal y la distancia sea 1 a 1 se utiliza un divisor de frecuencia para generar un reloj cuyos ciclos son equivalentes al tiempo que se demora la señal emitida por el ultrasonido en recorrer 1 cm. El diagrama de bloque que representa el funcionamiento del módulo es el siguiente:

<p align="center">
  <img src="/Imagenes/Contador.PNG" align="center">
</p>


El código utilizado para realizar este proceso es el siguiente:

```verilog
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
```

Podemos observar que la variable *count0* aumenta 1 por cada ciclo del reloj *CLKOUT* mientras que la señal de entrada *echo* se encuentra en 1, de tal forma cuando el pulso *echo* vuelve a tener valor de 0, se almacena en la variable *count* el valor de la distancia entre el ultrasonido y la pared más cercana. De igual forma la variable *calculate* toma valor de 1 cuando se termina de realizar el proceso de medición, actuando como un Done. 

### Módulo Generador de Pulsos (*genpulsos.v*)

La función de este módulo es crear una señal PWM de ancho de 10 microsegundos en alto cuando el módulo *contador.v* se lo indica. Esta señal corresponde a el *trigg* del ultrasonido, por lo tanto despues de recibir esta señal este empieza a operar y emite las ondas que se reflejan en las paredes del laberinto con el fin de medir la distancia. El diagrama de bloque que representa el funcionamiento del módulo es el siguiente:

<p align="center">
  <img src="/Imagenes/PWM_ultra.PNG" align="center">
</p>


El código utilizado para realizar este proceso es el siguiente:

```verilog
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
 ```
                 
El reloj *CLKOUT1* tiene período de 10 microsegundos, por lo tanto cuando la señal de entrada *pulse* tiene valor alto, en el primer ciclo del reloj las señales *Doit* y *NoDoit* tendran un valor distinto y por ende la señal de salida *trigg* sera alta durante este período, pero en el siguiente ciclo del reloj las señales seran iguales y la señal *trigg* tendra valor bajo, generando de tal forma un pulso de 10 microsegundos que corresponde a la entrada del ultrasonido.  

## Servomotor (*pwm.v*)

El driver de este periférico corresponde a un simple módulo PWM, ya que el desplazamiento angular del servomotor se define por el ciclo útil de una señal PWM con período de 20 milisegundos.  El diagrama que describe la conexión entre el driver y el periférico es el siguiente:

![Screenshot](/Imagenes/servos.png)

Y las ubicaciones de los registros en el mapa de memoria (*Soc_MemoryMap.csv*) son las siguientes:

<p align="center">
  <img src="/Imagenes/mem_servo.PNG" align="center">
</p>


El diagrama de bloques que describe el funcionamiento del módulo es el siguiente: 

<p align="center">
  <img src="/Imagenes/pwm.PNG" align="center">
</p>


El código utilizado para realizar este módulo es el siguiente:

```verilog
      reg[27:0] counter=28'd0;
      reg[15:0] limite;
      reg[15:0] activo;

      always @(posedge clk) begin

      counter <= counter + 28'd1;

           if(counter>=(period-1))
              counter <= 28'd0;

           if(counter <= dutty)
              pwm <= 1;
              else
              pwm <=0;

      end
 ```
 
El funcionamiento del código se basa en que se define un contador que aumenta con cada ciclo del reloj, y cuando el valor del contador es menor que la señal de entrada *dutty* correspondiente al ciclo útil, la señal de salida *pwm* tiene valor alto mientras que si el contador es mayor al ciclo útil la señal de salida se torna a valor bajo, y en el momento que el contador tiene el mismo valor que la señal de entrada *period*, este se reinicia repitiendo el proceso descrito anteriormente. 

