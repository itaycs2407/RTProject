#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
//__attribute__((optimize("O0")))
#pragma region module setup

// LCD settings.
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 7, d7 = 6;
LiquidCrystal lcd(12, 11, 5, 4, 7, 6);

// SIMCard Moudle
SoftwareSerial Sender(2, 3);
SoftwareSerial Reciver(2, 3);

// variable declaration + setup
// constants
#define timeToTakeChildrenOutFromTheCar 5000
#define PHONE "972587600202"
#define waitingForAnswerTime 10000

// char
char buffer[64];
char msg[64];
char incoming_char = 0;

// strings
String newString = "";
String tempMsgToSMS = "";

// int
uint8_t counter = 0;
uint8_t msgCounter = 1;
uint8_t actionToPreform = 1;
byte isMessageOK = 0;
byte answerToreturnIfGotSafteyKey = 0;
long currentMillis = 0;
long startOfWaitingTime = 0;

//bool
bool noAnswer = true;
bool childrenDetedtedWhileWaiting = false;
#pragma endregion

void setup()
{

  initilaizeLeds();

  //lcd setup
  lcd.begin(16, 2);

  SendMessageToScreenWithOutClear("Car is running    ", 0, 0);
  Serial.begin(9600);

  //    check for the car shut down
  while (digitalRead(10) == HIGH)
  {
    delay(1);
  }

  delay(3000);
  CarShutdown();
  SIMCardInit();
  SendMessageToScreenWithOutClear("initializing...  ", 0, 0);
  delay(1000);
  lcd.clear();
  delay(750);

  /*
    give some time to take out the children out from the
    car after the car was shutdown.
    time to delay is timeToTakeChildrenOutFromTheCar
    check for chiledren attached in the seat
  */

  SendMessageToScreen("Detecting seats  ", 0, 0);
  delay(2000);
  childrenDetedtedWhileWaiting = false;
  startOfWaitingTime = millis();
  currentMillis = startOfWaitingTime;
  while (!(childrenDetedtedWhileWaiting) && (currentMillis - timeToTakeChildrenOutFromTheCar) <= startOfWaitingTime)
  {
    if (digitalRead(0) == HIGH)
    {
      childrenDetedtedWhileWaiting = true;
    }
    currentMillis = millis();
  }

  /* 
    check for child in the seat :
    if no, continue to exit (end of setup section. answerFromParent = true;
    if yes, start the logic 
  */

  if (childrenDetedtedWhileWaiting)
  {
    ChildrenDetedted();
  }

  //childrenTrapped = childrenDetedtedWhileWaiting;
  startOfWaitingTime = 0;
  currentMillis = 0;

  while (childrenDetedtedWhileWaiting)
  {
    /* 
      logic to decide what sms to send.
      in the original it was array of 10 string, each represnting message in escalading order. 
      because of the Arduino's memory problem, working with IF stracture.
    */
    delay(1500);
    if (msgCounter <= 3)
    {
      tempMsgToSMS = "Check childeren out from the car!! message # " + String(msgCounter);
    }
    else if (msgCounter == 4)
    {
      tempMsgToSMS = "Sending message to backup number !! message # " + String(msgCounter);
    }
    else if (msgCounter == 5)
    {
      tempMsgToSMS = "Sending message to police HQ with GPS cord. !! message # " + String(msgCounter);
    }
    else if (msgCounter == 6)
    {
      tempMsgToSMS = "Calling 911 and health-care service !! message # " + String(msgCounter);
    }
    else
    {
      tempMsgToSMS = "Check childeren out from the car!! message # " + String(msgCounter);
    }
    SendMessageToScreen("message sent! #" + String(msgCounter) + "   ", 0, 0);
    msgCounter++;
    SendTextMessage(tempMsgToSMS, PHONE);
    delay(1000);

    /*
      while loop to wait for answer from parent
      if got answer = change answerFromParent to true
      else continue
    */

    noAnswer = true;
    startOfWaitingTime = millis();
    digitalWrite(8, HIGH);
    isMessageOK = getSMS();
    digitalWrite(8, LOW);
    /*
    validating the message. check if saftey key word accepted in the recived SMS message. 
    */
    SendMessageToScreen("messageOK: " + String(isMessageOK) + "  ", 0, 0);
    delay(1000);
    if (isMessageOK == 0)
    {
      /*
        two choises:
        1. didnt get any SMS
        2. got sms, but W/O the saftey word

        logic to decide what action to do.
        in the original it was array of 10 string, each represnting action to do in escalading order. 
        because using Arduino uno with not enough pins, only tell the user which action was done.
        */

      if (actionToPreform == 1)
      {
        //SendMessageToScreen("Action :        ", 0, 0);
        SendMessageToScreenWithOutClear("Open car Windows", 1, 0);
      }
      else if (actionToPreform == 2)
      {
        //SendMessageToScreen("Action :        ", 0, 0);
        SendMessageToScreenWithOutClear("Hazard lights On  ?", 01, 0);
      }
      else if (actionToPreform == 3)
      {
        //SendMessageToScreen("Action :        ", 0, 0);
        SendMessageToScreenWithOutClear("Start Horn      ", 1, 0);
      }
      else
      {
        SendMessageToScreen("Action : TBD.       ", 0, 0);
      }
      delay(1500);
      actionToPreform++;
    }
    else
    {
      childrenDetedtedWhileWaiting = false;
    }
    delay(1000);
  }

  SystemShutdown();
}

#pragma region auxelery function

void CarShutdown()
{
  // send massage to screen - the shutdown
  digitalWrite(13, HIGH);
  SendMessageToScreen("car shutdown !   ", 0, 0);
  delay(1500);
  //SendMessageToScreenWithOutClear("initializing.   ", 1, 0);
  delay(1500);
  lcd.clear();
}
void SystemShutdown()
{
  SendMessageToScreen("No children      ", 0, 0);
  delay(2000);
  digitalWrite(9, LOW);
  delay(2000);
  SendMessageToScreen("Starting shutdown", 0, 0);
  delay(2000);
  SendMessageToScreen("Goodbye           ", 0, 0);
  delay(1500);
  digitalWrite(13, LOW);
}
void ChildrenDetedted()
{
  SendMessageToScreen("Children detected", 0, 0);
  //SendMessageToScreenWithOutClear("detected         ", 1, 0);
  digitalWrite(9, HIGH);
}
void SIMCardInit()
{
  // sms moudle setup:
  Sender.begin(19200);  // the GPRS baud rate
  Reciver.begin(19200); // the GPRS baud rate
  delay(500);
  Sender.print("AT+CMGF=1\r");
  Reciver.print("AT+CMGF=1\r");
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
  lcd.print(msg);
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
uint8_t getSMS()
{

  delay(2000);
  while (noAnswer && !answerToreturnIfGotSafteyKey)
  {
    newString = "";
    delay(500);
    counter = 0;
    while (Reciver.available() > 0)
    {
      incoming_char = Reciver.read();
      digitalWrite(9, LOW);
      buffer[counter++] = incoming_char;
    }
    delay(1500);
    uint8_t i;
    for (i = 0; i < counter; i++)
    {
      msg[i] = buffer[i];
    }

    delay(1500);
    if (checkForStateCode(msg))
    {
      SendMessageToScreen("found 972", 0, 0);
      delay(500);
      msg[63] = '\0';
      newString = msg;
      newString.toLowerCase();
      newString = newString.substring(newString.indexOf("\n", 10));
      newString = newString.substring(1);
      newString.trim();
      if (newString == "ok")
      {
        answerToreturnIfGotSafteyKey = 1;
        SendMessageToScreen("found OK !", 0, 0);
        delay(750);
        return answerToreturnIfGotSafteyKey;
      }
      else if (newString == "momo")
      {
        answerToreturnIfGotSafteyKey = 1;
        SendMessageToScreen("whats up momo ?!?", 0, 0);
        delay(750);
        return answerToreturnIfGotSafteyKey;
      }
      delay(750);
      if (Reciver.read() > 0)
      {
        while (Reciver.read() > 0)
        {
          delay(1);
        }
      }
      for (i = 0; i < 64; i++)
      {
        msg[i] = '\0';
      }
    }

    delay(1500); /*
    if (counter > 0)
    {
      for (i = 64; i > 0; i--)
      {
        buffer[i - 1] = ' ';
      }
      counter = 0;
    }*/
    currentMillis = millis();
    SendMessageToScreen("sowt :" + String(startOfWaitingTime), 0, 0);
    SendMessageToScreenWithOutClear("cMillis :" + String(currentMillis), 1, 0);
    delay(1000);

    if (currentMillis * 2 + waitingForAnswerTime < startOfWaitingTime)
    {
      startOfWaitingTime = currentMillis + waitingForAnswerTime;
    }

    if ((currentMillis - waitingForAnswerTime) > startOfWaitingTime)
    {
      noAnswer = false;
    }
  }
  delay(1000);
  counter = 0;
  return answerToreturnIfGotSafteyKey;
}
bool checkForStateCode(char msg[])
{
  char *ptr = msg;
  uint8_t i;
  for (i = 0; i < 62; i++)
  {
    if (msg[i] == '9' && msg[i + 1] == '7' && msg[i + 2] == '2')
      return true;
  }
  return false;
}
void loop()
{
}
void initilaizeLeds()
{

  // leds
  //int carShutDownLed = 13;
  //int childrenTrappedLed = 9;
  //int waitingForSMSLed = 8;

  // switch
  //const int carIsOnPin = 10;
  //const int childrenDetedted = 0;
  pinMode(10, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
}