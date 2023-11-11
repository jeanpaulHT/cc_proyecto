## Datos 
Los datos proporcionados representan informacion relevante a los viajes en el servicio de Taxis de la ciudad de Nueva York en los últimos años, recaudados por la compañia *NYC Taxi and Limousine Commission* (https://drive.google.com/file/d/1NK96l1cQvVUjpmaAqmEpHPZ4dtgaZjRT/view).   
## Próposito
Se implemento un R-Tree para soportar la siguiente consulta:  
* Dada la posición de dos puntos P1 y P2 que representan una región rectangular, retornar los viajes que comenzaron en tal región.
Considerar, además, que la posición de un punto está determinada por la **latitud** y **longitud** de las coordenadas que representa.
## Estructura
Un R-Tree es un árbol indexado que permite agrupar a los elementos según su proximidad. Para ello, divide el espacio creando regiones (*Bounding Boxes* y para esta implementación en concreto: *Bounding Rectangles*), de forma que la repetición de elementos en estas sea mínima.

<img src="https://github.com/ecordovaa/cc_proyecto/blob/main/src/Rtree.png" width="400" height="280" class="center">

# Implementacion en Cloud

La arquitectura propuesta es la siguiente:

<img src="https://media.discordapp.net/attachments/707425093256609834/1172379957025050794/image.png?width=1920&height=592" width="800" height="280" class="center">

Los servicios de AWS que utilizarian serian:
* Api Gateway: Para poder levantar el lambda de acuerdo a un request
* Lambda: Para poder levantar una maquina ecs a demanda
* AWS Fargate: Levantar la imagen docker del ETL
* S3: Almacenar los archivos por la maquina del Fargate


Se utilizan las herramientas de AWS para poder generar una ejecución a demanda del proceso de descarga y procesamiento de los archivos fuente de la data.

Después estos archivos son subidos al ecosistema de snowflake, en donde son almacenados en formato sql para luego ser usados con la integración de streamlit para poder visualizar la data de acuerdo a las queries hechas.



# Almacenamiento de la data

Los archivos descargados se están almacenando en bucket s3 particionados por año y mes. Estos archivos están en formato csv.

<img src="https://media.discordapp.net/attachments/707425093256609834/1172757941334577172/image.png?ex=65617af2&is=654f05f2&hm=167a711044729e93e6caf7db2dcf2fbac95c5f38c22c0eb2db3ab3dc09326561" width="800" height="280" class="center">

Usando los conectores de Snowflake podemos cargar la data del s3 hacía la tabla generada en el datawarehouse.
<img src="https://media.discordapp.net/attachments/707425093256609834/1172761423257022484/image.png?ex=65617e30&is=654f0930&hm=4cbb58b72e3a08b02465f9fcd46497305c9668eab6e27ab05be93aff29edfcc5&=&width=1920&height=768" width="800" height="280" class="center">

Esta data luego es almacenada en la tabla creada en snowflake.
<img src="https://cdn.discordapp.com/attachments/707425093256609834/1172916017635270666/image.png?ex=65620e2a&is=654f992a&hm=dc072f384145b446b7ee1ae9a277d690e61794808aa3a1ad0b6358362d15fb31&" width="600" height="200" class="center">

Finalmente con la data en snow podemos conectarla a la aplicación de streamlit para poder visualizar y finalmente hacer consultas.
<img src="https://media.discordapp.net/attachments/707425093256609834/1172916416098349158/image.png?ex=65620e89&is=654f9989&hm=70093bac1af402f69c8d0c265a0119cdae689f97bcbf3674d015d7809192ca44&" width="800" height="400" class="center">
