#include <DHT.h>

//#define DHTPIN 2     // what digital pin we're connected to
#define DHTPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//define
const int mVoutPin = 0;
const static int mAnode[7] ={2, 3, 4, 5, 6 , 7, 8};
const static int mCathod[2] ={9,10};

int   mAnode_pin[7]  ={4, 3, 2, 1, 0, 5, 6 };
int   mCathod_pin[2]   ={0, 1   };

const int mMaxCathode=2;

int mCount=0;
int iMode=0;
int mMode_TMP =0;
int mMode_HUM =1;
static uint32_t mTimerTmp;

bool Num_Array[10][8]={
// a b c d e f g dp 
{ 1,1,1,1,1,1,0,0}, //0 :0
{ 0,1,1,0,0,0,0,0}, //1 :1
{ 1,1,0,1,1,0,1,0}, //2 :2
{ 1,1,1,1,0,0,1,0}, //3 :3
{ 0,1,1,0,0,1,1,0}, //4 :4
{ 1,0,1,1,0,1,1,0}, //5 :5
{ 1,0,1,1,1,1,1,0}, //6 :6
{ 1,1,1,0,0,0,0,0}, //7 :7
{ 1,1,1,1,1,1,1,0}, //8 :8
{ 1,1,1,1,0,1,1,0}, //9 :9   
};

//
void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("# Start-setup");
  //cathod
  for(int i= 0 ;i< 2;i++){
    pinMode(mCathod[i], OUTPUT);
  }
  for(int j=0 ;j<  7; j++){
    pinMode(mAnode[j], OUTPUT);
  }
}

//
void NumPrint(int Number){
  for (int i=0; i< 7; i++){
    int iTmp= mAnode_pin[i];
    if(Num_Array[Number][i]==1){
       digitalWrite(mAnode[iTmp ], HIGH);
//Serial.print(mAnode[iTmp ]);
    }
    delayMicroseconds( 500 );
    if(Num_Array[Number][i]==1){
       digitalWrite(mAnode[iTmp ], LOW);
//Serial.print("0");
    }
//Serial.println(" \n");    
  }
}

void set_cathod(int pinAnode){
  for(int i=0;i <mMaxCathode ; i++){
    if(mCathod[i]==pinAnode){
       //digitalWrite(mAnode[ i  ], HIGH);
       digitalWrite(mCathod[ i  ], LOW);
    }else{
       digitalWrite(mCathod[ i  ], HIGH);
//       digitalWrite(mAnode[ i  ], LOW );
    }
  }
}

/*
void proc_display(int iTemp){
  int iDeg_10= iTemp / 10;
  int iDeg_1 = iTemp % 10;
   for(int i=0;  i< 200; i++ ){
       set_cathod(mCathod[0]);
       NumPrint(iDeg_1);
       // dig-10
       set_cathod(mCathod[1]);
       NumPrint(iDeg_10);
   }
}
*/

void proc_display(int iTemp, int typMode){
  int iDeg_10= iTemp / 10;
  int iDeg_1 = iTemp % 10;
  if(typMode==mMode_TMP){
     for(int i=0;  i< 200; i++ ){
         set_cathod(mCathod[0]);
         NumPrint(iDeg_1);
         // dig-10
         set_cathod(mCathod[1]);
         NumPrint(iDeg_10);
     }
  }else{
       for(int j=0; j<3; j++){
	   for(int i=0;  i< 50; i++ ){
               set_cathod(mCathod[0]);
               NumPrint(iDeg_1);
               // dig-10
               set_cathod(mCathod[1]);
               NumPrint(iDeg_10);
	   }
	   delay(100);
	} 
  }
}
//
long convert_Map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//
void loop() {
  delay( 100 );
  int iNum=0;
  if (millis() > mTimerTmp) {
    mTimerTmp = millis()+ 3000;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
        return;
    }
    int itemp =(int)t;   
    int iHum = (int)h;   
    char sTmp[2+1];
    char sHum[3+1];
    sprintf(sTmp, "%2d", itemp);
Serial.print("dat="); 
Serial.print(sTmp);
//Serial.print("hum="); 
    sprintf(sHum, "%03d", iHum);
Serial.println( sHum );
    if(iMode== mMode_TMP){
      proc_display( itemp, mMode_TMP );
      iMode=mMode_HUM;
    }else{
      proc_display( iHum , mMode_HUM );
      iMode=mMode_TMP;
    }

    //proc_display( itemp );
    mCount=mCount+1;
  }

}



