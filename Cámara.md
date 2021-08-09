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
En este módulo se realiza la captura de datos de la cámara y se envían a la memoria RAM, además de realizar la conversión del formato RGB565 a RGB332. El diagrama de bloques que describe el proceso es el siguiente:

![Screenshot](/Imagenes/camara2.PNG)

La conversión del formato RGB se lleva a cabo por medio de un proceso denominado downsampling, y se basa en reducir el tamaño de la información seleccionando los datos más significativos, despues de este proceso es posible almacenar la información de un pixel en un byte a diferencia del formato RGB565 en donde se utilizaban dos bytes por pixel.

![Screenshot](/Imagenes/camara3.PNG)

El código utilizado para llevar a cabo este proceso es el siguiente:
```verilog
always @(posedge pclk)begin
	if (!rst)begin
		if(init && !init_old)begin
			start <= 1;
			done <= 0;
		end
		if(start>0 && (!vsync && vsync_old))begin
			start <=2;
		end
		if(start == 2 && (vsync && !vsync_old))begin
			mem_px_addr <= 15'h7fff;
			px_wr <= 0;
			start <= 0;
			if(count >= 19200)begin
				done <= 1;
				error <= 0;
			end else begin
				error <= 1;
				done <= 0;
			end
		end
			if(start==2)begin
				if(href)begin
					if(countData == 0)begin
						mem_px_data[7:2] <= {px_data[7:5],px_data[2:0]} ;
						countData <= 1;
						px_wr <= 0;
					end
					if(countData == 1)begin
						mem_px_data[1:0] <= {px_data[4:3]} ;
						mem_px_addr <= mem_px_addr + 1;
						px_wr <= #1 1;
						countData <= 0;
						count <= count+1;
					end
				end else begin
					px_wr <= 0;
				end
			end 
	end
	init_old = init;
	vsync_old = vsync;
end
```
