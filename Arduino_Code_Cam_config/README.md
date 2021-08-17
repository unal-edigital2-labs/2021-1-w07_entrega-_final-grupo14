# Arduino  :arrows_counterclockwise:
El Arduino MEGA 2560 se usó para la creación de la comunicación serial del I2C, por medio de este se configuraron los registros de la cámara para que pudiera obtener una buena imagen. El arduino está conectado a la cámara por medio de dos pines SCL y SDA, a su vez para la comunicación con el Bluetooth, el Arduino está conectado a la uart1 y la uart3, por lo tanto las conexiones con la FPGA son los pines seriales del arduino de lectura y escritura 2 y 3. De está misma manera se usan los pines digitales 10 y 11 del arduino para conectarlos con el [Bluetooth](/Soc_project/Bluetooth/). 

En la siguiente imagen se observa un diagrama de las conexiones anetriormente mencionadas.

![Screenshot](/Imagenes/Arduino.png)

A continuación observamos un diagrama con la conexión del arduino con los módulos uart 1 y 3, la explicación de su funcionamiento se encuentra dentro del archivo [Bluetooth](/Soc_project/Bluetooth/). La implementación de estos módulos dentro del [buildSocProject.py](/Soc_project/buildSoCproject.py) permite la creación de su espacio de memoria gracias a Litex. 
![Screenshot](/Imagenes/Bluetooth.png)

Por último observamos el mapa de memoria creado en el archivo [Soc_MemoryMap.csv](/Soc_project/Soc_MemoryMap.csv) para las uarts.
<p align="center">
  <img src="/Imagenes/mem_uart13.PNG" align="center">
</p>

