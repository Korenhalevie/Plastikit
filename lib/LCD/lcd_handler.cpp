#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "global_pins.h"

// Define the LCD address and dimensions
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Create an instance of the LiquidCrystal_I2C class
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void initializeLCD() {
    // Initialize I2C communication with custom pins
    Wire.begin(I2C_SDA, I2C_SCL);

    // Initialize the LCD
    lcd.begin(LCD_COLUMNS, LCD_ROWS);

    // Turn on the LCD backlight
    lcd.backlight();

    // Print initial message to the LCD
    lcd.setCursor(0, 0);
    lcd.print("LCD Initialized");
}

void displayMessage(const char* message1, const char* message2) {
    lcd.clear();

    if (message1 != nullptr && strlen(message1) > 0) {
        lcd.setCursor(0, 0);
        lcd.print(message1);
    }

    if (message2 != nullptr && strlen(message2) > 0) {
        lcd.setCursor(0, 1);
        lcd.print(message2);
    }
}
