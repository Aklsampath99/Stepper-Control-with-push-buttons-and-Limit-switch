#include <Bounce2.h>  //Bounce2 library

// Define the pins for the A4988 driver
#define STEP_PIN 5      // Step pin connected to D5
#define DIR_PIN 2       // Direction pin connected to D2
#define STEP_PIN_L 7    // Second motor step pin connected to D7
#define DIR_PIN_L 4     // Second motor direction pin connected to D4
#define ENABLE_PIN 8    // Enable pin of two stepper motor drive connected to D8 on CNC shield

// Define the push buttons
const int CW_BUTTON = 12;    // Button to rotate first motor clockwise
const int CCW_BUTTON = 9;    // Button to rotate first motor counterclockwise
const int CW_BUTTON_L = 11;  // Button to rotate second motor clockwise
const int CCW_BUTTON_L = 10; // Button to rotate second motor counterclockwise

// Define the toggle switch pin (D6)
const int TOGGLE_BUTTON = 6;  // Toggle switch button
const int TOGGLE_PIN = 3;     // Pin to toggle high/low

// Define limit switch pins
#define LIMIT_SWITCH_1 A0 // Upper motor limit switch
#define LIMIT_SWITCH_2 A1 // Down motor limit switch
#define LIMIT_SWITCH_3 A2 // Limit switch between beds

int SPEED = 500;// Pulse width

bool toggleState = false;  // Current state of the toggle

// Bounce object for the toggle button
Bounce toggleDebouncer = Bounce();

void setup() {
  // Set pin modes for the A4988 driver
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN_L, OUTPUT);
  pinMode(DIR_PIN_L, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // Set pin modes for the buttons
  pinMode(CW_BUTTON, INPUT_PULLUP);
  pinMode(CCW_BUTTON, INPUT_PULLUP);
  pinMode(CW_BUTTON_L, INPUT_PULLUP);
  pinMode(CCW_BUTTON_L, INPUT_PULLUP);

  // Set pin mode for limit switches
  pinMode(LIMIT_SWITCH_1, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_2, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_3, INPUT_PULLUP);

  // Set pin mode for the toggle button and pin
  pinMode(TOGGLE_BUTTON, INPUT_PULLUP);
  pinMode(TOGGLE_PIN, OUTPUT);

  // Initialize the Bounce object
  toggleDebouncer.attach(TOGGLE_BUTTON);
  toggleDebouncer.interval(25);  // Set debounce interval (25ms)

  // Enable the driver
  digitalWrite(ENABLE_PIN, HIGH); // LOW to enable A4988 driver
  Serial.begin(9600);
}

void loop() {
  // Read limit switch states
  bool limit1Triggered = !digitalRead(LIMIT_SWITCH_1);
  bool limit2Triggered = !digitalRead(LIMIT_SWITCH_2);
  bool limit3Triggered = !digitalRead(LIMIT_SWITCH_3);

  // Check button states and control first motor
  if (digitalRead(CW_BUTTON) == LOW && !limit3Triggered) {
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN, HIGH); // Set direction to clockwise
    rotateStepper();

  } else if (digitalRead(CCW_BUTTON) == LOW && !limit1Triggered) {
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);  // Set direction to counterclockwise
    rotateStepper();
    
  }

  // Check button states and control second motor
  if (digitalRead(CW_BUTTON_L) == LOW && !limit3Triggered) {
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN_L, HIGH); // Set direction to clockwise
    rotateStepperL();
    
  } else if (digitalRead(CCW_BUTTON_L) == LOW && !limit2Triggered) {
    digitalWrite(ENABLE_PIN, LOW);
    digitalWrite(DIR_PIN_L, LOW);  // Set direction to counterclockwise
    rotateStepperL();
    
  }

  // Disable motors when no buttons are pressed
  if (digitalRead(CW_BUTTON) == HIGH && digitalRead(CCW_BUTTON) == HIGH &&
      digitalRead(CW_BUTTON_L) == HIGH && digitalRead(CCW_BUTTON_L) == HIGH) {
    digitalWrite(ENABLE_PIN, HIGH);
  }

  // Update the debounce object
  toggleDebouncer.update();

  // Check if the toggle button was pressed
  if (toggleDebouncer.fell()) {  // Detect a button press event
    toggleState = !toggleState;  // Toggle the state
    digitalWrite(TOGGLE_PIN, toggleState ? HIGH : LOW);  // Set pin state based on toggleState

    // Print the new pin state to Serial Monitor
    Serial.print("Toggle Pin State: ");
    Serial.println(toggleState ? "HIGH" : "LOW");
  }
}

// Function to step the first motor
void rotateStepper() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(SPEED);  // Adjust delay to control speed
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(SPEED);
}

// Function to step the second motor
void rotateStepperL() {
  digitalWrite(STEP_PIN_L, HIGH);
  delayMicroseconds(SPEED);  // Adjust delay to control speed
  digitalWrite(STEP_PIN_L, LOW);
  delayMicroseconds(SPEED);
}
