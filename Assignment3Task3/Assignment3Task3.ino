//Task3//
#define squarewavein 19 //Square wave input pin number.
unsigned long SquarewaveStart=0;
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
    Frequency=0;
    SquarewaveState=digitalRead(squarewavein); // Saving the initial state of the square wave.
  LastSquarewaveState=SquarewaveState;
 SquarewaveStart=micros();
 while (micros()-SquarewaveStart<=100000){
    while (SquarewaveState==LastSquarewaveState){
    SquarewaveState = digitalRead(squarewavein); // We read until the state changes
  }
  LastSquarewaveState=SquarewaveState;
  Frequency=Frequency+1; //Once a change is registered, we add one to the counter, this is up and down so will double our frequency so will need to half it at the end.
      while (SquarewaveState==LastSquarewaveState){
    SquarewaveState = digitalRead(squarewavein); // We read until the state changes
  }
  LastSquarewaveState=SquarewaveState;
 }
 Frequency=Frequency*10;
 Serial.printf( "Frequency is %d, ", Frequency);
 Serial.println();
 vTaskDelay(900);
}
}
