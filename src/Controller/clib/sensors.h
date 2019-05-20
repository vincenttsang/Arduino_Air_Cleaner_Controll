//
//  sensors.h
//  
//
//  Created by Vincent Tsang on 2/5/2019.
//

#ifndef sensors_h
#define sensors_h


#endif /* sensors_h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MemoryFree.cpp"

char *CharTemp = NULL;
float Temp;

int getLaser(){
    int sensorValue,outputValue;
    sensorValue = analogRead(A3);
    outputValue = map(sensorValue, 0, 1023, 0, 1000);
    return outputValue;
}

int getCO2(){
    int sensorValue,outputValue;
    sensorValue = analogRead(A0)*(5);
    outputValue = map(sensorValue, 0, 2000, 350, 10000);
    return outputValue;
} //MG811 读取并转换

char *getCharCO2(){
    char fromFloatCO2[3];
    dtostrf(getCO2(),1,1,fromFloatCO2);
    char *CO2Data = fromFloatCO2;
    const char *unit = " ppm";
    char *CharCO2 = strcat(CO2Data,unit);
    return CharCO2;
} //转为char 带单位

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 2
OneWire oneWire(ONE_WIRE_BUS_1);
DallasTemperature sensor_DS18B20(&oneWire);
void initTemp(){
    sensor_DS18B20.begin();
} //初始化OneWire

float getFloatTemp(){
    sensor_DS18B20.requestTemperatures();
    Temp = sensor_DS18B20.getTempCByIndex(0);
    if (Temp>0.0) return Temp;
    if (Temp== -127.0) return 25.5;
    
} //DS18B20 读取温度

char *getCharTemp(char *a){
    a = NULL;
    char fromFloatTemp[4];
    fromFloatTemp[3] = '\0';
    dtostrf(getFloatTemp(),1,1,fromFloatTemp);
    const char *unit = " Deg.C";
    char *TempData = fromFloatTemp;
    a = strcat(TempData,unit);
    return a;
}//温度转换为char

int sv1 = 0;
int ov1 = 0;
int sv2 = 0;
int ov2 = 0;
int getX(){
    sv1 = analogRead(A1);
    //ov1 = map(sv1, 0, 1023, 0, 255);
    return sv1;
} //X轴角度

int getY(){
    sv2 = analogRead(A2);
    //ov2 = map(sv2, 0, 1023, 0, 255);
    return sv2;
} //Y轴角度

//ADXL335 角度读取

char fromFloatf[32];

char *floatToChar(float f ,int precision){
    dtostrf(f,1,precision,fromFloatf);
    return fromFloatf;
}//浮点转换为字符数组

char *charLinker(const char *a,const char *b){
    char *CharLink = strcat(a,b);
    return CharLink;
}//字符串拼接

void Mem(){
    void(* resetFunc) (void) = 0;
    if(freeMemory()<=250){
        delay(20);
        resetFunc();
    }
}
