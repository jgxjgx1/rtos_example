#include <Arduino_FreeRTOS.h>

// define tasks
void ReadAddition( void *pvParameters );
void AutoDecrease( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  xTaskCreate(
    ReadAddition
    ,  "ReadAddition"   // A name just for humans
    ,  256  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    AutoDecrease
    ,  "AutoDecrease"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

int dataNumber = 0;
int lukema = 0;

void ReadAddition(void *pvParameters)
{
  (void) pvParameters;
  // read a number from serial port as char and add to a variable as int

  for (;;)
  {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    if (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
        }
      dataNumber = 0;
      dataNumber = atoi(receivedChars);
      lukema += dataNumber;
    }
    vTaskDelay(10);
  }
}

void AutoDecrease(void *pvParameters)
{
  (void) pvParameters;
 // decrease the value every second, print value to serial monitor if greater than 0.
  for (;;)
  {
    lukema--;
    if(lukema > 0) Serial.println(lukema);
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}