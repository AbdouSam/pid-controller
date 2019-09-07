# pid-controller

PID implimentation for embedded

### Intro 
    The PID formula for this source is 
    
    u = kp*e + ki*e_intgral + kd*e_derivative + maintain_value
    
    where:
    e = set_point - y : the error
    y : system output or sensor reading
    e_integral = e_integral + e : integral of error
    e_derivative = e_derivative - e : derivitave of error
    u : command or control signal to the actuator

### pid_struct 
	In the Pid struct kp, ki and kd. are the main parameters
	Ti and Td can be given to calculate ki and kd instead, using
	pid_calculate_ki_kd();

	the IO struct in the PID handels the input and output where
	in_conv is called perior to pid calculation to do any unit conversion
	(Volt to temperature, or analog to volt, ...)
	out_conv is called after pid output is calculated to convert the entity 
	back its interface value.

### Some Notes
	The Physical Unit can confusing to work with and thus the in_conv and 
	out_conv methods should be implimented with care, you can see some 
	examples in the examples folder.

### Zigler Nichols
  Zigler Nichols tunining method, is an heuristic method to tune
  a PID controller of a stable system in an open loop configuration.
  Tuning rules work quite well when you have a system that is linear,
  monotonic, and sluggish, and a response that is dominated by a 
  single-pole exponential 'lag'.





