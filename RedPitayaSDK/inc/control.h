#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"

/* Define the Pin for signals and acquisitions and the size of the buffer */
#define PULSE_PIN RP_DIO1_P
#define PWM_PIN RP_DIO2_P
#define ACQUISITION_PIN RP_CH_1
#define RAMP_PIN RP_CH_2

/* Mandatory variable which represent the trigger state */
rp_acq_trig_state_t state;
pthread_t control_motor_thread;

/* The Thread controlling the motor */
void *control_motor (void *p_data);

/* The function initializing the control functions, setting everything up */
void init_control();

/* The function ending the control functions, setting everything down */
void end_control();

/* The function reseting the control functions, setting everything to zero */
void reset();

/* The function configuring the ramp used for controlling the TGC */
void configure_ramp();

/* The function configuring the PWM used for controlling the Motor */
void configure_pwm();

/* The function configuring the pulse used for controlling the Pulse */
void configure_pulse();

/* The function configuring the ADC used for acquiring the datas */
void configure_ADC();

/* The function sending a pulse */
void pulse();

/* The function sending a ramp */
void ramp();

/* The function acquiring datas through ADC */
float* acquireADC(uint32_t buff_size, float* temp);

#endif
