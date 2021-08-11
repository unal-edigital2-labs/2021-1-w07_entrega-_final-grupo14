# DFP player mini y Bluetooth  :arrows_counterclockwise:
## Bluetooth :calling:
Se uso un dispositivo Bluetooth de referencia HC-05, este posee 4 pines, dos de alimentación y otros dos para ser conectados a una uart, las cuales estan comunicados con la FPGA siendo un pin RX para la lectura y otro TX para la escritura hacia el periferico, la creación de esta uart se realizo dentro del archivo **buildSoCproject.py** como una copia dado que se necesita otra uart para el periferico del DFP player mini. No se realizo un modulo en Verilog dado que manejamos ambos perifericos con las Uart y el desarrollo de su funcionamiento se encuentra dentro del firmware como funciones. Sin embargo, cabe destacar que la comunicación es serial de tal forma que para enviar datos por el bluetooth se usaron funciones *for* con el fin de enviar dato a dato la matriz en donde se encuentra el robot y la información que a detectada hacia un celular donde el usuario pueda observar esta información.

![Screenshot](/Imagenes/bluetooth.jpg)

## DFP player mini :sound:
Como se menciono anteriormente para la comunicación con este periferico se usao otra Uart, La comunicación con el DFP player se basa que por el pin de TX (escritura) se envia una serie de 10 bytes las cuales estan organizados de la siguiente manera:

![Screenshot](/Imagenes/DFP1.jpg)

| Byte | Descripción |
| ------------- | ------------- |
|  $$ | Para comenzar la comunicación y que el DFP player sepa que se estan comnicando con él |
| VER | La version (siempre la manejamos como FF) |
| Len | numero de bytes luego de esta posición |
| CMD | Comando que le quiere enviar al DPF player  |
| Feedback | Si se quiere saber retroalimentación del proceso que este haciendo |
| parm1 | Parametro 1 para dar una orden  |
| parm2 | Parametro 2 para dar una orden  |
| checksum | Suma y verificación de los valores que se van a enviar |
| $O | Byte final que dice que se termino la orden enviada |

De esta manera para usar dar ordenes al DFP player se uso una función llamada **sendInfo()** con la cual solo modificamos los valores de CMD, parm1 y parm2 para reproducir un audio, parar el audio, subir el volumen y demas. La idea es que estos audios se reproduscan cada vez que la camara detecte el color de una pared dandonos la información por un parlante de 8W.

Pasando a las conexiones del DFP player se uso el pin VCC para la alimentacion de 5v con un arduino, los pines RX y TX como se menciono anteriormente irian conectados a la FPGa para la comunicacion con la Uart y el manejo del periferico, el pin GND como la tierra y por ultimo el DFP player posee un espacio para almacenar una memoria SD la cual posee los audios utilizados en el proyecto. Estos audios son nombrados como 0001, 0002... Esto dado que al ingresar el *parm2* dentro de la orden del DFP player pueda encontrar y reproducir el audio.

![Screenshot](/Imagenes/DFP2.jpg)