#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include <UniversalTelegramBot.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>
#include <ESPping.h>
#include <string>

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
const unsigned long RESET_TIME = 3600000;
unsigned long previousReset;

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
WiFiUDP UDP;
WakeOnLan WOL(UDP);

unsigned long bot_lasttime;          // last time messages' scan has been done
const int ledPin = LED_BUILTIN;

void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    Serial.println(bot.messages[i].from_id);
    if (bot.messages[i].from_id != MY_ID) continue;

    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/wol") {
      sendWOL();
      bot.sendMessage(chat_id, "Magic Packet sent!", "");
    } else if (text == "/ping") {
      bot.sendMessage(chat_id, "Pong.", "");
    } else if (text == "/status") {
      bot.sendMessage(chat_id, pingPc(), "");
    } else if (text == "/ledon") {
      digitalWrite(ledPin, LOW); // turn the LED on
      bot.sendMessage(chat_id, "Led is ON", "");
    } else if (text == "/ledoff") {
      digitalWrite(ledPin, HIGH); // turn the LED off
      bot.sendMessage(chat_id, "Led is OFF", "");
    } else if (text == "/start") {
      String welcome = "Welcome to **WOL Bot**, " + from_name + ".\n";
      welcome += "This bot allows you to remotely control your computer using Wake on LAN.\n\n";
      welcome += "Here are the available commands:\n";
      welcome += "/wol : Send the Magic Packet\n";
      welcome += "/ping : Check the bot status\n";
      welcome += "/status : Check the PC status\n";
      welcome += "/ledon : Switch the Led ON\n";
      welcome += "/ledoff : Switch the Led OFF\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void sendWOL() {
  digitalWrite(ledPin, LOW);
  WOL.sendMagicPacket(MAC_ADDR); // send WOL on default port (9)
  delay(300);
  digitalWrite(ledPin, HIGH);
}

const String pingPc() {
    bool ret = Ping.ping(PC_IP);
    String message = "PC is ";
    if (ret) {
      message += "ON";
    } else {
      message += "OFF";
    }
    return message;
}

void setup() {
  ESP.wdtEnable(WDTO_8S);       // watchdog timer initialization- set to 8 sec

  Serial.begin(115200);         // Start the Serial communication to send messages to the computer

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, HIGH); // initialize pin as off (active LOW)
  
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  WiFi.begin(ssid, password);             // Connect to the network
  delay(1200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.');
  }

  Serial.println(" OK");
  Serial.print("\nWiFi connected. IP address:\t");
  Serial.println(WiFi.localIP());

  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  ESP.wdtFeed();  //  watchdog timer feed
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
  if (millis() - previousReset > RESET_TIME) {
    ESP.restart();
  }
  delay(10);

  ESP.wdtFeed();  // watchdog timer feed in main loop
}
