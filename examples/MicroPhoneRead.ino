
const int soundPin = A0;
const int threshold=38;

void setup() {

  pinMode(soundPin, INPUT);
  pinMode(D4, OUTPUT);

  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {

  int soundSens = analogRead(soundPin);

  if (soundSens >= threshold) {
    digitalWrite(D4,HIGH);

    Serial.printf("{{%d; %d; %s; %d; %s}}",soundSens, threshold, "YES", 20, "Yo yo");
    Serial.println();
  }

  else {
    digitalWrite(D4,LOW);
    Serial.printf("{{%d; %d; %s; %d; %s}}",soundSens, threshold, "NO", 20, "Yo yo");
    Serial.println();
  }

  delay(50);
}
