/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 * Source: https://randomnerdtutorials.com/guide-for-microphone-sound-sensor-with-arduino/
*/

int ledPin = D2;
int micDigital = D4;
int micAnalog = A0;
boolean dval = 0;
int aval = 0;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(micDigital, INPUT);
  pinMode(micAnalog, INPUT);
  Serial.begin(9600);
}
  
void loop (){
  dval = digitalRead(micDigital);
  aval = analogRead(micAnalog);
  Serial.println(aval);
  // when the sensor detects a signal above the threshold value, LED flashes
  if(dval == HIGH){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  delay(100);
}
