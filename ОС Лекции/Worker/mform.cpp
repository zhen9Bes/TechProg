//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner), worker()
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartBtnClick(TObject *Sender)
{
    worker.startWork();    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopBtnClick(TObject *Sender)
{
    worker.stopWork();    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TimerTimer(TObject *Sender)
{
    if( worker.isDataUpdated() )
        Memo->Lines->Add( IntToStr( worker.getData() ) );    
}
//---------------------------------------------------------------------------

