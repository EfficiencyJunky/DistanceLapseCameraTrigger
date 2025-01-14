

#include "BL_LEDControl.h"

// ******************************************************************    
//                 LED Controller CLASS
//    This class manages the state of the led based
//    on the input from the buttons
//    It uses software timers to ensure that it doesn't 
//    block the main thread
// ******************************************************************


// ******************************************************************    
//                         UPDATE METHOD
// ******************************************************************  
void BL_LEDControl::Update() {

    switch (STATE) {
        // inactive state so we aren't constantly writing to the LED pin
        case INACTIVE:
        break;
        
        // turn the LED ON
        case LED_ON:
            // digitalWrite(ledPin, HIGH);
            analogWrite(ledPin, highBrightness);
            STATE = INACTIVE;
            break;
        
        // turn the LED OFF
        case LED_OFF:
            // digitalWrite(ledPin, LOW);
            analogWrite(ledPin, 0);
            STATE = INACTIVE;
            break;
        
        // turn the LED ON and then OFF after "ledOnOffInterval" milliseconds
        case LED_ON_OFF:
            // digitalWrite(ledPin, HIGH);
            analogWrite(ledPin, highBrightness);
            if( (millis() - ledOnOffStartTime) > ledOnOffInterval){
                STATE = LED_OFF;
            }
            break;
        
        // turn the LED OFF and then back to PULSE after "ledOnOffInterval" milliseconds
        case LED_OFF_PULSE: 
            // digitalWrite(ledPin, LOW);
            analogWrite(ledPin, 0);
            if( (millis() - ledOnOffStartTime) > ledOnOffInterval){
                STATE = LED_PULSE;
            }
            break;

        // slowly pulse the led brightness
        case LED_PULSE:
            if( (millis() - lastPulseUpdateTime) > pulseUpdateInterval){
                analogWrite(ledPin, brightness);

                // change the brightness for next time through the loop:
                brightness = brightness + fadeAmount;

                // reverse the direction of the fading at the ends of the fade:
                if (brightness <= lowBrightness || brightness >= highBrightness) {
                    fadeAmount = -fadeAmount;
                }

                lastPulseUpdateTime = millis();
            }
            break;

    //      default:
    //        break;

    }

}

/*----------------------------------------------------------------------*
/ Set the state to LED_ON                                               *
/-----------------------------------------------------------------------*/
void BL_LEDControl::turnOn() {
    STATE = LED_ON;
}

/*----------------------------------------------------------------------*
/ Set the state to LED_OFF                                              *
/-----------------------------------------------------------------------*/
void BL_LEDControl::turnOff() {
    STATE = LED_OFF;
}

/*----------------------------------------------------------------------*
/ Set the state to LED_ON_ON or LED_ON_OFF depending on if the input    *
/ is inverted and record the time this occured                          *
/-----------------------------------------------------------------------*/
void BL_LEDControl::turnOnOff(bool normalState) {
    if(normalState) {
        STATE = LED_ON_OFF;
    }
    else {
        STATE = LED_OFF_PULSE;
    }

    ledOnOffStartTime = millis();
}

/*----------------------------------------------------------------------*
/ Set the state to LED_PULSE                                            *
/-----------------------------------------------------------------------*/
void BL_LEDControl::pulse(bool normalState) {
    
    if(normalState) {
        STATE = LED_PULSE;
    }
    else {
        STATE = LED_OFF;
    }    
    
}



