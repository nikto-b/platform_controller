#include "avr-api/api.h"

#define DIR_TYPE    uint8_t
#define CW          1
#define ACW         0


#define M0_DPIN     18
#define M0_SPIN     0
#define M0_INVERT   0
#define M0_TICKS    0

#define M1_DPIN     21
#define M1_SPIN     0
#define M1_INVERT   0
#define M1_TICKS    0

#define M2_DPIN     0
#define M2_SPIN     0
#define M2_INVERT   0
#define M2_TICKS    0


class Motor
{
private:
    uint8_t         dpin,
                    spin,
                    inverted = 0,
                    paused = 0;
    uint16_t        ticksPerCycle;

    uint64_t        ticksRemained = 0;

    void            (*startTicking)(void) = NULL;

    void begin(void)
    {
        if(startTicking != NULL)
            startTicking();
    }

public:
    Motor(){}
    Motor(uint16_t ticksPerCycle, uint8_t dpin, uint8_t spin, void (*func)(void));
    void beginMoving(uint64_t ticks, DIR_TYPE dir);
    void pause(void);
    void unpause(void);
    void stop(void);
    void setDir(DIR_TYPE dir);
    DIR_TYPE getDir(void);
    void setDpin(uint8_t pin);
    void setSpin(uint8_t pin);
    void setTicksPerCycle(uint16_t ticks);
    void setTickingFunc(void (*func)(void));
    void setInverted(uint8_t state);
};



void m0BeginTick(void);
void m1BeginTick(void);
void m2BeginTick(void);



Motor   motor0,
        motor1,
        motor2;

void m0BeginTick(void)
{

}

void m0CheckTick(void)
{

}

void m1BeginTick(void)
{

}

void m1CheckTick(void)
{

}

void m2BeginTick(void)
{

}

void m2CheckTick(void)
{

}






int main(void)
{
    USART0Begin(115200);
    motor0.setDpin(M0_DPIN);
    motor1.setDpin(M1_DPIN);
    motor2.setDpin(M2_DPIN);

    motor0.setInverted(M0_INVERT);
    motor1.setInverted(M1_INVERT);
    motor2.setInverted(M2_INVERT);


    loop:

    motor0.setDir(CW);
    motor1.setDir(CW);
    motor2.setDir(CW);

    delay(600);

    motor0.setDir(ACW);
    motor1.setDir(ACW);
    motor2.setDir(ACW);

    delay(600);


    goto loop;//because "true" cycles is for plebs
    return 0;
}













Motor::Motor(uint16_t ticksPerCycle, uint8_t dpin, uint8_t spin, void (*func)(void))
{
    setTicksPerCycle(ticksPerCycle);
    setDpin(dpin);
    setSpin(spin);
    setTickingFunc(func);
}

void Motor::beginMoving(uint64_t ticks, DIR_TYPE dir)
{
    paused = 0;
    setDir(dir);
    ticksRemained = ticks;
    begin();
}

void Motor::pause(void)
{
    paused = 0;
}

void Motor::unpause(void)
{
    paused = 1;
    begin();
}

void Motor::stop(void)
{
    ticksRemained = 0;
}

void Motor::setDir(DIR_TYPE dir)
{
    digitalWrite(dpin, inverted?dir : !dir);
}

DIR_TYPE Motor::getDir(void)
{
    return digitalRead(dpin);
}

void Motor::setDpin(uint8_t pin)
{
    dpin = pin;
    pinMode(pin,OUTPUT);
}

void Motor::setSpin(uint8_t pin)
{
    spin = pin;
}

void Motor::setTicksPerCycle(uint16_t ticks)
{
    ticksPerCycle = ticks;
}

void Motor::setTickingFunc(void (*func)(void))
{
    startTicking = func;
}

void Motor::setInverted(uint8_t state)
{
    inverted = state;
}
