/*   
HC05 - Bluetooth AT-Command mode  
Serial Monitor is used to display state
LCD Display is used to display state
Commands from the BlueTooth Device to Control the ROV
0 = Relay 1 ON
1 = Relay 1 OFF
2 = Relay 2 ON
3 = Relay 2 OFF

*/ 
#include <SoftwareSerial.h>        // Need the software serial library
SoftwareSerial MyBlue(2, 3);       // Bluetooth RX | TX 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>     // Support for the display

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address. Use 20 Space 4-line LCD. 
//LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address. Use 16 Space 2-line LCD. 

int flag = 0;       // Received Data
#define pinRelay1 6    // Relay 1 pin
#define pinRelay2 7    // Relay 2 pin
#define pinENA 4     // bluetooth enable pin

void setup() 
{   
  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on the backlight. (lcd.noBacklight() turns off the backlight.) 
  lcd.write(12);
  lcd.setCursor(0,0);
  Serial.begin(9600);             // Starts serial communication at 9600 speed 
  MyBlue.begin(9600);             // Starts Bluetooth at 9600

 pinMode(pinRelay1, OUTPUT);      // relay 1 control
 pinMode(pinRelay2, OUTPUT);      // relay 2 control
 pinMode(pinENA, OUTPUT);         // Bluetooth enable pin 
 digitalWrite(pinRelay1, LOW);    // Relay OFF
 digitalWrite(pinRelay2, LOW);    // Relay OFF
 digitalWrite(pinENA, LOW);     // Enable LOW
 
 Serial.println("Ready to connect\nDefualt password is 1234 or 000"); 
  lcd.print("Testing Bluetooth");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print("CMDS= 0, 1, 2, 3");
} 
void loop() 
{ 
 if (MyBlue.available()) { 
   flag = MyBlue.read(); 
   MyBlue.print("Rcvd: ");   // Send message out to Bluetooth
   MyBlue.println(char(flag));     // Echo the received character back out to bluetooth
 }
 if (flag == '0') 
 { 
   digitalWrite(pinRelay1, HIGH);   // Turn Relay 1 ON
   Serial.println("Relay 1 ON"); 
   lcd.setCursor(0,0);
   lcd.print("Relay 1 ON  ");
   lcd.setCursor(0,3);       // move the cursor to 4th line  (adjust for 2 line LCD)
   lcd.print("Rcvd: ");      // Send a message to display
   lcd.print(char(flag));          // Send the received character
   lcd.print("  " );

 } 
 else if (flag == '1') 
 { 
   digitalWrite(pinRelay1, LOW);   // Turn Relay 1 OFF
   Serial.println("Relay 1 OFF"); 
   lcd.setCursor(0,0);
   lcd.print("Relay 1 OFF  ");
   lcd.setCursor(0,3);       // move the cursor to 4th line  (adjust for 2 line LCD)
   lcd.print("Rcvd: ");      // Send a message to display
   lcd.print(char(flag));          // Send the received character
   lcd.print("  " );

 } 
  else if (flag == '2') 
 { 
   digitalWrite(pinRelay2, HIGH);   // Turn Relay 2 ON
   Serial.println("Relay 2 ON"); 
   lcd.setCursor(0,1);
   lcd.print("Relay 2 ON  ");
   lcd.setCursor(0,3);       // move the cursor to 4th line  (adjust for 2 line LCD)
   lcd.print("Rcvd: ");      // Send a message to display
   lcd.print(char(flag));          // Send the received character
   lcd.print("  " );

 } 
  else if (flag == '3') 
 { 
   digitalWrite(pinRelay2, LOW);    // Turn Relay 2 OFF
   Serial.println("Relay 2 OFF"); 
   lcd.setCursor(0,1);
   lcd.print("Relay 2 OFF  ");
   lcd.setCursor(0,3);       // move the cursor to 4th line  (adjust for 2 line LCD)
   lcd.print("Rcvd: ");      // Send a message to display
   lcd.print(char(flag));          // Send the received character
   lcd.print("  " );

 } 
}  
