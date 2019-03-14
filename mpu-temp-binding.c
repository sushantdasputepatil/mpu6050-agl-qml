/*
 * Copyright (C) 2017 Linaro Ltd
 * Author: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define AFB_BINDING_VERSION 1
#define _GNU_SOURCE
#include<stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <json-c/json.h>
#include <afb/afb-binding.h>
#define MPU_TEMP1 0x41
#define MPU_TEMP2 0x42
#define MPU_POWER2 0x6c
#define MPU_ACCEL_XOUT1 0x3b
#define MPU_ACCEL_XOUT2 0x3c
#define MPU_ACCEL_YOUT1 0x3d
#define MPU_ACCEL_YOUT2 0x3e
#define MPU_ACCEL_ZOUT1 0x3f
#define MPU_ACCEL_ZOUT2 0x40
#define MPU_GYRO_XOUT1 0x43
#define MPU_GYRO_XOUT2 0x44
#define MPU_GYRO_YOUT1 0x45
#define MPU_GYRO_YOUT2 0x46
#define MPU_GYRO_ZOUT1 0x47
#define MPU_GYRO_ZOUT2 0x48
#define MPU_POWER2 0x6c
#define MPU_SMPLRT_DIV 0x19
#define MPU_GYRO_CONFIG  0x1B
char *fileName = "/dev/i2c-1";// i2c interface to connect with sensor on board
int fd;
int  address = 0x68;
int16_t temp2;
float temp3;
char temp[16];
char accel[18];
char gyro[18];
int16_t data[3];
const struct afb_binding_interface *interface;

static void get_temp(struct afb_req request)
{
  	temp2 = i2c_smbus_read_byte_data(fd, MPU_TEMP1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_TEMP2);
	if( temp2 > 0 ) {
		AFB_ERROR(interface, "Unable to read temp: failed with error : %d \n", temp2);
		afb_req_fail(request, "failed", "Read temp failed");
	} else {
		temp3=(temp2 / 340.0f )+ 36.53;
    		printf("temp: %f\n",temp3);
		AFB_ERROR(interface, " temp :",temp3);
		snprintf(temp, 6, "%f", temp3);
		AFB_ERROR(interface, " temp Done");
    		afb_req_success(request, NULL, temp); /* this function sends the reply,
                                                 attaching it to the object description*/
	}
}
static void get_accel(struct afb_req request)
{
  	int16_t xaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_XOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_XOUT2);
	int16_t yaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_YOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_YOUT2);
	int16_t zaccel = i2c_smbus_read_byte_data(fd, MPU_ACCEL_ZOUT1) << 8 |
                         i2c_smbus_read_byte_data(fd, MPU_ACCEL_ZOUT2);
		data[0]=xaccel;
		data[1]=yaccel;
		data[2]=zaccel;
		snprintf(accel, 18, "%d,%d,%d", (data[0],data[1],data[2]));
		AFB_ERROR(interface, " accel Done");
    		afb_req_success(request, NULL, accel); /* this function sends the reply,
                                                 attaching it to the object description*/
}
static void get_gyro(struct afb_req request)
{
  	int16_t xgyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_XOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_XOUT2);
	int16_t ygyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_YOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_YOUT2);
	int16_t zgyro = i2c_smbus_read_byte_data(fd, MPU_GYRO_ZOUT1) << 8 |
                        i2c_smbus_read_byte_data(fd, MPU_GYRO_ZOUT2);
		data[0]=xgyro;
		data[1]=ygyro;
		data[2]=zgyro;
		snprintf(gyro, 18, "%d,%d,%d", (data[0],data[1],data[2]));
		AFB_ERROR(interface, " gyro Done");
    		afb_req_success(request, NULL, gyro); /* this function sends the reply,
                                                 attaching it to the object description*/
}
/*Description of one of the method of the API provided by the binding,this enumeration is
  valid for bindings of type 1*/
static const struct afb_verb_desc_v1 verbs[]= {
  {"temp", AFB_SESSION_NONE, get_temp , "returns the mputemp data"},
  {"accel", AFB_SESSION_NONE, get_accel , "returns the mpuaccel data"},
  {"gyro", AFB_SESSION_NONE, get_gyro , "returns the mpugyro data"},
  {NULL}
};

/* binding_desc descibes the binding. It declares the version binding, its name,
a short description and its method list */
static const struct afb_binding binding_desc = {
	.type = AFB_BINDING_VERSION_1,
	.v1 = {
		.info = "mpu6050 temperature service",
		.prefix = "mpu6050",
		.verbs = verbs
	}
};

static int MPU6050_Init(void) // this function for intialization of temperature sensor
{
	if ((fd = open(fileName, O_RDWR)) < 0) {
        printf("Failed to open i2c port\n");
	AFB_ERROR(interface, "Failed to open i2c port\n");
        exit(1);
    }

    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        printf("Unable to get bus access to talk to slave\n");
	AFB_ERROR(interface, "Unable to get bus access to talk to slave\n");
        exit(1);
    }
	int8_t ret = i2c_smbus_write_byte_data(fd,0x6B,0x01);	/* Write to power management register */
        int8_t ret1 = i2c_smbus_write_byte_data(fd, 0x1a, 0x00);		/* Write to Configuration register */
	int8_t ret2 = i2c_smbus_write_byte_data( fd, 0x38, 0x01);	/*Write to interrupt enable register */
	int8_t ret3 = i2c_smbus_write_byte_data(fd,0x19,0x07);
	int8_t ret4 = i2c_smbus_write_byte_data(fd,0x1B,0x24);
	if( ret1 < 0 )
		AFB_DEBUG(interface, "Unable to write reg 0x1a \n");
	if( ret < 0 )
		AFB_DEBUG(interface, "Unable to write reg 0x6B \n");
	if( ret2 < 0 )
		AFB_DEBUG(interface, "Unable to write reg 0x38 \n");
	if( ret3 < 0 )
		AFB_DEBUG(interface, "Unable to write reg 0x6B \n");
	if( ret4 < 0 )
		AFB_DEBUG(interface, "Unable to write reg 0x38 \n");
	AFB_DEBUG(interface, "Init Done");
      return 0;
}
/*Activation function for registering the binding called by afb deamon*/
const struct afb_binding *afbBindingV1Register (const struct afb_binding_interface *itf)
{
	        interface = itf; /* variable 'interface' keeps the interface to afb-deamon
                            that should be used for logging and pushing events*/

		return &binding_desc;
}

int afbBindingV1ServiceInit(struct afb_service service) /*initialisation of bindings and
                                                        make its afb-deamon's interface
                                                        fully functional*/
{
	        return MPU6050_Init();
}

