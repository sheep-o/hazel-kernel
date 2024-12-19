#ifndef HAZEL_TIMER_H_
#define HAZEL_TIMER_H_

#define PIT_CHAN0_DATA 0x40
#define PIT_CHAN1_DATA 0x41
#define PIT_CHAN2_DATA 0x42
#define PIT_MODE_CMD   0x43

#define PIT_MODE0  0
#define PIT_MODE2  (2<<1)
#define PIT_LOBYTE (1<<4)
#define PIT_LOHIGH (3<<4)
#define PIT_CHAN0  0

#define PIT_FREQUENCY 1193182
#define DESIRED_FREQUENCY 1000
#define DIVISOR (PIT_FREQUENCY/DESIRED_FREQUENCY)

void tmr_init(void);
int tmr_pit_handler(void);

#endif
