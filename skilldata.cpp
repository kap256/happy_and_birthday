#include "skilldata.h"
#include "main.h"
#include "stdarg.h"
#include "gamewindow.h"
#include "menudata.h"
#include "battleconst.h"
#include "main.h"

const CSkillData 		NoneSkillData("�|�|�|",-1,SKILL_NONE,0);

CSkillData::CSkillData()
{
}

CSkillData::CSkillData(const char* name,int id,SkillType type,int param)
{
	mName=name;
	mID=id;
	mType=type;
	mParam=param;
}
void CSkillData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
//param&1	�K���ς݃X�L��
//param&2	���ʃX�L��
//param&4	�ڍוs���X�L��
//param&16	�������\�� 
	if(param & 1){
		window->DrawBox(x,y+8,999,16,0x40ff80ff);
		window->DrawText(0,"�K���ρI",10,y+6,0xffff0000,DT_RIGHT);
	}

	if(mType!=SKILL_NONE && mType!=SKILL_COMMAND){
		int dx,dy;
		switch(mType){
//			case SKILL_COMMAND:
//				dx=224;	dy=0;	break;
			case SKILL_AUTO:
				dx=192;	dy=32;	break;
			case SKILL_FIELD:
				dx=224;	dy=32;	break;
			default:
				dx=192;	dy=0;	break;
		}
		if(param & 16){
			window->DrawTexture(&SystemTex,0,dx,dy,32,32,x,y-2,24,24);
		}else{
			window->DrawTexture(&SystemTex,0,dx,dy,32,32,x,y-2);
		}
	}

	static char name[64];
	int namex=40;
	int namey=6;
	int font =0;
	if(param & 2){
		namex+=30;
	}
	
	if(param & 16){
		namex=20;
		namey=4;
		font=2;
	}

	if(param & 4){
		window->DrawText(font,"�H�H�H",x+namex,y+namey);
	}else if(mType==SKILL_COMMAND){
		int shiftx = 8;
		if(param&16)	shiftx=6;
		ComSkillData[mParam].Draw(window,x+shiftx,y+namey,8 + (param&18));
	}else{
		sprintf(name,"%s",mName);
		window->DrawText(font,name,x+namex,y+namey);
	}
	if(param & 2){
		window->DrawBox(x-10,y,999,2,0xff808040);
		if(mType!=SKILL_NONE){
			window->DrawText(2,"Special�I",x+20,y+namey+16,0xffff0000,DT_LEFT);
		}
	}
}
void CSkillData::Help(int param)const
{
	if(param & 4)	return;
	if(mType==SKILL_COMMAND){
		ComSkillData[mParam].Help(param);
		//MenuData.SetMenu(ComSkillHelp,mParam);
	}else{
		MenuData.SetMenu(SkillHelp,mID);
	}
}
const char*	CSkillData::GetName()const{
	static char name[64];
	if(mType==SKILL_COMMAND){
		sprintf(name,"�yC�z%s",ComSkillData[mParam].GetName());
	}else{
		switch(mType){
			case SKILL_STATUS:
				sprintf(name,"�yP�z%s",mName);	break;
			case SKILL_AUTO:
				sprintf(name,"�yA�z%s",mName);	break;
			case SKILL_FIELD:
				sprintf(name,"�yF�z%s",mName);	break;
		}
	}
	return name;
}
/////////////////////////////////////////////////////////////////////

CComSkillData::CComSkillData(void)
{
	memset(mQuickLoad,-1,sizeof(mQuickLoad));
}
CComSkillData::CComSkillData(const char* name,int id,int type,int sp,bool target,int range,int qload ...)
{
	mName=name;
	mID=id;
	mType=type;
	mSP=sp;
	mTarget=target;
	mRange=range;

	memset(mQuickLoad,-1,sizeof(mQuickLoad));
	int i=0;
	va_list marker;

	va_start( marker, qload ); 
	while(qload>=0 && i<MAX_QUICK_LOAD)
	{
		mQuickLoad[i]=qload;
		i++;
		qload = va_arg( marker, int);
	}
	va_end( marker );
}
unsigned int		CComSkillData::GetSortNo(void) const
{
	if(mID<8)	return mID;
	return ((mType&0xfff)<<20)+(mType&0x000ff000)+(mSP<<8)+mID;
}
void	CComSkillData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
//param&1 �g�p�s�\�X�L��
//param&2 ���ʃX�L��
//param&4 �ڍוs���X�L��
//param&8 ���x����\�����Ȃ�
//param&16 �������\��

	const int TYPE_MAX = 7;
	const D3DCOLOR ICON_COLOR[TYPE_MAX]=
	{
		0xff404040,
		0xff2040ff,
		0xffd0d000,
		0xffff2080,
		0xff00ff40,
		0xff404040,
		0xff3080ff
	};
	const D3DCOLOR MAGIC_COLOR[4]=
	{
		0xffff0060,
		0xff0030ff,
		0xff00ff20,
		0xffff3000
	};


	int slv=mSP%(MAX_CSKILL_LEVEL+1);
	int type=0;
	for(int i=0;i<TYPE_MAX;i++){
		if(mType & (1<<i)){
			type=i;
		}
	}
	
	int size = 16;
	if(param & 16){
		size=12;
	}
	window->DrawBox(x-1,y-1,size+2,size+2,COLOR_LINE);
	DTest.SetBlend(BLEND_ADD);

	D3DCOLOR iconc=ICON_COLOR[type];
	if(type==5){
		int mag=(mType&0x7000)>>12;
		if(mag>0){
			iconc=MAGIC_COLOR[mag-1];
		}
	}
	D3DCOLOR c=BlendColor(color,iconc);
	if(param & 16){
		window->DrawTexture(&SystemTex,0,80+type*16,48,16,16,x,y,size,size,0x60ffffff);
		window->DrawTexture(&SystemTex,0,80+type*16,48,16,16,x,y,size,size,c);
	}else{
		window->DrawTexture(&SystemTex,0,80+type*16,48,16,16,x,y,0x60ffffff);
		window->DrawTexture(&SystemTex,0,80+type*16,48,16,16,x,y,c);
	}
	DTest.SetBlend(BLEND_STANDARD);

	D3DCOLOR textcolor;
	if(param&1){
		textcolor=COLOR_GRAY;
	}else{
		textcolor=0;
	}

	int namex = 25;
	int namey = 0;
	int font =0;
	if(param & 16){
		namex=14;
		font=2;
	}else if(param&8){
		namex=32;
	}
	if(param & 2){
		namex+=30;
	}

	if(param&4){
		window->DrawText(font,"�H�H�H",x+namex,y,textcolor);
	}else{
		window->DrawText(font,mName,x+namex,y,textcolor);
		if(!(param&8)){
			static char text[32];
			sprintf(text,"SLv: %d",slv);
			window->DrawText(font,text,20,y,textcolor,DT_RIGHT);
		}
	}
}
void CComSkillData::Help(int param)const
{
	if(param & 4)	return;
	if(mType & CS_CALL_PET){
		MenuData.SetMenu(CallPetHelp,mID);
	}else{
		MenuData.SetMenu(ComSkillHelp,mID);
	}
}

///////////////////////////////////////////////////////////////////////////////
CStatusData::CStatusData(void)
{
}
CStatusData::CStatusData(const char* name,int id,const char* english,bool bad,bool show,int param,D3DCOLOR color,int se)
{
	mName=name;
	mID=id;

	mEnglish=english;
	mBad=bad;
	mShowParam=show;
	mParam=param;
	mColor=color;
	mSE=se;
}
void CStatusData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
	int dx=(mID/8)%2 * 128;
	int dy=mID%8 * 32;

	dx+=(Flag.mPlayTime/STATUS_ANI_TIME)%4*32;
	window->DrawTexture(&(StatusTex[mID/16]),0,dx,dy,32,32,x,y-2);


	static char name[64];
	const int namex=40;
	window->DrawText(0,mName,x+namex,y+6);
	if(mShowParam){
		char text[64];
		sprintf(text,"( %d )",param);
		window->DrawText(0,text,30,y+6,0,DT_RIGHT);
	}
}
void	CStatusData::Help(int param) const
{
	MenuData.SetMenu(StatusHelp,mID);
}
///////////////////////////////////////////////////////////////////////////////

const CComSkillData ComSkillData[MAX_CSKILL_NUMBER]={
	CComSkillData(	"�@��"				,0		,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"�a��"				,1		,CS_SWORD		|CS_MOVE		,0	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"�˂�"				,2		,CS_SPEAR		|CS_MOVE		,0	,true	,CR_ONE_M		,2,-1),
	CComSkillData(	"�@���؂�"			,3		,CS_AXE			|CS_MOVE		,0	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"�˂�"				,4		,CS_BOW			|CS_NO_MOVE		,0	,true	,CR_ONE_M		,4,-1),//���̕����ˑ�
	CComSkillData(	"���[�U�["			,5		,CS_SKILL		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,-1),
	CComSkillData(	"���s�I"			,6		,0				|CS_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	"�r�o������Ȃ��I"	,7		,0				|CS_MOVE		,0	,true	,CR_ONE			,-1),
	//�ȏ�A����p�r
	//�ȉ��A�K���p�X�L���B40�𑫂��B
		//���Z
	CComSkillData(	"�����a��"			,8		,CS_SWORD		|CS_MOVE		,0	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"�_�u���X���b�V��"	,9		,CS_SWORD		|CS_MOVE		,1	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"�u��"				,10		,CS_SWORD		|CS_QUICK_MOVE	,1	,true	,CR_ONE_M		,0,-1),//����������
	CComSkillData(	"��]�؂�"			,11		,CS_SWORD		|CS_MOVE		,1	,true	,CR_CIRCLE		,16,-1),
	CComSkillData(	"�g���C�X���X�g"	,12		,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,0,7,-1),
	CComSkillData(	"�M���̑���"		,13		,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,32,-1),//�N���e�B�J���A�b�v
	CComSkillData(	"�u���[�h�J���e�b�g",14		,CS_SWORD		|CS_MOVE		,3	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"���̕�"			,15		,CS_SWORD		|CS_NO_MOVE		,3	,true	,CR_ALL			,17,-1),//10��U��
	CComSkillData(	"�Z�u���X�^�[�Y"	,16		,CS_SWORD		|CS_MOVE		,4	,true	,CR_ONE_M		,17,-1),
	CComSkillData(	"���i�e�B�b�N�u���b�h",17	,CS_SWORD		|CS_MOVE		,4	,true	,CR_ONE_M		,66,-1),
		//���Z
	CComSkillData(	"�U���"			,18		,CS_SPEAR		|CS_MOVE		,0	,true	,CR_CIRCLE		,16,-1),
	CComSkillData(	"�Ȃ�����"			,19		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_VLINE		,17,-1),
	CComSkillData(	"��M"				,20		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_HLINE		,17,-1),
	CComSkillData(	"������"			,21		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_ONE_M		,24,-1),//�X�^���t�^
	CComSkillData(	"���n��"			,22		,CS_SPEAR		|CS_NO_MOVE		,1	,true	,CR_ONE_M		,14,-1),//���u�U��
	CComSkillData(	"��ԗ�"			,23		,CS_SPEAR		|CS_MOVE		,2	,true	,CR_BIG_CIRCLE	,16,-1),
	CComSkillData(	"�s���˂�"			,24		,CS_SPEAR		|CS_SLOW_NO_WALK,2	,true	,CR_ONE_M		,-1),//���ߍU��
	CComSkillData(	"���V���n"			,25		,CS_SPEAR		|CS_NO_MOVE		,3	,true	,CR_ALL			,28,22,-1),
	CComSkillData(	"�J�~�J�[�X�g���C�N",26		,CS_SPEAR		|CS_QUICK_NO_WALK,3	,true	,CR_ONE_M		,12,61,-1),//���U
	CComSkillData(	"��������"			,27		,CS_SPEAR		|CS_MOVE		,4	,true	,CR_ONE_M		,17,-1),
		//���Z
	CComSkillData(	"�ۑ��؂�"			,28		,CS_AXE			|CS_MOVE		,0	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"�w�r�[�A�^�b�N"	,29		,CS_AXE			|CS_MOVE		,1	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"�Ռ��g"			,30		,CS_AXE			|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,22,-1),
	CComSkillData(	"������΂�"		,31		,CS_AXE	|CS_MOVE |CS_TLOCK_HARD	,1	,true	,CR_ONE_M		,1,-1),//������΂�
	CComSkillData(	"�X�s�j���O�u���C�N",32		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"���ӂ�"			,33		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,44,-1),
	CComSkillData(	"���ӂ�"			,34		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,45,-1),
	CComSkillData(	"�X�J�C�_�C�r���O"	,35		,CS_AXE			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"���C�t�u���C�N"	,36		,CS_AXE			|CS_MOVE		,3	,true	,CR_ONE_M		,3,26,34,-1),
	CComSkillData(	"�t�@�[�}�[�N���b�V��",37	,CS_AXE			|CS_MOVE_NO_WALK,4	,true	,CR_CIRCLE		,22,49,60,-1),
		//�|�Z
	CComSkillData(	"�[�������ˌ�"		,38		,CS_BOW			|CS_MOVE		,0	,true	,CR_ONE_M		,4,-1),
	CComSkillData(	"�o�[�h�V���b�g"	,39		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,19,-1),
	CComSkillData(	"�Ŗ�"				,40		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,4,21,-1),//�ŕt�^
	CComSkillData(	"�}���`�v���V���b�g",41		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,4,-1),//�K��
	CComSkillData(	"�X�^�[�_�X�g�A���[",42		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ONE_M		,36,-1),
	CComSkillData(	"�e�D��"			,43		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ONE_M		,43,-1),//��ვt�^
	CComSkillData(	"�A���[���C��"		,44		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ALL			,4,43,-1),//���@�U��
	CComSkillData(	"�V���[�g��U����[��",45		,CS_BOW			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,4,52,-1),
	CComSkillData(	"���̓V�g"			,46		,CS_BOW			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,26,34,43,63,-1),//�����t�^
	CComSkillData(	"�\�E���u���C�N"	,47		,CS_BOW			|CS_NO_MOVE		,4	,true	,CR_ONE_M		,43,-1),//��������
		//�i���Z
	CComSkillData(	"����"				,48		,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"���Q�̈ꌂ"		,49		,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE			,-1),
	CComSkillData(	"�{�R�{�R"			,50		,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	""					,51		,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,-1),
	CComSkillData(	""					,52		,CS_SKILL		|CS_MOVE		,3	,true	,CR_ONE_M		,-1),
		//���@
	CComSkillData(	"�}�W�b�N�V���[�g"	,53		,CS_MAGIC		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,5,-1),
		//�����@
	CComSkillData(	"�t���C���{�[��"	,54		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,9,10,-1),//�t�@�C�A���C�����ˑ�
	CComSkillData(	"�V���C�j���O"		,55		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_ALL			,-1),//�L�͈͂��X�^��
	CComSkillData(	"�q�[�g�A�^�b�N"	,56		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//�U���E���U�A�b�v
	CComSkillData(	"�t�@�C�A�E�H�[��"	,57		,CS_MAGIC_FIRE	|CS_NO_MOVE		,2	,true	,CR_VLINE		,9,10,-1),
	CComSkillData(	"�o�[���u�[�X�^�["	,58		,CS_MAGIC_FIRE	|CS_QUICK		,2	,false	,CR_ALL			,10,-1),//���[�W2�{(1�^�[��)
	CComSkillData(	"�G�N�X�v���[�W����",59		,CS_MAGIC_FIRE	|CS_NO_MOVE		,3	,true	,CR_BIG_CIRCLE	,10,50,-1),
	CComSkillData(	"�t�F�j�b�N�X"		,60		,CS_MAGIC_FIRE	|CS_NO_MOVE		,3	,false	,CR_ONE			,57,-1),//�����@�C���@
	CComSkillData(	"�Z�C���g�X�^�["	,61		,CS_MAGIC_FIRE	|CS_NO_MOVE		,4	,true	,CR_CIRCLE		,10,-1),
		//�X���@
	CComSkillData(	"�A�C�X�j�[�h��"	,62		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,true	,CR_ONE_M		,11,12,-1),
	CComSkillData(	"�t���X�g�o�C�g"	,63		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,true	,CR_ONE			,12,-1),
	CComSkillData(	"�N�[���o���A"		,64		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//���h�A�b�v
	CComSkillData(	"�t���[�Y���\�[�h"	,65		,CS_MAGIC_ICE	|CS_NO_MOVE		,2	,true	,CR_HLINE		,38,-1),
	CComSkillData(	"�R�[���h�X���[�v"	,66		,CS_MAGIC_ICE	|CS_NO_MOVE		,2	,true	,CR_S_ALL		,12,-1),//����ǉ�
	CComSkillData(	"�A�C�V�N���X���b�T�[",67	,CS_MAGIC_ICE	|CS_NO_MOVE		,3	,true	,CR_ONE_M		,12,53,54,-1),
	CComSkillData(	"�q�[�����C��"		,68		,CS_MAGIC_ICE	|CS_NO_MOVE		,3	,false	,CR_ALL			,51,-1),//������(�i��)
	CComSkillData(	"�X�m�[�}�����b�V��",69		,CS_MAGIC_ICE	|CS_NO_MOVE		,4	,true	,CR_ONE_M		,12,54,72,73,-1),
		//�����@
	CComSkillData(	"�E�C���h�J�b�^�["	,70		,CS_MAGIC_WIND	|CS_NO_MOVE		,1	,true	,CR_HLINE		,13,14,-1),
	CComSkillData(	"�K�X�g�E�C���h"	,71		,CS_MAGIC_WIND	|CS_NO_MOVE		,1	,true	,CR_ALL		,30,-1),//�G�𐁂���΂�
	CComSkillData(	"�Q�C���X�s�[�h"	,72		,CS_MAGIC_WIND|CS_NO_MOVE|CS_TLOCK_VHARD,1,false,CR_CIRCLE	,18,-1),//���΂₳�A�b�v
	CComSkillData(	"�G�A�[�X���b�V��"	,73		,CS_MAGIC_WIND	|CS_NO_MOVE		,2	,true	,CR_ALL			,28,31,-1),
	CComSkillData(	"�X�J�C�E�H�[�J�["	,74		,CS_MAGIC_WIND	|CS_NO_MOVE		,2	,false	,CR_ONE			,56,-1),//�Ώۂɔ�s��t�^
	CComSkillData(	"�J�b�e�B���O�X�g�[��",75	,CS_MAGIC_WIND	|CS_NO_MOVE		,3	,true	,CR_ALL			,14,-1),
	CComSkillData(	"���s�b�h���r�b�g"	,76		,CS_MAGIC_WIND|CS_NO_MOVE|CS_TLOCK_VHARD,3,false,CR_ALL		,39,-1),//�����S���������ɍs��
	CComSkillData(	"�g���l�[�h"		,77		,CS_MAGIC_WIND	|CS_NO_MOVE		,4	,true	,CR_ALL			,30,28,-1),
		//�y���@
	CComSkillData(	"�_�[�g�����X"		,78		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,true	,CR_ONE_M		,15,-1),
	CComSkillData(	"�O���r�e�B�["		,79		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,35,-1),//�G���d������
	CComSkillData(	"�X�g�[���V�[���h"	,80		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//�h��̓A�b�v
	CComSkillData(	"���b�N�v���X"		,81		,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_ONE_M		,35,37,-1),
	CComSkillData(	"�O�����h�E�H�[��"	,82		,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_NONE		,48,49,-1),//�G�Ɩ����̊ԂɃJ�x�����
	CComSkillData(	"�A�[�X�N�G�C�N"	,83		,CS_MAGIC_EARTH	|CS_NO_MOVE		,3	,true	,CR_S_ALL		,15,49,-1),
	CComSkillData(	"���[���_�C�u"		,84		,CS_MAGIC_EARTH	|CS_QUICK		,3	,false	,CR_ALL			,49,-1),//�n�ʂɐ�����1�^�[�����߂���
	CComSkillData(	"���e�I�X�E�H�[��"	,85		,CS_MAGIC_EARTH	|CS_NO_MOVE		,4	,true	,CR_ALL			,49,50,62,-1),
		//���ꖂ�@
	CComSkillData(	"�C���p�N�g"		,86		,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,21,22,-1),
	CComSkillData(	"���C"				,87		,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"�u���b�N�z�[��"	,88		,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_BIG_CIRCLE	,58,-1),
	CComSkillData(	"���t���N�V����"	,89		,CS_MAGIC		|CS_NO_MOVE		,2	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	""					,90		,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
	CComSkillData(	"�G���V�����g���[��",91		,CS_MAGIC|CS_QUICK|CS_TLOCK_VHARD,4	,false	,CR_ALL			,26,74,-1),
		//�ȉ��A����
	CComSkillData(	"���}���u"			,92		,CS_OTHER_HEAL	|CS_MOVE		,1	,false	,CR_ONE			,6,-1),
	CComSkillData(	"�j��"				,93		,CS_OTHER_HEAL	|CS_NO_MOVE		,2	,false	,CR_BIG_CIRCLE	,26,41,-1),
	CComSkillData(	"����"				,94		,CS_OTHER_HEAL|CS_NO_MOVE|CS_TLOCK_HARD,1,false	,CR_CIRCLE	,25,26,-1),
	CComSkillData(	"�����̑���"		,95		,CS_OTHER_HEAL	|CS_RIVIVAL		,2	,false	,CR_ONE			,75,-1),
	CComSkillData(	"���̌��E"			,96		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"�X�̌��E"			,97		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"���̌��E"			,98		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"�y�̌��E"			,99		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"�͂̌��E"			,100	,CS_OTHER_FIELD	|CS_QUICK		,2	,false	,CR_NONE		,-1),
	CComSkillData(	"���̌��E"			,101	,CS_OTHER_FIELD	|CS_QUICK		,2	,false	,CR_NONE		,-1),
	CComSkillData(	"�ǂ��ǂ��K�X"		,102	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,20,21,-1),
	CComSkillData(	"�Ö��p"			,103	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,23,-1),
	CComSkillData(	"�U�f"				,104	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,26,34,-1),
	CComSkillData(	"�d�C�V���b�N"		,105	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"����"				,106	,CS_OTHER_ST	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,77,-1),
	CComSkillData(	"����m�̃c�{"		,107	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,65,-1),
	CComSkillData(	"���s�K�X"			,108	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,20,21,-1),
	CComSkillData(	""					,109	,CS_OTHER		|CS_MOVE		,2	,true	,CR_ONE			,-1),
	CComSkillData(	""					,110	,CS_OTHER		|CS_MOVE		,2	,true	,CR_ONE			,-1),
	CComSkillData(	"�����̌�"			,111	,CS_OTHER_HEAL	|CS_NO_MOVE		,3	,false	,CR_ALL			,6,26,-1),
	CComSkillData(	"���̋P��"			,112	,CS_OTHER_HEAL	|CS_RIVIVAL		,4	,false	,CR_ONE			,78,41,20,-1),
	CComSkillData(	""					,113	,CS_OTHER		|CS_MOVE		,1	,true	,CR_ONE			,-1),
	CComSkillData(	"�h��"				,114	,CS_OTHER		|CS_QUICK		,0	,false	,CR_SELF		,18,-1),
	CComSkillData(	"����"				,115	,CS_OTHER		|CS_QUICK		,1	,true	,CR_ALL			,42,-1),
	CComSkillData(	"���߂�"			,116	,CS_OTHER		|CS_NO_MOVE		,0	,false	,CR_SELF		,18,-1),
	CComSkillData(	"�N���{�["			,117	,CS_OTHER_PET	|CS_CALL_PET	,0	,true	,CR_NONE		,-1),
	CComSkillData(	"���@�ڂ���"		,118	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"��j"				,119	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"�����L�m�R"		,120	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"�{���{���{�[�C"	,121	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"�r�[�X�g�E�H�[���A",122	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"�E�b�h�}��"		,123	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"��������d��"		,124	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"�^�R�L���O"		,125	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"���C�o�[��"		,126	,CS_OTHER_PET	|CS_CALL_PET	,3	,true	,CR_NONE		,-1),
	CComSkillData(	""					,127	,CS_OTHER		|CS_NO_MOVE		,4	,true	,CR_NONE		,-1),
		//�ȉ��A�A�C�e���B
	CComSkillData(	"���C�t���J�o���["	,128	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,6,-1),
	CComSkillData(	"�`���[�W�X�L��"	,129	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,6,-1),
	CComSkillData(	"�v�����g�{���E�r"	,130	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,10,-1),
	CComSkillData(	"�v�����g�{���E�k"	,131	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"�q�[�����O"		,132	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_BIG_CIRCLE	,29,-1),
	CComSkillData(	"�N���A�����X"		,133	,CS_ITEM|CS_NO_MOVE|CS_TLOCK_HARD,0	,false	,CR_BIG_CIRCLE	,25,26,-1),
	CComSkillData(	"���o�C�o��"		,134	,CS_ITEM		|CS_RIVIVAL		,0	,false	,CR_ONE			,75,-1),
	CComSkillData(	"�C���X�^���g�o���A",135	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	"�J�E���^�[�j�[�h��",136	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,47,-1),
	CComSkillData(	"�_�u��"			,137	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,-1),
	CComSkillData(	"���t���N�V����"	,138	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"�G�X�P�[�v"		,139	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_NONE		,-1),
	CComSkillData(	"�K�[�h�u���C�N"	,140	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	""					,141	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
	CComSkillData(	""					,142	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
	CComSkillData(	""					,143	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
		//�ȉ��A�G��p�B
	CComSkillData(	"�̓�����"			,144	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,1,-1),
	CComSkillData(	"���ݕt��"			,145	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,8,-1),
	CComSkillData(	"����"				,146	,CS_OTHER		|CS_MOVE		,0	,true	,CR_BIG_CIRCLE	,10,-1),
	CComSkillData(	"�z��"				,147	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,21,-1),
	CComSkillData(	"���U���N�V����"	,148	,CS_MAGIC		|CS_RIVIVAL		,3	,false	,CR_ALL			,75,-1),
	CComSkillData(	"�C���X�^���g�o���A",149	,CS_MAGIC		|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	"�{���z��"			,150	,CS_SKILL		|CS_NO_MOVE		,1	,true	,CR_ONE_M		,33,-1),
	CComSkillData(	"�܂₩���̖��p�t"	,151	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_NONE		,-1),
	CComSkillData(	"�T�\���̓Őj"		,152	,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,2,-1),
	CComSkillData(	"����"				,153	,CS_OTHER		|CS_NO_MOVE		,1	,true	,CR_ONE			,46,-1),
	CComSkillData(	"�ˌ�"				,154	,CS_SKILL		|CS_MOVE		,2	,true	,CR_HLINE		,-1),
	CComSkillData(	"���t���N�V����"	,155	,CS_MAGIC		|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"�}�O�}�t�B�[���h"	,156	,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,10,49,59,-1),
	CComSkillData(	"�T���_�[�X�g�[��"	,157	,CS_MAGIC_WIND	|CS_NO_MOVE		,4	,true	,CR_ALL			,-1),
	CComSkillData(	"���g���X�|�A"		,158	,CS_MAGIC_EARTH	|CS_NO_MOVE		,4	,true	,CR_ALL			,64,-1),
	CComSkillData(	"�Ăяo��"			,159	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_NONE		,-1),
		//���������\���U��
	CComSkillData(	"�\���U���ELv0"		,160	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"�\���U���ELv1"		,161	,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"�\���U���ELv2"		,162	,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"�\���U���ELv3"		,163	,CS_SKILL		|CS_MOVE		,3	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"�\���U���ELv4"		,164	,CS_SKILL		|CS_MOVE		,4	,true	,CR_ONE_M		,1,-1),
		//�\���U�������
	CComSkillData(	""					,165	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	""					,166	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"�}���`�E���C"		,167	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ALL			,-1),
	CComSkillData(	"�f�X�T�[�N��"		,168	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ONE			,-1),
	CComSkillData(	"�t�H�[�X�X�t�B�A"	,169	,CS_MAGIC		|CS_SUPER_SLOW	,3	,true	,CR_NONE		,-1),
	CComSkillData(	"�t���[�g�u���C�h"	,170	,CS_SWORD		|CS_QUICK		,2	,true	,CR_ALL			,68,-1),
	CComSkillData(	""					,171	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
	CComSkillData(	"�ł̔g��"			,172	,CS_MAGIC		|CS_NO_MOVE		,4	,true	,CR_ALL			,20,-1),
	CComSkillData(	"�S������"			,173	,CS_SKILL		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,69,70,-1),
	CComSkillData(	""					,174	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ONE_M		,57,-1),
	CComSkillData(	"�q���Ă�"			,175	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_NONE		,-1),
	CComSkillData(	"�U�f�̉�"			,176	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"�����̉�"			,177	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"���_�̉�"			,178	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"�����̉�"			,179	,CS_OTHER_ST	|CS_NO_MOVE		,2	,false	,CR_ALL			,71,-1),
	CComSkillData(	"�Ή���"			,180	,CS_SWORD		|CS_MOVE		,2	,true	,CR_HLINE		,10,-1),
	CComSkillData(	"�X�ጕ"			,181	,CS_SWORD		|CS_MOVE		,2	,true	,CR_CIRCLE		,54,11,-1),
	CComSkillData(	"���w��"			,182	,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,27,28,-1),
	CComSkillData(	"�n����"			,183	,CS_SWORD		|CS_MOVE		,2	,true	,CR_ALL			,21,-1),
	CComSkillData(	""/*�^�[�Q�b�e�B���O*/	,184	,CS_OTHER		|CS_SLOW_NO_WALK,0	,true	,CR_ONE			,76,-1),
	CComSkillData(	""					,185	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	"�{���z��"			,186	,CS_SKILL		|CS_NO_MOVE		,1	,true	,CR_ALL			,33,-1),
	CComSkillData(	"����n"			,187	,CS_OTHER		|CS_NO_MOVE		,2	,true	,CR_ALL			,46,-1),
	CComSkillData(	""/*100�J�E���g�o���A*/	,188	,CS_EITEM|0x1000*20|CS_NO_MOVE	,0	,false	,CR_ONE			,55,-1),
	CComSkillData(	"�q�[�����O"		,189	,CS_EITEM|0x1000*5 |CS_NO_MOVE	,0	,false	,CR_BIG_CIRCLE	,29,-1),
	CComSkillData(	"�v�����g�{���E�k"	,190	,CS_EITEM|0x1000*14|CS_NO_MOVE	,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"�_�u��"			,191	,CS_EITEM|0x1000*22|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,-1),
	CComSkillData(	"�v�����g�{���E�r"	,192	,CS_EITEM|0x1000*12|CS_NO_MOVE	,0	,true	,CR_CIRCLE		,10,-1),
	CComSkillData(	"�J�E���^�[�j�[�h��",193	,CS_EITEM|0x1000*21|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,10,-1),
	CComSkillData(	"�X�L���h���C��Lv.1",194	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"�X�L���h���C��Lv.2",195	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"�X�L���h���C��Lv.3",196	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"�X�L���h���C��Lv.4",197	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"���[�U�[�J�m��"	,198	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ONE			,21,79,-1),
	CComSkillData(	"�A�C�e���X�e�B�[��",199	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,-1),
	CComSkillData(	"�����ȑ��̗D�����l�X",200	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"�x���Ă��ꂽ���ԒB",201	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"�ق̂��Ȉ��Ɨ��l"	,202	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"���O�̖����Ԏq"	,203	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"������"			,204	,CS_OTHER		|CS_SUPER_QUICK	,0	,false	,CR_S_ALL		,-1),
	CComSkillData(	"�v�����g�{���E�k"	,205	,CS_EITEM|0x1000*14|CS_NO_MOVE	,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"���̏I���"		,206	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"�X�L���h���C��Lv.1",207	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"�X�L���h���C��Lv.2",208	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"���̌��t"			,209	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E��"	,210	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E����"	,211	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E���"	,212	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E����"	,213	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E����"	,214	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E�\��"	,215	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�􂢂̌��t�E���s"	,216	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"�C���X�^���g�o���A",217	,CS_EITEM|0x1000*20|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	""/*���t���N�V����*/	,218	,CS_EITEM|0x1000*23|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"���o�C�o��"		,219	,CS_EITEM|0x1000*7 |CS_RIVIVAL	,0	,false	,CR_ONE			,75,-1),
	CComSkillData(	""/*�X�e�[�W�S�̔ėp*/,220	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	""/*�S�̍U���ėp*/	,221	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,-1),
	CComSkillData(	""					,222	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,80,-1),
	CComSkillData(	"�O���r�e�B�[�E�I�[��",223	,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_ALL			,35,-1),

	CComSkillData(	""/*�u�����g�E�F�|��*/	,224	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	""/*���X�g�A�[�}�[*/	,225	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"������"			,226	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ONE_M		,57,-1),
	CComSkillData(	"�f�B�X�y��"		,227	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ALL			,80,-1),
	CComSkillData(	"�T�C�����g���[���h",228	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
};

const CSkillData SkillData[MAX_SKILL_NUMBER]={
	CSkillData(		"LP�A�b�v Lv. 1"	,0		,SKILL_STATUS	,1*0x10000	+20),
	CSkillData(		"LP�A�b�v Lv. 2"	,1		,SKILL_STATUS	,1*0x10000	+30),
	CSkillData(		"LP�A�b�v Lv. 3"	,2		,SKILL_STATUS	,1*0x10000	+50),
	CSkillData(		"LP�A�b�v Lv. 4"	,3		,SKILL_STATUS	,1*0x10000	+70),
	CSkillData(		"LP�A�b�v Lv. 5"	,4		,SKILL_STATUS	,1*0x10000	+90),
	CSkillData(		"LP�A�b�v Lv. 6"	,5		,SKILL_STATUS	,1*0x10000	+120),
	CSkillData(		"LP�A�b�v Lv. 7"	,6		,SKILL_STATUS	,1*0x10000	+150),
	CSkillData(		"SP�A�b�v Lv. 1"	,7		,SKILL_STATUS	,2*0x10000	+3),
	CSkillData(		"SP�A�b�v Lv. 2"	,8		,SKILL_STATUS	,2*0x10000	+5),
	CSkillData(		"SP�A�b�v Lv. 3"	,9		,SKILL_STATUS	,2*0x10000	+7),
	CSkillData(		"SP�A�b�v Lv. 4"	,10		,SKILL_STATUS	,2*0x10000	+10),
	CSkillData(		"SP�A�b�v Lv. 5"	,11		,SKILL_STATUS	,2*0x10000	+15),
	CSkillData(		"SP�A�b�v Lv. 6"	,12		,SKILL_STATUS	,2*0x10000	+20),
	CSkillData(		"�U���̓A�b�v Lv. 1",13		,SKILL_STATUS	,4*0x10000	+10),
	CSkillData(		"�U���̓A�b�v Lv. 2",14		,SKILL_STATUS	,4*0x10000	+15),
	CSkillData(		"�U���̓A�b�v Lv. 3",15		,SKILL_STATUS	,4*0x10000	+20),
	CSkillData(		"�U���̓A�b�v Lv. 4",16		,SKILL_STATUS	,4*0x10000	+25),
	CSkillData(		"�U���̓A�b�v Lv. 5",17		,SKILL_STATUS	,4*0x10000	+30),
	CSkillData(		"�U���̓A�b�v Lv. 6",18		,SKILL_STATUS	,4*0x10000	+40),
	CSkillData(		"�U���̓A�b�v Lv. 7",19		,SKILL_STATUS	,4*0x10000	+50),
	CSkillData(		"�h��̓A�b�v Lv. 1",20		,SKILL_STATUS	,8*0x10000	+5),
	CSkillData(		"�h��̓A�b�v Lv. 2",21		,SKILL_STATUS	,8*0x10000	+10),
	CSkillData(		"�h��̓A�b�v Lv. 3",22		,SKILL_STATUS	,8*0x10000	+15),
	CSkillData(		"�h��̓A�b�v Lv. 4",23		,SKILL_STATUS	,8*0x10000	+20),
	CSkillData(		"�h��̓A�b�v Lv. 5",24		,SKILL_STATUS	,8*0x10000	+25),
	CSkillData(		"�h��̓A�b�v Lv. 6",25		,SKILL_STATUS	,8*0x10000	+30),
	CSkillData(		"�h��̓A�b�v Lv. 7",26		,SKILL_STATUS	,8*0x10000	+40),
	CSkillData(		"���@�U���A�b�v Lv. 1",27	,SKILL_STATUS	,16*0x10000	+10),
	CSkillData(		"���@�U���A�b�v Lv. 2",28	,SKILL_STATUS	,16*0x10000	+15),
	CSkillData(		"���@�U���A�b�v Lv. 3",29	,SKILL_STATUS	,16*0x10000	+20),
	CSkillData(		"���@�U���A�b�v Lv. 4",30	,SKILL_STATUS	,16*0x10000	+25),
	CSkillData(		"���@�U���A�b�v Lv. 5",31	,SKILL_STATUS	,16*0x10000	+30),
	CSkillData(		"���@�U���A�b�v Lv. 6",32	,SKILL_STATUS	,16*0x10000	+40),
	CSkillData(		"���@�U���A�b�v Lv. 7",33	,SKILL_STATUS	,16*0x10000	+50),
	CSkillData(		"���@�h��A�b�v Lv. 1",34	,SKILL_STATUS	,32*0x10000	+5),
	CSkillData(		"���@�h��A�b�v Lv. 2",35	,SKILL_STATUS	,32*0x10000	+10),
	CSkillData(		"���@�h��A�b�v Lv. 3",36	,SKILL_STATUS	,32*0x10000	+15),
	CSkillData(		"���@�h��A�b�v Lv. 4",37	,SKILL_STATUS	,32*0x10000	+20),
	CSkillData(		"���@�h��A�b�v Lv. 5",38	,SKILL_STATUS	,32*0x10000	+25),
	CSkillData(		"���@�h��A�b�v Lv. 6",39	,SKILL_STATUS	,32*0x10000	+30),
	CSkillData(		"���@�h��A�b�v Lv. 7",40	,SKILL_STATUS	,32*0x10000	+40),
	CSkillData(		"���΂₳ + 10"		,41		,SKILL_STATUS	,64*0x10000	+10),
	CSkillData(		"���΂₳ + 5"		,42		,SKILL_STATUS	,64*0x10000	+5),
	CSkillData(		"���΂₳ + 20"		,43		,SKILL_STATUS	,64*0x10000	+20),
	CSkillData(		"SP�A�b�v Lv. 7"	,44		,SKILL_STATUS	,2*0x10000	+25),
	CSkillData(		"�A�N�Z�T���[ + 1"	,45		,SKILL_STATUS	,256*0x10000+1),
	CSkillData(		"�A�C�e�� + 2"		,46		,SKILL_STATUS	,128*0x10000+2),
	CSkillData(		"�A�C�e�� + 4"		,47		,SKILL_STATUS	,128*0x10000+4),
	CSkillData(		NULL				,48		,SKILL_COMMAND	,8),
	CSkillData(		NULL				,49		,SKILL_COMMAND	,9),
	CSkillData(		NULL				,50		,SKILL_COMMAND	,10),
	CSkillData(		NULL				,51		,SKILL_COMMAND	,11),
	CSkillData(		NULL				,52		,SKILL_COMMAND	,12),
	CSkillData(		NULL				,53		,SKILL_COMMAND	,13),
	CSkillData(		NULL				,54		,SKILL_COMMAND	,14),
	CSkillData(		NULL				,55		,SKILL_COMMAND	,15),
	CSkillData(		NULL				,56		,SKILL_COMMAND	,16),
	CSkillData(		NULL				,57		,SKILL_COMMAND	,17),
	CSkillData(		NULL				,58		,SKILL_COMMAND	,18),
	CSkillData(		NULL				,59		,SKILL_COMMAND	,19),
	CSkillData(		NULL				,60		,SKILL_COMMAND	,20),
	CSkillData(		NULL				,61		,SKILL_COMMAND	,21),
	CSkillData(		NULL				,62		,SKILL_COMMAND	,22),
	CSkillData(		NULL				,63		,SKILL_COMMAND	,23),
	CSkillData(		NULL				,64		,SKILL_COMMAND	,24),
	CSkillData(		NULL				,65		,SKILL_COMMAND	,25),
	CSkillData(		NULL				,66		,SKILL_COMMAND	,26),
	CSkillData(		NULL				,67		,SKILL_COMMAND	,27),
	CSkillData(		NULL				,68		,SKILL_COMMAND	,28),
	CSkillData(		NULL				,69		,SKILL_COMMAND	,29),
	CSkillData(		NULL				,70		,SKILL_COMMAND	,30),
	CSkillData(		NULL				,71		,SKILL_COMMAND	,31),
	CSkillData(		NULL				,72		,SKILL_COMMAND	,32),
	CSkillData(		NULL				,73		,SKILL_COMMAND	,33),
	CSkillData(		NULL				,74		,SKILL_COMMAND	,34),
	CSkillData(		NULL				,75		,SKILL_COMMAND	,35),
	CSkillData(		NULL				,76		,SKILL_COMMAND	,36),
	CSkillData(		NULL				,77		,SKILL_COMMAND	,37),
	CSkillData(		NULL				,78		,SKILL_COMMAND	,38),
	CSkillData(		NULL				,79		,SKILL_COMMAND	,39),
	CSkillData(		NULL				,80		,SKILL_COMMAND	,40),
	CSkillData(		NULL				,81		,SKILL_COMMAND	,41),
	CSkillData(		NULL				,82		,SKILL_COMMAND	,42),
	CSkillData(		NULL				,83		,SKILL_COMMAND	,43),
	CSkillData(		NULL				,84		,SKILL_COMMAND	,44),
	CSkillData(		NULL				,85		,SKILL_COMMAND	,45),
	CSkillData(		NULL				,86		,SKILL_COMMAND	,46),
	CSkillData(		NULL				,87		,SKILL_COMMAND	,47),
	CSkillData(		NULL				,88		,SKILL_COMMAND	,48),
	CSkillData(		NULL				,89		,SKILL_COMMAND	,49),
	CSkillData(		NULL				,90		,SKILL_COMMAND	,50),
	CSkillData(		NULL				,91		,SKILL_COMMAND	,51),
	CSkillData(		NULL				,92		,SKILL_COMMAND	,52),
	CSkillData(		NULL				,93		,SKILL_COMMAND	,53),
	CSkillData(		NULL				,94		,SKILL_COMMAND	,54),
	CSkillData(		NULL				,95		,SKILL_COMMAND	,55),
	CSkillData(		NULL				,96		,SKILL_COMMAND	,56),
	CSkillData(		NULL				,97		,SKILL_COMMAND	,57),
	CSkillData(		NULL				,98		,SKILL_COMMAND	,58),
	CSkillData(		NULL				,99		,SKILL_COMMAND	,59),
	CSkillData(		NULL				,100	,SKILL_COMMAND	,60),
	CSkillData(		NULL				,101	,SKILL_COMMAND	,61),
	CSkillData(		NULL				,102	,SKILL_COMMAND	,62),
	CSkillData(		NULL				,103	,SKILL_COMMAND	,63),
	CSkillData(		NULL				,104	,SKILL_COMMAND	,64),
	CSkillData(		NULL				,105	,SKILL_COMMAND	,65),
	CSkillData(		NULL				,106	,SKILL_COMMAND	,66),
	CSkillData(		NULL				,107	,SKILL_COMMAND	,67),
	CSkillData(		NULL				,108	,SKILL_COMMAND	,68),
	CSkillData(		NULL				,109	,SKILL_COMMAND	,69),
	CSkillData(		NULL				,110	,SKILL_COMMAND	,70),
	CSkillData(		NULL				,111	,SKILL_COMMAND	,71),
	CSkillData(		NULL				,112	,SKILL_COMMAND	,72),
	CSkillData(		NULL				,113	,SKILL_COMMAND	,73),
	CSkillData(		NULL				,114	,SKILL_COMMAND	,74),
	CSkillData(		NULL				,115	,SKILL_COMMAND	,75),
	CSkillData(		NULL				,116	,SKILL_COMMAND	,76),
	CSkillData(		NULL				,117	,SKILL_COMMAND	,77),
	CSkillData(		NULL				,118	,SKILL_COMMAND	,78),
	CSkillData(		NULL				,119	,SKILL_COMMAND	,79),
	CSkillData(		NULL				,120	,SKILL_COMMAND	,80),
	CSkillData(		NULL				,121	,SKILL_COMMAND	,81),
	CSkillData(		NULL				,122	,SKILL_COMMAND	,82),
	CSkillData(		NULL				,123	,SKILL_COMMAND	,83),
	CSkillData(		NULL				,124	,SKILL_COMMAND	,84),
	CSkillData(		NULL				,125	,SKILL_COMMAND	,85),
	CSkillData(		NULL				,126	,SKILL_COMMAND	,86),
	CSkillData(		NULL				,127	,SKILL_COMMAND	,87),
	CSkillData(		NULL				,128	,SKILL_COMMAND	,88),
	CSkillData(		NULL				,129	,SKILL_COMMAND	,89),
	CSkillData(		NULL				,130	,SKILL_COMMAND	,90),
	CSkillData(		NULL				,131	,SKILL_COMMAND	,91),
	CSkillData(		NULL				,132	,SKILL_COMMAND	,92),
	CSkillData(		NULL				,133	,SKILL_COMMAND	,93),
	CSkillData(		NULL				,134	,SKILL_COMMAND	,94),
	CSkillData(		NULL				,135	,SKILL_COMMAND	,95),
	CSkillData(		NULL				,136	,SKILL_COMMAND	,96),
	CSkillData(		NULL				,137	,SKILL_COMMAND	,97),
	CSkillData(		NULL				,138	,SKILL_COMMAND	,98),
	CSkillData(		NULL				,139	,SKILL_COMMAND	,99),
	CSkillData(		NULL				,140	,SKILL_COMMAND	,100),
	CSkillData(		NULL				,141	,SKILL_COMMAND	,101),
	CSkillData(		NULL				,142	,SKILL_COMMAND	,102),
	CSkillData(		NULL				,143	,SKILL_COMMAND	,103),
	CSkillData(		NULL				,144	,SKILL_COMMAND	,104),
	CSkillData(		NULL				,145	,SKILL_COMMAND	,105),
	CSkillData(		NULL				,146	,SKILL_COMMAND	,106),
	CSkillData(		NULL				,147	,SKILL_COMMAND	,107),
	CSkillData(		NULL				,148	,SKILL_COMMAND	,108),
	CSkillData(		NULL				,149	,SKILL_COMMAND	,109),
	CSkillData(		NULL				,150	,SKILL_COMMAND	,110),
	CSkillData(		NULL				,151	,SKILL_COMMAND	,111),
	CSkillData(		NULL				,152	,SKILL_COMMAND	,112),
	CSkillData(		NULL				,153	,SKILL_COMMAND	,113),
	CSkillData(		NULL				,154	,SKILL_COMMAND	,114),
	CSkillData(		NULL				,155	,SKILL_COMMAND	,115),
	CSkillData(		NULL				,156	,SKILL_COMMAND	,116),
	CSkillData(		NULL				,157	,SKILL_COMMAND	,117),
	CSkillData(		NULL				,158	,SKILL_COMMAND	,118),
	CSkillData(		NULL				,159	,SKILL_COMMAND	,119),
	CSkillData(		NULL				,160	,SKILL_COMMAND	,120),
	CSkillData(		NULL				,161	,SKILL_COMMAND	,121),
	CSkillData(		NULL				,162	,SKILL_COMMAND	,122),
	CSkillData(		NULL				,163	,SKILL_COMMAND	,123),
	CSkillData(		NULL				,164	,SKILL_COMMAND	,124),
	CSkillData(		NULL				,165	,SKILL_COMMAND	,125),
	CSkillData(		NULL				,166	,SKILL_COMMAND	,126),
	CSkillData(		NULL				,167	,SKILL_COMMAND	,127),
	CSkillData(		"LP +15��"			,168	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +15),
	CSkillData(		"LP +25��"			,169	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +25),
	CSkillData(		"LP +30��"			,170	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +30),
	CSkillData(		"LP +50��"			,171	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +50),
	CSkillData(		"�U���� +15��"		,172	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +15),
	CSkillData(		"�U���� +25��"		,173	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +25),
	CSkillData(		"�U���� +30��"		,174	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +30),
	CSkillData(		"�U���� +50��"		,175	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +50),
	CSkillData(		"�h��� +15��"		,176	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +15),
	CSkillData(		"�h��� +25��"		,177	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +25),
	CSkillData(		"�h��� +30��"		,178	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +30),
	CSkillData(		"�h��� +50��"		,179	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +50),
	CSkillData(		"���@�U�� +15��"	,180	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +15),
	CSkillData(		"���@�U�� +25��"	,181	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +25),
	CSkillData(		"���@�U�� +30��"	,182	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +30),
	CSkillData(		"���@�U�� +50��"	,183	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +50),
	CSkillData(		"���@�h�� +15��"	,184	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +15),
	CSkillData(		"���@�h�� +25��"	,185	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +25),
	CSkillData(		"���@�h�� +30��"	,186	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +30),
	CSkillData(		"���@�h�� +50��"	,187	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +50),
	CSkillData(		""/*���΂₳ +15��*/	,188	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +15),
	CSkillData(		"���΂₳ +25��"	,189	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +25),
	CSkillData(		"���΂₳ +30��"	,190	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +30),
	CSkillData(		"���΂₳ +50��"	,191	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +50),
	CSkillData(		""/*SP +15��*/			,192	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +15),
	CSkillData(		"SP +25��"			,193	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +25),
	CSkillData(		"SP +30��"			,194	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +30),
	CSkillData(		"SP +50��"			,195	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +50),
	CSkillData(		"���_���[�W -50��"	,196	,SKILL_AUTO		,AS_FIRE_GUARD	+50),
	CSkillData(		"���_���[�W -90��"	,197	,SKILL_AUTO		,AS_FIRE_GUARD	+90),
	CSkillData(		"���_���[�W +50��"	,198	,SKILL_AUTO		,AS_FIRE_GUARD	+0xffff-50),
	CSkillData(		"���_���[�W +100��"	,199	,SKILL_AUTO		,AS_FIRE_GUARD	+0xffff-100),
	CSkillData(		"�X�_���[�W -50��"	,200	,SKILL_AUTO		,AS_ICE_GUARD	+50),
	CSkillData(		"�X�_���[�W -90��"	,201	,SKILL_AUTO		,AS_ICE_GUARD	+90),
	CSkillData(		"�X�_���[�W +50��"	,202	,SKILL_AUTO		,AS_ICE_GUARD	+0xffff-50),
	CSkillData(		"�X�_���[�W +100��"	,203	,SKILL_AUTO		,AS_ICE_GUARD	+0xffff-100),
	CSkillData(		"���_���[�W -50��"	,204	,SKILL_AUTO		,AS_WIND_GUARD	+50),
	CSkillData(		"���_���[�W -90��"	,205	,SKILL_AUTO		,AS_WIND_GUARD	+90),
	CSkillData(		"���_���[�W +50��"	,206	,SKILL_AUTO		,AS_WIND_GUARD	+0xffff-50),
	CSkillData(		"���_���[�W +100��"	,207	,SKILL_AUTO		,AS_WIND_GUARD	+0xffff-100),
	CSkillData(		"�y�_���[�W -50��"	,208	,SKILL_AUTO		,AS_EARTH_GUARD	+50),
	CSkillData(		"�y�_���[�W -90��"	,209	,SKILL_AUTO		,AS_EARTH_GUARD	+90),
	CSkillData(		"�y�_���[�W +50��"	,210	,SKILL_AUTO		,AS_EARTH_GUARD	+0xffff-50),
	CSkillData(		"�y�_���[�W +100��"	,211	,SKILL_AUTO		,AS_EARTH_GUARD	+0xffff-100),
	CSkillData(		"ST�h��E��"		,212	,SKILL_AUTO		,AS_STATUS_GUARD	+1),
	CSkillData(		"ST�h��E����"		,213	,SKILL_AUTO		,AS_STATUS_GUARD	+2),
	CSkillData(		"ST�h��E����"		,214	,SKILL_AUTO		,AS_STATUS_GUARD	+4),
	CSkillData(		"ST�h��E���"		,215	,SKILL_AUTO		,AS_STATUS_GUARD	+8),
	CSkillData(		"ST�h��E����"		,216	,SKILL_AUTO		,AS_STATUS_GUARD	+16),
	CSkillData(		"ST�h��E�\��"		,217	,SKILL_AUTO		,AS_STATUS_GUARD	+32),
	CSkillData(		"ST�h��E���s"		,218	,SKILL_AUTO		,AS_STATUS_GUARD	+64),
	CSkillData(		"ST�h��E����"		,219	,SKILL_AUTO		,AS_STATUS_GUARD	+128),
	CSkillData(		"�ǉ����ʁE��"		,220	,SKILL_AUTO		,AS_ADD_STATUS	+1),
	CSkillData(		"�ǉ����ʁE����"	,221	,SKILL_AUTO		,AS_ADD_STATUS	+2),
	CSkillData(		"�ǉ����ʁE����"	,222	,SKILL_AUTO		,AS_ADD_STATUS	+4),
	CSkillData(		"�ǉ����ʁE���"	,223	,SKILL_AUTO		,AS_ADD_STATUS	+8),
	CSkillData(		"�ǉ����ʁE����"	,224	,SKILL_AUTO		,AS_ADD_STATUS	+16),
	CSkillData(		"�ǉ����ʁE�\��"	,225	,SKILL_AUTO		,AS_ADD_STATUS	+32),
	CSkillData(		"�ǉ����ʁE���s"	,226	,SKILL_AUTO		,AS_ADD_STATUS	+64),
	CSkillData(		"�ǉ����ʁE����"	,227	,SKILL_AUTO		,AS_ADD_STATUS	+128),
	CSkillData(		"�N���e�B�J�� +5��"	,228	,SKILL_AUTO		,AS_CRT_UP		+5),
	CSkillData(		"�N���e�B�J�� +10��",229	,SKILL_AUTO		,AS_CRT_UP		+10),
	CSkillData(		"�N���e�B�J�� +15��",230	,SKILL_AUTO		,AS_CRT_UP		+15),
	CSkillData(		"��� +10��"		,231	,SKILL_AUTO		,AS_AVOID_UP	+10),
	CSkillData(		"��� +15��"		,232	,SKILL_AUTO		,AS_AVOID_UP	+15),
	CSkillData(		"��� +20��"		,233	,SKILL_AUTO		,AS_AVOID_UP	+20),
	CSkillData(		"�l�����z +30��"	,234	,SKILL_AUTO		,AS_MONEY_UP	+30),
	CSkillData(		"�l�����z +50��"	,235	,SKILL_AUTO		,AS_MONEY_UP	+50),
	CSkillData(		"�l�����z +70��"	,236	,SKILL_AUTO		,AS_MONEY_UP	+70),
	CSkillData(		"�l�����z +100��"	,237	,SKILL_AUTO		,AS_MONEY_UP	+100),
	CSkillData(		"�l���o���l +50��"	,238	,SKILL_AUTO		,AS_EXP_UP		+50),
	CSkillData(		"�l���o���l +100��"	,239	,SKILL_AUTO		,AS_EXP_UP		+100),
	CSkillData(		"�l���o���l +150��"	,240	,SKILL_AUTO		,AS_EXP_UP		+150),
	CSkillData(		"�l���o���l +200��"	,241	,SKILL_AUTO		,AS_EXP_UP		+200),
	CSkillData(		"��s"				,242	,SKILL_AUTO		,AS_MOVE+1),
	CSkillData(		""/*�e���|�[�g*/		,243	,SKILL_AUTO		,AS_MOVE+1),
	CSkillData(		"�h��͔���"		,244	,SKILL_AUTO		,AS_DEF_BREAK		+50),
	CSkillData(		"�h��͖���"		,245	,SKILL_AUTO		,AS_DEF_BREAK		+100),
	CSkillData(		"���@�h�䔼��"		,246	,SKILL_AUTO		,AS_MDEF_BREAK		+50),
	CSkillData(		"���@�h�䖳��"		,247	,SKILL_AUTO		,AS_MDEF_BREAK		+100),
	CSkillData(		""/*�����h�䔼��*/		,248	,SKILL_AUTO		,AS_ELEMENT_BREAK	+50),	//�X�L���ԍ����g���Ă�̂Œ��ӁI
	CSkillData(		"�����h�䖳��"		,249	,SKILL_AUTO		,AS_ELEMENT_BREAK	+100),	//�X�L���ԍ����g���Ă�̂Œ��ӁI
	CSkillData(		"��������"			,250	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"HP������"		,251	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�t���[�X�y�[�X"	,252	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"�����̉���"		,253	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*�����̉���*/		,254	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�_���[�W�u�[�X�^�[",255	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"�����t�^�E��"		,256	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_FIRE),
	CSkillData(		"�����t�^�E�X"		,257	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_ICE),
	CSkillData(		"�����t�^�E��"		,258	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_WIND),
	CSkillData(		"�����t�^�E�y"		,259	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_EARTH),
	CSkillData(		""/*HP�񕜗� +50��*/	,260	,SKILL_AUTO		,AS_HP_HEAL_UP	+50),
	CSkillData(		"�����̋F��"		,261	,SKILL_AUTO		,AS_HP_HEAL_UP	+50),
	CSkillData(		" --- "/*�K���s�\*/	,262	,SKILL_NONE		,AS_OTHER),
	CSkillData(		""/*LP�_���[�W -20��*/	,263	,SKILL_AUTO		,AS_LP_GUARD	+20),
	CSkillData(		""/*LP�_���[�W -40��*/	,264	,SKILL_AUTO		,AS_LP_GUARD	+40),
	CSkillData(		""/*LP�_���[�W -60��*/	,265	,SKILL_AUTO		,AS_LP_GUARD	+60),
	CSkillData(		""/*LP�_���[�W -80��*/	,266	,SKILL_AUTO		,AS_LP_GUARD	+80),
	CSkillData(		"�|�̎��"			,267	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"��ʓV"			,268	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		""/*�����˂�*/			,269	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		""					,270	,SKILL_FIELD	,95),
	CSkillData(		"�A���w"			,271	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"�E�e"				,272	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�Z�J���h�X�y�[�X"	,273	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"��̖�"			,274	,SKILL_FIELD	,10),
	CSkillData(		"������"			,275	,SKILL_FIELD	,20),
	CSkillData(		"�n�C�W�����v"		,276	,SKILL_FIELD	,30),
	CSkillData(		"�X�[�p�[�_�b�V��"	,277	,SKILL_FIELD	,40),
	CSkillData(		"�J�M�J���E��-��"	,278	,SKILL_FIELD	,50),
	CSkillData(		"�J�M�J���E��-��-��",279	,SKILL_FIELD	,60),
	CSkillData(		"�J�M�J���E��-��"	,280	,SKILL_FIELD	,70),
	CSkillData(		"�󒆕��V"			,281	,SKILL_FIELD	,35),
	CSkillData(		"�T���҂̒���"		,282	,SKILL_FIELD	,90),
	CSkillData(		"�g���b�v����"		,283	,SKILL_FIELD	,100),
	CSkillData(		"�E�ё�"			,284	,SKILL_FIELD	,25),
	CSkillData(		"���蔲��"			,285	,SKILL_FIELD	,15),
	CSkillData(		"�댯�\�m"			,286	,SKILL_FIELD	,120),
	CSkillData(		"���܂�"			,287	,SKILL_FIELD	,130),
	CSkillData(		"����"				,288	,SKILL_FIELD	,140),
	CSkillData(		"�񓁗�"			,289	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"��������"			,290	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�����w"			,291	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"���~��"			,292	,SKILL_AUTO		,AS_ADD_STATUS_OT),
	CSkillData(		"�_�b�V���s�\"		,293	,SKILL_FIELD	,45),
	CSkillData(		""		,298	,SKILL_FIELD	,170),
	CSkillData(		""		,299	,SKILL_FIELD	,180),
	CSkillData(		"�u���b�N����"		,296	,SKILL_FIELD	,150),
	CSkillData(		"�u���b�N���� �{2"	,297	,SKILL_FIELD	,160),
	CSkillData(		"���_���[�W -40��"	,298	,SKILL_AUTO		,AS_FIRE_GUARD	+40),
	CSkillData(		"�X�_���[�W -40��"	,299	,SKILL_AUTO		,AS_ICE_GUARD	+40),
	CSkillData(		"���_���[�W -40��"	,300	,SKILL_AUTO		,AS_WIND_GUARD	+40),
	CSkillData(		"�y�_���[�W -40��"	,301	,SKILL_AUTO		,AS_EARTH_GUARD	+40),
	CSkillData(		"�؂���"			,302	,SKILL_FIELD	,190),
	CSkillData(		"�U���͌Œ�"		,303	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�_�u���q�b�g"		,304	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"���̕s��"			,305	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�܂₩��"			,306	,SKILL_AUTO		,AS_OPTION+1),
	CSkillData(		"�_���[�W���f���[�T�[",307	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"���C�t���J�o���["	,308	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�I�[�����E���h"	,309	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*���p�t�̐S��*/		,310	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*��m�̐S��*/		,311	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		"�X�L���}�X�^�["	,312	,SKILL_AUTO		,AS_FS_OTHER),
	CSkillData(		"�挩�̖�"			,313	,SKILL_FIELD	,990),
	CSkillData(		"�y�Y�b"			,314	,SKILL_AUTO		,AS_EXP_UP_OTHER),
	CSkillData(		"���S�߂�"			,315	,SKILL_FIELD	,1000),
	CSkillData(		"���C��"			,316	,SKILL_AUTO		,AS_ST_OTHER),
	CSkillData(		"���ˁ{1"			,317	,SKILL_FIELD	,1010),
	CSkillData(		"���ˁ{2"			,318	,SKILL_FIELD	,1020),
	CSkillData(		"�y�n���E�t"		,319	,SKILL_FIELD	,980),
	CSkillData(		"�����͔|"			,320	,SKILL_FIELD	,1030),
	CSkillData(		""				,321	,SKILL_AUTO		,AS_BANSEI+1),
	CSkillData(		""				,322	,SKILL_AUTO		,AS_BANSEI+2),
	CSkillData(		""				,323	,SKILL_AUTO		,AS_BANSEI+3),
	CSkillData(		""				,324	,SKILL_AUTO		,AS_BANSEI+4),
	CSkillData(		""				,325	,SKILL_AUTO		,AS_BANSEI+5),
	CSkillData(		""				,326	,SKILL_AUTO		,AS_BANSEI+6),
	CSkillData(		""				,327	,SKILL_AUTO		,AS_BANSEI+7),
	CSkillData(		""/*���@�U���Œ�*/		,328	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�ǉ����ʁE�U����"	,329	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_ATK),
	CSkillData(		"�ǉ����ʁE�h�䁫"	,330	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_DEF),
	CSkillData(		"�ǉ����ʁE���U��"	,331	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_MATK),
	CSkillData(		"�ǉ����ʁE���h��"	,332	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_MDEF),
	CSkillData(		"�ǉ����ʁE������"	,333	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_SPD),
	CSkillData(		"�T�{�e���̐j"		,334	,SKILL_AUTO		,AS_COUNTER + AT_ELE_NONE),
	CSkillData(		""/*�h����*/			,335	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�p�����[�^�ቺ�h��",336	,SKILL_AUTO		,AS_SPECIAL_GUARD),//�p�����[�^��������Ȃ��B
	CSkillData(		"�΋�U��"			,337	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"�y�n���E��"		,338	,SKILL_FIELD	,981),
	CSkillData(		"�y�n���E�H"		,339	,SKILL_FIELD	,982),
	CSkillData(		"�y�n���E�~"		,340	,SKILL_FIELD	,983),
	CSkillData(		"����ړ�"			,341	,SKILL_FIELD	,33),
	CSkillData(		"�X�^�[�g�_�b�V��"	,342	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"�S��"				,343	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"���_�̏p"			,344	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"����̏p"			,345	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"�������ʃA�b�v"	,346	,SKILL_AUTO		,AS_ELE_POW_UP +100),
	CSkillData(		"�΋�~"			,347	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�}�W�b�N�N���e�B�J��",348	,SKILL_AUTO		,AS_CRT_OTHER),
	CSkillData(		"�G�������g�`�F���W",349	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"�}�W�b�N���t���N�g",350	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�n���ڂ�"			,351	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�N���e�B�J�� +25��",352	,SKILL_AUTO		,AS_CRT_UP		+25),
	CSkillData(		""/*�M�����u���[*/		,353	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�Z���@���]"		,354	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�������T��"		,355	,SKILL_FIELD	,985),
	CSkillData(		"���Z�̐S��"		,356	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_SWORD),
	CSkillData(		"���Z�̐S��"		,357	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_SPEAR),
	CSkillData(		"���Z�̐S��"		,358	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_AXE),
	CSkillData(		"���@�U�� -50��"	,359	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 -50),
	CSkillData(		"�s��p"			,360	,SKILL_FIELD	,102),
	CSkillData(		"�N���e�B�J���h��"	,361	,SKILL_AUTO		,AS_SPECIAL_GUARD),
	CSkillData(		"�L�����Z���h��"	,362	,SKILL_AUTO		,AS_SPECIAL_GUARD),
	CSkillData(		"�΋��U��"			,363	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"�N���e�B�J�� +3��"	,364	,SKILL_AUTO		,AS_CRT_UP		+3),
	CSkillData(		"�A���f�b�h"		,365	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"��� +3��"			,366	,SKILL_AUTO		,AS_AVOID_UP	+3),
	CSkillData(		"��� +5��"			,367	,SKILL_AUTO		,AS_AVOID_UP	+5),
	CSkillData(		"�Εs���U��"		,368	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"�d����"			,369	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ST�ُ��� 30��"	,370	,SKILL_AUTO		,AS_ANTI_STATUS	+30),
	CSkillData(		"ST�ُ��� 50��"	,371	,SKILL_AUTO		,AS_ANTI_STATUS	+50),
	CSkillData(		"ST�ُ��� 80��"	,372	,SKILL_AUTO		,AS_ANTI_STATUS	+80),
	CSkillData(		"ST�ُ��� 100��"	,373	,SKILL_AUTO		,AS_ANTI_STATUS	+100),
	CSkillData(		"�A�C�e������"		,374	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		"ST�ُ��� 20��"	,375	,SKILL_AUTO		,AS_ANTI_STATUS	+20),
	CSkillData(		"ST�ُ��� 40��"	,376	,SKILL_AUTO		,AS_ANTI_STATUS	+40),
	CSkillData(		"�N���e�B�J�� +100��",377	,SKILL_AUTO		,AS_CRT_UP		+100),
	CSkillData(		"��p�t�̔�p"		,378	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"���s��"			,379	,SKILL_AUTO		,AS_MULTI_ACTION+2),
	CSkillData(		"�O��s��"			,380	,SKILL_AUTO		,AS_MULTI_ACTION+3),
	CSkillData(		"�l��s��"			,381	,SKILL_AUTO		,AS_MULTI_ACTION+4),
	CSkillData(		"���؂�"			,382	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"���w����"			,383	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�������_���[�W -50��",384	,SKILL_AUTO		,AS_NO_ELE_GUARD	+50),
	CSkillData(		"���O"				,385	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		""/*�U�f�̉�*/		,386	,SKILL_COMMAND	,176),
	CSkillData(		""/*���̌�*/		,387	,SKILL_COMMAND	,180),
	CSkillData(		""/*�X�̌�*/		,388	,SKILL_COMMAND	,181),
	CSkillData(		""/*���̌�*/		,389	,SKILL_COMMAND	,182),
	CSkillData(		""/*�y�̌�*/		,390	,SKILL_COMMAND	,183),
	CSkillData(		"�K���X�̑�"		,391	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�r�o����O"		,392	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"����p�t�̔�V"	,393	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"LP -50��"			,394	,SKILL_AUTO		,AS_LP_UP	+ 0x100 -50),
	CSkillData(		"�ݑ�"				,395	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		"���_���[�W�z��"	,396	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_FIRE),
	CSkillData(		"�X�_���[�W�z��"	,397	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_ICE),
	CSkillData(		"���_���[�W�z��"	,398	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_WIND),
	CSkillData(		"�y�_���[�W�z��"	,399	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_EARTH),
	CSkillData(		"ST�h�䖳��"		,400	,SKILL_AUTO		,AS_ST_GUARD_BREAK),
	CSkillData(		"�T�[�h�X�y�[�X"	,401	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"�S�̉�"			,402	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�ЂƂ�ڂ����̖�"	,403	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"����"				,404	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"�܂Ƃ�"			,405	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"�тÂ��낢"		,406	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"���U��"			,407	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"�N�C�b�N�Ăяo��"	,408	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		""/*�T���_�[�X�g*/	,409	,SKILL_COMMAND	,157),
	CSkillData(		""/*���g���X�|�A*/	,410	,SKILL_COMMAND	,158),
	CSkillData(		""/*�X�L���h���P*/	,411	,SKILL_COMMAND	,207),
	CSkillData(		""/*�X�L���h���Q*/	,412	,SKILL_COMMAND	,208),
	CSkillData(		""/*�{���z���`*/	,413	,SKILL_COMMAND	,150),
	CSkillData(		""/*�{���z���a*/	,414	,SKILL_COMMAND	,186),
	CSkillData(		""/*����*/			,415	,SKILL_COMMAND	,153),
	CSkillData(		""/*���ꂷ��*/		,416	,SKILL_COMMAND	,187),
	CSkillData(		""/*����*/			,417	,SKILL_COMMAND	,146),
	CSkillData(		""/*�̓�����*/		,418	,SKILL_COMMAND	,144),
	CSkillData(		"��z��"			,419	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ST�ُ��� 99��"	,420	,SKILL_AUTO		,AS_ANTI_STATUS	+99),
	CSkillData(		"���M��"			,421	,SKILL_AUTO		,AS_COUNTER + AT_ELE_FIRE),
	CSkillData(		"��� +100��"		,422	,SKILL_AUTO		,AS_AVOID_UP	+100),
	CSkillData(		"�S�[���f���p���["	,423	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"�X���̐j"			,424	,SKILL_AUTO		,AS_COUNTER + AT_ELE_ICE),
	CSkillData(		"���l�̚���"		,425	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"�p�[�}�l���X"		,426	,SKILL_AUTO		,AS_ST_OTHER),
	CSkillData(		""/*����*/			,427	,SKILL_COMMAND	,48),
	CSkillData(		""/*���Q�̈ꌂ*/	,428	,SKILL_COMMAND	,49),
	CSkillData(		""/*�{�R�{�R*/		,429	,SKILL_COMMAND	,50),
	CSkillData(		""/*������*/		,430	,SKILL_COMMAND	,226),
	CSkillData(		""/*�f�B�X�y��*/	,431	,SKILL_COMMAND	,227),
	CSkillData(		""/*�r���[���h*/	,432	,SKILL_COMMAND	,228),
};
const CStatusData	StatusData[MAX_STATUS_NUMBER]=
{
	CStatusData("��"			,0,	"POISON"	,true	,false	,255	,0xff40b040,SE_POISON),
	CStatusData("����"			,1,	"SLEEP"		,true	,false	,4		,0xffb0b0ff,SE_SLEEP),
	CStatusData("����"			,2,	"CHARM"		,true	,false	,4		,0xffff8080,SE_CHARM),
	CStatusData("���"			,3,	"PARALYSE"	,true	,false	,8		,0xffffff00,SE_PARALYSE),
	CStatusData("����"			,4,	"SEAL"		,true	,false	,8		,0xff4040ff,SE_SEAL),
	CStatusData("�\��"			,5,	"BERSERK"	,true	,false	,8		,0xffff0000,SE_BERSERK),
	CStatusData("���s"			,6,	"ROT"		,true	,false	,255	,0xffff00ff,SE_ROT),
	CStatusData("����"			,7,	"DEATH"		,true	,false	,255	,0xff808080,-1),
	CStatusData("���߂�"		,8,	"CHARGE!"	,false	,false	,2		,0xffff8000,SE_PARAM_UP),
	CStatusData("�d��"			,9,	"GRAVITY"	,false	,true	,3		,0xff8080b0,SE_GRAVITY),
	CStatusData("�u�[�X�^�["	,10,"BOOST!"	,false	,false	,1		,0xffff8000,SE_FIRE),
	CStatusData("���s�b�h"		,11,"RAPID!"	,false	,false	,1		,0xff00ff00,SE_PARAM_UP),
	CStatusData("�I�[�g�q�[��"	,12,"HEAL!"		,false	,true	,5		,0xff0080ff,SE_HEAL),
	CStatusData("�X�J�C�E�H�[�N",13,"SKYWALK!"	,false	,false	,255	,0xff00ff00,SE_HEAL),
	CStatusData("���[��"		,14,"DIVE!"		,false	,false	,100	,0xffff8000,SE_DIVE),
	CStatusData("�h��"			,15,"GUARD!"	,false	,false	,1		,0xffff8000,SE_PARAM_UP),
	CStatusData("�o���A"		,16,"BARRIER!"	,false	,true	,1		,0xff80ff80,SE_MAGIC),
	CStatusData("�T�{�e���̐j"	,17,"NEEDLE!"	,false	,true	,255	,0xffffb000,SE_HARD_SLASH),
	CStatusData("����"			,18,"INK"		,true	,false	,255	,0xff808080,SE_INK),
	CStatusData("���g"			,19,"DOUBLE!"	,false	,true	,1		,0xffffffff,SE_MAGIC),
	CStatusData("����"			,20,"REFLECT!"	,false	,true	,1		,0xff00ffff,SE_MAGIC),
	CStatusData("����"			,21,"!!!"		,true	,false	,1		,0xffff0000,-1),
	CStatusData("�t�F�j�b�N�X"	,22,"PHOENIX!"	,false	,false	,1		,0xffffff40,SE_HEAL),
	CStatusData("�s��"			,23,"UNDEAD..."	,false	,false	,1		,0xff202020,SE_ROT),
	CStatusData("�K�E"			,24,"CRITICAL!"	,false	,false	,1		,0xffffff00,SE_PARAM_UP),
	CStatusData(""				,25,"TARGET!"	,false	,false	,2		,0xffff0000,-1),
	CStatusData("�C��"			,26,"STUN!"		,true	,false	,1		,0xffff8000,SE_HIT),
};