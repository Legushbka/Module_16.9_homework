#pragma once
#include <set>
#include <list>
#include "message.h"
#include <memory>
#include <iostream>
#include <conio.h>

namespace std
{
	class User;

	class Chat
	{
	private:
		shared_ptr<list<Message>> messagesPtr;
		bool notification;
		shared_ptr<User> otherUser;
		list<shared_ptr<Chat>>::iterator otherUserChat;
	public:
		Chat(shared_ptr<User> userReceiver, shared_ptr<list<Message>> _messagesPtr);
		Chat(shared_ptr<list<Message>> _allChatMessagesPtr); //@all
		void addPtrIntoListOfTheChat(shared_ptr<Message>);
		void coutChatHistory();

		void setOtherUserChat(list<shared_ptr<Chat>>::iterator chat);
		void setOtherUser(shared_ptr<User> user);
		bool getNotification();
		void giveNotification();
		void removeNotification();
		auto getMessagePtrsBeginIterator();
		shared_ptr<User> getOtherUser();
		list<shared_ptr<Chat>>::iterator getOtherUserChat();
	};
}
