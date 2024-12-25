#include <SoftwareSerial.h>
SoftwareSerial sw(37,39);

int sen[6] = {5,7,9,11,4,6};
#include <esp_now.h>
#include <WiFi.h> 
typedef struct struct_message {
  int id;
  int st;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;
int stt[5] = {0,0,0,0,0};

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print(myData.id);
  Serial.println(myData.st);
  stt[myData.id-1] = myData.st;
}

void setup() {
  Serial.begin(9600);
  sw.begin(9600);
  
  for (int j=0;j<6;j++) pinMode(sen[j],INPUT_PULLUP);
  delay(1000);
  String macAddress = WiFi.macAddress();
  Serial.println("Địa chỉ MAC của ESP32: " + macAddress);
  
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  
}

void loop() {
  
//  Serial.print(analogRead(5)); Serial.print("\t");
//  Serial.print(analogRead(7)); Serial.print("\t");
//  Serial.print(analogRead(9)); Serial.print("\t");
//  Serial.print(analogRead(11)); Serial.print("\t");
//  Serial.print(analogRead(4)); Serial.print("\t");
//  Serial.print(analogRead(6)); Serial.println("\t");

  int sum = 0;
  for (int j=0;j<6;j++) 
  {
    sum += analogRead(sen[j]);
    delay(10);
  }
  Serial.println(sum);
  if (sum<40000) 
  {
    stt[4] = 1;
    Serial.println("PHAT HIEN!!!!!!!!!");
  }
  else stt[4] = 0;
  char str[10];
  sprintf(str,"%d%d%d%d%d",stt[0],stt[1],stt[2],stt[3],stt[4]);
  sw.println(str);
  delay(100);
}
