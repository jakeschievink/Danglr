#include <LiquidCrystal.h>
#define BACKLIGHT_PIN 23
#define MOTION_PIN 14
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup(){
    Serial.begin(9600);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    pinMode(MOTION_PIN, INPUT);
    lcd.begin(16, 2);
    fade_in();
}
void loop(){
    if(digitalRead(MOTION_PIN)){
        fade_out();
        lcd.clear();
        for(int i = 0; i < random(100); i++){
            lcd.write(random(100));
        }
        fade_in();
        delay(2000);
    }
}
void fade_in(){
    for(int i = 0; i < 251; i++){
        analogWrite(BACKLIGHT_PIN, i);
        delay(10);
    }
}
void fade_out(){
    for(int i = 255; i > 1; i--){
        analogWrite(BACKLIGHT_PIN, i);
        delay(10);
    }
}
