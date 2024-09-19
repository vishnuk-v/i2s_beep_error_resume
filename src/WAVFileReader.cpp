#include <SPIFFS.h>
#include <FS.h>
#include "WAVFileReader.h"

#pragma pack(push, 1)
typedef struct
{
    char riff_header[4]; // Contains "RIFF"
    int wav_size;        // Size of the wav portion of the file
    char wave_header[4]; // Contains "WAVE"
    char fmt_header[4];  // Contains "fmt " (includes trailing space)
    int fmt_chunk_size;  // Should be 16 for PCM
    short audio_format;  // Should be 1 for PCM
    short num_channels;
    int sample_rate;
    int byte_rate;
    short sample_alignment;
    short bit_depth;
    char data_header[4]; // Contains "data"
    int data_bytes;
} wav_header_t;
#pragma pack(pop)

WAVFileReader::WAVFileReader(const char *file_name, float volume)
    : m_volume(volume)
{
    if (!SPIFFS.exists(file_name))
    {
        Serial.println("Failed to open file!");
        return;
    }

    m_file = SPIFFS.open(file_name, "r");
    wav_header_t wav_header;
    m_file.read((uint8_t *)&wav_header, sizeof(wav_header_t));

    if (wav_header.bit_depth != 16)
    {
        Serial.printf("ERROR: Unsupported bit depth: %d\n", wav_header.bit_depth);
    }

    m_num_channels = wav_header.num_channels;
    m_sample_rate = wav_header.sample_rate;
}

WAVFileReader::~WAVFileReader()
{
    m_file.close();
}

void WAVFileReader::getFrames(Frame_t *frames, int number_frames)
{
    for (int i = 0; i < number_frames; i++)
    {
        if (m_file.available() == 0)
        {
            m_file.seek(44); // Skip header on repeat
        }

        int16_t sample;
        m_file.read((uint8_t *)&sample, sizeof(int16_t));
        frames[i].left = sample * m_volume;

        if (m_num_channels == 1)
        {
            frames[i].right = frames[i].left;
        }
        else
        {
            m_file.read((uint8_t *)&sample, sizeof(int16_t));
            frames[i].right = sample * m_volume;
        }
    }
}
