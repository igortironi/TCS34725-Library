#include "TCS34725.h"


//send data to sensor
int8_t TCS34725::send_data(uint8_t addr, uint8_t data){
    //prepare register addres to be sended, as described in the datasheet
    addr = (addr & 0X1F) | 0XA0;

    Wire.beginTransmission(ADDR);
    Wire.write(addr);
    Wire.write(data);

    return Wire.endTransmission();
}


//send multiple data packges to the sensor
int8_t TCS34725::send_data(uint8_t addr, uint8_t data[], uint8_t lenght){
    //prepare register addres to be sended, as described in the datasheet
    addr = (addr & 0X1F) | 0XA0;

    Wire.beginTransmission(ADDR);
    Wire.write(addr);
    for(uint8_t i = 0 ; i < lenght ; i++){
        Wire.write(data[i]);
    }
    return Wire.endTransmission();
}


//resquests data from sensor
int8_t TCS34725::request_data(uint8_t **data, uint8_t addr, uint8_t lenght){
    addr = (addr & 0X1F) | 0XA0;

    //select register to get data and check for errors
    Wire.beginTransmission(ADDR);
    Wire.write(addr);
    int8_t return_status = Wire.endTransmission(false);

    if(return_status) return return_status;

    //request lenght data and checks if all data was received
    return_status = Wire.requestFrom((uint8_t)ADDR, lenght);
    if(return_status != lenght) return -1;

    //put received data on the data pointer, stepping lenght addresses
    for(uint8_t i = 0 ; i < lenght ; i++){
        **(data + i) = Wire.read();
    }

    return 0;
}



//initialize dependencies and sensor
int8_t TCS34725::begin(){
    Wire.begin();

    //activate sensor
    return send_data(ENABLE, 0X03);
}


//set integration time in ms
int8_t TCS34725::set_integration_time(int integration_time){
    //calc register value to aproximate integration time
    integration_time = 255 - (int)((float)integration_time / 2.4);
    unsigned long buff = (256 - (unsigned long)integration_time) * 1024;
    
    if(buff > 65535) buff = 65535;


    _max_count = (uint16_t)buff;

    if(integration_time > 255)integration_time = 255;
    else if(integration_time < 0)integration_time = 0;

    return send_data(ATIME, (uint8_t)integration_time);
}


//set gain
int8_t TCS34725::set_gain(uint8_t gain){
    //checks if desired gain is in range
    if(gain > 3)return -1;

    return send_data(CONTROL, gain);
}



//reads the sensor and stores the data in the struct colour
int8_t TCS34725::read(){

    //creates a pointer vector to facilitate data aquisition
    uint8_t *data_array[8] = {(uint8_t*)&RAW.C, (uint8_t*)&RAW.C + 1, 
    (uint8_t*)&RAW.R, (uint8_t*)&RAW.R + 1, 
    (uint8_t*)&RAW.G, (uint8_t*)&RAW.G + 1, 
    (uint8_t*)&RAW.B, (uint8_t*)&RAW.B + 1};

    //pass vector pointer to receive data
    int8_t return_status = request_data(data_array, DATA_START, 8);
    if(return_status) return return_status;

    //checks if the colou convertion option is set
    if(_colour_convertion){

        //convertion to RGB [0:255]
        RGB.R = (uint8_t)(((float)RAW.R / (float)_max_count) * 255.0);
        RGB.G = (uint8_t)(((float)RAW.G / (float)_max_count) * 255.0);
        RGB.B = (uint8_t)(((float)RAW.B / (float)_max_count) * 255.0);


        //convertion to HSV
        uint8_t max_v;
        uint8_t select;

        if(RGB.R >= RGB.G && RGB.R >= RGB.B){
            max_v = RGB.R;
            select = 0;
        }

        else if(RGB.G >= RGB.R && RGB.G >= RGB.B){
            max_v = RGB.G;
            select = 1;
        } 
        
        else{
            max_v = RGB.B;
            select = 2;
        }

        uint8_t min_v = min(RGB.R, RGB.G);
        min_v = min(min_v, RGB.B);

        float delta_v = (float)(max_v - min_v);

        HSV.V = (float)max_v / 255.0;
        if(max_v != 0){
            HSV.S = delta_v / ((float)max_v);
        }
        else HSV.S = 0.0;

        float h_buff;
        

        switch (select){
            case 0:
                h_buff = (((float)(RGB.G - RGB.B)) / delta_v);
                break;
            case 1:
                h_buff = (((float)(RGB.R - RGB.B)) / delta_v) + 2;
                break;
            case 2:
                h_buff = (((float)(RGB.G - RGB.R)) / delta_v) + 4;
                break;
        }

        h_buff *= 60.0;

        if(h_buff < 0)h_buff += 360.0;

        HSV.H = (unsigned int)h_buff;
    }


    return 0;
}

//turn on and off colour convertion to RGB and HSV
void TCS34725::colour_convertion(bool state){
    _colour_convertion = state;
}


int8_t TCS34725::set_interrupt_threshold(uint16_t low, uint16_t high){
    //load the data in the correct order in a array
    uint8_t data[4] = {
        (uint8_t)low,
        (uint8_t)(low>>8),
        (uint8_t)high,
        (uint8_t)(high>>8)
    };

    //send the data using a overload function
    return send_data(AILTL, data, 4);
}

//turns interrups on
int8_t TCS34725::turn_interrupt_on(uint8_t state){
    return(send_data(ENABLE, (uint8_t)(((state<<4) & 0X10) | 0X03)));
}


 //after a interrupt is triggered, this command resets it's state
int8_t TCS34725::clear_interrupt(){
    //this function needs to use another pattern of communication
    //so, data send must be rewrite exclusively to this end

    Wire.beginTransmission(ADDR);
    //clear the interruption
    Wire.write(0XE6);

    return Wire.endTransmission();
}

//persistence filter [0:15]
//0 -> every cycle generates an interrupt
//1 - 3 -> number of persistence needed [1:3]
//4 - 15 -> going 5 by 5 from 5 to 60 [:60]
int8_t TCS34725::set_persistence_filter(uint8_t value){
    if(value > 15)return -1;

    return send_data(PERS, value);
}




