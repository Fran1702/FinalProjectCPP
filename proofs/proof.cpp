//#include "funciones.h"
#include "timer.h"
//#include <stdio.h>
//#include <stdlib.h>
//#define NMUESTRAS 1000

#include <string>
#include <fstream>
#include <vector>
//#include <utility> // std::pair
//#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

#define MSG_DELIMITER ":" 
#define MSG_IMU "msg_IMU"
#define MSG_CAM "msg_CAM"

vector<vector<double>> Read_CSV_IMU_CAM(string inputFileName) {
 
    vector<vector<double> > data;
    ifstream inputFile(inputFileName);
    int l = 0;
 
    while (inputFile) {
        l++;
        string s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            istringstream ss(s);
            vector<double> record;
 
            while (ss) {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) {
                    cout << "NaN found in file " << inputFileName << " line " << l
                         << endl;
                    e.what();
                }
            }
 
            data.push_back(record);
        }
    }
 
    if (!inputFile.eof()) {
        cerr << "Could not read file " << inputFileName << "\n";
        __throw_invalid_argument("File not found.");
    }
 
    return data;
}
 
string decode_msg(string s,double v[3]){
	string ret;	
	try{
		string delimiter = MSG_DELIMITER;
		size_t pos = 0;
		string token;
		pos = s.find(delimiter);
		token = s.substr(0, pos);
		pos = s.find(delimiter);
		if(token=="!DAT"){
			s.erase(0, pos + delimiter.length());
			cout << s << endl;
			token = s.substr(0, s.find(delimiter));
			cout << token << endl;
			if(token=="IMU"){
				ret = MSG_IMU;
				for(int i=0;i<3;i++){
					pos = s.find(delimiter);
					s.erase(0, pos + delimiter.length());
					token = s.substr(0, s.find(delimiter));
					v[i] = (double) stoi(token);
					cout << "a: " << v[i] << endl;
				}
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));

			}
			if(token=="CAM"){
				ret = MSG_CAM;
				for(int i=0;i<3;i++){
					pos = s.find(delimiter);

					v[i] = (double) stoi(token);
					cout << "a: " << v[i] << endl;
				}
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));
			}
			if(token!="#"){
				throw token;
			}
		}
	}
	catch(string token){
		cout << "Excepción Fin de string invalido: ";
		cout << token << endl;
			return "error";
	}
	catch(...){
		cout << "Excepción msg" << endl;
			return "error";
	}

			return ret;

}
int main()
{	
	int countMeUp = 0;
	bool flag_IMU_MSG = false;
	Timer t1([&](){
			flag_IMU_MSG = true;
	}, 1000);

	t1.start();
	while(1){
		if(flag_IMU_MSG){
			countMeUp++;
			cout << countMeUp << endl;
			flag_IMU_MSG = false;
		}
	}
//	cin.get(d);
	t1.stop();

//	string s = "!DAT:CAM:54:82:-65:#";
//	string s = "!DAT:CAM:54:";
//	string t;
//	double vec[3];
//	t = decode_msg(s,vec);
	

//		*/
	

//  move(10,10);
 // refresh();
  //getch();

/*
    	vector<vector<double>> data = Read_CSV_IMU_CAM("IMU_tray_01.csv");
 
//    for (auto l : data) {
//        for (auto x : l)
        double x;
	x = data[0][0];
            cout << x << " ";
        cout << endl;
//    }
 */
    return 0;
}




/*
int main( int argc, char *argv[]) {
    int i;
    double dt = 0.10;

    double te[NMUESTRAS], t[NMUESTRAS];  
    struct timeval t0,ti;
    // Comienzo de experimento
    gettimeofday(&t0,NULL);
    for (i=0; i<NMUESTRAS; i++) { 
  		// Bucle Controlado
		gettimeofday(&ti,NULL); 
		t[i] = resta_timeval(&ti,&t0);
		printf("%f \n",resta_timeval(&ti,&t0));
		//Acciones
	     te[i] = espera_siguiente_intervalo(&t0,i+1,dt); 
    } 
	    for (i=0; i<NMUESTRAS; i++){
	printf("%f \t %f\n",te[i],t[i]);
	}
// post procesamiento de resultados
    return 0; 
}
*/


