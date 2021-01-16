#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooConstants.h"
#include "ElegooCarConfig.h"
#include "ElegooStatus.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"
#include "ElegooReceiverChain.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"
#include "ElegooButtonReceiver.h"
#include "ElegooCommandReader.h"
#include "ElegooManualDriver.h"
#include "ElegooManualDriver2.h"
#include "ElegooAutomaticDriver1.h"
#include "ElegooAutomaticDriver2.h"
#include "ElegooLineTrackingDriver.h"

class ElegooCarV3
{
private:

	ElegooCarConfig * carConfig;

	using ElegooStatusT = ElegooStatus<ElegooCarConfig::StatusConfig::RGB_LED_PIN>;
	ElegooStatusT status;

	ElegooDistanceUnit distUnit;

	ElegooMotorUnit motorUnit;

	ElegooReceiverChain<3> receivers;

	ElegooInfraredReceiver infraredReceiver;

	ElegooBluetoothReceiver bluetoothReceiver;

	using ElegooButtonReceiverT = ElegooButtonReceiver<ElegooCarConfig::ButtonConfig::MODE_SWITCH_PIN>;
	ElegooButtonReceiverT buttonReceiver;

	ElegooCommandReader commandReader;

	ElegooDriverBase * drivers[5];

	ElegooCommand activeDriver = {};

	int safetyDistanceInCM;

public:

	ElegooCarV3(ElegooCarConfig * pCarConfig) :
			carConfig(pCarConfig), //
			status(carConfig->statusConfig),
			distUnit(carConfig->distanceUnitConfig), //
			motorUnit(carConfig->motorUnitConfig), //
			infraredReceiver(carConfig->infraredReceiverConfig), //
			bluetoothReceiver(carConfig->bluetoothReceiverConfig), //
			buttonReceiver(),
			commandReader(receivers), //
			safetyDistanceInCM(carConfig->SAFETY_DISTANCE_CM)
	{
		receivers.add<0>(&infraredReceiver);
		receivers.add<1>(&bluetoothReceiver);
		receivers.add<2>(&buttonReceiver);
	}

	int setup()
	{
		Serial.begin(carConfig->serialConfig.BAUD_RATE);
		status.setup();
		distUnit.setup();
		distUnit.registerCommandReader(&commandReader);
		motorUnit.setup();
		motorUnit.registerCommandReader(&commandReader);
		infraredReceiver.setup();
		bluetoothReceiver.setup();
		buttonReceiver.setup();

		initializeDrivers();
		selectManualDriver();

		return ElegooConstants::OK;
	}

private:
	void initializeDrivers()
	{
		drivers[ElegooCommand::MANUAL_DRIVER_1] = //
				new ElegooManualDriver(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::MANUAL_DRIVER_2] = //
				new ElegooManualDriver2(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_1] = //
				new ElegooAutomaticDriver1(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_2] = //
				new ElegooAutomaticDriver2(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::LINE_TRACKING_DRIVER] = //
				new ElegooLineTrackingDriver(safetyDistanceInCM, distUnit, motorUnit);
	}

	bool isDriver(ElegooCommand possibleDriver)
	{
		switch (possibleDriver)
		{
		case ElegooCommand::MANUAL_DRIVER_1:
		case ElegooCommand::MANUAL_DRIVER_2:
		case ElegooCommand::AUTO_DRIVER_1:
		case ElegooCommand::AUTO_DRIVER_2:
		case ElegooCommand::LINE_TRACKING_DRIVER:
		case ElegooCommand::NEXT_DRIVER:
			return true;
		default:
			return false;
		}
	}

	ElegooCommand nextDriver() const
	{
		switch (activeDriver)
		{
		case ElegooCommand::MANUAL_DRIVER_1:
			return ElegooCommand::MANUAL_DRIVER_2;
		case ElegooCommand::MANUAL_DRIVER_2:
			return ElegooCommand::AUTO_DRIVER_1;
		case ElegooCommand::AUTO_DRIVER_1:
			return ElegooCommand::AUTO_DRIVER_2;
		case ElegooCommand::AUTO_DRIVER_2:
			return ElegooCommand::LINE_TRACKING_DRIVER;
		case ElegooCommand::LINE_TRACKING_DRIVER:
		default:
			return ElegooCommand::MANUAL_DRIVER_1;
		}
	}

	ElegooDriverBase& currentDriver() const
	{
		return *drivers[activeDriver];
	}

	int selectDriver(ElegooCommand newDriver)
	{
		if (newDriver == ElegooCommand::NEXT_DRIVER)
		{
			newDriver = nextDriver();
		}

		activeDriver = newDriver;
		status.activeDriver(newDriver);

		return ElegooConstants::OK;
	}

	bool usingManualDriver()
	{
		return (activeDriver == ElegooCommand::MANUAL_DRIVER_1 || //
				activeDriver == ElegooCommand::MANUAL_DRIVER_2);
	}

public:
	int selectManualDriver()
	{
		return selectDriver(ElegooCommand::MANUAL_DRIVER_1);
	}

	void registerInfraredConfig(ElegooInfraredConfigInterface * infraredConfig)
	{
		infraredReceiver.registerInfraredConfig(infraredConfig);
	}

	void registerBluetoothConfig(ElegooBluetoothConfigInterface * bluetoothConfig)
	{
		bluetoothReceiver.registerBluetoothConfig(bluetoothConfig);
	}

	int drive()
	{
		ElegooCommand cmd = commandReader.readCommand();
		if (cmd == ElegooCommand::STOP_MOVING)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			return ElegooConstants::OK;
		}

		if (isDriver(cmd))
		{
			motorUnit.stopMoving();
			return selectDriver(cmd);
		}

		if (!usingManualDriver() && cmd != ElegooCommand::NO_COMMAND)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			// continue processing the given command (button press) below
		}

		if (usingManualDriver())
		{
			// manual drivers do correctly handle UNK_COMMAND and even rely on NO_COMMAND
			return currentDriver().processCommand(cmd);
		}

		// automatic drivers will/must not listen to commands
		// they must just get "re-triggered"
		return currentDriver().processCommand(ElegooCommand::NO_COMMAND);
	}

	void testDistanceUnit()
	{
		Serial.println("Test Distance Unit");
		distUnit.frontDistance();
		distUnit.test();
		distUnit.frontDistance();
		Serial.println();
	}

	void testInfrared()
	{
		commandReader.testReceiver(infraredReceiver);
	}

	void testBluetooth()
	{
		commandReader.testReceiver(bluetoothReceiver);
	}

};

#endif
