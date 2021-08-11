# SoC :robot:


Este proyecto pasa por la creación de un SoC (System On Chip) que integra gran variedad de cosas de acuerdo a nuestras necesidades. En nuestro caso, hacemos uso de un procesador de AMD y por medio de buses de comunicación lo conectamos con memorias, registros y módulos hechos en el lenguaje Verilog que corresponden a los implementos necesarios para formar un robot cartógrafo que es capaz de recorrer cierta variedad de laberintos de manera autónoma. El proceso para realizar un SoC se realizó con el uso de una herramienta de código abierto llamada Litex, la cual nos permite ensamblar de una manera sencilla todos los módulos separados de FPGA que podamos llegar a tener (claro, sin exceder las capacidades de la tarjeta). En esta sección, veremos cómo es el proceso para lograr esta integración.



Litex sirve para construir hardware, nos da el procesador y contruye hw sobre la FPGa, ademas nos contruye ciertos perifericos que vienen con el procesador y la conexion con el punte wishbone y admeas a integrar los modulos en v por medio de py


```python
    #infrarrojo
    SoCCore.add_csr(self,"infra_cntrl")
    infras_in = Cat(*[platform.request("infras", i) for i in range(5)])
    self.submodules.infra_cntrl = infrarrojo.Infrarrojo(infras_in)
```    
    
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
