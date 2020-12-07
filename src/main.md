@mainpage Simulador Sistemas Embebidos Robotino

  
Esta es la documentación del proyecto final de la catedra de Programación Orientada a Objetos en C++.

## Objetivos
![Caption text](../img/Finalc++.png)

El objetivo es simular la comunicación de 2 arduinos y una raspberry. 
Para esto se utilizan la siguiente estructura de disparo de threads:
- Main
	- Arduino R: Main
	- Arduino L: Main
	- Raspberry Pi
		- Main
		- Camara
	


## Comunicaciones

Para simular la comunicación utilizo una QUEUE con strings como elementos donde voy metiendo los mensajes que quiero mandar a otros dispositivos, y el otro dispositivo lee si le corresponde y si le corresponde lo pop, sino no hace nada.

Una queue es para la simulación entre los arduinos y la raspberry y otra queue par ala comunicación dentro de la raspberry con la camara.

## Envio de datos
A continuación se muestra la estructura de los mensajes de cada dispositivo.
### IMU

!DAT:IMU:xxxx:yyyy:zzzz:#

### CAMERA

!DAT:CAM:xxxx:yyyy:zzzz:#

### LASER


!DAT:LAS:xxxx:#
