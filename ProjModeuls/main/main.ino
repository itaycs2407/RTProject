unsigned long time;
const int carIsOnPin = 10;
int ledPin = 13;
const int takeChildrenOutFromTheCar = 10000; // 3 sec
int newMillisRead = 0;
bool answerFromParent = false;
void setup() {
  // set carIsOn pin
  pinMode(carIsOnPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  Serial.println("start of prog");

  //    check for the car shut down
  time = millis();
  Serial.println(time);
  while (digitalRead(carIsOnPin) == HIGH) {
    delay(1);
  }
  CarShutdown();
  Serial.println("return from car shut down");
  
  /*
     give some time to take out the children out from the
     car after the car was shutdown.
     time to dely is takeChildrenOutFromTheCar
  */

  delay(takeChildrenOutFromTheCar);
  Serial.println("waited time to take children out of the car");

  /*
     check for child in the seat :
     if no, continue to exit (end of setup section. answerFromParent = true;
     if yes, start the logic
  */

  //while (!answerFromParent) {
  /*
     logic to decide what sms to send
  */
  // send the sms

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
}
/**
   massage + leds to say that the children are out from the car
*/
void CarShutdown() {
  // send massage to screen - the shutdown
  Serial.println("the car shutdown");
  digitalWrite(ledPin, HIGH);

}
void loop() {

}
