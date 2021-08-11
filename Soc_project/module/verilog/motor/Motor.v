`timescale 1ns / 1ps

module Motor(
    input clk,
    input [2:0] estado,
    output reg [3:0] pin
);


parameter AVANCE=2, RETROCESO=1, PAUSA=0, GIROD=3, GIROI=4;

always @(posedge clk) 
begin

      case(estado)
        AVANCE: pin = 4'b0110;
        RETROCESO: pin = 4'b1001;
        PAUSA: pin = 4'b0000;
        GIROD: pin = 4'b0101;
        GIROI: pin = 4'b1010;
      default:
        pin = 4'b0000;
      endcase
  end
endmodule
