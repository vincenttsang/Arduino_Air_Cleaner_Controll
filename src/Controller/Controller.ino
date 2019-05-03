#include "U8glib.h"
#include "clib/sensors.h"
#include "clib/MemoryFree.cpp"

U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);  

void(* resetFunc) (void) = 0;

void drawStatic(void){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 22, "Temperature: ");
  u8g.drawStr( 12, 22, "CO2:");
  u8g.drawStr( 24, 22, "X-axis:");
  u8g.drawStr( 36, 22, "Y-axis:");
  }
  
void draw(void) {
  float X=0,Y=0;
  X=getX()*1.0;
  Y=getY()*1.0;
  delay(15);
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 50, getCharTemp());
  u8g.drawStr( 12, 50, getCharCO2());
  u8g.drawStr( 24, 50, (floatToChar(X,1)," Deg"));
  u8g.drawStr( 36, 50, (floatToChar(Y,1)," Deg"));
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
