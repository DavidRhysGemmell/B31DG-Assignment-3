//Task3//
#define squarewavein 19 //Square wave input pin number.
unsigned long SquarewaveStart=0;
unsigned long SquarewaveEnd=0;
unsigned long Task4StartTime = 0;
unsigned long Task4Length = 0;
unsigned long Frequency = 0;
int SquarewaveState = 0;
int LastSquarewaveState = 0;
/////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Task3 Setup//
   pinMode(squarewavein, INPUT);
     xTaskCreatePinnedToCore(
    Task3
    ,  "Task3"   // A name just for humans
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


void Task3(void *pvParameters)  // This is a task.
{
  (void) pvParameters;


  for (;;) 
  {
  SquarewaveState=digitalRead(squarewavein); // Saving the initial state of the square wave.
  LastSquarewaveState=SquarewaveState;
  Task4StartTime=micros();
  
  while (SquarewaveState==LastSquarewaveState && micros()-Task4StartTime<4000){
    SquarewaveState = digitalRead(squarewavein); // We read until the state changes
  }
  
    SquarewaveStart=micros(); // Save this start time.
    LastSquarewaveState=SquarewaveState; // Save start state.
    while (SquarewaveState==LastSquarewaveState && micros()-Task4StartTime<4000){ //Read again until state changes again
      SquarewaveState = digitalRead(squarewavein);  
    }
      SquarewaveEnd = micros(); // Save the end time.
      if (micros()-Task4StartTime>=4000){
        Frequency=0; //frequency less than 500HZ
        Serial.printf("No square wave input detected, ");
      } else {
      Frequency = 1000000/(2*(SquarewaveEnd-SquarewaveStart)); // Calculate frequency     
  }
  Task4Length=(micros()-Task4StartTime-2)/1000; //Task4length in millis
 Serial.printf( "Frequency is %d, ", Frequency);
 Serial.println();
 vTaskDelay(1000-Task4Length); //Suitable delay
}
}
