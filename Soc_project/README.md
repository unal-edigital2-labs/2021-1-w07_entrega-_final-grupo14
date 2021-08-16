# SoC :robot:
EL proyecto del robot cartógrafo se basa en la creación de un SoC (System of Chip). Dentro de los archivos dados en clase, contamos con un procesador picoRV32 y un bus de comunicación Wishbone por medio del cual se realiza la comunicacion del procesador, la memoria y los perifericos que se usaron a lo largo del proyecto. Para lograr nuestro objetivo contamos con Litex como una herramienta que nos permite crear el hardware del procesador sobre la FPGA, además contruye ciertos perifericos que están conjuntos al procesador al igual que la conexión con el bus de datos Wishbone (remetirse a [buildSoCproject.py](/Soc_project/buildSoCproject.py)).

![Screenshot](/Imagenes/Soc.png)

## Periféricos :electric_plug:
Aquí presentamos una lista de los periféricos que se conectaron al SoC.
- [Cámara](/Soc_project/module/verilog/camara/)
- [Radar](/Soc_project/module/verilog/ultrasonido/)
- [Motores](/Soc_project/module/verilog/motor/)
- [Infrarrojos](/Soc_project/module/verilog/infrarrojo/)
- [DFP player mini](/Soc_project/DFP_Player/)
- [Bluetooth](/Soc_project/Bluetooth/)
- [Arduino](/Arduino_Code_Cam_config/)

Para poder hacer la integración de los periféricos se crearon módulos en Verilog junto a unos módulos en lenguaje de Python para poder comunicarse con litex y de esta manera agregar el periferico al SoC. Un ejemplo del código utilizado para realizar este proceso es el siguiente: 

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
 
 
