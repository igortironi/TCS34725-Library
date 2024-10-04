#ifndef TCS34725_H
#define TCS34725_H

#include <Wire.h>
#include <Arduino.h>


class TCS34725{

    private:
        //internal register defines:
        #define ADDR 0X29
        
        #define ENABLE 0X00
        #define ATIME 0X01
        #define WTIME 0X03
        
        #define AILTL 0X04
        #define AILTH 0X05
        #define AIHTL 0X06
        #define AIHTH 0X07
        #define PERS 0X0C
        
        #define CONFIG 0X0D
        #define CONTROL 0X0F
 
        #define DATA_START 0X14

        struct _raw{
            uint16_t R, G, B, C;
        };
        
        struct _rgb{
            uint8_t R, G, B;
        };
        
        struct _hsv{
            unsigned int H;
            float S, V;
        };

        uint16_t _max_count = 1024;
        bool _colour_convertion = false;



        //communication commands
        int8_t send_data(uint8_t addr, uint8_t data);
        int8_t request_data(uint8_t **data, uint8_t addr, uint8_t lenght);


        //send_data overload, send multiple data packges to the sensor
        int8_t send_data(uint8_t addr, uint8_t data[], uint8_t lenght);


    public:

        #define GAIN_1X 0X00
        #define GAIN_4X 0X01
        #define GAIN_16X 0X02
        #define GAIN_60X 0X03

        //struct to store the sensor data
        _raw RAW;
        _rgb RGB;
        _hsv HSV;

        //initialize sensor
        int8_t begin();

        //set integration time in ms
        int8_t set_integration_time(int integration_time);

        //set gain
        int8_t set_gain(uint8_t gain);

        //reads the sensor and stores the data in the struct colour
        int8_t read();

        //turn on and off colour convertion to RGB and HSV
        void colour_convertion(bool state);
        
        //set maximum and minimum threshold to the sensor clear interrupt
        int8_t set_interrupt_threshold(uint16_t low, uint16_t high);

        //turn on interrupt
        int8_t turn_interrupt_on(uint8_t state);

        //after a interrupt is triggered, this command resets it's state
        int8_t clear_interrupt();

        //persistence filter [0:15]
        //0 -> every cycle generates an interrupt
        //1 - 3 -> number of persistence needed [1:3]
        //4 - 15 -> going 5 by 5 from 5 to 60 [:60]
        int8_t set_persistence_filter(uint8_t value);


};


#endif
