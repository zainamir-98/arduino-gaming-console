/****************************************

NUST SEECS ELECTRICAL ENGINEERING BEE-9C
OOP END OF SEMESTER PROJECT 2018

By:
Sardar Hasan Arfan Khan 

3.3V Full Power: SCRL(2), RR(2) FOR DIFF 1, SCRL(2), RR(1) FOR DIFF 2
5.0V Low Power:

NEW: SCRL(2), RR(4)

****************************************/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define LCD_DEF_CONTRAST 70  //  Default was 50 on windows
#define LCD_WIDTH 84
#define LCD_HEIGHT 48

#define BUTTON1_PIN 12
#define BUTTON2_PIN 10
#define BUTTON3_PIN 8
#define BUTTON4_PIN 9

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 7, 6);

class Button {
  const unsigned int led_pin;
  bool held_down;

public:
  Button(unsigned int lp) : led_pin(lp) {}
  bool first_pressed(void);
  bool pressed(void);
};

bool Button::first_pressed(void) {
  if (held_down && !this->pressed()) held_down = false;

  if (!held_down && this->pressed()) {
    held_down = true;
    return true;
  } else return false;
}

bool Button::pressed(void) {
  return digitalRead(led_pin);
}

Button button_right(BUTTON1_PIN), button_left(BUTTON2_PIN);
Button button_up(BUTTON3_PIN), button_down(BUTTON4_PIN);

//
//class Screen {
//protected:
//  unsigned int id;
//
//public:
//  static unsigned int current_screen_id;
//
//  Screen(unsigned int i) : id(i) {};
//  virtual void addItem(MenuItem mi) {};
//  virtual void setScore(unsigned int s) {};
//  virtual void draw(void) {}; // Draw function
//  virtual void process(void) {}; // Process events
//};
//
//unsigned int Screen::current_screen_id = MAINMENU_ID;
//
//class Menu : public Screen {
//  String header;
//  MenuItem* items; // Dynamic array of menu items
//  unsigned int item_count;
//  int selection_index;
//
//public:
//  Menu() : Screen(0), header("N/A"), item_count(0), selection_index(0) {
//    items = NULL;
//  }
//
//  Menu(unsigned int i, String h) : Screen(i), header(h), item_count(0), selection_index(0) {
//    items = NULL;
//  };
//
//  void addItem(MenuItem mi);
//  void process(void);
//  void draw(void);
//};

//SANKE GAME CODE BEGINS HERE

static const unsigned char PROGMEM snakebody[] = {
 B11111111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111, B11111111,
B11111111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11111111,
};

static const unsigned char PROGMEM food[] = {
 B11111111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111, B11111111,
B11111111,B11111111,B11111111,B11111111,
B11111111,B11111111,B11111111,B11111111,
};


class snake //class relating to snake body
{ protected:
static const int partheigth=4,partwidth=4;
int headx,heady;
int tailx,taily;

public:
void drawpart(); //to draw body part of snake
void sethead(int x,int y); //to set head coordinates of each part
void settail(int x, int y); //to set tail coordinates of each part
int returntailx();
int returntaily();
int returnheadx();
int returnheady();
  
  
  };

void snake::drawpart()
{
  if (heady<=40)  //to restrict snake within boundries
  {
  display.drawBitmap(headx,heady,snakebody,partwidth,partheigth,1);
  //display.display();
  //display.clearDisplay();
  }
  };
void snake::sethead(int x, int y)
{
  //set head coordinates
  headx=x;
  heady=y;
  };
 void snake::settail(int x, int y)
 {
  //set tail coordinates
  tailx=x;
  taily=y;
  
  };

  int snake::returntailx()
  {
    return tailx;
    };

     int snake::returntaily()
  {
    return taily;
    };
int snake::returnheadx()
{
  return headx;
  };

  int snake::returnheady()
  {
    return heady;
    };

  
  class snakegame
  {
    protected:
    int headposx=20,headposy=20; //coordinates of head of snake part
    int tailposx=20,tailposy=20; //coordinates of tail of snake part
    int foodx,foody; //coordinates of food
    int delayfactor; //delay
    static const int limit=20; //limit of snake parts creatable
    snake *ptr[limit]; //array of class snake
    int level=1; //variable to store level
    int score=0; //variable to store score
    int count=0; //keep track of how many snake body parts defined
    bool up=0,down=0, right=0,left=0; // t0 keep track of motion
    bool foodeaten=1; //boolean to keep track of if food is eaten
    bool firstrun=0; //if first time code is run 
    static const int partheigth=4,partwidth=4,foodwidth=4,foodheigth=4; //height and width of bitmaps
    static const int topboundryy=0,bottomboundryy=40; // horixontal boundry poditions
    static const int leftboundryx=0,rightboumdryx=83; //vertical boundry positions


    public:
    //snakegame(unsigned int i);
    void process(); //function to manage running of game
    void grid(); //function to draw grid
    void boundries(); //to set up boundry of game
    void scorechecker(); //to keep track of score
    void levelchecker(); //to keep track of levels
    void stats(); // for printing stats
    void detectbutton(); //to detect buttom pressed for movement
    void movement(); //to check where to move
    void drawfood(); //to draw food
    void eatfood(); //to eat food and update score
    void detectcrash(); //to detect crash of snake
    void endmessage(); //to print end game message
    void resetvariables(); //to reset all variables once game is lost
    void draw(); //virtual function of screen class
    

    
    };
//parameterized constructor
//   snakegame:: snakegame(unsigned int i) : Screen(i) {
//  };

    
  
void snakegame::draw()
{
  int x=0;
  
  }
    
void snakegame::resetvariables()
{
    headposx=20,headposy=20; //coordinates of head of snake part
     tailposx=20,tailposy=20; //coordinates of tail of snake part
     level=1;
     score=0;
    
     up=0,down=0, right=0,left=0; // t0 keep track of motion
     foodeaten=1; //boolean to keep track of if food is eaten

    for (int i=4;i<count;i++)
    {
      delete []  ptr[i];
      
      }
 count=0; //keep track of how many snake body parts definedl 
 firstrun=0; 
    
    boundries(); //call boundries function
  };

  
void snakegame::process()
{
  boundries();
  detectbutton();
  detectcrash();
  grid();
  movement();
  eatfood();
  levelchecker();
  delay(delayfactor);
  };

void snakegame::grid()
{
  //draw boundry lines
  display.drawLine(0,0,83,0,BLACK); 
  display.drawLine(0,40,84,40,BLACK);
  display.drawLine(0,0,0,40,BLACK);
  display.drawLine(83,0,83,40,BLACK);

  for (int i=0;i<count;i++)
  { //display snake body parts
    ptr[i]->drawpart();
    }

drawfood();
    stats(); //display stats
    //display.drawBitmap(tailposx,tailposy,snakebody,partwidth,partheigth,1);
    display.display();
    display.clearDisplay();
  };


  
void snakegame::boundries()
{ //draw boundry lines

  if(firstrun==0)
  {

  display.drawLine(0,0,83,0,BLACK); 
  display.drawLine(0,40,84,40,BLACK);
  display.drawLine(0,0,0,40,BLACK);
  display.drawLine(83,0,83,40,BLACK);

int tempx=20,tempy=20;
for (int i=0;i<4;i++) //draw 4 snake body parts
{ if (i==0) //for first part
{
    ptr[i]= new snake; //create new object of class snake
    ptr[i]->sethead(tempx,tempy); //set coordinates of head
    ptr[i]->settail(tempx,tempy+partheigth); //set coordinates of tail
    ptr[i]->drawpart(); //draw part
    count++; //increment numbe rof parts created
}

else {  //for rest of  body
    ptr[i]= new snake; //create new object of class snake
    //set tail coordiantes of previous part as head coordinates of new part
    tempx=ptr[i-1]->returntailx(); 
    tempy=ptr[i-1]->returntaily();
    ptr[i]->sethead(tempx,tempy);
    ptr[i]->settail(tempx,tempy+partheigth); 
    ptr[i]->drawpart(); //draw the part
    count++; //increment number of parts created
  
  
  }
}
up=1;

stats();
    //display.drawBitmap(tailposx,tailposy,snakebody,partwidth,partheigth,1);
    display.display();
    display.clearDisplay();

  }

  firstrun=1;
  };

void snakegame::stats()
{
  display.setCursor(0,41); //position on grid to start text
  display.setTextColor(BLACK); //text color
  display.setTextSize(1); //text size

  display.print("Scr:"); //print string score:
  display.print(score); //print score value

  //display.setCursor(0,70); //position on grid to start text  
  display.print(" lvl:"); //print string level
  display.print(level);  //print level value
};

void snakegame::detectbutton()
{
  if (button_up.pressed()==1)
  {
    up=1;
    down=0;
    right=0;
    left=0;
    }

     if (button_down.pressed()==1)
    {
      down=1;
      up=0;
      right=0;
      left=0;
    }

         if (button_right.pressed()==1)
        {
          right=1;
          up=0;
          down=0;
          left=0;
          }

          if (button_left.first_pressed()==1)
      {
        left=1;
       up=0;
       down=0;
       right=0;
        }
  
  };

void snakegame::movement()
{
   int temp1x,temp1y,temp2x,temp2y; //variables to hold head positions
  temp1x=ptr[0]->returnheadx(); //x coordinate of current head of snake
  temp1y=ptr[0]->returnheady(); //y coordinate of current head of snake
  ptr[0]->sethead(headposx,headposy); //new head coordinates of snake set 

  for(int i=1;i<count;i++) //for restof body
  {
    temp2x=ptr[i]->returnheadx(); //x coordiante of part head stored
    temp2y=ptr[i]->returnheady(); //y coordiante of part head stored
    //new head pf part set as old head of previous part
    ptr[i]->sethead(temp1x,temp1y); 
    temp1y=temp2y; //old x coordinate of current part stored
    temp1x=temp2x; //old y coordinate of current part stored
    }

    
  if (up==1) //if movement is upwards
  headposy=headposy-partheigth; //new head of snake is one part heigth upwards
  else if (down==1) //if movement is downwards
  headposy=headposy+partheigth; //new head of snake is one part heigth downwards
  else if(right==1) //if movement is rigthwards
  headposx=headposx+partwidth; //new head of snake is one part width rigthwards
  else if(left==1) //if movement is leftwards
  headposx=headposx-partwidth; //new head of snake is one part width leftwards

//to wrap around snake if it exceeds boundries
  if(headposy<0)
  headposy=36;
  if (headposy>=40)
  headposy=4;
  if (headposx>=83)
  headposx=4;
  if (headposx<=0)
  headposx=79;
                
      };
 

void snakegame::drawfood()
{


  if(foodeaten==1) //if food is eaten
  {
 foodx=random(1,80); //randomly generate x coordinate of food head
 foody=random(1,36); //randomly generate y coordinate of food head
 
 
 
  foodeaten=0; //  food is not eaten
  }
  display.drawBitmap(foodx,foody,food,foodwidth,foodheigth,BLACK); //display food
  
  };        

  void snakegame:: eatfood()
  {
    int tempx, tempy; //temporary variables to store head coordinates
    int temp2x,temp2y; //temporary variables to store head coordinates
    tempx=ptr[0]->returnheadx(); //store current snake head x coordinate
    tempy=ptr[0]->returnheady(); //store current snake head y coordinate
    temp2x=ptr[0]->returntailx(); //store current snake tail x coordinate
    temp2y=ptr[0]->returntaily(); //store current snake tail y coordinate

    
    if (foodx<=tempx+foodwidth && foodx>=tempx-foodwidth) //food is eaten
    { 
      if (foody<=tempy+foodheigth && foody>=tempy-foodheigth)
      {
      foodeaten=1;
     scorechecker();
      int temp1,temp2;
     ptr[count]= new snake; //new part created
     temp1= ptr[count-1]->returntailx();//store previous part tail x coordinate
     temp2= ptr[count-1]->returntaily();//store previous part tail y coordinate
     ptr[count]->sethead(temp1,temp2); //set new part head
     ptr[count]->settail(temp1+partwidth,temp2+partheigth); //set new part tail
     count++; //increment number of parts 
      }
      
      }

      
      
    
    };

void snakegame:: scorechecker()
{
  score++; //increment score
 levelchecker(); //call function to check level
 };

void snakegame::levelchecker()
{
  if (score>=0 && score <3) //condition for level 1
  {
    level=1; //level is set
    delayfactor=500; //delay is set
    
    }

    else if (score>=3 && score<6)
    {
      level=2; //level is set
      delayfactor=300; //delay is set
      }

      else if (score>=6 && score <9)
      {
        level=3; //level is set
        delayfactor=100; //delay is set
        }
        else 
        {level=4; //level is set
        delayfactor=50; //delay is set
        }
  
  };

  void snakegame::detectcrash()
{
  int tempx; //variable to store body part's head's x coordinate
  int tempy; //variable to store body part's head's x coordinate

  
  for (int i=1;i<count;i++)
  {
        
    tempx=ptr[i]->returnheadx();  //store current snake body part's head's x coordinate
    tempy= ptr[i]->returnheady(); //store current snake body part's head's y coordinate
    
//      if (tempy<=headposy+partheigth && tempy>=headposy-partheigth)
//      if ( tempx<=headposx+partwidth && tempx>=headposx-partwidth)
//      {
//        delay(3000);
//         endmessage();
//         
//      }

       if (headposx==tempx && headposy==tempy) //head of snake coincides with head of rest of snake body parts
       {
        delay(3000); //3sec delay
        endmessage(); //call function to print end message
        
        }

        
//      tempx=ptr[i]->returntailx();
//      tempy= ptr[i]->returntaily();
//
//         if (headposx==tempx && headposy==tempy)
//       {
//        delay(3000);
//        endmessage();
//        
//        }

  }
     
    
};
  
  

void snakegame::endmessage()
{
  display.clearDisplay();
   display.setCursor(5,5); //set cursor to point
   display.print("Game over.Nice try!"); //print message
   display.setCursor(5,20); //set cursor to point
   display.print("Final score:"); //print message
   display.setCursor(5,30); //set cursor to point
   display.print(score); //print final score

   display.display();
   display.clearDisplay();
   delay(4000); //4 seconds delay
   resetvariables();
  };

snakegame s1;   
void processSetup(void);

void setup() {
processSetup();
s1.boundries();
}

void loop() {
  // put your main code here, to run repeatedly:

s1.process();
s1.draw();
  display.display();
  display.clearDisplay();
} 

   
   

void processSetup(void) {

  // Initialize LCD display
  Serial.begin(9600);
  display.begin();

  // Set contrast to default value (60)
  display.setContrast(LCD_DEF_CONTRAST);

  // This tells the processor that button1_pin and button2_pin are input pins
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);

  // Display splashscreen
  display.display();
  delay(2000);
  display.clearDisplay();
}
