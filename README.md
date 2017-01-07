# TelegramDoor
Using Telegram to open doors and chat presence as access control. Runs on Genuino MKR1000 and uses a relay to trigger the door. 

Send a keyword to a telegram bot, which will trigger a relay to press/short a button on a remote to open that door. Set up to work with multiple doors. Simply run one per door, but make sure, they run off the same code and only differ in the config.key.h where noted. 

The code uses 1 bot and 2 channels. One channel to log all actions to and one which grants access. I.e. every member in that channel has access to the door. Users directly talk to the bot in a private message to open the doors. 

## Notes
- Setup as a Platform.io project, including the required libraries. 
- There are a few changes to the Telegram-Bot library, rest is original. 
