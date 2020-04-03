unsigned long time;
int newTime = 0;
const int takeChildrenOutFromTheCar = 3000;
bool answerFromParent = false;

void setup() {
  Serial.begin(9600);
  time = millis();
  Serial.println(time);
  while (time <5000)
  { 
    time = millis();
  }
  Serial.println("end of time"); //prints time since program started
   /* 
    *  check for the car shut down
    *  
    */
   while (carIsOn){
    
   }
   time = millis();
   /*
    * give some time to take out the children out from the 
    * car after the car was shutdown. 
    * time to dely is takeChildrenOutFromTheCar
    */
   while ((newTime - takeChildrenOutFromTheCar) >  time) >){
        newTime = millis(); 
   }
   /*
    * check for child in the seat :
    * if no, continue to exit (end of setup section.
    * if yes, start the logic
    */
    
    while (!answerFromParent){
      /*
       * logic to decide what sms to send
       */
       // send the sms

       /*
        * while loop tho wait for answer from parent
        * if got answer = change answerFromParent state to true
        * else continue
        * 
        */

        // check for answerFromParent state :
        /*
         * if false do the logic : 
         * action to do.
         */
    }
  /**
   * massage + leds to say that the children are out from the car
   */
}
void loop() {
 
}
