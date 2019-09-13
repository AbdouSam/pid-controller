#ifndef _PID_CTRL_H
#define _PID_CTRL_H

#include <stdint.h>
#include <stdbool.h>


#define FLOAT_EPSILON 0.00001f

typedef struct
{

  float max_out;            /* PID maximum output */
  float min_out;            /* PID minimum output */
  float (*in_conv)(float);  /* Sensor equation adc to unit */
  float (*out_conv)(float); /* convert to dac or pwm */

}pid_io_t;

typedef struct 
{

  float kp;              /* Proportional*/
  float ki;              /* ki = kp *T / Ti  */
  float kd;              /* kd = kp * Td / T */
  float Ti;              /* Integral */     
  float Td;              /* Derivetive */     
  float maint;           /* Maintain value */
  float T;               /* Sampling time seconds*/
  float spt;             /* Set point */
  bool  direct;          /* reverse or direct mode */

  pid_io_t io; 
  float ekt_1;           /* Previous error state */
  float pkt_1;           /* Pervious integral state */

}pid_ctrl_t;

typedef struct
{

  float Kc;           /* System Gain */
  float lag;          /* System deadtime */
  float st;           /* System steady time */
  float step_value;   /* The step output amplitude */

}pid_zig_t;

/**
 * @brief Calculate sub-parameters ki and kd, from Ti and Td
 *
 * The parameters definition ki and kd may change from formula
 * to another, it is better to have Ti and Td, then calculate
 * ki and kd using this function.
 *
 * @param pid pointer to pid instance
 *
 * @return @c void
 */

void pid_calculate_ki_kd(pid_ctrl_t *pid);

/**
 * @brief initialization function
 *
 * Do nothing for now
 *
 */

void pid_init(pid_ctrl_t *pid);

/**
 * @brief Calculate the output of pid controller from the current input
 *
 * The pid perform the actual calculation of the command signal, from 
 * the sensor reading (input), and genrate the output, which should be
 * applied right a way after this function
 *
 * @param pid pointer to pid instance
 *
 * @param input the sensor reading (data conversion is done automatically
 *         if the in_conv method is provided)
 *
 * @return output the pid output after data conversion (out_conv)
 *
 * Example of usage
 * @code
 * pid_0.in_conv  = &in_conv_func;
 * pid_0.out_conv = &out_conv_func;
 * input = analog_read(channel_0);
 * output = pid_control(pid_0, input);
 * analog_write(channel_1, output);
 * @endcode
 */

float pid_control(pid_ctrl_t *pid, float input);

/**
 * @brief Start a zigleer_nichols RMP Identification
 *
 * The system should be set before calling this routine. The function
 * will output a step signal and start recording the input from the 
 * sensor to calculate the Dead time, the gain, and the steady state time
 *
 * @param pid_zig the identification parameters the function will find
 * 
 * @param pid the pid parameters kp ki and kd will be updated if the 
 *            identification is successful
 *
 * @return @c void
 */

void pid_zigler_nicols_identification(pid_zig_t *param, pid_ctrl_t *pid);

#endif