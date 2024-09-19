#ifndef __i2s_output_h__
#define __i2s_output_h__

#include <Arduino.h>
#include "driver/i2s.h"

class SampleSource;

class I2SOutput
{
private:
    TaskHandle_t m_i2sWriterTaskHandle;
    i2s_port_t m_i2sPort;
    SampleSource *m_sample_generator;

public:
    void start(i2s_port_t i2sPort, i2s_pin_config_t &i2sPins, SampleSource *sample_generator);
    void stop();   // Stops the I2S output
    void resume(); // Resumes the I2S output

    friend void i2sWriterTask(void *param);
};

#endif