#include <SoftwareSerial.h>
SoftwareSerial Sim900Serial(2, 3);

const String PHONE = "972587600202";

void setup()
{
    Serial.begin(9600); // the Hardware serial rate
                        //  Serial.println("Please type 's' to send  SMS");
}

void loop()
{
    String tempString = "972587600202";
    String stringOne = "Content-Type: text/html";
    Serial.println(tempString.indexOf("9725"));
    delay(1000);
    // substring(index) looks for the substring from the index position to the end:
    if (tempString.indexOf("9725") != -1)
    {
        Serial.println("all good, find cohen string");
    }
}
