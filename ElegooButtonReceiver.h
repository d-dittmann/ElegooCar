#ifndef __ELEGOO_BUTTON_RECEIVER_H__
#define __ELEGOO_BUTTON_RECEIVER_H__

#include <Arduino.h>

#include "ElegooCarConfig.h"
#include "ElegooCommand.h"
#include "ElegooReceiver.h"

namespace ElegooButtonReceiverDetail
{
	static volatile uint8_t buttonPressed = 0;

	static void isr(void)
	{
		static decltype(millis()) lastTime = 0;
		auto curTime = millis();
		if ((curTime < lastTime) || ((curTime - lastTime) > 500))
		{
			lastTime = curTime;
			++buttonPressed;
		}
	};
}

template <uint8_t MODE_BUTTON_PIN>
class ElegooButtonReceiver: public ElegooReceiver
{
private:
	uint8_t buttonPressed = ElegooButtonReceiverDetail::buttonPressed;

public:
	void setup()
	{
	  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
	  auto irq = digitalPinToInterrupt(MODE_BUTTON_PIN);
	  attachInterrupt(irq, ElegooButtonReceiverDetail::isr, FALLING);
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readCommand() override
	{
		if (buttonPressed != ElegooButtonReceiverDetail::buttonPressed)
		{
			++buttonPressed;
			return ElegooCommand::NEXT_DRIVER;
		}

		return ElegooCommand::NO_COMMAND;
	}
};

// this is a dummy, if the hardware does not support the button
template <>
class ElegooButtonReceiver<UNUSED_PIN>: public ElegooReceiver
{
public:
	void setup()
	{
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readCommand() override
	{
		return ElegooCommand::NO_COMMAND;
	}
};

#endif

