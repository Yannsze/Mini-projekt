// menu.c
#include "menu.h"
#include "dtekv-lib.h" // för print/debug

// Meny med standardvärden
void menu_init(menu_state_t* state) {
    state->kernel_selected = KERNEL_EDGE;
    state->kernel_size = 3;
    state->run_mode = 0;
    state->upload = 0;
    state->download = 0;
    state->reset = 0;
}

// Uppdatera meny baserat på toggles och knapp
// toggles[0-1]=kernel, toggles[2]=size, toggles[3]=run_mode, btn=action

void menu_update(menu_state_t* state, int toggles, int btn) {
    // Kernelval: toggles 0-1 (två bitar)
    state->kernel_selected = (kernel_type_t)(toggles & 0x3);

    // Kernelstorlek: toggle 2 (0=3x3, 1=5x5)
    state->kernel_size = (toggles & 0x4) ? 5 : 3;   // (toggles & 0x4) ? 5 : 3; betyder: 
                                                    // Om toggle 2 värde är satt till 1, dvs nedtryckt, så blir kernel_size 5.
                                                    // Om toggle 2 värde är satt till 0, dvs INTE nedtryckt, så blir kernel_size 3.

    // Run mode: toggle 3 (0=single, 1=chain)
    state->run_mode = (toggles & 0x8) ? 1 : 0;

    // Upload: toggle 4 (håll nere för upload)
    state->upload = (toggles & 0x10) ? 1 : 0;

    // Download: toggle 5 (håll nere för download)
    state->download = (toggles & 0x20) ? 1 : 0;

    // Reset: toggle 6 (håll nere för reset)
    state->reset = (toggles & 0x40) ? 1 : 0;

    // Använda btn för att "bekräfta" val? eller starta bearbetning?
    // Typ om btn trycks och upload är valt, trigga upload
}

// Visa meny/status på LED (kerneltyp på LED 0-1, storlek på LED 2, run_mode på LED 3)
void menu_show(const menu_state_t* state) {
    int led_mask = 0;
    led_mask |= (state->kernel_selected & 0x3);      // LED 0-1: kerneltyp
    led_mask |= (state->kernel_size == 5) << 2;      // LED 2: kernelstorlek
    led_mask |= (state->run_mode) << 3;              // LED 3: run mode
    led_mask |= (state->upload) << 4;                // LED 4: upload
    led_mask |= (state->download) << 5;              // LED 5: download
    led_mask |= (state->reset) << 6;                 // LED 6: reset

    // Anropa LED-funktion
    set_leds(led_mask);
}