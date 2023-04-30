// 定数定義
const int joystickX = A0;    // X軸ジョイスティックのピン
const int joystickY = A1;    // Y軸ジョイスティックのピン
const int motorLeft1 = 3;    // 左モータードライバのピン1
const int motorLeft2 = 4;    // 左モータードライバのピン2
const int motorRight1 = 5;   // 右モータードライバのピン1
const int motorRight2 = 6;   // 右モータードライバのピン2

// 変数定義
int joystickValueX;          // X軸ジョイスティックの値
int joystickValueY;          // Y軸ジョイスティックの値

void setup() {
  // ピンのモード設定
  pinMode(motorLeft1, OUTPUT);
  pinMode(motorLeft2, OUTPUT);
  pinMode(motorRight1, OUTPUT);
  pinMode(motorRight2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // ジョイスティックの値を読み取る
  joystickValueX = analogRead(joystickX);
  joystickValueY = analogRead(joystickY);
  

  // 左右モータの速度を計算
  int motorLeftSpeed = map(joystickValueY, 0, 1023, 0, 255);//map(value,a,b,x,y) a~b→x~y
  int motorRightSpeed = map(joystickValueY, 0, 1023, 0, 255);


  // ジョイスティックのX軸の値に基づいて左右モーターの速度を調整
  if (joystickValueX < 450) {
    // 左旋回
    motorLeftSpeed -= map(joystickValueX, 0, 450, 0, 100);
    motorRightSpeed += map(joystickValueX, 0, 450, 0, 100);
  } else if (joystickValueX > 550) {
    // 右旋回
    motorLeftSpeed += map(joystickValueX, 550, 1023, 0, 100);
    motorRightSpeed -= map(joystickValueX, 550, 1023, 0, 100);
  }

  // モーターの動きを制御
  if (joystickValueY < 450) {
    // 前進
    digitalWrite(motorLeft1, HIGH);
    digitalWrite(motorLeft2, LOW);
    digitalWrite(motorRight1, HIGH);
    digitalWrite(motorRight2, LOW);
  } else if (joystickValueY > 550) {
    // 後進
    digitalWrite(motorLeft1, LOW);
    digitalWrite(motorLeft2, HIGH);
    digitalWrite(motorRight1, LOW);
    digitalWrite(motorRight2, HIGH);
  } else {
    // 停止
    digitalWrite(motorLeft1, LOW);
    digitalWrite(motorLeft2, LOW);
    digitalWrite(motorRight1, LOW);
    digitalWrite(motorRight2, LOW);
  }

  Serial.print(motorLeftSpeed);
  Serial.print(",");
  Serial.println(motorRightSpeed);
  Serial.print(joystickValueX);
  Serial.print(",");
  Serial.println(joystickValueY);
  Serial.println("----------");

  // モーターの速度を設定
  analogWrite(motorLeft1, motorLeftSpeed);
  analogWrite(motorLeft2, motorLeftSpeed);
  analogWrite(motorRight1, motorRightSpeed);
  analogWrite(motorRight2, motorRightSpeed);
}
