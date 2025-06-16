#define  ANALOG_PIN A7

int A_read, M_read = 0;

void setup() {
  pinMode(ANALOG_PIN, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial connection established!");

}

void loop() {
  A_read = analogRead(ANALOG_PIN);
  M_read = ((A_read) / 100);
  Serial.print(" ( ");
  Serial.print(A_read);
  Serial.print(" -> ");
  Serial.print(M_read);
  Serial.println(" ) ");
}
