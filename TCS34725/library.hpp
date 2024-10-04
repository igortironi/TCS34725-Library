#ifndef TCS34725_H
#define TCS34725_H

#include <Wire.h>


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




        //communication commands
        int8_t send_data(uint8_t addr, uint8_t data);
        int8_t request_data(uint8_t *data, uint8_t addr, uint8_t lenght);


    public:
        int8_t begin();


};

//send data to sensor
int8_t TCS34725::send_data(uint8_t addr, uint8_t data){
    //prepare register addres to be sended, as described in the datasheet
    addr = (addr & 0X1F) | 0XA0;

    Wire.beginTransmission(ADDR);
    Wire.write(addr);
    Wire.write(data);

    return Wire.endTransmission();
}

//resquests data from sensor
int8_t TCS34725::request_data(uint8_t *data, uint8_t addr, uint8_t lenght){
    addr = (addr & 0X1F) | 0XA0;

    //select register to get data and check for errors
    Wire.beginTransmission(ADDR);
    Wire.write(addr);
    int8_t return_status = Wire.endTransmission(false);

    if(!return_status) return return_status;

    //request lenght data and checks if all data was received
    return_status = Wire.requestFrom(ADDR, lenght);
    if(return_status != lenght) return -1;

    //put received data on the data pointer, stepping lenght addresses
    for(uint8_t i = 0 ; i < lenght ; i++){
        *(data + i) = Wire.read();
    }

    return 0;
}



//initialize dependencies and sensor
int8_t TCS34725::begin(){
    Wire.begin();
}






#endif