// Copyright Casa Jasmina 2016
// LGPL License
//
// TelegramBot library
// https://github.com/CasaJasmina/TelegramBot-Library

#ifndef TelegramBot_h
#define TelegramBot_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>
#include <TelegramKeyboard.h>

#define HOST "api.telegram.org"
#define SSL_PORT 443
#define JSON_BUFF_SIZE 10000

struct message{
  String text;
  String chat_id;
  String sender;
  String date;
  String user_id;
  String type;
};

class TelegramBot
{
  public:
    TelegramBot(const char* token, Client &client);
  	void begin();
    String sendMessage(String chat_id, String text);
    String sendMessage(String chat_id, String text, TelegramKeyboard &keyboard_markup, bool one_time_keyboard = true, bool resize_keyboard = true);
    String postMessage(String msg);
    bool userInChat(String user_id, String chat_id);
    message getUpdates();

  private:
      String readPayload();
      const char* token;

      int last_message_recived;

      Client *client;
};

#endif
