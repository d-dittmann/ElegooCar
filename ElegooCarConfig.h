#ifndef __ELEGOO_CAR_CONFIG_H__
#define __ELEGOO_CAR_CONFIG_H__

#include <Arduino.h>

#if CAR_VERSION == 3
	#define DISTANCEUNIT_ECHO_PIN    PIN_A4
	#define DISTANCEUNIT_TRIGGER_PIN PIN_A5
	#define DISTANCEUNIT_SERVO_PIN   3
	#define MOTORUNIT_PWM_LEFT_PIN   5
	#define MOTORUNIT_PWM_RIGHT_PIN  6
	#define MOTORUNIT_LEFT_PIN       7
	#define MOTORUNIT_LEFT_INV_PIN   8
	#define MOTORUNIT_RIGHT_INV_PIN  9
	#define MOTORUNIT_RIGHT_PIN      11
	#define IRRECEIVER_RECEIVER_PIN  12
#elif CAR_VERSION == 4
	#define DISTANCEUNIT_ECHO_PIN    12
	#define DISTANCEUNIT_TRIGGER_PIN 13
	#define DISTANCEUNIT_SERVO_PIN   10
	#define MOTORUNIT_PWM_LEFT_PIN   6
	#define MOTORUNIT_PWM_RIGHT_PIN  5
	#define MOTORUNIT_LEFT_PIN       7
	#define MOTORUNIT_LEFT_INV_PIN   0 // unused
	#define MOTORUNIT_RIGHT_INV_PIN  8
	#define MOTORUNIT_RIGHT_PIN      0 // unused
	#define IRRECEIVER_RECEIVER_PIN  9
#else
	#error Unknown CAR_VERSION
#endif



struct ElegooCarConfig
{
	struct DistanceUnitConfig
	{
		uint8_t ECHO_PIN = DISTANCEUNIT_ECHO_PIN;

		uint8_t TRIGGER_PIN = DISTANCEUNIT_TRIGGER_PIN;

		int SERVO_PIN = DISTANCEUNIT_SERVO_PIN;

		int SERVO_RIGHT = 20; // taking 20 as default instead of 0, since sometimes the servo seems to have issues with value 0

		int SERVO_LEFT = 160; // taking 160 as default instead of 180, since sometimes the servo may have issues with value 180
	};

	struct MotorUnitConfig
	{
		uint8_t PWM_LEFT_PIN = MOTORUNIT_PWM_LEFT_PIN; // ENA

		uint8_t PWM_RIGHT_PIN = MOTORUNIT_PWM_RIGHT_PIN; // ENB

		uint8_t LEFT_PIN = MOTORUNIT_LEFT_PIN; // IN1

		uint8_t LEFT_INV_PIN = MOTORUNIT_LEFT_INV_PIN; // IN2

		uint8_t RIGHT_INV_PIN = MOTORUNIT_RIGHT_INV_PIN; // IN3

		uint8_t RIGHT_PIN = MOTORUNIT_RIGHT_PIN; // IN4

		int SPEED = 170;
	};

	struct InfraredReceiverConfig
	{
		uint8_t RECEIVER_PIN = IRRECEIVER_RECEIVER_PIN;

		int MAX_NUM_RECEIVERS = 10;
	};

	struct BluetoothReceiverConfig
	{
		int MAX_NUM_RECEIVERS = 10;
	};

	struct SerialConfig
	{
		unsigned long BAUD_RATE = 9600;
	};

	int SAFETY_DISTANCE_CM = 30;
	DistanceUnitConfig distanceUnitConfig;
	MotorUnitConfig motorUnitConfig;
	InfraredReceiverConfig infraredReceiverConfig;
	BluetoothReceiverConfig bluetoothReceiverConfig;
	SerialConfig serialConfig;
};

#endif
