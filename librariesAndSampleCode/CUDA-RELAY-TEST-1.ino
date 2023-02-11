/* Quick Relay test  */


/*  This routine will toggle Relay 1 ON for 2 seconds then turn OFF followed by 
 *   turning Relay 2 ON for 2 seconds then OFF
 *   It will repeat until stopped.
 *   The LCD display and the Serial Monitor will reflect the state of each relay
 *   modify the definition DelayTime below to change the toggle time.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define pinRelay1 6
#define pinRelay2 7
#define DelayTime 2000


LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address. Use 20 Space 4-line LCD. 
//LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address. Use 16 Space 2-line LCD. 

void setup() {
  lcd.init();
  lcd.backlight();                // Turn on the backlight. (lcd.noBacklight() turns off the backlight.) 
  lcd.write(12);
  Serial.begin(9600);             // Starts serial communication at 9600 speed 
  lcd.setCursor(0,0);
  Serial.println("Testing Relays");
  lcd.print("Testing Relays");
  delay(DelayTime);
  lcd.clear();
  pinMode(pinRelay1,OUTPUT);
  pinMode(pinRelay2,OUTPUT);
  digitalWrite(pinRelay1,LOW);
  digitalWrite(pinRelay2,LOW);
} 

void loop() {
  // Turn on Relay 1
  lcd.setCursor(0,0);
  lcd.print("Relay 1 ON ");
  Serial.println("Relay 1 ON ");
  digitalWrite(pinRelay1,HIGH);
  // delay 1 second
  delay(DelayTime);
  //turn off Relay 1
  digitalWrite(pinRelay1,LOW);
  // send the info messages
  lcd.setCursor(0,0);
  lcd.print("Relay 1 OFF");
  lcd.setCursor(0,1);
  lcd.print("Relay 2 ON ");
  Serial.println("Relay 1 OFF ");
  Serial.println("Relay 2 ON ");
  // Turn Relay 2 ON
  digitalWrite(pinRelay2,HIGH);
  // Delay 1 second
  delay(DelayTime);
  // turn relay 2 off
  digitalWrite(pinRelay2,LOW);
  lcd.setCursor(0,1);
  lcd.print("Relay 2 OFF");
  Serial.println("Relay 2 OFF");
}
