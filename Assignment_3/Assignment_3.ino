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



//Task1 setup//
#define GreenLED 27//insert green LED pin
///////////////

//Task2//
#define Button 13 //insert Button pin
int ButtonState = 0;
/////////

//Task3//
#define squarewavein 19 //Square wave input pin number.
unsigned long SquarewaveStart = 0;
unsigned long SquarewaveEnd = 0;
unsigned long Task4StartTime = 0;
unsigned long Task4Length = 0;
unsigned long Frequency = 0;
int SquarewaveState = 0;
int LastSquarewaveState = 0;
/////////

//Task4//
#define AnalogueInput 32 //pin number
int AnalogueRead = 0;
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
int error_code = 0;
int AverageAnaInput7 = 0;
/////////

//Task8//
#define RedLED 26//insert green pin number
int error_code8 = 0;
////////

//Task9//
int ButtonStatePrint = 0;
int FrequencyPrint = 0;
int AverageAnaInputPrint = 0;
////////

static const uint8_t AnalogueQueueLength = 2; //Queue length of the Analogue queue
static const uint8_t ErrorCodeQueueLength = 1; //Queue length of the error code queue

//Global Variables
static QueueHandle_t AnalogueQueue;  //Analogue queue name
static QueueHandle_t ErrorCodeQueue; //Error code queue name


struct PrintedVariables { // These are our global variables, only accessed using semaphores to protect them
  int ButtonStateGlobal = 0;
  int FrequencyGlobal = 0;
  int AverageAnalogueInputGlobal = 0;
} PrintedStuff;
SemaphoreHandle_t FreqSem = xSemaphoreCreateBinary(); //Create Frequency Semaphore
SemaphoreHandle_t ButtonSem = xSemaphoreCreateBinary(); // CreateButton Semaphore
SemaphoreHandle_t AnalogueSem = xSemaphoreCreateBinary(); // Create Filtered analogue Semaphore


//CHANGE FREQUENCYS
TickType_t Task1Freq = 34; //Work this out
TickType_t Task2Freq = 200;
TickType_t Task9Freq = 5000;


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
  
  Serial.begin(115200); // Start serial

  //Create queues
  AnalogueQueue = xQueueCreate(AnalogueQueueLength, sizeof(int));
  ErrorCodeQueue = xQueueCreate(ErrorCodeQueueLength, sizeof(int));


  //Task1 Setup//
  pinMode(GreenLED, OUTPUT);
  xTaskCreatePinnedToCore(
    Task1
    ,  "Task1"   // A name just for humans
    ,  570 //568 is high stack watermark
    // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task2 Setup//
  pinMode(Button, INPUT);
  xTaskCreatePinnedToCore(
    Task2
    ,  "Task2"   // A name just for humans
    ,  515 // 492 is HSW
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task3 Setup//
  pinMode(squarewavein, INPUT);
  xTaskCreatePinnedToCore(
    Task3
    ,  "Task3"   // A name just for humans
    ,  525  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task4 Setup//
  pinMode(AnalogueInput, INPUT);
  pinMode(Task4OutputPin, OUTPUT);
  xTaskCreatePinnedToCore(
    Task4
    ,  "Task4"   // A name just for humans
    ,  570  // 560 HSW
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task5 Setup//
  xTaskCreatePinnedToCore(
    Task5
    ,  "Task5"   // A name just for humans
    ,  510  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task6 Setup//
  xTaskCreatePinnedToCore(
    Task6
    ,  "Task6"   // A name just for humans
    ,  470  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task7 Setup//
  xTaskCreatePinnedToCore(
    Task7
    ,  "Task7"   // A name just for humans
    ,  520  
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task8 Setup//
  pinMode(RedLED, OUTPUT);
  xTaskCreatePinnedToCore(
    Task8
    ,  "Task8"   // A name just for humans
    ,  530  //
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////

  //Task9 Setup//
  xTaskCreatePinnedToCore(
    Task9
    ,  "Task9"   // A name just for humans
    ,  850  // Stack needs to be this high, stack is much higher when button is pressed and 
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////


}

void loop() {
  // put your main code here, to run repeatedly:

}

// Task 1
void Task1(void *pvParameters)  
{
  (void) pvParameters;


  for (;;)
  {
    digitalWrite(GreenLED, HIGH); // LED on
    delayMicroseconds(50);
    digitalWrite(GreenLED, LOW); // LED off
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(Task1Freq); // Delays task for required length of time until it is needed to be executed again
  }
}

// Task 2
void Task2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {

    ButtonState = digitalRead(Button); //Tells when Button is pressed
    xSemaphoreTake(ButtonSem, 5);
    PrintedStuff.ButtonStateGlobal = ButtonState;
    xSemaphoreGive(ButtonSem);
    //Serial.println(uxTaskGetStackHighWaterMark(NULL));
    vTaskDelay(Task2Freq); // Delays task for required length of time until it is needed to be executed again
  }
}


// Task 3
void Task3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    SquarewaveState = digitalRead(squarewavein); // Saving the initial state of the square wave.
    LastSquarewaveState = SquarewaveState;
    Task4StartTime = micros();

    while (SquarewaveState == LastSquarewaveState && micros() - Task4StartTime < 4000) {
      SquarewaveState = digitalRead(squarewavein); // We read until the state changes
    }

    SquarewaveStart = micros(); // Save this start time.
    LastSquarewaveState = SquarewaveState; // Save start state.
    while (SquarewaveState == LastSquarewaveState && micros() - Task4StartTime < 4000) { //Read again until state changes again
      SquarewaveState = digitalRead(squarewavein);
    }
    SquarewaveEnd = micros(); // Save the end time.
    if (micros() - Task4StartTime >= 4000) {
      Frequency = 0; //frequency less than 500HZ
      Serial.println("No square wave input detected, ");
    } else {
      Frequency = 1000000 / (2 * (SquarewaveEnd - SquarewaveStart)); // Calculate frequency
    }
    xSemaphoreTake(FreqSem, 5);
    PrintedStuff.FrequencyGlobal = Frequency;
    xSemaphoreGive(FreqSem);
    Task4Length = (micros() - Task4StartTime) / 1000; //Task4length in millis
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(1000 - Task4Length); // Delays task for required length of time until it is needed to be executed again
  }
}


// Task 4
void Task4(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    AnalogueRead = analogRead(AnalogueInput); // Reads analogue input
    if (xQueueSend(AnalogueQueue, &AnalogueRead, 20) != pdTRUE) { //Sends analogue input to analogue queue
      Serial.println("Queue 4 full"); // If queue is full this is posted to serial monitor
    }
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(41); // Delays task for required length of time until it is needed to be executed again
  }
}


// Task 5 
void Task5(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    if (xQueueReceive(AnalogueQueue, &Prev1AnaInput, 0) == pdTRUE) { //Does this task only when there has been a value fully received in the analogue queue, sent by task 4.
      Prev4AnaInput = Prev3AnaInput; //4th last input
      Prev3AnaInput = Prev2AnaInput; //3rd last input
      Prev2AnaInput = Prev1AnaInput; //2nd last input

      AverageAnaInput = (Prev4AnaInput + Prev3AnaInput + Prev2AnaInput + Prev1AnaInput) / 4; // Mean average.

      // Save to global variables using Analogue semaphore for tasks 7,9
      xSemaphoreTake(AnalogueSem, 5);
      PrintedStuff.AverageAnalogueInputGlobal = AverageAnaInput;
      xSemaphoreGive(AnalogueSem); 

      //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW

      vTaskDelay(41); // Delays task for required length of time until it is needed to be executed again
    }
  }
}


// Task 6
void Task6(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    for (int i = 1; i <= 1000; i++) {
      __asm__ __volatile__ ("nop");
    }
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(100); // Delays task for required length of time until it is needed to be executed again
  }
}


// Task 7
void Task7(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    xSemaphoreTake(AnalogueSem, 5); //Reads average analogue input from global variable using a semaphore to protect it
    AverageAnaInput7 = PrintedStuff.AverageAnalogueInputGlobal;
    xSemaphoreGive(AnalogueSem);
    if (AverageAnaInput7 > half_of_maximum_range_for_analogue_input) {
      error_code = 1;
    } else {
      error_code = 0;
    }
    if (xQueueSend(ErrorCodeQueue, &error_code, 20) != pdTRUE) { // Sends the error code to the error code queue, sends to task 8.
      Serial.print("Queue 7 Full");
    }
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(333); // Delays task for required length of time until it is needed to be executed again
  }
}


// Task 8
void Task8(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    if (xQueueReceive(ErrorCodeQueue, &error_code8, 0) != pdTRUE) {
      if (error_code8 == 1) {
        digitalWrite(RedLED, HIGH);
      } else {
        digitalWrite(RedLED, LOW);
      }
      //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
      vTaskDelay(333); // Delays task for required length of time until it is needed to be executed again
    }
  }
}


// Task 9
void Task9(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;)
  {
    // Uses all the semaphores to read the global variables.
    xSemaphoreTake(ButtonSem, 5);
    ButtonStatePrint = PrintedStuff.ButtonStateGlobal;
    xSemaphoreGive(ButtonSem);
    xSemaphoreTake(FreqSem, 5);
    FrequencyPrint = PrintedStuff.FrequencyGlobal;
    xSemaphoreGive(FreqSem);
    xSemaphoreTake(AnalogueSem, 5);
    AverageAnaInputPrint = PrintedStuff.AverageAnalogueInputGlobal;
    xSemaphoreGive(AnalogueSem);
    if (ButtonStatePrint == 1) { // Prints variables iff button is pressed
      Serial.printf( "Button State is %d, ", ButtonStatePrint);
      Serial.printf( "Frequency is %d, ", FrequencyPrint);
      Serial.printf( "Average Analogue input is %d. \n", AverageAnaInputPrint);
    }
    //Serial.println(uxTaskGetStackHighWaterMark(NULL)); //Used to test HSW
    vTaskDelay(Task9Freq); // Delays task for required length of time until it is needed to be executed again

  }
}
