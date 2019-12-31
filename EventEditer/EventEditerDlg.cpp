// EventEditerDlg.cpp : �����t�@�C��
//

#define _EVENT_EDITER_

#include "stdafx.h"
#include "EventEditer.h"
#include "EventEditerDlg.h"
#include ".\eventediterdlg.h"

#include "../eventdata.h"
#include "../mapconst.h"
#include "../dataconst.h"
#include "../battleconst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INIT_FNAME	"EventEditer.txt"

#define NUMBER_OF_LABEL		256
#define NUMBER_OF_ECONST	256
#define END_HEAD		"_END_HEAD_"
#define START_HEAD		"_START_HEAD_"

int		g_LINE;

typedef struct{
	int Line;
	char Name[128];
}LABEL;

struct EventConst{
	char* Name;
	int num;

	EventConst(){Name=NULL;};
	EventConst(char* n,int no){Name=n;num=no;};
};

const EventConst EConst[NUMBER_OF_ECONST]={
	EventConst("LeftTop"		,MAP_MES_LEFT_TOP),
	EventConst("LeftBottom"		,MAP_MES_LEFT_BOTTOM),
	EventConst("RightTop"		,MAP_MES_RIGHT_TOP),
	EventConst("RightBottom"	,MAP_MES_RIGHT_BOTTOM),
	EventConst("Center"			,MAP_MES_CENTER),

	EventConst("NoSound"		,SMES_NO_SOUND),

	EventConst("="				,IF_EQUAL),
	EventConst(">"				,IF_BIGGER),
	EventConst("<"				,IF_SMALLER),
	EventConst("!="				,IF_NOT_EQUAL),

	EventConst("GravNone"		,GRAV_NONE),
	EventConst("GravJump"		,GRAV_JUMP),
	EventConst("GravNoJump"		,GRAV_NO_JUMP),
	EventConst("GravLock"		,GRAV_LOCK),
	EventConst("GravAir"		,GRAV_AIR),
	EventConst("GravFallNone"	,GRAV_FALL_NONE),
	EventConst("GravWaterSki"	,GRAV_WATER_SKI+GRAV_JUMP),
	EventConst("GravHiJump"		,GRAV_HI_JUMP+GRAV_JUMP),

	EventConst("EQ"				,ID_EQ),
	EventConst("UI"				,ID_UI),
	EventConst("NSUI"			,ID_NSUI),
	EventConst("BirthStone"		,ITEM_BIRTH_STONE),
	EventConst("Key"			,ITEM_KEY),

	EventConst("BlockShadow"	,OBJOPT_BLOCK_SHADOW),
	EventConst("NoShadow"		,OBJOPT_NO_SHADOW),
	EventConst("LightShadow"	,OBJOPT_LIGHT_SHADOW),
	EventConst("ShowTurn"		,OBJOPT_SHOW_TURN),
	EventConst("NoGroundHit"	,OBJOPT_NO_GROUND_HIT),
	EventConst("Heavy1"			,OBJOPT_HEAVY_1),
	EventConst("Heavy2"			,OBJOPT_HEAVY_2),
	EventConst("Wall0"			,OBJOPT_WALL_0),
	EventConst("Wall1"			,OBJOPT_WALL_1),

	EventConst("LPx2"			,BOPT_LPUP_x2),
	EventConst("LPx3"			,BOPT_LPUP_x3),
	EventConst("LP_HALF"		,BOPT_LP_HALF),
	EventConst("POWx15"			,BOPT_POWUP_x15),
	EventConst("POW_HALF"		,BOPT_POW_HALF),
	EventConst("ShortEscape"	,BOPT_SHORT_ESCAPE),
	EventConst("PUpLeaderOnly"	,BOPT_P_UP_LEADER_ONLY),
	EventConst("PUpWOutLeader"	,BOPT_P_UP_WOUT_LEADER),
	EventConst("PUpWOutBig"		,BOPT_P_UP_WOUT_BIG),
	EventConst("EscapeOK"		,BOPT_ESCAPE_OK),
};

LABEL Label[NUMBER_OF_LABEL];
void Compile(FILE *pInput,FILE *pLog,const char* inputname);
int Error(char* mes,const char* filename,FILE *pLog,FILE *pEvent,FILE *pTmp);
int SearchLabel(char* labelname);
bool ReadExpression(char* script,int& pointer,FILE* pEvent,FILE* pLog);

void CompileInclude(FILE *pLog,const char* includefile);


CEventEditerDlg::CEventEditerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventEditerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEventEditerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, 1011, mEditBox);
}

BEGIN_MESSAGE_MAP(CEventEditerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_SAVE_AS, OnSaveAs)
	ON_COMMAND(ID_COMP, OnComp)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
//	ON_EN_CHANGE(1011, OnEnChange1011)
//	ON_EN_SETFOCUS(1011, OnEnSetfocus1011)
ON_EN_UPDATE(1011, OnEnUpdateEditbox)
ON_COMMAND(ID_SAVE_NC, OnSaveNc)
ON_COMMAND(ID_SAVE_AS_NC, OnSaveAsNc)
ON_COMMAND(ID_NEXT_FILE, OnNextFile)
ON_COMMAND(ID_BEFORE_FILE, OnBeforeFile)
END_MESSAGE_MAP()


// CEventEditerDlg ���b�Z�[�W �n���h��

BOOL CEventEditerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	strcpy(mFilename,INIT_FNAME);
	SetWindowText(mFilename);
	mOldCursor=0;
	
	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CEventEditerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CEventEditerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEventEditerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
#ifndef _DEBUG
	mEditBox.MoveWindow(0,0,cx,cy,true);
#endif
}
bool CEventEditerDlg::CheckSave()
{
	if(strcmp(mFilename,INIT_FNAME)){
		int ret=AfxMessageBox("�J���O�ɕۑ����Ƃ��H",MB_YESNOCANCEL,0);
		if(ret==IDYES){
			OnSave();
			return true;
		}else if(ret==IDCANCEL){
			return false;
		}
	}
	return true;
}
void CEventEditerDlg::OpenFile()
{
	FILE* pFile;
	if(pFile=fopen(mFilename,"rb")){
		int n;
		fseek(pFile,0,SEEK_END);
		n=ftell(pFile);
		char* text;
		text=new char[n+1];
		fseek(pFile,0,SEEK_SET);
		fread(text,1,n,pFile);
		text[n]='\0';

		mEditBox.SetWindowText(text);
		fclose(pFile);
	}else{
		mEditBox.SetWindowText(NULL);
	}
}

void CEventEditerDlg::OnOpen()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B

	if(!CheckSave())	return;

	CFileDialog fdlg(TRUE,"txt",mFilename,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�e�L�X�g�t�@�C��(txt)|*.txt|",this);
	if(fdlg.DoModal()==IDOK){
		strcpy(mFilename,fdlg.GetFileName());
		OpenFile();
	}
	SetWindowText(mFilename);
}
void CEventEditerDlg::MoveFile(int n)
{
	if(!CheckSave())	return;

	char fno[64];
	strcpy(fno,&mFilename[5]);
	int no=atoi(fno);

	sprintf(mFilename,"event%04d.txt",no+n);
	OpenFile();

	SetWindowText(mFilename);
}
void CEventEditerDlg::OnNextFile()
{
	MoveFile(1);
}

void CEventEditerDlg::OnBeforeFile()
{
	MoveFile(-1);
}

void CEventEditerDlg::OnSave()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	if(strcmp(mFilename,INIT_FNAME)==0){
		OnSaveAs();
		return;
	}
	
	FILE* pFile;
	if(pFile=fopen(mFilename,"wb")){
		int n;
		n=mEditBox.GetWindowTextLength();
		char* text;
		text=new char[n+1];
		mEditBox.GetWindowText(text,n+1);
		fwrite(text,1,n,pFile);

		fclose(pFile);
	}
	if(pFile=fopen(mFilename,"r")){
		FILE* pLog=NULL;
		pLog=fopen("_log.txt","w");
		if(pLog){
			Compile(pFile,pLog,mFilename);
			fclose(pLog);
		}else{
			Error("���O�t�@�C�����Ђ炯�܂���ł����B",mFilename,NULL,NULL,NULL);
			return;
		}

		fclose(pFile);
	}
}

void CEventEditerDlg::OnSaveAs()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CFileDialog fdlg(FALSE,"txt",mFilename,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�e�L�X�g�t�@�C��(txt)|*.txt|",this);
	if(fdlg.DoModal()==IDOK){
		CString fname;
		strcpy(mFilename,fdlg.GetFileName());
		SetWindowText(mFilename);
		OnSave();
	}
}
void CEventEditerDlg::OnComp()
{
}
void Compile(FILE *pInput,FILE *pLog,const char* inputname)
{
	FILE* pEvent=NULL;
	FILE* pTmpFile=NULL;

	if(!(pLog)){
		Error("���O�t�@�C�����Ђ炯�܂���ł����B",inputname,pLog,pEvent,pTmpFile);
		return;
	}

	char outname[512];
	strcpy(outname,inputname);
	int i=strlen(outname);
	outname[i-4]='\0';
	if(!(pEvent=fopen(outname,"wb"))){
		Error("�o�̓t�@�C�����Ђ炯�܂���ł����B",inputname,pLog,pEvent,pTmpFile);
		return;
	}
	fprintf(pLog,"///////// �t�@�C���F%s //////////////////////////////////////////////////\n\n",inputname);

#define SCRIPT_SIZE	1024
	memset(Label,-1,sizeof(Label));
	char script[SCRIPT_SIZE];
	char command[128];
	int line=-1;

	fgets(script,SCRIPT_SIZE,pInput);
	if(script[0]=='#'){
		fprintf(pLog,"�t�@�C���`���F�C���N���[�h�t�@�C��\n\n");
		CompileInclude(pLog,inputname);
		fclose(pEvent);
		return ;
	}else{
		fprintf(pLog,"�t�@�C���`���F�ʏ�C�x���g�t�@�C��\n\n");
	}
	fprintf(pLog,"-----�C���N���[�h�t�@�C���̌��o�ɓ���܂��B-----\n\n");
	fseek(pInput,0,SEEK_SET);
	if( (pTmpFile = fopen("tmp","w+")) == NULL ){
		Error("�ꎞ�t�@�C�����J���܂���ł����B",inputname,pLog,pEvent,pTmpFile);
		return;
	}
	g_LINE=0;
	while(!feof(pInput)){
		fgets(script,SCRIPT_SIZE,pInput);
		if(script[0]=='#'){
			int size=strlen(script);
			char fname[256];
			memcpy(fname,&script[1],size);
			fname[size-2]='\0';
			FILE* pInclude;

			if( (pInclude = fopen(fname,"r")) == NULL ){
				char text[512];
				sprintf(text,"�C���N���[�h�t�@�C�����J���܂���ł����B<%s>\n",fname);
				Error(text,inputname,pLog,pEvent,pTmpFile);
				return;
			}
			fprintf(pLog,"�C���N���[�h�F%s\n",fname);
			fgets(script,SCRIPT_SIZE,pInclude);
			while(!feof(pInclude)){
				fgets(script,SCRIPT_SIZE,pInclude);
				fputs(script,pTmpFile);
			}
			fclose(pInclude);

			char ininame[256];
			sprintf(ininame,"%s.ini",fname);
			if( (pInclude = fopen(ininame,"a+")) == NULL ){
				char text[512];
				sprintf(text,"�C���N���[�h�ݒ�t�@�C�����J���܂���ł����B<%s>\n",fname);
				Error(text,inputname,pLog,pEvent,pTmpFile);
				return;
			}

			bool seted=false;
			while(!feof(pInclude)){
				fgets(script,SCRIPT_SIZE,pInclude);
				int size=strlen(script);
				script[size-1]='\0';
				if(strcmp(script,inputname)==0){
					seted=true;
					break;
				}
			}
			if(!seted){
				fprintf(pInclude,"%s\n",inputname);
			}
			fclose(pInclude);
			memset(script,0,sizeof(script));

		}else{
			fputs(script,pTmpFile);
		}
	}


	fprintf(pLog,"\n-----���x���̌��o�ɓ���܂��B-----\n\n");
	fseek(pTmpFile,i,0);

	while(!feof(pTmpFile)){
		i=ftell(pTmpFile);
		fgets(script,SCRIPT_SIZE,pTmpFile);
		if(script[0]=='\n')	continue;

		sscanf(script,"%128s",command);

		if(!strcmp(command,END_HEAD)){
			line=0;
		}else if(command[0]==':'){
			if(command[1]!=':'){
				for(int n=0;n<NUMBER_OF_LABEL;n++){
					if(Label[n].Line<0){
						Label[n].Line=line;
						strcpy(Label[n].Name,command);
						break;
					}else if(!strcmp(command,Label[n].Name)){
						Error("�����̃��x������������܂��B",inputname,pLog,pEvent,pTmpFile);
						return;
					}
				}
				if(n>=NUMBER_OF_LABEL){
					Error("���x�����������܂��B",inputname,pLog,pEvent,pTmpFile);
					return;
				}
				fprintf(pLog,"���x�� %s %d�s��\n",Label[n].Name,Label[n].Line);
			}else{
				fprintf(pLog,"�R�����g %s \n",command);
			}
		}else if(line>=0){
			fseek(pTmpFile,i,SEEK_SET);
			int beforelength=0;
			while(!feof(pTmpFile)){
				fgets(&(script[beforelength]),SCRIPT_SIZE-beforelength,pTmpFile);
				if(script[0]=='\n')	continue;

				int length=strlen(script)-1;
				bool exit=false;
				for(int j=length-1;j>=beforelength;j--){
					if(script[j]==';'){
						exit=true;
						break;
					}else if(script[j]!=' ' && script[j]!='\n'){
						break;
					}
				}
				beforelength=length;
				if(exit){
					break;
				}
			}
			line++;
		}
	}

	fprintf(pLog,"\n\n-----��{���̌��o�ɓ���܂��B-----\n");
	fseek(pTmpFile,0,SEEK_SET);

	fgets(script,SCRIPT_SIZE,pTmpFile);
	fprintf(pLog,"�}�b�v���F%s",script);

	int tlen=strlen(script);
	for(i=0;i<=tlen;i++){
		putc(script[i]+64,pEvent);
	}

	fgets(script,SCRIPT_SIZE,pTmpFile);
	int pointer=0;
	fprintf(pLog,"�o������G�F",script);
	for(i=0;i<MAX_ENEMY_TYPE;i++){
		char text[64];
		for(int j=0;j<64;j++){
			text[j]=script[pointer];
			pointer++;
			switch(text[j]){
				case ',':	case '\n':
					text[j]='\0';
					goto out;
				case '\0':
					goto end;
			}
		}
out:
		int enemy=atoi(text);
		_putw(enemy,pEvent);
		fprintf(pLog,"%d , ",enemy);
	}
end:
	_putw(-1,pEvent);

	fprintf(pLog,"\n\n-----�C�x���g�̌��o�ɓ���܂��B-----\n\n");

	while(!feof(pTmpFile)){
		fgets(script,SCRIPT_SIZE,pTmpFile);
		if(script[0]=='\n')	continue;

		sscanf(script,"%128s",command);
		if(strcmp(command,END_HEAD)==0){
			_putw(-1,pEvent);
			break;
		}

		for(int m=0;m<MAX_EVENT_HEADER;m++){
			if(!strcmp(command,EventHeaderInfo[m].mCommandName)){
				char text[512];
				int parama,paramb;
				char labname[128];
				sscanf(script,"%128s %d %d %128s",command,&parama,&paramb,labname);
				int labline=SearchLabel(labname);

				if(labline<0){
					sprintf(text,"���x����������܂���B %s\n",labname);
					Error(text,inputname,pLog,pEvent,pTmpFile);
					return;
				}

				fprintf(pLog,"�C�x���g %s\n  ParamA�F%d\n  ParamB�F%d\n  �ړ���F%d�s��\n\n",EventHeaderInfo[m].mCommandName,parama,paramb,labline);
				_putw(EventHeaderInfo[m].mEvent,pEvent);
				_putw(parama,pEvent);
				_putw(paramb,pEvent);
				_putw(labline,pEvent);
				break;
			}
		}
		if(m>=MAX_EVENT_HEADER){
			char text[512];
			sprintf(text,"���̂悤�ȃC�x���g�͑��݂��܂���B<%s>\n",command);
			Error(text,inputname,pLog,pEvent,pTmpFile);
			return;
		}
	}

	fprintf(pLog,"\n\n-----�{���̌��o�ɓ���܂��B-----\n\n");

	line=0;
	while(!feof(pTmpFile)){
		i=ftell(pTmpFile);
		fgets(script,SCRIPT_SIZE,pTmpFile);
		if(script[0]=='\n')	continue;

		if(script[0]==':'){
			continue;
		}
		fseek(pTmpFile,i,SEEK_SET);

		int beforelength=0;
		while(!feof(pTmpFile)){
			fgets(&(script[beforelength]),SCRIPT_SIZE-beforelength,pTmpFile);
			if(script[0]=='\n')	continue;

			int length=strlen(script)-1;
			bool exit=false;
			for(int j=length;j>=beforelength;j--){
				if(script[j]==';'){
					script[j]='\0';
					exit=true;
					break;
				}else if(script[j]!=' ' && script[j]!='\n'){
					break;
				}
			}
			beforelength=length;
			if(exit){
				break;
			}
		}
		sscanf(script,"%128s",command);


		for(int n=0;n<MAX_EVENT_PATTERN;n++){
			if(!EventDataInfo[n].mCommandName)	continue;
			if(!strcmp(command,EventDataInfo[n].mCommandName)){
				_putw(EventDataInfo[n].mEvent,pEvent);
				fprintf(pLog,"%d:%s \n",line,EventDataInfo[n].mCommandName);
				int pointer=strlen(command)+1;

		//�����Ƀp�����[�^���o������B
				if(EventDataInfo[n].mText){
					static unsigned char text[SCRIPT_SIZE];
					int tlen=0;
					while(pointer<SCRIPT_SIZE){
						if(script[pointer]==',' || script[pointer]=='\0'){
							text[tlen]='\0';
							pointer++;
							break;
						}else if(script[pointer]!=' ' && script[pointer]!='\n'){
							text[tlen]=script[pointer];
							tlen++;
						}
						pointer++;
					}
					fprintf(pLog,"  text : %s\n",text);
					for(i=0;i<=tlen;i++){
						putc(text[i]+64,pEvent);
					}
				}

				for(int param=0;param<EventDataInfo[n].mDataNo;param++){
					fprintf(pLog,"  Param%3d : ",param);
					if(!ReadExpression(script,pointer,pEvent,pLog)){
						Error("�����̍\�����K���ł͂���܂���B",inputname,pLog,pEvent,pTmpFile);
						return;
					}
					fprintf(pLog,"\n",n);
					pointer++;
				}

		//�����Ƀp�����[�^���o������B
				fprintf(pLog,"\n");
				break;
			}
		}
		if(n>=MAX_EVENT_PATTERN){
			char text[512];
			sprintf(text,"���̂悤�Ȗ��߂͑��݂��܂���B<%s>\n",command);
			Error(text,inputname,pLog,pEvent,pTmpFile);
			return;
		}
		line++;
		g_LINE++;
		memset(script,0,sizeof(script));
	}
	_putw(-1,pEvent);

	fclose(pEvent);
	fclose(pTmpFile);
	fprintf(pLog,"����I�����܂����B\n\n");
	return;
}

void PrintBikkuri(FILE *pLog)
{
	fprintf(pLog,"\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}
int Error(char* mes,const char* filename,FILE *pLog,FILE *pEvent,FILE *pTmp)
{
	char text[512];
	sprintf(text,"�R���p�C���G���[�F\n   �@%s\n\n�t�@�C�����F%s",mes,filename);
	AfxMessageBox(text,MB_OK,0);
	if(pLog){
		PrintBikkuri(pLog);
		fprintf(pLog,text);
		fprintf(pLog,"\n\n�ُ�I�����܂����B");
		PrintBikkuri(pLog);
//		fclose(pLog);
	}
	if(pEvent)	fclose(pEvent);
	if(pTmp)	fclose(pTmp);

	return 0;
}
int Warning(char* mes,FILE *pLog)
{
	char text[512];
	sprintf(text,"�x���F%d�s��\n   �@%s",g_LINE,mes);
	AfxMessageBox(text,MB_OK,0);
	if(pLog){
		PrintBikkuri(pLog);
		fprintf(pLog,text);
		PrintBikkuri(pLog);
	}
	return 0;
}
int SearchLabel(char* labelname)
{
	for(int n=0;n<NUMBER_OF_LABEL;n++){
		if(!strcmp(labelname,Label[n].Name)){
			return Label[n].Line;
		}
	}
	return -1;
}
bool putnumber(char* num,int& numlength,FILE* pEvent,FILE* pLog)
{
	if(numlength==0){
		return false;
	}
	num[numlength]='\0';
	numlength=0;

	if(num[0]==':'){
		int number=SearchLabel(num);
		if(number<0){
			char text[512];
			sprintf(text,"���̃��x���͑��݂��܂���B\n   �@  �u%s�v",num);
			Warning(text,pLog);
		}
		_putw(number,pEvent);
		fprintf(pLog,"%d",number);
		return false;
	}

	for(int i=0;i<MAX_SNUM_NUMBER;i++){
		if(SpecialNumberInfo[i].mName){
			if(strcmp(num,SpecialNumberInfo[i].mName)==0){
				_putw(SpecialNumberInfo[i].mSNumber,pEvent);
				fprintf(pLog,SpecialNumberInfo[i].mName);
				return SpecialNumberInfo[i].mArray;
			}
		}
	}

	for(i=0;i<NUMBER_OF_ECONST;i++){
		if(EConst[i].Name){
			if(strcmp(num,EConst[i].Name)==0){
				_putw(EConst[i].num,pEvent);
				fprintf(pLog,"%s(%d)",EConst[i].Name,EConst[i].num);
				return false;
			}
		}
	}

	if(num[0]<'0' || '9'<num[0]){
		char text[512];
		sprintf(text,"���̒萔�͑��݂��܂���B\n   �@  �u%s�v",num);
		Warning(text,pLog);
	}

	int number=atoi(num);
	_putw(number,pEvent);
	fprintf(pLog,"%d",number);
	return false;

}
bool ReadExpression(char* script,int& pointer,FILE* pEvent,FILE* pLog)
{
	int  nump=0;
	char number[64];
	bool ret=false;

	while(pointer<SCRIPT_SIZE){

		switch(script[pointer]){
			case ',':
			case '\0':
			case ']':
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(END,pEvent);
				return ret;
			case '+':
				if(!ret)	return false;
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(ADD,pEvent);
				putc(script[pointer],pLog);
				break;
			case '-':
				if(!ret)	return false;
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(SUB,pEvent);
				putc(script[pointer],pLog);
				break;
			case '*':
				if(!ret)	return false;
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(MUL,pEvent);
				putc(script[pointer],pLog);
				break;
			case '/':
				if(!ret)	return false;
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(DIV,pEvent);
				putc(script[pointer],pLog);
				break;
			case '%':
				if(!ret)	return false;
				if(putnumber(number,nump,pEvent,pLog))
							return false;
				putc(RES,pEvent);
				putc(script[pointer],pLog);
				break;
			case '[':
				if(!ret)	return false;
				if(!putnumber(number,nump,pEvent,pLog))
							return false;
				fprintf(pLog,"[");
				pointer++;
				if(!ReadExpression(script,pointer,pEvent,pLog))	return false;
				fprintf(pLog,"]");
				break;
			case ' ':
				break;
			case ';':
				return false;
			default:
				number[nump]=script[pointer];
				nump++;
		}
		pointer++;
		ret=true;
	}
	return false;
}

void CompileInclude(FILE *pLog,const char* includefile)
{
	FILE *pIni;
	char ininame[256];
	sprintf(ininame,"%s.ini",includefile);
	if( (pIni = fopen(ininame,"r+")) == NULL ){
		fprintf(pLog,"�C���N���[�h���͌�����܂���ł����B\n�R���p�C�������ɏI�����܂��B�B");
		return;
	}
	
	char fname[256];
	while(!feof(pIni)){
		fgets(fname,256,pIni);
		int size=strlen(fname);
		fname[size-1]='\0';

		FILE *pFile;
		if(pFile=fopen(fname,"r")){
			Compile(pFile,pLog,fname);
			fclose(pFile);
		}
	}
}
/**/

void CEventEditerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CDialog::OnDropFiles(hDropInfo);
}

void CEventEditerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	if(strcmp(mFilename,INIT_FNAME)){
		int ret=AfxMessageBox("�ۑ����Ƃ��H",MB_YESNO,0);
		if(ret==IDYES){
			OnSave();
		}
	}
}
void CEventEditerDlg::Change()
{
	int start,end,line;
	mEditBox.GetSel(start,end);
	line=mEditBox.LineFromChar(start);
	if(start==end){
		char text[1024],command[128];
		int length=mEditBox.GetLine(line,text,1024);
		sscanf(text,"%128s",command);
		for(int n=0;n<MAX_EVENT_PATTERN;n++){
			if(!EventDataInfo[n].mCommandName)	continue;
			if(!strcmp(command,EventDataInfo[n].mCommandName)){
				char title[256];
				sprintf(title,"%s   %s",mFilename,EventDataInfo[n].mHelp);
				SetWindowText(title);
				break;
			}
		}

		if(length<=0 && mOldCursor<=start){
			int length=mEditBox.GetLine(line-1,text,1024);
			if(length>0 && text[length-1]!=';'){
				int i;
				bool space=false;
				for(i=0;i<length;i++){
					if(text[i]==' '){
						space=true;
					}else if(space){
						break;
					}
				}
				if(space){
					memset(text,' ',i);
					text[i]=0;
					mEditBox.ReplaceSel(text);
				}
			}
		}
	}

	mOldCursor=start;
}

void CEventEditerDlg::OnEnUpdateEditbox()
{
	// TODO :  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āA
	// OR ��Ԃ� ENM_CORRECTTEXT �t���O���}�X�N�ɓ���āA
	// OR ��Ԃ� ENM_CORRECTTEXT �t���O�� IParam �}�X�N�̂ɓ���āA
	// OR ��Ԃ� ENM_UPDATE �t���O�� lParam �}�X�N�ɓ���āA

	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B

	Change();
}

void CEventEditerDlg::OnSaveNc()
{
	if(strcmp(mFilename,INIT_FNAME)==0){
		OnSaveAsNc();
		return;
	}
	
	FILE* pFile;
	if(pFile=fopen(mFilename,"wb")){
		int n;
		n=mEditBox.GetWindowTextLength();
		char* text;
		text=new char[n+1];
		mEditBox.GetWindowText(text,n+1);
		fwrite(text,1,n,pFile);

		fclose(pFile);
	}
}

void CEventEditerDlg::OnSaveAsNc()
{
	CFileDialog fdlg(FALSE,"txt",mFilename,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�e�L�X�g�t�@�C��(txt)|*.txt|",this);
	if(fdlg.DoModal()==IDOK){
		CString fname;
		strcpy(mFilename,fdlg.GetFileName());
		SetWindowText(mFilename);
		OnSaveNc();
	}
}
