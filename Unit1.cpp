//---------------------------------------------------------------------------

#include <vcl.h>
#include "tlhelp32.h"
#include <shellapi.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TIniFile *Ini = new TIniFile(".//Config.ini");
TStringList *Base = new TStringList;
int Sec=0;
AnsiString PATH_RAR, PATH_BASE, PATH_COPY_BASE;
//---------------------------------------------------------------------------
bool SetPriorProcessByPid(DWORD *pid, int flags)              //������� ��� ����������� PID ����������
        {
         HANDLE ProcessHandle;
         ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,false,*pid);
         if (!SetPriorityClass(ProcessHandle, flags)) return false;
         CloseHandle(ProcessHandle);
         return true;
		}

bool GetProcessByExeName(DWORD* Pid,String ExeName)       //2� ������� ��� PID
{
HANDLE hProcessSnap = NULL;
PROCESSENTRY32 pe32      = {0};
String temp="";
hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
pe32.dwSize=sizeof(PROCESSENTRY32);
if(Process32First(hProcessSnap,&pe32)) temp=pe32.szExeFile;
if((temp.UpperCase()).Pos(ExeName.UpperCase()))
	   {
       *Pid= pe32.th32ProcessID;
       CloseHandle (hProcessSnap);
       return true;
       }
pe32.dwSize=sizeof(PROCESSENTRY32);
while(Process32Next(hProcessSnap,&pe32))
        {
        temp=pe32.szExeFile;
        if((temp.UpperCase()).Pos(ExeName.UpperCase()))
                {
                *Pid= pe32.th32ProcessID;
                CloseHandle(hProcessSnap);
                return true;
                }
        pe32.dwSize=sizeof(PROCESSENTRY32);
		}
CloseHandle(hProcessSnap);
*Pid=0;
return false;
}
//---------------------------------------------------------------------------
AnsiString Console (AnsiString text, int n, float Procent)
	{
		if(n==0)
			Form1->Memo1->Lines->Strings[n]=text + FloatToStr(Procent)+"%";
		if(n==1)
			if(Form1->Memo1->Lines->Strings[1]=="")
				Form1->Memo1->Lines->Add(text+ FloatToStr(Procent));
			else
				Form1->Memo1->Lines->Strings[1]=text+ FloatToStr(Procent);
		if(n>1)
			Form1->Memo1->Lines->Add(text);
	}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	PATH_RAR = Ini->ReadString("PATH","PATH_RAR","BAD");
	PATH_BASE = Ini->ReadString("PATH","PATH_BASE","BAD");       //���������� ���������� �� INI
	PATH_COPY_BASE = Ini->ReadString("PATH","PATH_COPY_BASE","BAD");

	Memo1->Clear();
	Base->Clear();
	Button2->Visible=false;
	Button1->Visible=false;
	int i,k=0;
	AnsiString S;
	String a,b;
	AnsiString USB = "ABCDEFJHIGKLMNOPQRSTUVWXWZ",PATH=":\\Programs\\Antivirus\\Kaspersky v6.0.4.1611\\";
	for(i=1;i<=26;i++)
		{
			Console("������������� ���������..",0,( i* 100/26 ) );
			Label1->Caption=USB[i];
			if(DirectoryExists(Label1->Caption+PATH_COPY_BASE))       //������������� ���������
				{
				   k=1;
					Base->Add(Label1->Caption+PATH_COPY_BASE);
				}
			Console("- ���������� ���������: ",1,k);
				Sleep(20);
		}
	if(k>0)
		{
			a=PATH_BASE+"Update.rar";
			b=Base->Strings[0]+"Update.rar";
			DeleteFile(a.w_str());    //�������� ����� �� �����
			DeleteFile(b.w_str());    //�������� ����� � ������
			Sleep(1000);
			Console("������ �������� ��������� ������.",2,0);    //������ �������� ��������� ������.
			S =PATH_RAR + " a -r -m0 -dh -ow "+ PATH_BASE+ "Update.rar "+ PATH_BASE +"Update\\*.* ";
			WinExec(S.c_str(),SW_SHOWDEFAULT);
			Timer1->Enabled=true;
		}
	else
		{
			Button2->Visible=true;
			Button1->Visible=true;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete Base;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Sec++;
	DWORD *pid = new DWORD;
	int i;
	String a,b;
	b=Base->Strings[0]+"Update.rar";
	if(Sec>2)
		{
			GetProcessByExeName(pid,"WinRar.exe");
			if(FileExists(PATH_BASE+"Update.rar"))
				{
				if(*pid==0)
					{
						Console("- ��������� ������ ��������� �������.",2,0);
						Console("����������� ������.",2,0);    //������ �������� ��������� ������.
						for(i=0;i<=Base->Count-1;i++)
							{
								a=PATH_BASE+"Update.rar";   //�����������
								b=Base->Strings[i]+"Update.rar";
														Timer1->Enabled=false;
								if(!CopyFile(a.w_str(),b.w_str(),false))
									Console("- ����������� ������ �� �������.",2,0);
								else
									Console("- ����������� ������ ���������.",2,0);	   //False - ����������� � �������
							}
					}
				}
			else
				{
					Console("- ������ ��������� ������.",2,0);
					Timer1->Enabled=false;
					Button2->Visible=true;
					Button1->Visible=true;
				}
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	int screenW=GetSystemMetrics(SM_CXSCREEN);
	this->Left=screenW-this->Width-5;
	this->Top=5;
	Timer2->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
	Sec++;
	if(Sec>=2)
		{
			Button2Click(Sender);
			Sec=0;
			Timer2->Enabled=false;
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Form2->Show();
	Form2->Top=this->Top;
	Form2->Left=GetSystemMetrics(SM_CXSCREEN)-
			   (GetSystemMetrics(SM_CXSCREEN)-this->Left)-Form2->Width;
}
//---------------------------------------------------------------------------

