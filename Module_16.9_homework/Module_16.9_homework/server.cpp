#include "server.h"

namespace std
{
	list<shared_ptr<Chat>>::iterator User::createNewChat(shared_ptr<User> userReceiver) // создаёт контейнер(Chat) с ссылкой на массив сообщений чата для
	{																					// отправителя первого сообщения и отдельный контейнер с ссылкой для его собеседника;
		shared_ptr<list<Message>> newMessagesListPtr = make_shared<list<Message>>();	// здесь создаётся массив сообщений чата между этими двумя пользователями
		shared_ptr<Chat> newPtrForSender = make_shared<Chat>(userReceiver, newMessagesListPtr);
		this->chats.push_front(newPtrForSender);										// добавляет эти контейнеры в списки чатов этих двух пользователей
		if (userReceiver != this->getSelfSharedPtr())
		{
			shared_ptr<Chat> newPtrForReceiver = make_shared<Chat>(this->getSelfSharedPtr(), newMessagesListPtr);
			userReceiver->chats.push_front(newPtrForReceiver);
			this->chats.front()->setOtherUserChat(userReceiver->chats.begin());			// даёт этим контейнерам ссылки друг на друга
			userReceiver->chats.front()->setOtherUserChat(this->chats.begin());
		}
		else this->chats.front()->setOtherUserChat(this->chats.begin()); //для чата с собой
		return this->chats.begin();
	}

	void User::sendMessage(string text, list<shared_ptr<Chat>>::iterator *chat)
	{
		shared_ptr<Message> newPtr = server->addMessageToTheHeap(text, nickname);		// сохраняет сообщение в общий список сообщений сервера
		(**chat)->addPtrIntoListOfTheChat(newPtr);										// добавляет ссылку на него в массив сообщений чата между двумя пользователями

		if (*chat != this->chats.begin())			// чат, в который отправлено сообщение, в начало списка чатов
		{
			this->chats.push_front(**chat);			// (итератор на чат передаётся для перемещения чата в начало списка чатов)
			this->chats.erase(*chat);
			*chat = this->chats.begin();
		}

		shared_ptr<User> otherUser = (**chat)->getOtherUser();
		list<shared_ptr<Chat>>::iterator otherUserChat = (**chat)->getOtherUserChat();

		if (otherUser != nullptr) {			//если чат не общий, то в списке собеседника тоже в начало
			if (otherUserChat != otherUser->chats.begin())
			{
				otherUser->chats.push_front(*otherUserChat);
				otherUser->chats.erase(otherUserChat);
				(**chat)->setOtherUserChat(otherUser->chats.begin());
			}
			(*this->chats.front()->getOtherUserChat())->giveNotification();
		}
		else					//если общий, проходит по всем пользователям и всем отправляет общий чат в начало их списков
		{
			for (set<shared_ptr<User>, compareNicknamesForSet>::iterator it = server->getUserPtrsBegin();
				it != server->getUserPtrsEnd(); it++)
			{
				cout << (*it)->getNickname();
				otherUserChat = (*(*it)->getAllChat());
				if ((*it) != this->getSelfSharedPtr())
				if (otherUserChat != (*it)->chats.begin())
				{
					(*it)->chats.push_front(*otherUserChat);
					(*it)->chats.erase(otherUserChat);
				}
				(*it)->allChat = (*it)->chats.begin();
				if (&(**it) != this) (**(*it)->getAllChat())->giveNotification();
			}
		}		
	}

	shared_ptr<User> Server::findUserByNickname(string theirNickname)
	{
		shared_ptr<User> temp = make_shared<User>(theirNickname);
		auto iterator = this->userPtrs.find(temp);
		if (iterator != this->userPtrs.end())
			return *iterator;
		else return nullptr;
	}

	shared_ptr<User> Server::findUserByLogin(string theirLogin)
	{
		shared_ptr<User> temp = make_shared<User>("", theirLogin);
		auto iterator = this->userPtrsSortedByLogins.find(temp);
		if (iterator != this->userPtrsSortedByLogins.end())
			return *iterator;
		else return nullptr;
	}

	shared_ptr<Message> Server::addMessageToTheHeap(string text, string sender)
	{
		shared_ptr<Message> newPtr = make_shared<Message>(text, sender);
		allMessages.push_back(newPtr);
		return newPtr;
	}

	void Server::addNewUser(string m_login, string m_passwordHash, string nickname)
	{
		shared_ptr<User> newUserPtr = make_shared<User>(m_login, m_passwordHash, nickname, this, this->all_chat);
		newUserPtr->giveSelfSharedPtr(*this->userPtrs.insert(newUserPtr).first); //insert возвращает пару значений, первое - итератор на позицию в set
		this->userPtrsSortedByLogins.insert(newUserPtr);
		list<shared_ptr<Chat>>::iterator adminChat = premadeChat("Admin", nickname);
		prewrittenMessage("Добро пожаловать в чатик", "Admin", adminChat);
		(*adminChat)->giveNotification();
	}

	set<shared_ptr<User>>::iterator Server::getUsersEnd() { return this->userPtrs.end(); }
	set<shared_ptr<User>>::iterator Server::getUsersByLoginEnd() { return this->userPtrsSortedByLogins.end(); }
	set<shared_ptr<User>, compareNicknamesForSet>::iterator Server::getUserPtrsBegin() { return userPtrs.begin(); }
	set<shared_ptr<User>, compareNicknamesForSet>::iterator Server::getUserPtrsEnd() { return userPtrs.end(); }

	//****************************************************************

	Server::Server()
	{
		all_chat = make_shared<list<Message>>();
		addNewUser("Admin", "Password", "Admin");
		addNewUser("Neeko", "qwertyui", "Nika");
		addNewUser("Sarah", "qwertyui", "Sara");
		addNewUser("Gleb", "ILovePublicTransport", "Gleb");
		addNewUser("ChineseCommunist", "PowerToTheWorkers", "MaoZeDong");

		cout << "1" << endl;
		prewrittenMessage("You can't be full with just a fifth dumpling", "MaoZeDong", premadeChat("MaoZeDong", "Sara"));
		cout << "2" << endl;
		prewrittenMessage("omg are you the real maozedong????????!!!!", "Gleb", premadeChat("Gleb", "MaoZeDong"));
		auto sara_nika = premadeChat("Sara", "Nika");
		cout << "3" << endl;
		prewrittenMessage("Ну и сообщение...", "Sara", sara_nika);
		cout << "4" << endl;
		prewrittenMessage("Ну и второе сообщение...", "Sara", sara_nika);
		cout << "5" << endl;
		prewrittenMessageInAllChat("неужели общий чат работает?", "Admin");
		prewrittenMessageInAllChat("самый лучший чат?", "Nika");
		prewrittenMessageInAllChat("заходил вчера?", "Sara");
		prewrittenMessageInAllChat("ночью ехать лень?", "Gleb");
		prewrittenMessageInAllChat("пробыл до утра?", "MaoZeDong");

	}

	void Server::prewrittenMessage(string _message, string _sender, list<shared_ptr<Chat>>::iterator _chat)
	{
		shared_ptr<User> sender = this->findUserByNickname(_sender);	// добавляет сообщения при запуске приложения
		sender->sendMessage(_message, &_chat);
	}

	void Server::prewrittenMessageInAllChat(string _message, string _sender)
	{
		shared_ptr<User> sender = this->findUserByNickname(_sender);	
		list<shared_ptr<Chat>>::iterator* chatIterator = sender->getAllChat();
		sender->sendMessage(_message, chatIterator);
	}

	list<shared_ptr<Chat>>::iterator Server::premadeChat(string _sender, string _receiver)
	{
		shared_ptr<User> sender = this->findUserByNickname(_sender);	// создаёт чаты при запуске приложения
		shared_ptr<User> receiver = this->findUserByNickname(_receiver);
		return sender->createNewChat(receiver);
	}

	//****************************************************************

	list<shared_ptr<Chat>>::iterator User::openChat(string theirNickname, bool* ignoreFlag)
	{																	// открывает чат после входа в аккаунт
		if (theirNickname == "@all")
		{
			(*this->allChat)->removeNotification();
			return (this->allChat);
		}
		list<shared_ptr<Chat>>::iterator chatCurrent = this->findChatByNickname(theirNickname);
		list<shared_ptr<Chat>>::iterator chatsEnd = this->chats.end();
		if (chatCurrent == chatsEnd)
		{
			shared_ptr<User> userCurrent = server->findUserByNickname(theirNickname);
			if (userCurrent == nullptr)
			{
				cout << "Пользователь " << theirNickname << " не найден. [Нажмите любую клавишу...]";
				_getch();
				return chatsEnd;
			}
			cout << "История переписки с пользователем " << theirNickname << " пока пуста. Хотите что-то им сказать, чтобы начать переписку? " << endl;
			cout << "(или введите -b, чтобы вернуться)" << endl;
			string newMessage;
			cin.ignore();
			*ignoreFlag = 0;
			getline(cin, newMessage);
			if (newMessage == "-b") return chatsEnd;
			chatCurrent = this->createNewChat(userCurrent);
			this->sendMessage(newMessage, &chatCurrent);
			(*(*chatCurrent)->getOtherUserChat())->giveNotification();
		}
		(*chatCurrent)->removeNotification();
		return chatCurrent;
	}

	shared_ptr<User> Server::loginSequence(string login)	// логин
	{
		int passwordHash;
		string input;
		system("cls");
		cout << "Введите пароль..." << endl << login << endl;
		cin >> input;
		passwordHash = hfunc(input);
		shared_ptr<User> currentUser = this->findUserByLogin(login);
		if (currentUser != nullptr)
		{
			if (passwordHash == currentUser->getPasswordHash()) return currentUser;
		}
		system("cls");
		cout << "Пароль неверный или пользователь не найден. [Нажмите любую клавишу...]" << endl;
		_getch();
		return nullptr;
	}

	void Server::registrationSequence()				// регистрация
	{
		string input;
		string login;
		string nickname;
		string password;
		bool loginTooShortFlag = 0;
		bool loginAlreadyExistsFlag = 0;
		bool nicknameTooShortFlag = 0;
		bool nicknameAlreadyExistsFlag = 0;
		bool passwordTooShortFlag = 0;
		bool passwordRepeatFlag = 0;
		bool passwordIncorrectFlag = 0;
		while (true)	//Ввод логина
		{
			system("cls");
			cout << "Регистрация нового пользователя. Для выхода введите -b" << endl;
			cout << "Введите логин." << endl;
			if (loginTooShortFlag)
			{
				cout << "Логин должен состоять как минимум из 3 символов." << endl;
				loginTooShortFlag = 0;
			}
			if (loginAlreadyExistsFlag)
			{
				cout << "Пользователь с таким логином уже существует." << endl;
				loginAlreadyExistsFlag = 0;
			}

			cin >> input;
			if (input == "-b") return;
			if (input.length() < 3)
			{
				loginTooShortFlag = 1;
				continue;
			}
			shared_ptr<User> currentUser = this->findUserByLogin(input);
			if (currentUser != nullptr)
			{
				loginAlreadyExistsFlag = 1;
				currentUser = nullptr;
				continue;
			}
			login = input;
			break;
		}
		while (true)	//Ввод никнейма
		{
			system("cls");
			cout << "Регистрация нового пользователя. Для выхода введите -b" << endl;
			cout << "Логин " << login << endl;
			cout << "Введите никнейм." << endl;
			if (nicknameTooShortFlag)
			{
				cout << "Никнейм должен состоять как минимум из 2 символов." << endl;
				nicknameTooShortFlag = 0;
			}
			if (nicknameAlreadyExistsFlag)
			{
				cout << "Никнейм занят." << endl;
				nicknameAlreadyExistsFlag = 0;
			}

			cin >> input;
			if (input == "-b") return;
			if (input.length() < 2)
			{
				nicknameTooShortFlag = 1;
				continue;
			}
			shared_ptr<User> currentUser = this->findUserByNickname(input);
			if (currentUser != nullptr || input == "@all")
			{
				nicknameAlreadyExistsFlag = 1;
				currentUser = nullptr;
				continue;
			}
			nickname = input;
			break;
		}
		while (true)	//Ввод пароля
		{
			system("cls");
			cout << "Регистрация нового пользователя. Для выхода введите -b" << endl;
			cout << "Логин " << login << endl;
			cout << "Никнейм " << nickname << endl;
			cout << "Придумайте пароль, состоящий как минимум из 8 символов." << endl;
			if (passwordTooShortFlag)
			{
				cout << "Пароль должен состоять как минимум из 8 символов." << endl;
				passwordTooShortFlag = 0;
			}
			if (passwordRepeatFlag)
			{
				cout << "Повторите пароль." << endl;
			}
			if (passwordIncorrectFlag)
			{
				cout << "Неверный пароль. Введите и повторите пароль заново." << endl;
				passwordIncorrectFlag = 0;
			}
			

			cin >> input;
			if (input == "-b") return;
			if (passwordRepeatFlag == 0 && input.length() < 8)
			{
				passwordTooShortFlag = 1;
				continue;
			}
			if (passwordRepeatFlag == 1)
			{
				if (input != password)
				{
					passwordIncorrectFlag = 1;
					passwordRepeatFlag = 0;
					password = "";
					continue;
				}
				else break; //Если пароль повторно введён верно, завершаем регистрацию
			}
			
			password = input;
			passwordRepeatFlag = 1;
		}
		this->addNewUser(login, password, nickname);
		system("cls");
		cout << "Регистрация прошла успешно!\nИспользуйте логин и пароль для входа.\n[Нажмите любую клавишу...]" << endl;
		_getch();
		return;
	}
}