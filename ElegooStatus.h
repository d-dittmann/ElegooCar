#ifndef __ELEGOO_STATUS_H__
#define __ELEGOO_STATUS_H__

#include <Arduino.h>

#include "ElegooCarConfig.h"
#include "ElegooCommand.h"
#include "FastLED.h"

template <uint8_t LED_PIN>
class ElegooStatus
{
private:
	const ElegooCarConfig::StatusConfig & config;
	CRGB led;
 	uint8_t brightness{25};

public:
	ElegooStatus(const ElegooCarConfig::StatusConfig & pConfig)
	  : config(pConfig)
	{
	}

	void setup()
	{
		FastLED.addLeds<NEOPIXEL, LED_PIN>(&led, 1);
		FastLED.setBrightness(brightness);
	}

	void activeDriver(ElegooCommand newDriver)
	{
		switch (newDriver)
		{
		case ElegooCommand::MANUAL_DRIVER_1:
			led = CRGB::Violet;
			break;
		case ElegooCommand::MANUAL_DRIVER_2:
			led = CRGB::DarkViolet;
			break;
		case ElegooCommand::AUTO_DRIVER_1:
			led = CRGB::LightYellow;
			break;
		case ElegooCommand::AUTO_DRIVER_2:
			led = CRGB::Yellow;
			break;
		case ElegooCommand::LINE_TRACKING_DRIVER:
			led = CRGB::Green;
			break;
		default:
			break;
		}

		FastLED.show();
	}
};

// this is a dummy, if the hardware does not support the led
template <>
class ElegooStatus<UNUSED_PIN>
{
public:
	ElegooStatus(const ElegooCarConfig::StatusConfig &)
	{
	}

	void setup()
	{
	}

	void activeDriver(ElegooCommand newDriver)
	{
	}
};

#endif
