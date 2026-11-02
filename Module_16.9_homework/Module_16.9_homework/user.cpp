#include "user.h"

namespace std
{
	User::User(string m_login, string m_password, string m_nickname, Server* m_server, shared_ptr<list<Message>> _allChatMessagesPtr)
	{
		login = m_login;
		passwordHash = hfunc(m_password);
		nickname = m_nickname;
		server = m_server;
		chats.push_front(make_shared<Chat>(_allChatMessagesPtr));
		allChat = chats.begin();
	}
	User::User(string nicknameWeAreTryingToFind) { nickname = nicknameWeAreTryingToFind; } //конструкторы временного пользователя для функции find
	User::User(string placeholder, string loginWeAreTryingToFind) { login = loginWeAreTryingToFind; }

	list<shared_ptr<Chat>>::iterator User::findChatByNickname(string theirNickname) //ищет чат с пользователем по никнейму
	{
		auto current = this->chats.begin();
		while (current != this->chats.end())
		{
			if ((*current)->getOtherUser() == nullptr)
			{
				current++;
				continue;
			}
			if ((*current)->getOtherUser()->getNickname() == theirNickname)
			{
				return current;
			}
			current++;
		}
		return this->chats.end();
	}

	void User::showMyChats() //выводит на консоль чаты
	{
		system("cls");
		cout << "Ваши переписки:" << endl;
		int counter = 1;
		auto current = this->chats.begin();
		while (current != this->chats.end())
		{
			cout << counter++ << ". "; 
			if ((*current)->getOtherUser() != nullptr) cout << (*current)->getOtherUser()->getNickname();
			else cout << "@all";
			if ((*current)->getNotification()) cout << "(new!)";
			cout << endl;
			current++;
		}
	}
	
	list<shared_ptr<Chat>>::iterator User::getEndOfChatPtrs() { return chats.end(); }
	list<shared_ptr<Chat>>::iterator User::getBeginOfChatPtrs() { return chats.begin(); }
	string User::getNickname() { return nickname; }
	string User::getLogin() const { return login; }
	int User::getPasswordHash() { return passwordHash; }
	list<shared_ptr<Chat>>::iterator* User::getAllChat() { return &allChat; }
	void User::giveSelfSharedPtr(shared_ptr<User> it) { selfSharedPtr = it; }
	shared_ptr<User> User::getSelfSharedPtr() { return selfSharedPtr; }
	/*
	bool User::operator <(const User& other) { return this->nickname < other.nickname; }
	bool User::operator >(const User& other) { return this->nickname > other.nickname; }
	*/
}