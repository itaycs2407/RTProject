
#include <SoftwareSerial.h>

SoftwareSerial SIM900SSender(2, 3);
SoftwareSerial SIM900SReciver(2, 3);
const String PHONE = "972587600202";
bool gotMessage = false;
char buffer[64];
char msg[10];
char incoming_char = 0;
int counter = 0;
int i;
String msggg = "";
void setup()

{
    SIM900SSender.begin(19200);
    SIM900SReciver.begin(19200);
    Serial.begin(9600);

    SIM900SSender.print("AT+CMGF=1\r");
    SIM900SReciver.print("AT+CMGF=1\r");
    delay(100);

    /*SIM900SSender.print("AT+CNMI=2,2,0,0,0\r");
    /*SIM900SReciver.print("AT+CNMI=2,2,0,0,0\r"); */
    delay(100);

    Serial.println("Sending Sms IN 5 SECONDS :");
    delay(5000);
    Serial.println("Sending now....");

    //sendMsg();

    Serial.println("SMS was sent!");
    Serial.println("Entering to LOOP section.... ");
}
void loop()
{
    SIM900SReciver.flush();
    counter = 0;
    while (SIM900SReciver.available() > 0)
    {

        // Serial.print("this is the SIM900SReciver.available : ");
        //Serial.println(SIM900SReciver.available());

        //Serial.println(currentTime);
        incoming_char = SIM900SReciver.read();
        //Serial.print(incoming_char);
        buffer[counter++] = incoming_char;
    }

    /* int j = 0;
        for (i = 36; i < 40; i++)
        {
            //  Serial.print(buffer[i]);
            msg[j++] = buffer[i];
        } */
    msggg = buffer;
    if (msggg.indexOf("9725") != -1)
    {
        Serial.println("===========================");
        // Serial.println(msggg.substring(msggg.indexOf(",\"\",")));
        Serial.println(msggg.substring(msggg.indexOf("+12") + 6));
        Serial.println("===========================");
        Serial.println(msggg);
        msggg = "";
    }
    else
    {
        Serial.println("===========================");
        //SIM900SReciver.flush();
        Serial.println(msggg); /*
        Serial.println(msggg.indexOf("9725"));
        Serial.println("didnt find the string"); */
    }
    msggg = "";
}

void sendMsg()
{
    SIM900SSender.print("AT+CMGF=1\r"); //Sending the SMS in text mode
    delay(100);
    SIM900SSender.println("AT + CMGS = \"" + PHONE + "\""); //The target phone number
    delay(100);
    SIM900SSender.println("this is the message"); //the content of the message
    delay(100);
    SIM900SSender.println((char)26); //the ASCII code of the ctrl+z is 26
    delay(100);
    SIM900SSender.println();
    delay(2000);
}