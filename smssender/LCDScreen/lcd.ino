#include <LiquidCrystal.h>
String words;

LiquidCrystal lcd(13, 12, 5, 4, 3, 2);

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
    lcd.clear();
    Serial.print("Enter the line : ");
    while (Serial.available() == 0)
    {
    }
    words = Serial.readString();
    Serial.println(words);
    lcd.setCursor(0, 0);
    lcd.print(words);
    delay(5000);
    Serial.println(" ");
}