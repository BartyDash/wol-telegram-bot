<div align="center">
  <h1>Wake on Lan ESP8266 Telegram Bot</h1>
</div>

## 📄 Description

This is a sketch for the ESP8266 board. The telegram bot is hosted on the board and listens for messages, after receiving the command `/wol` magic Wake-on-Lan packet is broadcast on the local network to turn on your PC.

## ⚙️ Config

Passwords and sensitive data are stored in the `secrets.h` file. Save the file in a folder according to the template:
```c++
#define SECRET_SSID "your_ssid"
#define SECRET_PASS "your_password"
#define BOT_TOKEN "0000000000:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define MY_ID "your_telegram_id"
#define MAC_ADDR "FF:FF:FF:FF:FF:FF"
#define PC_IP IPAddress(192, 168, 0, 1)
```
## 🔎 Usage
- Use `/start` to get a list of the available commands
- Use the `/wol` command to turn on your PC
- Use the `/ping` command to check if the bot is online
- Use the `/status` command to check if your PC is on or off
- Use the `/ledon` command to switch the Led ON
- Use the `/ledoff` command to switch the Led OFF
