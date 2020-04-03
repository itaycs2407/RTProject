#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
// LCD settings.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



int buttonState = 0; // variable for reading the pushbutton status
int toggle = 0;
int counter = 1;
char incoming_char=0;
const String PHONE = "972587600202";
const int buttonPin = 8; // the number of the pushbutton pin
const int ledPin = 13;
SoftwareSerial Sim900Serial(2, 3);

void setup()
{
    //lcd setup
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("Initialising.");
    delay(500);
    lcd.clear();
    
    
    
  // sms moudle setup:
  Sim900Serial.begin(115200); // the GPRS baud rate
  delay(500);
  Sim900Serial.println("AT+IPR=19200");
  delay(500);
  lcd.print("Initialising..");
  delay(300);
  lcd.clear();
  Sim900Serial.begin(19200); // the GPRS baud rate
  delay(1000);
  Serial.begin(9600); // the Hardware serial rate
 
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  lcd.print("Initialising...");
  delay(1000);
  lcd.clear();
  delay(1000);
  lcd.print("Standby Mode..");
  
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
      SendMessageToScreen("sms #" + String(counter));
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
  CheckForIncomingSMS();
}

void CheckForIncomingSMS(){
    Sim900Serial.print("AT+CNMI=2,2,0,0,0\r"); 
    delay(500);
    Serial.print(Sim900Serial.print("AT+CMGR=1\r"));    // AT Command to receive a live SMS 
    delay(500);
    Serial.println("changing to recevie mode...");
    if (Sim900Serial.available() >6 ){
      SendMessageToScreen(Sim900Serial.available());
     //  incoming_char=Sim900Serial.read();    
      // Serial.print(incoming_char);
    }    
}


void SendMessageToScreen(String msg)
{
  // clear the screen
  lcd.clear();
  // send the message to the screen
  lcd.println(msg);
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
