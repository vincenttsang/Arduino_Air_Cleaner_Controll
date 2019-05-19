#include "U8glib.h"
#include "clib/sensors.h"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);
int islean = 0;
int isboilon = 0;
int isboilfinish = 0;

void(* resetFunc) (void) = 0;

void drawStatic(void) {
  u8g.setFont(u8g_font_unifont);
  //  delay(15);
  //  u8g.setPrintPos(0, 20);
  //  u8g.print("Temperature: ");
  //  delay(15);
  //  u8g.setPrintPos(0, 50);
  //  u8g.print("CO2: ");
  //  delay(15);
  //  u8g.setPrintPos(0, 80);
  //  u8g.print("X-axis:");
  //  delay(15);
  //  u8g.setPrintPos(0, 100);
  //  u8g.print("Y-axis:");

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
  //char *Temp = getCharTemp(CharTemp);
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(30, 35);
  u8g.print(getFloatTemp());
  delay(15);
  u8g.setPrintPos(55, 50);
  u8g.print(getCharCO2());
  delay(15);
  u8g.setPrintPos(55, 80);
  u8g.print(charLinker(floatToChar(420.0 - X, 1), " Deg"));
  delay(15);
  u8g.setPrintPos(55, 100);
  u8g.print(charLinker(floatToChar(420.0 - Y, 1), " Deg"));
  delay(15);
  Serial.print("freeMemory() = ");
  Serial.println(freeMemory());
  Serial.print("CO2 ppm = ");
  Serial.println(getCO2());
  Serial.print("Temperature = ");
  Serial.println(getFloatTemp());
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
  pinMode(7, OUTPUT);//7进水水泵
  pinMode(8, OUTPUT);//8出水水泵
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
  if (analogRead(1)<290 or analogRead(1)>370) islean = 1;
  if (analogRead(2)<290 or analogRead(2)>370) islean = 1;
  // 判断是否倾斜
  //  if (isboilon == 1) {
  //    boilprogram();
  //  } else {
  //    if (islean == 0) {  //不倾斜显示数据
  //      digitalWrite(5, HIGH);
  //      digitalWrite(6, HIGH);
  //      digitalWrite(9, HIGH);
  //      u8g.firstPage();
  //      do {
  //        drawStatic();
  //        draw();
  //      } while ( u8g.nextPage() );
  //      u8g_Delay(10);
  //      delay(440);
  //    } else { //倾斜显示warning
  //      do {
  //        leaningwarning();
  //      } while ( u8g.nextPage() );
  //      Serial.println("leaning warning!");
  //    }
  //  }
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
      } else {
        //倾斜显示warning
        leaningwarning();
        Serial.println("leaning warning!");
      }
    } while ( u8g.nextPage() );
    u8g_Delay(100);
    // delay(440);
  }

  //operating(testing) panel内容
  //if (digitalRead(9) == HIGH) isboilon = 1;
  //  if (digitalRead(10) == HIGH) {
  //    digitalWrite(7, HIGH);
  //  } else {
  //    digitalWrite(7, LOW);
  //  }
  //  if (digitalRead(11) == HIGH) {
  //    digitalWrite(8, HIGH);
  //  } else {
  //    digitalWrite(8, LOW);
  //  }
}
