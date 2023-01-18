/*
 * "iScream" is a fun and interactive game in which the players are competing to see who can make the loudest and longest screams!
 * 
 * The game uses the microphone sensor on the Circuit Playground board to track the decibel level of the screams and light up the LEDs accordingly.
 * The game has two modes: two-player and one-player, which are determined by a slide switch and can be changed at any time during the game. 
 * In two-player mode, players must beat each other's previous scores, whereas in one-player mode, regardless of the player's score, 
 * a constant number is added to the bar. 
 * 
 * The game ends when one player loses and the LEDs turn red. If a player screamed for as much time as needed, the LEDs turned white. 
 *
 * Inbar Lahat
 * Tal Barda
 */

#include <Adafruit_CircuitPlayground.h>

long int currentScore;
long int currentHighScore;
int setScoreMode;
int mode;
int onePlayerSettedScore;
int onePlayerCurrentLevel = 0;
int mappedSample;
float soundValue;
float avarageSoundValue = 50;
float sensitivity = 0.99;
bool isSettingHighScore = true;
bool twoPlayersMode = true;
int ledsMap[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
int ledsThresholds[] = {
  5000, 5900, 6000, 6100, 6200, 6300, 6400,
  6500, 6600, 6700, 6800, 6900, 7000
};
int thersholdsLength = sizeof(ledsThresholds) / sizeof(ledsThresholds[0]);


void setup() {
  CircuitPlayground.begin();
  currentHighScore = 0;
  currentScore = 0;
  mode = 0;
  GameStartMelody();
}

void loop()
{
  PlayGame();
}

/* 
   The game implementation for both one or two players is the same, but the new highscores are being determind differently,
   the modes are getting us the right update in each situation.
*/

void PlayGame()
{
  if (CircuitPlayground.slideSwitch()) // If slideSwitch is On - 2 players mode
    {
      twoPlayersMode = true;
    }
    else                                 // else - 1 player mode
    {
      twoPlayersMode = false;
    }
    
  getVoiceSample();

  if (mode != 2)
  {
  for (int i = 0; i < thersholdsLength; i++) {
      if (mappedSample > ledsThresholds[i]) {
          CircuitPlayground.setPixelColor(ledsMap[i], 0,   0,   255);
      }
      else {
          CircuitPlayground.setPixelColor(ledsMap[i], 0,   0,   0);
      }
    }
  }

//  Not yet got over the volium bar for start counting the scream time.  
  if ((mappedSample > 7000 ) && (mode == 0))
  {
    mode = 1;
  }

//  Got over the volium bar, score starts being added.
  if ((mappedSample > 7000 ) && (mode == 1))
  {
    currentScore = currentScore + 1;
  }

//  Got lower the volium bar, score stops being added.
  if ((mappedSample < 7000 ) && (mode == 1))
  {
    mode = 2;
  }

//  Checks if the score was good to win the round. If yes, updated it accorting to the game mode. If not, game over.
  if(mode == 2)
  {
    if (currentScore > currentHighScore) //Win
    {
      for (int i = 0; i < thersholdsLength; i++)
        CircuitPlayground.setPixelColor(ledsMap[i], 144, 238, 144);
      delay(1500);
      CircuitPlayground.playTone(687, 1500);

      if (twoPlayersMode)
      {
        currentHighScore = currentScore;
      }
      else
      {
        currentHighScore = currentHighScore + 200;
      }
      
      currentScore = 0;
    }
    else //Lose
    {
      for (int i = 0; i < 10; i++)
        CircuitPlayground.setPixelColor(ledsMap[i], 255, 0, 0);
     
      delay(1500);
      CircuitPlayground.playTone(369, 1500);
      currentScore = 0;
      currentHighScore = 0;
    }
    
    mode = 0;  
  }  
}

// Retrun a mapped sample from the sound sensor
void getVoiceSample()
{
  soundValue = CircuitPlayground.mic.soundPressureLevel(10);
  avarageSoundValue = sensitivity * avarageSoundValue + (1 - sensitivity) * soundValue;
  mappedSample = avarageSoundValue * 100;
}


void GameStartMelody()
{
  for (int i = 0; i < 10; i++)
        CircuitPlayground.setPixelColor(ledsMap[i], 0, 255, 0);

      for (int i = 0; i < 10; i++)
        CircuitPlayground.playTone(70*i, 100);
        
      for (int i = 0; i < 10; i++)
        CircuitPlayground.setPixelColor(ledsMap[i], 0, 0, 0);
}
