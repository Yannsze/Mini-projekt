/* main.c
   This file written 2024 by Artur Podobas and Pedro Antunes
   For copyright and licensing, see file COPYING */

/* Below functions are external and found in other files. */

#include "menu.h"

extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime(int);
extern void enable_interrupt();

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";
int prime = 1234567;

#define timer_status (*(volatile unsigned int *) 0x04000020)
#define timer_control (*(volatile unsigned int *) 0x04000024)
#define timer_periodl (*(volatile unsigned int *) 0x04000028)
#define timer_periodh (*(volatile unsigned int *) 0x0400002C)

#define toggle_reg (*(volatile unsigned int *) 0x04000010)
#define toggle_regOffset (*(volatile unsigned int *) 0x04000018)

void set_leds(int led_mask);
void set_displays(int display_number, int value);
int get_sw(void);
int get_btn(void);

int timeoutcount = 0;

/* Timer and interrupt initialization */
void labinit(void) {
  // initialize timer every 100ms (10 times a second), dtekv = 30MHz
  timer_control = 0x8;   // disable timer 
  timer_periodl = 3000000 & 0xFFFF; // load lower 16bits
  timer_periodh = (3000000 >> 16) & 0xFFFF; // load upper 16bits
  timer_status = 0;  // clear time out flag (1 == timer is done, reached zero)
  timer_control = 0x7;  // enable start + count(removed**) + interrupt request

  toggle_regOffset = 0x4;

  enable_interrupt();
}

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {
  if (cause == 16) {

    if (timer_status & 1) {
      timer_status = 0; // clear flag
      timeoutcount++;
        // Kod för periodisk uppdatering, menyblink? status?
      }
      // Knapptryckningar och toggles här om vil ha snabb respons
    }
  }

/* LED- och displayfunktioner */
void set_leds(int led_mask) {
  // map leds
  volatile unsigned int *led_reg = (volatile unsigned int *) 0x04000000;   
  // mask 10 lsb
  *led_reg = led_mask & 0x3FF;                                
}

void set_displays(int display_number, int value) {
  int offset = 0x10;

  // invalid display_number
  if (display_number < 0 || display_number > 5) 
    return;  

  // map to the 7 segments
  volatile unsigned int *display_reg = (volatile unsigned int *) (0x04000050 + display_number * offset);

  unsigned char digit[10] = {
    0x40, // 0
    0x79,
    0x24,
    0x30,
    0x19,
    0x12,
    0x02,
    0x78,
    0x00,
    0x18  // 9
  };

  if (value < 0 || value > 9)
    return;

  *display_reg = digit[value];
}

// get status of the toggle 
int get_sw(void) {
  return toggle_reg & 0x3FF;
}

// get status of the second button
int get_btn(void) {
  volatile unsigned int *btn_reg = (volatile unsigned int *) 0x040000d0;
  return *btn_reg & 0x1;
}

//Huvudloopen. Meny och bildbehandling
int main ( void ) {
  labinit();

  menu_state_t menu;
  menu_init(&menu);

  while (1) {
    // TODO: Lägg till menyhantering, kernelval, bildbehandling osv.
    // Exempelvis:
    // 1. Läs toggles/knappar
    // 2. Uppdatera meny/status på LED/displayer
    // 3. Kör vald kernel på bilddata
    // 4. Hantera upload/download/reset

    int toggles = get_sw();
    int btn = get_btn();
    menu_update(&menu, toggles, btn);
    menu_show(&menu);

    // Använd menu.kernel_selected, menu.kernel_size, menu.run_mode osv i din logik
  }
  return 0;
}