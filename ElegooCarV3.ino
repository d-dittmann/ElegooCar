#include <Arduino.h>

#define CAR_VERSION 4 // choose the Elegoo Robot Car Version: 3 or 4
#define DEBUG_THE_CAR 1
#define TEST_THE_CAR 0

#include "ElegooCarV3.h"
#include "ElegooInfraredConfig.h"
#include "PanasonicInfraredConfig.h"
#include "MyBluetoothConfig.h"

ElegooCarV3 * car = 0;

// TODO (P1-MED) Implement line-tracking driver option
// TODO (P1-MED) Reduce/shorten output so that the in-App Bluetooth Terminal can be used to see what the car does

void setup()
{
	ElegooCarConfig * carConfig = new ElegooCarConfig();
	carConfig->distanceUnitConfig.SERVO_LEFT = 180;
	carConfig->infraredReceiverConfig.MAX_NUM_RECEIVERS = 4;
	carConfig->bluetoothReceiverConfig.MAX_NUM_RECEIVERS = 4;

	car = new ElegooCarV3(carConfig);
	car->setup();

	car->registerInfraredConfig(new ElegooInfraredConfig());
	car->registerInfraredConfig(new PanasonicInfraredConfig());
	car->registerBluetoothConfig(new MyBluetoothConfig());
	car->testDistanceUnit();

	if (TEST_THE_CAR)
	{
		car->testInfrared();
		car->testBluetooth();
	}
}

void loop()
{
	car->drive();
}
