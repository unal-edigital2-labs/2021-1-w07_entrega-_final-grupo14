# DFP player mini :sound:
 La comunicación con el DFP player se basá que por el pin de TX (escritura) se envía una serie de 10 bytes las cuales estan organizados de la siguiente manera:


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

De esta manera para usar dar ordenes al DFP player se uso una función llamada **sendInfo()** con la cual solo modificamos los valores de **CMD**, **parm1** y **parm2** para reproducir un audio, parar el audio, subir el volumen y demás. La idea es que estos audios se reproduzcan cada vez que la cámara detecte el color de una pared dandonos la información por un parlante de 8W.

Pasando a las conexiones del DFP player se usó el pin VCC para la alimentación de 5v con un arduino, los pines RX y TX como se mencionó anteriormente irían conectados a la FPGA para la comunicación con la Uart y el manejo del periférico, el pin GND como la tierra y por ultimo el DFP player posee un espacio para almacenar una memoria SD la cual posee los audios utilizados en el proyecto. Estos audios son nombrados como 0001, 0002... Esto dado que al ingresar el **parm2** dentro de la orden del DFP player pueda encontrar y reproducir el audio.

<p align="center">
  <img src="/Imagenes/DFP2.jpg" align="center">
</p>

En la siguiente imagen se puede observar la conexión con el DFP player mini, está conectado a partir de un módulo **uart2.py** por los pines Rx y Tx, y a su vez esta uart2 fue creada en el ([buildSoCproject.py](/Soc_project/buildSoCproject.py)) creando las conexiones y espacios de memoria gracias a la herramienta de Litex.

![Screenshot](/Imagenes/DFP.png)

En esta imagen podemos observar el espacio de memoria creado para la Uart2 con los pines de lectura-escritura y de solo lectura.

<p align="center">
  <img src="/Imagenes/mem_uart2.PNG" align="center">
</p>

