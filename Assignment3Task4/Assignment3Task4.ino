//Task4//
#define AnalogueInput 32 //pin number
int AnalogueRead=0;
#define Task4OutputPin 18
////////
void setup() {
  // put your setup code here, to run once:
  //Task4 Setup//
  Serial.begin(115200);
   pinMode(AnalogueInput, INPUT);
   pinMode(Task4OutputPin, OUTPUT);
     xTaskCreatePinnedToCore(
    Task4
    ,  "Task4"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
  //////////////

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
Serial.print(AnalogueRead);
Serial.println();
vTaskDelay(41);
  }
}
