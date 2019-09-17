#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "pid_ctrl.h"
/*
 * TODO:
 * - add first order filtering to derivetive
 * - 
 */

void pid_calculate_ki_kd(pid_ctrl_t *pid, float Ti, float Td)
{
  if(Ti < FLOAT_EPSILON)
  {
    pid->ki = 0;
  }
  else
  {
    pid->ki = pid->kp * pid->T / Ti;
  }

  if(pid->T < FLOAT_EPSILON)
  {
    pid->kd = 0;
  }
  else
  {
    pid->kd = pid->kp * Td / pid->T;
  }
}

void pid_init(pid_ctrl_t *pid)
{

}

float pid_control(pid_ctrl_t *pid, float input)
{
  float ekt, pkt, qkt, ukt, ykt;

  if(pid->io.in_conv != NULL)
  {
    ykt = pid->io.in_conv(input);
  }
  else
  {
    ykt = input;
  }

  if(pid->direct)
  {
    ekt = ykt - pid->spt;
  }
  else
  {
    ekt = pid->spt - ykt;
  }

  pkt = pid->ki * ekt + pid->pstate.pkt_1; 
  qkt = pid->kd * (ekt - pid->pstate.ekt_1); 
  /* Filter the derivative */
  qkt = (1 - pid->qcoeff) * qkt + pid->qcoeff * pid->pstate.qkt_1;
  ukt = pid->kp * ekt + pkt + qkt + pid->maint;

  if(ukt > pid->io.max_out)
  {
    pkt = pid->pstate.pkt_1;
    ukt = pid->io.max_out;
  }
  else if(ukt < pid->io.min_out)
  {
    pkt = pid->pstate.pkt_1;
    ukt = pid->io.min_out;
  }

  pid->pstate.pkt_1 = pkt;
  pid->pstate.ekt_1 = ekt;
  pid->pstate.qkt_1 = qkt;

  if(pid->io.out_conv != NULL)
  {
    return pid->io.out_conv(ukt);
  }
  else
  {
    return ukt;
  }
}

void pid_zigler_nicols_identification(pid_zig_t *param, pid_ctrl_t *pid)
{
  /*
   * Read input  
   * Write a step to output 
   * Start timer 
   * Record the point where small change has happened
   * Wait for steady state
   * Stop timer 
   * K = max - min;
   * T_dead = Timer_dead;
   * T_steady = Timer_steady;
   *
   * P   : kp = T_steady / (K * T_dead), Ti = 0, Td = 0;
   * PI  : kp = 0.9 * T_steady / (K * T_dead), Ti = 3.3 * T_dead, Td = 0;
   * PID : kp = 1.2 * T_steady / (K * T_dead), Ti = 2.0 * T_dead, Td = 0.5 * T_dead;
   */
  
  /* Get initial steady state ykt (physical unit) save as k_initial */
  
  /*  Apply a step of amplitude ukt + k_step */

  /* Start a timer */

  /* get the lag */
  /*  read continuously until the input exceed the noise tolerace value)*/

  /*  read the time at this point as the 'lag' */

  /* Get the Nmax (maximum slop point) */

  /* read continously every step time Ts the input and calculate the slopes  */

  /* N = (ykt - ykt_1) / Ts */

  /* compare N with N_1 and save the bigger (time, amplitude) */

  /* Do that until N reaches zero, no change, steady state */

  /*  at this point we have a tuple (stime, amplitude) */

  /*  Kc = k_step / (amplitude - k_initial) */
  /*  lag = lag */
  /*  st = stime */
}