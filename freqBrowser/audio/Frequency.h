#pragma once

#include <portaudio.h>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <kiss_fft.h>

//#include <Windows.h>
#include "wxx_wincore.h"
#include "wxx_controls.h"

using namespace std;

class Frequency:public CWnd
{
public:
    Frequency();
    void Initialize();
    void Terminate();
    wstring print_device_info();
    void getInpDevices(CComboBoxEx *m_comb);
    void setDevIndex(PaDeviceIndex deviceInd){deviceIndex=deviceInd;}
    double freqValue;
    int freqMult;
    bool evEnable;
    bool blockView;
    std::mutex mtx;
    int sampNumb;
    std::vector<kiss_fft_cpx> fftVal;
    std::vector<kiss_fft_cpx> fftValr;
private:
	PaDeviceIndex deviceIndex = 8;
	const char* deviceName = nullptr;
	PaStream* stream;
	PaError paError;
    std::vector<float> left;
    std::vector<float> right;
    void check_error(PaError err);
	int callbackExecutionCounter = 0;

    static int streamCallback(const void* input, void*, unsigned long frameCount, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags , void* userData);
};
