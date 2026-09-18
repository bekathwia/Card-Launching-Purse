#define AIN1   8
#define AIN2   7
#define SLP    9



void setup() {
  //start serial connection
  Serial.begin(9600);
  //pinMode(8, OUTPUT);
  pinMode(10, INPUT_PULLUP);

    // configure pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(SLP, OUTPUT);

  // enable DRV8833
  digitalWrite(SLP, HIGH);

}

void loop() {
       int sensorVal = digitalRead(10);     
       Serial.println(sensorVal);   

         if (sensorVal == LOW) {
            launchCards();
  } else{
    //stop
    digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  }
  delay(10);
}

void launchCards(){
  Serial.println("Forward");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  //delay(1000);
}