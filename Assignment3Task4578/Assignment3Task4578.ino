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
//Task7//
 int half_of_maximum_range_for_analogue_input = 2048; //Maximum is 4095.
 int error_code=0;
/////////
//Task8//
#define RedLED 26//insert green pin number
////////
static const uint8_t AnalogueQueueLength = 3;
static const uint8_t AverageAnalogueQueueLength = 1;
static const uint8_t ErrorCodeQueueLength = 1;
//Global Variable
static QueueHandle_t AnalogueQueue;
static QueueHandle_t AverageAnalogueQueue;
static QueueHandle_t ErrorCodeQueue;

void setup() {
  // put your setup code here, to run once:
  //Task4 Setup//
  Serial.begin(115200);
   pinMode(AnalogueInput, INPUT);
   pinMode(Task4OutputPin, OUTPUT);
     xTaskCreatePinnedToCore(
    Task4
    ,  "Task4"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
  //////////////
  //Task5 Setup//
    xTaskCreatePinnedToCore(
    Task5
    ,  "Task5"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  0);
  //////////////
      //Task7 Setup//
    xTaskCreatePinnedToCore(
    Task7
    ,  "Task7"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
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
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
  //////////////

  //Create queue
  AnalogueQueue = xQueueCreate(AnalogueQueueLength,sizeof(int));
  AverageAnalogueQueue = xQueueCreate(AverageAnalogueQueueLength,sizeof(int));
  ErrorCodeQueue = xQueueCreate(ErrorCodeQueueLength,sizeof(int));
}

void loop() {
  // put your main code here, to run repeatedly:

}
void Task4(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
AnalogueRead=analogRead(AnalogueInput);
Serial.printf( "Analogue input is %d. \n", AnalogueRead);
if (xQueueSend(AnalogueQueue, &AnalogueRead,20)!=pdTRUE){
Serial.println("Queue full");
  }
  vTaskDelay(41);
}
}
void Task5(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
   if (xQueueReceive(AnalogueQueue, &AnalogueRead, 0)==pdTRUE){ //Does this task only when there has been a value fully received in the queue.
  Prev4AnaInput = Prev3AnaInput; //4th last input
  Prev3AnaInput = Prev2AnaInput; //3rd last input
  Prev2AnaInput = Prev1AnaInput; //2nd last input
  Prev1AnaInput = AnalogueRead; //Last input
  AverageAnaInput = (Prev4AnaInput+Prev3AnaInput+Prev2AnaInput+Prev1AnaInput)/4; // Mean average.

  
  Serial.printf( "Average Analogue input is %d. \n", AverageAnaInput);
Serial.println();
  if (xQueueSend(AverageAnalogueQueue, &AverageAnaInput,20)!=pdTRUE){
  }
vTaskDelay(41);
   }
  }
}

void Task7(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
    if(xQueueReceive(AverageAnalogueQueue, &AverageAnaInput,0)!=pdTRUE){
  if (AverageAnaInput > half_of_maximum_range_for_analogue_input){
    error_code = 1;
  } else {
    error_code = 0;
  }
  vTaskDelay(333);
  }
  
}
}
void Task8(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
    if(xQueueReceive(ErrorCodeQueue, &error_code,0)!=pdTRUE){
  if (error_code==1){
    digitalWrite(RedLED, HIGH);
  } else {
    digitalWrite(RedLED, LOW);
  }
  vTaskDelay(333);
  }
  }
  
}
