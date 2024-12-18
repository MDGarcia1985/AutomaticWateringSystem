#include <Arduino.h>    // Standard Arduino library for basic functions
#include <avr/sleep.h>  // For low-power sleep mode
#include <avr/wdt.h>    // For Watchdog Timer to manage timed sleep
#include <util/delay.h> // For precise delays (_delay_ms)

// Pin Definitions
#define stepperPin1 6      // Stepper Motor IN1 (connected to ULN2003)
#define stepperPin2 10     // Stepper Motor IN2
#define stepperPin3 A1     // Stepper Motor IN3
#define stepperPin4 A2     // Stepper Motor IN4
#define pumpPin 9          // Pump control pin connected via transistor
#define dhtPin 7           // DHT11 sensor data pin
#define d4 5               // LCD D4 pin
#define d5 4               // LCD D5 pin
#define d6 3               // LCD D6 pin
#define d7 2               // LCD D7 pin
#define rs 12              // LCD RS pin
#define en 11              // LCD Enable pin

// Stepper Motor Configuration
#define STEPS_PER_REV 200  // Number of steps per revolution for the stepper motor

// Stepper Motor Position Tracking
int currentStepperPosition = 0; // Tracks the current position of the stepper motor

// Function Prototypes
void initializeComponents();                  // Initialize all hardware components
void displayReadings(float temp, float hum); // Display temperature and humidity readings
void waterPlants();                          // Handle the plant watering process
void goToSleep(unsigned long duration);      // Enter low-power sleep mode for a set duration
void stepMotor(int steps);                   // Move the stepper motor by a specified number of steps
void moveStepperTo(int targetPosition);      // Move the stepper motor to a specific position
void pumpOn();                               // Turn on the water pump
void pumpOff();                              // Turn off the water pump
void testComponents();                      // Test all components

int main(void) {
    init(); // Initialize the Arduino core functionality
    Serial.begin(9600); // Start serial communication for debugging

    initializeComponents(); // Set up all hardware components

    while (1) { // Main program loop
        testComponents(); // Test hardware components at the start of each loop

        // Simulated DHT sensor readings (replace with actual sensor code)
        float temp = 25.0; // Example temperature in Celsius
        float hum = 45.0;  // Example humidity as a percentage

        // Check for invalid sensor readings
        if (isnan(temp) || isnan(hum)) {
            Serial.println("Sensor error!"); // Display error message on Serial Monitor
            delay(2000);                    // Wait 2 seconds before retrying
            goToSleep(600000);              // Sleep for 10 minutes to conserve power
            continue;                       // Skip the rest of this loop iteration
        }

        displayReadings(temp, hum); // Show temperature and humidity on the Serial Monitor

        // Check conditions to determine if watering is needed
        if (hum < 50 && temp > 15) { // Example thresholds: humidity < 50% and temp > 15°C
            waterPlants();           // Initiate the watering process
        }

        goToSleep(600000); // Sleep for 10 minutes to save power
    }

    return 0; // This line will never be reached
}

void initializeComponents() {
    // Set pin modes for hardware components
    pinMode(stepperPin1, OUTPUT);
    pinMode(stepperPin2, OUTPUT);
    pinMode(stepperPin3, OUTPUT);
    pinMode(stepperPin4, OUTPUT);
    pinMode(pumpPin, OUTPUT);

    // Ensure the pump is initially off
    pumpOff();

    // Simulate LCD Initialization with Serial Monitor
    Serial.println("System Init..."); // Display initialization message
    delay(2000);                      // Wait for 2 seconds
    Serial.println("Ready");          // Indicate system is ready
}

void displayReadings(float temp, float hum) {
    // Simulated LCD output using Serial Monitor
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.println("°C");

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println("%");
}

void waterPlants() {
    // Water Plant A
    Serial.println("Watering Plant A"); // Indicate which plant is being watered
    moveStepperTo(0);                   // Move stepper to position for Plant A
    pumpOn();                           // Turn on the water pump
    delay(5000);                        // Water for 5 seconds
    pumpOff();                          // Turn off the pump

    delay(2000); // Wait 2 seconds before switching to the next plant

    // Water Plant B
    Serial.println("Watering Plant B"); // Indicate which plant is being watered
    moveStepperTo(100);                 // Move stepper to position for Plant B
    pumpOn();                           // Turn on the water pump
    delay(5000);                        // Water for 5 seconds
    pumpOff();                          // Turn off the pump
}

void stepMotor(int steps) {
    // Move the stepper motor step by step
    for (int i = 0; i < abs(steps); i++) {
        if (steps > 0) { // Step forward
            digitalWrite(stepperPin1, HIGH);
            _delay_ms(1);
            digitalWrite(stepperPin1, LOW);
        } else { // Step backward
            digitalWrite(stepperPin2, HIGH);
            _delay_ms(1);
            digitalWrite(stepperPin2, LOW);
        }
    }
    currentStepperPosition += steps; // Update the stepper position
}

void moveStepperTo(int targetPosition) {
    // Move the stepper to a specific position
    stepMotor(targetPosition - currentStepperPosition); // Calculate steps to move
}

void pumpOn() {
    digitalWrite(pumpPin, HIGH); // Activate the pump
}

void pumpOff() {
    digitalWrite(pumpPin, LOW); // Deactivate the pump
}

void goToSleep(unsigned long duration) {
    // Enter low-power sleep mode using the Watchdog Timer
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Configure power-down mode
    sleep_enable();                      // Enable sleep mode

    unsigned long sleepCycles = duration / 8000; // Calculate the number of 8-second sleep cycles

    while (sleepCycles > 0) {
        // Configure watchdog timer for ~8-second timeout
        WDTCSR = (1 << WDCE) | (1 << WDE);  // Enable watchdog changes
        WDTCSR = (1 << WDP3) | (1 << WDP0); // Set timeout
        WDTCSR |= (1 << WDIE);              // Enable interrupt

        sleep_mode(); // Enter sleep mode
        sleep_disable(); // Wake up
        sleepCycles--; // Decrement sleep cycle counter
    }

    WDTCSR = 0x00; // Disable the watchdog timer after waking up
}

void testComponents() {
    Serial.println("Testing Components...");

    // Test Stepper Motor
    Serial.println("Testing Stepper Motor...");
    moveStepperTo(50);  // Move stepper halfway
    delay(1000);        // Wait
    moveStepperTo(0);   // Return to start
    delay(1000);

    // Test Pump
    Serial.println("Testing Pump...");
    pumpOn();           // Activate the pump
    delay(2000);        // Run for 2 seconds
    pumpOff();          // Deactivate the pump
    delay(1000);

    Serial.println("Component Tests Complete");
}
