#pragma once
#include "user.h"
#include <map>
#include <conio.h>

namespace std
{
	struct compareLoginsForSet {
		bool operator()(shared_ptr<User> user1, shared_ptr<User> user2) const
		{
			return (user1->getLogin() < user2->getLogin());
		}
	};

	struct compareNicknamesForSet {
		bool operator()(shared_ptr<User> user1, shared_ptr<User> user2) const
		{
			return (user1->getNickname() < user2->getNickname());
		}
	};

	class Server
	{
	private:
		shared_ptr<list<Message>> all_chat;
		set<shared_ptr<User>, compareNicknamesForSet> userPtrs; //sorted by nicknames
		set<shared_ptr<User>, compareLoginsForSet> userPtrsSortedByLogins;
		list<shared_ptr<Message>> allMessages;
	public:
		Server();
		void addNewUser(string m_login, string m_passwordHash, string nickname);
		shared_ptr<Message> addMessageToTheHeap(string text, string sender);
		shared_ptr<User> findUserByNickname(string theirNickname);
		shared_ptr<User> findUserByLogin(string theirLogin);
		set<shared_ptr<User>>::iterator getUsersEnd();
		set<shared_ptr<User>>::iterator getUsersByLoginEnd();
		set<shared_ptr<User>, compareNicknamesForSet>::iterator getUserPtrsBegin();
		set<shared_ptr<User>, compareNicknamesForSet>::iterator getUserPtrsEnd();
		
		void prewrittenMessage(string _message, string _sender, list<shared_ptr<Chat>>::iterator _chat);
		void prewrittenMessageInAllChat(string _message, string _sender);
		list<shared_ptr<Chat>>::iterator premadeChat(string _sender, string _receiver);

		//*************************************************************

		shared_ptr<User> loginSequence(string login);
		void registrationSequence();
	};
}