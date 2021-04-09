# Fish Feeder
Arduino WIFI Fish Feeder controlled via Google Assistant

Components
* NodeMCU ESP8266 Development Board
* Stepper Motor and Driver (I used a 28BYJ-48 5V Stepper Motor with ULN2003 Driver Module)
* External Button (Optional used for feeding without sending WIFI command)

3D Files are avaliable [here](https://ql.nathan3dprint.tk/fish) if using this model you also need a film canistor to hold the fish food.

# Instructions

### ThingSpeak Instrucions

1. Create a [ThingSpeak](https://thingspeak.com/) Account
2. Create a ThingSpeak Channel
3. Create a ThingSpeak talkback
   1. Apps > TalkBack
   2. New TalkBack
4. Copy TalkBack ID and API Key

### Arduino Instructions
1. Download and install [Arduino](https://arduino.cc)
2. Open Arduino and open File > Preferences
3. Insert "http://arduino.esp8266.com/stable/package_esp8266com_index.json" into Additional Boards Manager URLs
4. Open Tools > Board > Board Manager
5. Search "esp8266" and install esp8266 by ESP8266 Community
6. Select Tools > Board > ESP8266 Boards > NodeMCU 1.0 (ESP-12E Module)
7. Select Arduino Port using Tools > Port
8. Open Fish Feeder Arduino Code
9. Insert your WIFI Settings and ThingSpeak API Key from above into Fish Feeder Code

### Google Assistant Integration
1. Go to [Google Assistant Console](https://console.actions.google.com/)
2. Create New Project
3. Select Custom as the action type
4. Select Blank
5. Enter your display name and Save
6. Create a new Custom Intent called "FeedFish"
7. Enter Training Phrase "Feed fish" and Save
8. Go to Webhook
9. Select HTTPS endpoint
10. Enter "https://api.thingspeak.com/talkbacks/TALKBACK_ID/commands?api_key=API_KEY&command_string=Feed&position=0 with your talkback ID and API Key and Save
11. Go to Main Invocation
12. Click the Your Action Bubble and tick "Call your Webhook"
13. Enter "FeedFish" as the handler and Save
14. Go to Test and click the "Talk to" button.
15. Open ThingSpeak Talkbacks at https://thingspeak.com/apps/talkbacks/ and see the Feed Command appear
16. Go to Deploy and enter you're information
17. Then go to Release and click Start new release
18. You can manage access by selecting Manage Alpha testers
