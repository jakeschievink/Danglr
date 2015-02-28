/*TODO 
  *switch to mailboxes for screen/serial communication
  *make all screen interactions in main part of screen (main will just send messages)
  *random number thread 
*/
#include <ChibiOS_AVR.h>
#include <LiquidCrystal.h>

#define BACKLIGHT_PIN 10
#define MOTION_PIN 13
#define SPEAKER_PIN 6
#define DEBUG
/*
Pin map:
    Screen
        1 -> GND
        2 -> 5V
        3 -> POT(Contrast)
        4 -> D12
        5 -> GND
        6 -> D11
        11 -> D5
        12 -> D4 
        13 -> D3
        14 -> D2
        15 -> D10(Backlight Control)
    Motion Sensor 
       OUT -> D13

    Speaker
        1 -> GND
        2 -> D6

   */

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char* sayings[] = {"You're my papa", "See you soon.", "Hello!", "I love you", "You Won!", "Congratulations", "I missed you", "good", "You're beautiful", "We made it!", "Keep on going!", "Perfect", "Great to see you!", "Let's cook eggs!"};


static WORKING_AREA(waTh1, 100);
static WORKING_AREA(waTh2, 100);
static WORKING_AREA(waTh3, 100);

MUTEX_DECL(screenMutex);
SEMAPHORE_DECL(speakerSem, 0);

Thread *screenT;

msg_t mainThread(void *args){
    boolean asleep = true;
    pinMode(BACKLIGHT_PIN, OUTPUT);
    pinMode(MOTION_PIN, INPUT);
    lcd.begin(16, 2);
    lcd.print("LOADING...");
    fade_in(BACKLIGHT_PIN);
    chThdSleepMilliseconds(2000);
    lcd.clear();
    while(1){
        Serial.println(digitalRead(MOTION_PIN));
        if(digitalRead(MOTION_PIN) && !asleep){
            chSemSignal(&speakerSem);
            chMsgSend(screenT, (msg_t)"A");
            asleep = true;
        }else if(random(1000) > 980){
            chMsgSend(screenT, (msg_t)"B");
            asleep = true;
            chThdSleepMilliseconds(random(5000));
        }else{
            chMsgSend(screenT, (msg_t)"C");
            if(asleep){
                chThdSleepMilliseconds(400);
                chMsgSend(screenT, (msg_t)"B");
                chThdSleepMilliseconds(400);
                chMsgSend(screenT, (msg_t)"C");
                chThdSleepMilliseconds(500);
                chMsgSend(screenT, (msg_t)"B");
                chThdSleepMilliseconds(500);
                chMsgSend(screenT, (msg_t)"C");
                chThdSleepMilliseconds(2000);
                asleep = false;
            }
        }
    }
}

msg_t screenThread(void *args){
    msg_t msg;
    Thread *tp;
    while(1){
        tp = chMsgWait();
        msg = chMsgGet(tp);
        if((char*)msg == "A"){
            fade_out(BACKLIGHT_PIN);
            lcd.clear(); 
            print_sayings();
            fade_in(BACKLIGHT_PIN);
            chThdSleepMilliseconds(4000);
            lcd.clear(); 
            chMsgRelease(tp, msg);
        }else if((char*)msg == "B"){
            chMsgRelease(tp, msg);
            close_eyes();
        }else if((char*)msg == "C"){
            chMsgRelease(tp, msg);
            shift_eyes();
        }
    }
}

msg_t speakerThread(void *args){
    while(1){
        chSemWait(&speakerSem);
#ifdef DEBUG
        Serial.println("Speakers playing but debug is on");
#else
        volatile int endNote = 100 + random(500);
        tone(SPEAKER_PIN, endNote); 
        chThdSleepMilliseconds(200+random(500));
        tone(SPEAKER_PIN, endNote+random(500)); 
        chThdSleepMilliseconds(200+random(500));
        tone(SPEAKER_PIN, endNote+random(500)-250); 
        chThdSleepMilliseconds(2000);
        noTone(SPEAKER_PIN);
#endif
    }
}

void setup(){
    Serial.begin(9600);
   chBegin(chSetup);
    while(1);
}

void chSetup(){
    chThdCreateStatic(waTh1, sizeof(waTh1), NORMALPRIO, mainThread, NULL);
    chThdCreateStatic(waTh2, sizeof(waTh2), NORMALPRIO, speakerThread, NULL);
    screenT = chThdCreateStatic(waTh3, sizeof(waTh3), NORMALPRIO, screenThread, NULL);
}

void close_eyes(){
    lcd.clear();
    draw_closed_eye(0);
    draw_closed_eye(12);
}

void shift_eyes(){
    chMtxLock(&screenMutex);
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
    chMtxUnlock();
}

void print_sayings(){
    chMtxLock(&screenMutex);
    lcd.setCursor(0,0);
    lcd.print(sayings[random(sizeof(sayings)/sizeof(sayings[0]))]);
    if(random(200) > 100){
        lcd.setCursor(0,1);
        lcd.print(sayings[random(sizeof(sayings)/sizeof(sayings[0]))]);
    }
    chMtxUnlock();
}

void fade_out(int pin){
    for(int i = 251; i > 1; i-=5){
        analogWrite(pin, i);
        chThdSleepMilliseconds(2);
    }
}

void fade_in(int pin){
    for(int i = 0; i < 251; i+=5){
        analogWrite(pin, i);
        chThdSleepMilliseconds(2);
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
void draw_closed_eye(int charpos){
    byte eye1[8] = {B00100,
        B00011,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000};
    byte eye2[8] = {B00000,
        B00000,
        B11000,
        B00111,
        B00000,
        B00000,
        B00000,
        B00000};
    byte eye3[8] = {B00000,
        B00000,
        B00011,
        B11100,
        B00000,
        B00000,
        B00000,
        B00000};
    byte eye4[8] = {B00100,
        B11000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000};
    lcd.createChar((byte)1, eye1);
    lcd.createChar((byte)2, eye2);
    lcd.createChar((byte)3, eye3);
    lcd.createChar((byte)4, eye4);
  lcd.setCursor(charpos, 1);
  lcd.write((byte)1);
  lcd.setCursor(charpos+1, 1);
  lcd.write((byte)2);
  lcd.setCursor(charpos+2, 1);
  lcd.write((byte)3);
  lcd.setCursor(charpos+3, 1);
  lcd.write((byte)4);
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
