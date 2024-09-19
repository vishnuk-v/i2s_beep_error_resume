#ifndef __beep_generator_h__
#define __beep_generator_h__

#include "SampleSource.h"
#include <math.h>

class BeepGenerator : public SampleSource
{
private:
    int m_sample_rate;
    int m_duration_ms;
    float m_volume;
    float m_frequency;
    int m_current_sample;

public:
    BeepGenerator(float frequency, int duration_ms, float volume = 1.0f, int sample_rate = 44100)
        : m_frequency(frequency), m_duration_ms(duration_ms), m_volume(volume), m_sample_rate(sample_rate), m_current_sample(0) {}

    int sampleRate() { return m_sample_rate; }

    void getFrames(Frame_t *frames, int number_frames)
    {
        for (int i = 0; i < number_frames; i++)
        {
            if (m_current_sample >= (m_sample_rate * m_duration_ms) / 1000)
            {
                frames[i].left = 0;
                frames[i].right = 0;
                continue;
            }

            float amplitude = sin(2 * M_PI * m_frequency * m_current_sample / m_sample_rate);
            int16_t sample = (int16_t)(amplitude * 32767 * m_volume);

            frames[i].left = sample;
            frames[i].right = sample;
            m_current_sample++;
        }
    }
};

#endif