#include <LiquidCrystal.h>
#include <ChibiOS_AVR.h>

#define BACKLIGHT_PIN 10
#define MOTION_PIN 13
#define SPEAKER_PIN 6

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char* sayings[] = {"You're my papa", "See you soon.", "Hello!", "I love you", "Great job", "You Won!", "Congratulations", "I missed you", "good", "You're beautiful", "We made it!", "Keep on going!", "Perfect", "Great to see you!"};

volatile boolean sensed = false;

static WORKING_AREA(waTh1, 100);
static WORKING_AREA(waTh2, 100);
static WORKING_AREA(waTh3, 100);
SEMAPHORE_DECL(speakerSem, 0);

msg_t mainThread(void *args){
    while(1){
        Serial.println(digitalRead(MOTION_PIN));
        if(digitalRead(MOTION_PIN)){
            chSemSignal(&speakerSem);
            sensed = true;
            chThdSleepMilliseconds(4000);
        }else{
            sensed = false;
            Serial.println("Shifting");
        }
    }
}

msg_t screenThread(void *args){
    while(1){
        if(sensed){
            lcd.clear(); 
            print_sayings();
            chThdSleepMilliseconds(4000);
            lcd.clear(); 
        }else{
            shift_eyes();
        }
    }
}

msg_t speakerThread(void *args){
    while(1){
        chSemWait(&speakerSem);
        volatile int endNote = 100 + random(500);
        tone(SPEAKER_PIN, endNote); 
        chThdSleepMilliseconds(200+random(500));
        tone(SPEAKER_PIN, endNote+random(500)); 
        chThdSleepMilliseconds(200+random(500));
        tone(SPEAKER_PIN, endNote+random(500)-250); 
        chThdSleepMilliseconds(2000);
        noTone(SPEAKER_PIN);
    }
}

void setup(){
    Serial.begin(9600);
   chBegin(chSetup);
    while(1);
}

void chSetup(){
    pinMode(BACKLIGHT_PIN, OUTPUT);
    pinMode(MOTION_PIN, INPUT);
    lcd.begin(16, 2);
    lcd.print("LOADING...");
    fade_in(BACKLIGHT_PIN);
    chThdSleepMilliseconds(2000);
    lcd.clear();
    chThdCreateStatic(waTh1, sizeof(waTh1), NORMALPRIO, mainThread, NULL);
    chThdCreateStatic(waTh2, sizeof(waTh2), NORMALPRIO, speakerThread, NULL);
    chThdCreateStatic(waTh3, sizeof(waTh3), NORMALPRIO, screenThread, NULL);
}

void shift_eyes(){
    static long start_time = 0;
    long time = millis();
    if(time < start_time + 1000){
        draw_large_eye_center(0);
        draw_large_eye_center(12);
    }else if(time < start_time + 2000){
        draw_large_eye_left(0);
        draw_large_eye_left(12);
    }else if(time < start_time + 3000){
        draw_large_eye_center(0);
        draw_large_eye_center(12);
    }else if(time < start_time + 4000){
        draw_large_eye_right(0);
        draw_large_eye_right(12);
    }else if(time < start_time + 5000){
        start_time = millis();
    }else{
        start_time = millis();
    }
}

void print_sayings(){
    lcd.setCursor(0,0);
    lcd.print(sayings[random(sizeof(sayings)/sizeof(sayings[0]))]);
    if(random(200) > 100){
        lcd.setCursor(0,1);
        lcd.print(sayings[random(sizeof(sayings)/sizeof(sayings[0]))]);
    }
}

void fade_out(int pin){
    for(int i = 255; i > 1; i--){
        analogWrite(pin, i);
        chThdSleepMilliseconds(5);
    }
}

void fade_in(int pin){
    for(int i = 0; i < 251; i++){
        analogWrite(pin, i);
        chThdSleepMilliseconds(5);
    }
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

void loop(){}
