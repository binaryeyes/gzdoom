int led = 13;
int led2 = 2;

int led1State = LOW;
int led2State = HIGH;
unsigned long led1StartMoment = 0;
unsigned long led2StartMoment = 0;
int led1Dur = 500;
int led2Dur = 1000;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  Serial.begin(9600); 

}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    int v = Serial.read();
    switch (v) {
      case '~':
        led1StartMoment = millis();
        break;
      case '!':
        led2StartMoment = millis();
        break;
    }
  }
  unsigned long nw = millis();
  if (nw - led1StartMoment < led1Dur)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  } else {
    digitalWrite(led, LOW);
  }
  
  if (nw - led2StartMoment < led2Dur) {
    digitalWrite(led2, HIGH);
  } else {
    digitalWrite(led2, LOW);
  }
}
