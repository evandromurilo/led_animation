/*
 *  LED_ANIMATION
 *  =============
 *   created by: Evandro Murilo
 *
 *   github.com/evandromurilo
 *   evandromurilo.com
*/

byte LED_PINS[] = {0, 1, 2, 3, 4, 5, 6};
byte SHORT_LED_PINS[] = {11, 12, 13, 255};

byte start_animation[] = {B01000000,
                          B00100000,
                          B00010000,
                          B00001000,
                          B00000100,
                          B00000010,
                          B00000001,
                          B10000000};

byte middle_animation[] = {B00000000,
                           B01000000,
                           B01010000,
                           B01010100,
                           B01010101,
                           B01110101,
                           B01111101,
                           B01111111,
                           B01111111,
                           B10000000};

byte end_animation[] = {B00000000,
                        B01111111,
                        B00000000,
                        B01111111,
                        B10000000};

byte short_animation[] = {B00000100,
                          B00000110,
                          B00000111,
                          B10000000};

void setup() {
    for (int i = 0; i < 7; ++i) {
        pinMode(LED_PINS[i], OUTPUT);
    }
  
 	for (int i = 0; i < 3; ++i) {
   		pinMode(SHORT_LED_PINS[i], OUTPUT);
  	}
}

void loop() {
    playFrame(SHORT_LED_PINS, short_animation);
    playAnimation(LED_PINS, start_animation, 100);
    playFrame(SHORT_LED_PINS, short_animation);
    playAnimation(LED_PINS, middle_animation, 100);
    playFrame(SHORT_LED_PINS, short_animation);
    playAnimation(LED_PINS, end_animation, 350);
}

/*
 * Function: updateLED
 * ---------------
 *  turn LEDs on or off according to led_status
 * 
 *  LEDs: an array of LED pins
 *  led_status: a byte where each bit represents a LED status
 */
void updateLED(byte *LEDs, byte led_status) {
    for (int i = 0; i < 7; ++i) {
        // 255 indicates there are no more LEDs
        if (LEDs[i] == 255) break;
        digitalWrite(LEDs[i], bitRead(led_status, i));
    }
}

/*
 * Function: playFrame
 * ---------------
 *  plays a frame of an animation on a set of LEDs
 *
 *  may be used when you don't want to lock the program for the
 *  duration of the animation
 *
 *  cannot play two animations simultaneously
 * 
 *  LEDs: an array of LED pins
 *  animation: an array of bytes, each containing the status of the LEDs during
 *             a frame of the animation
 *
 *  returns: false if last frame, true otherwise
 */
bool playFrame(byte *LEDs, byte *animation) {
    static byte *frame;
    static byte *curr_animation; 

    if (curr_animation != animation) frame = curr_animation = animation;

    updateLED(LEDs, *frame);
    frame++;

    // a 1 on the leftmost bit of a frame indicates the end of the animation
    if (bitRead(*frame, 7)) {
        frame = animation;
        return false;
    }
    else {
        return true;
    }
}

/*
 * Function: playAnimation
 * ---------------
 *  plays a full animation on a set of LEDs
 *
 *  locks the program until the animation ends
 * 
 *  LEDs: an array of LED pins
 *  animation: an array of bytes, each containing the status of the LEDs during
 *             a frame of the animation
 *  d: duration of each frame, in milliseconds
 */
void playAnimation(byte *LEDs, byte *animation, int d) {
    byte *frame = animation;

    // a 1 on the leftmost bit of a frame indicates the end of the animation
    while (!bitRead(*frame, 7)) {
        updateLED(LEDs, *frame);
        delay(d);
        ++frame;
    }
    
    delay(d);
}
