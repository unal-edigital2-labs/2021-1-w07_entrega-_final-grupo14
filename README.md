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
