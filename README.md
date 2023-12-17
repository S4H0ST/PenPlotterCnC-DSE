Pen Plotter CnC 
---
Proyecto que tiene como objetivo la creación de un pen plotter Cnc utilizando 3 motores stepper y un servo motor para la redacción de caracteres alfanumericos utilizando un código enteramente de Arduino.

La formación de estos caracteres estan representados con un metodo matricial de coordenadas de 3x3:

![bresenham](./Media/Matriz.png)

Donde para hacer las diagonales se ha utilizado la libreria multistepper, pero se ha inspirado en el algoritmo de Bresenham:

Para poder entender el algoritmo de bresenham es importante también tener varios conceptos previos como son el de la línea, tanto en trazado de una línea como el poder encontrar la línea a partir de dos puntos.

![bresenham](./media/Bresenham.gif)

También usa un mecanismo para iniciar y finalizar a una coordenada conocida, utilizando lo que se denomina ***Homing***.