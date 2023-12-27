/*
 Tyler Strach - tstra6
 Manav Kolhi - mkohli4

 Group 57
 The Maze

 Arduino 2 - Handling User I/O

 An arcade style game with a base premise of a maze, with milestones that occur after the user comes into contact with a game object.
 These milestones will take advantage of Arduino materials at our disposal. The main output for the game is the 16x32 LED Matrix.
 This will include the score, main game, and minigames. The two Arduinos will be connected to one another via wiring, with one arduino focusing solely on the LED Matrix,
 and the other one focusing on all other materials included. This is because the LED Matrix requires a large number of wires to function.
*/

​​#include <Wire.h>
#include <LiquidCrystal.h>
#include <string.h>


#define leftButtonPin 12
#define upButtonPin 11
#define downButtonPin 10
#define rightButtonPin 9
#define buzzerPin 8


LiquidCrystal lcd(7, 6, 2, 3, 4, 5);


//Used for sending to output arduino
char up[2] = "u";
char down[2] = "d";
char left[2] = "l";
char right[2] = "r";
//states depending on whether the game is in minigame or main game state
int state = 1;
int gameStart = 0;
int score = 1000; //current score
//scoring is dependent on time
unsigned long int time = 0;
unsigned long int currMillis = 0;
unsigned long int prevMillis = 0;


//variables for Button States and Debouncing
int ButtonState1;
int ButtonState2;
int ButtonState3;
int ButtonState4;


int ButtonState1Confirm;
int ButtonState2Confirm;
int ButtonState3Confirm;
int ButtonState4Confirm;




void setup() {
 // put your setup code here, to run once:
 pinMode(leftButtonPin, INPUT);
 pinMode(upButtonPin, INPUT);
 pinMode(downButtonPin, INPUT);
 pinMode(rightButtonPin, INPUT);
 pinMode(buzzerPin, OUTPUT);
 Serial.begin(9600);
}


void loop() {
 // put your main code here, to run repeatedly:
  if (state == 1){
   ButtonState1 = digitalRead(leftButtonPin);
   ButtonState2 = digitalRead(upButtonPin);
   ButtonState3 = digitalRead(downButtonPin);
   ButtonState4 = digitalRead(rightButtonPin);
   delay(100);
   ButtonState1Confirm = digitalRead(leftButtonPin);
   ButtonState2Confirm = digitalRead(upButtonPin);
   ButtonState3Confirm = digitalRead(downButtonPin);
   ButtonState4Confirm = digitalRead(rightButtonPin);


   // handle user inputs and send to other arduino
   if(digitalRead(leftButtonPin) == HIGH){
     if (ButtonState1 == ButtonState1Confirm){
       Serial.write(left,2);
       gameStart = 1;
       delay(250);
     }
   }
   if(digitalRead(rightButtonPin) == HIGH){
     if (ButtonState2 == ButtonState2Confirm){
       Serial.write(right,2);
       gameStart = 1;
       delay(250);
     }
   }
   if(digitalRead(upButtonPin) == HIGH){
     if (ButtonState3 == ButtonState3Confirm){
       Serial.write(up,2);
       gameStart = 1;
       delay(250);
     }
   }
   if(digitalRead(downButtonPin) == HIGH){
     if (ButtonState4 == ButtonState4Confirm){
       Serial.write(down,2);
       gameStart = 1;
       delay(250);
     }
   }


   //add score in the form of time
   //update score
   currMillis = millis();
   if(currMillis - prevMillis > 1000 && gameStart == 1){
     prevMillis = currMillis;
     score -= 1;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Score: ");
     lcd.print(score);
   }


   // wait until no button is pressed to continue
   while(ButtonState1 || ButtonState2 || ButtonState3 || ButtonState4){
     ButtonState1 = digitalRead(leftButtonPin);
     ButtonState2 = digitalRead(upButtonPin);
     ButtonState3 = digitalRead(downButtonPin);
     ButtonState4 = digitalRead(rightButtonPin);
   }


   // gather info from other arduino
   if(Serial.available()>0){
     String gameEvent = Serial.readStringUntil('\n');
   
     if (gameEvent == "s"){
       state = 2;
     }
     else if (gameEvent == "b"){
       //buzzer handling
       tone(buzzerPin, 350);
       delay(100);
       noTone(buzzerPin);


     }
     else if (gameEvent == "w"){ // print the final score
       currMillis = 0;
       prevMillis = 0;
       int finalScore = score;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("Final Score:");
       lcd.print(finalScore);
       delay(100000);
       return;
     }
   }
 }
 else if (state == 2){ // minigame state
   //minigame input-handling
   unsigned long int start_timer = 0;
   unsigned long int cur_timer = 0;
   int inTime = 0;
   unsigned long int speed = 0;


  
   //receive start timer from other arduino
   start_timer = millis();
   cur_timer = millis();
   while(cur_timer - start_timer < 2000){
     ButtonState1 = digitalRead(leftButtonPin);
     ButtonState2 = digitalRead(upButtonPin);
     ButtonState3 = digitalRead(downButtonPin);
     ButtonState4 = digitalRead(rightButtonPin);
     if(ButtonState1 || ButtonState2 || ButtonState3 || ButtonState4){
       inTime = 1;
       speed = cur_timer - start_timer;
     }
     cur_timer = millis();
   }
   //handling for if player wins/loses minigame
   if ( inTime ){
     score += 250;
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("WIN! +250  ");
     lcd.print(speed);
     lcd.print("ms");
     delay(4000);
   }
   else{
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("LOSE! +0  ");
     lcd.print(speed);
     lcd.print("ms");
     delay(4000);
   }


   state = 1;
 }
 }
