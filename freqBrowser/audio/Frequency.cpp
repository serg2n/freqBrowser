#include "Frequency.h"
#include "utfwstring.h"
#include "resource.h"


#include <iostream>

using namespace std;


Frequency::Frequency()
{
    evEnable=false;
    blockView=false;
    sampNumb=0;
    freqMult=2;
}

void Frequency::Initialize()
{
    paError = Pa_Initialize();

    if (paError != paNoError)
    {
        printf("PA ERROR: %s\n", Pa_GetErrorText(paError));
    }

    /* OPEN STREAM */

    const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(deviceIndex);
    deviceName = deviceInfo->name;

    PaStreamParameters inputParameters{};
     inputParameters.device = deviceIndex;
    inputParameters.channelCount = deviceInfo->maxInputChannels;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;

    unsigned long nyquistFreq = static_cast<unsigned long>(deviceInfo->defaultSampleRate / 2);


    paError = Pa_OpenStream(&stream,
                            &inputParameters,
                            NULL,
                            deviceInfo->defaultSampleRate,
                            nyquistFreq,
                            paNoFlag,
                            streamCallback,
                            this);

    if (paError)
    {
        printf("PA ERROR: %s\n", Pa_GetErrorText(paError));
    }

    paError = Pa_StartStream(stream);

    if (paError)
    {
        printf("PA ERROR: %s\n", Pa_GetErrorText(paError));
    }
}

void Frequency::Terminate()
{
    paError = Pa_StopStream(stream);

    if (paError)
    {
        printf("PA ERROR: %s\n", Pa_GetErrorText(paError));
    }

    paError = Pa_CloseStream(stream);

    if (paError)
    {
        printf("PA ERROR: %s\n", Pa_GetErrorText(paError));
    }
}

int Frequency::streamCallback(const void* input, void*,
                              unsigned long frameCount,
                              const PaStreamCallbackTimeInfo * ,
                              PaStreamCallbackFlags ,
                              void* userData)
{
    Frequency* frequency = static_cast<Frequency*>(userData);
    frequency->evEnable=false;
    PaDeviceIndex deviceIndex = frequency->deviceIndex;
    float* inputStream = (float*)input;
    int inputChannels = Pa_GetDeviceInfo(deviceIndex)->maxInputChannels;
    double sampleRate = Pa_GetDeviceInfo(deviceIndex)->defaultSampleRate;
    unsigned long bufferSize =frameCount  * inputChannels;
    int fftSize = static_cast<int>(sampleRate / 2);

    //    std::vector<float> left(fftSize);
    //	std::vector<float> right(fftSize);
    frequency->left.resize(fftSize);
    frequency->right.resize(fftSize);
    int leftIndex = 0;
    int rightIndex = 0;

    for (int index = 0; index < static_cast<int>(bufferSize); index++) {
        if (index % 2 == 0) {
            if (leftIndex < fftSize) {
                frequency->left[leftIndex] = inputStream[index];
                leftIndex++;
            }
            else {
                // Handle buffer overflow
                break;
            }
        }
        else {
            if (rightIndex < fftSize) {
                frequency->right[rightIndex] = inputStream[index];
                rightIndex++;
            }
            else {
                // Handle buffer overflow
                break;
            }
        }
    }
#if 0
    for (int index = frameCount; index < fftSize; index++)
    {
        left[index] = 0.0;
        right[index] = 0.0;
    }
#endif
    kiss_fft_cpx* sampleStreamInput = new kiss_fft_cpx[fftSize];
    kiss_fft_cpx* sampleStreamOutput = new kiss_fft_cpx[fftSize];
    kiss_fft_cpx* sampleStreamOutput1 = new kiss_fft_cpx[fftSize];

    for (int index = 0; index < fftSize; index++)
    {
        sampleStreamInput[index].r = frequency->left[index];
        sampleStreamInput[index].i = 0;
    }

    kiss_fft_cfg configuration = kiss_fft_alloc(fftSize, 0, NULL, NULL);
    kiss_fft(configuration, sampleStreamInput, sampleStreamOutput);

    for (int index = 0; index < fftSize; index++)
    {
        sampleStreamInput[index].r = frequency->right[index];
        sampleStreamInput[index].i = 0;
    }

    kiss_fft_cfg configuration1 = kiss_fft_alloc(fftSize, 0, NULL, NULL);
    kiss_fft(configuration1, sampleStreamInput, sampleStreamOutput1);

    double peakMagnitude = 0.0;
    int peakIndex = 0;

    for (int index = 0; index < fftSize / 2; index++)
    {
        double outputFrequencyMagnitude = sampleStreamOutput[index].r;

        if (outputFrequencyMagnitude > peakMagnitude)
        {
            peakMagnitude = outputFrequencyMagnitude;
            peakIndex = index;
        }
    }
    frequency->mtx.lock();
    if(!frequency->blockView)
    {
        frequency->fftVal.clear();
        frequency->fftValr.clear();
        for(int k=0;k<fftSize;k++)
        {
            frequency->fftVal.push_back(sampleStreamOutput[k]);
            frequency->fftValr.push_back(sampleStreamOutput1[k]);
        }
        frequency->freqValue = static_cast<double>(peakIndex) * (static_cast<double>(bufferSize) / static_cast<double>(fftSize));
        frequency->freqMult=(int)(static_cast<double>(bufferSize) / static_cast<double>(fftSize));
        frequency->sampNumb++;
        frequency->evEnable=true;
    }
    frequency->mtx.unlock();
     frequency->callbackExecutionCounter++;

    delete[] sampleStreamInput;
    delete[] sampleStreamOutput;
    kiss_fft_free(configuration);
    delete[] sampleStreamOutput1;
    kiss_fft_free(configuration1);
    return paContinue;
}
//--------------------------------------
std::wstring  Frequency::print_device_info()
{
    PaError err = Pa_Initialize();
    std::wstring szInfo;
    check_error(err);
    int num_devices = Pa_GetDeviceCount(); // Pa_GetDeviceCount() returns the number of devices found
    // it returns -1 if there is an error
    printf("Number of devices: %d\n", num_devices);
    printf(" \n");
    szInfo.append(utf8_to_wstring("Number of devices: "+ std::to_string(num_devices)+"\n"));
    if (num_devices < 0)
    {
        printf("Error getting devices found\n");
        szInfo.append(utf8_to_wstring("Error getting devices found\n"));
        exit(1); // exit the program because it cant loop to get the device info
    }
    else if (num_devices == 0)
    {
        printf("No devices found\n");
        szInfo.append(utf8_to_wstring("No devices found\n"));
        exit(0); // exit the program because it cant loop to get the device info
        // is a success code, but it just means that there are no devices found
    }

    const PaDeviceInfo *device_info; // a constant pointer because Pa_GetDeviceInfo() returns a constant pointer
    // PaDeviceInfo is a struct that contains information about the device
    for (int i = 0; i < num_devices; i++)
    {
        device_info = Pa_GetDeviceInfo(i);
        printf("Device %d: %s\n", i, device_info->name);
        szInfo.append(utf8_to_wstring("Device: "+ std::to_string(i)+" "+device_info->name+"\n"));
        printf("Max input channels: %d\n", device_info->maxInputChannels);
        szInfo.append(utf8_to_wstring("Max input channels: "+ std::to_string(device_info->maxInputChannels)+"\n"));
        printf("Max output channels: %d\n", device_info->maxOutputChannels);
        szInfo.append(utf8_to_wstring("Max output channels: "+ std::to_string(device_info->maxOutputChannels)+"\n"));
        printf("Default sample rate: %f\n", device_info->defaultSampleRate);
        szInfo.append(utf8_to_wstring("Default sample rate: "+ std::to_string(device_info->defaultSampleRate)+"\n"));
        printf(" \n");
    }

    err = Pa_Terminate();
    check_error(err);
    return szInfo;
}
//--------------------------------------
void Frequency::getInpDevices(CComboBoxEx* m_comb)
{
    PaError err = Pa_Initialize();
    m_comb->Clear();
    check_error(err);
    int num_devices = Pa_GetDeviceCount(); // Pa_GetDeviceCount() returns the number of devices found
      if (num_devices < 0)
    {
        printf("Error getting devices found\n");
        exit(1); // exit the program because it cant loop to get the device info
    }
    else if (num_devices == 0)
    {
        printf("No devices found\n");
         exit(0); // exit the program because it cant loop to get the device info
        // is a success code, but it just means that there are no devices found
    }

    const PaDeviceInfo *device_info; // a constant pointer because Pa_GetDeviceInfo() returns a constant pointer
    // PaDeviceInfo is a struct that contains information about the device
    for (int i = 0; i < num_devices; i++)
    {
        device_info = Pa_GetDeviceInfo(i);
        if(device_info->maxInputChannels>0)
        {
            m_comb->AddString(utf8_to_wstring(device_info->name).c_str());
            m_comb->SetItemData(m_comb->GetCount()-1,i);
            //  szInfo.push_back(utf8_to_wstring(std::to_string(i)+": "+device_info->name));
        }
     }
    err = Pa_Terminate();
    check_error(err);
}
//--------------------------------------
void Frequency::check_error(PaError err)
{
    if (err != paNoError) // paNoError is a value of the PaError object that means there is no error
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        Pa_Terminate();
        exit(1); // exit the program with an error code
    }
}

