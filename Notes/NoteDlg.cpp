// NoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Notes.h"
#include "NoteDlg.h"
#include "afxdialogex.h"


// CNoteDlg dialog

IMPLEMENT_DYNAMIC(CNoteDlg, CDialogEx)

CNoteDlg::CNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoteDlg::IDD, pParent)
{

}

CNoteDlg::~CNoteDlg()
{
}

void CNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoteDlg, CDialogEx)
END_MESSAGE_MAP()


// CNoteDlg message handlers


BOOL CNoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNoteDlg::SetWindownAlpha( float fAlpha )
{
	ModifyStyleEx(0, WS_EX_LAYERED); 
	SetLayeredWindowAttributes(0, 255 * fAlpha, LWA_ALPHA);
}

void CNoteDlg::SetMouseThought()
{
	DWORD dwNewLong = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
	dwNewLong |= WS_EX_TRANSPARENT |WS_EX_LAYERED; 
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwNewLong);
}

void CNoteDlg::Init()
{
	SetMouseThought();
	SetWindownAlpha(m_Note.GetNoteGroup().nOpacity / 100.0);
	MoveWindow(m_Note.GetNoteGroup().rect);
	ShowWindow(m_Note.GetNoteGroup().bVisible ? SW_SHOW : SW_HIDE);
}


