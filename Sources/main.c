/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))

/* Mapping of LEDs and buttons to specific port pins: */
// Note: only D9, SW3 and SW5 are used in this sample app
#define LED_D9  0x20      // Port B, bit 5
#define LED_D10 0x10      // Port B, bit 4
#define LED_D11 0x8       // Port B, bit 3
#define LED_D12 0x4       // Port B, bit 2

#define BTN_SW2 0x400     // Port E, bit 10
#define BTN_SW3 0x1000    // Port E, bit 12
#define BTN_SW4 0x8000000 // Port E, bit 27
#define BTN_SW5 0x4000000 // Port E, bit 26
#define BTN_SW6 0x800     // Port E, bit 11

#define SPK 0x10          // Speaker is on PTA4

volatile uint8_t power_mode = 0;

int pressed_up = 0, pressed_down = 0;
int beep_flag = 0;
unsigned int compare = 0x200;

/* A delay function */
void delay(long long bound) {

  long long i;
  for(i=0;i<bound;i++);
}

void beep(void) {
	for (uint32_t q = 0; q < 200; q++) {
		GPIOA_PDOR |= SPK;
		delay(200);
		GPIOA_PDOR &= ~SPK;
		delay(200);
	}
}

/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void PortsInit(void)
{
    /* Turn on all port clocks */
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTA_MASK;

    /* Set corresponding PTB pins (connected to LED's) for GPIO functionality */
    PORTB->PCR[5] = PORT_PCR_MUX(0x01); // D9
    PORTB->PCR[4] = PORT_PCR_MUX(0x01); // D10
    PORTB->PCR[3] = PORT_PCR_MUX(0x01); // D11
    PORTB->PCR[2] = PORT_PCR_MUX(0x01); // D12

    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01)
        | PORT_PCR_ISF(0x01)
		| PORT_PCR_IRQC(0x0A)
		| PORT_PCR_PE(0x01)
		| PORT_PCR_PS(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6
    NVIC_EnableIRQ(PORTE_IRQn);

    PORTA->PCR[4] = PORT_PCR_MUX(0x01);  // Speaker

    /* Change corresponding PTB port pins as outputs */
    PTB->PDDR = GPIO_PDDR_PDD(0x3C);     // LED ports as outputs
    PTA->PDDR = GPIO_PDDR_PDD(SPK);     // Speaker as output
    PTB->PDOR |= GPIO_PDOR_PDO(0x3C);    // turn all LEDs OFF
    PTA->PDOR &= GPIO_PDOR_PDO(~SPK);   // Speaker off, beep_flag is false
}

/// Enters the WAIT mode
void wait_mode(void) {
	SMC->PMCTRL = SMC_PMCTRL_STOPM(0);
	__asm("WFI");
}

/// Enter the STOP mode
void stop_mode(void) {
	SMC->PMCTRL = SMC_PMCTRL_STOPM(2);
	__asm("WFI");
}

/// Button press handler
void PORTE_IRQHandler(void) {
	// delay(1000);
	beep();

	if (PORTE->ISFR & BTN_SW5) {
		beep();
	}

	PORTE->ISFR = PORTE->ISFR;
}

int main(void)
{
    MCUInit();
    PortsInit();

    beep();

    while (1) {
        /*// pressing the up button decreases the compare value,
        // i.e. the compare event will occur more often;
        // testing pressed_up avoids uncontrolled modification
        // of compare if the button is hold.
        if (!pressed_up && !(GPIOE_PDIR & BTN_SW5))
        {
            pressed_up = 1;
            compare -= 0x40;
        }
        else if (GPIOE_PDIR & BTN_SW5) pressed_up = 0;
        // pressing the down button increases the compare value,
        // i.e. the compare event will occur less often;
        if (!pressed_down && !(GPIOE_PDIR & BTN_SW3))
        {
            pressed_down = 1;
            compare += 0x40;
        }
        else if (GPIOE_PDIR & BTN_SW3) pressed_down = 0;
        // some limits - in order to keep the LED blinking reasonable
        if (compare < 0x40) compare = 0x40;
        if (compare > 0x400) compare = 0x400;
        // test beep_flag in order to beep or not beep
        if (beep_flag)
        {
            GPIOA_PDOR ^= SPK;   // invert speaker state
            delay(1000);
        }
        else GPIOA_PDOR &= ~SPK; // logic 0 on speaker port if beep is false*/
    }

    return 0;
}
