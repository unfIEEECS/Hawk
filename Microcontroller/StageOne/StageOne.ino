#include <LiquidCrystal.h>
//Pin Definitions
#define lcdContrast 5
#define lcdRs 53
#define lcdEnable 52
#define lcdD4 51
#define lcdD5 50
#define lcdD6 49
#define lcdD7 58
#define prbOne A14
#define prbTwo A15
#define transPin 45
#define btnPin 44
//Value Definitions
#define WIRE 1
#define RESISTOR 2
#define CAPACITOR 3
#define INDUCTOR 4
#define DIODE 5
//Ranges 
#define WireVal 0
#define MinCapVal 900
#define MinResistVal 400
#define MaxResistVal 850
#define MinIndVal 40
#define MaxIndVal 70
#define MinDioVal 80
#define MaxDioVal 150
//global Variables
LiquidCrystal lcd(lcdRs, lcdEnable, lcdD4, lcdD5, lcdD6, lcdD7);
int buttonState = 0; 
int prbOneVal = 0;
int prbTwoVal = 0;
void setup() {
//pin setup
  pinMode(lcdContrast, OUTPUT);
  pinMode(transPin, OUTPUT);
  pinMode(btnPin, INPUT);
  Serial.begin(9600);
  analogWrite(lcdContrast,500);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  
}
void loop() {
   buttonState = digitalRead(btnPin);
   if (buttonState == HIGH){
    
    prbOneVal = classifyComponent(prbOne,transPin);
    prbTwoVal = classifyComponent(prbTwo,transPin);
    Serial.print(prbOneVal);
    Serial.println(prbTwoVal);
    
    buttonState = LOW;
   }
}

int classifyComponent(int pin, int transistorPin){
  digitalWrite(transistorPin,HIGH);
  delay(250);
  int value = analogRead(pin);
  int estVal = -1;
  if(value <= MaxIndVal && value >= MinIndVal)
    estVal = INDUCTOR;
  else if(value <= MaxResistVal && value >= MinResistVal)
    estVal = RESISTOR;
  else if(value <= MaxDioVal && value >= MinDioVal)
   estVal = DIODE;
  else if(value == WireVal)
    estVal = WIRE;
  Serial.println(value);
  if(value > 900){
    digitalWrite(transistorPin,LOW);
    int sum = 0;
    for(int i =0; i<10;i++){
      sum  += analogRead(pin);
      delay(50);
    }
    value = sum/10;
    Serial.print("Sum: ");
    Serial.println(value);
    if(value <= 3)
      estVal = DIODE;
    else
      estVal = CAPACITOR;
  }
  if(estVal == -1){
    estVal = INDUCTOR;
  }
  digitalWrite(transistorPin,LOW);
  return estVal;
}

