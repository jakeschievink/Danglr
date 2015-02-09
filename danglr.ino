#include <LiquidCrystal.h>
#define BACKLIGHT_PIN 10
#define MOTION_PIN 14
#define SPEAKER_PIN 6
#define THIRD_EYE 13
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char* sayings[] = {"You're my papa", "I love you", "Great job", "You Won!", "Congratulations", "I missed you", "good", "You're beautiful"};
void setup(){
    Serial.begin(9600);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    pinMode(MOTION_PIN, INPUT);
    lcd.begin(16, 2);
    analogWrite(BACKLIGHT_PIN, 255);
}
void loop(){
        draw_large_eye_center(0);
        draw_large_eye_center(12);
        delay(1000);
        draw_large_eye_left(0);
        draw_large_eye_left(12);
        delay(1000);
        draw_large_eye_center(0);
        draw_large_eye_center(12);
        delay(100);
        draw_large_eye_right(0);
        draw_large_eye_right(12);
        delay(1000);
        play_sound();
        lcd.clear();
        lcd.print(sayings[random(sizeof(sayings)/sizeof(sayings[0]))]);
        delay(1000);
        noTone(SPEAKER_PIN);
        lcd.clear();
}
void play_sound(){
    for(int i = 50; i<1000; i*=1.2){
        tone(SPEAKER_PIN, i+random(10), random(10));
        delay(10);
    }
}
void third_eye_pulse(){
    fade_in(THIRD_EYE);
    fade_out(THIRD_EYE);
}
void fade_in(int pin){
    for(int i = 0; i < 251; i++){
        analogWrite(pin, i);
        delay(10);
    }
}
void draw_small_eye(int line, int charpos){
  byte eye[8] = { B00000,
                B01110,
                B10001,
                B10101,
                B01110,
                B00000,
                B00000,
                B00000 };
  lcd.createChar((byte)0, eye);
  lcd.setCursor(charpos, line);
  lcd.write((byte)0);
}
void draw_large_eye_center(int charpos){
    byte eye1[8] = {B00000,
                    B00000,
                    B00001,
                    B00001,
                    B00011,
                    B00011,
                    B00111,
                    B00111};
    byte eye2[8] = {B00011,
                    B01111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye3[8] = {B11000,
                    B11110,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye4[8] = {B00000,
                    B00000,
                    B10000,
                    B10000,
                    B11000,
                    B11000,
                    B11100,
                    B11100};
    byte eye5[8] = {B00111,
                    B00111,
                    B00011,
                    B00011,
                    B00001,
                    B00001,
                    B00000,
                    B00000};
    byte eye6[8] = {B11111,
                    B11110,
                    B11100,
                    B11110,
                    B11111,
                    B11111,
                    B01111,
                    B00011};
    byte eye7[8] = {B11111,
                    B01111,
                    B00111,
                    B01111,
                    B11111,
                    B11111,
                    B11110,
                    B11000};
    byte eye8[8] = {B11100,
                    B11100,
                    B11000,
                    B11000,
                    B10000,
                    B10000,
                    B00000,
                    B00000};
 
  lcd.createChar((byte)1, eye1);
  lcd.createChar((byte)2, eye2);
  lcd.createChar((byte)3, eye3);
  lcd.createChar((byte)4, eye4);
  lcd.createChar((byte)5, eye5);
  lcd.createChar((byte)6, eye6);
  lcd.createChar((byte)7, eye7);
  lcd.createChar((byte)8, eye8);
  lcd.setCursor(charpos, 0);
  lcd.write((byte)1);
  lcd.setCursor(charpos+1, 0);
  lcd.write((byte)2);
  lcd.setCursor(charpos+2, 0);
  lcd.write((byte)3);
  lcd.setCursor(charpos+3, 0);
  lcd.write((byte)4);
  lcd.setCursor(charpos, 1);
  lcd.write((byte)5);
  lcd.setCursor(charpos+1, 1);
  lcd.write((byte)6);
  lcd.setCursor(charpos+2, 1);
  lcd.write((byte)7);
  lcd.setCursor(charpos+3, 1);
  lcd.write((byte)8);
}
void draw_large_eye(int charpos){
    byte eye1[8] = {B00000,
                    B00000,
                    B00001,
                    B00001,
                    B00011,
                    B00011,
                    B00111,
                    B00111};
    byte eye2[8] = {B00011,
                    B01111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye3[8] = {B11000,
                    B11110,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye4[8] = {B00000,
                    B00000,
                    B10000,
                    B10000,
                    B11000,
                    B11000,
                    B11100,
                    B11100};
    byte eye5[8] = {B00111,
                    B00111,
                    B00011,
                    B00011,
                    B00001,
                    B00001,
                    B00000,
                    B00000};
    byte eye6[8] = {B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B01111,
                    B00011};
    byte eye7[8] = {B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11110,
                    B11000};
    byte eye8[8] = {B11100,
                    B11100,
                    B11000,
                    B11000,
                    B10000,
                    B10000,
                    B00000,
                    B00000};
 
  lcd.createChar((byte)1, eye1);
  lcd.createChar((byte)2, eye2);
  lcd.createChar((byte)3, eye3);
  lcd.createChar((byte)4, eye4);
  lcd.createChar((byte)5, eye5);
  lcd.createChar((byte)6, eye6);
  lcd.createChar((byte)7, eye7);
  lcd.createChar((byte)8, eye8);
  lcd.setCursor(charpos, 0);
  lcd.write((byte)1);
  lcd.setCursor(charpos+1, 0);
  lcd.write((byte)2);
  lcd.setCursor(charpos+2, 0);
  lcd.write((byte)3);
  lcd.setCursor(charpos+3, 0);
  lcd.write((byte)4);
  lcd.setCursor(charpos, 1);
  lcd.write((byte)5);
  lcd.setCursor(charpos+1, 1);
  lcd.write((byte)6);
  lcd.setCursor(charpos+2, 1);
  lcd.write((byte)7);
  lcd.setCursor(charpos+3, 1);
  lcd.write((byte)8);
}
void draw_large_eye_left(int charpos){
    byte eye1[8] = {B00000,
                    B00000,
                    B00001,
                    B00001,
                    B00011,
                    B00011,
                    B00111,
                    B00110};
    byte eye2[8] = {B00011,
                    B01111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B01111,
                    B00111};
    byte eye3[8] = {B11000,
                    B11110,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye4[8] = {B00000,
                    B00000,
                    B10000,
                    B10000,
                    B11000,
                    B11000,
                    B11100,
                    B11100};
    byte eye5[8] = {B00110,
                    B00111,
                    B00011,
                    B00011,
                    B00001,
                    B00001,
                    B00000,
                    B00000};
    byte eye6[8] = {B00111,
                    B01111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B01111,
                    B00011};
    byte eye7[8] = {B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11110,
                    B11000};
    byte eye8[8] = {B11100,
                    B11100,
                    B11000,
                    B11000,
                    B10000,
                    B10000,
                    B00000,
                    B00000};
 
  lcd.createChar((byte)1, eye1);
  lcd.createChar((byte)2, eye2);
  lcd.createChar((byte)3, eye3);
  lcd.createChar((byte)4, eye4);
  lcd.createChar((byte)5, eye5);
  lcd.createChar((byte)6, eye6);
  lcd.createChar((byte)7, eye7);
  lcd.createChar((byte)8, eye8);
  lcd.setCursor(charpos, 0);
  lcd.write((byte)1);
  lcd.setCursor(charpos+1, 0);
  lcd.write((byte)2);
  lcd.setCursor(charpos+2, 0);
  lcd.write((byte)3);
  lcd.setCursor(charpos+3, 0);
  lcd.write((byte)4);
  lcd.setCursor(charpos, 1);
  lcd.write((byte)5);
  lcd.setCursor(charpos+1, 1);
  lcd.write((byte)6);
  lcd.setCursor(charpos+2, 1);
  lcd.write((byte)7);
  lcd.setCursor(charpos+3, 1);
  lcd.write((byte)8);
}
void draw_large_eye_right(int charpos){
    byte eye1[8] = {B00000,
                    B00000,
                    B00001,
                    B00001,
                    B00011,
                    B00011,
                    B00111,
                    B00111};
    byte eye2[8] = {B00011,
                    B01111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111};
    byte eye3[8] = {B11000,
                    B11110,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11110,
                    B11100};
    byte eye4[8] = {B00000,
                    B00000,
                    B10000,
                    B10000,
                    B11000,
                    B11000,
                    B11100,
                    B01100};
    byte eye5[8] = {B00111,
                    B00111,
                    B00011,
                    B00011,
                    B00001,
                    B00001,
                    B00000,
                    B00000};
    byte eye6[8] = {B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B01111,
                    B00011};
    byte eye7[8] = {B11100,
                    B11110,
                    B11111,
                    B11111,
                    B11111,
                    B11111,
                    B11110,
                    B11000};
    byte eye8[8] = {B01100,
                    B11100,
                    B11000,
                    B11000,
                    B10000,
                    B10000,
                    B00000,
                    B00000};
 
  lcd.createChar((byte)1, eye1);
  lcd.createChar((byte)2, eye2);
  lcd.createChar((byte)3, eye3);
  lcd.createChar((byte)4, eye4);
  lcd.createChar((byte)5, eye5);
  lcd.createChar((byte)6, eye6);
  lcd.createChar((byte)7, eye7);
  lcd.createChar((byte)8, eye8);
  lcd.setCursor(charpos, 0);
  lcd.write((byte)1);
  lcd.setCursor(charpos+1, 0);
  lcd.write((byte)2);
  lcd.setCursor(charpos+2, 0);
  lcd.write((byte)3);
  lcd.setCursor(charpos+3, 0);
  lcd.write((byte)4);
  lcd.setCursor(charpos, 1);
  lcd.write((byte)5);
  lcd.setCursor(charpos+1, 1);
  lcd.write((byte)6);
  lcd.setCursor(charpos+2, 1);
  lcd.write((byte)7);
  lcd.setCursor(charpos+3, 1);
  lcd.write((byte)8);
}
void fade_out(int pin){
    for(int i = 255; i > 1; i--){
        analogWrite(pin, i);
        delay(10);
    }
}
