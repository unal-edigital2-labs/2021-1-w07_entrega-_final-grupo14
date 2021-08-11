# Entrega Final - Digital II - Grupo 4
## Autores :fountain_pen:
- Diego Steven Peña Cortes :mechanic:
- Daniel Machado Roa :technologist:
- Andrés Felipe Rodríguez Contreras :office_worker:

Este es el repositorio del proyecto final de la asignatura Electrónica Digital II del semestre 2021-1 (Universidad Nacional de Colombia - Sede Bogotá). A continuación se mostrará el proyecto de un Robot Cartográfico conformado por una arquitectura de SoC, donde a partir de un micropocesador y diferentes periféricos se realizan operaciones que permiten al robot avanzar por un laberinto con el fin de identificar y mapear en una matriz los muros encontrados y el color correspondiente a estos. 

![Screenshot](/Imagenes/robot.jpeg)

Los periféricos que se utilizaron en el proyecto, para complementar al procesador, fueron una camara (OV7670), infrarrojos (Sensor de seguidor de linea de 5 canales), un par de motores manejados a partir de un puente H, una conexión bluetooth HC-05, un DFP player mini, un motor paso a paso y un ultrasonido HC-sr04.

A continuación haremos una breve explicación del Soc, el firmware y los perfiféricos integrados al Soc junto a sus respectivos links en donde se puede observar su funcionamiento más en detalle.

## [SoC](w07_entrega-_final-grupo14/Camara.md ) :robot:

En este enlace se describe la arquitectura del robot cartógrafo y el proceso que se llevo a cabo usando el entorno Litex para el ensamble y la integración del microprocesador picoRV32, el bus de datos Wishbone y los diferentes módulos de los periféricos que componen el robot. 

## [Firmware](/Soc_project/firmware/) :man_technologist:
Dentro del enlace de firmware se encuentra la información del código usado para el desarrollo del funcionamiento del robot cartógrafo, de igual forma se puede observar la explicación de las funciones que se encuentran dentro del archivo *main.c*.

## Periféricos :electric_plug:
Aquí presentamos una lista de los periféricos usados en el robot cartógrafo, cada uno de estos links mostraran el módulo en verilog y a su vez se hace una explicación detallada del código utilizado para el funcionamiento del módulo de cada periférico.
- [Cámara](/Soc_project/module/verilog/camara/)
- [Radar](/Soc_project/module/verilog/ultrasonido/)
- [Motores](/Soc_project/module/verilog/motor/)
- [Infrarrojos](/Soc_project/module/verilog/infrarrojo/)
- [DFP player mini y Bluetooth](/Arduino_Code_Cam_config/)

## Pruebas de Funcionamiento 

En los siguientes enlaces se encuentran los videos correspondientes a las pruebas de funcionamiento realizadas al robot cartógrafo, en ellos se puede observar al robot recorriendo el laberinto de forma autónoma a medida que reconoce los colores de las paredes. De igual forma, se observa simultaneamente la impresión del mapeo realizado por el robot que se envía a un celular por medio de bluetooth.
- [Video 1](https://www.youtube.com/watch?v=-sIw7MB7exA)
- [Video 2](https://www.youtube.com/watch?v=XjEla83Jrmw)


### Problemas Presentados
Durante la realización del proyecto se presentaron diversos problemas, los más significativos fueron los siguientes:
- **Detección de colores:** Despues de realizar distintas pruebas al módulo de la cámara encargado de analizar las imágenes capturadas, notamos que se presentaban problemas al momento de detectar los colores ya que en algunas ocasiones no se detectaba el color correcto, esto se puede evidenciar en los videos de prueba de funcionamiento del robot cartógrafo en donde el robot detecta el color erroneo en algunas paredes del laberinto. Este problema se debe a que, por las limitaciones de la cámara utilizada, cuando la iluminación del ambiente no es ideal la captura de datos es erronea y la activación de los pixeles RGB no se da de forma correcta. 
- **Módulo Bluetooth:** Se nos presento otro inconveniente al tratar de implementar el módulo bluetooth HC-05 con un periferico UART generado en la FPGA, a pesar de que se tuvo en cuenta los baudios a los que trabajaba el dispositivo (9600), no pudimos lograr que mediante de la FPGA pudieramos enviar datos por bluetooth. Debido a ello, decidimos realizar la implementación de la comunicación bluetooth con uno de los seriales que presta el Arduino Mega 2560 (tanto para recepción como para envóo de información).
- **Motorreductores:** En tanto a los motorreductores, al hacer pruebas de funcionamiento del robot cartógrafo, nos percatamos que perdían potencia a medida que ejecutabamos prueba tras prueba, esto resultaba en que los tiempos de giro variaran cada vez, y que el carro no quedase orientado correctamente en la dirección que está supuesto a tomar. Para ese entonces teniamos energizados los motores con una bateria de 9V externa que alimentaba el puente H con el cual funcionaban los motores. Por lo tanto optamos por alimentar el puente H con 12V suministrados por 8 pilas recargables AA en serie, de está manera podemos asegurar que cada vez que realizemos pruebas los motores del motor funcionen a la misma potencia.  
