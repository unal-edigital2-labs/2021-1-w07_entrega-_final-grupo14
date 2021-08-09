from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
class Camara(Module,AutoCSR):
    def __init__(self, xclk,pclk,href,vsync,cam_data_in):
        self.clk = ClockSignal()   
        self.rst = ResetSignal()  
        self.xclk = xclk
        self.pclk = pclk
        self.vsync= vsync
        self.href= href
        self.px_data = cam_data_in
        
        
        self.done= CSRStatus(1)
        self.init = CSRStorage(1)
        self.error = CSRStatus(1)
        self.res = CSRStatus(3)  

        self.specials +=Instance("test_cam",
            i_clk = self.clk,
            i_rst = self.rst,
            o_CAM_xclk = self.xclk,
            i_CAM_pclk = self.pclk,
            i_CAM_px_data = self.px_data,
            i_CAM_vsync = self.vsync,
            i_CAM_href = self.href,
            o_done =self.done.status,
            o_res = self.res.status,
            o_error = self.error.status,
            i_init = self.init.storage,
        )
        
        
        #self.submodules.ev = EventManager()
        #self.ev.ok = EventSourceProcess()
        #self.ev.finalize()
 
        #self.ev.ok.trigger.eq(self.done.status)
