# SoC :robot:
EL proyecto del robot cartografo se basa en que hace la creación de un SoC (system of Chip). Dentro de los archivos dados en clase contamos que usaremos un procesador picoRV32 y por medio de un bus de comunicación Wishbone que permitira la comunicacion del procesador,la memoria y los perifericos que se usaran a lo largo del proyecto. Para lograr nuestro objetivo contamos con Litex como una herramienta que nos permita crear el hardware del procesador sobre la FPGA, ademas nos contruye ciertos perifericos que vienen con el procesador al igual que la conexion con el bus de datos wishbone. Por lo tanto para poder integrar nuestro perifericos se crearon modulos en lenguaje de Verilog, a partir de esto debemos crear unos modulos en lenguaje de python para poder comunicarnos con litex y de esta manera agregar el periferico al SoC 

```python
    class Infrarrojo(Module,AutoCSR):
    def __init__(self, infras):
     
        self.infras = infras
        self.infras2 = CSRStatus(5)

        self.specials +=Instance("infrarrojo",
            i_infras = self.infras,
            o_infras2 = self.infras2.status,
        )
 ```
El codigo de arriba hace referencia al codigo usado para la implementacion del periferico de los infrarrojos (**infrarrojo.py**). Dentro de este podemos observar que se inicializan las pines de entrada del modulo **infrarrojos.v** luego se le asigna la cantidad de bits que posee esta entrada o salida, y por ultimo se hace la instancia.

```python
    #infrarrojo
    SoCCore.add_csr(self,"infra_cntrl")
    infras_in = Cat(*[platform.request("infras", i) for i in range(5)])
    self.submodules.infra_cntrl = infrarrojo.Infrarrojo(infras_in)
```    
