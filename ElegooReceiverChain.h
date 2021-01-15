#ifndef __ELEGOO_RECEIVERCHAIN_H__
#define __ELEGOO_RECEIVERCHAIN_H__

#include "ElegooReceiver.h"

template<uint8_t NoOfReceivers>
class ElegooReceiverChain : public ElegooReceiver
{
private:
	ElegooReceiver * receivers[NoOfReceivers] = {};

public:
	ElegooCommand readCommand() override
	{
		for(uint8_t i = 0; i < NoOfReceivers && receivers[i] != nullptr; ++i)
		{
			auto cmd = receivers[i]->readCommand();
			if (ElegooCommandUtil::isValidCommand(cmd))
			{
				return cmd;
			}
		}
		return ElegooCommand::NO_COMMAND;
	}

	template <uint8_t Pos>
	void add(ElegooReceiver * rec)
	{
		static_assert(Pos < NoOfReceivers, "Pos exceeds NoOfReceivers");
		receivers[Pos] = rec;
	}
};

#endif
