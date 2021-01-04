# LightSwarm
Wireless awesome light project for BurningMan and beyond.

A platform for art cars, art bikes, wearable art, and stationary art to synchronize their LED animations.  Various art pieces can communicate with each other to simulate dancing, flirting, or other emotions and expressions.

## Videos of LightSwarm in Action
https://www.facebook.com/groups/717748685076630/permalink/724183724433126/
https://www.facebook.com/jc.ebert/videos/10211179810025091/
https://www.facebook.com/rolf.kirby/videos/10154702708152507/
https://www.facebook.com/jc.ebert/videos/10211283437215706/

## Definitions
Below is an unsorted list of capabilities, libraries, and modules that we are working towards.

### NODE
* Unit that can receive and send messages, usually wirelessly
* Usually controls one or more LEDs, sometimes several hundred LEDs organized into stands or tentacles

### ESP8266
* Originally 2.4GHz Wifi chip
* "Hijacked" by the open source community as cheap arduino killer with radio
* We have selected ESP8266 as the initial MCU+Radio because of cost and community and support
* The main module is the "Wemos D1 mini" for now

## Libraries and Tools
Quick list of software components we use.

### painlessMesh
https://gitlab.com/BlackEdder/painlessMesh/wikis/home

* An ad-hoc (no hub access point) library that allows ESPs to connect with each other without a WiFi hub
* Ad-hoc mesh networking allows nodes to come and go
* Provides time synchronization which is KEY to syncing animations

### FastLED
http://fastled.io/

* Library to drive WS2812 (2811, 2813) LEDs
* Tried and true with any hardware targets
* Latest branch/commit allows ESP8266 to drive 4 LED strands in parallel.

### ArduinoJSON
https://github.com/bblanchon/ArduinoJson

* Easy library to serialize JSON
