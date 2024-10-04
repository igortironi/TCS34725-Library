#include <TCS34725.h>

TCS34725 Color;


void setup(){
    Serial.begin(115200);

    //initialize sensor
    Color.begin();
    
    //set gain 
    //GAIN_1X
    //GAIN_4X
    //GAIN_16X
    //GAIN_60X
    //
    Color.set_gain(GAIN_60X);
    
    //set integration time in milliseconds [0ms : 700ms]
    Color.set_integration_time(100);

    //enable the RAW data conversion to RGB and HSV
    Color.colour_convertion(true);

    //config threshold for the interruption
    Color.set_interrupt_threshold(0, 1000);
    
    //config persistence filter for the interrupt [0 : 15]
    //0 -> every cycle generates an interrupt
    //1 - 3 -> number of persistence needed [1 : 3]
    //4 - 15 -> going 5 by 5 from 5 to 60 [5 : 60]
    Color.set_persistence_filter(1);
    
    //set to true to turn on the interruption
    Color.turn_interrupt_on(false);


}


void loop(){
    static int count = 0;
    //read sensor
    Color.read();
    
    //show all data
    Serial.print("RAW C: ");
    Serial.print(Color.RAW.C);
    Serial.print(" - int: ");
    Serial.print(digitalRead(3));
    Serial.print(" - RAW R: ");
    Serial.print(Color.RAW.R);
    Serial.print(" - RAW G: ");
    Serial.print(Color.RAW.G);
    Serial.print(" - RAW B: ");
    Serial.print(Color.RAW.B);
    Serial.print(" - R: ");
    Serial.print(Color.RGB.R);
    Serial.print(" - G: ");
    Serial.print(Color.RGB.G);
    Serial.print(" - B: ");
    Serial.print(Color.RGB.B);
    Serial.print(" - H: ");
    Serial.print(Color.HSV.H);
    Serial.print(" - S: ");
    Serial.print(Color.HSV.S);
    Serial.print(" - V: ");
    Serial.println(Color.HSV.V);

    if(!digitalRead(3))count++;
    if(count > 5){
        count = 0;
        //clear the interruption after it triggers
        Color.clear_interrupt();
        Serial.println("clear");
    }

    delay(500);
}


