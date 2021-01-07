#ifndef __ELEGOO_MOTOR_UNIT_H__
#define __ELEGOO_MOTOR_UNIT_H__

#include <Arduino.h>
#include "ElegooBase.h"
#include "ElegooInterruptibleUnit.h"
#include "ElegooCarConfig.h"

class Action
{
public:
	Action()
	{
	}

	virtual void execute()
	{
	}

	virtual ~Action()
	{
	}
};

class DriveAction: public Action
{
private:
	uint8_t pin1, pin2, pin3, pin4;
	uint8_t val1, val2, val3, val4;

	static void condDigitalWrite(uint8_t pin, uint8_t val) {
		if (pin != UNUSED_PIN) {
			::digitalWrite(pin, val);
		}
	}

public:
	DriveAction( //
			uint8_t pPin1, uint8_t pVal1, //
			uint8_t pPin2, uint8_t pVal2, //
			uint8_t pPin3, uint8_t pVal3, //
			uint8_t pPin4, uint8_t pVal4) : //
			Action()
	{
		pin1 = pPin1, pin2 = pPin2, pin3 = pPin3, pin4 = pPin4;
		val1 = pVal1, val2 = pVal2, val3 = pVal3, val4 = pVal4;
	}

	virtual void execute()
	{
		condDigitalWrite(pin1, val1);
		condDigitalWrite(pin2, val2);
		condDigitalWrite(pin3, val3);
		condDigitalWrite(pin4, val4);
	}

	virtual ~DriveAction()
	{
	}
};

class ElegooMotorUnit: public ElegooBase, public ElegooInterruptibleUnit
{
private:

	ElegooCarConfig::MotorUnitConfig & config;

	static void condPinMode(uint8_t pin, uint8_t mode) {
		if (pin != UNUSED_PIN) {
			::pinMode(pin, mode);
		}
	}

public:

	ElegooMotorUnit(ElegooCarConfig::MotorUnitConfig & pMotorUnitConfig) :
			ElegooBase(), //
			ElegooInterruptibleUnit(), //
			config(pMotorUnitConfig)
	{
	}

	ElegooMotorUnit & setup()
	{
		pinMode(config.PWM_LEFT_PIN, OUTPUT);
		pinMode(config.PWM_RIGHT_PIN, OUTPUT);
		condPinMode(config.LEFT_PIN, OUTPUT);
		condPinMode(config.LEFT_INV_PIN, OUTPUT);
		condPinMode(config.RIGHT_INV_PIN, OUTPUT);
		condPinMode(config.RIGHT_PIN, OUTPUT);
		return *this;
	}

	ElegooMotorUnit & moveForwards(int delayMS = 0)
	{
		printMovement(delayMS, ElegooCommand::MOVE_FORWARDS);
		moveWheelsForTime(HIGH, LOW, LOW, HIGH, delayMS);
		return *this;
	}

	ElegooMotorUnit & moveBackwards(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::MOVE_BACKWARDS);
		moveWheelsForTime(LOW, HIGH, HIGH, LOW, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnLeft(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_LEFT);
		moveWheelsForTime(LOW, HIGH, LOW, HIGH, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnHalfLeft(int delayMS = 250)
	{
		return turnLeft(delayMS);
	}

	ElegooMotorUnit & turnRight(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_RIGHT);
		moveWheelsForTime(HIGH, LOW, HIGH, LOW, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnHalfRight(int delayMS = 250)
	{
		return turnRight(delayMS);
	}

	ElegooMotorUnit & stopMoving(int delayMS = 250)
	{
		printMovement(delayMS, ElegooCommand::STOP_MOVING);

		stopWheels();

		Action noAction;

		return runInterruptableAction(noAction, delayMS);
	}

private:

	ElegooMotorUnit & powerOnWheels()
	{
		analogWrite(config.PWM_LEFT_PIN, config.SPEED);
		analogWrite(config.PWM_RIGHT_PIN, config.SPEED);
		return *this;
	}

	ElegooMotorUnit & stopWheels()
	{
		digitalWrite(config.PWM_LEFT_PIN, LOW);
		digitalWrite(config.PWM_RIGHT_PIN, LOW);
		return *this;
	}

	ElegooMotorUnit & moveWheelsForTime(uint8_t valIn1, uint8_t valIn2, uint8_t valIn3, uint8_t valIn4, int timeMS)
	{
		if (hasCommand())
		{
			return *this;
		}

		powerOnWheels();

		DriveAction driveAction(config.LEFT_PIN, valIn1, config.LEFT_INV_PIN, valIn2, config.RIGHT_INV_PIN, valIn3, config.RIGHT_PIN, valIn4);

		return runInterruptableAction(driveAction, timeMS);
	}

	ElegooMotorUnit & runInterruptableAction(Action & action, int delayTotalMS, int delayIncrementMS = 50)
	{
		// call execute, so that in case delayTotalMS==0, we at least execute once
		// must keep this! it adjusts the driving direction
		action.execute();

		for (int i = 0; i < delayTotalMS; i += delayIncrementMS)
		{
			if (hasCommand())
			{
				return *this;
			}

			delay(delayIncrementMS);

			if (hasCommand())
			{
				return *this;
			}

			action.execute();
		}

		return *this;
	}

	ElegooMotorUnit & printMovement(int delayMs, ElegooCommand cmd)
	{
		// suppress printing in this case
		if (hasCommand())
		{
			return *this;
		}

		Serial.print(ElegooCommandUtil::getCommandString(cmd));
		Serial.print(": ");
		Serial.print(delayMs);
		Serial.println(" MS");
		return *this;
	}
};

#endif

