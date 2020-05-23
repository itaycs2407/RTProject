
#include <SoftwareSerial.h>

SoftwareSerial SIM900SSender(2, 3);
SoftwareSerial SIM900SReciver(2, 3);
const String PHONE = "972587600202";
bool gotMessage = false;
bool gotMsg = false;
char buffer[64];
char msg[64];
char incoming_char = 0;
int counter = 0;
int i;
String msggg = "", newString = "";
void setup()

{
    SIM900SSender.begin(19200);
    SIM900SReciver.begin(19200);
    Serial.begin(9600);

    // entering text mode for both the instances
    SIM900SSender.print("AT+CMGF=1\r");
    SIM900SReciver.print("AT+CMGF=1\r");
    delay(200);
    Serial.println("Sending Sms IN 2 SECONDS :");
    delay(2000);
    Serial.println("Sending now....");
    //sendMsg("write me somthing...");
    Serial.println("SMS was sent!");
    Serial.println("Entering to LOOP section.... ");

    while (true)
    {
        delay(500);
        counter = 0;
        int i;
        while (SIM900SReciver.available() > 0)
        {
            incoming_char = SIM900SReciver.read();
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

        if (checkForStateCode(msg))
        {

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
            }
            if (newString == "momo")
            {
                Serial.println("momo is in the house !");
                //sendMsg("what up momo ?? ");
            }
            if (newString == "go")
            {
                Serial.println("found go !");
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
    }
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
    char *ptr = msg;
    int i;
    for (i = 0; i < 62; i++)
    {
        if (msg[i] == '9' && msg[i + 1] == '7' && msg[i + 2] == '2')
            return true;
    }
    return false;
}
