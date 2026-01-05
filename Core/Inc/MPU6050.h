/*
 * MPU6050.h
 *
 *  Created on: Jan 4, 2026
 *      Author: Ebad Rehman
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f4xx_hal.h" // Needed for I2C_HandleTypeDef

// MPU6050 I2C Address
// The default address is 0x68.
// We shift it left by 1 bit (0x68 << 1) because I2C addresses are 7-bits + 1 Read/Write bit.
// 0x68 << 1 = 0xD0
#define MPU6050_ADDR 0xD0

class MPU6050 {
private:
    I2C_HandleTypeDef *i2cHandle; // The "Phone Line" we use to talk to the sensor
    float accel_data[3];          // Stores X, Y, Z acceleration data

public:
    // Constructor: Assigns the specific I2C line (I2C1, I2C2, etc.)
    MPU6050(I2C_HandleTypeDef *hi2c) {
        this->i2cHandle = hi2c;
    }

    // Initialization: Wakes up the sensor (It sleeps by default)
    void init() {
        uint8_t check_device_id;
        uint8_t wake_up_command = 0; // Writing 0 to Power Register wakes it up

        // 1. CHECK: Who am I? (Register 0x75)
        // We read 1 byte. It should return 0x68.
        HAL_I2C_Mem_Read(i2cHandle, MPU6050_ADDR, 0x75, 1, &check_device_id, 1, 1000);

        if (check_device_id == 0x68) {
            // 2. WAKE UP: Power Management 1 (Register 0x6B)
            // We write 0x00 to this register to wake it up.
            HAL_I2C_Mem_Write(i2cHandle, MPU6050_ADDR, 0x6B, 1, &wake_up_command, 1, 1000);
        }
    }

    // Read: Gets raw data and converts to m/s^2
    void readAcceleration() {
        uint8_t raw_data[6]; // X_High, X_Low, Y_High, Y_Low, Z_High, Z_Low

        // 1. Read 6 Bytes starting from ACCEL_XOUT_H (Register 0x3B)
        HAL_I2C_Mem_Read(i2cHandle, MPU6050_ADDR, 0x3B, 1, raw_data, 6, 1000);

        // 2. Combine High and Low bytes (Bit manipulation)
        // (int16_t) casts it to a signed 16-bit integer
        int16_t raw_x = (int16_t)(raw_data[0] << 8 | raw_data[1]);
        int16_t raw_y = (int16_t)(raw_data[2] << 8 | raw_data[3]);
        int16_t raw_z = (int16_t)(raw_data[4] << 8 | raw_data[5]);

        // 3. Convert to Gravity (1g = 9.81 m/s^2)
        // The default sensitivity is 16384 LSB/g
        accel_data[0] = (raw_x / 16384.0f) * 9.81f;
        accel_data[1] = (raw_y / 16384.0f) * 9.81f;
        accel_data[2] = (raw_z / 16384.0f) * 9.81f;
    }

    // Getters for your Physics Engine
    float getAx() { return accel_data[0]; }
    float getAy() { return accel_data[1]; }
    float getAz() { return accel_data[2]; }
};

#endif /* MPU6050_H_ */
