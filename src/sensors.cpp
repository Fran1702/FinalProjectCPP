#include "sensors.h"
#include <iostream>
#include <cstdlib>
#include <ctime>


using namespace std;

void MPU9250::readAccelData(int16_t * destination,vector<vector<double>> data, int i)
{
  destination[0] = (int16_t) (data[i][0]*10000.0);
  destination[1] = (int16_t) (data[i][1]*10000.0);
  destination[2] = (int16_t) (data[i][2]*10000.0);

}
