# SoC :robot:
EL proyecto del robot cartografo se basa en que hace la creación de un SoC (system of Chip). Dentro de los archivos dados en clase contamos que usaremos un procesador picoRV32 y por medio de un bus de comunicación Wishbone que permitira la comunicacion del procesador,la memoria y los perifericos que se usaran a lo largo del proyecto. Para lograr nuestro objetivo contamos con Litex como una herramienta que nos permita crear el hardware del procesador sobre la FPGA, ademas nos contruye ciertos perifericos que vienen con el procesador al igual que la conexion con el bus de datos wishbone. Por lo tanto para poder integrar nuestro perifericos se crearon modulos en lenguaje de Verilog, a partir de esto debemos crear unos modulos en lenguaje de python para poder comunicarnos con litex y de esta manera agregar el periferico al SoC 

```python
    class Ultra(Module,AutoCSR):
    def __init__(self, trigg, ECHO):
    
        self.clk = ClockSignal() 
        self.orden = CSRStorage(1)
        self.d = CSRStatus(8) 
        self.DONE = CSRStatus(1)  
        self.ECHO = ECHO
        self.trigg = trigg
         

        self.specials +=Instance("bloqueultrasonido",
            i_clk = self.clk,
            i_orden = self.orden.storage,
            o_d = self.d.status,
            o_DONE = self.DONE.status,
            o_trigg=self.trigg,
            i_ECHO=self.ECHO,
        )
 ```
El codigo de arriba hace referencia al codigo usado para la implementacion del periferico del ultrasonido (**ultrasonido.py**). Dentro de este podemos observar que se inicializan los pines de entrada del modulo **ultrasonido.v** luego para crear los registros de memoria se usan dos función, la primera es CSRStatus() para registros de solo lectura cuyo argumento es el número de bits que usará, en este caso **"d"** son los pines de lectura y son 8 para obtener la distancia. Para las señales de lectura y escritura se usaran la función CSRStorage() como vemos es el caso de **"orden"** y por ultimo se hace la instanciación de nuestro modulo de verilog teniendo en cuenta los pines usados con sus espacios de memoria.

```python
    #ultrasonido
    SoCCore.add_csr(self,"ultra_cntrl")
    self.submodules.ultra_cntrl = ultrasonido.Ultra(platform.request("trigg"),platform.request("ECHO"))
```    
 Luego pasamos al archivo buildSocproject.py  dentro de este creamos las anteriores lineas de codigo en donde la primera se crea el espacio de memoria para el ultra sonido, luego en la segunda linea de codigo se relacionan los pines fisicos con la clase Ultra creada anteiormente dentro del archivo .py para que se realcionen con el modulo en verilog. lUego de esto solo basta compilar este archivo para que se guarden los cambios y la conexion entre el periferico y el SoC.
 
 
