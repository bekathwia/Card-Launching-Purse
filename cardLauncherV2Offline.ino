
void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(20, INPUT_PULLUP);
}

void loop() {
       int sensorVal = digitalRead(20);     
       Serial.println(sensorVal);   

         if (sensorVal == LOW) {
            launchCards();
  } 
  delay(10);
}

void launchCards(){
  for (int i = 0; i<7; i++){
  digitalWrite(8, HIGH); 
  delay(100);            
  digitalWrite(8, LOW);  
  delay(400);   
  }
}