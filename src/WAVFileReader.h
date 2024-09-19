#ifndef __WAVFILEREADER_H__
#define __WAVFILEREADER_H__

#include <SPIFFS.h>
#include <FS.h>
#include "SampleSource.h"

class WAVFileReader : public SampleSource
{
private:
    int m_num_channels;
    int m_sample_rate;
    File m_file;
    float m_volume; // Volume control factor

public:
    WAVFileReader(const char *file_name, float volume = 1.0f);
    ~WAVFileReader();
    int sampleRate() { return m_sample_rate; }
    void getFrames(Frame_t *frames, int number_frames);
    void setVolume(float volume) { m_volume = constrain(volume, 0.0f, 1.0f); }
};

#endif
