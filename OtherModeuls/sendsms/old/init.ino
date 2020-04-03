
bool carRunning = true;
bool weight = false;

void setup()
{
    Serial.begin(9600);
    GSMInit();
}

void loop()
{
    if (checkWeight())
        printToSerial("massage");
}

void printToSerial(String str)
{
    Serial.println(str);
}
// return the weight status
bool checkWeight()
{
    return weight;
}
/* send sms with the string */
void sendSMS(String massgeToSend)
{
}

/* Init the gsm sheild */
void GSMInit()
{
}