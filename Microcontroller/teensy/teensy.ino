/* 
 * Authors: Nagavarun Kanaparthy
 * Resources:
 * Sabertooth Motor Controller Library
 * https://www.dimensionengineering.com/software/SabertoothArduinoLibrary/html/class_sabertooth.html
 * Mechanum Drive Alogrithm Reference
 * http://thinktank.wpi.edu/resources/346/ControllingMecanumDrive.pdf
 */
#include <Sabertooth.h>
//Pin Definitions

//Motor Serial only Transmit(TX) no Receive(RX).
#define MotorSerial Serial1
//MotorControllers
#define MotorAddress1 128
#define MotorAddress2 129
#define MotorRangeConst 127
//Constants
const float pi = 3.14;
//Variables
char incomingByte;
byte usbByteCount = 0;
String usbCommmand = "";
//Motors
Sabertooth frontMotors = Sabertooth(MotorAddress1, MotorSerial);
Sabertooth rearMotors = Sabertooth(MotorAddress2, MotorSerial);

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
}
void loop() {
  while (Serial.available() && usbByteCount < 20) {
    incomingByte = Serial.read();
    if(incomingByte == '|'){
      handleUsbCommand(usbCommmand);
      usbByteCount = 0;
    }
    else{
      usbCommmand += incomingByte;
      usbByteCount++;
    }
  }
}