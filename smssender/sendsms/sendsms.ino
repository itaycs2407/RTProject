#include <SoftwareSerial.h>
SoftwareSerial Sim900Serial(2, 3);
void setup()
{
    Sim900Serial.begin(115200); // the GPRS baud rate
    delay(500);
    Sim900Serial.println("AT+IPR=19200");
    delay(500);
    Sim900Serial.begin(19200); // the GPRS baud rate
    delay(1000);
    Serial.begin(9600); // the Hardware serial rate
    Serial.println("Please type 's' to send  SMS");
}

void loop()
{
    if (Serial.available())
        switch (Serial.read())
        {
        case 's':
            makePhoneCall();
            break;
        }
    if (Sim900Serial.available())
        Serial.write(Sim900Serial.read());
}

void SendTextMessage()
{
    Sim900Serial.print("AT+CMGF=1\r"); //Sending the SMS in text mode
    delay(100);
    Sim900Serial.println("AT + CMGS = \"***********\""); //The target phone number
    delay(100);
    Sim900Serial.println("hello"); //the content of the message
    delay(100);
    Sim900Serial.println((char)26); //the ASCII code of the ctrl+z is 26
    delay(100);
    Sim900Serial.println();
}

void makePhoneCall()
{
    Sim900Serial.println("ATDP972587600202;"); //'*'instead the phone number you want to dial
    while (1)
        ;
}
