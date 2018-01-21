// Pin Assignments For Sound Sensor And LED
int soundSensor = A0;
int led = 7;

boolean in_high_control=true;
boolean in_low_control=false;
boolean clap;
int clap_count = 0;

unsigned long control_time;
unsigned long start_timer;
unsigned long stop_timer;

boolean light_status = false;
boolean sound_perceived = false;

void setup() {
  Serial.begin(9600);
  pinMode(soundSensor, INPUT);
  pinMode(led, OUTPUT);
}

boolean is_clap(unsigned long start_time, unsigned long stop_time, int lowThresHold, int highThresHold) {
  // Check the difference between start and end time. 20-80 10-90
  Serial.println(stop_time-start_time);
  if (stop_time - start_time < highThresHold && stop_time - start_time > lowThresHold){
    return true;
  }
  else{
    return false;
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

void loop(){
  sound_perceived = analogToDigital(analogRead(soundSensor), 500);
  if(sound_perceived){
    // Check the sound for it is clap or not.
    if(high_voltage_control){
      start_timer = millis();
      high_voltage_control=false;
      low_voltage_control=true;  
    }
    // delay(100); If you use delay, you can delete controls
    stop_timer = millis();
  }  
  else{
    // If last whistle heard 2 sec ago, reset counter.
    if(millis()-stop_timer > 2000){
      clap_count=0;
    }
    if(low_voltage_control){
      whistle = is_whistle(start_timer, stop_timer); // Was That Sound a Whistle?
      if(whistle){
        
      }
      else{
        Serial.println("Not Whistle!");
      }
      in_low_control=false;
      in_high_control=true;
    }
    if(whistle_count==2){
      
    }
  }
}

void check_rhythm(){
  switch (whistle_count) {
    case 0:  
      // Serial.println("First Whistle!"); FOR DEBUG
      whistle_count+=1;
      control_time = stop_timer;
      break;
    case 1:
      if(millis() - control_time > 1000 && millis() - control_time < 1800){
        // Serial.println("Second Whistle Founded!"); FOR DEBUG
        clap_count+=1;
        control_time = millis();
      }
      break;
    case 2:
      if(millis() - control_time > 0 && millis() - control_time < 400){
        // Serial.println("Third Whistle Founded!"); FOR DEBUG
        whistle_count+=1;
        
      }
      break;
  }
}
void turn_light_status(boolean light_status, circuit_switch){
  if (!light_status){
    light_status = true;
    digitalWrite(circuit_switch, HIGH);
  }
  else{
    light_status = false;
    digitalWrite(circuit_switch, LOW);
  }
  clap_count=0;
}

