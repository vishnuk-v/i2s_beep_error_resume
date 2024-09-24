#include "I2SOutput.h"
#include "SampleSource.h"

#define NUM_FRAMES_TO_SEND 512

void i2sWriterTask(void *param)
{
    I2SOutput *output = (I2SOutput *)param;
    Frame_t frames[NUM_FRAMES_TO_SEND];
    size_t bytesWritten = 0;

    while (true)
    {
        output->m_sample_generator->getFrames(frames, NUM_FRAMES_TO_SEND);
        i2s_write(output->m_i2sPort, (const char *)frames, NUM_FRAMES_TO_SEND * sizeof(Frame_t), &bytesWritten, portMAX_DELAY);
    }
}

void I2SOutput::start(i2s_port_t i2sPort, i2s_pin_config_t &i2sPins, SampleSource *sample_generator)
{
    m_sample_generator = sample_generator;

    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = m_sample_generator->sampleRate(),
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 1024
    };

    m_i2sPort = i2sPort;

    i2s_driver_install(m_i2sPort, &i2sConfig, 0, NULL);
    i2s_set_pin(m_i2sPort, &i2sPins);
    i2s_zero_dma_buffer(m_i2sPort);

    xTaskCreate(i2sWriterTask, "i2s Writer Task", 4096, this, 1, &m_i2sWriterTaskHandle);
}

void I2SOutput::stop()
{
    if (m_i2sWriterTaskHandle)
    {
        vTaskDelete(m_i2sWriterTaskHandle); // Stop the writer task
        m_i2sWriterTaskHandle = NULL;
    }
}
// void I2SOutput:: resume(){
// if(m_i2sWriterTaskHandle == NULL){
//     xTaskCreate(i2sWriterTask, "i2s Writer Task", 4096, this, 1, &m_i2sWriterTaskHandle);
// }
// }

