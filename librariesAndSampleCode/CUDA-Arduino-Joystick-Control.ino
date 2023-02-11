// Serial SaberTooth Motor Test
// authors: Scott Fraser & LeRoy van der Vegt
// created: July 2020
// Creative license use freely
//
// This tests 2 Sabertooth controllers and 4 motors.  Each motor will be tested forward and reverse.
// Test status displayed on a 4x20 LCD screen.  Pins for each Sabertooth controller have to be set
// in the following order.
//
//    Simplified Serial Mode 3 (MicroController controlled-Arduino)
//    pin 1 - ON     (Lithum battery - ON, other battery type - OFF)
//    pin 2 - OFF    (R/C radio control signal control - loss of signal stop motors; active-ON, not active-OFF)
//    pin 3 - ON     (Lithum battery voltage protection - protection-OFF, no protection-ON)
//    pin 4 - OFF    (set Baud rate of serial connection @9600 see below)
//    pin 5 - ON     (set Baud rate of serial connection @9600 see below)
//    pin 6 - OFF    (Serial Slave - controllers are independent-OFF controller are synchronized-ON)  
//
//    Different baud rate for pin 4 and 5
//    baud  pin  pin
//    rate   4    5
//    ----- ---  ---
//    2400  ON   ON
//    9600  OFF  ON   <------  9600 baud is used in this sketch
//    19200 ON   OFF
//    38400 OFF  OFF
//

// Arduino Serial communications 
#include <SoftwareSerial.h>    // used for Sabretooth & BlueTooth

// Sabertooth device driver
#include <SabertoothSimplified.h>

// include the DISPLAY library 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int SerialBaudRate = 9600;

// Initialize the LCD screen
int LCD_I2C_Addr = 0x27;   //I2C address for the LCD Screen (Default=0x27)
LiquidCrystal_I2C lcd(LCD_I2C_Addr, 20, 4); // Set the LCD I2C address. Use 20 Space 4-line LCD. 

// Initialize Arduino serial communications
SoftwareSerial SWSerial(NOT_A_PIN, 10); // RX on no pin (unused), TX on pin 10 (to S1).

SoftwareSerial BTSerial(2,3);    // For communication to with the Bluetooth Device
int BT_Ena = 4;

// Initialize Sabertooth driver passing it the Arduino serial communications object
SabertoothSimplified ST(SWSerial);      // Use SWSerial as the serial port.
int ST1_S2 = 8;                         // Arduino pin attached to Sabertooth controller
int ST2_S2 = 9;                         // Arduino pin attached to Sabertooth controller

#define VECTORED 0    // ROV frame that has angled vertical thrusters for up/down or side-to-side movement
#define ORTHO 1       // ROV frame with one vertical thruster and one crabbing thruster
#define ROV_Type VECTORED   // define the configuration of the ROV Vectored or Orthogonal


// Sends motor testing information to LCD screen
void motorMessage(int motorNum, int power){
  int lineNum=-1;
  int colNum=-1;
  
  switch (motorNum) {
    case 1:    // Motor number
      lineNum = 0;
      colNum = 3;
      break;
    case 2:    // Motor number
      lineNum = 0;
      colNum = 11;
      break;
    case 3:    // Motor number
      lineNum = 1;
      colNum = 3;
      break;
    case 4:    // Motor number
      lineNum = 1;
      colNum = 11;
      break;
  }
  lcd.setCursor(colNum,lineNum);
  lcd.print(power);
  lcd.print(" ");
}


// Initialize motor test parameters  
void setMotor(int motorNum, int power){
  
  int controllerNum = -1;    // Controller to be tested
  int motorNumber = -1;      // Motor on that the Controller to be tested
  
  switch (motorNum) {
    case 1:    // Motor to be updated
      controllerNum = ST2_S2;
      motorNumber = 1;
      break;
    case 2:    // Motor to be updated
      controllerNum = ST2_S2;
      motorNumber = 2;
      break;
    case 3:    // Motor to be updated
      controllerNum = ST1_S2;
      motorNumber = 1;
      break;
    case 4:    // Motor to be updated
      controllerNum = ST1_S2;
      motorNumber = 2;
      break;
  }

  // Set Motor power
  digitalWrite(controllerNum,HIGH);       
  motorMessage(motorNum, power);
  ST.motor(motorNumber, power);
  delayMicroseconds(50);
  digitalWrite(controllerNum,LOW);
}

void setup()
{
  // Set Arduino pin for each sabertooth as OUTPUT
  pinMode(ST1_S2,OUTPUT);            // Arduino pin control to sabertooth
  pinMode(ST2_S2,OUTPUT);            // Arduino pin control to sabertooth
  pinMode(BT_Ena,OUTPUT);            // Bluetooth ena pin
  digitalWrite(BT_Ena,LOW);          // Set the Bluetooth ena low
  // Start Serial Communications
  SWSerial.begin(SerialBaudRate);    // Start the Sabretooth channel
  BTSerial.begin(SerialBaudRate);    // Start the Bluetooth channel
  
  // Set up LCD 
  lcd.init();
  lcd.backlight();                
  lcd.write(12);
  lcd.setCursor(0,0);
  lcd.print("Connection at: "); 
  lcd.print(SerialBaudRate);
  lcd.setCursor(0,1);
  lcd.print("Test...");   

 
  lcd.setCursor(8,1);
  lcd.print("COMPLETE!");  
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("M1:     M2: ");  // 'Ms' - Motor Speed for motor 1 and 2
  lcd.setCursor(0,1);
  lcd.print("M3:     M4: ");  // 'Ms' - Motor Speed for motor 3 and 4

}

void loop()
{
    
    int Joy1_H, Joy1_V, Joy2_H, Joy2_V;
    int pwr1, pwr2, pwr3, pwr4;
    int MtrHR, MtrHL, MtrVR, MtrVL;

    Joy1_V = analogRead(A0);       // get the left vertical (Y) joystick input
    Joy1_H = analogRead(A1);       // get the left horizontal (X) joystick input
    Joy2_V = analogRead(A2);       // get the right vertical (Y) joystick input
    Joy2_H = analogRead(A3);       // get the right horizontal (X) joystick input

    pwr1 = map(Joy1_V,0,1023,-511,511);   //scale the left inputs for further math
    pwr2 = map(Joy1_H,0,1023,-511,511);   //scale the left inputs for further math
    pwr3 = map(Joy2_V,0,1023,-127,127);   //scale the right inputs for direct use
    pwr4 = map(Joy2_H,0,1023,-127,127);   //scale the right inputs for direct use

//
// Calculate the mixing for the Horizontal Thrusters
//  
  MtrHR = pwr1+pwr2;    //Val_Y1 + Val_X1;      // create the X & Y mixed values
  MtrHL = pwr1-pwr2;    //Val_Y1 - Val_X1;      // create the X & Y mixed values
  MtrHR /=4;                                    // scale back down to +-127
  MtrHL /=4;                                    // scale back down to +_127
  
  if (MtrHR > 127) {          // handle the large positive values and saturate at 127
     MtrHR = 127;
  }
  if (MtrHR < -127) {         // handle the large negative values and saturate at -127
     MtrHR = -127;
  }
  if (MtrHL > 127) {          // handle the large positive values and saturate at 127
     MtrHL = 127;
  }
  if (MtrHL < -127) {         // handle the large negative values and saturate at -127
     MtrHL = -127;
  }
//
// Assign the proper values for the Vertical Thrusters
//
  if (ROV_Type == VECTORED) {
      if (abs(pwr4) < 20) {    // check for no crabbing input
          MtrVR = pwr3;         // no crabbing then put both vertical the same
          MtrVL = pwr3;
      }
      else {
          MtrVR = pwr4;         // crabbing input then switch to X input from joystick
          MtrVL = -pwr4;        // left is opposite from right for crabbing.
      }
  }
   else {                        // for Orthogonal design
       MtrVR = pwr3;           // vertical thruster = Y input
       MtrVL = pwr4;           // crabbing thruster = X input
   }


//
// Set the power in each motor
//  
    setMotor(1, -MtrHL);  // not sure why -sign is needed here
    delay(1);
    setMotor(2, MtrHR);
    delay(1);
    setMotor(3, -MtrVL);  // not sure why -sign is needed here
    delay(1);
    setMotor(4, MtrVR);
    delay(1);
 
}
