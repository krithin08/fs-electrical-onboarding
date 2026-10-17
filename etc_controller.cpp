//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"

// Assign appropriate GPIO objects depending on pin parameters


ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) 
: apps1(APPS1_pin), apps2(APPS2_pin) {}



/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    apps1_voltage = apps1.read() * 3.3f;
    apps2_voltage = apps2.read() * 3.3f;

    if (apps1_voltage < 0.0f || apps1_voltage > 3.3f || apps2_voltage < 0.0f || apps2_voltage > 3.3f) {
        value = 0.0f;
    }

    else {
        value = 1.0f;
    }


    return value; // Required so the float function always returns something
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {

        bool isTrue = false;
    if (value >= low && value <= high) {
        isTrue = true;
    }
    else
    {
        isTrue = false;
    }
       return isTrue;

}

/*
    Complicated method with the goal of refreshing voltages,
    pedal positions, implausibilities, and motor torque demand.
*/
void ETCController::update_state() { 
    /*
        Retrieve both APPS sensor voltages, determine pedal
        positions from globally set variables in header file, 
        then calculate a motor torque demand based off the 
        average if the two positions. 

        If you want an extra challenge, include the pedal
        deadzone percentage when determining pedal positions. 
        If you want to get really difficult, maybe even 
        include your own pedal mapping to make it non   
   
        */
        apps_1_voltage = apps1.read() * 3.3f;
        apps_2_voltage = apps2.read() * 3.3f;

        float apps_1_position = (apps_1_voltage - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE);
        float apps_2_position = (apps_2_voltage - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE);

        float modified apps_1_position = clamps(apps_1_position);
        float modified apps_2_position = clamps(apps_2_position);

        motor_torque_demand = (modified apps_1_position + modified apps_2_position) / 2.0f * MAX_TORQUE;



    /*
        Finish the step listed above.
    */

    
    
    // Here to simplify this function and readability.
    update_implausibilities();
}

/*
    Timer based logic for the ETC to be rules compliant. MUST
    include rules T.4.2.4, T.4.2.9 (out of range).
*/
void ETCController::update_implausibilities() {
    /*
        Start of implaus logic checking.
    */

    if(apps1_voltage < APPS1_MIN_VOLTAGE || apps1_voltage > APPS1_MAX_VOLTAGE || 
       apps2_voltage < APPS2_MIN_VOLTAGE || apps2_voltage > APPS2_MAX_VOLTAGE) {
        
        bool isImplaus = true;
        
        // Out of range implausibility logic here
    }

    /*
        End of implaus logic checking.
    */

        if (implausibility_timer.read() > IMPLAUSIBILITY_TIMEOUT) {
            isImplaus = false;

           
        }
        else {
            isImplaus = true;
            implausibility_timer.reset();
            implausibility_timer.start();
           
            
        }



    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */



    
}
