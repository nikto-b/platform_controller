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



uint64_t    m0TicksRemained = 0,
            m1TicksRemained = 0,
            m2TicksRemained = 0;
uint8_t     m0Paused        = 0,
            m1Paused        = 0,
            m2Paused        = 0;
uint8_t     m0Dir           = 0,
            m1Dir           = 0,
            m2Dir           = 0;
uint8_t     m0Spd           = 18,
            m1Spd           = 18,
            m2Spd           = 18;
uint8_t     m0Prescaller    = TIMER2_CLK_SRC_1024,
            m1Prescaller    = TIMER0_CLK_SRC_1024,
            m2Prescaller    = TIMER1_CLK_SRC_1024;
            


void m0BeginTick(void);
void m1BeginTick(void);
void m2BeginTick(void);
void m0Tick(void);
void m1Tick(void);
void m2Tick(void);


void m0BeginTick(void)
{
    cli();
    TIMER2Init(TIMER2_COMB_TOGGLE, TIMER2_WF_CTC, m0Prescaller);
    TIMER2EnableCOMPBInterrupt();
    setCustomFunc(INTERRUPT_CUSTOMFUNC_TC2_COMPB, m0Tick);
    TIMER2SetA(m0Spd);
    sei();
}

void m0StopTick(void)
{
    TIMER2Flush();
}

void m0Tick(void)
{
    if(m0TicksRemained == 0)
        m0StopTick();
    else
        m0TicksRemained--;
}



void m1BeginTick(void)
{
    cli();
    TIMER0Init(TIMER0_COMA_NPWM_TOGGLE, TIMER0_WF_CTC, m1Prescaller);
    TIMER0EnableCOMPAInterrupt();
    setCustomFunc(INTERRUPT_CUSTOMFUNC_TC0_COMPA, m1Tick);
    TIMER0SetA(m1Spd);
    sei();
}

void m1StopTick(void)
{
    TIMER0Flush();
}

void m1Tick(void)
{
    if(m1TicksRemained == 0)
        m1StopTick();
    else
        m1TicksRemained--;
}



void m2BeginTick(void)
{
    cli();
                                //FPWM because fuck you Atmel!
    TIMER1Init(TIMER1_COMA_TOGGLE, TIMER1_WF_FPWM_TOPOCR1A, m2Prescaller);
    TIMER1EnableCOMPAInterrupt();
    setCustomFunc(INTERRUPT_CUSTOMFUNC_TC1_COMPA, m2Tick);
    TIMER1SetA((m2Spd * 0xFFFF) / 0xFF);
    sei();
}

void m2StopTick(void)
{
    TIMER1Flush();
}

void m2Tick(void)
{
    USART0Println((int)m2TicksRemained);
    if(m2TicksRemained == 0)
        m2StopTick();
    else
        m2TicksRemained--;
}





int main(void)
{
    //start USART at standart speed
    USART0Begin(USART_SPD);
    
    DDRB = 0xFF;
    DDRD = 0xFF;
    DDRC = 0xFF;
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

    // m1BeginTick();
    // m2BeginTick();
    
    PORTC = (0 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);
    
    // m0TicksRemained = 1000;
    // m0BeginTick();
    // // 
    // 
    // m1TicksRemained = 1000;
    // m1BeginTick();
    // 
    m2TicksRemained = 1000;
    m2BeginTick();
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
    
    // PORTB = ~PORTB;
    // PORTB = (1 << PB1);
    // // delay(1);
    // _delay_us(70);
    // PORTB = 0;
    // // delay(1);
    // _delay_us(70);
    goto loop;//because "true" cycles is for plebs
    return 0;
}












