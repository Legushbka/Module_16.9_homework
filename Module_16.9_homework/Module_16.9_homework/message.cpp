#include "message.h"

namespace std
{
	Message::Message(string text, string sender)
	{
		sendersNickname = sender;
		messageText = text;
		timeSent = time(NULL);
	}

	string Message::getTimeSentInHoursMinutes()
	{
		struct tm timeSentPtr;
		localtime_s(&timeSentPtr, &timeSent);
		char hm[9];
		strftime(hm, 9, "[%H:%M] ", &timeSentPtr);
		return hm;
	}

	string Message::getMessageText() { return messageText; }

	string Message::getSendersNickname() { return sendersNickname; }
}