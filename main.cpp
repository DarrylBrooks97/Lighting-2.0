#include <LiquidCrystal.h>
#include <IRremote.h>
#define Red 5
#define Green 4
#define Blue 3
#define receiver 15
int option = 0;
int enter = 0;
int i,q,f = 0;
int old_option = 0;
int pause=300;
int brightness=0;
long previousTime,previousTimeone;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
IRrecv irrecv(receiver);
decode_results results;
byte p20[8] =
{ B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] =
{ B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};

byte p60[8] =
{ B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] =
{ B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};

byte p100[8] =
{ B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
void setup()
{
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(receiver, INPUT);
  lcd.begin(16, 2);
  lcd.createChar(0,p20);
  lcd.createChar(1,p40);
  lcd.createChar(2,p60);
  lcd.createChar(3,p80);
  lcd.createChar(4,p100);
  Serial.begin(9600);
  irrecv.enableIRIn();

}

void Startup()
{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LIGHTING");
  delay(500);
  lcd.setCursor(10, 1);
  lcd.print("2.0");
  delay(3000);
  lcd.clear();
}
void loadingBar()
{
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      lcd.setCursor(i, 1);
      lcd.write(j);
      if (enter == 0)
        break;
      delay(50);

    }
  }

}

void inputReceived()
{
  switch (results.value)
  {
    case 0xFF22DD:   //Prev
      option--;
      old_option = option;
      if (option < 0)
      {
        option = 5;
        old_option = 0;
      }

      break;


    case 0xFF02FD:  //Next
      option++;
      if (option > 5)
        option = 0;
      break;

    case 0xFFC23D:  //Play-Pause
      if (enter == 1)
      {
        enter = 0;
      }
      else
        enter = 1;

      break;

     case 0xFFE01F:  //Vol -
     if(brightness>=255)
     {
     brightness=255;
     }
     else
     brightness+=85;
     break;

     case 0xFFA857:  //Vol +
     if(brightness<=0)
     {
      brightness=0;
     }
     else
     brightness-=85;
     break;

    default:
      Serial.print("Unknown Button:");
      Serial.print(results.value, HEX);
      Serial.print("\n");

  }
  delay(135);
}

void LEDambient()
{
  int RED = 255,
      BLUE = 0,
      GREEN = 255;

  for (/*code*/; RED > 0, BLUE < 255; RED--, BLUE++)
  {
    analogWrite(Red, RED);
    analogWrite(Blue, BLUE);
    delay(10);
        
    if (irrecv.decode(&results))
    {
      inputReceived();
      irrecv.resume();
      //loadingBar();
      if (enter == 0)
        break;
    }
  }

  for (/*code*/; GREEN >= 0, RED < 255; GREEN--, RED++)
  {
    analogWrite(Green, GREEN);
    analogWrite(Red, RED);
    delay(10);
//    loadingBar();
    if (irrecv.decode(&results))
    {
      inputReceived();
      irrecv.resume();
      if (enter == 0)
        break;
    }
  }

  for (/*code*/; BLUE >= 0, GREEN < 255; GREEN++, BLUE--)
  {
    analogWrite(Green, GREEN);
    analogWrite(Blue, BLUE);
    delay(10);
//    loadingBar();
    if (irrecv.decode(&results))
    {
      inputReceived();
      irrecv.resume();
      if (enter == 0)
        break;
    }
  }


}

void ambient()
{
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Ambient");

  if (enter == 1)
  {
    while (enter == 1)
    {
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Ambient");
      LEDambient();
      delay(100);
    }
  }
}

void strobe()
{
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Strobe");
  if (enter == 1)
  {
    while(enter==1)
    {
      
      
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Strobe");

    long currentTimeone=millis();
  
    if(currentTimeone-previousTime>=pause)
     {
       previousTimeone=currentTimeone;
       if(f==0)
       {
       f=255;
       analogWrite(Red,f);
       analogWrite(Blue,f);
       analogWrite(Green,f);
       }
       else
       {
       f=0;
       analogWrite(Red,f);
       analogWrite(Blue,f);
       analogWrite(Green,f);
       }
       
       delay(350);
       
       }
     if (irrecv.decode(&results))
      {
        inputReceived();
        irrecv.resume();
        if(enter==0)
        break;
      }
     
    }
  }
}


void warning()
{

  for (int b = 15; b = 0; b--)
  {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("MENU");
    lcd.setCursor(b, 1);
    lcd.print(">Warning");
    delay(200);
    if (b == 2)
      break;
  }

  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Warning");
  if (enter == 1)
  {
    while(enter==1)
    {
long currentTime=millis();
     lcd.clear();
     lcd.setCursor(4,0);
     lcd.print("Warning");
  
    if(currentTime-previousTime>=pause)
     {
       previousTime=currentTime;
       if(q==0)
       {
       q=255;
       analogWrite(Red,q);
       analogWrite(Blue,0);
       analogWrite(Green,255);
       }
       else
       {
       q=0;
       analogWrite(Red,q);
       analogWrite(Blue,255);
       analogWrite(Green,255);
       }
       }
       delay(250);
    if (irrecv.decode(&results))
      {
        inputReceived();
        irrecv.resume();
        if(enter==0)
        break;
      }
      
    }
  }
}


void red()
{
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Red");
  if (enter == 1)
  {
    while(enter==1)
    {
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("Red");
      
      analogWrite(Red,brightness);
      analogWrite(Green,255);
      analogWrite(Blue,255);
    
    if (irrecv.decode(&results))
          {
            inputReceived();
            irrecv.resume();
            if(enter==0)
            break;
          }
        delay(200);
    }

  }
}

void green()
{
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Green");
  if (enter == 1)
  {
     while(enter==1)
    {
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("Green");
      
      analogWrite(Red,255);
      analogWrite(Green,brightness);
      analogWrite(Blue,255);
    
    if (irrecv.decode(&results))
          {
            inputReceived();
            irrecv.resume();
            if(enter==0)
            break;
          }
        delay(200);
    }
  }
}

void blue()
{
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("MENU");
  lcd.setCursor(2, 1);
  lcd.print(">Blue");
  if (enter == 1)
  {
     while(enter==1)
    {
      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("Blue");
      
      analogWrite(Red,255);
      analogWrite(Green,255);
      analogWrite(Blue,brightness);
    
    if (irrecv.decode(&results))
          {
            inputReceived();
            irrecv.resume();
            if(enter==0)
            break;
          }
        delay(200);
    }
  }
}


void loop()
{
  analogWrite(Red, 255);
  analogWrite(Blue, 255);
  analogWrite(Green, 255);

  while (i == 0)
  {
    Startup();
    i++;
  }

  if (irrecv.decode(&results))
  {
    inputReceived();
    irrecv.resume();
  }

  delay(150);
  switch (option)
  {
    case 0:
      ambient();
      break;

    case 1:
      strobe();
      break;

    case 2:
      warning();
      break;

    case 3:
      red();
      break;

    case 4:
      green();
      break;

    case 5:
      blue();
      break;
  }

}

