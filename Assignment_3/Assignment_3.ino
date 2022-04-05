//This program is composed of 9 different tasks at variable rates per second, executed using FreeRTOS;
// Task 1: 30Hz. Produces a 50microsecond pulse.
// Task 2: 5Hz. Monitors a digital input (Button).
// Task 3: 1Hz. Measures the frequency of a Square wave signal, 500Hz+ range, 50% duty cycle.
// Task 4: 24Hz. Reads an analogue input.
// Task 5: 24Hz. Takes an average of the last 4 recorded analogue inputs.
// Task 6: 10Hz. Executes __asm__ __volatile__ ("nop") 1000 times.
// Task 7: 3Hz. Produces an error code if the current analogue input is greater than half of the maximum range of the analogue input.
// Task 8: 3Hz. Visulaises the error code with an LED.
// Task 9: 0.2Hz. Prints to the serial port: Button state, Frequency of square wave, average analogue input.
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

struct GlobalVariables{
  
}.GVariables;
//Task1 setup//
#define GreenLED 27//insert green LED pin
///////////////

//Task2//
#define Button 13 //insert Button pin
int ButtonState=0;
/////////

//Task3//
#define squarewavein 19 //Square wave input pin number.
unsigned long SquarewaveStart=0;
unsigned long SquarewaveEnd=0;
unsigned long Frequency = 0;
int SquarewaveState = 0;
int LastSquarewaveState = 0;
/////////

//Task4//
#define AnalogueInput 32 //pin number
int AnalogueRead=0;
#define Task4OutputPin 18
////////

//Task5//
int Prev1AnaInput = 0;
int Prev2AnaInput = 0;
int Prev3AnaInput = 0;
int Prev4AnaInput = 0;
int AverageAnaInput = 0;
/////////

//Task6//
/////////

//Task7//
 int half_of_maximum_range_for_analogue_input = 2048; //Maximum is 4095.
 int error_code=0;
/////////

//Task8//
#define RedLED 26//insert green pin number
////////

//Task9//
////////

//Defines 9 tasks.
void Task1( void *pvParameters );
void Task2( void *pvParameters );
void Task3( void *pvParameters );
void Task4( void *pvParameters );
void Task5( void *pvParameters );
void Task6( void *pvParameters );
void Task7( void *pvParameters );
void Task8( void *pvParameters );
void Task9( void *pvParameters );


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Task1 Setup//
  pinMode(GreenLED, OUTPUT);
    xTaskCreatePinnedToCore(
    Task1
    ,  "Task1"   // A name just for humans
    ,  512  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task2 Setup//
   pinMode(Button, INPUT);
     xTaskCreatePinnedToCore(
    Task2
    ,  "Task2"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task3 Setup//
   pinMode(squarewavein, INPUT);
     xTaskCreatePinnedToCore(
    Task3
    ,  "Task3"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task4 Setup//
   pinMode(AnalogueInput, INPUT);
   pinMode(Task4OutputPin, OUTPUT);
     xTaskCreatePinnedToCore(
    Task4
    ,  "Task4"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task5 Setup//
    xTaskCreatePinnedToCore(
    Task5
    ,  "Task5"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task6 Setup//
    xTaskCreatePinnedToCore(
    Task6
    ,  "Task6"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////
 
  //Task7 Setup//
    xTaskCreatePinnedToCore(
    Task7
    ,  "Task7"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task8 Setup//
   pinMode(RedLED, OUTPUT);
     xTaskCreatePinnedToCore(
    Task8
    ,  "Task8"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  //////////////

  //Task9 Setup//
    xTaskCreatePinnedToCore(
    Task9
    ,  "Task9"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);
  ////////////// 
}

void loop() {
  // put your main code here, to run repeatedly:

}
void Task1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  digitalWrite(GreenLED, HIGH);
  vTaskDelay(50);
  digitalWrite(GreenLED, LOW);
  vTaskDelay(33700);
  }
}

void Task2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
ButtonState=digitalRead(Button); //Tells when Button is pressed
  }
}

void Task3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  SquarewaveState=digitalRead(squarewavein); // Saving the initial state of the square wave.
  LastSquarewaveState=SquarewaveState;
  while (SquarewaveState==LastSquarewaveState){
    SquarewaveState = digitalRead(squarewavein); // We read until the state changes
  }
  if (SquarewaveState != LastSquarewaveState){
    SquarewaveStart=micros(); // Save this start time.
    LastSquarewaveState=SquarewaveState; // Save start state.
    while (SquarewaveState==LastSquarewaveState){ //Read again until state changes again
      SquarewaveState = digitalRead(squarewavein);  
    }
      SquarewaveEnd = micros(); // Save the end time.
      Frequency = 1000000/(2*(SquarewaveEnd-SquarewaveStart)); // Calculate frequency     
  }
  }
}

void Task4(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
AnalogueRead=analogRead(AnalogueInput);
  }
}

void Task5(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  Prev4AnaInput = Prev3AnaInput; //4th last input
  Prev3AnaInput = Prev2AnaInput; //3rd last input
  Prev2AnaInput = Prev1AnaInput; //2nd last input
  Prev1AnaInput = AnalogueRead; //Last input
  AverageAnaInput = (Prev4AnaInput+Prev3AnaInput+Prev2AnaInput+Prev1AnaInput)/4; // Mean average.
  }
}

void Task6(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  for (int i=1; i<=1000; i++){
    __asm__ __volatile__ ("nop");
  }
  }
}

void Task7(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  if (AverageAnaInput > half_of_maximum_range_for_analogue_input){
    error_code = 1;
  } else {
    error_code = 0;
  }
  }
}

void Task8(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  if (error_code==1){
    digitalWrite(RedLED, HIGH);
  } else {
    digitalWrite(RedLED, LOW);
  }
  }
}

void Task9(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  Serial.printf( "Button State is %d, ", ButtonState);
  Serial.printf( "Frequency is %d, ", Frequency);
  Serial.printf( "Average Analogue input is %d. \n", AverageAnaInput);
  }
}
