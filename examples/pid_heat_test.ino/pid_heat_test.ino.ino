/*
 * In this example We have a heating control system
 * that Uses LM35 temperature sensor
 * And as actuator a linear Dimmer that takes
 * 0-5V input and generate 0-120V output
 * 
 * System Modeling :
 * Before modeling the system, we should define the system
 *           _____       _____
 *  w--(+-)-|_PID_|- u -|_SYS_|---> y
 *      ^________________________|
 *
 * In our case System is u-->Dimmer->Heater->Sensor->y
 * w : set point in Temperature unit [°C] thus y should be [°C]
 * u is in volts ranging [0 - 5V]
 * In implimenting the input_conversion and output_conversion methods
 * the PID should take [°C] in input and will output Volts.
 * The Output should then be converted to its ouput interface (PWM 0-255)
 * 
 * These conversion are done automatically in pid_control, when they
 * are referenced in the pid structure
 * 
 * Pin connection : 
 * LM35 VOUT pin -> A0
 * Dimmer command Pin -> 3 (PWM Out)
 */

#include "pid_ctrl.h"

static float input_conversion(float analog_input)
{
  /* 10bit ADC with 5.0V reference */
  float volts = analog_input * 5.0f / 1023.0f;

  /*  LM35 :  10mV (0.01V) for 1 °C */
  float temperature = volts * 1.0f / 0.01 ; 
  return temperature; /* [°C] */
}

static float output_conversion(float output /*[V]*/)
{
  /*  8bit DAC with 5V logic */
  uint16_t analog_output = output * 255 / 5.0;
  return analog_output;
} 

int sensorPin = A0;
int actuatorPin = 3;

float temperature;
float heaterValue;

pid_ctrl_t pid_0 = 
{
  0.15f,         /* kp */
  0.01f,         /* ki */
  0.1f,          /* kd */
  0,             /* Ti, no need since we have ki calculated */
  0,             /* Td, no need since we have kd calculated */
  0,             /* maintain = 0 */
  1.0f,          /* Sampling time 5 second */
  30.0,          /* Set point 30 °C */
  false,         /* Reverse mode*/
  {
    5.0f,                /* max 5.0V */
    0.0f,                /* min 0.0V */
    &input_conversion,   /* Analog Values to Temperature */
    &output_conversion,  /* Volts to Analog values */
  },
  0, /* Should be initialized to zero, never change them*/
  0, /* Should be initialized to zero, never change them*/
};
 
void setup() 
{
  Serial.begin(9600);
  pinMode(actuatorPin, OUTPUT);
  
}

void loop() {
  /* read the input on analog pin 0 */
  temperature = analogRead(sensorPin);
  
  /* print out the value you read */
  Serial.println(input_conversion(temperature));

  /* calculate the controlled output */
  heaterValue = pid_control(&pid_0, (float)temperature);

  /* Apply the output */
  analogWrite(actuatorPin, heaterValue);

  /* Print the ouput */
  Serial.println(heaterValue);

  /* Wait for Sampling time */
  delay(pid_0.T*1000);  
}
