#define adc(A) analogRead(A)
#define BUZZ 2
int IN1 = 18, IN2 = 17, IN3 = 16, IN4 = 4;

void tick(int x = 1, int y = 90) {
    while (x--) {
        digitalWrite(BUZZ, 1);
        delay(y);
        digitalWrite(BUZZ, 0);
        delay(y);
    }
}
void dung() {
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
}
void tien(int p = 255) {
    analogWrite(IN1, p);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, p);
}
void lui(int p = 255) {
    analogWrite(IN1, 0);
    analogWrite(IN2, p);
    analogWrite(IN3, p);
    analogWrite(IN4, 0);
}
void trai(int p = 255) {
    analogWrite(IN1, 0);
    analogWrite(IN2, p);
    analogWrite(IN3, 0);
    analogWrite(IN4, p);
}
void phai(int p = 255) {
    analogWrite(IN1, p);
    analogWrite(IN2, 0);
    analogWrite(IN3, p);
    analogWrite(IN4, 0);
}
