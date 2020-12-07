// @file utils.cpp

//#include <string>
#include <fstream>
//#include <vector>
#include <sstream> // std::stringstream
#include <iostream>

#include "utils.h"

using namespace std;

void print_endMenu(void){
	cout << "|";
	for(int i=0;i<78;i++)cout<<"-";
	cout << "|" << endl;
	cout << "|";
	for(int i=0;i<29;i++)cout<<"-";
	cout << "FIN DE LA SIMULACIÓN";
	for(int i=0;i<29;i++)cout<<"-";
	cout << "|" << endl;
	cout << "|";
	for(int i=0;i<78;i++)cout<<"-";
	cout << "|" <<  endl;
}
void print_initMenu(void){
	cout << "|";
	for(int i=0;i<78;i++)cout<<"-";
	cout << "|" << endl;
	cout << "|";
	for(int i=0;i<27;i++)cout<<"-";
	cout << "INICIO DE LA SIMULACIÓN";
	for(int i=0;i<28;i++)cout<<"-";
	cout << "|" << endl;
	cout << "|";
	for(int i=0;i<78;i++)cout<<"-";
	cout << "|" << endl;
	cout << "| PRESIONE ENTER PARA TERMINAR";
	cout << endl;
	cout  << "\33[4;79H |";


}
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
			token = s.substr(0, s.find(delimiter));
			if(token=="IMU"){
				ret = MSG_IMU;
				for(int i=0;i<3;i++){
					pos = s.find(delimiter);
					s.erase(0, pos + delimiter.length());
					token = s.substr(0, s.find(delimiter));
					v[i] = (double) stoi(token);
				}
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));

			}
			if(token=="CAM"){
				ret = MSG_CAM;
				for(int i=0;i<3;i++){
					pos = s.find(delimiter);
					s.erase(0, pos + delimiter.length());
					token = s.substr(0, s.find(delimiter));
					v[i] = (double) stoi(token);
				}
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));
			}
			if(token=="BOM"){
				ret = MSG_BOM;
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));
				v[0] = (double) stoi(token);
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));
			}
			if(token=="LAS"){
				ret = MSG_LAS;
				pos = s.find(delimiter);
				s.erase(0, pos + delimiter.length());
				token = s.substr(0, s.find(delimiter));
				v[0] = (double) stoi(token);
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
