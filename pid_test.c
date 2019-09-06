#include <stdint.h>
#include <stdbool.h>

volatile static bool pid_flag = false;

INTERRUPT_TIMER_Vector_Routine()
{
  pid_flag = true;
}

int main(int argc, char const *argv[])
{
	/* Using Zigler nichols on a heating system we have
   * Unit step response gives
   * K = 50 °C
   * T_dead = 30 s
   * T_stdy = 230 s
   * 
   */
  float analog_temp;
  float pwm_heater;

  pid_t pid_0 = 
  {
    14.5f,         /* kp */
    2.4f,          /* ki */
    43.5f,         /* kd */
    0,             /* Ti, no need since we have ki calculated */
    0,             /* Td, no need since we have kd calculated */
    0,             /* maintain = 0 */
    5.0f,          /* Sampling time 5 second */
    30.0,          /* Set point 30 °C */
    false,         /* Reverse mode*/
    &analog_temp,  /* pointer to sensor input */
    &pwm_heater,   /* pointer to output */
    10.0f,         /* max 10.0V */
    0.0f,          /* min 0.0V */
    NULL;          /* No conversion needed in input */
    NULL;          /* No conversion needed in output */
  };

  adc_setup(CHANNEL_0);
  dac_setup(PWM_0);
  timer_setup(TIMER0, pid_0.T);
  
  while(true)
  {

    if(pid_flag)
    {
      pid_flag = false;

      adc_read(CHANNEL_0, &analog_temp);

      pid_ctrl(pid_0)

      dac_write(PWM_0, pwm_heater);
    }
  }
  
	return 0;
}