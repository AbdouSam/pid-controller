#include <stdint.h>
#include <stdbool.h>

#include "pid_ctrl.h"

void pid_calculate_ki_kd(pid_t *pid)
{
  if(pid->Ti < FLOAT_EPSILON)
  {
    pid->ki = 0;
  }
  else
  {
    pid->ki = pid->kp * pid->T / pid->Ti;
  }

  if(pid->T < FLOAT_EPSILON)
  {
    pid->kd = 0;
  }
  else
  {
    pid->kd = pid->kp * pid->Td / pid->T;
  }
}

void pid_init(pid_t *pid)
{

}

void pid_control(pid_t *pid)
{
  float ekt, pkt, qkt, ukt, ykt;

  static float ekt_1 = 0;
  static float pkt_1 = 0;
  
  ykt = pid->in_conv(*pid->in);

  if(pid->direct)
  {
    ekt = ykt - pid->set_point;
  }
  else
  {
    ekt = pid->set_point - ykt;
  }

  pkt = pid->ki * ekt + pkt_1; 
  qkt = pid->kd * (ekt - ekt_1); 
  ukt = pid->kp * ekt + pkt + qkt + pid->maint;

  if(ukt > pid->max_out)
  {
    pkt = pkt_1;
    ukt = pid->max_out;
  }
  else if(ukt < pid->min_out)
  {
    pkt = pkt_1;
    ukt = pid->min_out;
  }

  /* apply */
  *pid->out = pid->out_conv(ukt);

  pkt_1 = pkt;
  ekt_1 = ekt_1;
}

void pid_zigler_nicols_identification(zig_param *param)
{
  /*
   *  read input  
   *  Write a step to output 
   *  Start timer 
   *  record the point where small change has happened
   *  wait for steady state
   *  stop timer 
   * K = max - min;
   * T_dead = Timer_dead;
   * T_steady = Timer_steady;
   *
   * P   : kp = T_steady / (K * T_dead), Ti = 0, Td = 0;
   * PI  : kp = 0.9 * T_steady / (K * T_dead), Ti = 3.3 * T_dead, Td = 0;
   * PID : kp = 1.2 * T_steady / (K * T_dead), Ti = 2.0 * T_dead, Td = 0.5 * T_dead;
   */
}