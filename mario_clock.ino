#include <TimeLib.h>

#include <TFT_eSPI.h>       // Hardware-specific library
#include "TFT_eSPI_User_Setup.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
TFT_eSprite spr_clock_block = TFT_eSprite(&tft);
TFT_eSprite spr_mario = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
TFT_eSprite spr_background = TFT_eSprite(&tft);
TFT_eSprite spr_ground = TFT_eSprite(&tft);
TFT_eSprite spr_bush = TFT_eSprite(&tft);
TFT_eSprite spr_hill = TFT_eSprite(&tft);
TFT_eSprite spr_cloud = TFT_eSprite(&tft);


#include "bush.h"
#include "clock_bg.h"
#include "cloud.h"
#include "clock_face_font.h"
#include "ground.h"
#include "hill.h"
#include "mario_jump.h"
#include "mario_stand.h"
#include "mario_walk_1.h"
#include "mario_walk_2.h"
#include "mario_walk_3.h"

#define TRANSPARENT_COLOR 0xd97e
#define BACKGROUND_COLOR 0x5cbf

#define CLOCK_DIGIT_Y 26
#define CLOCK_FIRST_DIGIT_TENS 19
#define CLOCK_FIRST_DIGIT_ONES 40
#define CLOCK_SECOND_DIGIT_TENS 67
#define CLOCK_SECOND_DIGIT_ONES 88

const unsigned short * clock_face_font[] = {
  clock_face_font_0,
  clock_face_font_1,
  clock_face_font_2,
  clock_face_font_3,
  clock_face_font_4,
  clock_face_font_5,
  clock_face_font_6,
  clock_face_font_7,
  clock_face_font_8,
  clock_face_font_9
};

// void drawBackground() {
//   tft.fillScreen(BACKGROUND_COLOR);

//   for (uint8_t i = 0; i < tft.width() / GROUND_WIDTH; i++) {
//     tft.pushImage(i * GROUND_WIDTH, 112, GROUND_WIDTH, GROUND_HEIGHT, ground, TRANSPARENT_COLOR);
//   }
//   tft.pushImage(-40, 77, HILL_WIDTH, HILL_HEIGHT, hill, TRANSPARENT_COLOR);
//   tft.pushImage(84, 96, BUSH_WIDTH, BUSH_HEIGHT, bush, TRANSPARENT_COLOR);

//   tft.pushImage(-20, 32, CLOUD_WIDTH, CLOUD_HEIGHT, cloud, TRANSPARENT_COLOR);
//   tft.pushImage(115, 4, CLOUD_WIDTH, CLOUD_HEIGHT, cloud, TRANSPARENT_COLOR);

//   tft.pushImage(15, 14, CLOCK_BG_WIDTH, CLOCK_BG_HEIGHT, clock_bg, TRANSPARENT_COLOR);
//   tft.pushImage(63, 14, CLOCK_BG_WIDTH, CLOCK_BG_HEIGHT, clock_bg, TRANSPARENT_COLOR);
// }

// void displayTime(uint8_t hours, uint8_t minutes) {
//   tft.pushImage(CLOCK_FIRST_DIGIT_TENS, CLOCK_DIGIT_Y, CLOCK_FACE_FONT_DIGIT_WIDTH, CLOCK_FACE_FONT_DIGIT_HEIGHT, clock_face_font[hours / 10]/*, TRANSPARENT_COLOR*/);
//   tft.pushImage(CLOCK_FIRST_DIGIT_ONES, CLOCK_DIGIT_Y, CLOCK_FACE_FONT_DIGIT_WIDTH, CLOCK_FACE_FONT_DIGIT_HEIGHT, clock_face_font[hours % 10], TRANSPARENT_COLOR);
//   tft.pushImage(CLOCK_SECOND_DIGIT_TENS, CLOCK_DIGIT_Y, CLOCK_FACE_FONT_DIGIT_WIDTH, CLOCK_FACE_FONT_DIGIT_HEIGHT, clock_face_font[minutes / 10], TRANSPARENT_COLOR);
//   tft.pushImage(CLOCK_SECOND_DIGIT_ONES, CLOCK_DIGIT_Y, CLOCK_FACE_FONT_DIGIT_WIDTH, CLOCK_FACE_FONT_DIGIT_HEIGHT, clock_face_font[minutes % 10], TRANSPARENT_COLOR);
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Mario Clock"));
  Serial.println(ESP.getFreeHeap());

  tft.begin();
  tft.setRotation(1);	// landscape

  // Swap the colour byte order when rendering
  tft.setSwapBytes(true);

  tft.fillScreen(TRANSPARENT_COLOR);

  // Draw the background assets
  spr_background.createSprite(tft.width(), tft.height());
  spr_background.fillSprite(BACKGROUND_COLOR);

  // crate sprites for all the elements
  spr_ground.createSprite(GROUND_WIDTH, GROUND_HEIGHT);
  spr_ground.setSwapBytes(true);
  spr_ground.pushImage(0, 0, GROUND_WIDTH, GROUND_HEIGHT, ground);

  spr_bush.createSprite(BUSH_WIDTH - 20, BUSH_HEIGHT);
  spr_bush.setSwapBytes(true);
  spr_bush.pushImage(0, 0, BUSH_WIDTH, BUSH_HEIGHT, bush);

  spr_hill.createSprite(HILL_WIDTH - 40, HILL_HEIGHT);
  spr_hill.setSwapBytes(true);
  spr_hill.pushImage(0 - 40, 0, HILL_WIDTH, HILL_HEIGHT, hill);

  spr_cloud.createSprite(CLOUD_WIDTH, CLOUD_HEIGHT);
  spr_cloud.setSwapBytes(true);
  spr_cloud.pushImage(0, 0, CLOUD_WIDTH, CLOUD_HEIGHT, cloud);


  spr_mario.createSprite(MARIO_STAND_WIDTH, MARIO_STAND_HEIGHT);
  spr_mario.setSwapBytes(true);
  spr_mario.pushImage(0, 0, MARIO_STAND_WIDTH, MARIO_STAND_HEIGHT, mario_stand);

  spr_clock_block.createSprite(CLOCK_BG_WIDTH, CLOCK_BG_HEIGHT);
  spr_clock_block.setSwapBytes(true);
  spr_clock_block.pushImage(0, 0, CLOCK_BG_WIDTH, CLOCK_BG_HEIGHT, clock_bg);


  for (uint8_t i = 0; i < (tft.width() / GROUND_WIDTH); i++) {
    spr_ground.pushToSprite(&spr_background, i * GROUND_WIDTH, 112, TRANSPARENT_COLOR);
  }
  spr_bush.pushToSprite(&spr_background, 84, 96, TRANSPARENT_COLOR);
  spr_hill.pushToSprite(&spr_background, 0, 77, TRANSPARENT_COLOR);
  spr_cloud.pushToSprite(&spr_background, -20, 32, TRANSPARENT_COLOR);
  spr_cloud.pushToSprite(&spr_background, 115, 4, TRANSPARENT_COLOR);



  spr_clock_block.pushToSprite(&spr_background, 15, 14, TRANSPARENT_COLOR);
  spr_clock_block.pushToSprite(&spr_background, 63, 14, TRANSPARENT_COLOR);
  spr_mario.pushToSprite(&spr_background, 47, 80, TRANSPARENT_COLOR);
  spr_background.pushSprite(0, 0);

  Serial.println(ESP.getFreeHeap());

  // Pause here to admire the icons!
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  // drawBackground();
  //tft.fillScreen(BACKGROUND_COLOR);
  // spr_mario.pushSprite(random(tft.width() - MARIO_STAND_WIDTH), random(tft.height() - MARIO_STAND_HEIGHT) , TRANSPARENT_COLOR);
  //spr_mario.pushSprite(random(10), random(10) , TRANSPARENT_COLOR);

  spr_background.fillSprite(BACKGROUND_COLOR);
  for (uint8_t i = 0; i < (tft.width() / GROUND_WIDTH); i++) {
    spr_ground.pushToSprite(&spr_background, i * GROUND_WIDTH, 112, TRANSPARENT_COLOR);
  }
  spr_bush.pushToSprite(&spr_background, 84, 96, TRANSPARENT_COLOR);
  spr_hill.pushToSprite(&spr_background, 0, 77, TRANSPARENT_COLOR);
  spr_cloud.pushToSprite(&spr_background, -20, 32, TRANSPARENT_COLOR);
  spr_cloud.pushToSprite(&spr_background, 115, 4, TRANSPARENT_COLOR);

  spr_clock_block.pushToSprite(&spr_background, 15, 14, TRANSPARENT_COLOR);
  spr_clock_block.pushToSprite(&spr_background, 63, 14, TRANSPARENT_COLOR);
  // spr_mario.pushToSprite(&spr_background, 47, 80, TRANSPARENT_COLOR);
  spr_mario.pushToSprite(&spr_background, random(100), random(100), TRANSPARENT_COLOR);
  
  spr_background.pushSprite(0, 0);
  //delay(100);
}
