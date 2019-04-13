#include <Servo.h>

#include <SPI.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <deprecated.h>
#include <MFRC522.h>

#define SERVO_PIN         3
#define SERVO_LOCK_POS    180
#define SERVO_UNLOCK_POS  0

#define BUZZER_PIN        4

#define RFID_RST_PIN      9
#define RFID_SS_PIN       10

#define UNLOCK_TIMEOUT    200000

bool is_unlocked = false;
unsigned long unlock_time;
unsigned long elapsed_time_since_unlock;

Servo servo_unlocker;
MFRC522 rfid_reader(RFID_SS_PIN, RFID_RST_PIN);

void beep(int duration_ms = 200) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration_ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void servo_to_position(int pos) {
  servo_unlocker.attach(SERVO_PIN);
  delay(100);

  servo_unlocker.write(pos);
  delay(1000);
  
  servo_unlocker.detach();
}

void servo_to_lock_position() {
  servo_to_position(SERVO_LOCK_POS);
}

void servo_to_unlock_position() {
  servo_to_position(SERVO_UNLOCK_POS);
}



void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600, SERIAL_8N1);

  while (!Serial);
  Serial.println("Serial Port initialized (Baud rate 9600, SERIAL_8N1).");

  SPI.begin();
  rfid_reader.PCD_Init();
  rfid_reader.PCD_DumpVersionToSerial();
  
  Serial.println("RFID unlocker online.");
}

void loop() {
  if (!rfid_reader.PICC_IsNewCardPresent()) {
    if (is_unlocked && !rfid_reader.PICC_ReadCardSerial()) {
      // if card is not detected, add to elapsed time
      elapsed_time_since_unlock += (millis() - unlock_time);
  
      // check if unlock has timed out (and card is not detected)
      if (elapsed_time_since_unlock >= UNLOCK_TIMEOUT) {
        Serial.println("Card not detected anymore. Locking door!");
        servo_to_lock_position();
        is_unlocked = false;
        elapsed_time_since_unlock = 0;
      }
    }

    return;
  }

  if (is_unlocked && rfid_reader.PICC_ReadCardSerial()) {
    // reset the timeout because the card is still being held up to the reader  
    elapsed_time_since_unlock = 0;
  } else {
    is_unlocked = true;

    // do the door unlock
    Serial.println("PICC detected!");
    
    // beep
    beep();
  
    // move servo
    servo_to_unlock_position();  
  }
  
  unlock_time = millis();
}
