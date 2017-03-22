/* 
 * Authors: Nagavarun Kanaparthy
 * Resources:
 * Arduino Mega Reference
 * https://www.arduino.cc/en/Main/arduinoBoardMega
 * Sabertooth Motor Controller Library
 * https://www.dimensionengineering.com/software/SabertoothArduinoLibrary/html/class_sabertooth.html
 * Mechanum Drive Alogrithm Reference
 * http://thinktank.wpi.edu/resources/346/ControllingMecanumDrive.pdf
 */
#include <Sabertooth.h>
//Pin Definitions
#define prbOne A0
#define prbTwo A1
#define prbThree A2
#define prbFour A3
#define prbFive A4
#define transPin 53
//Motor Serial only Transmit(TX) no Receive(RX).
//Serial1 are pins 18(TX),19(RX) for Mega
#define MotorSerial Serial1

//MotorControllers
#define MotorAddress1 128
#define MotorAddress2 129
#define MotorRangeConst 127

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

//Constants
const float pi = 3.14;
//Variables
//Usb Communication
char incomingByte;
byte usbByteCount = 0;
String usbCommmand = "";
//Stage 1 
int prbOneVal = 0;
int prbTwoVal = 0;
int prbThreeVal = 0;
int prbFourVal = 0;
int prbFiveVal = 0;
//Motors
Sabertooth frontMotors = Sabertooth(MotorAddress1, MotorSerial);
Sabertooth rearMotors = Sabertooth(MotorAddress2, MotorSerial);
//Stage 1 Functions
int classifyComponent(int pin, int transistorPin) {
	digitalWrite(transistorPin, HIGH);
	delay(100);
	int value = analogRead(pin);
	int estVal = -1;
	if (value <= MaxResistVal && value >= MinResistVal)
		estVal = RESISTOR;
	else if (value <= MaxIndVal && value >= MinIndVal)
		estVal = INDUCTOR;
	else if (value <= MaxDioVal && value >= MinDioVal)
		estVal = DIODE;
	else if (value == WireVal)
		estVal = WIRE;
	else if (value > 900) {
		digitalWrite(transistorPin, LOW);
		delay(500);
		value = analogRead(pin);
		if (value <= 10)
			estVal = DIODE;
		else if (value > 800)
			estVal = CAPACITOR;
	}
	if (estVal == -1) {
		estVal = INDUCTOR;
	}
	digitalWrite(transistorPin, LOW);
	return estVal;
}
//Support Functions
void setMotion(String angleString, String magString, String speedString){
  //conversion
  float angle = angleString.toFloat();
  float mag = magString.toFloat();
  float rad = angle * 1000.00 / 57296.00;
  float moveSpeed = speedString.toFloat();
  //Taken from Mechanum Drive Alogrithm Reference
  float frontLeftMultipler = (mag * sin(rad + pi / 4) + moveSpeed) / 2;
  float frontRightMultipler = (mag * cos(rad + pi / 4) - moveSpeed) / 2;
  float rearLeftMultipler = (mag * cos(rad + pi / 4) + moveSpeed) / 2;
  float rearRightMultipler = (mag * sin(rad + pi / 4) - moveSpeed) / 2;
  //Sabertooth Related Conversion
  int fLMotorVal = frontLeftMultipler * MotorRangeConst;
  int fRMotorVal = frontRightMultipler * MotorRangeConst;
  int rLMotorVal = rearLeftMultipler * MotorRangeConst;
  int rRMotorVal = rearRightMultipler * MotorRangeConst;
  //Sabertooth Control
  frontMotors.motor(1,fLMotorVal);
  frontMotors.motor(2,fRMotorVal);
  rearMotors.motor(1,rLMotorVal);
  rearMotors.motor(2,rRMotorVal);
}
void handleUsbCommand(String value){
  switch(value.charAt(0)){
    //X = [0-9]
    /*
    * Motor Command 
    * M:angle,mag,speed
    * M:XX.XX,X.XXX,X.XXX
    */
    case 'M':
      setMotion(value.substring(2,7), value.substring(8,13), value.substring(14));
      Serial.println(value);
      break;
    /*
    * Decrypt PCB (stage 1)
    * D:
    * prints code D:XXXXX
    */
    /*
    * Output Combination (stage 3)
    * O:code
    * O:XXXXX
    * prints completed message O:done
    */
    /*
    * BumpMode (stage 2)
    * B:
    * prints completed message B:done
    */
    /*
    * Gun Fire Command (stage 4) 
    * G:angle
    * G:XXX
    * prints completed message G:done
    */
    default:
      Serial.println("I:"+value);
      break;
  }
}

//MicroController/Arduino Main
void setup() {
  Serial.begin(9600);
  MotorSerial.begin(9600);
  pinMode(transPin, OUTPUT);
}
void loop() {
  while (Serial.available() && usbByteCount < 20) {
    incomingByte = Serial.read();
    if(incomingByte == '\n'){
      handleUsbCommand(usbCommmand);
      usbByteCount = 0;
    }
    else{
      usbCommmand += incomingByte;
      usbByteCount++;
    }
  }
}