/*********
  Complete project details at https://randomnerdtutorials.com
*********/
char buffer[64];
char msg[10];
int counter = 0;
int i;
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(2, 3);
//Variable to save incoming SMS characters
char incoming_char = 0;
int index = 0;
int count = 10;
void setup()
{
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);
  // For serial monitor
  Serial.begin(9600);
  // Give time to your GSM shield log on to network
  //delay(10000);
  Serial.println("finish 10 sec delay");

  // AT command to set SIM900 to SMS mode
  //SIM900.print("AT+CMGF=1\r");
  delay(100);
  // Set module to send SMS data to serial out upon receipt
  //SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  long timer = millis();
  long currentTime = 0;
  Serial.print("current m iilis time : ");
  Serial.println(timer);

  SIM900.print("AT+CMGF=1\r");
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  Serial.println("entering the while loop");
  while (currentTime - 10000 < timer)
  {

    while (SIM900.available() > 0)

    {
      //Serial.println(currentTime);
      incoming_char = SIM900.read();
      //Serial.print(incoming_char);
      buffer[counter++] = incoming_char;
    }
    delay(1000);
    count--;
    Serial.print("more ");
    Serial.print(count);
    Serial.println(" sec to go");
    currentTime = millis();
    //Serial.println(currentTime);
  }
  Serial.println("exiting from loop");
  //Serial.println(buffer);
  int j = 0;
  for (i = 36; i < 40; i++)
  {
    //  Serial.print(buffer[i]);
    msg[j++] = buffer[i];
  }
  Serial.println("===========================");
  Serial.println(msg);
  Serial.println("===========================");
}
void loop()
{
}
