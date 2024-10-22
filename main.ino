#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>
#define PIN        8 // LED devant le boitier 
#define NUMPIXELS 3  // Nombre de LED
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int reserveLedCible = -1; 
int intensiteLed=255;
int nbCibles=3;
int boutonG = 8;
int boutonD = 9;
int cible1 = 5;
int cible2 = 6;
int cible3 = 7;
int Buzzer = 2;
int statusBoutonG, statusBoutonD, boule1, boule2, boule3;
int cibleTours;

int score;

void setup() {
  pinMode(boutonG, INPUT_PULLUP);
  pinMode(boutonD, INPUT_PULLUP);
  pinMode(cible1,INPUT);
  pinMode(cible2,INPUT); 
  pinMode(cible3,INPUT);
  pixels.begin();
  pixels.clear();
  pixels.show();
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  AfficherMain();
}

void loop() {
  statusBoutonG = digitalRead(boutonG);
  statusBoutonD = digitalRead(boutonD); 
  boule1 = digitalRead(cible1);
  boule2 = digitalRead(cible2);
  boule3 = digitalRead(cible3);
  /*if (statusBoutonG==LOW){
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();
    delay(500);
  }
  else if (statusBoutonD==LOW){
    pixels.setPixelColor(2, pixels.Color(0, 0, 255));
    pixels.show();
    delay(500);
  } else {
    pixels.clear();
    pixels.show();
  }*/
  startGame();
}

void AfficherMain() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Commencer le jeu");
  
  lcd.setCursor(1, 2);
  lcd.print("<- Start");

  lcd.setCursor(12, 2);
  lcd.print("Info ->");
}

void SoundBuzzer() {
  tone(Buzzer,450,50);
  delay(500);
  noTone(Buzzer);
}

void SonPolice() {
  tone(Buzzer,435,1000);
  delay(500);
  tone(Buzzer,580,1000);
  delay(500);
  noTone(Buzzer);
}

void getStatus() {
  lcd.home();
  lcd.print(String(digitalRead(cible3)));
}

void startGame() {
  delay(5000);
  lcd.noDisplay();
  

}
