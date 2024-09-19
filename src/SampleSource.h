#ifndef __sample_source_h__
#define __sample_source_h__

typedef struct
{
    int16_t left;
    int16_t right;
} Frame_t;

class SampleSource
{
public:
    virtual int sampleRate() = 0;
    virtual void getFrames(Frame_t *frames, int number_frames) = 0;
    virtual ~SampleSource() {}
};

#endif
