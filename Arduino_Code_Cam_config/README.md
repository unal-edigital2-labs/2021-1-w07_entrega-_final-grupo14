# Arduino  :arrows_counterclockwise:
El Arduino MEGA 2560 se uso para la creación de la comunicación seria del I2c, por medio de este se configuraban los registros de la camara para que pudiera obtener una buena imagen. El arduino estaba conectado a la camara por medio de dos pines SCL y SDA, a su vez para la comunicación con el Bluetooth, el Arduino esta conectado a la uart1 y la uart3, por lo tanto las conexiones con la FPGA son los pines seriales del arduino de lectura y escritura 2 y 3. De esta misma manera se usa los pines digitales 10 y 11 del arduino para conectarlos con el [Bluetooth](/Soc_project/Bluetooth/). 

En la siguiente imagen se observa un diagrama de las conexiones anetriormente mencionadas.

![Screenshot](/Imagenes/Arduino.png)

A continuacion observamos un diagrama con la conexión del arduino con los modulos uart 1 y 3, la explicacion de su funcionamiento se encuentra dentro del archivo [Bluetooth](/Soc_project/Bluetooth/). De la misma, la implementación de estos modulos dentro del [buildSocProject.py](/Soc_project/buildSoCproject.py) permite la creacion de su espacio de memoria gracias a Litex 
![Screenshot](/Imagenes/Bluetooth.png)

Por ultimo observamos el mapa de memoria creado en el archivo [Soc_MemoryMap.csv](/Soc_project/Soc_MemoryMap.csv) para las uarts.
<p align="center">
  <img src="/Imagenes/mem_uart13.PNG" align="center">
</p>

