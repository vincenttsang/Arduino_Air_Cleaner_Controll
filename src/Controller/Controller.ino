#include "U8glib.h"
#include "clib/sensors.h"
#include "clib/MemoryFree.cpp"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);  

void(* resetFunc) (void) = 0;

void drawStatic(void){
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(0, 20); 
  u8g.print("Temperature: ");
  u8g.setPrintPos(0,50);
  u8g.print("CO2: ");
  u8g.setPrintPos(0,80);
  u8g.print("X-axis:");
  u8g.setPrintPos(0,100);
  u8g.print("Y-axis:");
  }
  
void draw(void) {
  float X=0,Y=0;
  X=getX()*1.0;
  Y=getY()*1.0;
  delay(15);
  u8g.setFont(u8g_font_unifont);
  u8g.setPrintPos(30, 35);
  u8g.print(getCharTemp());
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
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  if(freeMemory()<=250){
    delay(20);
    resetFunc(); 
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initTemp();
}

void loop() {
  // put your main code here, to run repeatedly:
   u8g.firstPage();
   do {
    drawStatic();
    draw();
  } while( u8g.nextPage() );
   delay(800);
}
