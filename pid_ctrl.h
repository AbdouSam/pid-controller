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

  float Kc;  /* System Gain */
  float lag; /* System deadtime */
  float st;  /* System steady time */

}pid_zig_t;

void pid_calculate_ki_kd(pid_ctrl_t *pid);
void pid_init(pid_ctrl_t *pid);
float pid_control(pid_ctrl_t *pid, float input);
void pid_zigler_nicols_identification(pid_zig_t *param, pid_ctrl_t pid);

#endif