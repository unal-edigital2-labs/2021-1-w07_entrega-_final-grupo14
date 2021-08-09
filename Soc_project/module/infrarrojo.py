from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
class Infrarrojo(Module,AutoCSR):
    def __init__(self, infras):
     
        self.infras = infras
        self.infras2 = CSRStatus(5)

        self.specials +=Instance("infrarrojo",
            i_infras = self.infras,
            o_infras2 = self.infras2.status,
        )

