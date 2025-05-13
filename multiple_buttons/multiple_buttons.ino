#define ANALOG_BUTTON_PIN 5

int mode, value, index_value = 0;
/*
** This program allows you to read multiple buttons from a single analog pin

== circuit ==

         Analog pin 5
            |
Ground--1K--|--------|--------|-------|
            |        |        |       |
           btn1     btn2     btn3    btn4 
            |        |        |       |    
         220 Ohm  330 Ohm  560 Ohm   1.0K 
            |--------|--------|-------|----- +5V


  NOTE: if you whant to use different resistence values you have to calibrate it agan
*/

void setup() {
  // put your setup code here, to run once:
  pinMode(ANALOG_BUTTON_PIN, INPUT);
  Serial.begin(9600);
	while(!Serial);
	Serial.println("[ INFO ] successful serial connection");

}

void loop() 
  mode = analogRead(ANALOG_BUTTON_PIN);
  //Serial.println(mode);
  value = mode * 5 / 301.5;
  
  if(value != 0 && value != index_value){
    
    /*
    Serial.print(value);
    Serial.print(", ");
    Serial.print(mode);
    Serial.print(", ");
    Serial.println(index_value);
    */
    switch (value){
      case 1:
        Serial.println("Button 1 pressed!\n");
        index_value = value;
        break;

      case 2:
        Serial.println("Button 2 pressed!\n");
        index_value = value;
        break;
      
      case 3:
        Serial.println("Button 3 pressed!\n");
        index_value = value;
        break;
      
      case 5:
        Serial.println("Button 4 pressed!\n");
        index_value = value;
        break;

      default:
        break;
    }
    delay(300);
  }
}
