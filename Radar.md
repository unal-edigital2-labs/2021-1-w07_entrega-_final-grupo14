# Radar
El funcionamiento de este periférico se basa en que, por medio de un sensor de ultrasonido HC-SR04, se mide la distancia a la pared del laberinto que se encuentre apuntando el sensor, y usando un motor paso a paso se varía la dirección en la cual apunta el ultrasonido con el fin de obtener las distancias existentes entre el robot y las paredes que se encuentran a su derecha, su izquierda y el frente, y de tal forma determinar las posiciones donde se encuentran las paredes del laberinto para que el robot pueda mapearlas posteriormente y de igual tomar la decisión sobre hacia que dirección debe avanzar.

Para el desarrollo de los drivers correspondientes a este periférico nos guiamos por el trabajo realizado por el Grupo 2 del semestre 2020-II ([Ultrasonido](https://github.com/unal-edigital2/w07_entrega-_final-grupo02/tree/main/Hardware/Modulos/ultrasonido)), en donde para determinar la distancia se utilizan principalmente dos módulos (*contador.v* y *genpulsos.v*)

