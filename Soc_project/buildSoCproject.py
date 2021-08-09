#!/usr/bin/env python3

from migen import *
from migen.genlib.io import CRG
from migen.genlib.cdc import MultiReg

import nexys4ddr as tarjeta
#import c4e6e10 as tarjeta

from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.interconnect.csr import *
from litex.soc.integration.soc import *

from litex.soc.cores import gpio
from litex.soc.cores import uart
from module import rgbled
from module import camara
from module import pwm
from module import ultrasonido
from module import infrarrojo
from module import motores



# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
	def __init__(self):
		platform = tarjeta.Platform()
		
		## add source verilog

		#Camara
		platform.add_source("module/verilog/Analyzer.v")
		platform.add_source("module/verilog/test_cam.v")
		platform.add_source("module/verilog/cam_read.v")
		platform.add_source("module/verilog/buffer_ram_dp.v")
		platform.add_source("module/verilog/clk24_25_nexys4.v")
		#platform.add_source("module/verilog/camara.v")
		
		
		#Radar
		#pwm
		platform.add_source("module/verilog/pwm.v")
		#ultrasonido
		platform.add_source("module/verilog//ultrasonido/bloqueultrasonido.v")
		platform.add_source("module/verilog//ultrasonido/contador.v")
		platform.add_source("module/verilog//ultrasonido/divisor.v")
		platform.add_source("module/verilog//ultrasonido/divisorfrec.v")
		platform.add_source("module/verilog//ultrasonido/divisorfrecd.v")
		platform.add_source("module/verilog//ultrasonido/divisorfrecgen.v")
		platform.add_source("module/verilog//ultrasonido/divisorfrecme.v")
		platform.add_source("module/verilog//ultrasonido/genpulsos.v")
		platform.add_source("module/verilog//ultrasonido/maquinadeestados.v")
		platform.add_source("module/verilog//ultrasonido/meultrasonido.v")
		platform.add_source("module/verilog//ultrasonido/ultrasonido.v")
		
		#infrarrojo
		platform.add_source("module/verilog/infrarrojo.v")
		
		#Motores
		platform.add_source("module/verilog/Motor.v")
		
		

		# SoC with CPU
		SoCCore.__init__(self, platform,
 			cpu_type="picorv32",
#			cpu_type="vexriscv",
			clk_freq=100e6,
			integrated_rom_size=0x8000,
			integrated_main_ram_size=16*1024)

		# Clock Reset Generation
		self.submodules.crg = CRG(platform.request("clk"), ~platform.request("cpu_reset"))

		# Leds
		SoCCore.add_csr(self,"leds")
		user_leds = Cat(*[platform.request("led", i) for i in range(10)])
		self.submodules.leds = gpio.GPIOOut(user_leds)
		
		# Switchs
		#SoCCore.add_csr(self,"switchs")
		#user_switchs = Cat(*[platform.request("sw", i) for i in range(8)])
		#self.submodules.switchs = gpio.GPIOIn(user_switchs)
		
		# Buttons
		SoCCore.add_csr(self,"buttons")
		user_buttons = Cat(*[platform.request("btn%c" %c) for c in ['c','r','l']])
		self.submodules.buttons = gpio.GPIOIn(user_buttons)
		

		# RGB leds
		SoCCore.add_csr(self,"ledRGB_1")
		self.submodules.ledRGB_1 = rgbled.RGBLed(platform.request("ledRGB",1))
		
		SoCCore.add_csr(self,"ledRGB_2")
		self.submodules.ledRGB_2 = rgbled.RGBLed(platform.request("ledRGB",2))
		
		
		#camara
		#SoCCore.add_csr(self,"camara_cntrl")
		#SoCCore.add_interrupt(self,"camara_cntrl")
		#CAM_px_data = Cat(*[platform.request("CAM_px_data", i) for i in range(8)])		
		#self.submodules.camara_cntrl = camara.Camara(platform.request("CAM_xclk"),platform.request("CAM_pclk"),platform.request("CAM_href"),platform.request("CAM_vsync"),CAM_px_data)
		
		#camara
		SoCCore.add_csr(self,"camara_cntrl")
		#SoCCore.add_interrupt(self,"camara_cntrl")
		cam_data_in = Cat(*[platform.request("CAM_px_data", i) for i in range(8)])		
		self.submodules.camara_cntrl = camara.Camara(platform.request("CAM_xclk"),platform.request("CAM_pclk"),platform.request("CAM_href"),platform.request("CAM_vsync"),cam_data_in)
		
		
		#pwm
		SoCCore.add_csr(self,"pwm_cntrl")
		self.submodules.pwm_cntrl = pwm.Pwm(platform.request("pwm"))
		
		#ultrasonido
		SoCCore.add_csr(self,"ultra_cntrl")
		self.submodules.ultra_cntrl = ultrasonido.Ultra(platform.request("trigg"),platform.request("ECHO"))
		
		#infrarrojo
		SoCCore.add_csr(self,"infra_cntrl")
		infras_in = Cat(*[platform.request("infras", i) for i in range(5)])
		self.submodules.infra_cntrl = infrarrojo.Infrarrojo(infras_in)
		
		#motores
		SoCCore.add_csr(self,"motor_cntrl")
		motor_out = Cat(*[platform.request("pin", i) for i in range(4)])
		self.submodules.motor_cntrl = motores.Motores(motor_out)
		
		
		#UART_1
		
		self.submodules.uart1_phy = uart.UARTPHY(
			pads     = platform.request("uart1"),
			clk_freq = self.sys_clk_freq,
			baudrate = 9600)
		self.submodules.uart1 = ResetInserter()(uart.UART(self.uart1_phy,
			tx_fifo_depth = 16,
			rx_fifo_depth = 16))
		self.csr.add("uart1_phy", use_loc_if_exists=True)
		self.csr.add("uart1", use_loc_if_exists=True)
		if hasattr(self.cpu, "interrupt"):
			self.irq.add("uart1", use_loc_if_exists=True)
		else:
			self.add_constant("UART_POLLING")		

	        #UART_2
		
		self.submodules.uart2_phy = uart.UARTPHY(
			pads     = platform.request("uart2"),
			clk_freq = self.sys_clk_freq,
			baudrate = 9600)
		self.submodules.uart2 = ResetInserter()(uart.UART(self.uart2_phy,
			tx_fifo_depth = 16,
			rx_fifo_depth = 16))
		self.csr.add("uart2_phy", use_loc_if_exists=True)
		self.csr.add("uart2", use_loc_if_exists=True)
		if hasattr(self.cpu, "interrupt"):
			self.irq.add("uart2", use_loc_if_exists=True)
		else:
			self.add_constant("UART_POLLING")
			
		#UART_3
		
		self.submodules.uart3_phy = uart.UARTPHY(
			pads     = platform.request("uart3"),
			clk_freq = self.sys_clk_freq,
			baudrate = 9600)
		self.submodules.uart3 = ResetInserter()(uart.UART(self.uart3_phy,
			tx_fifo_depth = 16,
			rx_fifo_depth = 16))
		self.csr.add("uart3_phy", use_loc_if_exists=True)
		self.csr.add("uart3", use_loc_if_exists=True)
		if hasattr(self.cpu, "interrupt"):
			self.irq.add("uart3", use_loc_if_exists=True)
		else:
			self.add_constant("UART_POLLING")
# Build --------------------------------------------------------------------------------------------
if __name__ == "__main__":
	builder = Builder(BaseSoC(),csr_csv="Soc_MemoryMap.csv")
	builder.build()

