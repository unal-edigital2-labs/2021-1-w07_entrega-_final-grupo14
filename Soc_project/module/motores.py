from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
class Motores(Module,AutoCSR):
    def __init__(self, pin):
    
        self.clk = ClockSignal() 
        self.pin = pin
        self.estado = CSRStorage(3) 
         

        self.specials +=Instance("Motor",
            i_clk = self.clk,
            o_pin = self.pin,
            i_estado = self.estado.storage,
        )

