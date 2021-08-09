# Motores ⚙️

Se utilizaron dos motores y un puente H (L298N) para el movimiento del robot cartógrafo, despues de varias pruebas se decidio alimentar a los motores con 12V para que tuvieran un desempeño óptimo. El puente H, según los valores de sus entradas, permite que el robot avance, gire o se detenga, en la siguiente tabla se resumen las acciones que puede realizar dependiendo del valor de sus entradas.
| Acción | S1 | S2 | S3 | S4 |
| ------------- | ------------- | ------------- |------------- |------------- |
| Avance | 0 | 1 | 1 | 0 |
| Retroceso | 1 | 0 | 0 | 1 |
| Giro Derecha | 1 | 0 | 1 | 0 |
| Giro Izquierda | 0 | 1 | 0 | 1 |
| Pausa | 0 | 0 | 0 | 0 |

