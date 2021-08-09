# Cámara 📸
Se utilizo una cámara de referencia OV7670 y para el desarrollo de los drivers nos guiamos en el trabajo realizado por el Grupo 5 del semestre 2019-II ([Cámara](https://github.com/unal-edigital1-2019-2/work04-proyectofinal-grupo-05-1)). El funcionamiento de la cámara y el procesamiento de las imagenes se describen de forma general en el siguente diagrama:

![Screenshot](/Imagenes/camara1.PNG)

## Configuración cámara por I2C (*Camara_OV.ino*)
Se establecen los valores de los registros de la cámara por medio del protocolo de comunicación serial I2C, se utilizo un microcontrolador Arduino para llevar a cabo este proceso y se describe en el archivo *Camara_OV.ino*. El código con el cual se establecen los registros y el formato de la cámara es el siguiente:
```c
    OV7670_write(0xFF, 0xF0); 
     OV7670_write(0x12, 0x0C);  //COM7: Set QCIF and RGB
     //OV7670_write(0x12, 0x04);  //COM7: Set RGB
     OV7670_write(0x11, 0xC0);       //CLKR: Set internal clock to use external clock
     //OV7670_write(0x11, 0x80);    
     OV7670_write(0x0C, 0x08);       //COM3: Enable Scaler
     //OV7670_write(0x0C, 0x00);
     OV7670_write(0x3E, 0x00);
     //OV7670_write(0x3E, 0x1B);   //Enable Scaling for Predefined Formats using COM14
     OV7670_write(0x40,0xD0);      //COM15: Set RGB 565
     OV7670_write(0x8C,0x00);
```
De igual forma tambien se establecen una serie de registros, que se denominaron "Registros magicos", con el fin de que la imagen obtenida por la cámara sea de mejor calidad.

## Captura de datos (*cam_read.v*)
