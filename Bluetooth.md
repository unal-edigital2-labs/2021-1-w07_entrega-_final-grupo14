# Bluetooth :calling:
Se uso un dispositivo Bluetooth de referencia HC-05, este posee 4 pines, dos de alimentación y otros dos para ser conectados a una uart, las cuales estan comunicados con la FPGA siendo un pin RX para la lectura y otro TX para la escritura hacia el periferico, se realizo la creación de dos uarts (uart1 y uart3) dentro del archivo **buildSoCproject.py** como copias de una uart que ya venia con el proyecto. No se realizo un modulo en Verilog dado que manejamos el periferico con las Uart y el desarrollo de su funcionamiento se encuentra dentro del firmware como una función llamada **enviarM()** y **main()**. A su vez, cabe destacar que la comunicación es serial de tal forma que para enviar datos por el bluetooth se usaron funciones *for* con el fin de enviar dato a dato la matriz en donde se encuentra el robot y la información que a detectada hacia un celular donde el usuario pueda observar esta información por medio de la aplicación Bluetooth terminal HC05.

La conexión del bluetooth no se hizo directamente con la uart de la FPGA dado que hubo un problema de comunicación, de esta manera se conectaron la uart1 y uart3 a un Arduino MEGA 2560 y este solamente recibira la información de las uart y transmitira al periferico bluetooth para luego recibirla en le celular. la Uart1 se uso para transmitir la información de la matriz por donde el robot esta pasando y lo que esta viendo, mientras que la Uart3 se uso para recibir los comandos dados por el usuario para activar ciertas funciones, se hicieron estas dos uarts para evitar problemas de sincronización a la hora dde ejecutar las funciones.

Por ultimo, mostraremos el codigo de arduino que se uso para la comunicacion con el bluetooth. por un lado se recibe lo que manda la FPGa y se transmite, y luego lo que llega por el bluetooth y se envia a la FPGA.

```C
     if(miBT.available()){
        dato1 = miBT.read();
        Serial3.write(dato1);

        }

      if(Serial3.available()){
        dato3 = Serial3.read();
        miBT.write(dato3);
        }
```

