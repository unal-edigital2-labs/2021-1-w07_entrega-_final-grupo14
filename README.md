# Entrega Final - Digital II - Grupo 4
## Autores :fountain_pen:
- Diego Steven Peña Cortes :mechanic:
- Daniel Machado Roa :technologist:
- Andrés Felipe Rodríguez Contreras :office_worker:

Este es el repositorio del proyecto final de la asignatura Electrónica Digital II del semestre 2021-1 (Universidad Nacional de Colombia - Sede Bogotá). A continuación se mostrará el proyecto de un Robot Cartográfico conformado por una arquitectura de SoC, donde a partir de un micropocesador y diferentes periféricos se realizan operaciones que permiten al robot avanzar por un laberinto con el fin de identificar y mapear en una matriz los muros encontrados y el color correspondiente a estos. 

![Screenshot](/Imagenes/robot.jpeg)

Los periféricos que se utilizaron en el proyecto, para complementar al procesador, fueron una camara (OV7670), infrarrojos (Sensor de seguidor de linea de 5 canales), un par de motores manejados a partir de un puente H, una conexión bluetooth HC-05, un DFP player mini, un motor paso a paso y un ultrasonido HC-sr04.

A continuación haremos una breve explicación del Soc, el firmware y los perfiféricos integrados al Soc junto a sus respectivos links en donde se puede observar su funcionamiento más en detalle.

## [SoC](w07_entrega-_final-grupo14/Camara.md )

En este enlace se describe la arquitectura del robot cartógrafo y el proceso que se llevo a cabo usando el entorno Litex para el ensamble y la integración del microprocesador picoRV32, el bus de datos Wishbone y los diferentes módulos de los periféricos que componen el robot. 

## [Firmware](w07_entrega-_final-grupo14/Camara.md )
Dentro del enlace de firmware se encuentra la información del código usado para el desarrollo del funcionamiento del robot cartógrafo, de igual forma se puede observar la explicación de las funciones que se encuentran dentro del archivo *main.c*.

## Periféricos 
Aquí presentamos una lista de los periféricos usados en el robot cartógrafo, cada uno de estos links mostraran el módulo en verilog y su archivo .py respectivo, a su vez se hace una explicación detallada del código utilizado para el funcionamiento del módulo de cada periférico.
- [Cámara](w07_entrega-_final-grupo14/Camara.md )
- [Radar](w07_entrega-_final-grupo14/Camara.md )
- [Motores](w07_entrega-_final-grupo14/Camara.md )
- [Infrarrojos](w07_entrega-_final-grupo14/Camara.md )
- [DFP player mini y Bluetooth](w07_entrega-_final-grupo14/Camara.md )


