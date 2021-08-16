# Motores ⚙️

Se utilizaron dos motores y un puente H (L298N) para el movimiento del robot cartógrafo, despues de varias pruebas se decidio alimentar a los motores con 12V para que tuvieran un desempeño óptimo. El puente H, según los valores de sus entradas, permite que el robot avance, gire o se detenga, en la siguiente tabla se resumen las acciones que puede realizar dependiendo del valor de sus entradas.
| Acción | S1 | S2 | S3 | S4 |
| ------------- | ------------- | ------------- |------------- |------------- |
| Avance | 0 | 1 | 1 | 0 |
| Retroceso | 1 | 0 | 0 | 1 |
| Giro Derecha | 1 | 0 | 1 | 0 |
| Giro Izquierda | 0 | 1 | 0 | 1 |
| Pausa | 0 | 0 | 0 | 0 |

Teniendo en cuenta la anterior tabla se desarrollo el módulo ***Motor.v*** que cumple la función de driver para los motores. El código utilizado es el siguiente:

```verilog
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
  ```
      
El funcionamiento del módulo se basa en que según el valor de la señal de entrada *estado* se establecen los valores de los 4 pines de salida siguiendo la tabla descrita anteriormente para que los motores realicen la acción deseada. 

![Screenshot](/Imagenes/puenteH.png)
