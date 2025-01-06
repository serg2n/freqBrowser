

  # Freqbrowser
Visualization of the frequency spectrum of an audio signal for different sources.
   [ Win32++](https://win32-framework.sourceforge.net), [Portaudio](https://github.com/PortAudio/portaudio), [Kissfft](https://github.com/mborgerding/kissfft) libraries were used. The idea to combine [Portaudio](https://github.com/PortAudio/portaudio) and [Kissfft](https://github.com/mborgerding/kissfft) was borrowed from the [tuner-app](https://github.com/danielanthonyl/tuner-app) project by [danielanthonyl](https://github.com/danielanthonyl).
   

# Building

   To develop and build the project we use Qt Creator 4.5.1. The project build configuration is described in the [freqBrowser.pro](https://github.com/serg2n/freqBrowser/blob/main/freqBrowser/freqBrowser.pro)  file. To build the project we use Microsoft Visual C++ Compiler 15.0 (amd64).
   To install the compiler, use vs_BuildTools_2017 or VS2017. For the resource editor we use the excellent program [RisohEditor by katahiromz](https://github.com/katahiromz/RisohEditor).
   Header files and static dependency libraries are located in the project's [include](https://github.com/serg2n/freqBrowser/tree/main/include) and [lib](https://github.com/serg2n/freqBrowser/tree/main/lib) directories.


# Screenshot
![](img/screen.png)
![](https://github.com/serg2n/freqBrowser/blob/main/img/screen.PNG)

Explanation of the picture:


* ComboBox on top for selecting audio signal device
* CheckBox Left and Right for channel selection
* dB buttons to increase/decrease the sensitivity of the spectrum analyzer
* buttons below for expanding/narrowing the viewing area
* Slider for discrete movement of the viewing band within the frequency spectrum
* button play/stop to start/stop playing the spectrum graph
* The amplitude scale is displayed in dB, the values ​​are relative
* Frequency scale in Hz

# Operating system load
Windows 10 
Intel(R) Core(TM) i5-8500 CPU @ 3.00GHz /16GB:


* CPU load 0.5 - 1.5%
* Memory 2.7 - 3.6 MB