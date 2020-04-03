
const byte buttonPin1 = 1;
const byte buttonPin2 = 2;

const byte ledPin1 =  13; 
const byte ledPin2 =  12; 

//byte buttonState1 = 0; 
byte buttonState2 = 0; 

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
 
}

void loop() {

byte buttonState1 = digitalRead(buttonPin1);
buttonState2 = digitalRead(buttonPin2);

if (buttonState1){
    digitalWrite(ledPin1, HIGH);
    Serial.println("Swich1 on");
    delay(1);
}
   else{
    digitalWrite(ledPin1, LOW);
    Serial.println("Swich1 off");
    delay(1);
    }
}
