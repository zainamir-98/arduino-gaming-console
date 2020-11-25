
/****************************************

NUST SEECS ELECTRICAL ENGINEERING BEE-9C
OOP END OF SEMESTER PROJECT 2018

By:
Zain Amir Zaman (GUI, Dino)
Sardar Hasan Arfan Khan (Car, Snake)
Asfandyar Ahmed (Tic Tac Toe, Hardware setup)

3.3V Full Power: SCRL(2), RR(2) FOR DIFF 1, SCRL(2), RR(1) FOR DIFF 2
5.0V Low Power:

NEW: SCRL(2), RR(4)

****************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <EEPROM.h>

#define LCD_DEF_CONTRAST 70  //  Default was 50 on windows
#define LCD_WIDTH 84
#define LCD_HEIGHT 48

#define BUTTON1_PIN 12
#define BUTTON2_PIN 10
#define BUTTON3_PIN 8
#define BUTTON4_PIN 9

#define MAINMENU_ID 0
#define GAMESELECTION_ID 1
#define SETTINGS_ID 6
#define ABOUT_ID 3
#define DINOGAME_ID 2
#define CARGAME_ID 4
#define SNAKEGAME_ID 5

// EEPROM addresses

uint8_t EEPROMAdd_temp = 130;
uint8_t EEPROMAdd_dino_highscore = 131;
uint8_t EEPROMAdd_car_highscore = 132;
uint8_t EEPROMAdd_snake_highscore = 133;

Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, 5, 7, 6);

//*** Bitmap arrays ***//

static const unsigned char PROGMEM test_bmp[] = { // Size: 24 x 25
    B00000001,B11111110,B00000000,
    B00001111,B11111111,B11000000,
    B00111111,B11111111,B11110000,
    B00111111,B11111111,B11110000,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11111111,B11111100,
    B11111111,B11001111,B11111100,
    B01101011,B10000111,B01011000,
    B01100000,B00000000,B00011000,
    B01100011,B10000111,B00011000,
    B00100011,B10000111,B00010000,
    B00100000,B00000000,B00010000,
    B00100000,B00000000,B00010000,
    B00100000,B10000100,B00010000,
    B00100000,B11111100,B00010000,
    B00100000,B00000000,B00010000,
    B00011000,B00000000,B01100000,
    B00000110,B00000001,B10000000,
    B00000001,B11111110,B00000000,
 };

static const unsigned char PROGMEM dino[] = { // Size: 16 x 11
      B00000011,B11100000,
      B00000111,B01100000,
      B00000111,B11100000,
      B00000111,B11000000,
      B00001110,B00000000,
      B10011111,B00000000,
      B11111110,B00000000,
      B01111110,B00000000,
      B00111100,B00000000,
      B00010100,B00000000,
      B00010110,B00000000,
};

static const unsigned char PROGMEM cactus_single[] = { // Size: 5 x 10
B00010000,
B01110000,
B01110000,
B11110000,
B01110000,
B01111000,
B11110000,
B11110000,
B01110000,
B01110000,
};

static const unsigned char PROGMEM cactus_double[] = { // Size: 11 x 10
  B00000001,B00000000,
B00000111,B11000000,
B00000111,B11000000,
B01100001,B11100000,
B11110001,B11000000,
B01111001,B11000000,
B01110001,B11000000,
B01110001,B11000000,
B01110001,B11000000,
B01110001,B11000000,
};

static const unsigned char PROGMEM cloud_big[] = { // Size: 48 x 13
  B00000000,B00000000,B00000000,B01111000,B00000000,B00000000,
  B00000000,B00000000,B00001111,B11001110,B00000000,B00000000,
  B00000000,B00000000,B00011000,B00000011,B00000000,B00000000,
  B00000000,B00000000,B01110000,B00000001,B00000000,B00000000,
  B00000000,B00000000,B01000000,B00000001,B11100000,B00000000,
  B00000000,B00000000,B01000000,B00000001,B00111111,B00000000,
  B00000000,B00000011,B11000000,B00000010,B00000001,B00000000,
  B00000000,B00000110,B00000000,B00000000,B00000001,B11100000,
  B00000011,B11111100,B00000000,B00000000,B00000000,B00100000,
  B00000110,B00000000,B00000000,B00000000,B00000000,B00111000,
  B00000100,B00000000,B00000000,B00000000,B00000000,B00001000,
  B01101100,B01000000,B00000000,B00000000,B00000000,B00000100,
  B11000000,B00111111,B11111111,B11111111,B11111111,B11111100,
};

static const unsigned char PROGMEM cloud_small[] = { // Size: 24 x 9
  B00000000,B00010000,B00000000,
  B00000000,B01101000,B00000000,
  B00000000,B10000100,B00000000,
  B00001111,B00000011,B10000000,
  B00010000,B00000100,B01100000,
  B01000000,B00000000,B00001000,
  B10000001,B11111111,B11100000,
  B01000010,B00000000,B00000000,
  B00011000,B00000000,B00000000,
};

//*** Classes w/ member function prototypes only ***//

class Button {
  const unsigned int led_pin;
  bool held_down;

public:
  Button(unsigned int lp) : led_pin(lp) {}
  bool first_pressed(void);
  bool pressed(void);
};

Button button_right(BUTTON1_PIN), button_left(BUTTON2_PIN);
Button button_up(BUTTON3_PIN), button_down(BUTTON4_PIN);

class MenuItem {
  String name; // Name of item as it appears on screen
  unsigned int screen_id; // ID of screen to go to when item is selected

public:
  MenuItem() : name("N/A"), screen_id(0) {};
  MenuItem(String n, unsigned int si) : name(n), screen_id(si) {};
  void performAction(void);
  String getName(void);
  unsigned int getScreenID(void);
};

class Screen {
protected:
  unsigned int id;

public:
  static unsigned int current_screen_id;

  Screen(unsigned int i) : id(i) {};
  virtual void addItem(MenuItem mi) {};
  virtual void setScore(unsigned int s) {};
  virtual void draw(void) {}; // Draw function
  virtual void process(void) {}; // Process events
};

unsigned int Screen::current_screen_id = MAINMENU_ID;

class Menu : public Screen {
  String header;
  MenuItem* items; // Dynamic array of menu items
  unsigned int item_count;
  int selection_index;

public:
  Menu() : Screen(0), header("N/A"), item_count(0), selection_index(0) {
    items = NULL;
  }

  Menu(unsigned int i, String h) : Screen(i), header(h), item_count(0), selection_index(0) {
    items = NULL;
  };

  void addItem(MenuItem mi);
  void process(void);
  void draw(void);
};

class DinoGameProperties {
protected:
  // Ideal setting for LOW difficulty
  static unsigned int scroll_speed;
  static unsigned int refresh_rate;
  static unsigned int counter;

  unsigned int ground_height = 4;
};

unsigned int DinoGameProperties::scroll_speed = 2;
unsigned int DinoGameProperties::refresh_rate = 3;
unsigned int DinoGameProperties::counter = 0;

class Cactus : public DinoGameProperties {
  bool type; // 0 for single, 1 for double
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;

  unsigned int destroy_distance = 2;

public:
  Cactus() : type(0) {
    x = LCD_WIDTH;

    if (type == 0) {
      w = 5;
      h = 10;
      y = LCD_HEIGHT - ground_height - h;
    } else {
      w = 11;
      h = 10;
      y = LCD_HEIGHT - ground_height - h;
    }
  };
  unsigned int getX(void) {
    return x;
  }
  unsigned int getY(void) {
    return y;
  }
  void changeToDouble(void) {
    type = 1;
    w = 11;
    h = 10;
    y = LCD_HEIGHT - ground_height - h;
  }
  void process(void);
  void draw(void);
};

class DinoGame : public Screen, public DinoGameProperties {

  // Dinosaur properties
  const unsigned int dino_width = 16; // Width of dinosaur in pixels
  const unsigned int dino_height = 11; // Height of dinosaur in pixels
  unsigned int x = 4;
  const unsigned int default_height = LCD_HEIGHT - ground_height - dino_height;
  float y = default_height;
  const float gravity = 0.12;
  const float jump_speed = 2.3;
  float velocity = 0;
  bool on_ground = true;

  Cactus cactusArray[5];
  bool cactusExists[5] = {0, 0, 0, 0, 0};
  unsigned int cacArr_counter = 0;

  // Cactus properties
  const unsigned int cactus_height = 10;
  const unsigned int cactus_width = 5;
  float cactus_x = LCD_WIDTH + cactus_width;
  float cactus_y = LCD_HEIGHT - ground_height - cactus_height;

  // Cloud properties
  const unsigned int cloud_big_width = 48;
  const unsigned int cloud_big_height = 13;
  const unsigned int cloud_small_width = 24;
  const unsigned int cloud_small_height = 9;
  float cloud_big_x = LCD_WIDTH;
  float cloud_big_y = (LCD_HEIGHT - ground_height) / 2 - cloud_big_height / 2;
  float cloud_small_x = LCD_WIDTH + 60;
  float cloud_small_y = (LCD_HEIGHT - ground_height) / 2 - cloud_big_height / 2;

  unsigned long int score = 0;
  unsigned int score_counter = 0;

  int cactus_counter = 0;
  int cactus_counter_avg = 100;
  int cactus_counter_deviation = 20;
  int cactus_counter_limit = cactus_counter_avg + random(-cactus_counter_deviation, cactus_counter_deviation);

  bool game_over = false;
  unsigned int game_over_state = 0;
  int game_over_counter = 0;

public:
  DinoGame(unsigned int i) : Screen(i) {
    cacArr_counter = 0;
  };
  bool checkCollision(void);
  void createCactus();
  void process(void);
  void reset(void);
  void draw(void);
};

class AboutScreen : public Screen {
  const String header = "About";
  int timer;
  const int timer_limit = 100;
  const int array_size = 2;
  String content[2];
  unsigned int array_index = 0;

public:
  AboutScreen(unsigned int i) : Screen(i) {
    content[0] = "NUST SEECS ELECTRICAL ENG.\n\n2018 OOP \nProject";
    content[1] = "BEE-9C\nBy Zain Amir, Sardar Hassan, Asfandyar Ahmed";
  };
  void process(void);
  void draw(void);
};

//*** Class member function definitions ***//

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

String MenuItem::getName(void) {
  return name;
}

unsigned int MenuItem::getScreenID(void) {
  return screen_id;
}

void MenuItem::performAction(void) {
  Screen::current_screen_id = screen_id;
}

void Menu::addItem(MenuItem mi) {
    // Adding an item for the first time in the array
    if (items == NULL) {
        items = new MenuItem[1];
        items[0] = mi;
        ++item_count;
    } else {
        MenuItem* temp_array = items;
        items = new MenuItem[item_count + 1];

        // Assign items from previous items array to new items array
        for (unsigned int i = 0; i < item_count; i++) {
            items[i] = temp_array[i];
        }

        // Assign new value
        items[item_count] = mi;
        ++item_count;
        delete [] temp_array;
    }
}

void Menu::process(void) {
  if (button_up.first_pressed()) {
    if (selection_index <= 0) selection_index = item_count - 1; else --selection_index;
  }

  if (button_down.first_pressed()) {
    if (selection_index >= item_count - 1) selection_index = 0; else ++selection_index;
  }

  if (button_right.first_pressed()) items[selection_index].performAction();
}

void Menu::draw(void) {
  if (header != "N/A") {
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println(header);
  }

  display.setTextSize(1);

  for (unsigned int i = 0; i < item_count; i++) {
    display.setCursor(5 * (i == selection_index), 20 + i * 8);
    display.println(items[i].getName());
  }

  display.drawRoundRect(0, 20 + selection_index * 8 - 1, LCD_WIDTH, 10, 3, BLACK);
}

void Cactus::process(void) {
  if (counter % refresh_rate == 0) x -= scroll_speed;
  //if (x < 6) x = LCD_WIDTH + w;
};

void Cactus::draw(void) {
  if (type == 0) {
    display.drawBitmap(static_cast<int>(x), y, cactus_single, w, h, 1);
  } else {
    display.drawBitmap(static_cast<int>(x), y, cactus_double, w, h, 1);
  }

}

void DinoGame::process(void) {
  counter++;

  if (counter % refresh_rate == 0) {
    cactus_x -= scroll_speed;
    cloud_big_x -= scroll_speed / 2;
    cloud_small_x -= scroll_speed / 2 - 0.3;
  }

  if (cactus_x < 6) cactus_x = LCD_WIDTH + cactus_width;

  if (cloud_big_x < -static_cast<float>(cloud_big_width)) {
    cloud_big_x = LCD_WIDTH;
    // cloud_big_y randomize
  }

  if (cloud_small_x < -static_cast<float>(cloud_small_width)) {
    cloud_small_x = LCD_WIDTH;
    // cloud_small_y randomize
  }

  // Dinosaur physics

  if (button_up.first_pressed() && on_ground) {
    velocity = -jump_speed;
    on_ground = false;
  }

  if (velocity != 0 && !on_ground) {
    y += velocity;
    velocity += gravity;
  }

  if (y >= default_height) on_ground = true; //y = default_height

  if (checkCollision()) game_over = true;

  // Create cactus objects

  this->createCactus();

  // Process difficulty

  if (score == 15) {
    cactus_counter_avg = 70;
    cactus_counter_deviation = 40;
  } else if (score == 30) {
    DinoGameProperties::refresh_rate = 2;
  } else if (score == 50) {
    cactus_counter_avg = 40;
    cactus_counter_deviation = 20;
  } else if (score == 70) {
    DinoGameProperties::refresh_rate = 1;
  }

  // Destroy cactus object when it reaches end of screen

  for (int i = 0; i < 5; i++) {
    if (!cactusExists[i]) continue;

    cactusArray[i].process();
    if (cactusArray[i].getX() < 5) cactusExists[i] = false;
  }

  ++score_counter;

  if (!game_over && score_counter > 50) {
    ++score;
    score_counter = 0;
  }
}

bool DinoGame::checkCollision(void) {
  for (unsigned int i = 0; i < 5; i++) {
    if (!cactusExists[i]) continue;

    int collision_x_allowance = 3;
    int collision_y_allowance = 3;

    if ((x + dino_width - collision_x_allowance >= cactusArray[i].getX()) && (y + dino_height - collision_y_allowance > cactusArray[i].getY())) {
      return true;
    } else {
      return false;
    }
  }
}

void DinoGame::createCactus(void) {
  ++cactus_counter;

  if (cactus_counter >= cactus_counter_limit) {
    for (int i = 0; i < 5; i++) {
      if (!cactusExists[i]) {
          Cactus cactusObj;
          //cactusObj.changeToDouble();
          cactusArray[i] = cactusObj;
          cactusExists[i] = true;
          break;
      }
    }

    cactus_counter_limit = cactus_counter_avg + random(-cactus_counter_deviation, cactus_counter_deviation);
    cactus_counter = 0;
  }
}

void DinoGame::reset(void) {
  DinoGameProperties::refresh_rate = 3;

  x = 4;
  y = default_height;
  velocity = 0;
  on_ground = true;

  for (int i = 0; i < 5; i++) {
    cactusExists[i] = 0;
  }
  cacArr_counter = 0;

  cactus_x = LCD_WIDTH + cactus_width;
  cactus_y = LCD_HEIGHT - ground_height - cactus_height;

  cloud_big_x = LCD_WIDTH;
  cloud_big_y = (LCD_HEIGHT - ground_height) / 2 - cloud_big_height / 2;
  cloud_small_x = LCD_WIDTH + 60;
  cloud_small_y = (LCD_HEIGHT - ground_height) / 2 - cloud_big_height / 2;

  score = 0;
  score_counter = 0;

  cactus_counter = 0;
  cactus_counter_avg = 100;
  cactus_counter_deviation = 20;
  cactus_counter_limit = cactus_counter_avg + random(-cactus_counter_deviation, cactus_counter_deviation);

  game_over = false;
  game_over_state = 0;
  game_over_counter = 0;
}

void DinoGame::draw(void) {
  if (!game_over) {
    // Draw ground
    for (int i = 0; i < ground_height; i++) {
       display.drawLine(0, LCD_HEIGHT - ground_height + i, LCD_WIDTH, LCD_HEIGHT - ground_height + i, BLACK);
    }

    display.drawLine(0, LCD_HEIGHT - ground_height, LCD_WIDTH, LCD_HEIGHT - ground_height, BLACK);

    // Draw graphics
    display.drawBitmap(x, static_cast<int>(y), dino, dino_width, dino_height, 1);
    display.drawBitmap(static_cast<int>(cloud_big_x), cloud_big_y, cloud_big, cloud_big_width, cloud_big_height, 1);
    display.drawBitmap(static_cast<int>(cloud_small_x), cloud_small_y, cloud_small, cloud_small_width, cloud_small_height, 1);

    for (int i = 0; i < 5; i++) {
      if (!cactusExists[i]) continue;

      cactusArray[i].draw();
    }

    //display.drawBitmap(static_cast<int>(cactus_x), cactus_y, cactus_single, cactus_width, cactus_height, 1);

    // Draw HUD
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println(score);
    //display.println("\nSCRL: ");
    //display.println(scroll_speed);
    //display.println(" RR: ");
    //display.println(refresh_rate);
  } else {
    display.setTextColor(BLACK);
    display.setCursor(10, 5);
    display.setTextSize(1);

    EEPROM.write(EEPROMAdd_temp, score);
    if (score > EEPROM.read(EEPROMAdd_dino_highscore)) {
      EEPROM.write(EEPROMAdd_dino_highscore, score);
    }

    switch (game_over_state) {
      case 0:
      display.print("GAME OVER! \n\nScore: ");
      display.print(EEPROM.read(EEPROMAdd_temp));
      display.print("\nHighscore: ");
      display.print(EEPROM.read(EEPROMAdd_dino_highscore));

      ++game_over_counter;

      if (game_over_counter > 200) {
        this->reset();
        Screen::current_screen_id = MAINMENU_ID;
      }
      break;
    }
  }
}

void AboutScreen::process(void) {
  ++timer;

  if (timer >= timer_limit) {
    if (array_index == array_size - 1) {
      timer = 0;
      array_index = 0;
      Screen::current_screen_id = MAINMENU_ID;
    } else {
      ++array_index;
      timer = 0;
    }
  }
}

void AboutScreen::draw(void) {
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  //display.println(header);

  display.setTextSize(1);

  display.println(content[array_index]);
  display.setCursor(LCD_WIDTH/2, LCD_HEIGHT - 20);
}

//*** Global variables ***//

Screen* screens[6];

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


  class snakegame: public Screen
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
    snakegame(unsigned int i);
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
   snakegame:: snakegame(unsigned int i) : Screen(i) {
  };



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

//    boundries(); //call boundries function
    
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
  EEPROM.write(EEPROMAdd_temp, score);
  if (score > EEPROM.read(EEPROMAdd_snake_highscore)) {
    EEPROM.write(EEPROMAdd_snake_highscore, score);
  }
  display.clearDisplay();
  display.setCursor(0,0); //set cursor to point
  display.print("Game over!\n\n"); //print message
  display.print("Score: "); //print message
  display.print(score); //print final score
  display.print("\nHighscore:");
  display.print(EEPROM.read(EEPROMAdd_snake_highscore));

   display.display();
   display.clearDisplay();
   delay(4000); //4 seconds delay
   resetvariables();
   Screen::current_screen_id=MAINMENU_ID; //return to main menu
  };


//CAR GAME CODE BEGINS HERE




static const unsigned char PROGMEM car1[] = {
B00111111,B00000000,B00000000,
B00100001,B00000000,B00000000,
B01111111,B11111110,B00000000,
B11111111,B11111111,B00111100,
B11111110,B00000111,B00100100,
B11111101,B10011011,B11111110,
B11111011,B10011101,B11111111,
B11111011,B10011101,B11111101,
B11111011,B10011101,B11111111,
B11111011,B00001101,B11111111,
B11111010,B01100101,B11111101,
B11111000,B11110001,B11111111,
B11111101,B11111011,B11111110,
B11111110,B00000111,B00100100,
B11111111,B11111111,B00111100,
B01111111,B11111110,B00000000,
B00100001,B00000000,B00000000,
B00111111,B00000000,B00000000,
};

static const unsigned char PROGMEM obstacle[] = {
B00111111,B00000000,
B01111111,B10000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B11111111,B11000000,
B01111111,B10000000,
B00111111,B00000000,
};


//global variables
long scoreinterval=1000; //interval for score increament
long previousmillis=0; //refrence time


class Car: public Screen
{
  protected:
  static const int carheigth=18, carwidth=24;
  static const int obstacleheigth=18, obstaclewidth=10;
  static const int carystartpos1=0,carystartpos2=21, carxstartpos=0;
  static const int obsx1startpos=84, obsy1startpos=0, obsx2startpos=84,obsy2startpos=21;
  static const int obsx1endpos,obsy1endpos, obsx2endpos, obsy2endpos;
  boolean carup=1,upperobstacle=0, lowerobstacle=0;
  boolean obsup1=0, obsup2=0, obslow1=0, obslow2=0;
  boolean crash=0;
  boolean firsttime=1; //first run of game?
  int temp1= 84,temp2=84, temp3=84, temp4=84;
  int level=1;
  int score=0;
  int lives=3;
  int delaylevel1=1000, delaylevel2=500, delaylevel3=200;
  int delayfactor=500;
int x; //dummy variable
  public:
  unsigned long timer=0;//keep track of time
  Car(int i): Screen(i) {
   //parameterized constructor
   x=0;
  };
  void process();  //runs game
  void startmessage(); //print message at start of game
  void setupgrid(); //to set up car track
  void drawobstacle(); //to draw obstacle
  void movecarup(); // to move the car upwards
  void movecardown(); //to move the car downwards
  void moveobstacle(); //to decide which lane obscatcle emerges
  void checkcrash(); //to detect crash
  void resetgrid(); //to reset track after crash
  void endgame(); //to end game, store score, and print end message
  void scorechecker(); //to increase score
  void livesremaining(); //to keep track of lives
  void stats(); //stats displayed at end of screen
  void draw(); //virtual function of screen class

  };




void Car::draw()
{
  int x=0;

  }


void Car::process()
{
  timer=millis();
  startmessage();
  setupgrid();
  checkcrash();
  movecardown();
  movecarup();
  scorechecker();
  delay(delayfactor);
  resetgrid();

  };

void Car::startmessage()
{
  if (firsttime==1)
  {
  display.setCursor(20,0); //position on grid to start text
  display.setTextColor(BLACK); //text color
  display.setTextSize(1); //text size set to factor 2
  display.print("You have 3 lives"); //print message
  display.setCursor(20,30); //set cursor to new position
  display.print("GOOD LUCK!"); //print message

  display.display();
  display.clearDisplay();

  firsttime=0;

  }


  };
void Car::setupgrid()
{
  display.drawLine(0,0,84,0,BLACK); //first lane perimeter
  display.drawLine(0,20,84,20,BLACK); //first lane perimeter
  display.drawLine(0,40,84,40,BLACK); //second lane

  if (carup==1)
  {
  display.drawBitmap(carxstartpos,carystartpos1,car1,carwidth,carheigth,1); //drawing the car on upper lane
  }

  if (carup==0)
  {
    display.drawBitmap(carxstartpos,carystartpos2,car1, carwidth,carheigth,1); //drawing the car on down lane
  }

  drawobstacle();
  stats(); //display stats
  display.display();
  display.clearDisplay();
 };


void Car::stats()
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


void Car::scorechecker()
{

  if ((timer-previousmillis)>scoreinterval)
  {
    score++;
    previousmillis=timer;
    }

  if (score>=5 && level <2) //condition to reach level 2
  {
    level++;
    delayfactor=250;

  }
  else if(score >=10 && level<3) //condition to reach level 3
  {
    level++;
    delayfactor=150;

  }

  else if(score>=15 && level<4) //condition to reach level 4
  {
    level++;
    delayfactor=100;

  }
  };


  void Car::movecardown()
  {
     if((carup==1)&&(button_down.first_pressed()))
     {
       //display.drawBitmap(carxstartpos,carystartpos2,car1,carwidth,carheigth,1); //drawing the car in second lane
       //display.drawLine(0,0,84,0,BLACK); //first lane perimeter
       //display.drawLine(0,20,84,20,BLACK); //first lane perimeter
       //display.drawLine(0,40,84,40,BLACK); //second lane

       carup=0; //Car is in down lane


       //display.display();
       //display.clearDisplay();

      }

    };

    void Car::movecarup()
    {

      if((carup==0)&&(button_up.first_pressed()))
     {
       //display.drawBitmap(carxstartpos,carystartpos1,car1,carwidth,carheigth,1); //drawing the car in first lane
       //display.drawLine(0,0,84,0,BLACK); //first lane perimeter
       //display.drawLine(0,20,84,20,BLACK); //first lane perimeter
       //display.drawLine(0,40,84,40,BLACK); //second lane

       carup=1; //Car is in upper lane


       //display.display();
       //display.clearDisplay();
      }
      };



      void Car::drawobstacle()
      {
        int randomizer=random(1,30); //generates random number between 1 and 30

            if (randomizer%2==0 && lowerobstacle==0 && upperobstacle==0) //condition for obstacle to appear in upper lane
            {
              upperobstacle=1; //booleon to show object in upper lane
            display.drawBitmap(obsx1startpos,obsy1startpos, obstacle, obstaclewidth,obstacleheigth,1);
            //display.display();
            //display.clearDisplay();
            if (obsup1==0) //check if first obstacle in upper lane is being printed or not
            {
              obsup1=1;
              }
              else if (obsup2==0) //check if second obstacle in upper lane is being printed or not
              {
                obsup2=1;
                }


            }
            else if (randomizer%3==0 && upperobstacle==0 && lowerobstacle==0) //condition for obstacle to appear in lower lane
            {
              lowerobstacle=1; //booleon to show object in upper lane
            display.drawBitmap(obsx2startpos,obsy2startpos, obstacle, obstaclewidth,obstacleheigth,1);
            //display.display();
            //display.clearDisplay();

            if (obslow1==0) //check if first obstacle in lower lane is being printed or not
            {
              obslow1=1;
              }
              else if (obslow2==0) //check if second obstacle in lower lane is being printed or not
              {
                obslow2=1;
                }

              }

        moveobstacle(); //function to make obstacle move

        };


    void Car::moveobstacle()
    {
      if (obsup1==1) //first obstacle in upper lane is drawn
      {
        //code to move upper obstacle towards car
        display.drawBitmap(temp1,obsy1startpos, obstacle, obstaclewidth,obstacleheigth,1);
        temp1=temp1-5;
        }

        if (obsup2==1) //second obstacle in upper lane is drawn
      {
        //code to move upper obstacle towards car
        display.drawBitmap(temp2,obsy1startpos, obstacle, obstaclewidth,obstacleheigth,1);
        temp2=temp2-5;
        }

        if (obslow1==1)  //first obstacle in lower lane is drawn
        {
          //code to move lower obstacle towards car
          display.drawBitmap(temp3,obsy2startpos, obstacle, obstaclewidth,obstacleheigth,1);
          temp3=temp3-5;
          }

           if (obslow2==1)  //second obstacle in lower lane is drawn
        {
          //code to move lower obstacle towards car
          display.drawBitmap(temp4,obsy2startpos, obstacle, obstaclewidth,obstacleheigth,1);
          temp4=temp4-5;
          }


          if (temp1== 63-carwidth &&  obsup2==0) //condition to allow obstacle to appear in second lane as well
          {
            upperobstacle=0;
            }

            if (temp2== 63-carwidth && obsup1==0 ) //condition to allow obstacle to appear in second lane as well
          {
            upperobstacle=0;
            }


          if (temp3== 63-carwidth && obslow2==0) //condition to allow obstacle to appear in first lane as well
          {
            lowerobstacle=0;
            }

             if (temp4== 63-carwidth && obslow1==0 ) //condition to allow obstacle to appear in first lane as well
          {
            lowerobstacle=0;
            }

          if (temp1<=0)  //to make first upper obstacle dissapper if it passes car
          {
            //to check if there is room for car to pass and no two obstacles come together
          if (obsup2==0)
          {
            //reset variables
            obsup1=0;
            temp1=obsx1startpos;
            upperobstacle=0;
          }
          else
          {
            if(temp2<=63-carwidth)
            {
              //reset variables
              obsup1=0;
            temp1=obsx1startpos;
            upperobstacle=0;
              }



            }
          }

            if (temp2<=0)  //to make second upper obstacle dissapper if it passes car
          {
            //to check if there is room for car to pass and no two obstacles come together
            if  (obsup1==0)
            {
              //reset variables
              obsup2=0;
            temp2=obsx1startpos;
            upperobstacle=0;
            }
            else
            {

              if (temp1<=63-carwidth)
              {
                //reset variables
                obsup2=0;
            temp2=obsx1startpos;
            upperobstacle=0;

                }

              }

            }

             if (temp3<=0) //to make first lower obstacle disappear if it passes car
          {
            //to check if there is room for car to pass and no two obstacles come together
            if (obslow2==0)
            {
              //reset variables
             obslow1=0;
            temp3=obsx1startpos;
            lowerobstacle=0;
            }
            else
            {

              if (temp4<=63-carwidth)
              {
                //reset variables
                 obslow1=0;
            temp3=obsx1startpos;
            lowerobstacle=0;

                }

              }
            }



            if (temp4<=0) //to make second lower obstacle disappear if it passes car
          {
            //to check if there is room for car to pass and no two obstacles come together
            if (obslow1==0)
            {
              //reset variables
            obslow2=0;
            temp4=obsx1startpos;
            lowerobstacle=0;
            }
            else
            {
              //reset variables
              if (temp3<=63-carwidth)
              {
                obslow2=0;
            temp4=obsx1startpos;
            lowerobstacle=0;
                }
                }
            }



      };

      void Car::checkcrash()
      {
        if (obsup1==1 && carup==1) //car and obstacle both in upper lane
        {
          if (temp1<=carwidth-1) //obstacle being drawn in cars zone
          {
            delay(1000); //1seconds delay
            livesremaining(); //crash has occured
            crash=1;
            }
          }
          else if (obsup2==1 && carup==1) //car and obstacle both in upper lane
          {
            if (temp2<= carwidth-1) //obstacle being drawn in cars zone
            {
              delay(1000); //1seconds delay
              livesremaining(); //crash has occured
              crash=1;
              }

            }
        else if (obslow1==1 && carup==0) //car and obstacle both in lower lane
        {
          if (temp3<=carwidth-1) //obstacle being drawn in cars zone
          {
            delay(1000); //1seconds delay
            livesremaining(); //crash has occured
            crash=1;
            }
          }
          else if (obslow2==1 && carup==0) //car and obstacle both in lower lane
          {
            if (temp4<= carwidth-1) //obstacle being drawn in cars zone
            {
              delay(1000); //1seconds delay
              livesremaining(); //crash has occured
              crash=1;
              }

            }


        };

        void Car::livesremaining()
        {
          lives--;
          display.setCursor(5,10); //set cursor to point
          display.print("Crash occured"); //print crash statement
          display.setCursor(5,30); //set cursor to point
          display.print("Lives Remaining:"); //print crash statement
          display.print(lives); //print lives remaining
          display.display();
          display.clearDisplay();
          delay(4000); //4 seconds delay

          if (lives==0)
          {
            endgame();
            }
          };


      void Car::resetgrid()
      {
        if (crash==1) //crash occured and have to reset track
        { //resetting variables required to start printing obstacle from start
          temp1=obsx1startpos;
          temp2=obsx1startpos;
          temp3=obsx1startpos;
          temp4=obsx1startpos;
          lowerobstacle=0;
          upperobstacle=0;
          obsup1=0;
          obsup2=0;
          obslow1=0;
          obslow2=0;
          crash=0;

          }


        if (lives==0)
        {
          firsttime=1;
          }

      };


      void Car::endgame()
      {
        EEPROM.write(EEPROMAdd_temp, score);
        if (score > EEPROM.read(EEPROMAdd_car_highscore)) {
          EEPROM.write(EEPROMAdd_car_highscore, score);
        }
        display.setCursor(0,0); //set cursor to point
        display.print("Game over!\n\n"); //print message
        display.print("Score: "); //print message
        display.print(score); //print final score
        display.print("\nHighscore:");
        display.print(EEPROM.read(EEPROMAdd_car_highscore));

        display.display();
        display.clearDisplay();
        delay(4000); //4 seconds delay

        //reset variables
       level=1;
          score=0;
          lives=3;
         delayfactor=500;

       Screen::current_screen_id=MAINMENU_ID; //return to main menu
        };

void setup() {
  // Initialize LCD display
  Serial.begin(9600);
  display.begin();

  // Set contrast to default value (60)
  display.setContrast(LCD_DEF_CONTRAST);

  // To tell the arduino that the given button pins are meant for input
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
  pinMode(BUTTON4_PIN, INPUT);

  randomSeed(analogRead(0)); // nothing connected to 0 so read sees noise

  // Main Menu
  MenuItem playItem("Play", GAMESELECTION_ID);
  MenuItem aboutItem("About", ABOUT_ID);

  screens[0] = new Menu(MAINMENU_ID, "Gameboy");
  screens[0]->addItem(playItem);
  screens[0]->addItem(aboutItem);

  // Game selection
  MenuItem dinoItem("Dino Game", DINOGAME_ID);
  MenuItem carItem("Car Game", CARGAME_ID);
  MenuItem snakeItem("Snake Game", SNAKEGAME_ID);

  screens[1] = new Menu(GAMESELECTION_ID, "Games");
  screens[1]->addItem(dinoItem);
  screens[1]->addItem(carItem);
  screens[1]->addItem(snakeItem);

  // Dino game
  screens[2] = new DinoGame(DINOGAME_ID);

  // About
  screens[3] = new AboutScreen(ABOUT_ID);

  //Car + snake game
  screens[4] = new Car(CARGAME_ID);
  screens[5] = new snakegame(SNAKEGAME_ID);

  EEPROM.write(EEPROMAdd_dino_highscore, 0);
  EEPROM.write(EEPROMAdd_car_highscore, 0);
  EEPROM.write(EEPROMAdd_snake_highscore, 0);

  // Display splashscreen
  display.display();
  delay(2000);
  display.clearDisplay();

  // Show bitmap image
  display.clearDisplay();
}

//*** LOOP function ***//

void loop() {
  screens[Screen::current_screen_id]->process();
  screens[Screen::current_screen_id]->draw();

  display.display();
  display.clearDisplay();
}

