#include <Arduino.h>
#include <SPIFFS.h>
#include "WAVFileReader.h"
#include "I2SOutput.h"
#include "BeepGenerator.h"

// Global variables
WAVFileReader *sampleSource;
I2SOutput *output;
BeepGenerator *beepSource;
#define SD_PIN 11
i2s_pin_config_t i2sPins = {
        .bck_io_num = GPIO_NUM_14,   // bclk
        .ws_io_num = GPIO_NUM_13,    // lrc
        .data_out_num = GPIO_NUM_12, // din
        .data_in_num = -1            // Not used
    };

void playBeep(float volume)
{
    // Create a short beep with the given volume
    beepSource = new BeepGenerator(1000.0f, 200, volume); // 1kHz beep, 200ms duration

    // Stop the current playback and play the beep
    output->stop();
    delay(200); // Wait for the beep to finish
    output->start(I2S_NUM_1, i2sPins, beepSource);

    

    // Resume the WAV file playback
    output->stop(); // Stop beep playback
    output->resume();
    output->start(I2S_NUM_1, i2sPins, sampleSource);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting up");

    analogWrite(SD_PIN, 955); // Set SD_PIN to 0.77V (for stereo mode selection)

    SPIFFS.begin();

    if (!SPIFFS.exists("/24khz_med.wav"))
    {
        Serial.println("WAV file not found!");
        return;
    }

    sampleSource = new WAVFileReader("/24khz_med.wav", 0.2f); // Default volume 0.2

    Serial.println("Starting I2S Output");

    

    output = new I2SOutput();
    output->start(I2S_NUM_1, i2sPins, sampleSource);

    Serial.println("Audio playback started. Enter volume (0.0 to 1.0) to adjust:");
}

void loop()
{
    if (Serial.available() > 0)
    {
        float volume = Serial.parseFloat();
        volume = constrain(volume, 0.0f, 1.0f); // Ensure valid range

        // Play the beep to indicate volume change
        playBeep(volume);

        // Set the new volume for the WAV file
        sampleSource->setVolume(volume);
        Serial.printf("Volume set to %.2f\n", volume);

        while (Serial.available())
        {
            Serial.read(); // Clear the buffer
        }
    }
}
