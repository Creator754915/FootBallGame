#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>
#define PIN        4 // LED devant le boitier 
#define NUMPIXELS 3  // Nombre de LED
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int reserveLedCible = -1; 
int intensiteLed = 255;
int nbCibles = 3;
int boutonG = 8;
int boutonD = 9;
int cible1 = 5;
int cible2 = 6;
int cible3 = 7;
int Buzzer = 2;
int statusBoutonG, statusBoutonD, boule1, boule2, boule3;
int cibleTours;
int timer = 60;

int score;
int bestScore[] = {0, 0, 0};

byte underscore[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};

byte f_letter[] = {
  B11111,
  B10000,
  B10000,
  B11100,
  B10000,
  B10000,
  B10000,
  B10000
};

byte i_letter[] = {
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111
};

byte n_letter[] = {
  B10001,
  B11001,
  B10101,
  B10101,
  B10101,
  B10101,
  B10011,
  B10001
};

byte blank[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  pinMode(boutonG, INPUT_PULLUP);
  pinMode(boutonD, INPUT_PULLUP);
  pinMode(cible1,INPUT);
  pinMode(cible2,INPUT); 
  pinMode(cible3,INPUT);

  // Initialiser les LEDS et l'ecran
  pixels.begin();
  pixels.clear();
  pixels.show();
  lcd.init();
  lcd.backlight();
  
  // Définir une seed random grâce au port Analogique 0
  Serial.begin(9600);
  randomSeed(analogRead(0));

  SoundBuzzer();
  
  AfficherMain();

  delay(5000);
  lcd.clear();
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
  lcd.setCursor(4, 0);
  lcd.print("Vise le trou");

  /*if(digitalRead(cible1) == LOW) {
    cibleTours = random(1, 3);
    SoundBuzzer();
  } 
  else if (digitalRead(cible2) == LOW) {
    cibleTours = random(1, 3);
    SoundBuzzer();
  }
  else if (digitalRead(cible3) == LOW) {
    cibleTours = random(1, 3);
    SoundBuzzer();
  }*/
  statusBoutonG = digitalRead(boutonG);
  if (statusBoutonG==LOW) {
    cibleTours = random(0, 3);
  }


  if (timer > 10) {
    timer -= 1;
    lcd.setCursor(9, 2);
    lcd.print(timer);
    lcd.setCursor(11, 2);
    lcd.print("s");
  } 
  else if (timer <= 10 && timer > 0) {
    timer -= 1;
    lcd.createChar(0, blank);
    lcd.setCursor(9, 2);
    lcd.write(0);

    lcd.setCursor(10, 2);
    lcd.print(timer);
    lcd.setCursor(11, 2);
    lcd.print("s");

    tone(Buzzer, 1500, 500);
  }
  else if (timer <= 0) {
    noTone(Buzzer);
    EndGame();
  }

  pixels.clear();
  pixels.setPixelColor(cibleTours, pixels.Color(0, 0, 255));
  pixels.show();

  delay(1000);
}

void EndGame() {
  lcd.setCursor(5, 2);
  lcd.print("FIN DU JEU");
}

void Square(int posX, int posY) {
  lcd.createChar(0, underscore);
  lcd.setCursor(posX, posY);
  lcd.write(0);

  lcd.createChar(0, underscore);
  lcd.setCursor(posX + 1, posY);
  lcd.write(0);

  lcd.createChar(0, underscore);
  lcd.setCursor(posX, posY+1);
  lcd.write(0);

  lcd.createChar(0, underscore);
  lcd.setCursor(posX + 1, posY + 1);
  lcd.write(0);
}
