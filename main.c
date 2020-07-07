#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Initialization function for graphics
void initializeGraphics(Graphics_Context* g_sContext_p) {
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontFixed6x8);

    // Clear the screen
    Graphics_clearDisplay(g_sContext_p);
}

// Function to restart 2 second timer
void restartTimer() {
    Timer32_setCount(TIMER32_0_BASE, 6000000); // 2 seconds
    Timer32_startTimer(TIMER32_0_BASE, true);
}

// Function to check if timer is expired
bool isTimerExpired() {
    return (Timer32_getValue(TIMER32_0_BASE) == 0);
}

int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initializations
    Graphics_Context g_sContext; // Create context
    initializeGraphics(&g_sContext);

    // Initialize and start a one-shot timer
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    restartTimer();

    while(!isTimerExpired()); // Wait 2 seconds

    // Change background color
    // Notice that the LCD screen is still colored black
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    restartTimer(); // Restart the timer

    while(!isTimerExpired()); // Wait 2 seconds

    // Clear display
    // Now, the LCD screen is colored blue!
    Graphics_clearDisplay(&g_sContext);

    while(1);
}
