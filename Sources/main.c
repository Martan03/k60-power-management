#include "MK60D10.h"
#include "stdbool.h"

// Macros for bit-level registers manipulation
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))

// Port B, bit 5
#define LED_D9  0x20
// Port B, bit 4
#define LED_D10 0x10
// Port B, bit 3
#define LED_D11 0x8
// Port B, bit 2
#define LED_D12 0x4

// Port E, bit 10
#define BTN_RIGHT 0x400
// Port E, bit 12
#define BTN_DOWN 0x1000
// Port E, bit 27
#define BTN_LEFT 0x8000000
// Port E, bit 26
#define BTN_UP 0x4000000
// Port E, bit 11
#define BTN_ENT 0x800

// Speaker on PTA4
#define SPK 0x10

volatile bool wfi_call = false;

/// @brief Delays the execution
void delay(long long bound) {
    for(long long i = 0; i < bound; i++);
}

/// @brief Sequentially turns on and off the speaker to make a beep sound
void beep(void) {
	for (uint32_t q = 0; q < 200; q++) {
		GPIOA_PDOR |= SPK;
		delay(200);
		GPIOA_PDOR &= ~SPK;
		delay(200);
	}
}

/// @brief Initializes MCU - basic clock settings, watchdog off
void mcu_init(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/// @brief Initializes ports on the MCU
void ports_init(void) {
    // Turns clock on ports
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK
        | SIM_SCGC5_PORTE_MASK
        | SIM_SCGC5_PORTA_MASK;

    // Sets pins connected to LEDs for GPIO
    PORTB->PCR[5] = PORT_PCR_MUX(0x01);
    PORTB->PCR[4] = PORT_PCR_MUX(0x01);
    PORTB->PCR[3] = PORT_PCR_MUX(0x01);
    PORTB->PCR[2] = PORT_PCR_MUX(0x01);

    // Configures buttons, enables interupt
    int buttons[] = {10, 12, 27, 26, 11};
    for (int i = 0; i < sizeof(buttons) / sizeof(int); ++i) {
        PORTE->PCR[buttons[i]] = PORT_PCR_MUX(0x01)
            | PORT_PCR_ISF(0x01)
            | PORT_PCR_IRQC(0x0A)
            | PORT_PCR_PE(0x01)
            | PORT_PCR_PS(0x01);
    }
    NVIC_EnableIRQ(PORTE_IRQn);

    // Sets pins connected to speaker for GPIO
    PORTA->PCR[4] = PORT_PCR_MUX(0x01);

    // Changes pins as outputs for LED's and speaker
    PTB->PDDR = GPIO_PDDR_PDD(0x3C);
    PTA->PDDR = GPIO_PDDR_PDD(SPK);
    PTB->PDOR |= GPIO_PDOR_PDO(0x3C);
    PTA->PDOR &= GPIO_PDOR_PDO(~SPK);
}

/// @brief Enters normal wait mode (WAIT)
void wait_mode(void) {
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    wfi_call = true;
}

/// @brief Enters stop mode corresponding to given value
/// @param mode stop mode type based on STOPM in SMC_PMCTRL
/// 0 Normal Stop
/// 2 Low-Power Stop
/// 3 Low-Leakage Stop
/// 4 Very-Low-Leakage Stop
void stop_mode(int mode) {
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	SMC->PMCTRL = SMC_PMCTRL_STOPM(mode);
    wfi_call = true;
}

/// @brief Enters the Very Low Leakage stop mode with corresponding sub-mode
/// @param sub_mode sub mode representing which VLLS to use
/// b001 VLLS1
/// b010 VLLS2
/// b011 VLLS3
void vlls_mode(int sub_mode) {
    SMC_VLLSCTRL &= ~0x7;
    SMC_VLLSCTRL |= 0x3;
    stop_mode(4);
}

/// @brief Buttons interupt handler
void PORTE_IRQHandler(void) {
	delay(1000);

    // Very-low-leakage stop (VLLS) on right press
    if (PORTE->ISFR & BTN_RIGHT) {
        vlls_mode(1);
    }
    // Low-leakage stop (LLS)
    if (PORTE->ISFR & BTN_DOWN) {
        stop_mode(3);
    }
    // Low-power stop (LPS)
    if (PORTE->ISFR & BTN_LEFT) {
        stop_mode(2);
    }
    // Normal stop (STOP)
    if (PORTE->ISFR & BTN_UP) {
        stop_mode(0);
    }
    // Normal wait (WAIT)
    if (PORTE->ISFR & BTN_ENT) {
        wait_mode();
    }

	delay(1000);
	PORTE->ISFR = PORTE->ISFR;
}

int main(void) {
	mcu_init();
    ports_init();

    beep();
    int light = 0;
    while (1) {
        GPIOB_PDOR &= ~LED_D9;
        delay(light);
        light = (light + 1) % 1000;
        GPIOB_PDOR |= LED_D9;
        delay(1000);

        if (wfi_call) {
            __WFI();
            wfi_call = false;
        }
    }

    return 0;
}
