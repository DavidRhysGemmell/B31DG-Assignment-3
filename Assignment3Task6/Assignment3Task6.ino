void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  //Task6 Setup//
    xTaskCreatePinnedToCore(
    Task6
    ,  "Task6"   // A name just for humans
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
void Task6(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  for (int i=1; i<=1000; i++){
    __asm__ __volatile__ ("nop");
  }
  Serial.println("Task 6 Completed");
  vTaskDelay(100);
  }
}
