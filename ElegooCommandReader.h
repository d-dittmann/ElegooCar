#ifndef __ELEGOO_COMMAND_READER_H__
#define __ELEGOO_COMMAND_READER_H__

#include "ElegooCommand.h"
#include "ElegooReceiver.h"

class ElegooCommandReader
{
private:
	ElegooReceiver & receiver;

	ElegooCommand command = ElegooCommand::NO_COMMAND;

public:
	ElegooCommandReader(ElegooReceiver & pReceiver) : receiver{pReceiver}
	{
	}

	bool hasCommand()
	{
		if (command == ElegooCommand::NO_COMMAND)
		{
			command = receiver.readCommand();
		}

		return command != ElegooCommand::NO_COMMAND;
	}

	ElegooCommand readCommand()
	{
		hasCommand();

		auto cmd = command;
		command = ElegooCommand::NO_COMMAND;
		return cmd;
	}

public:
	static void testReceiver(ElegooReceiver & receiver)
	{
		long noCmdCnt = 0;
		while (true)
		{
			ElegooCommand cmd = receiver.readCommand();
			if (cmd == ElegooCommand::NO_COMMAND)
			{
				noCmdCnt++;
			}
			else
			{
				const char * cmdString = ElegooCommandUtil::getCommandString(cmd);
				Serial.print(noCmdCnt);
				Serial.print(": ");
				Serial.println(cmdString);

				if (cmd == ElegooCommand::STOP_MOVING)
				{
					return;
				}
			}
		}
	}
};

#endif
