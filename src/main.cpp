/// @file main.cpp

#include <iostream>
#include <mutex>
#include "robsim.h"
#include <queue>
#include <thread>
#include <atomic>

using namespace std;
atomic<bool> quit_now;	//!< Variable para indicar que terminen los bucles de los threads
mutex m_queue; 		//!< Mutex para manejo de mensajes entre los embebidos
mutex m_cout; 		//!< Mutex para impresión en pantalla
queue <string> q; 	//!< Cola de los mensajes entre los embebidos
mutex m_queue_cam;	//!< Mutex para el manejo de mensajes en la cola de comuncacion Raspi-Camara
queue <string> q_cam; 	//!< Cola de mensajes entre raspberry y camara


/** Funcion que corre en el th del arduinoR.
 */
void ArduinoRTh(){
	ArduinoR a1;
	a1.main();
}

/** Funcion que corre en el th del arduinoL.
 */
void ArduinoLTh(){
	ArduinoL a2;
	a2.main();
}

/** Funcion que corre en el th de la raspberry.
 */
void RPITh(){
	Raspi rpi;
	rpi.main();
}

/** Funcion que corre un thread para verificar si se presiona enter y terminar el programa.
 */
void quitTh()
{
      string s;
      while(1)
      {
           cin.get();
	   break;
      }
}

/** Bucle principal del programa.
 */
int main(){
	system("clear");
	
	unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
	print_initMenu();
	lck_cout.unlock();

	thread Quit(quitTh);
	thread AL(ArduinoLTh);
	thread AR(ArduinoRTh);
	thread RPI(RPITh);
	

	Quit.join();
	quit_now = true;
	RPI.join();
	AR.join();
	AL.join();

	lck_cout.lock(); // Agarro mutex
	print_endMenu();
	lck_cout.unlock();
	return 0;
}


