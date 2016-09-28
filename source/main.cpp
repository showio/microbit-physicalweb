#include "MicroBit.h"

MicroBit uBit;

char INTRO[] = "SHOW.IO";
char URL[] = "https://show.io/Fq5sf7O9Y";

// lvl : Pwr@ 1m : Pwr@ 0m
//  0  :   -90   :   -49
//  1  :   -78   :   -37
//  2  :   -74   :   -33
//  3  :   -69   :   -28 
//  4  :   -66   :   -25 
//  5  :   -61   :   -20 
//  6  :   -56   :   -15 
//  7  :   -51   :   -10 
const int8_t CALLIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

uint8_t power = 4;
bool screen_off = false;

/**
* Displays the current transmission power on the led display
*/
void updateDisplay()
{
    if (screen_off) {
        MicroBitImage clear_display("0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
        uBit.display.print(clear_display);
    } else {
        if (power == 0)
        {
            MicroBitImage tick("255,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 1)
        {
            MicroBitImage tick("255,255,0,0,0\n255,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 2)
        {
            MicroBitImage tick("255,255,255,0,0\n255,255,0,0,0\n255,0,0,0,0\n0,0,0,0,0\n0,0,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 3)
        {
            MicroBitImage tick("255,255,255,255,0\n255,255,255,0,0\n255,255,0,0,0\n255,0,0,0,0\n0,0,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 4)
        {
            MicroBitImage tick("255,255,255,255,255\n255,255,255,255,0\n255,255,255,0,0\n255,255,0,0,0\n255,0,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 5)
        {
            MicroBitImage tick("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,0\n255,255,255,0,0\n255,255,0,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 6)
        {
            MicroBitImage tick("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,0\n255,255,255,0,0\n");
            uBit.display.print(tick);
        }
        else if (power == 7)
        {
            MicroBitImage tick("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,0\n");
            uBit.display.print(tick);
        }
        else if (power == 8)
        {
            MicroBitImage tick("255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n255,255,255,255,255\n");
            uBit.display.print(tick);
        }
    }
}

/**
* Handles incoming events
* @param evt: the event that triggered
*/
void onEvent(MicroBitEvent evt)
{
    // Device turn on
    if (screen_off == true && evt.value == MICROBIT_BUTTON_EVT_CLICK) {
        screen_off = false;
        updateDisplay();
        return;
    }

    // Decrease power
    if (evt.source == MICROBIT_ID_BUTTON_A && evt.value == MICROBIT_BUTTON_EVT_CLICK) {
        if (power <= 0) return;
        power--;
        if (power == 0) {
            uBit.bleManager.stopAdvertising();
        } else {
            uBit.bleManager.advertiseEddystoneUrl(URL, CALLIBRATED_POWERS[power - 1], false);
            uBit.bleManager.setTransmitPower(power-1);
        }
        updateDisplay();
        return;
    }

    // Increase power
    if (evt.source == MICROBIT_ID_BUTTON_B && evt.value == MICROBIT_BUTTON_EVT_CLICK) {
        if (power >= 8) return;
        power++;
        uBit.bleManager.advertiseEddystoneUrl(URL, CALLIBRATED_POWERS[power - 1], false);
        uBit.bleManager.setTransmitPower(power-1);
        updateDisplay();
        return;
    }

    // Turn off display
  if (evt.source == MICROBIT_ID_ACCELEROMETER) {
    if (uBit.accelerometer.getGesture() > 5) {
      screen_off = true;
      updateDisplay();
    }
    return;
  }
}

/**
* Main
*/
int main()
{
    uBit.init();
    uBit.display.scroll(INTRO);

    uBit.bleManager.advertiseEddystoneUrl(URL, CALLIBRATED_POWERS[power - 1], false);
    uBit.bleManager.setTransmitPower(power-1);
    updateDisplay();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, onEvent);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_EVT_ANY, onEvent);
    uBit.messageBus.listen(MICROBIT_ID_ACCELEROMETER, MICROBIT_EVT_ANY, onEvent);

    release_fiber();
}

