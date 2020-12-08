// @file utils.h
#ifndef UTILS_H_
#define UTILS_H_

#include <string>
//#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

#define MSG_DELIMITER ":" 
#define MSG_IMU "msg_IMU"
#define MSG_CAM "msg_CAM"
#define MSG_BOM "msg_BOM"
#define MSG_LAS "msg_LAS"

using namespace std;

vector<vector<double>> Read_CSV_IMU_CAM(string inputFileName);

string decode_msg(string s,double v[3]);
void print_initMenu(void);
void print_endMenu(void);


#endif
