/// @file robsim.h

#ifndef ROBSIM_H_
#define ROBSIM_H_


/**
 * Clase que representa un Arduino  
 */
class Arduino{
	int ID;
};

/**
 * Clase que representa un Raspberry  
 */
class Raspi{
	int ID;
};

/**
 * Clase Mensaje  
 */
class Message{
	public:
		void send_msg();
		void write_msg();
	private:
		Type type;
			
};

/**
 * Clase mensaje de telemetria 
 */
class TelemetryMsg: public Message{


};

/**
 * Clase mensaje de accion de control
 */
class ActionMsg: public Message{


};

#endif
