// Pin Assignments For Sound Sensor And LED
int soundSensor = A0;
int led = 7;

// boolean logic controls.
boolean high_voltage_control=true;
boolean low_voltage_control=false;
boolean whistle=false;
boolean light_status = false;
boolean sound_perceived = false;

int whistle_count = 0;

// Time Variables
unsigned long control_time;
unsigned long start_timer = -50;
unsigned long stop_timer = 100000;

void setup() { 
  Serial.begin(9600);
  pinMode(soundSensor, INPUT);
  pinMode(led, OUTPUT);
}

void loop(){
  sound_perceived = analogToDigital(analogRead(soundSensor), 500);
  
  if(sound_perceived){
    // Check the sound for it is clap or not.
    if(high_voltage_control && (millis() - start_timer > 50) ){
      start_timer = millis();
      high_voltage_control=false;
      low_voltage_control=true;  
    }
    delay(200);
    stop_timer = millis();
  }
    
  else{
    // If last whistle heard 2 sec ago, reset counter. ( 200ms delay )
    if(millis()-stop_timer > 2200){
      whistle_count=0;
    }

    // Otherwise, check the last heard sound once.
    if(low_voltage_control){ 
      check_rhythm(whistle_count);
      
      // Change control logic
      low_voltage_control=false;
      high_voltage_control=true;
      
      // After determine the whistle count. Check the number for change the light status
      if(whistle_count == 3){
        turn_light_status(light_status, led);
      }
    }
  }
}

boolean analogToDigital(int analogVoltage, int threshold){
  if(analogVoltage>threshold){
    return true;
  }
  else{
    return false;
  }
}

/* <----------------- TEST PHASE ----------------->
 boolean is_whistle(unsigned long start_time, unsigned long stop_time, int lowThresHold, int highThresHold) {
  // Check the difference between start and end time.
  if ( (stop_time - start_time < highThresHold) && (stop_time - start_time > lowThresHold) ){
    return true;
  }
  else{
    return false;
  }
} */

void check_rhythm(int whistle_counter){
  switch (whistle_counter) {
    case 0:  
      Serial.println("First Whistle!"); // FOR DEBUG
      whistle_count+=1;
      control_time = stop_timer;
      break;
    case 1:
      if( (millis() - control_time > 1000) && (millis() - control_time < 1800) ){
        Serial.println("Second Whistle Founded!"); // FOR DEBUG
        whistle_count+=1;
        control_time = stop_timer;
      }
      break;
    case 2:
      if( (millis() - control_time > 1000) && (millis() - control_time < 1800) ){
        Serial.println("Third Whistle Founded!"); // FOR DEBUG
        whistle_count+=1;
      }
      break;
  }
}

void turn_light_status(boolean light, int circuit_switch){
  if (!light){
    light_status = true;
    digitalWrite(circuit_switch, HIGH);
  }
  else{
    light_status = false;
    digitalWrite(circuit_switch, LOW);
  }
  whistle_count=0;
}

