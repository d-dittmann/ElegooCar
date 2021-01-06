#ifndef __ELEGOO_CAR_CONFIG_H__
#define __ELEGOO_CAR_CONFIG_H__

#include <Arduino.h>

struct ElegooCarConfig
{
	struct DistanceUnitConfig
	{
		uint8_t ECHO_PIN = PIN_A4;

		uint8_t TRIGGER_PIN = PIN_A5;

		int SERVO_PIN = 3;

		int SERVO_RIGHT = 20; // taking 20 as default instead of 0, since sometimes the servo seems to have issues with value 0

		int SERVO_LEFT = 160; // taking 160 as default instead of 180, since sometimes the servo may have issues with value 180
	};

	struct MotorUnitConfig
	{
		uint8_t PWM_LEFT_PIN = 5; // ENA

		uint8_t PWM_RIGHT_PIN = 6; // ENB

		uint8_t LEFT_PIN = 7; // IN1

		uint8_t LEFT_INV_PIN = 8; // IN2

		uint8_t RIGHT_INV_PIN = 9; // IN3

		uint8_t RIGHT_PIN = 11; // IN4

		int SPEED = 170;
	};

	struct InfraredReceiverConfig
	{
		uint8_t RECEIVER_PIN = 12;

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
