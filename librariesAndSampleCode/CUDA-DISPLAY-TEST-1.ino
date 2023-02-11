/* Quick I2C Display test  */


/*  This sketch shows that 1602 LCD uses I2C communication. 
   This routine will display the characters 0123456789ABCDEF on row 0 then FEDCBA9876543210 
   on the next row.  Each character will be displayed one at a time with a programmable delay
   between each character.  At the end of each 32 character group, the delay will be reduced by
   25ms.  The delay will be printed to the screen before the start of the characters.
   The characters will also be sent out the Serial Monitor window
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD I2C address. Use 20 Space 4-line LCD. 
//LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address. Use 16 Space 2-line LCD. 

void setup() {
  lcd.init();
  lcd.backlight();                // Turn on the backlight. (lcd.noBacklight() turns off the backlight.) 
  lcd.write(12);
  Serial.begin(9600);             // Starts serial communication at 9600 speed 
  lcd.setCursor(0,0);
  Serial.println("Starting LCD");
  lcd.print("Starting LCD");
  delay(2000);
} 

void loop() {
int i, j, k;
  for(j=500; j>99; j=j-50){
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(j);
  lcd.print("mS");
  Serial.println(" ");
  Serial.print(j);
  Serial.println("mS");
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);

  for (i=0; i<16; i++) {
    lcd.print(i,HEX);
    Serial.print(i,HEX);
    delay(j);
  }
    lcd.setCursor(0,1);
    Serial.println(" ");
  for (i=15; i>-1; i--){
    lcd.print(i,HEX);
    Serial.print(i,HEX);
    delay(j);
  }
  }
}
