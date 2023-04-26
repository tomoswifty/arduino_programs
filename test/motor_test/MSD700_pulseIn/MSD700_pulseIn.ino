// confirm
const int PIN = 21;
void setup() {
  pinMode(PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long  interval_HIGH = 0;
  unsigned long  interval_LOW = 0;
  unsigned long  interval = 0;
  unsigned long  rpm = 0;
  interval = 2 * pulseIn(PIN, LOW, 300000);
  //interval_HIGH = pulseIn(PIN,HIGH);
  //interval_LOW = pulseIn(PIN,LOW);
  //interval=interval_HIGH+interval_LOW;

  rpm = 4000000 / interval; //1パルス4秒

  if (rpm <= 5000) {
    Serial.print(rpm);
    Serial.print(",");
    Serial.print(00);
    Serial.print("\n");
  }
  //  Serial.println(rpm);
}
