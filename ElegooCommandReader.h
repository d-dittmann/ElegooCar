#ifndef __ELEGOO_COMMAND_READER_H__
#define __ELEGOO_COMMAND_READER_H__

#include "ElegooCommand.h"
#include "ElegooReceiver.h"

class ElegooCommandReader
{
private:
	ElegooReceiver & receiver;

	static const int EMPTY_CACHE = -1;

	int cachedCommand = EMPTY_CACHE;

public:
	ElegooCommandReader(ElegooReceiver & pReceiver) : receiver{pReceiver}
	{
	}

	bool hasCommand()
	{
		if (cachedCommand == EMPTY_CACHE)
		{
			cachedCommand = readTheCommand();
		}

		if (cachedCommand == ElegooCommand::NO_COMMAND)
		{
			cachedCommand = EMPTY_CACHE;
			return false;
		}
		else
		{
			return true;
		}
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readCommand()
	{
		if (hasCommand())
		{
			ElegooCommand theCmd = (ElegooCommand) cachedCommand;
			cachedCommand = EMPTY_CACHE;
			return theCmd;
		}
		else
		{
			return ElegooCommand::NO_COMMAND;
		}
	}

private:
	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readTheCommand()
	{
		return receiver.readCommand();
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
