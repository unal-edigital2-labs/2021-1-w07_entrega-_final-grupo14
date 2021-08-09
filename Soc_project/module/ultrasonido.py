from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
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

