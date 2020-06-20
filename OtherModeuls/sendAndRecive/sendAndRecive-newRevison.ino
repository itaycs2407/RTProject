#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

SoftwareSerial SIM900SSender(2, 3);
SoftwareSerial SIM900SReciver(2, 3);

LiquidCrystal lcd(12, 11, 5, 4, 7, 6);
const String PHONE = "972587600202";
bool gotMessage = false;
bool gotMsg = false;
char buffer[64];
char msg[64];
char incoming_char = 0;
int counter = 0;
int i;
uint8_t msgCounter = 1;
uint8_t actionToPreform = 0;
int x = 10;
String msggg = "", newString = "";
String tempMsgToSMS = "";
bool childrenDetedtedWhileWaiting = false;

void setup()
{
    SIM900SSender.begin(19200);
    SIM900SReciver.begin(19200);
    //Serial.begin(9600);

    // entering text mode for both the instances
    SIM900SSender.print("AT+CMGF=1\r");
    SIM900SReciver.print("AT+CMGF=1\r");
    delay(200);
    pinMode(10, INPUT_PULLUP);
    pinMode(0, INPUT_PULLUP);
    pinMode(13, OUTPUT);
    //pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);
    digitalWrite(13, LOW);
    //digitalWrite(9, LOW);
    digitalWrite(8, LOW);
    lcd.begin(16, 2);

    SendMessageToScreen("Car is running    ", 0, 0);
    //Serial.begin(9600);

    //    check for the car shut down
    while (digitalRead(10) == HIGH)
    {
        delay(1);
    }
    digitalWrite(13, HIGH);
    SendMessageToScreen("car shutdown !   ", 0, 0);
    //Serial.println("car shutdown");
    delay(1000);
    SendMessageToScreen("Detecting seats ", 0, 0);
    //Serial.println("Detecting seats");
    for (i = 10; i > 0; i--)
    {
        SendMessageToScreenWithOutClear("      " + String(i) + "      ", 1, 0);
        delay(1000);
    }
    if (digitalRead(0) == HIGH)
    {
        childrenDetedtedWhileWaiting = true;
    }
    if (childrenDetedtedWhileWaiting)
    {
        SendMessageToScreen("Children detected", 0, 0);
        //  Serial.println("Children detected");
    }

    //Serial.println("Sending Sms IN 2 SECONDS :");
    // delay(2000);
    /// Serial.println("Init sms - tell the user that the system in online...");
    sendMsg("Init sms - tell the user that the system in online...");
    // Serial.println("SMS was sent!");
    // Serial.println("Entering to LOOP section.... ");

    while (childrenDetedtedWhileWaiting && x > 9)
    {
        //SIM900SReciver.println("AT+CMGL=\"ALL\",0 ");
        //SIM900SSender.println("AT+CMGL=\"ALL\",0 ");
        //SIM900SReciver.print("AT+CMGDA=\"");
        //SIM900SReciver.println("DEL ALL\"");

        //SIM900SSender.print("AT+CMGDA=\"");
        //SIM900SSender.println("DEL ALL\"");

        digitalWrite(8, HIGH);

        delay(500);
        counter = 0;
        int i;
        // Serial.println("waiting for response...\n");
        /* SendMessageToScreen("waiting for sms..", 0, 0);
        for (i = 5; i > 0; i--)
        {
            SendMessageToScreenWithOutClear("      " + String(i) + "      ", 1, 0);
            delay(1000);
        }*/
        while (SIM900SReciver.available() > 0)
        {
            incoming_char = SIM900SReciver.read();
            //Serial.print(incoming_char);
            buffer[counter++] = incoming_char;
        }

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
        //Serial.println("before if 972");
        // Serial.println(msg);
        if (checkForStateCode(msg))
        {
            //Serial.print("got some SMS");
            msg[63] = '\0';

            newString = msg;
            newString.toLowerCase();
            newString = newString.substring(newString.indexOf("\n", 10));
            newString = newString.substring(1);
            newString.trim();
            //Serial.print("Incoming message : " + newString + "\n");
            if (newString == "ok")
            {
                //Serial.println("found OK !");
            }
            if (newString == "momo")
            {
                //Serial.println("got the safe word. shuting down");
                x = 0;
                sendMsg("No one in the car, starting shutdown.");
            }
            if (newString == "go")
            {
                //Serial.println("found go !");
            }
            while (SIM900SReciver.read() > 0)
            {
                delay(1);
            }
            for (i = 0; i < 64; i++)
            {
                msg[i] = '\0';
            }
            newString = "";
        }
        digitalWrite(8, LOW);
        SendMessageToScreen(newString, 0, 0);
        delay(1000);

        if (x > 9)
        {
            if (actionToPreform == 1)
            {
                tempMsgToSMS = "Open car Windows";
            }
            else if (actionToPreform == 2)
            {
                tempMsgToSMS = "Hazard lights On";
            }
            else if (actionToPreform == 3)
            {
                tempMsgToSMS = "Start Horn      ";
            }
            else if (actionToPreform > 3)
            {
                tempMsgToSMS = "Action : TBD.       ";
            }
            if (actionToPreform > 0)
            {
                //Serial.println(tempMsgToSMS + "\n");
                SendMessageToScreen("Action :         ", 0, 0);
                SendMessageToScreenWithOutClear(tempMsgToSMS + "  ", 1, 0);
                delay(1500);
            }
            actionToPreform++;

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
            //Serial.println(tempMsgToSMS + "\n");
            SendMessageToScreen("Sending message  ", 0, 0);
            SendMessageToScreenWithOutClear("    #" + String(msgCounter) + "    ", 1, 0);
            //sendMsg(tempMsgToSMS);
            msgCounter++;
            delay(1500);
        }
    }
    // Serial.println("going to shutdown...");
    SystemShutdown();
}
void loop()
{
}
void sendMsg(String msgToSend)
{
    SIM900SSender.print("AT+CMGF=1\r"); //Sending the SMS in text mode
    delay(100);
    SIM900SSender.println("AT + CMGS = \"" + PHONE + "\""); //The target phone number
    delay(100);
    SIM900SSender.println(msgToSend); //the content of the message
    delay(100);
    SIM900SSender.println((char)26); //the ASCII code of the ctrl+z is 26
    delay(100);
    SIM900SSender.println();
    delay(2000);
}

bool checkForStateCode(char msg[])
{
    //Serial.println("in the 972 check");
    char *ptr = msg;
    int i;
    bool result = false;
    for (i = 0; i < 62; i++)
    {
        // Serial.print(msg[i]);
        // Serial.print(msg[i + 1]);
        //  Serial.println(msg[i + 2]);

        if (msg[i] == '9' && msg[i + 1] == '7' && msg[i + 2] == '2')
            result = true;
    }
    return result;
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
void SystemShutdown()
{
    SendMessageToScreen("No one is in     ", 0, 0);
    SendMessageToScreenWithOutClear("the car.        ", 1, 0);
    delay(1500);
    SendMessageToScreen("Starting shutdown", 0, 0);
    delay(1500);
    SendMessageToScreen("Goodbye           ", 0, 0);
    delay(1500);
    digitalWrite(13, LOW);
}