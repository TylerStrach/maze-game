/* 
  Tyler Strach - tstra6
  Manav Kolhi - mkohli4

  Group 57
  The Maze

  An arcade style game with a base premise of a maze, with milestones that occur after the user comes into contact with a game object. 
  These milestones will take advantage of Arduino materials at our disposal. The main output for the game is the 16x32 LED Matrix.
  This will include the score, main game, and minigames. The two Arduinos will be connected to one another via wiring, with one arduino focusing solely on the LED Matrix, 
  and the other one focusing on all other materials included. This is because the LED Matrix requires a large number of wires to function.
*/

#include <RGBmatrixPanel.h> // for the output panel
#include <string.h> // for string manipulation
#include <time.h> // for random number


#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

#define RED matrix.Color333(7,0,0)
#define GREEN matrix.Color333(0,7,0)
#define BLUE matrix.Color333(0,0,7)
#define BLACK matrix.Color333(0,0,0)


char start[2] = "s";
char win[2] = "w";
char buzz[2] = "b";

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

String userInput = "";
int prevX = 0;
int userX = 0;
int prevY = 7;
int userY = 7;

// represents the maze - 0=blank tile, 1=wall, 2=minigame, 3=end goal
char maze[15][31] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 2, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 2, 1, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 3},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

void drawBoard(){
  //draw border
  matrix.drawLine(0, 0, 0, 14, BLUE);
  matrix.drawLine(30, 0, 30, 14, BLUE);
  matrix.drawLine(0, 0, 30, 0, BLUE);
  matrix.drawLine(0, 14, 30, 14, BLUE);
  delay(250);

  // draw maze (even only)

  // horizontal lines
  matrix.drawLine(0, 4, 2, 4, BLUE);

  matrix.drawLine(2, 8, 2, 14, BLUE);
  matrix.drawLine(2, 4, 2, 2, BLUE);
  delay(50);

  matrix.drawLine(4, 0, 4, 2, BLUE);
  matrix.drawLine(4, 4, 4, 12, BLUE);
  delay(50);


  matrix.drawLine(6, 0, 6, 4, BLUE);
  matrix.drawLine(6, 10, 6, 14, BLUE);
  delay(50);

  matrix.drawLine(8, 8, 8, 10, BLUE);

  matrix.drawLine(10, 2, 10, 4, BLUE);
  matrix.drawLine(10, 10, 10, 12, BLUE);
  delay(50);

  matrix.drawLine(12, 0, 12, 4, BLUE);
  matrix.drawLine(12, 8, 12, 10, BLUE);
  delay(50);

  matrix.drawLine(14, 4, 14, 12, BLUE);
  delay(50);

  matrix.drawLine(16, 2, 16, 4, BLUE);
  matrix.drawLine(16, 6, 16, 10, BLUE);
  delay(50);

  matrix.drawLine(18, 2, 18, 6, BLUE);
  matrix.drawLine(18, 10, 18, 14, BLUE);
  delay(50);

  matrix.drawLine(20, 0, 20, 2, BLUE);
  matrix.drawLine(20, 4, 20, 12, BLUE);
  delay(50);

  matrix.drawLine(22, 2, 22, 4, BLUE);
  matrix.drawLine(22, 6, 22, 8, BLUE);
  matrix.drawLine(22, 10, 22, 14, BLUE);
  delay(50);

  matrix.drawLine(24, 2, 24, 8, BLUE);
  delay(50);

  matrix.drawLine(26, 2, 26, 6, BLUE);
  delay(50);

  matrix.drawLine(28, 0, 28, 4, BLUE);
  delay(50);

  //vertical lines
  matrix.drawLine(8, 2, 10, 2, BLUE);
  matrix.drawLine(14, 2, 16, 2, BLUE);
  matrix.drawLine(18, 2, 20, 2, BLUE);
  matrix.drawLine(22, 2, 24, 2, BLUE);
  delay(50);

  matrix.drawLine(0, 4, 2, 4, BLUE);
  matrix.drawLine(4, 4, 8, 4, BLUE);
  matrix.drawLine(10, 4, 12, 4, BLUE);
  matrix.drawLine(16, 4, 18, 4, BLUE);
  matrix.drawLine(20, 4, 22, 4, BLUE);
  delay(50);

  matrix.drawLine(2, 6, 4, 6, BLUE);
  matrix.drawLine(6, 6, 16, 6, BLUE);
  matrix.drawLine(22, 6, 24, 6, BLUE);
  matrix.drawLine(26, 6, 30, 6, BLUE);
  delay(50);

  matrix.drawLine(4, 8, 6, 8, BLUE);
  matrix.drawLine(8, 8, 12, 8, BLUE);
  matrix.drawLine(16, 8, 20, 8, BLUE);
  matrix.drawLine(24, 8, 30, 8, BLUE);
  delay(50);

  matrix.drawLine(6, 10, 8, 10, BLUE);
  matrix.drawLine(22, 10, 28, 10, BLUE);
  delay(50);

  matrix.drawLine(8, 12, 16, 12, BLUE);
  matrix.drawLine(24, 12, 30, 12, BLUE);
  delay(50);

  //draw minigame tiles
  matrix.drawPixel(23, 3, matrix.Color333(7, 0, 0));
  matrix.drawPixel(29, 13, matrix.Color333(7, 0, 0));
  matrix.drawPixel(1, 3, matrix.Color333(7, 0, 0));

  //draw player
  matrix.drawPixel(0, 7, BLACK);
  matrix.drawPixel(userX, userY, GREEN);

  //draw end goal 
  matrix.drawPixel(30, 7, matrix.Color333(7, 7, 0));

  delay(250);
}

// Receive user input from the other arduino u, d, l, r and adjust player pixel
void getUserInput(){
  if(Serial.available() > 0){
    String move = Serial.readStringUntil('\n');
    if(move == "u"){
      if( ((userY - 1) < 0) || (maze[userY-1][userX] == 1) ){
        Serial.write(buzz, 2);
        return;
      }
      else{
        userY = prevY-1;
      }
    }
    else if(move == "d"){
      if( ((userY + 1) > 14) || (maze[userY+1][userX] == 1) ){
        Serial.write(buzz, 2);
        return;
      }
      else{
        userY = prevY+1;
      }
    }
    else if(move == "l"){
      if( ((userX - 1) < 0) || (maze[userY][userX-1] == 1) ){
        Serial.write(buzz, 2);
        return;
      }
      else{
        userX = prevX-1;
      }
    }
    else if(move == "r"){
      if( ((userX + 1) > 31) || (maze[userY][userX+1] == 1) ){
        Serial.write(buzz, 2);
        return;
      }
      else{
        userX = prevX+1;
      }
    }
  }
}

// where all teh code lies, contains a while loop to simulate loop()
void setup() {
  matrix.begin();
  drawBoard();

  // setting the wire to listen for data from the other arduino
  Serial.begin(9600);

  // replicating the loop() function because matrix cannot be written to inside loop
  while(true){
    // check for the user input
    getUserInput();

    // checking for minigame
    if(maze[userY][userX] == 2){
      maze[userY][userX] = 0;
      // fill the screen with red
      matrix.fillScreen(RED);

      // generate random time from 3-12 seconds
      srand(time(NULL));
      int random_number = (rand() % 10 + 3);

      // delay until 0.5 second before the screen changes, and then send signal
      delay((random_number) * 1000);
      delay(500);
      Serial.write(start, 2); // send 's'
      delay(1000);
      matrix.fillScreen(GREEN); // change the screen for 2 seconds
      delay(500);
      matrix.fillScreen(BLACK); // change the screen for 2 seconds
      delay(5000);
      drawBoard();
      continue;
    }
    else if(maze[userY][userX] == 3){ // handle win state
      Serial.write(win, 2); // send 'w'
      matrix.fillScreen(BLACK);
      matrix.setCursor(6, 5);  // next line
      matrix.setTextColor(GREEN);
      matrix.print("WIN!");
      break;
    }

    //update player location
    matrix.drawPixel(prevX, prevY, matrix.Color333(0, 0, 0));
    matrix.drawPixel(userX, userY, matrix.Color333(0, 7, 0));
    prevX = userX;
    prevY = userY;
  }
}

void loop() {
  // cannot use loop to update the display
}
