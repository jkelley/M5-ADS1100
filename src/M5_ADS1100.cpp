#include "M5_ADS1100.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

// Abstract away platform differences in Arduino wire library
static uint8_t i2cread(void) {
#if ARDUINO >= 100
    return Wire.read();
#else
    return Wire.receive();
#endif
}

// Abstract away platform differences in Arduino wire library
static void i2cwrite(uint8_t x) {
#if ARDUINO >= 100
    Wire.write((uint8_t)x);
#else
    Wire.send(x);
#endif
}

/*! @brief Writes 8-bits to the destination register.*/
static void writeRegister(uint8_t i2cAddress, uint8_t value) {
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/*! @brief Reads 16-bits from the destination register.*/
static uint16_t readRegister(uint8_t i2cAddress) {
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | i2cread());
}

/*! @brief Get ADS1100 I2C address */
uint8_t ADS1100::getAddr(void) {
    return ads_i2cAddress;
}

/*! @brief Set ADS1100 I2C address */
void ADS1100::setAddr(uint8_t i2cAddress) {
    ads_i2cAddress = i2cAddress;
}

/*! @brief Sets up the Hardware.*/
void ADS1100::begin() {
    Wire.begin();
}

/*! @brief Sets the Operational status/single-shot conversion start
        This determines the operational status of the device.*/
void ADS1100::setOSMode(adsOSMode_t osmode) {
    ads_osmode = osmode;
}

/*! @brief Gets the Operational status/single-shot conversion start.*/
adsOSMode_t ADS1100::getOSMode() {
    return ads_osmode;
}

/*! @brief Sets the Device operating mode
    This controls the current operational mode of the ADS1100.*/
void ADS1100::setMode(adsMode_t mode) {
    ads_mode = mode;
}

/*! @brief Gets the Device operating mode.*/
adsMode_t ADS1100::getMode() {
    return ads_mode;
}

/*! @brief Sets the Date Rate.
    This controls the data rate setting.*/
void ADS1100::setRate(adsRate_t rate) {
    ads_rate = rate;
}

/*! @brief Gets the Date Rate.*/
adsRate_t ADS1100::getRate() {
    return ads_rate;
}

/*! @brief Sets the gain and input voltage range
    This configures the programmable gain amplifier.*/
void ADS1100::setGain(adsGain_t gain) {
    ads_gain = gain;
}

/*! @brief Gets a gain and input voltage range.*/
adsGain_t ADS1100::getGain() {
    return ads_gain;
}

/*! @brief Reads the conversion results, measuring the voltage
    difference between the P and N input
    Generates a signed value since the difference can be either
    positive or negative.*/
int16_t ADS1100::Measure_Differential() {
    uint16_t config = 0;

    // Set Operational status/single-shot conversion start
    config |= ads_osmode;

    // Set Device operating mode
    config |= ads_mode;

    // Set Data rate
    config |= ads_rate;

    // Set PGA/voltage range
    config |= ads_gain;

    // Write config register to the ADC
    writeRegister(ads_i2cAddress, config);

    // Wait for the conversion to complete
    delay(ads_conversionDelay);

    // Read the conversion results
    uint16_t raw_adc = readRegister(ads_i2cAddress);
    return (int16_t)raw_adc;
}
