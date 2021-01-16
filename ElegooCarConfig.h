#ifndef __ELEGOO_CAR_CONFIG_H__
#define __ELEGOO_CAR_CONFIG_H__

#include <Arduino.h>

// pin 0 of Arduino is used by UART (RX) at the car and not by any others
// so we use it as magic number to mark the pin as unused.
static constexpr uint8_t UNUSED_PIN = 0;

template <int CVERSION>
struct ElegooCarPinConfig {};

template <>
struct ElegooCarPinConfig<3>
{
	struct StatusConfig
	{
		static constexpr uint8_t RGB_LED_PIN = UNUSED_PIN;
	};

	struct ButtonConfig
	{
		static constexpr uint8_t MODE_SWITCH_PIN = UNUSED_PIN;
	};

	struct DistanceUnitConfig
	{
		static constexpr uint8_t ECHO_PIN = PIN_A4;
		static constexpr uint8_t TRIGGER_PIN = PIN_A5;
		static constexpr int SERVO_PIN = 3;
	};

	struct MotorUnitConfig
	{
		static constexpr uint8_t PWM_LEFT_PIN = 5; // ENA
		static constexpr uint8_t PWM_RIGHT_PIN = 6; // ENB
		static constexpr uint8_t LEFT_PIN = 7; // IN1
		static constexpr uint8_t LEFT_INV_PIN = 8; // IN2
		static constexpr uint8_t RIGHT_INV_PIN = 9; // IN3
		static constexpr uint8_t RIGHT_PIN = 11; // IN4
	};

	struct InfraredReceiverConfig
	{
		static constexpr uint8_t RECEIVER_PIN = 12;
	};
};

template <>
struct ElegooCarPinConfig<4>
{
	struct StatusConfig
	{
		static constexpr uint8_t RGB_LED_PIN = 4;
	};

	struct ButtonConfig
	{
		static constexpr uint8_t MODE_SWITCH_PIN = 2;
	};

	struct DistanceUnitConfig
	{
		static constexpr uint8_t ECHO_PIN = 12;
		static constexpr uint8_t TRIGGER_PIN = 13;
		static constexpr int SERVO_PIN = 10;
	};

	struct MotorUnitConfig
	{
		static constexpr uint8_t PWM_LEFT_PIN = 6; // ENB
		static constexpr uint8_t PWM_RIGHT_PIN = 5; // ENA
		static constexpr uint8_t LEFT_PIN = 7; // BIN1
		static constexpr uint8_t LEFT_INV_PIN = UNUSED_PIN;
		static constexpr uint8_t RIGHT_INV_PIN = 8; // AIN1
		static constexpr uint8_t RIGHT_PIN = UNUSED_PIN;
	};

	struct InfraredReceiverConfig
	{
		static constexpr uint8_t RECEIVER_PIN = 9;
	};
};


struct ElegooCarConfig
{
	using PinConfig = ElegooCarPinConfig<CAR_VERSION>;

	struct StatusConfig : PinConfig::StatusConfig
	{
	};

	struct ButtonConfig : PinConfig::ButtonConfig
	{
	};

	struct DistanceUnitConfig : PinConfig::DistanceUnitConfig
	{
		int SERVO_RIGHT = 20; // taking 20 as default instead of 0, since sometimes the servo seems to have issues with value 0

		int SERVO_LEFT = 160; // taking 160 as default instead of 180, since sometimes the servo may have issues with value 180
	};

	struct MotorUnitConfig : PinConfig::MotorUnitConfig
	{
		int SPEED = 170;
	};

	struct InfraredReceiverConfig : PinConfig::InfraredReceiverConfig
	{
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
	StatusConfig statusConfig;
	DistanceUnitConfig distanceUnitConfig;
	MotorUnitConfig motorUnitConfig;
	InfraredReceiverConfig infraredReceiverConfig;
	BluetoothReceiverConfig bluetoothReceiverConfig;
	SerialConfig serialConfig;
};

#endif
