#define SERIAL_BAUDRATE 9600
#define CLK_PIN D2 // 定義連接腳位
#define DT_PIN  D3
#define SW_PIN  D4

#define interruptA 0

uint32_t t = 0;

uint8_t Stat,LastStat;

typedef struct {
		uint8_t LayerCtrl;
		uint8_t LayerItem[2];
    uint8_t Max,Min;
    uint8_t Show;
} _ScreenStat;

_ScreenStat ScreenStat;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);

  attachInterrupt(interruptA, rotaryEncoderChanged, CHANGE  );
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT); 
  pinMode(SW_PIN, INPUT); 

  LastStat  = digitalRead(CLK_PIN);

  ScreenStat.LayerCtrl    = 0;
  ScreenStat.LayerItem[0] = 0;
  ScreenStat.LayerItem[1] = 0;
  ScreenStat.Max          = 30;
  ScreenStat.Min          = 5;
  ScreenStat.Show         = 1;

}

void loop() 
{
  uint8_t Layer,Item;

  if(digitalRead(SW_PIN) == LOW)
  {
    Layer = (ScreenStat.LayerCtrl%2);
    ScreenStat.LayerCtrl = (Layer==0)?1:0;

    delay(300);
    ScreenStat.Show = 1;
  }

  if(ScreenStat.Show)
  {
    Layer = ScreenStat.LayerCtrl%2;
    Item  = ScreenStat.LayerItem[0]%2 ;
    if( Layer == 0)
    {
      if( Item == 0)
      {
          Serial.print  (" > Max = ");
          Serial.println(ScreenStat.Max);
          Serial.print  ("   Min = ");
          Serial.println(ScreenStat.Min);
      }
      else
      {
          Serial.print  ("   Max = ");
          Serial.println(ScreenStat.Max);
          Serial.print  (" > Min = ");
          Serial.println(ScreenStat.Min);
      }
      Serial.println("");
    }
    else //Layer 1
    {
      if( Item == 0 )                    //Set Max
      {
          Serial.print(" Set Max  = ");
          Serial.println(ScreenStat.Max);
      }
      else                               //Set Min
      {
          Serial.print(" Set Min  = ");
          Serial.println(ScreenStat.Min);
      }
    }
    ScreenStat.Show = 0;
  }

}
void rotaryEncoderChanged()
{
  uint8_t a,b;
  uint8_t Layer,Item;
  int8_t  cnt;

  unsigned long temp = millis();
  if(temp - t < 100) // 去彈跳
    return;
  t = temp;

  a = digitalRead(CLK_PIN);
  b = digitalRead(DT_PIN);
  cnt = (a!=b)?1:-1;

  Layer = ScreenStat.LayerCtrl%2;
  Item  = ScreenStat.LayerItem[0]%2;

  if(Layer == 0) //First Layer
  {
    ScreenStat.LayerItem[Layer]+=cnt;
  }
  else           //Second Layer
  {
    if(Item == 0)
    {
      ScreenStat.Max+=cnt;
    }
    else
    {
      ScreenStat.Min+=cnt;
    }
  }

  ScreenStat.Show = 1;
  //Serial.println(ScreenStat.LayerCtrl);
}
