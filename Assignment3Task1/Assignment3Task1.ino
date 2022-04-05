//Task1 setup//
#define GreenLED 27//insert green LED pin
///////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Task1 Setup//
  pinMode(GreenLED, OUTPUT);
    xTaskCreatePinnedToCore(
    Task1
    ,  "Task1"   // A name just for humans
    ,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0); //CPU core
  //////////////
}

void loop() {}
  // put your main code here, to run repeatedly:
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
