/** @file robsim.h
 *
 *
 * @author Francisco Ortiz
*/
#ifndef ROBSIM_H_
#define ROBSIM_H_

#include <queue>
#include "sensors.h"


#include <sys/time.h> //Para generar los eventos por time
#define MSG_TELEMETRY Message::Type::TELEMETRY
#define MSG_ACTION Message::Type::ACTION
#define MSG_CAMERA Message::Type::CAMERA

/**
 * Calcula el tiempo faltante para el siguiente intervalo
 *
 * @param[in] *t0 puntero a timeval con el tiempo incial del experimento
 * @param[in] n numero de intervalor
 * @param[in] dt paso de tiempo
 *
 * @return Tiempo en ms hasta el intervalo n
 */ 
int time_falt_next_interv_ms(struct timeval *t0,int n, double dt);

/**
 * Función bloqueante que espera hasta el siguiente intervalor
 *
 * @param[in] *t0 puntero a timeval con el tiempo incial del experimento
 * @param[in] n numero de intervalo
 * @param[in] dt paso de tiempo
 *
 */
void espera_siguiente_intervalo(struct timeval *t0,int n, double dt);


/**a
 * Clase base Sistema embebido  
 * Tiene como metodo virtual puro el main de los SE
 */
class EmbededSystem{
	public:
		virtual void main()=0; //!< Funcion virtual pura que corre el main del SE
};


/**
 * Clase que representa el Arduino dedicado al sistema 1
 * El metodo main es el bucle infinito del SE
 * Hereda de la clase Sistema Embebido
 */
class ArduinoR : public EmbededSystem{
	public:
		void main(); 
		std::string readIMU();
};

/**
 * Clase que representa el Arduino dedicado al sistema 2
 * El metodo main es el bucle infinito del SE
 */
class ArduinoL : public EmbededSystem{
	public:
		void main();
};
/**
 * Clase que representa la Raspberry  
 * El metodo main es el bucle infinito del SE
 */
class Raspi : public EmbededSystem{
	public:
		void main();
};

/**
 * Clase que representa la camara
 * El metodo main es el bucle infinito del SE
 */
class Camera : public EmbededSystem{
	public:
		void main();
};

/**
 * Clase base actuadores 
 */
class Actuator{
};

/* Queda por implementar algo de esto
 * Clase Stepper 
class StepperMotor : public Actuator{
};
 */

/**
 * Clase JetPump 
 *
 * Representa una bombita del robot
 * Tiene como atributo la tensión en bornes de la bomba
 * y metodos que permiten setear y leer este valor
 */
class JetPump : public Actuator{
	public:
		JetPump():V(0){} //!< Constructor por defecto pone la tensión de la bomba en 0
		/**
		 * Función que setea el valor de tensión de la bomba
		 * @param[in] V_: Tensión que quiero colocar en la bomba
		 */
		void set(double V_){V = V_;}; 
		/**
		 * Función que retorna el valor de la tensión actual de la bomba
		 *
		 *  @return Tensión en la bomba
		 */
		double get(){return V;};
	private:
		double V; //!< Tensión de la bomba
};

/**
 * Clase base sensores 
 */
class Sensor{
};

/**
 * Clase que representa el sensor de dist  
 */
class DistSens:public Sensor{
	
};
/**
 * Clase Mensaje  
 */
class Message{
	public:
		enum class Type {TELEMETRY,ACTION,CAMERA};
		Type getType() const;
		void setType(Type t){type = t;};
		void send_msg(std::queue <std::string>& q);
		void write_msg(std::string msg_);
		void print_msg();
		void delete_msg(void);
		Message(std::string msg_);
		Message();
		//Message():msg(""){};
	private:
		Type type;
		std::string msg;
};

/**
 * Clase mensaje de telemetria 
 */
class TelemetryMsg: public Message{
	public:
		TelemetryMsg();
		/** Metodo que recibe los valores de aceleración de la IMU y lo escribe en un msj (atributo de clase base
		 * @param[in] IMUx: Aceleración en x medida por la IMU
		 * @param[in] IMUy: Aceleración en y medida por la IMU
		 * @param[in] IMUz: Aceleración en z medida por la IMU
		 */
		void write_IMU(double IMUx,double IMUy,double IMUz);
};

/**
 * Clase mensaje de accion de control
 */
class ActionMsg: public Message{
	public:
		ActionMsg();
		
};

/**
 * Clase mensaje de camara 
 */
class CameraMsg: public Message{
	public:
		CameraMsg();
		/** Metodo que recibe los valores de posicion y lo escribe en un msj (atributo de clase base
		 * @param[in] camX: Posición en x medida por la camara
		 * @param[in] camY: Posición en y medida por la camara
		 * @param[in] camZ: Posición en z medida por la camara
		 */
		void write_pos(double camX,double camY,double camZ);
};

#endif
