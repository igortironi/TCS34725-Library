#define LED_PIN 2   
#include "library.hpp"

TCS34725 Color;


void setup(){
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(3, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(3), interrupt, FALLING);


    Color.begin();
    Color.set_gain(GAIN_60X);
    Color.set_integration_time(1000);

    Color.colour_convertion(true);
    Color.set_interrupt_threshold(0, 1000);
    Color.set_persistence_filter(1);
    Color.turn_interrupt_on(false);


}


void loop(){
    static int count = 0;
    Color.read();

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
        Color.clear_interrupt();
        Serial.println("clear");
        digitalWrite(LED_PIN, HIGH);
    }

    delay(500);
}

void interrupt(){
    digitalWrite(LED_PIN, LOW);
}