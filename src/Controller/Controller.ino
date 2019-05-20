#include "U8glib.h"
#include "clib/sensors.h"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);
int islean = 0;
int isboilon = 0;
int isboilfinish = 0;
int Laser = 0;
float X = 0, Y = 0;

void(* resetFunc) (void) = 0;

void drawStatic(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, 20, "Temperature: ");
  u8g.drawStr(0, 50, "CO2: ");
  u8g.drawStr(0, 80, "X-axis:");
  u8g.drawStr(0, 95, "Y-axis:");
  u8g.drawStr(0, 110, "Light:");
}

void draw(void) {
  X = getX() * 1.0;
  Y = getY() * 1.0;
  Laser = getLaser();
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(30, 35);
  u8g.print(getFloatTemp());
  u8g.setPrintPos(55, 50);
  u8g.print(getCharCO2());
  u8g.setPrintPos(55, 80);
  u8g.print(charLinker(floatToChar(520.0 - X/5, 1), " Deg"));
  u8g.setPrintPos(55, 95);
  u8g.print(charLinker(floatToChar(520.0 - Y/5, 1), " Deg"));
  u8g.setPrintPos(55, 110);
  u8g.print(Laser);
  delay(50);
  Serial.print("剩余内存 =  ");
  Serial.println(freeMemory());
  Serial.print("CO2 ppm = ");
  Serial.println(getCO2());
  Serial.print("温度 = ");
  Serial.println(getFloatTemp());
  Serial.print("小球藻浓度 = ");
  Serial.println(Laser);
  Mem();
}

void leaningwarning(void) {
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);
  digitalWrite(5, LOW);

  u8g.setFont(u8g_font_unifont);
  u8g.drawRFrame(8, 15, 112, 34, 5);
  u8g.drawStr(30, 30, "WARNING");
  u8g.drawStr(10, 65, "Device");
  u8g.drawStr(10, 80, "Leaning.");
}

void boilprogram(void) {
  digitalWrite(4, HIGH);
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(10, 10, "Boiling.");
  u8g.drawStr(10, 25, "Temperature:");
  u8g.setPrintPos(30, 40);
  u8g.print(getFloatTemp());
  if (getFloatTemp() > 96.00) {
    isboilfinish = 1;
    digitalWrite(4, LOW);
  }
  if (isboilfinish == 1) {
    digitalWrite(8, HIGH);
    delay(5000);
    digitalWrite(8, LOW);
    delay(2000);
    digitalWrite(7, HIGH);
    delay(5000);
    digitalWrite(7, LOW);
    isboilfinish = 0;
    isboilon = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  initTemp();
  pinMode(4, OUTPUT);//4发热丝
  pinMode(5, OUTPUT);//5灯环
  pinMode(6, OUTPUT);//6二氧化碳电源
  pinMode(7, INPUT);//7进水水泵
  pinMode(8, INPUT);//8出水水泵
  pinMode(9, OUTPUT);//9气泵

  pinMode(12, INPUT); //12 开始加热开关
  pinMode(10, INPUT);//10进水水泵开关
  pinMode(11, INPUT);//11出水水泵开关

  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

  islean = 0;
  if (analogRead(1)<=390 or analogRead(1)>=470) islean = 1;
  if (analogRead(2)<=390 or analogRead(2)>=470) islean = 1;
  if (isboilon == 1) {
    boilprogram();
  } else {
    if (islean == 0) {
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(9, HIGH);
    }
    u8g.firstPage();
    do {
      if (islean == 0) { //不倾斜显示数据
          drawStatic();
          draw();
          Serial.println(analogRead(1));
          Serial.println(analogRead(2));
          }
      else{//倾斜显示warning
        leaningwarning();
        Serial.println("leaning warning!");
        Serial.println(analogRead(1));
        Serial.println(analogRead(2));
      }
    } while ( u8g.nextPage() );
    u8g_Delay(100);
  }
}
