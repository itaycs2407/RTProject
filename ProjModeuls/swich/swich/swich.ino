int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  digitalWrite(7, HIGH);
}

void loop() {
  if (digitalRead(6) == LOW) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
  delay(10);
}
