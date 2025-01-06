/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "DialogApp.h"

//#include <thread>

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

int main(int argc, char** argv)
{
    CDialogApp theApp;
       // Run the application.
//    std::thread t1(tunerStart); //стартуем tuner
//    t1.join();
     theApp.Run();
  //   printf("theApp.m_myDialog.GetHwnd() %d \n",(int)theApp.m_myDialog.GetWindow(0).GetHwnd());

    return 0;
}
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    try
    {
        // Start Win32++.
//       std::thread t1(tunerStart); //стартуем tuner
//       t1.join();
     //   CDialogApp theApp;
        // Run the application.
      //  return  theApp.Run();
        return main(__argc, __argv);
      }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return -1;
  }

