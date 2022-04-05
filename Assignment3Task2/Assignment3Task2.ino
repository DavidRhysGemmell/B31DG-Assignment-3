//Task2//
#define Button 13 //insert Button pin
int ButtonState=0;
/////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
     pinMode(Button, INPUT);
     xTaskCreatePinnedToCore(
    Task2
    ,  "Task2"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);
}

void loop() {}
  // put your main code here, to run repeatedly:
  void Task2(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
ButtonState=digitalRead(Button); //Tells when Button is pressed
Serial.printf( "Button State is %d, ", ButtonState);
Serial.println();
vTaskDelay(200); 

  }
}
