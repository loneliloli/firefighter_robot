#include "config.h"
#include <SoftwareSerial.h>
SoftwareSerial sw(5,15);

#include "BluetoothSerial.h"
BluetoothSerial bluetooth;
#include <Servo.h>
Servo sv1, sv2, sv3;

char ch = 0;
int goc2=90, goc3=90;
int cmode=0;
int sen[11] = {13,14,27,26,25,33,32,35,34,39,36};
unsigned int a[11],maxa[11],mina[11],thresh[11];
int Err=0, pre_Err=0;

void setup() {
  Serial.begin(9600);
  sw.begin(9600);
  pinMode(BUZZ,OUTPUT); tick();
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  dung();
  
  sv1.attach(19); sv1.write(60);
  sv2.attach(21); sv2.write(goc2);
  sv3.attach(22); sv3.write(goc3);
  bluetooth.begin("XeCuuHoa");
//  while(1)
//  {
//    if (sw.available())
//    Serial.write(sw.read());
//    delay(1);
//  }
  tick(2);
  memset(maxa,0,11);
  memset(mina,4095,11);
  unsigned long timeout = millis();
  while(millis()-timeout<8000)
  {
    for(int i=0;i<11;i++){
      unsigned int sum=0;
      int re = adc(sen[i]);
      delayMicroseconds(50);
      if (re>maxa[i]) maxa[i]=re;
      if (re<mina[i]) mina[i]=re;
    }
    delay(1);
  }
  for(int i=0;i<11;i++) {Serial.print(mina[i]); Serial.print(",");} Serial.println(" ");
  for(int i=0;i<11;i++) {Serial.print(maxa[i]); Serial.print(",");} Serial.println(" ");
  for(int i=0;i<11;i++) thresh[i] = (maxa[i]+mina[i])/2;
  for(int i=0;i<11;i++) {Serial.print(thresh[i]); Serial.print(",");} Serial.println(" ");
  tick(3);
//  while(1)
//  {
//    Serial.println(line(),BIN);
//    delay(100);
//  }
}

unsigned int line()
{
  unsigned int temp=0;
  for(int i=0;i<11;i++)
  {
    if (adc(sen[i])>thresh[i]) temp|=(0x01<<i);
    if (adc(sen[i])<=thresh[i]) temp&=~(0x01<<i);
    delayMicroseconds(50);
  }
  return temp;
}
void findErr()
{
  if (line()==0b0000000000000000) {Err=10; return;}
  //else if (line()==0b0111111111111111) {Err=-10; return;} 
  
//  else if (line()==0b0000000000100000) {Err=0; return;}
  else if (line()==0b0000000000110000) {Err=0; return;}
  else if (line()==0b0000000001100000) {Err=0; return;}
  else if (line()==0b0000000001110000) {Err=0; return;}
  else if (line()==0b0000000001111000) {Err=0; return;}
  else if (line()==0b0000000011110000) {Err=0; return;}
  
//  else if (line()==0b0000000000010000) {Err=1; return;}
  else if (line()==0b0000000000011000) {Err=1; return;}
  else if (line()==0b0000000000111000) {Err=1; return;}
  
//  else if (line()==0b0000000000001000) {Err=2; return;}
  else if (line()==0b0000000000001100) {Err=2; return;}
  else if (line()==0b0000000000011100) {Err=2; return;}
  
//  else if (line()==0b0000000000000100) {Err=3; return;}
  else if (line()==0b0000000000000110) {Err=3; return;}
  else if (line()==0b0000000000001110) {Err=3; return;}
  
//  else if (line()==0b0000000000000010) {Err=4; return;}
  else if (line()==0b0000000000000111) {Err=4; return;}
 
  else if (line()==0b0000000000000011) {Err=5; return;}
//  else if (line()==0b0000000000000001) {Err=6; return;}
  
//  else if (line()==0b0000000001000000) {Err=-1; return;}
  else if (line()==0b0000000011000000) {Err=-1; return;}
  else if (line()==0b0000000011100000) {Err=-1; return;}
  
//  else if (line()==0b0000000010000000) {Err=-2; return;}
  else if (line()==0b0000000110000000) {Err=-2; return;}
  else if (line()==0b0000000111000000) {Err=-2; return;}
  
//  else if (line()==0b0000000100000000) {Err=-3; return;}
  else if (line()==0b0000001100000000) {Err=-3; return;}
  else if (line()==0b0000001110000000) {Err=-3; return;}
  
//  else if (line()==0b0000001000000000) {Err=-4; return;}
  else if (line()==0b0000011100000000) {Err=-4; return;}
    
  else if (line()==0b0000011000000000) {Err=-5; return;}
//  else if (line()==0b0000010000000000) {Err=-6; return;}
  else 
  {
    if (adc(sen[2])>thresh[2] && adc(sen[3])>thresh[3] && adc(sen[4])>thresh[4] && adc(sen[8])<thresh[8]) {Err=15; return;}
    else if (adc(sen[1])>thresh[1] && adc(sen[2])>thresh[2] && adc(sen[3])>thresh[3] && adc(sen[8])<thresh[8]) {Err=15; return;}
    else if (adc(sen[0])>thresh[0] && adc(sen[1])>thresh[1] && adc(sen[2])>thresh[2] && adc(sen[8])<thresh[8]) {Err=15; return;}
    else if (adc(sen[6])>thresh[6] && adc(sen[7])>thresh[7] && adc(sen[8])>thresh[8] && adc(sen[2])<thresh[2]) {Err=-15; return;}
    else if (adc(sen[7])>thresh[7] && adc(sen[8])>thresh[8] && adc(sen[9])>thresh[9] && adc(sen[2])<thresh[2]) {Err=-15; return;}
    else if (adc(sen[8])>thresh[8] && adc(sen[9])>thresh[9] && adc(sen[10])>thresh[10] && adc(sen[2])<thresh[2]) {Err=-15; return;}
    else if (adc(sen[3])>thresh[3] && adc(sen[4])>thresh[4] && adc(sen[5])>thresh[5] && adc(sen[6])>thresh[6] && adc(sen[7])>thresh[7]) {Err=16; return;}
  }
}
void control_line(int ln)
{
  switch(ln)
  {
    case 10: {analogWrite(IN1,0); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,0); break;}
    
    case -15: {analogWrite(IN1,0); analogWrite(IN2,130); analogWrite(IN3,0); analogWrite(IN4,210); break;}
    case -6: {analogWrite(IN1,0); analogWrite(IN2,100); analogWrite(IN3,0); analogWrite(IN4,190); break;}
    case -5: {analogWrite(IN1,0); analogWrite(IN2,70); analogWrite(IN3,0); analogWrite(IN4,210); break;}
    case -4: {analogWrite(IN1,80); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,200); break;}
    case -3: {analogWrite(IN1,100); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,180); break;}
    case -2: {analogWrite(IN1,115); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,170); break;}
    case -1: {analogWrite(IN1,130); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,160); break;}
    case 0: {analogWrite(IN1,150); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,150); break;}
    case 1: {analogWrite(IN1,160); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,130); break;}
    case 2: {analogWrite(IN1,170); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,115); break;}
    case 3: {analogWrite(IN1,180); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,100); break;}
    case 4: {analogWrite(IN1,190); analogWrite(IN2,0); analogWrite(IN3,0); analogWrite(IN4,80); break;}
    case 5: {analogWrite(IN1,210); analogWrite(IN2,0); analogWrite(IN3,70); analogWrite(IN4,0); break;}
    case 6: {analogWrite(IN1,190); analogWrite(IN2,0); analogWrite(IN3,100); analogWrite(IN4,0); break;}
    case 15: {analogWrite(IN1,210); analogWrite(IN2,0); analogWrite(IN3,130); analogWrite(IN4,0); break;}
    default: break;
  }
}

void timDUNG()
{
  while(1)
  {
    Serial.println(line(),BIN);
    findErr();
    control_line(Err);
    if (Err==10) 
    {
      dung();
      return;
    }
  }
}
void timNGA4(int x)
{
  while(1)
  {
    Serial.println(line(),BIN);
    findErr();
    control_line(Err);
    if (Err==16)
    {
      control_line(0); // đi thẳng
      delay(1000);
      dung(); 
      if (x==1) 
      {
        trai(150);
        delay(1000);
        while(adc(sen[5])<thresh[5]-5) Serial.println(line(),BIN); 
      }
      else if (x==2)
      {
        phai(150);
        delay(1500);
        while(adc(sen[5])<thresh[5]-5) Serial.println(line(),BIN); 
      }
      dung();
      return;
    }
  }
}
void quaydau()
{
  tien(150);
  delay(700);
  phai(150);
  delay(3000);
  while(adc(sen[5])<thresh[5]-5 && (adc(sen[4])<thresh[4]-5) || adc(sen[6])<thresh[6]-5) Serial.println(line(),BIN);
  dung();
}

void loop() {
  if (cmode==1)
  {
    while (line()==0) tien(130);
    dung();
    tick(2);
    int loca=0;
    sw.begin(9600);
    while (cmode==1)
    {
      if (sw.available())
      {
        String rev = sw.readStringUntil('\n');
        rev.trim();
        if (rev.length()>=5)
        {
          if (rev[0]=='1')      loca = 1;
          else if (rev[1]=='1') loca = 2;
          else if (rev[2]=='1') loca = 3;
          else if (rev[3]=='1') loca = 4;
        }
      }
      if (loca>0)
      {
        if (loca==1)
        {
          timNGA4(1); // trai
          timDUNG();
        }
        else if (loca==2)
        {
          timNGA4(2); // phai
          timDUNG();
        }
        else if (loca==3)
        {
          timNGA4(0); 
          timNGA4(1); // trai
          timDUNG();
        }
        else
        {
          timNGA4(0); 
          timNGA4(2); // phai
          timDUNG();
        }
        ///////////////////////////////////////////
          unsigned long tim=millis();
          int thuan=1, nghich=0;
          goc2 = 90;
          sw.end(); delay(1000); sw.begin(9600);
          while (millis()-tim<10000)
          {
            sv2.write(goc2); delay(5);
            if (goc2>=140) {thuan=0; nghich=1;}
            if (goc2<=40) {thuan=1; nghich=0;}
            if (thuan==1) goc2++;
            if (nghich==1) goc2--;
            if (sw.available())
            {
              String rev2 = sw.readStringUntil('\n');
              rev2.trim();
              if (rev2.length()>=5 && rev2[4]=='1')
              {
                sv1.write(160); // Mở bình
                for (int j=90;j<=120;j++) { sv3.write(j); delay(5); }
                for (int k=0;k<=5;k++)
                {
                  for (int j=120;j>=60;j--) { sv3.write(j); delay(5); }
                  for (int j=60;j<=120;j++) { sv3.write(j); delay(5); }
                }
                for (int j=120;j>=90;j--) { sv3.write(j); delay(5); }
                sv1.write(60); // Tắt bình
              }
          }
          }
        ////////////////////////////////////////
        quaydau();
        if (loca==1)
        {
          timNGA4(2); // phai
          timDUNG();
        }
        else if (loca==2)
        {
          timNGA4(1); // trai
          timDUNG();
        }
        else if (loca==3)
        {
          timNGA4(2); // phai
          timNGA4(0);
          timDUNG();
        }
        else
        {
          timNGA4(1); // trai
          timNGA4(0);
          timDUNG();
        }
        quaydau();
        tick(3);
        loca=0;
        sw.end(); delay(1000);
        sw.begin(9600);
      }
      if (bluetooth.available())
      {
        if (bluetooth.read()=='E') cmode=0;
      }
      delay(1);
    }
  }
  while (cmode==0)
  { 
    
    if (bluetooth.available())
    {
      ch = bluetooth.read();
      if (ch=='A') tien();
      else if (ch=='B') lui();
      else if (ch=='C') trai();
      else if (ch=='D') phai();
      else if (ch=='E') dung();
      else if (ch=='P') sv1.write(160);
      else if (ch=='Q') sv1.write(60);
      else if (ch=='R') // tự động
      {
        cmode = 1;
        tick(2,100);
        goc2=90; goc3=90;
        sv1.write(60);
        sv2.write(goc2);
        sv3.write(goc3);
      }
    }
  
    else if (ch=='I') 
    {
      if (goc2>40) goc2--;
      sv2.write(goc2);
      delay(10);
    }
    else if (ch=='K') 
    {
      if (goc2<140) goc2++;
      sv2.write(goc2);
      delay(10);
    }
    else if (ch=='M') 
    {
      if (goc3>60) goc3--;
      sv3.write(goc3);
      delay(10);
    }
    else if (ch=='L') 
    {
      if (goc3<120) goc3++;
      sv3.write(goc3);
      delay(10);
    }
    delay(10);
  }
}
