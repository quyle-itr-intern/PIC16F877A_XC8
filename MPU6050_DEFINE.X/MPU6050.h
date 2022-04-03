/* 
 * File:   MPU6050.h
 * Author: levan
 *
 * Created on February 18, 2022, 9:26 AM
 */

#ifndef MPU6050_H
#define	MPU6050_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MPU6050_ADDR         0x68
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

  void begin();

  void setGyroOffsets(float x, float y, float z);

  void writeMPU6050(byte reg, byte data);
  byte readMPU6050(byte reg);

  int16_t getRawAccX(){ return rawAccX; };
  int16_t getRawAccY(){ return rawAccY; };
  int16_t getRawAccZ(){ return rawAccZ; };

  int16_t getRawTemp(){ return rawTemp; };

  int16_t getRawGyroX(){ return rawGyroX; };
  int16_t getRawGyroY(){ return rawGyroY; };
  int16_t getRawGyroZ(){ return rawGyroZ; };

  float getTemp(){ return temp; };

  float getAccX(){ return accX; };
  float getAccY(){ return accY; };
  float getAccZ(){ return accZ; };

  float getGyroX(){ return gyroX; };
  float getGyroY(){ return gyroY; };
  float getGyroZ(){ return gyroZ; };

	void calcGyroOffsets(bool console = false, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);

  float getGyroXoffset(){ return gyroXoffset; };
  float getGyroYoffset(){ return gyroYoffset; };
  float getGyroZoffset(){ return gyroZoffset; };

  void update();

  float getAccAngleX(){ return angleAccX; };
  float getAccAngleY(){ return angleAccY; };

  float getGyroAngleX(){ return angleGyroX; };
  float getGyroAngleY(){ return angleGyroY; };
  float getGyroAngleZ(){ return angleGyroZ; };

  float getAngleX(){ return angleX; };
  float getAngleY(){ return angleY; };
  float getAngleZ(){ return angleZ; };

  private:

  TwoWire *wire;

  int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
  rawGyroX, rawGyroY, rawGyroZ;

  float gyroXoffset, gyroYoffset, gyroZoffset;

  float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;

  float angleGyroX, angleGyroY, angleGyroZ,
  angleAccX, angleAccY, angleAccZ;

  float angleX, angleY, angleZ;

  float interval;
  long preInterval;

  float accCoef, gyroCoef;


#endif

#ifdef	__cplusplus
}
#endif

#endif	/* MPU6050_H */

