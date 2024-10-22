//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int Cible1 = 5;
int Cible2 = 6;
int Cible3 = 7;
//pin8 neopixel
int BoutonGauche = 9; 
int BoutonDroit = 10;
int Buzzer = 11;
int GameState = 0;
int NombreCibles = 3;
int Target1Status=0;
int Target2Status=0;
int Target3Status=0;
// Timer variables
int MillisTimer = millis();
int timer = 60;
int oldMillisTimer = 0;

int score;
int bestScore[] = {0, 0, 0};

bool scoreDisplay = false; 
bool goDisplay = false;

uint8_t blank[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

uint8_t custChar[8][8] = {
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
},// Small top line - 0

{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
},// Small bottom line - 1


{
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
},// Small lines top and bottom -2


{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
}, // -3



{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111,
  B00111
},// Left bottom chamfer full - 4

{
  B11100,
  B11110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
},// Right top chamfer full -5


{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11110,
  B11100
},// Right bottom chamfer full -6

{
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
},// Left top chamfer full -7
 
};

// Define our numbers 0 thru 9
// 254 is blank and 255 is the "Full Block"
uint8_t bigNums[38][6] = {
  {7, 0, 5, 4, 1, 6}, //0
  {0, 5, 254, 1, 255, 1},     //1
  {0, 0, 5, 4, 2, 2},         //2
  {3, 3, 5, 2, 2, 6},         //3
  {4, 1, 5, 254, 254, 255},   //4
  {255, 3, 3, 2, 2, 6},       //5 
  {7, 3, 3, 4, 2, 6},         //6 
  {0, 0, 5, 254, 7, 254},     //7 
  {7, 3, 5, 4, 2, 6},         //8 
  {7, 3, 5, 254, 254, 6},     //9
  {0, 3, 5, 254, 2, 254},//? 10
  {254, 255, 254, 254, 2, 254},//! 11 
 {7, 3, 5, 255, 254, 255}, //A 12
 {255, 3, 6, 255, 2, 5}, //B 13
 {7, 0, 0, 4, 1, 1}, //C 14
 {255, 0, 5, 255, 1, 6}, //D 15
 {255, 3, 3, 255, 2, 2}, //E 16
 {255, 3, 3, 255, 254, 254}, //F 17
 {7, 0, 0, 4, 1, 5}, //G 18
{255, 1, 255, 255, 254, 255},  //H 19
 {0, 255, 0, 1, 255, 1}, //I 20
 {254, 254, 255,1,1, 6}, //J 21
 {255, 1, 6, 255, 254, 5}, //K 22
 {255, 254, 254, 255, 1, 1}, //L 23
 {5,1,7,255,254,255}, //M 24
 {5,1,255,255,0,4}, //N 25
 {7, 0, 5, 4, 1, 6}, //O 26
 {255, 3, 5, 255, 254, 254}, //P 27
 {7,0,5,4,1,255}, //Q 28
 {255, 3, 5, 255, 254,5}, //R 29
 {7, 3, 3, 2, 2,6}, //S 30
 {0, 255, 0, 254, 255, 254}, //T 31
 {255, 254, 255, 4, 1, 6}, //U 32
 {5, 254, 7, 4, 1, 6}, //V 33
 {5, 1, 7, 4,255, 6}, //W 34
 {4, 1, 6, 7, 254, 5}, //X 35
 {4,1,6,254,255,254}, //Y 36
 {0,3,6,7,2,1}, //Z 37
  
};

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN        8 // 
#define NUMPIXELS  3
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void setup()
{
  pinMode(BoutonGauche, INPUT_PULLUP);
  pinMode(BoutonDroit, INPUT_PULLUP);
  pinMode(Cible1,INPUT);
  pinMode(Cible2,INPUT); 
  pinMode(Cible3,INPUT);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  for (int cnt = 0; cnt < sizeof(custChar) / 8; cnt++) {
        lcd.createChar(cnt, custChar[cnt]);
    }

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
 
  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(0, 0, 150));
  pixels.show();
  tone(Buzzer, 523,1000);
  delay(1000);
 
  pixels.setPixelColor(1, pixels.Color(150, 150, 150));

  pixels.show();
  tone(Buzzer, 587,1000);
  delay(1000);

  pixels.setPixelColor(2, pixels.Color(150, 0, 0));
  pixels.show();
  tone(Buzzer, 659,1000);
  delay(1000);
  pixels.clear();
  pixels.show();
  Serial.begin(9600);
}


void loop()
{
  
  if (GameState == 0){
    printJAVtoLCD();
    debug();
    if (! digitalRead(BoutonDroit)){
      EcranGo();
      GameState = 1;
    }
  }
  else if (GameState == 1){
    if (! digitalRead(Cible1)){
      pixels.setPixelColor(0, pixels.Color(0, 150, 0));
      pixels.show();
      Target1Status=1;
      soundHit();
    }
    if (! digitalRead(Cible2)){
      pixels.setPixelColor(1, pixels.Color(0, 150, 0));
      pixels.show();
      Target2Status=1;
      soundHit();     
    }
    if (! digitalRead(Cible3)){
      pixels.setPixelColor(2, pixels.Color(0, 150, 0));
      pixels.show();
      Target3Status=1;
      soundHit();     
    }
    if (Target1Status==1 && Target2Status==1 && Target3Status==1){
      GameState=3;
    }

    lcd.setCursor(13, 0);

    MillisTimer=millis();
    if (MillisTimer - oldMillisTimer > 1000) {
      timer -= 1;
      oldMillisTimer = MillisTimer;
      AffichageTimer();
    }

    if (timer < 10 && timer > 0) {
      LowTime();
    }
    else if (timer <= 0) {
      tone(Buzzer, 1500, 1000);
      EndGame();
    }
  }
  else if (GameState == 3) {
    EndGame();
  }
  
  if(!digitalRead(BoutonGauche)) {
    Replay();
  }
}

void debug()
{
  Serial.print("btG = ");
  Serial.print(digitalRead(BoutonGauche));
  Serial.print(" btD = ");
  Serial.print(digitalRead(BoutonDroit));
  Serial.print(" cible1 = ");
  Serial.print(digitalRead(Cible1));
  Serial.print(" cible2 = ");
  Serial.print(digitalRead(Cible2));
  Serial.print(" cible3 = ");
  Serial.println(digitalRead(Cible3));
  if (! digitalRead(Cible1)){
      pixels.setPixelColor(0, pixels.Color(0, 250, 0));
      pixels.show();
      
  }
  else{
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  }
    
  if (! digitalRead(Cible2)){
    pixels.setPixelColor(1, pixels.Color(0, 250, 0));
    pixels.show();     
  } else{
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();     
  }
    
  if (! digitalRead(Cible3)){
    pixels.setPixelColor(2, pixels.Color(0, 250, 0));
    pixels.show();     
  } else {
    pixels.setPixelColor(2, pixels.Color(0, 0, 0));
    pixels.show();     
  }
}


void printBigNum(int number, int startCol, int startRow) {
  // Position cursor to requested position (each char takes 3 cols plus a space col)
  lcd.setCursor(startCol, startRow);
  // Each number split over two lines, 3 chars per line. Retrieve character
  // from the main array to make working with it here a bit easier.
  uint8_t thisNumber[6];
  for (int cnt = 0; cnt < 6; cnt++) {
    thisNumber[cnt] = bigNums[number][cnt];
  }
  // First line (top half) of digit
  for (int cnt = 0; cnt < 3; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }
  // Now position cursor to next line at same start column for digit
  lcd.setCursor(startCol, startRow + 1);
  // 2nd line (bottom half)
  for (int cnt = 3; cnt < 6; cnt++) {
    lcd.print((char)thisNumber[cnt]);
  }
}


void printJAVtoLCD(){
  printBigNum(21, 2, 0);
  printBigNum(12, 8, 0);
  printBigNum(33, 14, 0);
  lcd.setCursor(2, 3);
  lcd.print("---------------");
}

void EcranGo()
{
  if (!goDisplay) {
    lcd.clear();
    goDisplay = true;
    tone(Buzzer,523,1000);
    delay(1000);  
    tone(Buzzer,523,1000);
    delay(1000);
    printBigNum(18, 7, 1);
    printBigNum(26, 11, 1);
    tone(Buzzer,1000,1000); 
    delay(1000);
  } 
}

void EndGame() {
  if (!scoreDisplay) {
    lcd.clear();
    scoreDisplay = true;
    lcd.setCursor(3, 0);
    lcd.print("Score:");

    score = timer * 10;

    if (score > bestScore[0]) {
      bestScore[0] = score;
    } 
    else if (score > bestScore[1] && score < bestScore[1]) {
      bestScore[1] = score;
    } 
    else if (score > bestScore[2] && score < bestScore[2]) {
      bestScore[2] = score;
    }

    lcd.setCursor(9, 0);
    lcd.print(score);
    printBigNum(score, 1, 1);

    AfficherScore();
  }
}

void Replay() {
  lcd.clear();
  score = 0;
  goDisplay = false;
  scoreDisplay = false;
  Target1Status = 0;
  Target2Status = 0;
  Target3Status = 0;
  timer = 60;
  GameState = 0;

}

void soundHit(){
  tone(Buzzer,988,125);
  delay(125);
  noTone(Buzzer);
  tone(Buzzer,1319,600);
  delay(600);
  noTone(Buzzer);
}

void LowTime() {
  tone(Buzzer, 1000, 1000);
  delay(100);
  tone(Buzzer, 1000, 1000);
  delay(100);
}

void AffichageTimer(){
  lcd.clear();
  lcd.setCursor(7, 0);
  lcd.print("Temps:");
  String scorestring = String(timer);
  if (scorestring.length()==3){
    String centaine = String(scorestring.charAt(0));
    printBigNum(centaine.toInt(), 5, 1);
    String decimale = String(scorestring.charAt(1));
    printBigNum(decimale.toInt(), 8, 1);
    String unite = String(scorestring.charAt(2));
    printBigNum(unite.toInt(), 11, 1);
  }
  else if (scorestring.length()==2){
    String decimale = String(scorestring.charAt(0));
    printBigNum(decimale.toInt(), 7, 1);
    String unite = String(scorestring.charAt(1));
    printBigNum(unite.toInt(), 10, 1);
  }
  else{
    String unite = String(scorestring.charAt(0));
    printBigNum(0, 8, 1);
    printBigNum(unite.toInt(), 11, 1);
  }
  
}

void AfficherScore(){
   lcd.clear();
   lcd.setCursor(7, 0);
   lcd.print("Score:");
    String scorestring = String(score);
    if (scorestring.length()==3){
      String centaine = String(scorestring.charAt(0));
      printBigNum(centaine.toInt(), 5, 1);
      String decimale = String(scorestring.charAt(1));
      printBigNum(decimale.toInt(), 8, 1);
      String unite = String(scorestring.charAt(2));
      printBigNum(unite.toInt(), 11, 1);
    }
    else if (scorestring.length()==2){
      String decimale = String(scorestring.charAt(0));
      printBigNum(decimale.toInt(), 7, 1);
      String unite = String(scorestring.charAt(1));
      printBigNum(unite.toInt(), 10, 1);
    }
    else{
      String unite = String(scorestring.charAt(0));
      printBigNum(0, 8, 1);
      printBigNum(unite.toInt(), 11, 1);
    }
  
}
