#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include "wxx_dialog.h"
#include "wxx_stdcontrols.h"
#include "wxx_controls.h"
//#include "Frequency.h"

class AboutDlg : public CDialog
{
public:
    AboutDlg(UINT resID);
    virtual ~AboutDlg();
protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy() override;
    virtual BOOL    OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
private:
    CStatic m_about;
};

#endif // ABOUTDLG_H
