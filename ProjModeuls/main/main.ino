#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
// Moudules setup

// LCD settings.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// SIMCard Moudle
SoftwareSerial Sim900Serial(2, 3);

// variable declaratio + setup

const int timeToTakeChildrenOutFromTheCar = 5000; // 3 sec
const String PHONE = "972587600202";
const int carIsOnPin = 10;
int ledPin = 13;
int actionNumber = 1;
int msgCounter = 1;
String tempMsgToSMS = "";
String tempMsgToScreen = "";
unsigned long time;

bool answerFromParent = false;



void setup() {
  // set carIsOn pin
  pinMode(carIsOnPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //lcd setup
  lcd.begin(16, 2);
  SendMessageToScreenWithOutClear("Car in runnig     ", 0, 0);


  Serial.begin(9600);
  Serial.println("start of prog");

  //    check for the car shut down

  while (digitalRead(carIsOnPin) == HIGH) {
    delay(1);
  }
  CarShutdown();
  SIMCardInit();
  SendMessageToScreenWithOutClear("Initialising...  ", 0, 0);

  delay(1000);
  lcd.clear();
  delay(750);
  SendMessageToScreen("Detecting seats  ", 0, 0);
  delay(timeToTakeChildrenOutFromTheCar);
  /*
    give some time to take out the children out from the
    car after the car was shutdown.
    time to dely is timeToTakeChildrenOutFromTheCar
  */

  //Serial.println("waited time to take children out of the car");
  SendMessageToScreen            ("Children         ", 0, 0);
  SendMessageToScreenWithOutClear("detected         ", 1, 0);
  answerFromParent = false;

  /*
    check for child in the seat :
    if no, continue to exit (end of setup section. answerFromParent = true;
    if yes, start the logic
  */

  while (!answerFromParent) {
    /*
      logic to decide what sms to send
    */
    tempMsgToSMS = "this is message number " + String(msgCounter);
    tempMsgToScreen = "message sent!" + String(msgCounter) + "   " ;
    Serial.println(tempMsgToScreen);
    msgCounter++;
    delay(2000);
    //SendTextMessage(tempMsgToSMS, PHONE);
    SendMessageToScreen(tempMsgToScreen, 0, 0);
    

    /*
      while loop tho wait for answer from parent
      if got answer = change answerFromParent state to true
      else continue

    */

    // check for answerFromParent state :
    /*
      if false do the logic :
      action to do.
    */
    delay(5000);
  }
}
  /**
    massage + leds to say that the children are out from the car
  */
  void CarShutdown() {
    // send massage to screen - the shutdown
    Serial.println("the car shutdown    ");
    digitalWrite(ledPin, HIGH);

    SendMessageToScreen("car shutdown !   ", 0, 0);
    delay(1500);

    SendMessageToScreenWithOutClear("Initialising.   ", 1, 0);

    delay(1500);
    lcd.clear();

  }

  void SIMCardInit() {

    // sms moudle setup:
    Sim900Serial.begin(115200); // the GPRS baud rate
    delay(500);
    Sim900Serial.println("AT+IPR=19200");
    delay(500);

    SendMessageToScreen("Initialising..    ", 0, 0);
    delay(300);
    lcd.clear();
    Sim900Serial.begin(19200); // the GPRS baud rate
    delay(1000);
  }

  void SendMessageToScreen(String msg, int row, int col)
  {
    // clear the screen
    lcd.clear();
    //set cursor
    lcd.setCursor(col, row);
    // send the message to the screen
    lcd.println(msg);
  }

  void SendMessageToScreenWithOutClear(String msg, int row, int col)
  {
    //set cursor
    lcd.setCursor(col, row);
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

  void loop() {

  }
