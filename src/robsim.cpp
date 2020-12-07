/// @file robsim.cpp
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
//#include <fstream>
#include <vector>
#include <iomanip>
//#include <ctime>
#include <atomic>
#include <cmath>

#include "sensors.h"
#include "robsim.h"
#include "utils.h"
#include "timer.h"

#define SAMPLE_TIME_ARDUINO 0.1;
#define SAMPLE_TIME_CAMERA 0.1;
#define SAMPLE_TIME_RASPI 0.1;

using namespace std;


int time_falt_next_interv_ms(struct timeval *t0,int n, double dt){
	double t_es;
	struct timeval ta;
	gettimeofday(&ta,NULL);
	t_es = (double)n*dt + (t0->tv_sec+t0->tv_usec*1e-6)-(ta.tv_sec+ta.tv_usec*1e-6);

	return (int)t_es*1000;
}

void espera_siguiente_intervalo(struct timeval *t0,int n, double dt){
	double t_es;
	struct timeval ta;
	struct timespec ts;
	gettimeofday(&ta,NULL);
	t_es = (double)n*dt + (t0->tv_sec+t0->tv_usec*1e-6)-(ta.tv_sec+ta.tv_usec*1e-6);

	if(t_es>0.0){
		ts.tv_sec = (int)t_es;
		ts.tv_nsec = (t_es-ts.tv_sec)*1e9;
		nanosleep(&ts,NULL);
	}
//	return t_es;
}

/*
 * ACA TOdas las func relacionadas a MSG
 */ 

void Message::send_msg(queue <string>& q){
	// --- Parte mutex
	switch(type){
		case MSG_TELEMETRY:
		{		
			extern mutex m_queue;
			unique_lock<mutex> lck(m_queue); // Agarro mutex
			q.push(msg);
			lck.unlock();
		}
			break;
		case MSG_ACTION:
		{
			extern mutex m_queue;
			unique_lock<mutex> lck(m_queue); // Agarro mutex
			q.push(msg);
			lck.unlock();
		}
			break;
		case MSG_CAMERA:
		{
			extern mutex m_queue_cam;
			unique_lock<mutex> lck(m_queue_cam); // Agarro mutex
			q.push(msg);
			lck.unlock();
		}
			break;
		default:
			cout << "Default" << endl;
	}
}

void Message::delete_msg(void){
	// Restarteo el mensaje de 0
	msg = "!DAT:";
}
void Message::write_msg(string msg_){
	if(msg_ !="#")
		msg = msg + msg_ + ":";
	else
		msg = msg + msg_;
}
void Message::print_msg(){cout << "msg: " << msg << endl;}

Message::Message(string msg_){
	msg = msg_;
}
Message::Message(void){
	msg = "";
}

TelemetryMsg::TelemetryMsg(void){
	write_msg("!DAT");
	setType(MSG_TELEMETRY);
}


ActionMsg::ActionMsg(void){
	write_msg("!DAT");
	setType(MSG_ACTION);
}

CameraMsg::CameraMsg(void){
	write_msg("!DAT");
	setType(MSG_CAMERA);
}

void CameraMsg::write_pos(double camX,double camY,double camZ){
	this->write_msg("CAM");
	this->write_msg(to_string((int) camX));
	this->write_msg(to_string((int) camY));
	this->write_msg(to_string((int) camZ));
	this->write_msg("#");
}
void TelemetryMsg::write_IMU(double IMUx,double IMUy,double IMUz){
	this->write_msg("IMU");
	this->write_msg(to_string((int) IMUx));
	this->write_msg(to_string((int) IMUy));
	this->write_msg(to_string((int) IMUz));
	this->write_msg("#");
}
/*
 * ACA TOdas las func relacionadas a Embedded
 */ 
void Camera::main(){
	// Leo los datos de la camara
	extern queue <string> q_cam;
	extern atomic<bool> quit_now;
	vector<vector<double>> data = Read_CSV_IMU_CAM("CAM_tray_01.csv");
	
	CameraMsg msg_CAM;
	
	srand(time(NULL));
	
	double dt,camX,camY,camZ;
	dt = SAMPLE_TIME_CAMERA;
	int i=0;
	struct timeval t0,ti;
	// Arranco
	gettimeofday(&t0,NULL);
	while(!quit_now){
		gettimeofday(&ti,NULL);
	//	cout << "CAM: "<< x << endl;
	// --------------------- ACA EL CODIGO
		// Escribo el mensaje con los datos de la IMU
		camX = data[i][0]*200; camY = data[i][1]*200; camZ = data[i][2]*200;	
		msg_CAM.write_pos(camX,camY,camZ);
		msg_CAM.send_msg(q_cam);
		msg_CAM.delete_msg();

		espera_siguiente_intervalo(&t0,i+1,dt);
		i++;
	}
}



void ArduinoL::main(){
	extern mutex m_cout;
	extern atomic<bool> quit_now;

	srand((int) time(0));

	unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
	cout << "\33[5;0H| ArduinoL: INICIO"; 
	cout << "\r" << flush << "\33[79C|" << endl;
	lck_cout.unlock();
	
	extern queue <string> q;
	//	Creo un mensaje con los datos de la IMU
	TelemetryMsg msg_LASER;
	
	double dt=SAMPLE_TIME_ARDUINO;
	bool flag_LAS_MSG = false;
	Timer t1([&](){ flag_LAS_MSG = true; }, dt*1000);
	t1.start();
	while(!quit_now){
		// Bucle controlado
		if(flag_LAS_MSG==true){
			flag_LAS_MSG = false;
			msg_LASER.write_msg("LAS");
			int v = (rand() %10) -5;
			msg_LASER.write_msg(to_string(v));
			msg_LASER.write_msg("#");
			// Printeo msg para ver q ta bien
	//		msg_IMU.print_msg();
			msg_LASER.send_msg(q);
			msg_LASER.delete_msg();
		}
	}
	t1.stop();
	lck_cout.lock(); // Agarro mutex
	cout << "\33[12;0H ArduinoL: FIN" << endl;
	lck_cout.unlock();
}

void f(bool *flag){
	*flag = true;
}

void ArduinoR::main(){
	MPU9250 IMU;
	JetPump Bombita;
	extern mutex m_cout;
	extern mutex m_queue;
	extern atomic<bool> quit_now;

	unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
	cout << "\33[6;0H| ArduinoR: INICIO"; 
	cout << "\r" << flush << "\33[79C|" << endl;
	lck_cout.unlock();
	
	extern queue <string> q;
	//	Creo un mensaje con los datos de la IMU
	TelemetryMsg msg_IMU;
	int i=0;
	// Leo los datos
	vector<vector<double>> data = Read_CSV_IMU_CAM("IMU_tray_01.csv");

	double dt = SAMPLE_TIME_ARDUINO;
	bool flag_IMU_MSG = false;
	Timer t1([&](){ flag_IMU_MSG = true; }, dt*1000);
	t1.start();
	double v[3];
	string t;
	while(!quit_now){
		// Bucle controlado
		//
		// ----- PARTE IMU
		// Leo la IMU
		if(flag_IMU_MSG==true){
			IMU.readAccelData(IMU.accelCount,data,i);
			IMU.ax = (float)IMU.accelCount[0];
			IMU.ay = (float)IMU.accelCount[1];
			IMU.az = (float)IMU.accelCount[2];
			
			// Escribo el mensaje con los datos de la IMU
			msg_IMU.write_IMU(IMU.ax,IMU.ay,IMU.az);

			// Printeo msg para ver q ta bien
			//msg_IMU.print_msg();
			msg_IMU.send_msg(q);
			msg_IMU.delete_msg();
			flag_IMU_MSG = false;
		}
		// ----- PARTE JetPump
		// -------------------------
		// ---- LECTURA DE MENSAJES
		// -------------------------
		// Reviso si la queue de ard tiene un msg
		unique_lock<mutex> lck_embeded(m_queue); // Agarro mutex EMBEDED
		if(!q.empty()) {
			t = decode_msg(q.front(),v);
			if(t==MSG_BOM){
				Bombita.set(v[0]);
				lck_cout.lock();
				cout  << "\33[10;0H";
				cout << "\33[2K" ;
				cout << "| Bomba Seteada en: " << Bombita.get() <<"\33[10;80H|" <<endl;
				lck_cout.unlock();
				q.pop();
			}
		}
		lck_embeded.unlock();
	}
	t1.stop();
	this_thread::sleep_for(std::chrono::microseconds(200));
	lck_cout.lock(); // Agarro mutex
	cout << "\33[13;0H ArduinoR: FIN" << endl;
	lck_cout.unlock();
}


void CamTh(){
	Camera cam;
	cam.main();
}

void Raspi::main(){
	extern mutex m_cout;
	extern mutex m_queue;
	extern mutex m_queue_cam;
	extern queue <string> q;
	extern queue <string> q_cam;
	extern atomic<bool> quit_now;
	bool flag_IMU, flag_CAM;
	ActionMsg msg_JetPump; // Creo el mensaje pa mandar al ARD

	flag_IMU = false; flag_CAM=false;
	unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
	cout << "\33[7;0H| Raspberry: INICIO"; 
	cout << "\r" << flush << "\33[79C|" << endl;
	cout << "\33[8;0H|";
	for(int i=0;i<78;i++)cout<<"-";
	cout << "|" << endl;
	lck_cout.unlock();
// ---- pal th de la camara
	thread CAM(CamTh);


	// Vectores donde guardo las mediciones decodificadas
	double a_IMU[3],p_CAM[3],p_IMU[3],p_estimada[3];
	p_IMU[0] = 0;p_IMU[1] = 0;p_IMU[2] = 0; // Inicializo en 0 el vector de posición de la IMU
	string t;
	string command;
	while(!quit_now){
		// -------------------------
		// ---- LECTURA DE MENSAJES
		// -------------------------
		// Reviso si la queue de ard tiene un msg
		unique_lock<mutex> lck_embeded(m_queue); // Agarro mutex EMBEDED
		if(!q.empty()) {
			// Tengo medicion de IMU, pongo el flag a true
			double v[3];
			t = decode_msg(q.front(),v);
			if(t==MSG_IMU){
				flag_IMU = true;
			       	a_IMU[0] = v[0]; a_IMU[1] = v[1]; a_IMU[2] = v[2];
				q.pop();
			}
			if(t==MSG_LAS){
				unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
				cout  << "\33[11;0H| Laser:" << v[0] <<"\33[11;80H|"<< endl;
				lck_cout.unlock();
				q.pop();
			}
		}
		lck_embeded.unlock();
	// Reviso si la queue de la camara tiene un msg
		unique_lock<mutex> lck_cam(m_queue_cam); // Agarro mutex CAMARA
		if(!q_cam.empty()) {
			// Tengo medicion de camara, pongo el flag a true
			flag_CAM = true;
			double v[3];
			t = decode_msg(q_cam.front(),v);
			if(t==MSG_CAM){
			       	p_CAM[0] = v[0]; p_CAM[1] = v[1]; p_CAM[2] = v[2];
				q_cam.pop();
			}
		}
		lck_cam.unlock();
		if(flag_IMU & flag_CAM){
			// Si tengo las mediciones estimo la nueva posición (PROMEDIO)
			unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
			cout  << "\33[9;0H";
			cout << "\33[2K" ;
			cout << "| Posición estimada: ";
			lck_cout.unlock();
			string coord = "xyz";	
			double dt = SAMPLE_TIME_ARDUINO;
			for(int i=0;i<3;i++){  
				p_IMU[i] = 0.5*dt*dt*a_IMU[i] + p_IMU[i];
				p_estimada[i] = (p_IMU[i]+p_CAM[i])/2.0;		
				unique_lock<mutex> lck_cout(m_cout); // Agarro mutex
				cout << "\t" << coord[i] << ":" << setprecision(3) <<p_estimada[i]  ;
				lck_cout.unlock();
			}
			lck_cout.lock(); // Agarro mutex
			cout <<"\33[9;80H|"<< endl;
			lck_cout.unlock();

			flag_IMU = false; flag_CAM=false;

			// -------------------------
			// ---- ENVIO DE MENSAJES
			// -------------------------
			msg_JetPump.write_msg("BOM");
			//cout << p_estimada[0] << endl;
			msg_JetPump.write_msg(to_string((int) round(p_estimada[0])));
			msg_JetPump.write_msg("#");
			// Printeo msg para ver q ta bien
			//msg_JetPump.print_msg();
			msg_JetPump.send_msg(q);
			msg_JetPump.delete_msg();

			lck_cout.lock(); // Agarro mutex
			lck_cout.unlock();
		}
	}
	this_thread::sleep_for(std::chrono::milliseconds(100));

	CAM.join();
	lck_cout.lock();
	cout << "\33[14;0H Raspberry: FIN" << endl;
	lck_cout.unlock();

}




