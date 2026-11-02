#pragma once
#include <string>
#include <time.h>

namespace std
{
	class Message
	{
	private:
		string sendersNickname;
		string messageText;
		time_t timeSent;
	public:
		Message(string text, string sender);
		string getMessageText();
		string getSendersNickname();
		string getTimeSentInHoursMinutes();
	};
}

