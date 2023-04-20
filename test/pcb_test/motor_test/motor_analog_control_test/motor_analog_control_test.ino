// Arduinoに接続した可変抵抗をアナログ入力で読み取る
// ↓
// アナログ入力の値に応じてPWMのデューティー比を変える

const int analogPin = 2;

void setup()
{
    TCCR3B = (TCCR3B & 0b11111000) | 0x01;
    pinMode(analogPin, OUTPUT);
}

void loop()
{
    for (int i = 0; i < 210; i++)
    {
        analogWrite(analogPin, i);
        delay(100);
    }
    for (int i = 210; i > 0; i--)
    {
        analogWrite(analogPin, i);
        delay(100);
    }
}