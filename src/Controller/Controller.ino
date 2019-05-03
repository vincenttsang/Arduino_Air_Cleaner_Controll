#include "U8glib.h"
#include "clib/sensors.h"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);  

void(* resetFunc) (void) = 0;

void drawStatic(void){
  u8g.setFont(u8g_font_unifont);
  delay(15);
  u8g.setPrintPos(0, 20); 
  u8g.print("Temperature: ");
  delay(15);
  u8g.setPrintPos(0,50);
  u8g.print("CO2: ");
  delay(15);
  u8g.setPrintPos(0,80);
  u8g.print("X-axis:");
  delay(15);
  u8g.setPrintPos(0,100);
  u8g.print("Y-axis:");
  }
  
void draw(void) {
  float X=0,Y=0;
  X=getX()*1.0;
  Y=getY()*1.0;
  char *Temp = getCharTemp(CharTemp);
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(30, 35);
  u8g.print(Temp);
  delay(15);
  u8g.setPrintPos(55,50);
  u8g.print(getCharCO2());
  delay(15);
  u8g.setPrintPos(55,80);
  u8g.print(charLinker(floatToChar(X,1)," Deg"));
  delay(15);
  u8g.setPrintPos(55,100);
  u8g.print(charLinker(floatToChar(Y,1)," Deg"));
  delay(15);
  Serial.print("freeMemory() = ");
  Serial.println(freeMemory());
  Serial.print("CO2 ppm = ");
  Serial.println(getCO2());
  Serial.print("Temperature = ");
  Serial.println(Temp);
  Mem();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  initTemp();
}

void loop() {
  // put your main code here, to run repeatedly:
   u8g.firstPage();
   do {
    drawStatic();
    draw();
  } while( u8g.nextPage() );
  u8g_Delay(10);
}
