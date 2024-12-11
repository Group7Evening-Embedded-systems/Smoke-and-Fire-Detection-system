MQ-22......Sensor
#define PORTC_REG (*(volatile uint8_t *)0x28) // PORTC
#define DDRC_REG (*(volatile uint8_t *)0x27)  // DDRC
#define PINC_REG (*(volatile uint8_t *)0x26)  // PINC

// Define the pin for the MQ-2 sensor (Analog Pin A0 = PC0)
#define MQ2_PIN 0

void setup() {
  // Configure the MQ-2 sensor pin (PC0) as input
  DDRC_REG &= ~(1 << MQ2_PIN);  // Clear bit 0 of DDRC to set PC0 as input

  // Disable the pull-up resistor (not needed for analog input)
  PORTC_REG &= ~(1 << MQ2_PIN);

  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
}

uint16_t readAnalog(uint8_t pin) {
  // Configure ADC registers
  ADMUX = (1 << REFS0) | pin;   // AVcc as reference, select input channel
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC, start conversion, prescaler 128

  // Wait for conversion to complete
  while (ADCSRA & (1 << ADSC));

  return ADC;
}

void loop() {
  // Read the analog value from the MQ-2 sensor
  uint16_t gasLevel = readAnalog(MQ2_PIN);

  // Print gas level via serial communication
  // Serial.print("Gas Level: ");
  // Serial.println(gasLevel);

  // Send gas level as a byte (for external processing)
  if (gasLevel > 300) {  // Threshold for gas detection
    // Serial.println("Gas Detected!");
    Serial.write('1');
  } else {
    // Serial.println("Safe Environment");
    Serial.write('0');
  }

  // Add a delay using the built-in delay() function
  delay(1000); // 1-second delay
}