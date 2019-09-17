#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "pid_ctrl.h"

/* PID Control heating system using LM35V and BTA08-600
 * with heating element.
 * the system is modeled as G(s) = Y(s) / U(s).
 * Y(s) : system output in °C (temperature)
 * U(s) : system input (command) in V (0V - 5V ) 
 *
 * The PID parameters and calcultaed on the basis of the system
 * units PID is modeled as R(s) = U(s) / (W(s) - Y(s))
 * W(s) is the set point measured in °C
 */

/* The input is LM35 1 °C for 10mV */
float ambien_temp = 20;


float input_conversion(float analog_input)
{
  /* 10bit ADC with 5.0V reference */

  float volts = analog_input * 5.0f / 1023.0f;

  /*  LM35 :  100mV (0.1V) for 1 °C */

  float temperature __attribute__ ((unused)) = volts * 1.0f / 0.01 ; 
  
  return analog_input;
}

float output_conversion(float output)
{
  /*  8bit DAC with 5V logic */
  uint16_t analog_output = output * 255 / 5.0;

  return analog_output;
} 

uint16_t analogRead(int pin)
{
  return ambien_temp;
}

void analogWrite(int pin, int output)
{

}

int main(int argc, char const *argv[])
{

  float analog_temp;
  float pwm_heater;

  int sensorPin     = 1;
  int actuatorPin   = 2;
  
  pid_ctrl_t pid_0 = 
  {
    0.15f,         /* kp */
    0.01f,         /* ki */
    0.1f,          /* kd */
    0,             /* maintain = 0 */
    5.0f,          /* Sampling time 5 second */
    30.0,          /* Set point 30 °C */
    false,         /* Reverse mode*/
    0.02,          /* Derivative coeff*/
    {
      5.0f,                /* max 5.0V */
      0.0f,                /* min 0.0V */
      &input_conversion,   /* If No conversion needed set to NULL */
      &output_conversion,  /* If No conversion needed set to NULL */
    }
  };

  while(true)
  {

    printf("Getting Temperature.\n");
    analog_temp = analogRead(sensorPin); 

    printf("Temperature = %d \n", (int)analog_temp );

    pwm_heater = pid_control(&pid_0, (float)analog_temp);

    printf("PID output = %0.2f\n",pwm_heater );

    analogWrite(actuatorPin, pwm_heater);

    for(int i = 0; i < 5000000; i++)
    {

    }
    ambien_temp+=0.5;

    if(ambien_temp > pid_0.spt + 2)
    {
      break;
    }
  }
	return 0;
}