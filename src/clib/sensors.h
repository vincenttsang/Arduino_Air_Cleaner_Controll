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

int getCO2(){
    int sensorValue,outputValue;
    sensorValue = analogRead(A0)*(5);
    outputValue = map(sensorValue, 0, 2000, 350, 10000);
    return outputValue;
} //MG811 读取并转换
char *getCharCO2(){
    char fromFloatCO2[3];
    dtostrf(getCO2(),1,1,fromFloatCO2);
    char *unit = " ppm";
    char *CO2Data = fromFloatCO2;
    char *CharCO2 = (char *) malloc(strlen(CO2Data) + strlen(unit));
    sprintf(CharCO2, "%s%s", CO2Data, unit);
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
    int Temp;
    sensor_DS18B20.requestTemperatures();
    Temp = sensor_DS18B20.getTempCByIndex(0);
    return Temp;
} //DS18B20 读取温度
char *getCharTemp(){
    char fromFloatTemp[3];
    dtostrf(getFloatTemp(),1,1,fromFloatTemp);
    char *unit = " Deg.C";
    char *TempData = fromFloatTemp;
    char *CharTemp = (char *) malloc(strlen(TempData) + strlen(unit));
    sprintf(CharTemp, "%s%s", TempData, unit);
    return CharTemp;
}//温度转换为char

int sv1 = 0;
int ov1 = 0;
int sv2 = 0;
int ov2 = 0;
int GetX(){
    sv1 = analogRead(A1);
    ov1 = map(sv1, 0, 1023, 0, 255);
    return ov1;
} //X轴角度
int GetY(){
    sv2 = analogRead(A2);
    ov2 = map(sv2, 0, 1023, 0, 255);
    return ov2;
} //Y轴角度
//ADXL335 角度读取
