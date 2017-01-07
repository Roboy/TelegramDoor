#include <Arduino.h>

#include <Wire.h>

#include <WiFi101.h>
#include <TelegramBot.h>

//The SleepyDog library is needed to reset the board when something goes wrong.
#include <Adafruit_SleepyDog.h>

#define RELAY_PIN 10 //6 //Pin where the relay is attached

#include <config.key.h> //Config file to be included. All variables start with conf. Rename from config.key.h-example to config.key.h and fill in.

WiFiSSLClient client;
TelegramBot bot (confBotToken, client);

bool stringInArray(String key,const String arr[]){
  for(unsigned int i = 0; i < sizeof(arr)-1;i++){
    Serial.println(arr[i]+"!="+key);
    if(key == arr[i]) return true;
  }
  return false;
}

String toStr(const String arr[],const String glue){
  String str = "";
  unsigned int l = sizeof(arr)-1;
  for(unsigned int i = 0; i < l;i++){
    str += arr[i];
    if(i < l-1) str += glue;
  }
  return str;
}

void setup() {

  Serial.begin(115200);
  delay(3000);

  pinMode(RELAY_PIN, OUTPUT);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(confSSID);
  while ( (confWiFiPass.length()==0?WiFi.begin(confSSID):WiFi.begin(confSSID, confWiFiPass)) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //---- BOT Setup ---//
  bot.begin();
  Watchdog.enable(20000); //in ms
  bot.sendMessage(confControlChatID, "Ready to serve, I listen to: \""+confAccessWords[confAccessWordIndex]+"\".");
}

void loop() {

    Watchdog.reset();

    message m = bot.getUpdates(); // Read new messages
    if ( m.chat_id != 0 and m.type == "private"){ //ignore messages that are not directly directed to the bot.
      Serial.println(m.chat_id+": "+m.text+" - "+m.user_id+" / "+bot.userInChat(m.user_id,confAccessChatID));
      if(String(m.text[0]) == "/") m.text = m.text.substring(1);
      if(bot.userInChat(m.user_id,confAccessChatID)){ //every user that is in the confAccessChatID chat, will get access.
        if(stringInArray(m.text,confAccessWords)){ //user used one of the available bots.
          if(m.text == confAccessWords[confAccessWordIndex]){ //this is me, let's answer.
            bot.sendMessage(m.chat_id, "Welcome, we're open!");  // Reply to the same chat with the same text
            bot.sendMessage(confControlChatID, "Access granted: "+m.sender+" said \""+m.text+"\"");
            digitalWrite(RELAY_PIN, HIGH);
            delay(3000);
          }else if(m.text == confSequenceAccessWord){
            bot.sendMessage(m.chat_id, "Sequence triggered, I will open in "+String(confSequenceDelay)+" seconds.");
            delay(1000*confSequenceDelay);
            bot.sendMessage(confControlChatID, "Sequential access granted: "+m.sender+" said \""+m.text+"\"");
            digitalWrite(RELAY_PIN, HIGH);
            delay(3000);
          } // else: don't answer, another bot should.
        }else{ //Here each bot that listens will answer on it's own.
          bot.sendMessage(m.chat_id, "Oops, that's none of the magic words! Speak \""+confAccessWords[confAccessWordIndex]+"\" and enter.");
          bot.sendMessage(confControlChatID, "Access denied, wrong word: "+m.sender+" said \""+m.text+"\"");
        }
      }else{
        bot.sendMessage(m.chat_id, "I don't know you! Talk to the [x] team, to get added to the channel!");
        bot.sendMessage(confControlChatID, "Access denied, not in channel: "+m.sender+" said \""+m.text+"\"");
      }
    }else{
      Serial.println("no new message");
    }

    digitalWrite(RELAY_PIN, LOW);
}
