#include <LiquidCrystal.h>
String words;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LiquidCrystal lcd(13, 12, 5, 4, 3, 2);

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Initialising.");
    delay(1000);
    lcd.clear();
    lcd.print("Initialising..");
    delay(1000);
    lcd.clear();
    lcd.print("Initialising...");
    delay(1000);
    lcd.clear();
    lcd.print("Maker : Techduino");
    delay(2000);
}

void loop()
{

    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Initialising.");
    delay(1000);
    lcd.clear();
    lcd.print("Initialising..");
    delay(1000);
    lcd.clear();
    lcd.print("Initialising...");
    delay(1000);
    lcd.clear();
}
