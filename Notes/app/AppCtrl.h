#pragma once
#include "NoteDlg.h"

class CAppCtrl
{
public:
	CAppCtrl(void);
	~CAppCtrl(void);

	void Init();

private:
	vector<CNoteDlg*> m_NoteListDlg;
};

