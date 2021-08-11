# SoC :robot:

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
