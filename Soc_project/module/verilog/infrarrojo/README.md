# Infrarrojo :footprints:
El periferico de los sensores seguidores de linea que funcionan a partir de infrarrojos, el que se uso dentro del proyecto posee 5 sensores como se puede ver en la siguiente imagen.

![Screenshot](/Imagenes/IR.jpeg)

Este periferico se uso a partir de un driver del infrarrojo es decir que cada uno de estos sensores posee un solo pin de entrada y de salida, cabe destacar que este periferico ademas de las 5 conexiones con el driver posee otros dos pines los cuales son de alimentacion GND y el otro debe ser alimentado con 5V que obtenemos del arduino.

A continuacion observamos el modulo en verilog del infrarrojo, el desarrollo de este es muy simple, ya que se declaran 5 pines de la FPGA como entradas *infras* y luego se igualan a los 5 pines de salida *infras2*. Para implementación de este periferico dentro de nuestro robot se desarrollo su codigo en software, como se puede obserbar en la parte de firmware tiene su propia función la cual se explica con mas detalle. Como tal los infrarrojos le diran al robot si debe avanzar y saber si se desvio de su camino original gracias a que un infrarrojo al detectar cinta negra mandaria un 0 hacia la FPGA, es decir que si quiere seguir un camino se recibira 11011 por los pines de la FPGA, si otro sensor envia 0 es decir que el robot se desvio de su camino o deberia girar, y si todos detectan cinta negra (00000) entonces el robot se detendra lo que significa que llego a una posicion de la matriz.

```verilog
    `timescale 1ns / 1ps

      module infrarrojo(
      input [4:0] infras,
      output [4:0] infras2
    );

     assign infras2 = infras;

    endmodule
```
A partir de lo anterior podemos ver el siguiente diagrama de bloques donde vemos la conexión con el periferico de infrarrojo, en este observamos que la conexión se logra gracias a al modulo infrarrojo.py que a su vez su implementación dentro del **buildSoCproject.py** permite la creación de su espacio de memoria.  

![Screenshot](/Imagenes/infras.png)

Por ultimo vemos su espacio de memoria dentro del archivo **Soc_MemoryMap.csv**.

![Screenshot](/Imagenes/mem_infra.PNG)
