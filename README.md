# Wake on Lan ESP8266 Telegram Bot

## Config

Passwords and sensitive data are stored in the `secrets.h` file. Save the file in a folder according to the template:
```c++
#define SECRET_SSID "your_ssid"
#define SECRET_PASS "your_password"
#define BOT_TOKEN "0000000000:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define MY_ID "your_telegram_id"
#define MAC_ADDR "FF:FF:FF:FF:FF:FF"
#define PC_IP IPAddress(192, 168, 0, 1)
```
