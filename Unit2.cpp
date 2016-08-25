//---------------------------------------------------------------------------
#include <FileCtrl.hpp>
#include <vcl.h>
#include <IniFiles.hpp>
//#include <FMX.Dialogs.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
TIniFile *Ini = new TIniFile(".//Config.ini");
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2Click(TObject *Sender)
{
	Edit1->Text = Ini->ReadString("PATH","PATH_RAR","BAD");
	Edit2->Text = Ini->ReadString("PATH","PATH_BASE","BAD");       //���������� ���������� �� INI
	Edit3->Text = Ini->ReadString("PATH","PATH_COPY_BASE","BAD");
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
Button2Click(Sender);
}
//---------------------------------------------------------------------------
 void __fastcall TForm2::SpeedButton1Click(TObject *Sender)
{
	OpenDialog1->FileName="C:\\Program Files\\WinRAR\\WinRAR.exe";
	if(OpenDialog1->Execute())
	{
		Edit1->Text = OpenDialog1->FileName;
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm2::SpeedButton3Click(TObject *Sender)
{
  String Dir;
  if(SelectDirectory("Select Directory", "", Dir))
  	Edit3->Text=Dir+"\\";
  if(Edit3->Text!="")
  	Edit3->Text=Edit3->Text.Delete(1,1);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::SpeedButton2Click(TObject *Sender)
{
  String Dir;
  if(SelectDirectory("Select Directory", "", Dir))
  Edit2->Text=Dir+"\\";
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1Click(TObject *Sender)
{
	Ini->WriteString("PATH","PATH_RAR",Edit1->Text);
	Ini->WriteString("PATH","PATH_BASE",Edit2->Text);       //������ ���������� � INI
	Ini->WriteString("PATH","PATH_COPY_BASE",Edit3->Text);
}
//---------------------------------------------------------------------------

