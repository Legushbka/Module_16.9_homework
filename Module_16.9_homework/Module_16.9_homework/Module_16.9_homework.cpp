#pragma once
#include <iostream>
#include <stdlib.h>
#include "server.h"
#include <Windows.h>

using namespace std;

int main()
{
    system("chcp 1251");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "");
    shared_ptr<Server> Mess_anger = make_shared<Server>();
    string input;
    while (true)
    {
        while (true) {
            system("cls");
            cout << "Введите логин. Для регистрации введите -r. Для выключения мессенджера введите -turnServerOff" << endl;
            cin >> input;
            if (input == "-turnServerOff")
            {
                return 0;
            }
            if (input == "-r")
            {
                Mess_anger->registrationSequence();
            }
            else break;
        }
        shared_ptr<User> currentUser = Mess_anger->loginSequence(input);
        if (currentUser != nullptr) while (true)
        {                       
            //успешный логин
            currentUser->showMyChats();
            cout << "Введите никнейм пользователя, с которым хотите начать беседу. Для входа в общий чат наберите @all. Для выхода введите -b" << endl;
            cin >> input;
            string inputNickname = input;
            if (input == "-b")
            {
                currentUser = nullptr;
                break;
            }
            bool cinIgnoreFlag = 1;
            bool notChatsEnd;
            auto currentChat = currentUser->openChat(input, &cinIgnoreFlag);
            if (inputNickname != "@all")
            {
                if (currentChat != currentUser->getEndOfChatPtrs()) notChatsEnd = 1;
                else notChatsEnd = 0;
            }
            else notChatsEnd = 1;

            if (notChatsEnd) while (true)
            {
                //успешный вход в переписку
                system("cls");
                (*currentChat)->coutChatHistory();
                if (inputNickname == "@all") cout << "*** Вы в общем чате. Для выхода введите -b ***" << endl;
                else cout << "*** Вы в чате с пользователем " << (*currentChat)->getOtherUser()->getNickname() << ". Для выхода введите -b ***" << endl;
                if (cinIgnoreFlag) {
                    cin.ignore();
                    cinIgnoreFlag = 0;
                }
                getline(cin, input);
                if (input == "-b")
                {
                    break;
                }
                currentUser->sendMessage(input, &currentChat);
            }
            
        }
    }
    return 0;
}