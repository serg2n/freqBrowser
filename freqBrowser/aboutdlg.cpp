
#include "aboutdlg.h"
#include "stdafx.h"
#include "resource.h"
#include "utfwstring.h"

#include <windowsx.h>


AboutDlg::AboutDlg(UINT resID) : CDialog(resID)
{
;
}
//------------------------------------------
AboutDlg::~AboutDlg()
{
}
#if 1
//------------------------------
void AboutDlg::OnDestroy()
{
    // End the application
    Close();
}
//----------------------------------------------
BOOL AboutDlg::OnInitDialog()
{
    // Set the application icon
    SetIconLarge(IDI_LOGO);
    SetIconSmall(IDI_LOGO);

    AttachItem(IDC_STATSPECTR, m_about);

    HBITMAP bitmap = reinterpret_cast<HBITMAP>(GetApp()->LoadImage(IDB_SPECTR, IMAGE_BITMAP,m_about.GetClientRect().Width(),m_about.GetClientRect().Height(), LR_SHARED));
    m_about.SetBitmap(bitmap);

    return true;
}
//--------------------------------------------
INT_PTR AboutDlg::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
//        switch (msg)
//        {
//        }
        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    return 0;
}
//---------------------------------------------
BOOL AboutDlg::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
//    switch (id)
//    {

//    }
    return FALSE;
}
#endif
