#include <Arduino.h>                // Standard Arduino library for basic functions
#include <Sodaq_wdt.h>              // For Watchdog Timer
#include <Adafruit_LiquidCrystal.h> // For the LCD
#include <Stepper.h>                // For the Stepper motor
#include <DHT.h>                    // For the DHT11 sensor

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

// Configuration
#define STEPS_PER_REV 200  // Number of steps per revolution for the stepper motor
#define DHTTYPE DHT11      // Define the DHT sensor type

// Initialize Components
Adafruit_LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // LCD object
Stepper stepper(STEPS_PER_REV, stepperPin1, stepperPin3, stepperPin2, stepperPin4); // Stepper motor object
DHT dht(dhtPin, DHTTYPE); // DHT11 object

// Stepper Motor Position Tracking
int currentStepperPosition = 0; // Tracks the current position of the stepper motor

// Function Prototypes
void initializeComponents();                  // Initialize all hardware components
void displayReadings(float temp, float hum); // Display temperature and humidity readings
void waterPlants();                          // Handle the plant watering process
void goToSleep(unsigned long duration);      // Enter low-power sleep mode for a set duration
void moveStepperTo(int targetPosition);      // Move the stepper motor to a specific position
void pumpOn();                               // Turn on the water pump
void pumpOff();                              // Turn off the water pump
void testComponents();                       // Test all components

int main(void)
{
    init(); // Initialize the Arduino core functionality
    Serial.begin(9600); // Start serial communication for debugging

    initializeComponents(); // Set up all hardware components

    while (1)
    {
        testComponents(); // Test hardware components at the start of each loop

        // Read sensor values
        float temp = dht.readTemperature();
        float hum = dht.readHumidity();

        // Check for invalid sensor readings
        if (isnan(temp) || isnan(hum))
        {
            lcd.clear();
            lcd.print("Sensor error!"); // Display error on LCD
            delay(2000);               // Wait 2 seconds
            goToSleep(600000);         // Sleep for 10 minutes
            continue;                  // Skip this iteration
        }

        displayReadings(temp, hum); // Show temperature and humidity on the LCD

        // Check conditions to determine if watering is needed
        if (hum < 50 && temp > 15)
        { 
            waterPlants(); // Initiate watering
        }

        goToSleep(600000); // Sleep for 10 minutes
    }

    return 0; // This line will never be reached
}

void initializeComponents()
{
    // Initialize pins for the pump
    pinMode(pumpPin, OUTPUT);
    pumpOff(); // Ensure the pump is initially off

    lcd.begin(16, 2);       // Initialize the LCD
    lcd.print("System Init"); // Display initialization message
    delay(2000);
    lcd.clear();
    lcd.print("Ready");     // Indicate system is ready
    delay(1000);

    // Initialize DHT sensor
    dht.begin();

    // Initialize Stepper Motor
    stepper.setSpeed(60); // Set stepper motor speed (RPM)

    // Initialize Watchdog Timer
    sodaq_wdt_enable(WDT_PERIOD_8X); // Enable WDT with an 8-second timeout
}

void displayReadings(float temp, float hum)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(hum);
    lcd.print(" %");
}

void waterPlants()
{
    lcd.clear();
    lcd.print("Watering Plant A");
    moveStepperTo(0); // Move stepper to Plant A position
    pumpOn();
    delay(5000); // Water Plant A for 5 seconds
    pumpOff();

    delay(2000); // Wait before switching to Plant B

    lcd.clear();
    lcd.print("Watering Plant B");
    moveStepperTo(100); // Move stepper to Plant B position
    pumpOn();
    delay(5000); // Water Plant B for 5 seconds
    pumpOff();
}

void moveStepperTo(int targetPosition)
{
    int stepsToMove = targetPosition - currentStepperPosition;
    stepper.step(stepsToMove); // Move stepper
    currentStepperPosition = targetPosition;
}

void pumpOn()
{
    digitalWrite(pumpPin, HIGH); 
}

void pumpOff()
{
    digitalWrite(pumpPin, LOW); 
}

void goToSleep(unsigned long duration)
{
    unsigned long sleepCycles = duration / 8000; // Calculate number of 8-second cycles

    while (sleepCycles > 0)
    {
        sodaq_wdt_reset(); // Reset watchdog before sleep
        sodaq_wdt_safe_delay(8000); // Safe delay for 8 seconds
        sleepCycles--;
    }
}

void testComponents()
{
    lcd.clear();
    lcd.print("Testing LCD");
    delay(2000);

    lcd.clear();
    lcd.print("Testing Stepper");
    moveStepperTo(50); // Test Stepper movement
    delay(1000);
    moveStepperTo(0);  // Reset Stepper
    delay(1000);

    lcd.clear();
    lcd.print("Testing Pump");
    pumpOn();          // Test Pump
    delay(2000);
    pumpOff();         // Turn off Pump
    delay(1000);

    lcd.clear();
    lcd.print("Tests Complete");
    delay(2000);
}
