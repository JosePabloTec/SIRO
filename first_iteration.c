/* 
 * File:   newmain.c
 * Author: jpman
 *
 * Created on August 18, 2026, 10:31 AM
 */

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = XT_XT        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>


// function-set command for the 8-bit, two-line mode
#define ClrScreen       0x01
#define ReturnHome      0x02
#define DecCursor       0x04
#define IncCursor       0x06
#define ShiftRight      0x05
#define ShiftLeft       0x07
#define DispOFFCurOFF   0x08
#define DispOFFCurON    0x0A
#define DispONCurOFF    0x0C
#define DispONCurBk     0x0E
#define DispOFFCurBk    0x0F
#define ShiftCurLeft    0x10
#define ShiftCurRight   0x14
#define ShiftDispLeft   0x18
#define ShiftDispRight  0x1C
#define FirstLine       0x80
#define SecondLine      0xC0
#define TwoLines57Mat   0x38

#define RS PORTCbits.RC5
#define RW PORTCbits.RC6
#define EN PORTCbits.RC7

#define _XTAL_FREQ 4000000

#define LCD_Port PORTD
#define LCD_Tris TRISD
#define Delay_LCD 2

void Lcd_CmdWrite(unsigned char c)
{
    LCD_Port = c;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(Delay_LCD);
    EN = 0;
}

void Lcd_DataWrite(unsigned char d)
{
    LCD_Port = d;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(Delay_LCD);
    EN = 0;
}

void Message_LCD(const char *s)
{
    while (*s)
        Lcd_DataWrite((unsigned char)*s++);
}

void Init_LCD(void)
{
    LCD_Tris = 0x00;
    TRISC = TRISC & 0x1F;

    LCD_Port = 0;
    RS = 0;
    RW = 0;
    EN = 0;

    __delay_ms(20);
    Lcd_CmdWrite(TwoLines57Mat);
    Lcd_CmdWrite(DispONCurOFF);
    Lcd_CmdWrite(ClrScreen);
    Lcd_CmdWrite(IncCursor);
}

// LED, Shift register crap
#define SR1_DATA PORTBbits.RB4
#define SR1_CLK  PORTBbits.RB5
#define SR2_DATA PORTBbits.RB6
#define SR2_CLK  PORTBbits.RB7
// Use your existing #include <xc.h> and _XTAL_FREQ definition.

#define SR1_DATA PORTBbits.RB4
#define SR1_CLK  PORTBbits.RB5
#define SR2_DATA PORTBbits.RB6
#define SR2_CLK  PORTBbits.RB7

static unsigned char sr1_state = 0;
static unsigned char sr2_state = 0;

void shift_SR1(unsigned char value)
{
    int bit;
    for (bit = 7; bit >= 0; bit--)
    {
        SR1_CLK = 0;
        SR1_DATA = (value >> bit) & 1;
        __delay_us(2);
        SR1_CLK = 1;        // 74164 shifts on rising edge
        __delay_us(2);
    }

    SR1_CLK = 0;
}

void shift_SR2(unsigned char value)
{
    int bit;
    for (bit = 7; bit >= 0; bit--)
    {
        SR2_CLK = 0;
        SR2_DATA = (value >> bit) & 1;
        __delay_us(2);
        SR2_CLK = 1;
        __delay_us(2);
    }
    SR2_CLK = 0;
}

void Init_ShiftRegisters(void)
{
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    SR1_DATA = 0;
    SR1_CLK = 0;
    SR2_DATA = 0;
    SR2_CLK = 0;
    sr1_state = 0;
    sr2_state = 0;

    shift_SR1(sr1_state);
    shift_SR2(sr2_state);
}

void turn_on(int LED_number)
{
    if (LED_number >= 1 && LED_number <= 6)
        sr1_state |= (unsigned char)(1u << (LED_number - 1));
    else if (LED_number >= 7 && LED_number <= 12)
        sr2_state |= (unsigned char)(1u << (LED_number - 7));
    else if (LED_number >= 13 && LED_number <= 14)
        sr1_state |= (unsigned char)(1u << (LED_number - 7));
    else if (LED_number >= 15 && LED_number <= 16)
        sr2_state |= (unsigned char)(1u << (LED_number - 9));
    else
        return;

    shift_SR1(sr1_state);
    shift_SR2(sr2_state);
}

void turn_off(int LED_number)
{
    if (LED_number >= 1 && LED_number <= 6)
        sr1_state &= (unsigned char)~(1u << (LED_number - 1));
    else if (LED_number >= 7 && LED_number <= 12)
        sr2_state &= (unsigned char)~(1u << (LED_number - 7));
    else if (LED_number >= 13 && LED_number <= 14)
        sr1_state &= (unsigned char)~(1u << (LED_number - 7));
    else if (LED_number >= 15 && LED_number <= 16)
        sr2_state &= (unsigned char)~(1u << (LED_number - 9));
    else
        return;

    shift_SR1(sr1_state);
    shift_SR2(sr2_state);
}

unsigned int Read_Timer0(void);

void wait(int ms)
{
    unsigned int start_ticks;
    unsigned int required_ticks;

    if (ms <= 0)
        return;

    start_ticks = Read_Timer0();
    required_ticks = (unsigned int)(((unsigned long)ms * 1000UL + 63UL) / 64UL);

    while ((unsigned int)(Read_Timer0() - start_ticks) < required_ticks)
    {
    }
}

// initial show

void init_sequence(){
    Lcd_CmdWrite(ClrScreen);
    Message_LCD("All goodie");
    Lcd_CmdWrite(SecondLine);
    Message_LCD("in the hoodie");
    wait(1000);
    Lcd_CmdWrite(ClrScreen);

    turn_off(1); turn_off(2); turn_off(3);
    turn_off(4); turn_off(5); turn_off(6);
    turn_off(7); turn_off(8); turn_off(9);
    turn_on(5); wait(150); turn_off(5); wait(100);
    turn_on(5); wait(150); turn_off(5); wait(100);
    turn_on(5); wait(150); turn_off(5); wait(100);
    turn_on(1); wait(120);
    turn_on(2); turn_off(1); wait(120);
    turn_on(3); turn_off(2); wait(120);
    turn_on(6); turn_off(3); wait(120);
    turn_on(9); turn_off(6); wait(120);
    turn_on(8); turn_off(9); wait(120);
    turn_on(7); turn_off(8); wait(120);
    turn_on(4); turn_off(7); wait(120);
    turn_off(4);
    turn_on(1); turn_on(2); turn_on(3); wait(250);
    turn_off(1); turn_off(2); turn_off(3);
    turn_on(4); turn_on(5); turn_on(6); wait(250);
    turn_off(4); turn_off(5); turn_off(6);
    turn_on(7); turn_on(8); turn_on(9); wait(250);
    turn_off(7); turn_off(8); turn_off(9);
    turn_on(1); turn_on(4); turn_on(7); wait(250);
    turn_off(1); turn_off(4); turn_off(7);
    turn_on(2); turn_on(5); turn_on(8); wait(250);
    turn_off(2); turn_off(5); turn_off(8);
    turn_on(3); turn_on(6); turn_on(9); wait(250);
    turn_off(3); turn_off(6); turn_off(9);
    turn_on(1); turn_on(5); turn_on(9); wait(300);
    turn_off(1); turn_off(5); turn_off(9);
    turn_on(3); turn_on(5); turn_on(7); wait(300);
    turn_off(3); turn_off(5); turn_off(7);
    turn_on(1); turn_on(3); turn_on(5);
    turn_on(7); turn_on(9); wait(250);
    turn_off(1); turn_off(3); turn_off(5);
    turn_off(7); turn_off(9);
    turn_on(2); turn_on(4); turn_on(6);
    turn_on(8); wait(250);
    turn_off(2); turn_off(4); turn_off(6);
    turn_off(8);
    turn_on(1); turn_on(3); turn_on(5);
    turn_on(7); turn_on(9); wait(250);
    turn_off(1); turn_off(3); turn_off(5);
    turn_off(7); turn_off(9);
    turn_on(2); turn_on(4); turn_on(6);
    turn_on(8); wait(250);
    turn_off(2); turn_off(4); turn_off(6);
    turn_off(8);
    turn_on(1); wait(60);
    turn_on(2); wait(60);
    turn_on(3); wait(60);
    turn_on(4); wait(60);
    turn_on(5); wait(60);
    turn_on(6); wait(60);
    turn_on(7); wait(60);
    turn_on(8); wait(60);
    turn_on(9); wait(500);
    turn_off(1); turn_off(2); turn_off(3);
    turn_off(4); turn_off(5); turn_off(6);
    turn_off(7); turn_off(8); turn_off(9);
}

// ADC Crap

void Init_ADC_Module(void)
{
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA5 = 1;

    ADFM = 1;
    VCFG1 = 0;
    VCFG0 = 0;

    PCFG3 = 0;
    PCFG2 = 0;
    PCFG1 = 0;
    PCFG0 = 0;

    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 1;

    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;

    ADON = 1;
}

unsigned long COMMON_RESISTOR_OHMS = 10000;


unsigned int Read_Potentiometer(unsigned char channel)
{
    unsigned long sum = 0;
    unsigned int value;
    unsigned char sample;

    CHS3 = (channel >> 3) & 1;
    CHS2 = (channel >> 2) & 1;
    CHS1 = (channel >> 1) & 1;
    CHS0 = channel & 1;

    for (sample = 0; sample < 4; sample++)
    {
        __delay_us(20);
        GO_DONE = 1;
        while (GO_DONE) { }
        sum += ((unsigned int)ADRESH << 8) | ADRESL;
    }

    value = (unsigned int)(sum / 4);

    if (value > 675)
        value = 675;

    return value;
}

unsigned long BUTTON_RESISTORS_OHMS[12] =
{
    100,      // PB1:  100 ohm
    1000,     // PB2:  1 k
    2200,     // PB3:  2.2 k
    3900,     // PB4:  3.9 k
    5600,     // PB5:  5.6 k
    8200,     // PB6:  8.2 k
    12000,    // PB7:  12 k
    18000,    // PB8:  18 k
    27000,    // PB9:  27 k
    43000,    // PB10: 43 k
    91000,    // PB11: 91 k
    220000   // PB12: 220 k
};

void Set_AN0_Channel(void){
    CHS3 = 0;
    CHS2 = 0;
    CHS1 = 0;
    CHS0 = 0;
}

unsigned char Read_and_print_Button_A0(void)
{
    unsigned long adc_sum = 0;
    unsigned long expected_adc;
    unsigned long error;
    unsigned long best_error;
    unsigned long denominator;
    unsigned int adc_value;
    unsigned char button;
    unsigned char best_button;
    unsigned char i;

    Set_AN0_Channel();

    for (i = 0; i < 8; i++)
    {
        __delay_us(20);
        GO_DONE = 1;
        while (GO_DONE) { }
        adc_sum += ((unsigned int)ADRESH << 8) | ADRESL;
    }

    adc_value = (unsigned int)(adc_sum / 8);
    if (adc_value < 10)
        return 0;
    best_button = 0;
    best_error = 1023 - adc_value;

    for (i = 0; i < 12; i++)
    {
        denominator = COMMON_RESISTOR_OHMS + BUTTON_RESISTORS_OHMS[i];
        expected_adc =
            (1023 * BUTTON_RESISTORS_OHMS[i] + denominator / 2)
            / denominator;

        if (adc_value >= expected_adc)
            error = adc_value - expected_adc;
        else
            error = expected_adc - adc_value;

        if (error < best_error)
        {
            best_error = error;
            best_button = i + 1;
        }
    }

    Lcd_CmdWrite(FirstLine);

    if (best_button == 0)
    {
        Message_LCD("No button      ");
    }
    else if (best_button < 10)
    {
        Message_LCD("PB ");
        Lcd_DataWrite('0' + best_button);
        Message_LCD(" pressed       ");
    }
    else
    {
        Message_LCD("PB ");
        Lcd_DataWrite('0' + (best_button / 10));
        Lcd_DataWrite('0' + (best_button % 10));
        Message_LCD(" pressed      ");
    }

    return best_button;
}

unsigned char Read_Button_A0(void)
{
    unsigned long adc_sum = 0;
    unsigned long expected_adc;
    unsigned long error;
    unsigned long best_error;
    unsigned long denominator;
    unsigned int adc_value;
    unsigned char button;
    unsigned char best_button;
    unsigned char i;
    Set_AN0_Channel();
    for (i = 0; i < 8; i++)
    {
        __delay_us(20);
        GO_DONE = 1;
        while (GO_DONE) { }
        adc_sum += ((unsigned int)ADRESH << 8) | ADRESL;
    }

    adc_value = (unsigned int)(adc_sum / 8);
    if (adc_value < 10)
        return 0;
    best_button = 0;
    best_error = 1023 - adc_value;
    for (i = 0; i < 12; i++)
    {
        denominator = COMMON_RESISTOR_OHMS + BUTTON_RESISTORS_OHMS[i];
        expected_adc =
            (1023 * BUTTON_RESISTORS_OHMS[i] + denominator / 2)
            / denominator;

        if (adc_value >= expected_adc)
            error = adc_value - expected_adc;
        else
            error = expected_adc - adc_value;

        if (error < best_error)
        {
            best_error = error;
            best_button = i + 1;
        }
    }
    return best_button;
}

volatile unsigned char Juego_detenido = 0;
volatile unsigned int Timer0_OverflowCount = 0;


// Enable interrupts (INT and RB port change)
void Enable_Interrupts(void)
{
    ADCON1 = 0x0F;             // Pines analógicos como digitales
    TRISBbits.TRISB0 = 1;      // RB0/INT0 como entrada

    INTCONbits.INT0IF = 0;     // Limpiar bandera pendiente
    INTCON2bits.INTEDG0 = 0;   // Interrupción en flanco descendente
    INTCONbits.INT0IE = 1;     // Habilitar INT0

    INTCONbits.PEIE = 1;       // Habilitar interrupciones periféricas
    INTCONbits.GIE = 1;        // Habilitar interrupciones globales
}

void __interrupt() Juego_Interrupt(void)
{
    if (INTCONbits.INT0IF)
    {
        INTCONbits.INT0IF = 0;
        Juego_detenido = 1;

        Lcd_CmdWrite(ClrScreen);
        Lcd_CmdWrite(FirstLine);
        Message_LCD("RB0 interrupt");
    }

    if (INTCONbits.TMR0IF)
    {
        Timer0_OverflowCount++;
        INTCONbits.TMR0IF = 0;
    }
}


// Math crap
#include <stdlib.h>

typedef struct
{
    unsigned char number1;
    unsigned char number2;
    char operation;
    int solution;
} MathProblem;

unsigned int RandomBelow(unsigned int limit)
{
    unsigned long range = (unsigned long)RAND_MAX + 1UL;
    unsigned long cutoff = range - (range % limit);
    unsigned long value;

    do
    {
        value = (unsigned long)rand();
    } while (value >= cutoff);

    return (unsigned int)(value % limit);
}

MathProblem GenerateProblem(void)
{
    MathProblem problem;
    unsigned char temp;

    switch (RandomBelow(3))
    {
        case 0:
            problem.operation = '+';
            break;
        case 1:
            problem.operation = '-';
            break;
        default:
            problem.operation = '*';
            break;
    }

    do
    {
        problem.number1 = (unsigned char)(RandomBelow(10) + 1);
        problem.number2 = (unsigned char)(RandomBelow(10) + 1);

        if (problem.operation == '-' && problem.number2 > problem.number1)
        {
            temp = problem.number1;
            problem.number1 = problem.number2;
            problem.number2 = temp;
        }

        if (problem.operation == '+')
            problem.solution = problem.number1 + problem.number2;
        else if (problem.operation == '-')
            problem.solution = problem.number1 - problem.number2;
        else
            problem.solution = problem.number1 * problem.number2;

    } while (problem.solution > 100);

    return problem;
}

void LCD_WriteNumber(unsigned int number)
{
    if (number >= 100)
        Lcd_DataWrite((unsigned char)('0' + number / 100));

    if (number >= 10)
        Lcd_DataWrite((unsigned char)('0' + (number / 10) % 10));

    Lcd_DataWrite((unsigned char)('0' + number % 10));
}

void LCD_ShowProblem(MathProblem problem, unsigned int answer, unsigned char digits_entered)
{
    Lcd_CmdWrite(ClrScreen);
    Lcd_CmdWrite(FirstLine);

    LCD_WriteNumber(problem.number1);
    Lcd_DataWrite(problem.operation);
    LCD_WriteNumber(problem.number2);
    Lcd_DataWrite('=');
    Lcd_DataWrite('?');

    Lcd_CmdWrite(SecondLine);
    Message_LCD("Ans: ");

    if (digits_entered > 0)
        LCD_WriteNumber(answer);
}

unsigned char GameDelay(unsigned int milliseconds)
{
    unsigned int i;

    for (i = 0; i < milliseconds; i++)
    {
        if (Juego_detenido)
            return 0;

        wait(1);
    }

    return 1;
}

unsigned char WaitForKeyPress(void)
{
    unsigned char key;

    while (!Juego_detenido)
    {
        key = Read_Button_A0();

        if (key != 0)
        {
            wait(20);

            if (Read_Button_A0() == key)
            {
                while (Read_Button_A0() != 0 && !Juego_detenido)
                    wait(10);

                if (Juego_detenido)
                    return 0;

                return key;
            }
        }

        wait(1);
    }

    return 0;
}

void TurnOffAllLEDs(void)
{
    unsigned char i;

    for (i = 1; i <= 16; i++)
        turn_off(i);
}

void ShowRandomCongratulations(void)
{
    unsigned char message;

    message = (unsigned char)RandomBelow(3);

    Lcd_CmdWrite(ClrScreen);
    Lcd_CmdWrite(FirstLine);

    if (message == 0)
        Message_LCD("Bien hecho!");
    else if (message == 1)
        Message_LCD("Sigue asi!");
    else
        Message_LCD("Bravo!");

    GameDelay(2000);
}

unsigned char HasZeroDigit(unsigned int number)
{
    if (number == 0)
        return 1;

    while (number > 0)
    {
        if (number % 10 == 0)
            return 1;

        number /= 10;
    }

    return 0;
}


void PlayMathGame(void)
{
    MathProblem problem;
    unsigned int answer;
    unsigned char digits_entered;
    unsigned char key;
    unsigned int i;

    while (!Juego_detenido)
    {
        do
        {
            problem = GenerateProblem();
        }
        while ((problem.solution <= 0 ||
                HasZeroDigit((unsigned int)problem.solution)) &&
               !Juego_detenido);

        if (Juego_detenido)
            break;

        answer = 0;
        digits_entered = 0;
        LCD_ShowProblem(problem, answer, digits_entered);

        while (!Juego_detenido)
        {
            key = WaitForKeyPress();

            if (Juego_detenido)
                break;

            if (key >= 1 && key <= 9 && digits_entered < 3)
            {
                answer = answer * 10 + key;
                digits_entered++;
                LCD_ShowProblem(problem, answer, digits_entered);
            }
            else if (key == 11 && digits_entered < 3)
            {
                answer = answer * 10;
                digits_entered++;
                LCD_ShowProblem(problem, answer, digits_entered);
            }
            else if (key == 10 && digits_entered > 0)
            {
                if (answer == problem.solution)
                {
                    Lcd_CmdWrite(ClrScreen);
                    Lcd_CmdWrite(FirstLine);
                    Message_LCD("Bien hecho");
                    GameDelay(2000);
                    break;
                }

                Lcd_CmdWrite(ClrScreen);
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Incorrecto");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("Intenta otra vez");

                if (!GameDelay(1500))
                    break;

                answer = 0;
                digits_entered = 0;
                LCD_ShowProblem(problem, answer, digits_entered);
            }
        }

        if (!GameDelay(100))
            break;
    }

    TurnOffAllLEDs();
    Lcd_CmdWrite(ClrScreen);
}

void PlayMemoryGame(void)
{
    unsigned char sequence[255];
    unsigned int sequence_length = 0;
    unsigned int index;
    unsigned int guess_index;
    unsigned char key;
    unsigned char correct;
    unsigned char message;

    while (!Juego_detenido)
    {
        if (sequence_length >= 255)
            sequence_length = 0;

        sequence[sequence_length] = (unsigned char)(RandomBelow(12) + 1);
        sequence_length++;

        for (index = 0; index < sequence_length && !Juego_detenido; index++)
        {
            turn_on(sequence[index]);

            if (!GameDelay(1000))
            {
                turn_off(sequence[index]);
                break;
            }

            turn_off(sequence[index]);

            if (!GameDelay(250))
                break;
        }

        if (Juego_detenido)
            break;

        Lcd_CmdWrite(ClrScreen);
        Lcd_CmdWrite(FirstLine);
        Message_LCD("Recuerda la");
        Lcd_CmdWrite(SecondLine);
        Message_LCD("secuencia");

        correct = 1;

        for (guess_index = 0;
             guess_index < sequence_length && !Juego_detenido;
             guess_index++)
        {
            key = WaitForKeyPress();

            if (Juego_detenido)
                break;

            if (key != sequence[guess_index])
            {
                correct = 0;
                break;
            }
        }

        if (Juego_detenido)
            break;

        if (!correct)
        {
            TurnOffAllLEDs();
            Lcd_CmdWrite(ClrScreen);
            Lcd_CmdWrite(FirstLine);
            Message_LCD("Perdiste!");

            if (!GameDelay(2000))
                break;

            sequence_length = 0;
        }
        else
        {
            message = (unsigned char)RandomBelow(3);

            Lcd_CmdWrite(ClrScreen);
            Lcd_CmdWrite(FirstLine);

            if (message == 0)
                Message_LCD("Bravo=)");
            else if (message == 1)
                Message_LCD("Bien hecho =)");
            else
                Message_LCD("Sigue asi =)");

            if (!GameDelay(1500))
                break;
        }
    }

    TurnOffAllLEDs();
    Lcd_CmdWrite(ClrScreen);
}

unsigned long Read_Timer0Extended(void);

void PlayAgilityGame(void)
{
    unsigned char target;
    unsigned char key;
    unsigned char correct;
    unsigned int time_limit = 20;
    unsigned int remaining;
    unsigned int last_remaining;
    unsigned long start_ticks;
    unsigned long elapsed_ticks;
    unsigned long elapsed_ms;

    while (!Juego_detenido)
    {
        target = (unsigned char)(RandomBelow(12) + 1);
        correct = 0;

        Lcd_CmdWrite(ClrScreen);
        Lcd_CmdWrite(FirstLine);
        Message_LCD("Atento");
        Lcd_CmdWrite(SecondLine);
        Message_LCD("Presiona el boton");

        turn_on(target);

        if (!GameDelay(500))
        {
            turn_off(target);
            break;
        }

        turn_off(target);

        start_ticks = Read_Timer0Extended();
        last_remaining = time_limit;

        while (!Juego_detenido)
        {
            elapsed_ticks = Read_Timer0Extended() - start_ticks;
            elapsed_ms = (elapsed_ticks * 64UL) / 1000UL;

            if (elapsed_ms >= (unsigned long)time_limit * 1000UL)
                break;

            remaining = time_limit - (unsigned int)(elapsed_ms / 1000UL);

            if (remaining != last_remaining)
            {
                Lcd_CmdWrite(ClrScreen);
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Tiempo: ");
                LCD_WriteNumber(remaining);
                Message_LCD("s");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("Presiona el boton");

                last_remaining = remaining;
            }

            key = Read_Button_A0();

            if (key != 0)
            {
                wait(20);

                if (Read_Button_A0() == key)
                {
                    correct = (key == target);

                    while (Read_Button_A0() != 0 && !Juego_detenido)
                        wait(10);

                    break;
                }
            }

            wait(1);
        }

        if (Juego_detenido)
            break;

        if (correct)
        {
            ShowRandomCongratulations();

            if (!GameDelay(1200))
                break;

            if (time_limit > 1)
                time_limit--;
        }
        else
        {
            TurnOffAllLEDs();
            Lcd_CmdWrite(ClrScreen);
            Lcd_CmdWrite(FirstLine);
            Message_LCD("Perdiste!");

            if (!GameDelay(2000))
                break;

            time_limit = 20;
        }
    }

    TurnOffAllLEDs();
    Lcd_CmdWrite(ClrScreen);
}


void PlayAgilityGameWithPotentiometers(void)
{
    unsigned char target;
    unsigned char key;
    unsigned char correct;
    unsigned char pot;
    unsigned int time_limit = 20;
    unsigned int remaining;
    unsigned int last_remaining;
    unsigned int pot_start[4];
    unsigned int pot_threshold[4];
    unsigned int pot_value;
    unsigned int difference;
    unsigned int maximum_change;
    unsigned long start_ticks;
    unsigned long elapsed_ticks;
    unsigned long elapsed_ms;

    while (!Juego_detenido)
    {
        if (RandomBelow(2) == 0)
            target = (unsigned char)(RandomBelow(12) + 1);
        else
            target = (unsigned char)(RandomBelow(4) + 13);

        correct = 0;

        Lcd_CmdWrite(ClrScreen);
        Lcd_CmdWrite(FirstLine);
        Message_LCD("Atento");
        Lcd_CmdWrite(SecondLine);
        Message_LCD("Presiona o gira");

        turn_on(target);

        if (!GameDelay(1000))
        {
            turn_off(target);
            break;
        }

        turn_off(target);

        for (pot = 0; pot < 4; pot++)
        {
            pot_start[pot] = Read_Potentiometer(pot + 1);

            maximum_change = pot_start[pot];

            if ((675 - pot_start[pot]) > maximum_change)
                maximum_change = 675 - pot_start[pot];

            if (maximum_change < 400)
                pot_threshold[pot] = 300;
            else
                pot_threshold[pot] = 400;
        }

        start_ticks = Read_Timer0Extended();
        last_remaining = time_limit;

        while (!Juego_detenido)
        {
            elapsed_ticks = Read_Timer0Extended() - start_ticks;
            elapsed_ms = (elapsed_ticks * 64UL) / 1000UL;

            if (elapsed_ms >= (unsigned long)time_limit * 1000UL)
                break;

            remaining = time_limit - (unsigned int)(elapsed_ms / 1000UL);

            if (remaining != last_remaining)
            {
                Lcd_CmdWrite(ClrScreen);
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Tiempo: ");
                LCD_WriteNumber(remaining);
                Message_LCD("s");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("Presiona o gira");
                last_remaining = remaining;
            }

            key = Read_Button_A0();

            if (key != 0)
            {
                wait(20);

                if (Read_Button_A0() != key)
                    key = 0;
            }
            else
            {
                for (pot = 0; pot < 4; pot++)
                {
                    pot_value = Read_Potentiometer(pot + 1);

                    if (pot_value >= pot_start[pot])
                        difference = pot_value - pot_start[pot];
                    else
                        difference = pot_start[pot] - pot_value;

                    if (difference >= pot_threshold[pot])
                    {
                        key = 13 + pot;
                        wait(20);

                        pot_value = Read_Potentiometer(pot + 1);

                        if (pot_value >= pot_start[pot])
                            difference = pot_value - pot_start[pot];
                        else
                            difference = pot_start[pot] - pot_value;

                        if (difference < pot_threshold[pot])
                            key = 0;

                        break;
                    }
                }
            }

            if (key != 0)
            {
                elapsed_ticks = Read_Timer0Extended() - start_ticks;
                elapsed_ms = (elapsed_ticks * 64UL) / 1000UL;

                if (elapsed_ms < (unsigned long)time_limit * 1000UL)
                {
                    if (key <= 12)
                    {
                        while (Read_Button_A0() != 0 && !Juego_detenido)
                            wait(10);
                    }

                    correct = (key == target);
                    break;
                }
            }

            wait(1);
        }

        if (Juego_detenido)
            break;

        if (correct)
        {
            ShowRandomCongratulations();

            if (!GameDelay(1200))
                break;

            if (time_limit > 1)
                time_limit--;
        }
        else
        {
            TurnOffAllLEDs();
            Lcd_CmdWrite(ClrScreen);
            Lcd_CmdWrite(FirstLine);
            Message_LCD("!Perdiste!");

            if (!GameDelay(2000))
                break;

            time_limit = 20;
        }
    }

    TurnOffAllLEDs();
    Lcd_CmdWrite(ClrScreen);
}


// R = 10k (common for all PBs)
// R1 = 470 ohms
// R2 = 1k
// R3 = 2.2k
// R4 = 3.9k
// R5 = 5.6k
// R6 = 8.2k
// R7 = 12k
// R8 = 18k
// R9 = 27k
// R10 = 43k
// R11 = 91k
// R12 = 1000k


// SR1/A/RB4
// SR1/CLK/RB5
// SR2/A/RB6
// SR2/CLK/RB7

// LED1/Q0/SR1
// LED2/Q1/SR1
// LED3/Q2/SR1
// LED4/Q3/SR1
// LED5/Q4/SR1
// LED6/Q5/SR1

// LED7/Q0/SR2
// LED8/Q1/SR2
// LED9/Q2/SR2
// LED10/Q3/SR2
// LED11/Q4/SR2
// LED12/Q5/SR2



unsigned int Read_Timer0(void)
{
    unsigned char low_byte = TMR0L;
    unsigned char high_byte = TMR0H;

    return ((unsigned int)high_byte << 8) | low_byte;
}

unsigned long Read_Timer0Extended(void)
{
    unsigned char low_byte;
    unsigned char high_byte;
    unsigned char gie_state;
    unsigned int overflow_count;
    unsigned int timer_count;

    gie_state = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    T0CONbits.TMR0ON = 0;

    low_byte = TMR0L;
    high_byte = TMR0H;
    timer_count = ((unsigned int)high_byte << 8) | low_byte;
    overflow_count = Timer0_OverflowCount;

    if (INTCONbits.TMR0IF)
        overflow_count++;

    T0CONbits.TMR0ON = 1;
    INTCONbits.GIE = gie_state;

    return ((unsigned long)overflow_count << 16) | timer_count;
}


void main(void)
{
    unsigned char button_pressed = 0;
    unsigned char menu_stage;
    unsigned int random_seed;
    unsigned long menu_start_ticks;
    unsigned long timer_ticks;
    unsigned long elapsed_ms;

    Init_LCD();
    Init_ShiftRegisters();
    Init_ADC_Module();

    T0CON = 0x85;
    init_sequence();

    random_seed = Read_Timer0();
    srand(random_seed);

    T0CONbits.TMR0ON = 0;
    TMR0H = 0;
    TMR0L = 0;
    Timer0_OverflowCount = 0;
    INTCONbits.TMR0IF = 0;
    T0CONbits.TMR0ON = 1;

    Enable_Interrupts();

    while (1)
    {
        Juego_detenido = 0;
        button_pressed = 0;
        menu_stage = 0;
        menu_start_ticks = Read_Timer0Extended();
        Lcd_CmdWrite(ClrScreen);

        while (button_pressed != 1 &&
               button_pressed != 2 &&
               button_pressed != 3 &&
               !Juego_detenido)
        {
            timer_ticks = Read_Timer0Extended();
            elapsed_ms =
                ((timer_ticks - menu_start_ticks) * 64UL) / 1000UL;

            if (elapsed_ms >= 6500)
            {
                menu_start_ticks = Read_Timer0Extended();
                elapsed_ms = 0;
                menu_stage = 0;
                Lcd_CmdWrite(ClrScreen);
            }

            if (menu_stage == 0 && elapsed_ms >= 500)
            {
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Hola Paco");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("=)");
                menu_stage = 1;
            }
            else if (menu_stage == 1 && elapsed_ms >= 1500)
            {
                Lcd_CmdWrite(ClrScreen);
                menu_stage = 2;
            }
            else if (menu_stage == 2 && elapsed_ms >= 2000)
            {
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Boton 1: Juego");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("de Matematicas");
                menu_stage = 3;
            }
            else if (menu_stage == 3 && elapsed_ms >= 3000)
            {
                Lcd_CmdWrite(ClrScreen);
                menu_stage = 4;
            }
            else if (menu_stage == 4 && elapsed_ms >= 3500)
            {
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Boton 2: Juego");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("de Memoria");
                menu_stage = 5;
            }
            else if (menu_stage == 5 && elapsed_ms >= 4500)
            {
                Lcd_CmdWrite(ClrScreen);
                menu_stage = 6;
            }
            else if (menu_stage == 6 && elapsed_ms >= 5000)
            {
                Lcd_CmdWrite(FirstLine);
                Message_LCD("Boton 3: Juego");
                Lcd_CmdWrite(SecondLine);
                Message_LCD("de Agilidad");
                menu_stage = 7;
            }
            else if (menu_stage == 7 && elapsed_ms >= 6000)
            {
                Lcd_CmdWrite(ClrScreen);
                menu_stage = 8;
            }

            wait(1);
            button_pressed = Read_Button_A0();
        }

        if (Juego_detenido)
        {
            Juego_detenido = 0;
            INT0IF = 0;
            continue;
        }

        if (button_pressed == 1 ||
            button_pressed == 2 ||
            button_pressed == 3)
        {
            while (Read_Button_A0() != 0 && !Juego_detenido)
                wait(10);

            if (!Juego_detenido)
            {
                if (button_pressed == 1)
                    PlayMathGame();
                else if (button_pressed == 2)
                    PlayMemoryGame();
                else if (button_pressed == 3)
                    PlayAgilityGameWithPotentiometers();
            }

            if (Juego_detenido)
            {
                TurnOffAllLEDs();
                Lcd_CmdWrite(ClrScreen);

                while (PORTBbits.RB0 == 0)
                    ;

                Juego_detenido = 0;
                INT0IF = 0;
                continue;
            }
        }

        TurnOffAllLEDs();
        Lcd_CmdWrite(ClrScreen);
    }
}