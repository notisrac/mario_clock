/**************************************************************************************************
 * SUPER MARIO CLOCK
 * (c) 2023 Noti
 * 
 * TODO https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
 *
 **************************************************************************************************/

// https://johnmu.com/2022-esp8266-wifi-speed/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <NTPClient.h>  // https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>

// #include <ESP8266HTTPClient.h>
// #include <WiFiClientSecure.h>

// #include <ArduinoJson.h>

#include <TimeLib.h>    // https://github.com/PaulStoffregen/Time
#include <Timezone.h>   // https://github.com/JChristensen/Timezone


#include "../include/clock_face_font.h"
#include "../include/gamespritesheet.h"
#include "../include/main_tilemap.h"

#include "../lib/gameengine/Game.h"
#include "../lib/gameengine/BitFlag.h"
#include "../lib/gameengine/Dictionary.h"

#include "../lib/gameengine/gameobject/gameobjects/clock/ClockFace.h"
#include "../lib/gameengine/gameobject/gameobjects/cloud/Cloud.h"
// #include "../lib/gameengine/gameobject/gameobjects/lakitucloud/LakituCloud.h"
#include "../lib/gameengine/gameobject/gameobjects/mario/Mario.h"
#include "../lib/gameengine/tilemap/TileMap.h"
#include "../lib/gameengine/spritesheet/SpriteSheet.h"

#define WIDTH 128
#define HEIGHT 128
#define SCALE 4
#define FPS 60

#define TIME_UPDATE_INTERVAL 10000

const char *ssid     = "...";
const char *password = "...";
// String timeServerUrl = "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Budapest";


WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "ntp.telekom.hu", 3600000); // update time every 1 hour
NTPClient timeClient(ntpUDP);
bool wifiConnectedNotified = false;
// unsigned long timeLastUpdated = 0;
int timeMinutes = 0;
int timeHours = 0;
// JsonDocument doc;

// Central European Time Zone (Budapest)
TimeChangeRule myDST = {"CEST", Last, Sun, Mar, 1, 120};    // Daylight time = UTC + 2 hours
TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 1, 60};     // Standard time = UTC + 1 hours
Timezone myTZ(myDST, mySTD);
TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev

const int frameDelay = 1000 / FPS;

unsigned long frameStart = 0;
int frameTime;
int lastFrameTime = 0;
int lastClockBumpTime = 0;
int worldXPos = 0;

Renderer renderer = Renderer(WIDTH, HEIGHT, SCALE, nullptr, 0x5cbf, 0xd97e);

Game game = Game(&renderer);

// load sprite sheets
SpriteSheet gameSprites = SpriteSheet(spritesheet, SPRITESHEET_WIDTH, SPRITESHEET_HEIGHT, 16, 16);
SpriteSheet clockFontSprites = SpriteSheet(clock_face_font, CLOCK_FACE_FONT_DIGIT_WIDTH * 10, CLOCK_FACE_FONT_DIGIT_HEIGHT, CLOCK_FACE_FONT_DIGIT_WIDTH, CLOCK_FACE_FONT_DIGIT_HEIGHT);

// create game objects
// LakituCloud lakituCloud = LakituCloud(&gameSprites, &renderer, -20, -20);
ClockFace clockFace = ClockFace(&gameSprites, &clockFontSprites, &renderer, 0, 0);
Mario mario = Mario(&gameSprites, &renderer, 0, 80);
TileMap tileMap = TileMap(&gameSprites, &renderer, main_tile_map, MAIN_TILEMAP_WIDTH, MAIN_TILEMAP_HEIGHT);
Cloud cloud1 = Cloud(&gameSprites, &renderer, -20, -20, 0, 300);
Cloud cloud2 = Cloud(&gameSprites, &renderer, -20, -20, 0, 200);

int cnt = 0;
int screenCenter = (WIDTH / 2 - mario.getWidth() / 2);

bool automationActive = true;
bool loopActive = true;
unsigned long gameStart = millis();

time_t requestTimeSync()
{
	if (WiFi.status() == WL_CONNECTED)
	{
		if(timeClient.update())
		{
			Serial.printf_P(PSTR("NTP time: %s\n"), timeClient.getFormattedTime().c_str());
			time_t utc = timeClient.getEpochTime();
			time_t localTime = myTZ.toLocal(utc, &tcr);
			
			return localTime;
		}
		else
		{
			Serial.println(F("Failed to update time!"));
		}		
	}

	return 0;
}


void setup()
{
	// register game objects - this specifies the render order too!
	game.addGameObject(&tileMap);
	game.addGameObject(&cloud1);
	game.addGameObject(&cloud2);
	game.addGameObject(&clockFace);
	game.addGameObject(&mario);
	// game.addGameObject(&lakituCloud);

	// add the dynamic game objects, that represent an item on the tilemap to the tilemap
	tileMap.registerGameObject(0, &clockFace);
	tileMap.registerGameObject(1, &cloud1);
	tileMap.registerGameObject(2, &cloud2);

	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println();
	Serial.println(F(" -= Super Mario Clock =-"));
	Serial.println(F("(c) noti, 2023"));
	Serial.println();

	Serial.printf_P(PSTR("%d bytes free\n"), ESP.getFreeHeap());
	Serial.printf_P(PSTR("Heap fragmentation: %d\n"), ESP.getHeapFragmentation());
	Serial.printf_P(PSTR("Vcc: %1.2fV\n"), (float)ESP.getVcc() / 1000.0);
	Serial.println();

	Serial.printf_P(PSTR("Connecting to %s...\n"), ssid);
	// Serial.setDebugOutput(true);
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	setSyncProvider(requestTimeSync);  //set function to call when sync required
	setSyncInterval(3600);         // set the number of seconds between re-sync
	// game.render();
}

void loop()
{
	lastFrameTime = millis() - frameStart;
	frameStart = millis();

	BitFlag *events = game.handleEvents();

	// render everything, including the new location for mario
	game.update(lastFrameTime);

	// update the world pos ( = mario's pos)
	worldXPos = mario.getXPos();

	// wifi related stuff
	if (WiFi.status() == WL_CONNECTED)
	{
		if (!wifiConnectedNotified)
		{ // not notified yet
			// Serial.println(F("Connected to WIFI!"));
			Serial.printf_P(PSTR("Connected! (IP: %s)\n"), WiFi.localIP().toString().c_str());
			//WiFi.printDiag(Serial);
			events->SetFlag(EventTypes::ButtonSpace);
			wifiConnectedNotified = true;

			// Connect to the NTP client
			timeClient.begin();
			timeClient.setTimeOffset(0);
			//timeClient.forceUpdate();
			setTime(requestTimeSync());
			timeHours = hour();
			timeMinutes = minute();
			clockFace.update(lastFrameTime, timeHours, timeMinutes);
		}
		else
		{ // wifi connected and notified
			if (events->HasFlag(EventTypes::ButtonSpace))
			{ // unset the "allow Mario to continue to the clock" button
				events->UnsetFlag(EventTypes::ButtonSpace);
			}
			if (events->HasFlag(EventTypes::ButtonUp))
			{ // release the up button, so Mario only jumps once
				events->UnsetFlag(EventTypes::ButtonUp);
			}

			timeHours = hour();
			int newMinutes = minute();

			if (newMinutes != timeMinutes)
			{ // a minute has passed, signal the jump
				if (!automationActive)
				{ // prevent Mario from jumping while not at the clock
					events->SetFlag(EventTypes::ButtonUp);
				}
				timeMinutes = newMinutes;
				Serial.printf("%s\n", timeClient.getFormattedTime().c_str());
			}			
		}
	}
	else
	{ // waiting for connection
		Serial.print(F("."));
	}
	

	if (automationActive)
	{
		if (millis() - gameStart > 2000)
		{ // wait a few seconds on the title part of the map
			// while not in position under the clock
			if (worldXPos <= 43 * gameSprites.spriteWidth())
			{
				// keep pressing forward
				events->SetFlag(EventTypes::ButtonRight);
			}
			else
			{ // in position
				// release the button
				events->UnsetFlag(EventTypes::ButtonRight);
				// start the moving clouds
				cloud1.setSpeed(-40);
				cloud2.setSpeed(25);
				// automation can be stopped
				automationActive = false;
			}
		}

		// break the loop on space bar
		if (events->HasFlag(EventTypes::ButtonSpace))
		{
			loopActive = false;
		}

		// loop
		if (loopActive && worldXPos >= 36 * gameSprites.spriteWidth())
		{
			worldXPos = 12 * gameSprites.spriteWidth();
			mario.setPosition(worldXPos, mario.getYPos(), false);
		}
	}


	// if mario is in the center of the screen, hold him there
	if (worldXPos > screenCenter && worldXPos < (tileMap.getWidth() - screenCenter))
	{
		mario.setPosition(screenCenter, mario.getYPos(), true);
	}
	// at the right end of the map, let mario go, so the map can stay put
	if (worldXPos >= (tileMap.getWidth() - WIDTH / 2 - mario.getWidth() / 2))
	{
		mario.setPosition(worldXPos - (tileMap.getWidth() - WIDTH), mario.getYPos(), true);
	}

	// move the tilemap based on the world pos
	if (worldXPos > screenCenter)
	{
		tileMap.setPosition(worldXPos - screenCenter, 0);
	}

	// check the collision of mario and the clock
	if (game.detectCollision(mario.getXPos(), mario.getYPos(), mario.getWidth(), mario.getHeight(), clockFace.getXPos(), clockFace.getYPos(), clockFace.getWidth(), clockFace.getHeight()))
	{
		if (millis() - lastClockBumpTime > 500)
		{
			clockFace.handleEvents(&BitFlag().SetFlag(EventTypes::ButtonReturn));
			lastClockBumpTime = millis();
			clockFace.update(lastFrameTime, timeHours, timeMinutes);
			mario.collisionTop();
		}
	}

	game.render();

	// time took to process and render frame, in ms
	frameTime = millis() - frameStart;
	//Serial.println(1000 / frameTime); // FPS

	if (frameDelay > frameTime)
	{
		delay(frameDelay - frameTime);
	}

	// Serial.println(ESP.getFreeHeap());
}
