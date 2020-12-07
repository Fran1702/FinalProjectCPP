/// @file sensors.h
#ifndef SENSORS_H_
#define SENSORS_H_

#include <cstdint>
#include "utils.h"

class MPU9250
{
  public:
    float pitch, yaw, roll;
    float temperature;   // Stores the real internal chip temperature in Celsius
    int16_t tempCount;   // Temperature raw count output
    uint32_t delt_t = 0; // Used to control display output rate

    uint32_t count = 0, sumCount = 0; // used to control display output rate
    float deltat = 0.0f, sum = 0.0f;  // integration interval for both filter schemes
    uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
    uint32_t Now = 0;        // used to calculate integration interval

    int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
    int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
    // Scale resolutions per LSB for the sensors
    float aRes, gRes, mRes;
    // Variables to hold latest sensor data values
    float ax, ay, az, gx, gy, gz, mx, my, mz;
    // Factory mag calibration and mag bias
    float factoryMagCalibration[3] = {0, 0, 0}, factoryMagBias[3] = {0, 0, 0};
    // Bias corrections for gyro, accelerometer, and magnetometer
    float gyroBias[3]  = {0, 0, 0},
          accelBias[3] = {0, 0, 0},
          magBias[3]   = {0, 0, 0},
          magScale[3]  = {0, 0, 0};
    float selfTest[6];
    // Stores the 16-bit signed accelerometer sensor output
    int16_t accelCount[3];

    // Public method declarations
    void getMres();
    void getGres();
    void getAres();
    void readAccelData(int16_t *,vector<vector<double>>,int);
    void readGyroData(int16_t *);
    void readMagData(int16_t *);
    int16_t readTempData();
    void updateTime();
    void initAK8963(float *);
    void initMPU9250();
    void calibrateMPU9250(float * gyroBias, float * accelBias);
    void MPU9250SelfTest(float * destination);
    void magCalMPU9250(float * dest1, float * dest2);
    uint8_t writeByte(uint8_t, uint8_t, uint8_t);
    uint8_t readByte(uint8_t, uint8_t);
    uint8_t readBytes(uint8_t, uint8_t, uint8_t, uint8_t *);
    // TODO: make SPI/Wire private
    uint8_t readBytesSPI(uint8_t, uint8_t, uint8_t *);
    uint8_t readBytesWire(uint8_t, uint8_t, uint8_t, uint8_t *);
    bool begin();
};  // class MPU9250





#endif // _MPU9250_H_



