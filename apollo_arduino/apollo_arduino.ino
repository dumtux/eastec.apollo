#include "analogWrite.h"
#include "BluetoothA2DPSink.h"

#define I2S_BCK  26
#define I2S_LRCK 22
#define I2S_DOUT 25

#define LED2     5
#define LED3     21

#define STROBE1   19
#define RESET1    18
#define AREAD1    34
#define STROBE2   17
#define RESET2    16
#define AREAD2    35

#define DRV2_1    12
#define DRV2_2    13
#define DRV1_1    33
#define DRV1_2    32

#define BTN1      5
#define BTN2      2
#define BTN3      27
#define BTN4      4
#define BTN5      15
#define BTN6      13

// custom animation parameters
#define ANIM_MAGNITUTE 0.8  // between 0 ~ 1.0, average animation magnitude
#define FREQ_63    true
#define FREQ_160   true
#define FREQ_400   true
#define FREQ_1000  true
#define FREQ_2500  true
#define FREQ_6250  true
#define FREQ_16000 true


int eq_l[7];
int eq_r[7];
int avg_l, avg_r;

BluetoothA2DPSink a2dp_sink;
bool is_playing = false;
float volume = 0.5;


void initMSGEQ7() {
    pinMode(RESET1, OUTPUT);
    pinMode(STROBE1, OUTPUT);
    pinMode(AREAD1, INPUT_PULLUP);
    pinMode(RESET2, OUTPUT);
    pinMode(STROBE2, OUTPUT);
    pinMode(AREAD2, INPUT_PULLUP);

    digitalWrite(RESET1, LOW);
    digitalWrite(STROBE1, HIGH);
    digitalWrite(RESET2, LOW);
    digitalWrite(STROBE2, HIGH);
}


void readMSGEQ7() {
    // get the MSGEQ7's attention
    digitalWrite( RESET1, HIGH );
    digitalWrite( RESET2, HIGH );
    delayMicroseconds(30);
    digitalWrite( RESET1, LOW );
    digitalWrite( RESET2, LOW );

    // reset average values
    avg_l = 0;
    avg_r = 0;

    // for each available EQ band
    for(int band = 0; band < 7; band++) {
        digitalWrite( STROBE1, LOW );
        digitalWrite( STROBE2, LOW );
        delayMicroseconds(30); // should be 36 but works with 30
        eq_l[band]  = analogRead(AREAD1);
        eq_r[band] = analogRead(AREAD2);
        avg_l += eq_l[band];
        avg_r += eq_r[band];
        digitalWrite( STROBE1, HIGH );
        digitalWrite( STROBE2, HIGH );
    }
    avg_l /=7;
    avg_r /=7;
}


void init_btn() {
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);
    pinMode(BTN3, INPUT_PULLUP);
    pinMode(BTN4, INPUT_PULLUP);
    pinMode(BTN5, INPUT_PULLUP);
    pinMode(BTN6, INPUT_PULLUP);
}


void read_btn() {
    if (digitalRead(BTN1) == LOW) {
        // bt detection mode
        delay(200);
    } else if (digitalRead(BTN2) == LOW) {
        if (is_playing) {
            is_playing = false;
            a2dp_sink.pause();
        } else {
            is_playing = true;
            a2dp_sink.play();
        }
        delay(200);
    } else if (digitalRead(BTN3) == LOW) {
        is_playing = true;
        a2dp_sink.previous();
        delay(200);
    } else if (digitalRead(BTN4) == LOW) {
        is_playing = true;
        a2dp_sink.next();
        delay(200);
    } else if (digitalRead(BTN5) == LOW) {
        if (volume >=0.1) {
            volume -= 0.1;
        }
        delay(200);
    } else if (digitalRead(BTN6) == LOW) {
        if (volume <=0.9) {
            volume += 0.1;
        }
        delay(200);
    }
}


void read_data_stream(const uint8_t *data, uint32_t length)
{
    int16_t *ptr16 = (int16_t*) data; 
    uint32_t len16 = length/2;
    for(int j=0;j<len16;j++){
        ptr16[j] = volume * ptr16[j];
    }
}


void setup() {
    Serial.begin(9600);

   pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(DRV1_1, OUTPUT);
    pinMode(DRV1_2, OUTPUT);
    pinMode(DRV2_1, OUTPUT);
    pinMode(DRV2_2, OUTPUT);

    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(DRV1_1, HIGH);
    digitalWrite(DRV1_2, HIGH);
    digitalWrite(DRV2_1, HIGH);
    digitalWrite(DRV2_2, HIGH);

    i2s_pin_config_t my_pin_config = {
        .bck_io_num = I2S_BCK,
        .ws_io_num = I2S_LRCK,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    a2dp_sink.set_pin_config(my_pin_config);
    a2dp_sink.set_stream_reader(read_data_stream);
    a2dp_sink.start("ApolloMusic");

    initMSGEQ7();
    init_btn();
}


void loop() {
    bool is_audio_active = a2dp_sink.get_audio_state();
    if (is_audio_active) {
        readMSGEQ7();

        int j = 0;
        int eq = 0;
        if (FREQ_63) {
            eq += eq_l[0] + eq_r[0];
            j += 2;
        }
        if (FREQ_160) {
            eq += eq_l[1] + eq_r[1];
            j += 2;
        }
        if (FREQ_400) {
            eq += eq_l[2] + eq_r[2];
            j += 2;
        }
        if (FREQ_1000) {
            eq += eq_l[3] + eq_r[3];
            j += 2;
        }
        if (FREQ_2500) {
            eq += eq_l[4] + eq_r[4];
            j += 2;
        }
        if (FREQ_6250) {
            eq += eq_l[5] + eq_r[5];
            j += 2;
        }
        if (FREQ_16000) {
            eq += eq_l[6] + eq_r[6];
            j += 2;
        }
        if (j > 0) {
            eq = eq / j;
        }
        int anim_val = (int) eq / 10 * volume * ANIM_MAGNITUTE;

        Serial.println(anim_val, DEC);
        analogWrite(DRV1_2, anim_val);
        analogWrite(LED3, anim_val);
    } else {
        digitalWrite(DRV1_2, HIGH);
        digitalWrite(LED3, LOW);
    }

    read_btn();
    delay(10);
}
