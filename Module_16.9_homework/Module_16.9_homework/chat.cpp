#include "chat.h"

namespace std
{
	Chat::Chat(shared_ptr<User> userReceiver, shared_ptr<list<Message>> _messagesPtr)
	{
		otherUser = userReceiver;
		messagesPtr = _messagesPtr;
		notification = false;
	}

	Chat::Chat(shared_ptr<list<Message>> _allChatMessagesPtr) // для @all
	{
		otherUser = nullptr;
		messagesPtr = _allChatMessagesPtr;
		notification = false;
	}

	void Chat::addPtrIntoListOfTheChat(shared_ptr<Message> message)
	{
		messagesPtr->push_back(*message);
	}

	void Chat::coutChatHistory()	//выводит сообщения чата на консоль
	{
		list<Message>::iterator current = (*messagesPtr).begin();
		while (current != (*messagesPtr).end())
		{
			cout << current->getTimeSentInHoursMinutes() << current->getSendersNickname() << " - " << current->getMessageText() << endl;
			current++;
		}
	}

	void Chat::setOtherUserChat(list<shared_ptr<Chat>>::iterator chat) { otherUserChat = chat; }
	void Chat::setOtherUser(shared_ptr<User> user) { otherUser = user; }
	bool Chat::getNotification() { return notification; }
	void Chat::giveNotification() { notification = true; }
	void Chat::removeNotification() { notification = false; }
	auto Chat::getMessagePtrsBeginIterator() { return (*messagesPtr).begin(); }
	shared_ptr<User> Chat::getOtherUser() { return otherUser; }
	list<shared_ptr<Chat>>::iterator Chat::getOtherUserChat() { return otherUserChat; }

}