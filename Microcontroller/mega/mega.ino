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
//Constants
const float pi = 3.14;
//Variables
//Usb Communication
char incomingByte;
bool goState = false;
byte usbByteCount = 0;
String usbCommand = "";

void handleUsbCommand(String value) {
	switch (value.charAt(0)) {
  //Stage Commands
  case 'P':
    //Do Stage One
    Serial.println("Stage One Done");
    Serial.println("Ready");
    break;
  case 'W':
    //Do Stage Two
    Serial.println("Stage Two Done");
    Serial.println("Ready");
    break;
  case 'F':
    //Do Stage Four
    Serial.println("Stage Four Done");
    Serial.println("Ready");
    break;
  //Support Commands
  case 'R':
    Serial.println("Ready");
    break;
  /* Motor Command
    * M:angle,mag,speed
    * M:XX.XX,X.XXX,X.XXX
    */
  case 'M':
    Serial.println(value);
    break;
  /* Turn Command
   * T:Angle 
   * T:XXXXXX
   */
  case 'T':
    Serial.println("Turned");
    break;
	default:
		Serial.println("I:" + value);
		break;
	}
}
void checkReady(String command){
  if(command.equals("Ready")){
    goState = true;
    //Change led to not blinking
    Serial.println("Ready");
    
  }
}
//MicroController/Arduino Main
void setup() {
	Serial.begin(9600);
}
void loop() {
	while (Serial.available() && usbByteCount < 20) {
		//read incoming byte
		incomingByte = Serial.read();
    //messages are delimited by the newline character
		if (incomingByte == '\n') {
      if(goState){
			  handleUsbCommand(usbCommand);
      }
      else{
        checkReady(usbCommand);
      }
      //Reset Variables
		  usbByteCount = 0;
		  usbCommand = "";
		}
		else {
      //Add Char to String
			usbCommand += incomingByte;
			usbByteCount++;
		}
	}
}
