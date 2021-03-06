# C谩mara 馃摳
Se utilizo una c谩mara de referencia OV7670 y para el desarrollo de los drivers nos guiamos en el trabajo realizado por el Grupo 5 del semestre 2019-II ([C谩mara](https://github.com/unal-edigital1-2019-2/work04-proyectofinal-grupo-05-1)). 


# Pruebas de imagen y configuraci贸n de la camara

Las pruebas de imagen se desarrollaron mediante un proyecto implementado en verilog, en donde el m贸dulo top ([test_cam.v](/Soc_project/module/verilog/camara/test_cam.v)) se encargaba de coordinar un driver de VGA ([VGA_driver.v](/Soc_project/module/verilog/camara/VGA_driver.v)) proporcionado en el repositorio WP02. Adem谩s de los distintos m贸dulos implementados que permiten la captura y downsampling de datos ([cam_read.v](/Soc_project/module/verilog/camara/cam_read.v)), almacenamiento ([buffer_ram_dp.v](/Soc_project/module/verilog/camara/buffer_ram_dp.v)), y generacion de clk's ([clk24_25_nexys4.v](/Soc_project/module/verilog/camara/clk24_25nexys4.v)). Como se puede ver:
 ![Screenshot](/Imagenes/camara2.png)
 Por otro lado tambien era necesario realizar la configuraci贸n de los registros de la c谩mara por medio de I2C, de donde nos servimos de un arduino para llevar a cabo dicho procedimiento.


## Configuraci贸n c谩mara por I2C (*Camara_OV.ino*)
Se establecen los valores de los registros de la c谩mara por medio del protocolo de comunicaci贸n serial I2C, se utilizo un microcontrolador Arduino para llevar a cabo este proceso y se describe en el archivo *Camara_OV.ino*. El c贸digo con el cual se establecen los registros y el formato de la c谩mara es el siguiente:
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
De igual forma tambien se establecen una serie de registros, que se denominaron "Registros magicos", con el fin de que la imagen obtenida por la c谩mara nos permita visualizar bien los colores del entorno (para mas informacion remitase a [CamaraOV7670](/datasheets/OV7670.pdf)).

## Pruebas de imagen

Al realizar la implementaci贸n del proyecto en verilog y subirlo a la FPGA, adem谩s de la correspondiente configuraci贸n de los registros en Arduino, nos percatamos de distintos escenarios favorables y desfavorables en los cuales se podria encontrar nuestra c谩mara al momento de las respectivas pruebas con el Robot cart贸grafo. Adicionalmente utilizamos la siguiente paleta de colores improvisada para comprobar la imagen que se est谩 recibiendo de la c谩mara. 

<p align="center">
  <img src="/Imagenes/20210815_153806.jpg" align="center" width="480">
</p>


### Prueba 1: Sin luz

Sin luz la paleta de colores es ligeramente visible para la c谩mara, impidiendo el an谩lisis para colores como verde y azul.
<p align="center">
  <img src="/Imagenes/20210815_153323.jpg" align="center" width="480">
</p>

### Prueba 2: Poca luz

Al encender un led blanco instalado en el robot cart贸grafo, la relfexi贸n que provoca este sobre la paleta de colores permite que se puedan distinguir m谩s los colores azul y verde.

<p align="center">
  <img src="/Imagenes/20210815_153356.jpg" align="center" width="480">
</p>

### Prueba 3: Con luz

Esta prueba se realiz贸 con lamparas de luz, siendo la situaci贸n ideal para la cual la c谩mara realiza una captura de datos correcta. Sin embargo cabe aclarar que son condiciones que cambian dependiendo de la luz ambiente. 

<p align="center">
  <img src="/Imagenes/20210815_153440.jpg" align="center" width="480">
</p>

### Prueba 4: Mucha luz

Al acercarle la lampara a la c谩mara, se distorsiona la imagen hasta el punto de realizar una captura de datos erronea, este caso nos impide reconocer claramente los colores de la paleta, y debe ser evitado.

<p align="center">
  <img src="/Imagenes/20210816_153254_capture.jpg" align="center" width="480">
</p>

En conclusi贸n, se necesitan condiciones id贸neas para que la c谩mara pueda capturar los datos correspondientes a los diferentes colores que se colocaran en el laberinto. Por lo contrario, se generan errores al momento de realizar el an谩lisis posterior de estos datos.

# Integraci贸n camara con el procesador
El funcionamiento de la c谩mara y el procesamiento de las imagenes se describen de forma general en el siguente diagrama, en donde se hace la integracion de los modulos en verilog mencionados anteriormente y el modulo en python [camara.py](/Soc_project/module/camara.py):

![Screenshot](/Imagenes/correc.png)


## Captura de datos ([cam_read.v](/Soc_project/module/verilog/camara/cam_read.v))
En este m贸dulo se realiza la captura de datos de la c谩mara y se env铆an a la memoria RAM, adem谩s de realizar la conversi贸n del formato RGB565 a RGB332. El diagrama de bloques que describe el proceso es el siguiente:

La conversi贸n del formato RGB se lleva a cabo por medio de un proceso denominado downsampling, y se basa en reducir el tama帽o de la informaci贸n seleccionando los datos m谩s significativos, despues de este proceso es posible almacenar la informaci贸n de un pixel en un byte a diferencia del formato RGB565 en donde se utilizaban dos bytes por pixel.

![Screenshot](/Imagenes/camara3.PNG)

El c贸digo utilizado para llevar a cabo este proceso es el siguiente:
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
Para entender el funcionamiento del c贸digo hay que tener en cuenta la forma como la c谩mara env铆a la informaci贸n, en la siguiente imagen se describe este proceso.

![Screenshot](/Imagenes/camara4.PNG)

El primer flanco de la se帽al **vsync** indica que la transmisi贸n de datos va a iniciar y el segundo indica que la transmisi贸n ha concluido, mientras que la se帽al **href** indica cuando se est谩 transmitiendo la informaci贸n de una l铆nea de pixeles. Teniendo en cuenta esto, en el c贸digo podemos observar que por cada ciclo del reloj de la c谩mara **pclk** se revisa inicialmente si la se帽al de entrada **init** tuvo un flanco de subida para que se de inicio al proceso de captura de datos. Posteriormente a esto se revisa si la se帽al **vsync** tuvo su primer flanco de bajada, lo cual significa que la c谩mara dar谩 inicio a la transmisi贸n de datos. 

Mientras la c谩mara se encuentre transmiendo datos, es dec铆r que la variable **start** sea igual a 2, se verifica si la se帽al **href** tiene valor alto para llevar a cabo el proceso de downsampling  revisando siempre si los datos corresponden al primer o al segundo byte del pixel por medio de la variable **countData** y llevando la cuenta de los pixels convertidos en la variable **count**. Finalmente se revisa si la se帽al **vsync** ya tuvo su segundo flanco de subida, indicando que la transmisi贸n de datos ha concluido, para reiniciar las variables y por medio de la salida **done** se帽alar que el proceso de captura de datos ha finalizado.   

## Procesamiento de im谩genes ([Analyzer.v](/Soc_project/module/verilog/camara/Analyzer.v))

Este m贸dulo se encarga de analizar las imagenes obtenidas de la captura de datos y determinar el color predominante en la imagen realizando el conteo de la intensidad de cada color en la imagen y finalmente haciendo una comparaci贸n entre los valores obtenidos. El c贸digo utilizado para llevar a cabo este proceso es el siguiente: 

```verilog
assign dataR = data[7:5];
assign dataG = data[4:2];
assign dataB = {data[2:0],1'b0};

always @(posedge clk)begin
		if(init && !init_old)begin
			start <= 1;
			done <= 0;
			sumaR <= 0;
			sumaG <= 0;
			sumaB <= 0;
		end
	if(start)begin
		if(count >= 19200)begin
			start <= 0;
			count <= 0;
			addr <= 15'h7fff;
			done <= 1;
			if((sumaR > sumaG) && (sumaR > sumaB))begin
				res = 3'b100;
			end else begin
				if((sumaG > sumaR) && (sumaG > sumaB))begin
				res = 3'b010;
				end else begin
					if((sumaB > sumaR) && (sumaB > sumaG))begin
						res = 3'b001;
					end else begin
						res =3'b111; //No hay ninguno mayor
					end
				end
			end
		end else begin
			addr <= addr+1;
			count <= count+1;
			sumaR <= #1 sumaR+dataR;
			sumaG <= #1 sumaG+dataG;
			sumaB <= #1 sumaB+dataB;
			done <= 0;
		end
	end 
	init_old = init;
end
```
Como se puede observar, inicialmente el byte de informaci贸n del pixel en formato RGB332 se descompone entre las variables **dataR**, **dataB** y **dataG** almacenando en cada variable el valor de intensidad en el pixel correspondiente a su color. Posteriormente se almacena en las variables **sumaR**, **sumaG** y **sumaB** el acumulado obtenido de todos los pixeles de la imagen para finalmente realizar la comparaci贸n e indicar en la se帽al de salida **res** cual es el color predominante en la imagen.  




