#ifndef _PID_CTRL_H
#define _PID_CTRL_H

#include <stdint.h>
#include <stdbool.h>

#define FLOAT_EPSILON 0.00001f

typedef struct 
{
  float   kp;           /* Proportional*/
  float   ki;           /* ki = kp *T / Ti  */
  float   kd;           /* kd = kp * Td / T */
  float   Ti;           /* Integral */     
  float   Td;           /* Derivetive */     
  float   maintain = 0; /* Maintain value */
  float   T;            /* Sampling time */
  float   set_point;    /* Set point */
  bool    direct = false;

  float   *in;          /* PID input or System Output */
  float   *out;         /* PID output or System input */
  
  float   max_out;      /* PID maximum output */
  float   min_out;      /* PID minimum output */

  float (*in_conv)(float);  /* Sensor equation adc to unit */
  float (*out_conv)(float); /* convert to dac or pwm */
}pid_t;

typedef struct
{
  
}zig_param_t;

void pid_calculate_ki_kd(pid_t *pid);
void pid_init(pid_t *pid);
void pid_control(pid_t *pid);
void pid_zigler_nicols_identification(zig_param_t *param);

#endif