programming features of this demo (11)
---

Mostly like demo10, changes:

**main.c**:
1. LED1 and LED2 from the option board have PWM capability, in this case 
   PWM mode 2 is used for TPU3, where PB0 is channel A and PB2 is channel C. 
   Channel D is used for PWM-period and timekeeping for pusb-button sampling.
1. dimmer_do() takes care of updating pwm values as with pwm mode 2 no 
   synchronisation is needed except in two cases:
   1. if switching from off to on with a small pwm value the match moment 
      could have passed and then the LED would be on for too much of the 
      current cycle. The timer is re-initialised to ensure init as well match 
      in the current period.
   1. if switching off when the pwm value was at least the period value there 
      was no match and the LED is on. The timer is re-initialised to ensure
      init at the off-value, regardless of the pwm-setting before off.
1. The interrupt routine now only has timing for the push-button events.
