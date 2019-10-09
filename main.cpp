#include "avr-api/api.h"

#define DIR_TYPE    uint8_t
#define CW          1
#define ACW         0


#define USART_SPD   115200


#define M0_DPIN     18
#define M0_SPIN     0
#define M0_INVERT   0
#define M0_TICKS    0
//step0 on OC2B

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
    TIMER2Init(TIMER2_COMB_TOGGLE, TIMER2_WF_CTC, TIMER2_CLK_SRC_1024);

    TIMER2SetA(255);
}

void m0StopTick(void)
{
    TIMER2Flush();
}

void m0CheckTick(void)
{

}



void m1BeginTick(void)
{
    TIMER0Init(TIMER0_COMB_NPWM_TOGGLE, TIMER0_WF_CTC, TIMER0_CLK_SRC_1024);

    TIMER0SetA(255);
}

void m1StopTick(void)
{
    TIMER0Flush();
}

void m1CheckTick(void)
{

}



void m2BeginTick(void)
{
    TIMER1Init(TIMER1_COMB_TOGGLE, TIMER1_WF_CTC_TOPOCR1A, TIMER1_CLK_SRC_1024);

    TIMER1SetA(255);
}

void m2StopTick(void)
{
    TIMER1Flush();
}

void m2CheckTick(void)
{

}







int main(void)
{
    DDRD = 0xFF;
    //start USART at standart speed
    USART0Begin(USART_SPD);
    USART0Println("STARTED");
    //
    // //set direction pins for motors
    // motor0.setDpin(M0_DPIN);
    // motor1.setDpin(M1_DPIN);
    // motor2.setDpin(M2_DPIN);
    //
    // //set inversion for motors
    // motor0.setInverted(M0_INVERT);
    // motor1.setInverted(M1_INVERT);
    // motor2.setInverted(M2_INVERT);

    m0BeginTick();
    m1BeginTick();
    m2BeginTick();
    m3BeginTick();


    // const int del = 40;
    loop:
    //
    // // motor0.setDir(CW);
    // // motor1.setDir(CW);
    // // motor2.setDir(CW);
    // //
    // // delay(600);
    // //
    // // motor0.setDir(ACW);
    // // motor1.setDir(ACW);
    // // motor2.setDir(ACW);
    // //
    // // delay(600)
    // // for(uint8_t i = 0; i < 0xFF; i++)
    // //     asm("NOP");
    // _delay_us(del);
    // PORTD = 0;
    // _delay_us(del);
    // PORTD = 0xFF;

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
