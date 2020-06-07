#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#pragma region module setup

// LCD settings.
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// SIMCard Moudle
SoftwareSerial Sender(2, 3);
SoftwareSerial Reciver(2, 3);

// variable declaration + setup
// constants
const int timeToTakeChildrenOutFromTheCar = 3000; // 3 sec
const String PHONE = "972587600202";
const long waitingForAnswerTime = 10000;

// char
char buffer[64];
char msg[64];
char incoming_char = 0;

// strings
String newString = "";
String tempMsgToSMS = "";
String tempMsgToScreen = "";

// leds
int carShutDownLed = 13;
int childrenTrappedLed = 9;

// switch
const int carIsOnPin = 10;

// int
int counter = 0;
int i;
int actionNumber = 1;
int msgCounter = 1;
long currentMillis = 0;
long startOfWaitingTime = 0;

//bool
bool childrenTrapped = true;
bool answerFromParent = false;
bool noAnswer = true;
#pragma endregion

void setup()
{
  // set carIsOn pin
  pinMode(carIsOnPin, INPUT_PULLUP);
  pinMode(carShutDownLed, OUTPUT);
  pinMode(childrenTrappedLed, OUTPUT);
  digitalWrite(carShutDownLed, LOW);
  digitalWrite(childrenTrappedLed, LOW);

  //lcd setup
  lcd.begin(16, 2);

  SendMessageToScreenWithOutClear("Car is runnig     ", 0, 0);
  Serial.begin(9600);
  Serial.println("start of prog");

  //    check for the car shut down
  while (digitalRead(carIsOnPin) == HIGH)
  {
    delay(1);
  }
  CarShutdown();
  SIMCardInit();
  SendMessageToScreenWithOutClear("initializing...  ", 0, 0);
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
  Serial.println("waited time to take children out of the car");
  ChildrenDetedted();
  /*
    check for child in the seat :
    if no, continue to exit (end of setup section. answerFromParent = true;
    if yes, start the logic
  */
  while (childrenTrapped)
  {
    /*
      logic to decide what sms to send
    */
    tempMsgToSMS = "this is message number " + String(msgCounter);
    tempMsgToScreen = "message sent! #" + String(msgCounter) + "   ";
    Serial.println(tempMsgToScreen);
    msgCounter++;
    delay(2000);
    // SendTextMessage(tempMsgToSMS, PHONE);
    SendMessageToScreen(tempMsgToScreen, 0, 0);
    delay(1000);
    /*
      while loop tho wait for answer from parent
      if got answer = change answerFromParent state to true
      else continue
    */
    answerFromParent = false;
    noAnswer = true;

    while (!answerFromParent)
    {
      Serial.println("going to get sms");
      String newStr = getSMS();
      SendMessageToScreen(newStr, 0, 0);
      // check for answerFromParent state :
      delay(1000);
      SendMessageToScreen("starting new loop ", 0, 0);
      /*
      if false do the logic :
      action to do.
    */
      noAnswer = true;
      delay(1000);
    }
  }
}
//  SystemShutdown();

/**
  massage + leds to say that the children are out from the car
*/
#pragma region auxelery function
void CarShutdown()
{
  // send massage to screen - the shutdown
  Serial.println("the car shutdown    ");
  digitalWrite(carShutDownLed, HIGH);
  SendMessageToScreen("car shutdown !   ", 0, 0);
  delay(1500);
  SendMessageToScreenWithOutClear("initializing.   ", 1, 0);
  delay(1500);
  lcd.clear();
}
void SystemShutdown()
{
  SendMessageToScreen("System shutdown..", 0, 0);
  delay(2000);
  digitalWrite(childrenTrappedLed, LOW);
  delay(2000);
  SendMessageToScreen("Goodbye           ", 0, 0);
  delay(1500);
  digitalWrite(carShutDownLed, LOW);
}
void ChildrenDetedted()
{
  SendMessageToScreen("Children         ", 0, 0);
  SendMessageToScreenWithOutClear("detected         ", 1, 0);
  digitalWrite(childrenTrappedLed, HIGH);
  childrenTrapped = true;
}
void SIMCardInit()
{
  // sms moudle setup:
  Sender.begin(19200);  // the GPRS baud rate
  Reciver.begin(19200); // the GPRS baud rate
  delay(500);
  Serial.println("in init the sim card");
  Sender.print("AT+CMGF=1\r");
  Reciver.print("AT+CMGF=1\r");
  // MERGE : Sender.println("AT+IPR=19200");
  delay(500);
  SendMessageToScreen("initializing..    ", 0, 0);
  delay(300);
  lcd.clear();
  Sender.begin(19200); // the GPRS baud rate
  Reciver.begin(19200);
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
  Sender.print("AT+CMGF=1\r"); //Sending the SMS in text mode
  delay(100);
  Sender.println("AT + CMGS = \"" + phoneNumber + "\""); //The target phone number
  delay(100);
  Sender.println(msg); //the content of the message
  delay(100);
  Sender.println((char)26); //the ASCII code of the ctrl+z is 26
  delay(100);
  Sender.println();
}
String getSMS()
{
  Serial.println("***********entering to get sms message********");
  startOfWaitingTime = millis();
  currentMillis = millis();
  SendMessageToScreen("waiting for sms         ", 0, 0);
  delay(2000);
  while (noAnswer)
  {

    SendMessageToScreen("in loop", 0, 0);
    newString = "";
    delay(500);
    counter = 0;
    int i;
    while (Reciver.available() > 0)
    {
      incoming_char = Reciver.read();
      digitalWrite(childrenTrappedLed, LOW);
      buffer[counter++] = incoming_char;
    }

    //Serial.println(buffer);
    Serial.println(Reciver.read());
    for (i = 0; i < counter; i++)
    {
      msg[i] = buffer[i];
    }

    if (counter > 0)
    {
      while (counter >= 0)
      {
        buffer[counter--] = ' ';
      }
    }
    Serial.println("before the check if");
    //Serial.println(msg);
    if (checkForStateCode(msg))
    {
      Serial.println("after the if /-**/*-/*-/-*/*-/*-/-*/*-/*-/-*/-*");
      SendMessageToScreen("found 972", 0, 0);
      delay(500);
      msg[63] = '\0';

      newString = msg;
      newString.toLowerCase();
      newString = newString.substring(newString.indexOf("\n", 10));
      newString = newString.substring(1);
      newString.trim();
      Serial.print("Incoming message : " + newString + "\n");
      if (newString == "ok")
      {
        Serial.println("found OK !");
        SendMessageToScreen("found OK !", 0, 0);
        delay(100);
      }
      if (newString == "momo")
      {
        Serial.println("momo is in the house !");
        SendMessageToScreen("whats up momo ?!?", 0, 0);
        Serial.println("whats up momo ?!?");
        SendTextMessage("whats up momo ?!?", PHONE);
        delay(500);
      }
      if (newString == "go")
      {
        Serial.println("found go !");
      }
      while (Reciver.read() > 0)
      {
        delay(1);
      }
      for (i = 0; i < 64; i++)
      {
        msg[i] = '\0';
      }
    }

    currentMillis = millis();
    if ((currentMillis - waitingForAnswerTime) > startOfWaitingTime)
    {
      noAnswer = false;
    }
  }
  SendMessageToScreen("done waiting         ", 0, 0);
  delay(200);
  return newString;
}
bool checkForStateCode(char msg[])
{
  char *ptr = msg;
  int i;
  bool valToReturn = false;
  for (i = 0; i < 62; i++)
  {
    if (msg[i] == '9' && msg[i + 1] == '7' && msg[i + 2] == '2')
      valToReturn = true;
  }
  return valToReturn;
}
void loop()
{
}
#pragma endregion