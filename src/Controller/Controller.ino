#include "U8glib.h"
#include "clib/sensors.h"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);
int islean = 0;

void(* resetFunc) (void) = 0;

void drawStatic(void) {
  u8g.setFont(u8g_font_unifont);
  delay(15);
  u8g.drawStr(0, 20, "Temperature: ");
  delay(15);
  u8g.drawStr(0, 50, "CO2: ");
  delay(15);
  u8g.drawStr(0, 80, "Y-axis:");
  delay(15);
  u8g.drawStr(0, 100, "Y-axis:");
}

void draw(void) {
  float X = 0, Y = 0;
  X = getX() * 1.0;
  Y = getY() * 1.0;
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(30, 35);
  u8g.print(Temp);
  delay(15);
  u8g.setPrintPos(55, 50);
  u8g.print(getCharCO2());
  delay(15);
  u8g.setPrintPos(55, 80);
  u8g.print(charLinker(floatToChar(X, 1), " Deg"));
  delay(15);
  u8g.setPrintPos(55, 100);
  u8g.print(charLinker(floatToChar(Y, 1), " Deg"));
  delay(15);
  Serial.print("freeMemory() = ");
  Serial.println(freeMemory());
  Serial.print("CO2 ppm = ");
  Serial.println(getCO2());
  Serial.print("Temperature = ");
  Serial.println(getFloatTemp());
  Mem();
}

void leaningwarning(void){
    islean = 0;
    //if (getX() * 1.0 > 10.0) islean = 1;
    //if (getY() * 1.0 > 10.0) islean = 1;
    // 判断是否倾斜
    digitalWrite(6, LOW);
    digitalWrite(9, LOW);
      if (islean == 0) {  //不倾斜显示数据
    digitalWrite(6, HIGH);
    digitalWrite(9, HIGH);
    drawStatic();
    draw();
  } else { //倾斜显示warning
    u8g.drawRFrame(8, 15, 112, 34, 5);
    u8g.drawStr(10, 17, "WARNING");
    u8g.drawStr(10, 65, "Device");
    u8g.drawStr(10, 70, "Leaning.");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  initTemp();
  pinMode(6, OUTPUT);//6二氧化碳电源
  pinMode(7, OUTPUT);//7进水水泵
  pinMode(8, OUTPUT);//8出水水泵
  pinMode(9, OUTPUT);//9气泵
  digitalWrite(6, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);

  pinMode(9, INPUT); //9 开始加热开关
  pinMode(10, INPUT);//10进水水泵开关
  pinMode(11, INPUT);//11出水水泵开关

}

void loop() {
  // put your main code here, to run repeatedly:
  u8g.firstPage();
    do {
      leaningwarning();
    } while ( u8g.nextPage() );
    u8g_Delay(10);
      //testing panel内容
  if (digitalRead(10) == HIGH) digitalWrite(7, HIGH);
  if (digitalRead(11) == HIGH) digitalWrite(8, HIGH);
}
