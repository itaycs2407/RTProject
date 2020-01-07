#include <SoftwareSerial.h>
int buttonState = 0; // variable for reading the pushbutton status
int toggle = 0;
int counter = 1;
const String PHONE = "972587600202";
const int buttonPin = 8; // the number of the pushbutton pin
const int ledPin = 13;
SoftwareSerial Sim900Serial(2, 3);

void setup()
{
  // sms moudle setup:
  Sim900Serial.begin(115200); // the GPRS baud rate
  delay(500);
  Sim900Serial.println("AT+IPR=19200");
  delay(500);
  Sim900Serial.begin(19200); // the GPRS baud rate
  delay(1000);
  Serial.begin(9600); // the Hardware serial rate
  //Serial.println("Please type 's' to send  SMS");

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop()
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH)
  {
    // check if the button is already pressed
    if (!toggle)
    {
      Serial.println("the button pressed, sending sms...." + String(counter));
      String tempMsg = "this is message number " + String(counter);
      SendMessageToScreen("message typed to screen");
      SendTextMessage(tempMsg, PHONE);
      counter++;
    }
    // sign that the button is pressed
    toggle = 1;
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    // the button relese
    toggle = 0;
    digitalWrite(ledPin, LOW);
  }
}
void SendTextMessage(String msg, String phoneNumber)
{
  Sim900Serial.print("AT+CMGF=1\r"); //Sending the SMS in text mode
  delay(100);
  Sim900Serial.println("AT + CMGS = \"" + phoneNumber + "\""); //The target phone number
  delay(100);
  Sim900Serial.println(msg); //the content of the message
  delay(100);
  Sim900Serial.println((char)26); //the ASCII code of the ctrl+z is 26
  delay(100);
  Sim900Serial.println();
}

void SendMessageToScreen(Strign msg)
{
  // clear the screen

  // send the message to the screen
  Serial.println(msg);
}