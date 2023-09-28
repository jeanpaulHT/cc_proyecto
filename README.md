## Datos 
Los datos proporcionados representan informacion relevante a los viajes en el servicio de Taxis de la ciudad de Nueva York durante el año 2015, recaudados por la compañia *NYC Taxi and Limousine Commission* (https://drive.google.com/file/d/1NK96l1cQvVUjpmaAqmEpHPZ4dtgaZjRT/view).   
## Próposito
Se implemento un R-Tree para soportar la siguiente consulta:  
* Dada la posición de dos puntos P1 y P2 que representan una región rectangular, retornar los viajes que comenzaron en tal región.
Considerar, además, que la posición de un punto está determinada por la **latitud** y **longitud** de las coordenadas que representa.
## Estructura
Un R-Tree es un árbol indexado que permite agrupar a los elementos según su proximidad. Para ello, divide el espacio creando regiones (*Bounding Boxes* y para esta implementación en concreto: *Bounding Rectangles*), de forma que la repetición de elementos en estas sea mínima.

![Estructura del R-Tree](https://github.com/ecordovaa/cc_proyecto/tree/main/src/Rtree.png)
