# Radar
El funcionamiento de este periférico se basa en que, por medio de un sensor de ultrasonido HC-SR04, se mide la distancia a la pared del laberinto que se encuentre apuntando el sensor, y usando un motor paso a paso se varía la dirección en la cual apunta el ultrasonido con el fin de obtener las distancias existentes entre el robot y las paredes que se encuentran a su derecha, su izquierda y el frente, y de tal forma determinar las posiciones donde se encuentran las paredes del laberinto para que el robot pueda mapearlas posteriormente y de igual tomar la decisión sobre hacia que dirección debe avanzar.

Para el desarrollo de los drivers correspondientes a este periférico nos guiamos por el trabajo realizado por el Grupo 2 del semestre 2020-II ([Ultrasonido](https://github.com/unal-edigital2/w07_entrega-_final-grupo02/tree/main/Hardware/Modulos/ultrasonido)), en donde para determinar la distancia se utilizan principalmente dos módulos (*contador.v* y *genpulsos.v*) junto a otros módulos auxiliares que cumplen la función de divisores de frecuencia para hacer relojes o se encargan de que se cumpla la máquina de estados. 

### Módulo Contador (*contador.v*)
Las funciones de este módulo son activar la generación del pulso que le indica al ultrasonido que debe inciar a operar, y calcular la distancia entre el ultrasonido y la pared midiendo el ancho de la señal *echo*, la cual es la salida del ultrasonido y proporcional a la distancia. Para que la proporción entre el ancho de la señal y la distancia sea 1 a 1 se utiliza un divisor de frecuencia para generar un reloj cuyos ciclos son equivalentes al tiempo que se demora la señal emitida por el ultrasonido en recorrer 1 cm. 

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

