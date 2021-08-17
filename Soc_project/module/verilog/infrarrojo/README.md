# Infrarrojo :footprints:
El periférico de los sensores seguidores de línea que funcionan a partir de infrarrojos, el que se usó dentro del proyecto posee 5 sensores como se puede ver en la siguiente imagen.

<p align="center">
  <img src="/Imagenes/IR.jpeg" align="center">
</p>

Los sensores usados poseen un único pin de entrada y de salida, por lo cual se tienen 5 conexiones con el driver además de otros dos pines que cumplen la función de conexión a tierra GND y de alimentación con 5V que se obtienen del arduino.

A continuación observamos el módulo en verilog del infrarrojo, el desarrollo de este es muy simple, ya que se declaran 5 pines de la FPGA como entradas **infras** y luego se igualan a los 5 pines de salida **infras2**. Para la implementación de este periférico dentro del robot se desarrollo el código en software, como se puede observar en la sección del firmware, tiene su propia función la cual se explica con más detalle. Como tal los infrarrojos le indican al robot si debe avanzar y saber si se desvío de su camino original gracias a que un infrarrojo, al detectar cinta negra, mandaria un 0 hacia la FPGA, es decir que si quiere seguir un camino se recibira 11011 por los pines de la FPGA, si otro sensor envia 0 es decir que el robot se desvio de su camino o deberia girar, y si todos detectan cinta negra (00000) entonces el robot se detendra lo que significa que llego a una posición de la matriz.

```verilog
    `timescale 1ns / 1ps

      module infrarrojo(
      input [4:0] infras,
      output [4:0] infras2
    );

     assign infras2 = infras;

    endmodule
```
A continuación podemos observar el diagrama de bloques que describe la conexión con el periférico de infrarrojo, en este observamos que la conexión se lográ gracias al módulo ([infrarrojo.py](/Soc_project/module/infrarrojo.py)) que a su vez su implementación dentro del ([buildSoCproject.py](/Soc_project/buildSoCproject.py)) permite la creación de su espacio de memoria.  

![Screenshot](/Imagenes/infras.png)

Por ultimo vemos su espacio de memoria dentro del archivo **Soc_MemoryMap.csv**.

<p align="center">
  <img src="/Imagenes/mem_infra.PNG" align="center">
</p>

