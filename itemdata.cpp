#include "itemdata.h"
#include "stdarg.h"
#include "gamewindow.h"
#include "skilldata.h"
#include "menudata.h"


const CItemData*		AllItem[MAX_ITEM_NUMBER+1];
const CItemData 		NoneItemData("  �D�D�D ",NO_ITEM,-1,0x7fffffff,0);
CDTTexture				ItemTex[ITEM_TEX_NUMBER];
const int				NO_ICON		= ITEM_TEX_NUMBER*64-1;

bool CarryItem :: Spend()
{
	param--;
	if(param<=0){
		param=0;
		if(Flag.mDropUsedItem){
			Flag.mUsedFarm[item-ID_UI]++;
			item=NO_ITEM;
		}
		return false;
	}
	return true;
};

//////////////////////////////////////////////////////////////////////////////////////////////////

CItemData::CItemData(void)
{
//	mName="- - -";
	mID=0;
	mIcon=NO_ICON;
}
CItemData::CItemData(const char* name,int id,int icon,int price,int type)
{
	mName=name;
	mID=id;
	mIcon=icon;
	mPrice=price;
	mType=type;
}

void CItemData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
//param&0x1			������
//param&0x2			���j���[�ł̃A�C�e���g�p����
//param&0x4			�퓬���̃A�C�e���g�p����
//param&0x8			���i
//param&0x10		�_��
//param&0x20		�q��
//param&0x40		���p
//???
//param&0x100		��񕐊�
//param&0x200		�V���i
//param&0x400		�A�C�e���̎g�p�񐔂�\������
//param&0x800		�ⓚ���p�ňÂ������ɂ���
//param&0x00ff0000	�A�C�e���̎g�p�񐔂Ȃ�
//param&0xff000000	�_��̍݌�
	y-=4;

	if(param&1){
		D3DCOLOR bgcolor=0x00ffffff;
		D3DCOLOR txtcolor=0x00ffffff;
		const char *text;
		if((mType&0xffff0000) == TYPE_WEAPON){
			bgcolor=0x40ff80ff;
			txtcolor=0xffb00000;
			text="MainWeapon";
		}else if((mType&0xffff0000) == TYPE_ARMOR){
			bgcolor=0x40b0b0ff;
			txtcolor=0xff0000b0;
			text="Armor";
		}else if((mType&0xffff0000) == TYPE_ACCESSORY){
			bgcolor=0x4080ffff;
			txtcolor=0xff00b000;
			switch(mType){
				case TYPE_SHIELD:	text="Shield";		break;
				case TYPE_HEAD:		text="Helm";		break;
				case TYPE_GLOVE:	text="Glove";		break;
				case TYPE_SHOES:	text="Shoes";		break;
				case TYPE_TAILSMAN:	text="Talisman";	break;
			}
		}else{
			bgcolor=0;
			txtcolor=0;
			text="";
		}
		window->DrawBox(x+30,y+16,1000,10,bgcolor);
		window->DrawText(2,text,x+176,y+16,txtcolor);
	}
	if(param&256){
		window->DrawBox(x+30,y+20,1000,6,0x40ff8000);
		window->DrawText(2,"SubWeapon",x+176,y+16,0xffb08000);
	}

	D3DCOLOR tcolor=0;
	if(param&0x800)	tcolor=COLOR_GRAY;
	if((param&0x400) && (mType & TYPE_USE_ITEM) && (mType & TYPE_SPEND) && (param&0xff0000)<=0)	tcolor=COLOR_GRAY;
	if(param&2 && !((mType & TYPE_ON_MENU)	 && (mType & TYPE_USE_ITEM)))	tcolor=COLOR_GRAY;
	if(param&4 && !((mType & TYPE_ON_BATTLE) && (mType & TYPE_USE_ITEM)))	tcolor=COLOR_GRAY;

	if(mIcon>=0 && mIcon<ITEM_TEX_NUMBER * 64){
		int tx=mIcon%8*32;
		int ty=(mIcon/8)%8*32;
		window->DrawTexture(&ItemTex[mIcon/64],0,tx,ty,32,32,x-1,y+2,0x80000000);
		window->DrawTexture(&ItemTex[mIcon/64],0,tx,ty,32,32,x,y,color);
	}

/*	if(param&1){
		window->DrawTexture(&SystemTex,0,64,48,16,16,x-10,y+16,color);
	}
*/	
	if(param&8){
		if(mPrice>Flag.mMoney)		tcolor=COLOR_GRAY;
		if(mID<NO_ITEM){
			char text[64];
			sprintf(text,"%d M",mPrice);
			window->DrawText(0,text,20,y+8,tcolor,DT_RIGHT);
		}
	}
	if(param & 16){
		if(ID_UI<=mID && mID<ID_NSUI){
			int item=param/0x1000000;
			int farm=Flag.GetFarm(mID-ID_UI);
			if(item>=farm){
				tcolor=COLOR_GRAY;
			}
			char text[64];
			int no=farm-item;
			if(no<0)	no=0;
			sprintf(text,"%2d /",no);
			window->DrawText(0,text,50,y+8,tcolor,DT_RIGHT);
			sprintf(text,"%2d",farm);
			window->DrawText(0,text,20,y+8,tcolor,DT_RIGHT);
			//window->DrawText(2,"���Y��",90,y+4,tcolor,DT_RIGHT);
		}
	}
	if(param&32){
		if(mID<NO_ITEM){
			char text[64];
			sprintf(text,"%d",Flag.mCloset[mID]);
			window->DrawText(0,text,20,y+8,tcolor,DT_RIGHT);
			window->DrawText(0,"�~",50,y+8,tcolor,DT_RIGHT);
		}
	}
	if(param & 64){
		if(mID<NO_ITEM){
			char text[64];
			sprintf(text,"%d M",GetSellPrice());
			window->DrawText(0,text,20,y+8,tcolor,DT_RIGHT);
		}
	}
	if(param & 0x200){
		if(mID!=NO_ITEM){
			int img;
			switch((Flag.mPlayTime/10)%4){
				case 0:	img=1;	break;
				case 2:	img=2;	break;
				default:img=0;	break;
			}
			window->DrawTexture(&SystemTex,0,64,144+img*16,48,16,x-20,y+16);
		}
	}

	window->DrawText(0,mName,x+40,y+8,tcolor);
	if((param&0x400)&&(mType & TYPE_USE_ITEM)){
		char text[32];
		int count = (param&0xff0000)/0x10000;
		if(mType & TYPE_COLLECT){
			if(count<=1)	return;
			sprintf(text,"x %d",count);
		}else{
			if(count<=0)	return;
			sprintf(text,"( %d )",count);
		}
		window->DrawText(0,text,x+190,y+8,tcolor);
	}
}
void CItemData::Help(int param)const
{
	if(mID==NO_ITEM)	return;
	MenuData.SetMenu(ItemHelp,mID);
}

int		CItemData::GetPrice(void) const
{
	if(mPrice<0)	return 0;
	return mPrice;
}
int		CItemData::GetSellPrice(void) const
{
	if(ID_UI<=mID && mID<ID_NSUI)	return 0;
	return GetPrice()/2;
}

const char* CItemData::GetTypeName(void) const
{
	static char text[64];

	switch(mType & 0xf0000000){
		case TYPE_EQUIPMENT:
			switch(mType & 0xff000000){
				case TYPE_WEAPON:
					strcpy(text,"����");
					bool other;
					other=true;
					if(mType & 0xff & TYPE_SWORD)	{strcat(text,"(��)");	other=false;}
					if(mType & 0xff & TYPE_SPEAR)	{strcat(text,"(��)");	other=false;}
					if(mType & 0xff & TYPE_AXE)		{strcat(text,"(��)");	other=false;}
					if(mType & 0xff & TYPE_BOW)		{strcat(text,"(�|)");	other=false;}
					if(mType & 0xff & TYPE_LASER)	{strcat(text,"(���w����)");	other=false;}
					if(other)						{strcat(text,"(���̑�)");}
					break;
				case TYPE_ARMOR:
					strcpy(text,"�h��");	break;
				case TYPE_ACCESSORY:
					strcpy(text,"�����i - ");	
					if(mType & 0xff & TYPE_SHIELD)		{strcat(text,"��");		break;}
					if(mType & 0xff & TYPE_HEAD)		{strcat(text,"�X�q");	break;}
					if(mType & 0xff & TYPE_GLOVE)		{strcat(text,"���");	break;}
					if(mType & 0xff & TYPE_SHOES)		{strcat(text,"�C");		break;}
					if(mType & 0xff & TYPE_TAILSMAN)	{strcat(text,"�아");	break;}
					strcat(text,"���̑�");
					break;
			}
			break;
		case TYPE_USE_ITEM:
			strcpy(text,"����");	break;
		case TYPE_SEED:
			strcpy(text,"��");	break;
		default :
			strcpy(text," ");	break;
	}
	return text;
}

const int CItemData::GetCategory(void) const
{
	for(int i=1;i<CATEGORY_MAX;i++){
		if(mID<ItemCategory[i].mMax)	return i;
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////

CEquipmentData::CEquipmentData()
{
}

CEquipmentData::CEquipmentData(const char* name,int id,int icon,int price,int type,
						int lp,int atk,int def,int matk,int mdef,int speed,
						int skill, ...)
{
	mName=name;
	mID=id;
	mIcon=icon;
	mType=type | TYPE_EQUIPMENT;
	mPrice=price;

	mLP=lp;
	mAtk=atk;
	mDef=def;
	mMagAtk=matk;
	mMagDef=mdef;
	mSpeed=speed;

	int i=0;
	va_list marker;

	va_start( marker, skill ); 
	while(skill>=0 && i<MAX_WEAPON_SKILL)
	{
		mSkill[i]=skill;
		i++;
		skill = va_arg( marker, int);
	}
	va_end( marker );
	mSkillNo=i;
}
void CEquipmentData::Help(int param)const
{
	MenuData.SetMenu(EquipmentHelp,mID);
}

/////////////////////////////////////////////////////////////////////////////////////
CUseItemData::CUseItemData(void)
{
}
CUseItemData::CUseItemData(const char* name,int id,int icon,int price,
							int type,int parama,int paramb,int count)
{
	mName=name;
	mID=id;
	mIcon=icon;
	mPrice=price;
	mType=type | TYPE_USE_ITEM;
	mParamA=parama;
	mParamB=paramb;
	if(mType &TYPE_COLLECT){
		mUseCount = 10;
	}else{
		mUseCount = count;
	}
}
/////////////////////////////////////////////////////////////////////////////////////
CSeedData::CSeedData(void)
{
}
CSeedData::CSeedData(int id,int type)
{
	mName="�A���̃^�l";
	mID=id;
	mIcon=287;
	mPrice=0;
	mType=type | TYPE_SEED | TYPE_SEND_CLOSET;
}
/////////////////////////////////////////////////////////////////////////////////////

CItemCategoryData::CItemCategoryData(const char* name,int id,int min,int max)
{
	mName	=name;
	mID		=id;
	mMin	=min;
	mMax	=max;
}
/////////////////////////////////////////////////////////////////////////////////////

const  CEquipmentData EquipmentData[MAX_EQUIPMENT_NUMBER]=
{
//�����E��1����
	CEquipmentData(
		"�S�̌�",//
		ID_SWD+WLV_0+0,0,300,
		TYPE_SWORD	,
		0,25,0,0,0,2,
		-1),
	CEquipmentData(
		"�����O�\�[�h",//
		ID_SWD+WLV_0+1,1,590,
		TYPE_SWORD,
		0,35,0,0,0,0,
		-1),
//�����E��2����
	CEquipmentData(
		"�t�����x���W��",//
		ID_SWD+WLV_1+0,2,1800,
		TYPE_SWORD,
		0,55,0,0,0,3,
		256,-1),
	CEquipmentData(
		"�A�C�V�N���G�b�W",//
		ID_SWD+WLV_1+1,3,2200,
		TYPE_SWORD	,
		0,65,0,0,0,3,
		257,-1),
	CEquipmentData(
		"�󕗓�",//
		ID_SWD+WLV_1+2,4,2800,
		TYPE_SWORD	,
		0,60,0,0,0,10,
		258,-1),
	CEquipmentData(
		"���b�N�u���[�h",//
		ID_SWD+WLV_1+3,5,2600,
		TYPE_SWORD,
		0,55,8,0,0,0,
		259,-1),
//�����E��3����
	CEquipmentData(
		"�N���C���A",//
		ID_SWD+WLV_2+0,9,4400,
		TYPE_SWORD	,
		0,95,0,0,0,0,
		358,-1),
	CEquipmentData(
		"�ԎK�̒Z��",//
		ID_SWD+WLV_2+1,7,3400,
		TYPE_SWORD	,
		0,85,0,0,0,10,
		226,148,-1),
	CEquipmentData(
		"�N���e�B�J���u���C�Y",//
		ID_SWD+WLV_2+2,8,6200,
		TYPE_SWORD	,
		0,95,-20,0,-20,4,
		256,228,229,230,-1),
	CEquipmentData(
		"�}�W�J���\�[�h",//
		ID_SWD+WLV_2+3,6,7500,
		TYPE_SWORD	,
		0,100,0,95,0,4,
		94,102,110,118,-1),
//�����E��4����
	CEquipmentData(
		"�c�C���G�b�W",//
		ID_SWD+WLV_3+0,14,8800,
		TYPE_SWORD	,
		0,145,0,0,0,5,
		49,-1),
	CEquipmentData(
		"������",//
		ID_SWD+WLV_3+1,11,9200,
		TYPE_SWORD	,
		0,80,0,0,0,8,
		245,-1),
	CEquipmentData(
		"���Ԃ�",//
		ID_SWD+WLV_3+2,16,11000,
		TYPE_SWORD	,
		0,145,0,20,0,15,
		258,337,-1),
	CEquipmentData(
		"�i�C�g�u���[�h",//
		ID_SWD+WLV_3+3,13,20000,
		TYPE_SWORD	,
		0,160,0,20,0,5,
		387,388,389,390,-1),
//�����EExtra
	CEquipmentData(
		"�u���C�u�\�[�h",
		ID_SWD+WLV_4+0,10,17000,
		TYPE_SWORD	,
		0,225,0,0,0,5,
		49,50,51,-1),
	CEquipmentData(
		"�����V���o�[�t���A",//
		ID_SWD+WLV_4+1,12,30000,
		TYPE_SWORD	,
		0,265,0,40,0,5,
		256,368,-1),
	CEquipmentData(
		"�G�������^���}�X�^�[",
		ID_SWD+WLV_4+2,15,28000,
		TYPE_SWORD	,
		0,250,0,40,0,0,
		136,137,138,139,256,257,258,259,-1),
	CEquipmentData(
		"�A���W���x��",
		ID_SWD+WLV_4+3,17,32000,
		TYPE_SWORD	,
		0,300,0,60,0,15,
		257,352,-1),
//���E��1����
	CEquipmentData(
		"�S�̑�",//
		ID_SPR+WLV_0+0,18,300,
		TYPE_SPEAR	,
		0,24,2,0,0,0,
		-1),
	CEquipmentData(
		"���m�̑�",//
		ID_SPR+WLV_0+1,19,590,
		TYPE_SPEAR	,
		0,24,8,0,0,0,
		-1),
//���E��2����
	CEquipmentData(
		"�g���C�f���g",//
		ID_SPR+WLV_1+0,20,1600,
		TYPE_SPEAR	,
		0,52,8,0,0,0,
		-1),
	CEquipmentData(
		"�V���[�g�X�s�A",//
		ID_SPR+WLV_1+1,21,1800,
		TYPE_SPEAR	,
		0,48,0,0,0,0,
		195,-1),
	CEquipmentData(
		"�o�[�h�L���[",//
		ID_SPR+WLV_1+2,23,2800,
		TYPE_SPEAR	,
		0,45,0,0,0,12,
		337,-1),
	CEquipmentData(
		"�����̑�",//
		ID_SPR+WLV_1+3,22,3200,
		TYPE_SPEAR	,
		0,65,0,0,0,0,
		257,258,-1),
//���E��3����
	CEquipmentData(
		"�_����̑�",//
		ID_SPR+WLV_2+0,24,4200,
		TYPE_SPEAR	,
		0,92,12,0,8,0,
		368,-1),
	CEquipmentData(
		"�����̎h��",//
		ID_SPR+WLV_2+1,31,3800,
		TYPE_SPEAR	,
		0,80,20,0,10,5,
		257,292,-1),
	CEquipmentData(
		"�|�ۈ��",//
		ID_SPR+WLV_2+2,29,5400,
		TYPE_SPEAR	,
		0,50,0,0,0,0,
		245,-1),
	CEquipmentData(
		"���ɂ����̑�",//
		ID_SPR+WLV_2+3,26,7400,
		TYPE_SPEAR	,
		0,105,0,0,0,0,
		194,195,-1),
//���E��4����
	CEquipmentData(
		"���C�g�j���O�����X",//
		ID_SPR+WLV_3+0,32,8200,
		TYPE_SPEAR	,
		0,145,0,0,0,20,
		-1),
	CEquipmentData(
		"�n��_�ւ̕�葄",//
		ID_SPR+WLV_3+1,25,9900,
		TYPE_SPEAR	,
		200,145,10,0,0,0,
		259,-1),
	CEquipmentData(
		"�f�B�t�F���T�[�X�s�A",//
		ID_SPR+WLV_3+2,30,10800,
		TYPE_SPEAR	,
		0,140,40,0,40,-10,
		154,-1),
	CEquipmentData(
		"�㓁",//
		ID_SPR+WLV_3+3,27,12200,
		TYPE_SPEAR,
		0,165,0,0,0,0,
		356,-1),
//���EExtra
	CEquipmentData(
		"�X���܉�",
		ID_SPR+WLV_4+0,28,16800,
		TYPE_SPEAR	,
		0,220,20,0,0,0,
		257,-1),
	CEquipmentData(
		"���z������",
		ID_SPR+WLV_4+1,33,100,
		TYPE_SPEAR	,
		-100,300,-20,0,-20,0,
		255,-1),
	CEquipmentData(
		"�~�R�g�̑�",	
		ID_SPR+WLV_4+2,34,22000,
		TYPE_SPEAR	,
		0,230,0,0,0,0,
		259,171,-1),
	CEquipmentData(
		"���R�l�̊M��",
		ID_SPR+WLV_4+3,35,30000,
		TYPE_SPEAR	,
		0,260,0,0,0,0,
		252,401,-1),
//���E��1����
	CEquipmentData(
		"�S�̕�",//
		ID_AXE+WLV_0+0,36,300,
		TYPE_AXE	,
		0,28,0,0,0,-5,
		-1),
	CEquipmentData(
		"�啀",//
		ID_AXE+WLV_0+1,37,560,
		TYPE_AXE	,
		0,45,0,0,0,-20,
		-1),
//���E��2����
	CEquipmentData(
		"�o�g���A�b�N�X",//
		ID_AXE+WLV_1+0,38,1800,
		TYPE_AXE	,
		0,60,0,0,0,-8,
		-1),
	CEquipmentData(
		"�w�r�[�A�b�N�X",//
		ID_AXE+WLV_1+1,39,2200,
		TYPE_AXE	,
		0,95,0,0,0,-64,
		-1),
	CEquipmentData(
		"�����̍�",//
		ID_AXE+WLV_1+2,40,3400,
		TYPE_AXE	,
		0,75,0,0,0,-16,
		329,330,-1),
	CEquipmentData(
		"�؂���̕�",//
		ID_AXE+WLV_1+3,41,1000,
		TYPE_AXE	,
		0,50,0,0,0,0,
		302,-1),
//���E��3����
	CEquipmentData(
		"�C���̕�",//
		ID_AXE+WLV_2+0,46,4800,
		TYPE_AXE	,
		0,95,0,0,0,0,
		257,-1),
	CEquipmentData(
		"�R���̕�",//
		ID_AXE+WLV_2+1,48,4800,
		TYPE_AXE	,
		0,110,0,0,0,-24,
		259,-1),
	CEquipmentData(
		"�T���E���h�A�b�N�X",//
		ID_AXE+WLV_2+2,47,7200,//��������
		TYPE_AXE	,
		0,90,0,0,0,-12,
		290,-1),
	CEquipmentData(
		"�|�[���A�N�X",//
		ID_AXE+WLV_2+3,45,5000,
		TYPE_AXE	,
		0,115,0,0,0,-12,
		357,-1),
//���E��4����
	CEquipmentData(
		"�L���O�A�b�N�X",//
		ID_AXE+WLV_3+0,42,8000,
		TYPE_AXE	,
		0,175,0,0,0,-8,
		-1),
	CEquipmentData(
		"������",//
		ID_AXE+WLV_3+1,43,7500,
		TYPE_AXE	,
		0,150,0,0,0,20,
		258,-1),
	CEquipmentData(
		"�~���N���A�b�N�X",//
		ID_AXE+WLV_3+2,44,9200,
		TYPE_AXE	,
		0,170,0,0,0,-20,
		337,368,363,-1),
	CEquipmentData(
		"�K���X�̕�",//
		ID_AXE+WLV_3+3,49,10000,
		TYPE_AXE	,
		0,175,0,0,0,0,
		228,229,230,351,-1),
//���EExtra
	CEquipmentData(
		"�W���[�C�Y�X�y�V����",
		ID_AXE+WLV_4+0,53,22000,
		TYPE_AXE	,
		0,270,0,0,0,-16,
		-1),
	CEquipmentData(
		"�A�[�X�u���C�J�[",
		ID_AXE+WLV_4+1,51,19000,
		TYPE_AXE	,
		0,340,0,0,0,-20,
		259,290,347,-1),
	CEquipmentData(
		"�����̕�",
		ID_AXE+WLV_4+2,52,28000,
		TYPE_AXE	,
		0,270,-10,-10,-10,-16,
		257,329,330,331,332,333,-1),
	CEquipmentData(
		"�S�[���f���A�b�N�X",
		ID_AXE+WLV_4+3,50,100000,
		TYPE_AXE	,
		0,450,20,20,20,-200,
		-1),
//�|�E��1����
	CEquipmentData(
		"�����ȋ|",//
		ID_BOW+WLV_0+0,55,300,
		TYPE_BOW	,
		0,20,0,0,0,5,
		-1),
	CEquipmentData(
		"��l�̋|",//
		ID_BOW+WLV_0+1,54,560,
		TYPE_BOW	,
		0,30,0,0,0,0,
		-1),
//�|�E��2����
	CEquipmentData(
		"���|",//
		ID_BOW+WLV_1+0,56,1800,
		TYPE_BOW	,
		0,50,0,0,0,5,
		-1),
	CEquipmentData(
		"�L���[�{�E",//
		ID_BOW+WLV_1+1,57,2200,
		TYPE_BOW	,
		0,65,-10,0,-10,0,
		228,229,-1),
	CEquipmentData(
		"�|�C�Y���V���b�g",//
		ID_BOW+WLV_1+2,58,3200,
		TYPE_BOW	,
		0,40,0,0,0,0,
		220,142,-1),
	CEquipmentData(
		"�p�����C�Y�V���b�g",//
		ID_BOW+WLV_1+3,59,4200,
		TYPE_BOW	,
		0,40,0,0,0,0,
		223,145,-1),
//�|�E��3����
	CEquipmentData(
		"���m�|",//
		ID_BOW+WLV_2+0,60,4200,
		TYPE_BOW	,
		0,85,0,0,0,0,
		-1),
	CEquipmentData(
		"�N���X�{�E",//
		ID_BOW+WLV_2+1,62,3200,
		TYPE_BOW	,
		0,200,0,0,0,-10,
		303,-1),
	CEquipmentData(
		"�X�g�b�v���[�V����",//
		ID_BOW+WLV_2+2,61,5900,
		TYPE_BOW	,
		0,90,0,0,0,10,
		292,-1),
	CEquipmentData(
		"�n���̋|",//
		ID_BOW+WLV_2+3,63,8000,
		TYPE_BOW	,
		0,100,0,0,0,0,
		227,-1),
//�|�E��4����
	CEquipmentData(
		"�e���y�X�g",//
		ID_BOW+WLV_3+0,67,7800,
		TYPE_BOW	,
		0,135,0,0,10,10,
		257,258,-1),
	CEquipmentData(
		"�{���P�C�m",//
		ID_BOW+WLV_3+1,66,7800,
		TYPE_BOW	,
		0,140,10,0,0,0,
		256,259,-1),
	CEquipmentData(
		"�w�r�[�N���X�{�E",//
		ID_BOW+WLV_3+2,68,7800,
		TYPE_BOW	,
		0,300,0,0,0,-12,
		303,-1),
	CEquipmentData(
		"�C�̕P�̒G��",//
		ID_BOW+WLV_3+3,69,7800,
		TYPE_BOW	,
		0,175,0,0,0,0,
		257,222,386,-1),
//�|�EExtra
	CEquipmentData(
		"�V���̋|",
		ID_BOW+WLV_4+0,64,18000,
		TYPE_BOW	,
		0,220,0,0,0,5,
		-1),
	CEquipmentData(
		"�O���l�[�h�N���X�{�E",
		ID_BOW+WLV_4+1,65,21000,
		TYPE_BOW	,
		0,500,0,0,0,-16,
		256,303,290,-1),
	CEquipmentData(
		"�c�C���w�b�h�h���S��",
		ID_BOW+WLV_4+2,70,22000,
		TYPE_BOW	,
		0,80,0,0,0,0,
		258,304,-1),
	CEquipmentData(
		"�f�B�U�X�^�[",
		ID_BOW+WLV_4+3,71,28000,
		TYPE_BOW	,
		0,0,0,0,0,0,
		220,221,222,223,224,225,226,227,-1),
//��E��1����
	CEquipmentData(
		"�~�̏�",//
		ID_WND+WLV_0+0,72,300,
		TYPE_WEAPON	,
		0,8,0,30,0,0,
		-1),
	CEquipmentData(
		"���S�҂̏�",//
		ID_WND+WLV_0+1,73,480,
		TYPE_WEAPON	,
		0,5,0,60,0,0,
		93,359,-1),
//��E��2����
	CEquipmentData(
		"�E�B�U�[�h���b�h",//
		ID_WND+WLV_1+0,74,1500,
		TYPE_WEAPON	,
		0,10,0,65,0,0,
		-1),
	CEquipmentData(
		"�K�[�h�|�[��",//
		ID_WND+WLV_1+1,75,1800,
		TYPE_WEAPON	,
		0,25,10,55,10,0,
		-1),
	CEquipmentData(
		"�����̏�",//
		ID_WND+WLV_1+2,76,2200,
		TYPE_WEAPON	,
		0,10,0,45,0,0,
		132,-1),
	CEquipmentData(
		"���C�X",//
		ID_WND+WLV_1+3,77,4400,
		TYPE_WEAPON	,
		0,95,0,0,0,-5,
		-1),
//��E��3����
	CEquipmentData(
		"�B���p�t�̏�",//
		ID_WND+WLV_2+0,78,4500,
		TYPE_WEAPON	,
		0,20,0,100,0,0,
		237,-1),
	CEquipmentData(
		"��V���l�̒��J",//
		ID_WND+WLV_2+1,84,4500,
		TYPE_WEAPON	,
		0,50,5,110,5,5,
		96,104,112,120,-1),
	CEquipmentData(
		"�G���n���X���b�h",//
		ID_WND+WLV_2+2,80,4500,
		TYPE_WEAPON	,
		0,0,0,110,0,0,
		346,364,-1),
	CEquipmentData(
		"����̏�",//
		ID_WND+WLV_2+3,81,4500,
		TYPE_WEAPON	,
		0,125,0,110,0,0,
		224,146,-1),
//��E��4����
	CEquipmentData(
		"�|�W�e�B�u���b�h",//
		ID_WND+WLV_3+0,82,8900,
		TYPE_WEAPON	,
		0,30,0,165,0,0,
		94,110,-1),
	CEquipmentData(
		"�l�K�e�B�u���b�h",//
		ID_WND+WLV_3+1,83,8900,
		TYPE_WEAPON	,
		0,30,0,165,0,0,
		102,118,-1),
	CEquipmentData(
		"�A�z�t�̖_",//
		ID_WND+WLV_3+2,79,7500,
		TYPE_WEAPON	,
		0,0,0,180,0,0,
		354,-1),
	CEquipmentData(
		"�A�����b�N�X",//
		ID_WND+WLV_3+3,85,9900,
		TYPE_WEAPON	,
		0,80,0,170,0,0,
		278,279,280,360,-1),
//��EExtra
	CEquipmentData(
		"�����̏�",
		ID_WND+WLV_4+0,87,19000,
		TYPE_WEAPON	,
		0,50,0,250,0,0,
		135,-1),
	CEquipmentData(
		"�����̏�",
		ID_WND+WLV_4+1,86,18000,
		TYPE_WEAPON	,
		0,30,0,300,0,5,
		292,-1),
	CEquipmentData(
		"���؂�̎���",
		ID_WND+WLV_4+2,88,22000,
		TYPE_WEAPON	,
		0,520,0,540,0,0,
		382,-1),
	CEquipmentData(
		"�j��_�̖_",
		ID_WND+WLV_4+3,89,25000,
		TYPE_WEAPON	,
		0,666,0,0,0,0,
		-1),
//���ꕐ��
	CEquipmentData(
		"���̖��p��",//
		ID_SWP+0,90,2800,
		TYPE_SWORD|TYPE_SPEAR|TYPE_AXE	,
		0,0,0,50,0,0,
		253,-1),
	CEquipmentData(
		"���[�U�[�V���b�g",
		ID_SWP+1,91,8000,
		TYPE_LASER	,
		0,250,0,0,0,0,
		303,-1),
	CEquipmentData(
		"���܂̂ʂ������",//
		ID_SWP+2,93,0,
		TYPE_WEAPON	,
		0,0,0,0,0,0,
		182,183,186,187,-1),
	CEquipmentData(
		"���������̖�",//
		ID_SWP+3,92,0,
		TYPE_SWORD|TYPE_SPEAR|TYPE_AXE|TYPE_BOW	,
		0,500,0,500,0,50,
		245,247,249,377,268,304,392,309,-1),
//�h��E��1����
	CEquipmentData(
		"��v�ȕ�",//
		ID_AMR+ALV_0+0,94,200,
		TYPE_ARMOR	,
		100,0,0,0,0,0,
		376,-1),
	CEquipmentData(
		"�`�F�C�����C��",//
		ID_AMR+ALV_0+1,95,380,
		TYPE_ARMOR	,
		140,0,5,-5,0,0,
		376,-1),
	CEquipmentData(
		"���[�u",//
		ID_AMR+ALV_0+2,96,320,
		TYPE_ARMOR	,
		80,0,0,5,10,0,
		376,-1),
	CEquipmentData(
		"�����̍�",
		ID_AMR+ALV_0+3,97,19998,
		TYPE_ARMOR	,
		-9999,0,0,0,0,0,
		-1),
//�h��E��2����
	CEquipmentData(
		"���̊Z",//
		ID_AMR+ALV_1+0,98,1400,
		TYPE_ARMOR	,
		240,5,0,0,0,0,
		196,376,-1),
	CEquipmentData(
		"�X�̊Z",//
		ID_AMR+ALV_1+1,99,1400,
		TYPE_ARMOR	,
		240,0,0,5,0,0,
		200,376,-1),
	CEquipmentData(
		"�v���[�g�A�[�}�[",//
		ID_AMR+ALV_1+2,102,2600,
		TYPE_ARMOR	,
		350,0,15,-100,0,-16,
		376,-1),
	CEquipmentData(
		"���@�̃��[�u",//
		ID_AMR+ALV_1+3,103,2200,
		TYPE_ARMOR	,
		220,0,0,20,15,0,
		376,-1),
//�h��E��3����
	CEquipmentData(
		"���C�g�A�[�}�[",//
		ID_AMR+ALV_2+0,104,3900,
		TYPE_ARMOR	,
		450,0,0,0,0,5,
		376,-1),
	CEquipmentData(
		"�d�R�m�̊Z",//
		ID_AMR+ALV_2+1,106,5200,
		TYPE_ARMOR	,
		700,0,20,-400,0,-24,
		371,-1),
	CEquipmentData(
		"���̖͂@��",//
		ID_AMR+ALV_2+2,107,4200,
		TYPE_ARMOR	,
		420,0,0,30,20,0,
		371,-1),
	CEquipmentData(
		"�Ός̖є�",//
		ID_AMR+ALV_2+3,105,6000,
		TYPE_ARMOR	,
		480,15,0,0,0,0,
		298,196,-1),
	CEquipmentData(
		"�����̕�",//
		ID_AMR+ALV_2+4,109,4400,
		TYPE_ARMOR	,
		450,30,0,0,0,0,
		-1),
	CEquipmentData(
		"�C���r�W�u���R�[�g",//���w����
		ID_AMR+ALV_2+5,115,8000,
		TYPE_ARMOR	,
		370,0,0,0,0,0,
		383,-1),
//�h��E��4����
	CEquipmentData(
		"���_�̊Z",//
		ID_AMR+ALV_3+0,110,6800,
		TYPE_ARMOR	,
		800,0,0,0,0,10,
		204,371,-1),
	CEquipmentData(
		"�n�_�̊Z",//
		ID_AMR+ALV_3+1,111,6800,
		TYPE_ARMOR	,
		900,0,5,0,0,0,
		208,371,-1),
	CEquipmentData(
		"�O���[�g�A�[�}�[",//
		ID_AMR+ALV_3+2,112,9000,
		TYPE_ARMOR	,
		1300,0,30,-600,0,-32,
		371,-1),
	CEquipmentData(
		"���҂̃��[�u",//
		ID_AMR+ALV_3+3,113,8800,
		TYPE_ARMOR	,
		750,0,0,40,30,0,
		371,-1),
	CEquipmentData(
		"�����̊Z",//
		ID_AMR+ALV_3+4,108,4200,
		TYPE_ARMOR	,
		1000,30,0,0,0,0,
		54,65,75,85,-1),
	CEquipmentData(
		"�}�[���C�h�N���X",//
		ID_AMR+ALV_3+5,114,13000,
		TYPE_ARMOR	,
		800,0,0,0,30,0,
		371,200,299,-1),
	CEquipmentData(
		"�݋T�̍b��",
		ID_AMR+ALV_3+6,100,18000,
		TYPE_ARMOR	,
		1900,0,0,0,0,0,
		200,395,-1),
	CEquipmentData(
		"�s���҂̈�",//
		ID_AMR+ALV_3+7,101,14000,
		TYPE_ARMOR	,
		999,0,-40,0,-40,0,
		365,393,199,-1),
//�h��EExtra
	CEquipmentData(
		"�v���e�N�g�t�B�[���h",
		ID_AMR+ALV_4+0,116,14000,
		TYPE_ARMOR	,
		1500,0,0,0,0,0,
		371,-1),
	CEquipmentData(
		"�G���y���[�A�[�}�[",
		ID_AMR+ALV_4+1,117,19000,
		TYPE_ARMOR	,
		2500,0,50,-999,0,-24,
		371,-1),
	CEquipmentData(
		"�s���̊Z",
		ID_AMR+ALV_4+2,118,24000,
		TYPE_ARMOR	,
		2500,0,50,-999,0,-999,
		307,-1),
	CEquipmentData(
		"�V�g�̈�",
		ID_AMR+ALV_4+3,119,18000,
		TYPE_ARMOR	,
		1400,0,0,60,50,0,
		371,-1),
	CEquipmentData(
		"���ԕ�",
		ID_AMR+ALV_4+4,120,19000,
		TYPE_ARMOR	,
		1200,0,0,0,0,0,
		204,242,-1),
	CEquipmentData(
		"���C���q�̒Z�p��",
		ID_AMR+ALV_4+5,121,20000,
		TYPE_ARMOR	,
		2000,0,-80,0,-80,15,
		420,-1),
//�A�N�Z�T���[
//��
	CEquipmentData(
		"�؂̏�",
		ID_SLD+0,128,250,
		TYPE_SHIELD	,
		0,0,6,0,0,0,
		366,-1),
	CEquipmentData(
		"���C�g�V�[���h",
		ID_SLD+1,129,460,//����A�b�v
		TYPE_SHIELD	,
		0,0,8,0,0,0,
		367,231,-1),
	CEquipmentData(
		"�ŉ�̏�",		//�ŁA��ბϐ�
		ID_SLD+2,136,580,
		TYPE_SHIELD	,
		0,0,14,0,0,0,
		366,212,215,-1),
	CEquipmentData(
		"",
		ID_SLD+3,NO_ICON,80,
		TYPE_SHIELD	,
		0,0,4,0,0,0,
		-1),
	CEquipmentData(//����A�b�v
		"�A�C�A���V�[���h",
		ID_SLD+4,130,880,
		TYPE_SHIELD	,
		0,0,18,0,0,0,
		231,-1),
	CEquipmentData(
		"�n��̏�",
		ID_SLD+5,131,1600,//���h��
		TYPE_SHIELD	,
		0,0,26,0,0,0,
		196,208,367,-1),
	CEquipmentData(
		"�����̏�",
		ID_SLD+6,132,1600,//�X�h��
		TYPE_SHIELD	,
		0,0,26,0,0,0,
		200,204,367,-1),
	CEquipmentData(
		"�V���o�[�V�[���h",//�ŁA���s�h��
		ID_SLD+7,134,2400,
		TYPE_SHIELD	,
		0,0,35,0,0,0,
		200,212,218,367,-1),
	CEquipmentData(//����A�b�v
		"�R�m�c�̏�",
		ID_SLD+8,133,3200,
		TYPE_SHIELD	,
		0,0,45,0,0,0,
		231,-1),
	CEquipmentData(
		"�v���X�e�B�b�N�K�[�h",	//��ზh��
		ID_SLD+9,135,5500,
		TYPE_SHIELD	,
		0,0,45,0,0,0,
		196,204,215,367,-1),
	CEquipmentData(
		"�X�����̏�",
		ID_SLD+10,137,4800,
		TYPE_SHIELD	,
		0,0,60,0,0,0,
		200,361,362,-1),
	CEquipmentData(
		"��n�̌b�݂̏�",		//HP������
		ID_SLD+11,185,8000,
		TYPE_SHIELD	,
		0,0,45,0,20,0,
		208,251,367,-1),
	CEquipmentData(
		"�ʖ�",					//�S�����ɑϐ�
		ID_SLD+12,186,9900,
		TYPE_SHIELD	,
		0,0,20,0,0,0,
		196,200,204,208,367,-1),
	CEquipmentData(	
		"�}�e���A���o���A�[",	//�������h��
		ID_SLD+13,187,9900,
		TYPE_SHIELD	,
		0,0,20,0,0,0,
		384,198,202,206,210,-1),
	CEquipmentData(
		"�t�F�U�[�V�[���h",		//����A�b�v
		ID_SLD+14,188,50000,
		TYPE_SHIELD	,
		0,0,0,0,0,0,
		204,231,232,233,-1),
	CEquipmentData(
		"�K�[�f�B�A���V�[���h",	//�h��͂��ō��̏�
		ID_SLD+15,189,50000,
		TYPE_SHIELD	,
		0,0,120,0,0,0,
		208,231,-1),
//��
	CEquipmentData(
		"�o���_�i",
		ID_HED+0,138,180,
		TYPE_HEAD	,
		0,0,0,0,8,0,
		-1),
	CEquipmentData(
		"�H�X�q",
		ID_HED+1,139,380,
		TYPE_HEAD	,
		0,0,0,0,8,6,
		204,-1),
	CEquipmentData(
		"���f�̔��ǂ�",		//����A�����h��
		ID_HED+2,140,520,
		TYPE_HEAD	,
		0,0,0,0,14,0,
		213,214,-1),
	CEquipmentData(
		"�O�p�X�q",		//����h��A���@�A�b�v
		ID_HED+3,141,480,
		TYPE_HEAD	,
		0,0,0,15,15,0,
		216,-1),
	CEquipmentData(
		"�A�C�A���w����",		//�N���e�B�J���h��
		ID_HED+4,142,800,
		TYPE_HEAD	,
		0,0,0,0,18,0,
		361,-1),
	CEquipmentData(
		"��w�̉H",				//�X�^�[�g�_�b�V��
		ID_HED+5,143,1200,
		TYPE_HEAD	,
		0,0,0,0,0,0,
		342,-1),
	CEquipmentData(
		"�����̊�",				//�l���o���l�A�b�v
		ID_HED+6,146,1600,
		TYPE_HEAD	,
		0,0,0,20,26,0,
		239,-1),
	CEquipmentData(
		"���ꂱ����",			//���s�A�����h��
		ID_HED+7,144,100,
		TYPE_HEAD	,
		0,0,25,0,-25,0,
		218,219,-1),
	CEquipmentData(
		"�X�R�[�s�I���w����",	//�ŁA��ზh��
		ID_HED+8,147,1280,
		TYPE_HEAD	,
		0,0,14,0,14,0,
		196,212,215,-1),
	CEquipmentData(
		"�����X",				//�����A�\���h��
		ID_HED+9,182,2000,
		TYPE_HEAD	,
		0,0,0,0,32,0,
		200,214,217,-1),
	CEquipmentData(
		"�i�C�g�w����",			//�N���e�B�J���h��
		ID_HED+10,145,3600,
		TYPE_HEAD	,
		0,0,0,0,45,0,
		361,-1),
	CEquipmentData(
		"�z�[�N�n�b�g",			//��̊�
		ID_HED+11,183,12000,
		TYPE_HEAD	,
		0,0,0,25,45,0,
		204,274,-1),
	CEquipmentData(
		"����̊�",				//�����A�\���A����h��
		ID_HED+12,190,4800,
		TYPE_HEAD	,
		0,0,0,0,60,0,
		213,214,217,-1),
	CEquipmentData(
		"�▂�̊�",				//�ŋ��̖��@�h���
		ID_HED+13,205,5000,
		TYPE_HEAD	,
		0,0,-300,0,300,0,
		-1),
	CEquipmentData(
		"���҂̃e�B�A��",		//����h��ASP,���@�A�b�v
		ID_HED+14,191,5200,
		TYPE_HEAD	,
		0,0,0,60,60,0,
		194,216,-1),
	CEquipmentData(
		"�S�̉���",				//�ŁA���s�A�����h��A�N���e�B�J���h��
		ID_HED+15,192,50000,
		TYPE_HEAD	,
		0,0,0,30,110,0,
		361,212,218,219,-1),
//���
	CEquipmentData(
		"���",
		ID_GLV+0,148,220,
		TYPE_GLOVE,
		0,10,0,0,0,0,
		364,-1),
	CEquipmentData(
		"��t�̎��",
		ID_GLV+1,184,490,
		TYPE_GLOVE	,
		20,0,0,0,0,0,
		374,-1),
	CEquipmentData(
		"�p���[�O���[�u",	//�u���b�N����
		ID_GLV+2,150,RARE_ITEM,
		TYPE_GLOVE	,
		0,22,0,0,0,0,
		296,-1),
	CEquipmentData(
		"�����̘r��",		//�t�F�C���g
		ID_GLV+3,194,560,
		TYPE_GLOVE	,
		0,0,0,0,0,0,
		250,-1),
	CEquipmentData(
		"���@�ڂ���̘r��",	//�}�W�b�N�V���[�g
		ID_GLV+4,156,800,
		TYPE_GLOVE	,
		0,0,0,16,0,0,
		93,-1),
	CEquipmentData(
		"�����̎��",		//�l�����z�A�b�v
		ID_GLV+5,157,1000,
		TYPE_GLOVE	,
		0,14,0,0,0,0,
		235,-1),
	CEquipmentData(
		"�o�g���O���u",
		ID_GLV+6,152,960,//
		TYPE_GLOVE	,
		0,22,0,0,0,0,
		228,-1),
	CEquipmentData(
		"�����̘r��",		//���Z�K��
		ID_GLV+7,153,1600,
		TYPE_GLOVE	,
		0,35,0,0,0,5,
		49,50,51,-1),
	CEquipmentData(
		"���R�̘r��",		//���Z�K��
		ID_GLV+8,154,1600,
		TYPE_GLOVE	,
		0,35,5,0,0,0,
		59,60,61,-1),
	CEquipmentData(
		"�퉤�̘r��",		//���Z�K��
		ID_GLV+9,155,1600,
		TYPE_GLOVE	,
		0,45,0,0,0,0,
		69,70,71,-1),
	CEquipmentData(
		"�����̎�b",		//�h�䔼��
		ID_GLV+10,151,3200,
		TYPE_GLOVE	,
		0,30,-30,0,0,0,
		244,-1),
	CEquipmentData(
		"�����̎�b",		//���@�h�䔼��
		ID_GLV+11,195,3200,
		TYPE_GLOVE	,
		0,0,0,30,-30,0,
		246,-1),
	CEquipmentData(
		"�i�C�g�O���[�u",
		ID_GLV+12,196,3800,
		TYPE_GLOVE	,
		0,60,0,0,0,0,
		228,-1),
	CEquipmentData(
		"�K�E�̘r��",
		ID_GLV+13,197,8000,
		TYPE_GLOVE	,
		0,55,0,0,0,0,
		352,-1),
	CEquipmentData(
		"�o�C�I���b�g���[�Y",
		ID_GLV+14,198,6400,
		TYPE_GLOVE	,
		0,80,0,80,0,0,
		-1),
	CEquipmentData(
		"���P�����ނ̎�",
		ID_GLV+15,199,50000,
		TYPE_GLOVE	,
		0,160,0,0,0,5,
		250,256,364,228,-1),
//�C
	CEquipmentData(
		"�u�[�c",
		ID_SHE+0,158,210,
		TYPE_SHOES	,
		0,0,0,0,0,18,
		-1),
	CEquipmentData(
		"�y�ʃV���[�Y",
		ID_SHE+1,159,800,
		TYPE_SHOES	,
		0,0,0,0,0,24,
		-1),
	CEquipmentData(
		"���N�T���_��",
		ID_SHE+2,160,580,
		TYPE_SHOES	,
		30,0,0,0,0,12,
		213,-1),
	CEquipmentData(
		"�|�̂��˓���",		//�L�����Z���h��
		ID_SHE+3,161,900,
		TYPE_SHOES	,
		0,0,0,0,0,20,
		362,-1),
	CEquipmentData(
		"��n�̌C",			//�y�h��
		ID_SHE+4,162,1200,
		TYPE_SHOES	,
		0,0,0,0,0,22,
		362,208,-1),
	CEquipmentData(
		"�K���X�̌C",		//�����h��
		ID_SHE+5,200,2800,
		TYPE_SHOES	,
		0,0,0,10,10,24,
		200,214,-1),
	CEquipmentData(
		"",	
		ID_SHE+6,NO_ICON,60,
		TYPE_SHOES	,
		0,0,0,0,0,5,
		-1),
	CEquipmentData(
		"����X�L�[",		//����X�L�[
		ID_SHE+7,163,RARE_ITEM,
		TYPE_SHOES	,
		0,0,10,0,0,0,
		200,341,-1),
	CEquipmentData(
		"�w�r�B�u�[�c",
		ID_SHE+8,202,60,
		TYPE_SHOES	,
		0,0,20,0,20,-32,
		362,208,287,293,-1),
	CEquipmentData(
		"�R�m�c�̋r�b",		//�L�����Z���h��
		ID_SHE+9,201,3200,
		TYPE_SHOES	,
		0,0,0,0,0,30,
		362,-1),
	CEquipmentData(
		"�E�C���O�V���[�Y",	//�󒆕��V
		ID_SHE+10,164,RARE_ITEM,
		TYPE_SHOES	,
		0,0,0,0,0,25,
		204,281,-1),
	CEquipmentData(
		"�V���[�e�B���O�X�^�[",
		ID_SHE+11,203,6800,
		TYPE_SHOES	,
		0,24,0,0,0,35,
		362,196,204,-1),
	CEquipmentData(
		"�X�v�����O�V���[�Y",	//�n�C�W�����v
		ID_SHE+12,165,30000,
		TYPE_SHOES	,
		0,0,0,0,0,20,
		276,-1),
	CEquipmentData(
		"�W�F�b�g�u�[�c",		//�X�[�p�[�_�b�V��
		ID_SHE+13,166,30000,
		TYPE_SHOES	,
		0,0,0,0,0,45,
		277,-1),
	CEquipmentData(	
		"�R���L�X�^�h�[��",		//��ჁA����A�L�����Z���h��
		ID_SHE+14,167,50000,
		TYPE_SHOES	,
		0,0,15,0,15,70,
		362,200,208,215,216,-1),
	CEquipmentData(
		"�N�C�b�N�N���b�N",
		ID_SHE+15,204,60,
		TYPE_SHOES	,
		-1000,0,0,0,0,250,
		-1),
//�����
	CEquipmentData(
		"���̂����",
		ID_AME+0,176,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		170,-1),
	CEquipmentData(
		"�͂̂����",
		ID_AME+1,177,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		174,-1),
	CEquipmentData(
		"�h�̂����",
		ID_AME+2,178,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		178,-1),
	CEquipmentData(
		"���̂����",
		ID_AME+3,177,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		182,-1),
	CEquipmentData(
		"�R�̂����",
		ID_AME+4,178,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		186,-1),
	CEquipmentData(
		"�Z�̂����",
		ID_AME+5,179,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		194,-1),
	CEquipmentData(
		"���̂����",
		ID_AME+6,180,5000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		190,-1),
	CEquipmentData(
		"�ω��̂����",
		ID_AME+7,172,1000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		196,-1),
	CEquipmentData(
		"�ϕX�̂����",
		ID_AME+8,173,1000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		200,-1),
	CEquipmentData(
		"�ϕ��̂����",
		ID_AME+9,174,1000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		204,-1),
	CEquipmentData(
		"�ϓy�̂����",
		ID_AME+10,175,1000,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		208,-1),
	CEquipmentData(
		"��ł̂����",
		ID_AME+11,170,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		212,-1),
	CEquipmentData(
		"�𖰂̂����",
		ID_AME+12,169,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		213,-1),
	CEquipmentData(
		"�𖣂̂����",
		ID_AME+13,168,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		214,-1),
	CEquipmentData(
		"��Ⴢ̂����",
		ID_AME+14,171,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		215,-1),
	CEquipmentData(
		"�𕕂̂����",
		ID_AME+15,170,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		216,-1),
	CEquipmentData(
		"��\�̂����",
		ID_AME+16,168,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		217,-1),
	CEquipmentData(
		"�𕅂̂����",
		ID_AME+17,169,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		218,-1),
	CEquipmentData(
		"�����̂����",
		ID_AME+18,181,800,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		219,-1),
	CEquipmentData(
		"",
		ID_AME+19,NO_ICON,100,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		-1),
	CEquipmentData(
		"",
		ID_AME+20,NO_ICON,100,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		-1),
	CEquipmentData(
		"",
		ID_AME+21,NO_ICON,100,
		TYPE_TAILSMAN	,
		0,0,0,0,0,0,
		-1),
};
const CUseItemData	UseItemData[MAX_USE_ITEM_NUMBER]={
	CUseItemData(
		"��",
		ID_UI+0,256,10,
		TYPE_LP_HEAL,
		30 ,0 ,3),
	CUseItemData(
		"�����̑�",
		ID_UI+1,256,100,
		TYPE_LP_HEAL,
		300,0 ,1),
	CUseItemData(
		"���̑�",
		ID_UI+2,257,500,
		TYPE_LP_HEAL,
		1000,0 ,1),
	CUseItemData(
		"��Ղ̑�",
		ID_UI+3,257,2000,
		TYPE_LP_HEAL,
		5000,0 ,1),
	CUseItemData(
		"�q�[���^���|�|",
		ID_UI+4,265,500,
		TYPE_HP_HEAL,
		1000,0 ,1),
	CUseItemData(
		"���J�o���[�^���|�|",
		ID_UI+5,265,2000,
		TYPE_HP_HEAL,
		6000,0 ,1),
	CUseItemData(
		"�򉻂̑�",
		ID_UI+6,256,50,
		TYPE_STATUS_HEAL,
		0,0 ,5),
	CUseItemData(
		"�����̑�",
		ID_UI+7,268,800,
		TYPE_REVIVAL,
		80,0 ,1),
	CUseItemData(
		"�̃X�L����",
		ID_UI+8,258,20,
		TYPE_SP_HEAL,
		10,0 ,3),
	CUseItemData(
		"�΂̃X�L����",
		ID_UI+9,259,120,
		TYPE_SP_HEAL,
		8,1 ,1),
	CUseItemData(
		"���̃X�L����",
		ID_UI+10,260,520,
		TYPE_SP_HEAL,
		5,2 ,1),
	CUseItemData(
		"�Ԃ̃X�L����",
		ID_UI+11,261,2200,
		TYPE_SP_HEAL,
		3,3 ,1),
	CUseItemData(
		"�Ζ�",
		ID_UI+12,262,30,
		TYPE_SMALL_BOMB,
		100,0 ,3),
	CUseItemData(
		"���e��",
		ID_UI+13,263,150,
		TYPE_SMALL_BOMB,
		300,0 ,3),
	CUseItemData(
		"�唚��",
		ID_UI+14,272,450,
		TYPE_BIG_BOMB,
		1000,0 ,2),
	CUseItemData(
		"���唚��",
		ID_UI+15,273,2500,
		TYPE_BIG_BOMB,
		5000,0 ,1),
	CUseItemData(
		"",
		ID_UI+16,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"",
		ID_UI+17,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"",
		ID_UI+18,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"",
		ID_UI+19,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"�I�I�_�e�n�X",
		ID_UI+20,269,400,
		TYPE_INSTANT_BARRIER,
		3,0 ,1),
	CUseItemData(
		"�T�{�e��",
		ID_UI+21,270,250,
		TYPE_SABOTEN,
		20,0 ,3),
	CUseItemData(
		"���g�킩��",
		ID_UI+22,271,800,
		TYPE_WAKAME,
		5,0 ,2),
	CUseItemData(
		"�K���X�̉�",
		ID_UI+23,274,1000,
		TYPE_GLASS,
		5,0 ,1),
	CUseItemData(
		"",
		ID_UI+24,277,10,
		TYPE_DMG_BOOST,
		3,0,1),
	CUseItemData(
		"�C�\�M���`���N",
		ID_UI+25,276,10,
		TYPE_GUARD_BREAK,
		5000,0,5),
	CUseItemData(
		"�M���̎�",
		ID_UI+26,275,10,
		TYPE_ESCAPE,
		0,0,2),
	CUseItemData(
		"",
		ID_UI+27,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"�������[�t",
		ID_UI+28,267,100,
		TYPE_SAVE,
		0,0 ,5),
	CUseItemData(
		"",
		ID_UI+29,266,50,
		TYPE_CLOSET,
		0,0 ,5),
	CUseItemData(
		"",
		ID_UI+30,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
	CUseItemData(
		"",
		ID_UI+31,NO_ICON,10,
		TYPE_LP_HEAL,
		10,0),
//�a����
	CUseItemData(
		"�K�[�l�b�g",
		ID_BS+0,288,300,
		TYPE_BIRTH_STONE,
		0,0),
	CUseItemData(
		"�A���W�X�g",
		ID_BS+1,289,500,
		TYPE_BIRTH_STONE,
		1,0),
	CUseItemData(
		"�A�N�A�}����",
		ID_BS+2,290,800,
		TYPE_BIRTH_STONE,
		2,0),
	CUseItemData(
		"�_�C�A�����h",
		ID_BS+3,291,1000,
		TYPE_BIRTH_STONE,
		3,0),
	CUseItemData(
		"�G�������h",
		ID_BS+4,292,1500,
		TYPE_BIRTH_STONE,
		4,0),
	CUseItemData(
		"�p�[��",
		ID_BS+5,293,2000,
		TYPE_BIRTH_STONE,
		5,0),
	CUseItemData(
		"���r�[",
		ID_BS+6,294,2500,
		TYPE_BIRTH_STONE,
		6,0),
	CUseItemData(
		"�y���h�b�g",
		ID_BS+7,295,3000,
		TYPE_BIRTH_STONE,
		7,0),
	CUseItemData(
		"�T�t�@�C�A",
		ID_BS+8,296,3500,
		TYPE_BIRTH_STONE,
		8,0),
	CUseItemData(
		"�g���}����",
		ID_BS+9,297,4000,
		TYPE_BIRTH_STONE,
		9,0),
	CUseItemData(
		"�g�p�[�Y",
		ID_BS+10,298,5000,
		TYPE_BIRTH_STONE,
		10,0),
	CUseItemData(
		"���s�X���Y��",
		ID_BS+11,299,10000,
		TYPE_BIRTH_STONE,
		11,0),
	CUseItemData(
		"�ւ�Ȑ΂���",
		ID_BS+12,300,10,
		TYPE_BIRTH_STONE,
		12,0),
	CUseItemData(
		"���K�[�l�b�g",
		ID_BS+13,288,2000,
		TYPE_BIRTH_STONE,
		0,1),
	CUseItemData(
		"���A���W�X�g",
		ID_BS+14,289,4000,
		TYPE_BIRTH_STONE,
		1,1),
	CUseItemData(
		"���A�N�A�}����",
		ID_BS+15,290,8000,
		TYPE_BIRTH_STONE,
		2,1),
	CUseItemData(
		"���_�C�A�����h",
		ID_BS+16,291,12000,
		TYPE_BIRTH_STONE,
		3,1),
	CUseItemData(
		"���G�������h",
		ID_BS+17,292,16000,
		TYPE_BIRTH_STONE,
		4,1),
	CUseItemData(
		"���p�[��",
		ID_BS+18,293,24000,
		TYPE_BIRTH_STONE,
		5,1),
	CUseItemData(
		"�����r�[",
		ID_BS+19,294,32000,
		TYPE_BIRTH_STONE,
		6,1),
	CUseItemData(
		"���y���h�b�g",
		ID_BS+20,295,40000,
		TYPE_BIRTH_STONE,
		7,1),
	CUseItemData(
		"���T�t�@�C�A",
		ID_BS+21,296,48000,
		TYPE_BIRTH_STONE,
		8,1),
	CUseItemData(
		"���g���}����",
		ID_BS+22,297,60000,
		TYPE_BIRTH_STONE,
		9,1),
	CUseItemData(
		"���g�p�[�Y",
		ID_BS+23,298,72000,
		TYPE_BIRTH_STONE,
		10,1),
	CUseItemData(
		"�����s�X���Y��",
		ID_BS+24,299,1000000,
		TYPE_BIRTH_STONE,
		11,1),
//�J�M
	CUseItemData(
		"���̃J�M",
		ID_KEY+0,301,1000,
		TYPE_KEY,
		0,0),
	CUseItemData(
		"�̃J�M",
		ID_KEY+1,302,1000,
		TYPE_KEY,
		1,0),
	CUseItemData(
		"���̃J�M",
		ID_KEY+2,303,1000,
		TYPE_KEY,
		2,0),
	CUseItemData(
		"�΂̃J�M",
		ID_KEY+3,304,1000,
		TYPE_KEY,
		0,0),
	CUseItemData(
		"���̃J�M",
		ID_KEY+4,305,1000,
		TYPE_KEY,
		0,0),
	CUseItemData(
		"��̃J�M",
		ID_KEY+5,306,1000,
		TYPE_KEY,
		0,0),
	CUseItemData(
		"�Ԃ̃J�M",
		ID_KEY+6,311,1000,
		TYPE_KEY,
		0,0),
	CUseItemData(
		"",
		ID_KEY+7,312,1000,
		TYPE_KEY,
		0,0),
//�����A�C�e��
	CUseItemData(
		"500 M",
		ID_MED+0,307,500,
		TYPE_MONEY ,
		0,0),
	CUseItemData(
		"3000 M",
		ID_MED+1,308,3000,
		TYPE_MONEY ,
		0,0),
	CUseItemData(
		"10000 M",
		ID_MED+2,309,10000,
		TYPE_MONEY ,
		0,0),
	CUseItemData(
		"30000 M",
		ID_MED+3,310,30000,
		TYPE_MONEY ,
		0,0),
//���̑��̃A�C�e��
	CUseItemData(
		"���",
		ID_OTH+0,277,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"�݂���",
		ID_OTH+1,278,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"����",
		ID_OTH+2,279,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"��������",
		ID_OTH+3,280,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"�Ԃǂ�",
		ID_OTH+4,281,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"����",
		ID_OTH+5,282,10,
		TYPE_NO_USE,
		10 ,0 ,1),
	CUseItemData(
		"����̃J�M",
		ID_OTH+6,283,1000,
		TYPE_RARE,
		10,0 ,1),

	CUseItemData(
		"�l�Y�~�̃����[�t",
		ID_OTH+7,313,0,
		TYPE_RARE,
		10,0 ,1),
	CUseItemData(
		"�X�Y���̃����[�t",
		ID_OTH+8,314,0,
		TYPE_RARE,
		10,0 ,1),
	CUseItemData(
		"�L���M���̃����[�t",
		ID_OTH+9,315,0,
		TYPE_RARE,
		10,0 ,1),
	CUseItemData(
		"�J���̃����[�t",	//���O��ς���ꍇ�Aevent608���ύX���邱�ƁI
		ID_OTH+10,316,0,
		TYPE_RARE,
		10,0 ,1),

	CUseItemData(
		"���̂�����",
		ID_OTH+11,317,0,
		TYPE_RARE,
		0,0 ,1),


};
const CSeedData		SeedData[MAX_SEED_NUMBER]={
	CSeedData(ID_SD+0,0),
	CSeedData(ID_SD+1,0),
	CSeedData(ID_SD+2,0),
	CSeedData(ID_SD+3,0),
	CSeedData(ID_SD+4,0),
	CSeedData(ID_SD+5,0),
	CSeedData(ID_SD+6,0),
	CSeedData(ID_SD+7,0),
	CSeedData(ID_SD+8,0),
	CSeedData(ID_SD+9,0),
	CSeedData(ID_SD+10,0),
	CSeedData(ID_SD+11,0),
	CSeedData(ID_SD+12,0),
	CSeedData(ID_SD+13,0),
	CSeedData(ID_SD+14,0),
	CSeedData(ID_SD+15,0),
	CSeedData(ID_SD+16,0),
	CSeedData(ID_SD+17,0),
	CSeedData(ID_SD+18,0),
	CSeedData(ID_SD+19,0),
	CSeedData(ID_SD+20,0),
	CSeedData(ID_SD+21,0),
	CSeedData(ID_SD+22,0),
	CSeedData(ID_SD+23,0),
	CSeedData(ID_SD+24,0),
	CSeedData(ID_SD+25,0),
	CSeedData(ID_SD+26,0),
	CSeedData(ID_SD+27,0),
	CSeedData(ID_SD+28,0),
	CSeedData(ID_SD+29,0),
	CSeedData(ID_SD+30,0),
	CSeedData(ID_SD+31,0)
};
const CItemCategoryData 		ItemCategory[CATEGORY_MAX]=
{
	CItemCategoryData("���ׂ�"				,0	,0		,MAX_ITEM_NUMBER),
	CItemCategoryData("���� - ��"			,1	,ID_SWD	,ID_SPR),
	CItemCategoryData("���� - ��"			,2	,ID_SPR	,ID_AXE),
	CItemCategoryData("���� - ��"			,3	,ID_AXE	,ID_BOW),
	CItemCategoryData("���� - �|"			,4	,ID_BOW	,ID_WND),
	CItemCategoryData("���� - ��E���̑�"	,5	,ID_WND	,ID_AMR),
	CItemCategoryData("�h��"				,6	,ID_AMR	,ID_ACC),
	CItemCategoryData("�����i - ��"			,7	,ID_SLD	,ID_HED),
	CItemCategoryData("�����i - �X�q"		,8	,ID_HED	,ID_GLV),
	CItemCategoryData("�����i - ���"		,9	,ID_GLV	,ID_SHE),
	CItemCategoryData("�����i - �C"			,10	,ID_SHE	,ID_AME),
	CItemCategoryData("�����i - �아"		,11	,ID_AME	,ID_UI),
	CItemCategoryData("�a����"				,12	,ID_UI	,ID_KEY),
	CItemCategoryData("�J�M�E���̑�"		,13	,ID_KEY	,MAX_ITEM_NUMBER)
};
