#define BUZZ 15
// 84:FC:E6:D1:24:54

#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t broadcastAddress[] = {0x84, 0xFC, 0xE6, 0xD1, 0x24, 0x54};
int id = 3;
int sen[6] = {12, 13, 14, 16, 5, 4};

typedef struct struct_message {
    int id;
    int st;
} struct_message;
struct_message myData;

void setup() {
    pinMode(BUZZ, OUTPUT);
    digitalWrite(BUZZ, 0);
    pinMode(2, OUTPUT);
    digitalWrite(2, 1);
    Serial.begin(9600);
    for (int j = 0; j < 6; j++) {
        pinMode(sen[j], INPUT_PULLUP);
    }
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    Serial.println("ESP-NOW OK");
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

    myData.id = id;
    myData.st = 0;
}
void loop() {
    Serial.print(digitalRead(sen[0]));
    Serial.print(digitalRead(sen[1]));
    Serial.print(digitalRead(sen[2]));
    Serial.print(digitalRead(sen[3]));
    Serial.print(digitalRead(sen[4]));
    Serial.println(digitalRead(sen[5]));

    int sum = 0;
    for (int j = 0; j < 6; j++)
        sum += digitalRead(sen[j]);
    if (sum < 6) {
        myData.st = 1;
        digitalWrite(BUZZ, !digitalRead(BUZZ));
        digitalWrite(2, !digitalRead(2));
    } else {
        myData.st = 0;
        digitalWrite(BUZZ, 0);
        digitalWrite(2, 1);
    }
    esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    delay(100);
}
