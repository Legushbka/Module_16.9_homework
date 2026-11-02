#pragma once
#include "chat.h"
#include "hfunc.h"
#include <tuple>

namespace std
{
	class Server;

	class User
	{
	private:
		Server* server;
		list<shared_ptr<Chat>>::iterator allChat;
		shared_ptr<User> selfSharedPtr;
		list<shared_ptr<Chat>> chats;
		string login;
		int passwordHash;
		string nickname;
	public:
		User(string m_login, string m_password, string m_nickname, Server* m_server, shared_ptr<list<Message>> _allChatMessagesPtr);
		User(string nicknameWeAreTryingToFind);
		User(string placeholder, string loginWeAreTryingToFind);
		list<shared_ptr<Chat>>::iterator findChatByNickname(string theirNickname);
		void sendMessage(string text, list<shared_ptr<Chat>>::iterator *chat);
		void showMyChats();
		list<shared_ptr<Chat>>::iterator createNewChat(shared_ptr<User> user);
		list<shared_ptr<Chat>>::iterator getEndOfChatPtrs();
		list<shared_ptr<Chat>>::iterator getBeginOfChatPtrs();
		string getNickname();
		string getLogin() const;
		int getPasswordHash();
		list<shared_ptr<Chat>>::iterator* getAllChat();
		void giveSelfSharedPtr(shared_ptr<User> it);
		shared_ptr<User> getSelfSharedPtr();
		/*
		bool operator < (const User& other);
		bool operator > (const User& other);
		*/

		list<shared_ptr<Chat>>::iterator openChat(string theirNickname, bool* ignoreFlag);
	};
}