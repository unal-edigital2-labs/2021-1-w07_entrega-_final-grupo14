# Infrarrojo :footprints:
El periferico de de los sensores seguidores de linea que funcionan a partir de infrarrojos, el que se uso dentro del proyecto poseia 5 sensores como se puede ver en la siguiente imagen, Este periferico se uso a partir de un GPIO



![Screenshot](/Imagenes/Contador.PNG)


    `timescale 1ns / 1ps

    module infrarrojo(
      input [4:0] infras,
      output [4:0] infras2
    );

      assign infras2 = infras;

    endmodule
