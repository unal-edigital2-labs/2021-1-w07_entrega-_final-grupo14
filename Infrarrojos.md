# Infrarrojo :footprints:
El periferico de de los sensores seguidores de linea que funcionan a partir de infrarrojos, el que se uso dentro del proyecto posee 5 sensores como se puede ver en la siguiente imagen, Este periferico se uso a partir de un GPIO es decir que cada uno de estos sensores posee un un solo pin de entrada y de salida, cabe destacar que este periferico ademas de las 5 conexiones con el GPIO posee otros dos pines los cuales son de alimentacion GND y el otro debe ser alimentado con 5V que obtenemos del arduino.



![Screenshot](/Imagenes/GPIOir.jpg)

A continuacion observamos el modulo en verilog del infrarrojo, el desarrollo de este es muy simple, ya que se declaran 5 pines de la FPGA como entrdas *infras* y luegos se igualan a los 5 pines de salida *infras2*. Para implementación de este periferico dentro de nuestro robot se desarrollo su codigo en software, como se puede obserbar en la parte de firmware tiene su propia función ayuda al robot co

    `timescale 1ns / 1ps

    module infrarrojo(
      input [4:0] infras,
      output [4:0] infras2
    );

      assign infras2 = infras;

    endmodule
