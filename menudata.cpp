#include "menudata.h"
#include "math.h"
#include "flag.h"
#include "itemdata.h"

int						CMenuData::mBGTimer=MENU_BG_TIME*2+1;
bool					CMenuData::mDrawBG=true;
CCharaDataWindowMenu	CMenuData::mCDataWindow[MAX_CHARACTER_NUMBER+MAX_PARTY_NUMBER];
CMenuData::CMenuData(void)
{
	mNext=NULL;
	mDeleteTimer=0;
	mUpdate=true;
}

CMenuData::~CMenuData(void)
{
}

bool	CMenuData::OnTimer(void)
{
	if(mDeleteTimer>0){
		mDeleteTimer++;
		if(mNext)	mNext->OnTimer();
		mUpdate=false;
		return false;
	}
	while(mNext){
		if(mNext->mDeleteTimer>WINDOW_OPEN_TIME){
			CMenuData* del=mNext;
			mNext=mNext->mNext;
			delete del;
		}else{
			mNext->OnTimer();
			if(!(mNext->mDeleteTimer>1)){
				mUpdate=false;
				return false;
			}
			break;
		}
	}

	if(!mUpdate){
		Update();
		mUpdate=true;
	}

	return true;
}
void	CMenuData::OnDraw(void)
{
	CMenuData* mdata=mNext;
	if(mdata){
		bool bg=false;
		bool bgdrawd=false;
		if(mdata->mDeleteTimer>0){
			bg=true;
		}
		while(mdata){
			if(!mdata->mNext || mdata->mNext->mDeleteTimer>0){
				if(!bgdrawd){
					DrawBG(bg);
					bgdrawd=true;
				}
			}
			mdata->OnDraw();
			mdata=mdata->mNext;
		}
	}else{
		DrawBG(true);
	}
}
void	CMenuData::Update(void)
{
}
void	CMenuData::Destroy(void)
{
	if(mDeleteTimer<=0)	mDeleteTimer=1;
}

void	CMenuData::DrawBG(bool time)
{
	if(!mDrawBG)	return;
	if(time && mBGTimer<MENU_BG_TIME){
		mBGTimer=MENU_BG_TIME*2-mBGTimer;
	}
	if(!time && mBGTimer>MENU_BG_TIME){
		if(mBGTimer>=MENU_BG_TIME*2){
			mBGTimer=0;
		}else if(mBGTimer>MENU_BG_TIME){
			mBGTimer=MENU_BG_TIME*2-mBGTimer;
		}
	}
	if(mBGTimer<MENU_BG_TIME || time){
		mBGTimer++;
	}

	if(mBGTimer==MENU_BG_TIME){
		DTest.DrawBox(0,0,640,480,MENU_BG_COLOR);
	}else if(mBGTimer<=MENU_BG_TIME*2){
		for(int x=0;x<640/MENU_BG_SIZE;x++){
			for(int y=0;y<480/MENU_BG_SIZE;y++){
				int time=MENU_PIECE_TIME;
				if(mBGTimer<MENU_BG_TIME){
					time=mBGTimer-(x+y)/2;
				}else if(mBGTimer>MENU_BG_TIME){
					time=MENU_BG_TIME*2-mBGTimer-(x+y)/2;
				}
				
				if(time<MENU_PIECE_TIME){
					if(time<0)	continue;
					int s=int(sin(3.141592/2*time/MENU_PIECE_TIME)*MENU_BG_SIZE/2);
					DTest.DrawBox(	(x*2+1)*MENU_BG_SIZE/2-s,
									(y*2+1)*MENU_BG_SIZE/2-s,
									(x+1)*MENU_BG_SIZE,
									y*MENU_BG_SIZE,
									x*MENU_BG_SIZE,
									(y+1)*MENU_BG_SIZE,
									(x*2+1)*MENU_BG_SIZE/2+s,
									(y*2+1)*MENU_BG_SIZE/2+s,
									MENU_BG_COLOR,
									MENU_BG_COLOR,
									MENU_BG_COLOR,
									MENU_BG_COLOR);
				}else{
					DTest.DrawBox(	x*MENU_BG_SIZE,
									y*MENU_BG_SIZE,
									(x+1)*MENU_BG_SIZE,
									(y+1)*MENU_BG_SIZE,
									MENU_BG_COLOR);
				}
			}
		}
	} 
}


void	CMenuData::InitItemList(int character,CItemListWindow* window,int x,int y,int h)
{
	CPlayerData &cdata=Flag.mPlayerData[character];
	InitItemList(cdata,window,x,y,h);
}

void	CMenuData::InitItemList(CPlayerData &cdata,CItemListWindow* window,int x,int y,int h)
{
	InitItemList(cdata,(CListWindow*)window,x,y,h);
	window->SetPData(cdata);
}
void	CMenuData::InitItemList(int character,CListWindow* window,int x,int y,int h)
{
	CPlayerData &cdata=Flag.mPlayerData[character];
	InitItemList(cdata,window,x,y,h);
}
void	CMenuData::InitItemList(CPlayerData &cdata,CListWindow* window,int x,int y,int h)
{
	int maxitem;
	int itemlist[MAX_ITEM];
	int param[MAX_ITEM];

	GetItemListInfo(cdata,&maxitem,itemlist,param);

	if(h<0 || maxitem<h){
		h=maxitem;
	}

	window->InitListWindow(	x,y,ITEM_LIST_WIDTH,
								(const CElement**)AllItem,itemlist,
								maxitem,h,param);
}
void	CMenuData::UpdataItemList(int character,CListWindow* window)
{
	CPlayerData &cdata=Flag.mPlayerData[character];
	UpdataItemList(cdata,window);
}
void	CMenuData::UpdataItemList(CPlayerData &cdata,CListWindow* window)
{
	int maxitem;
	int itemlist[MAX_ITEM];
	int param[MAX_ITEM];

	GetItemListInfo(cdata,&maxitem,itemlist,param);

	window->ResetList((const CElement**)AllItem,itemlist,maxitem,param);
}
void	CMenuData::GetItemListInfo(CPlayerData &cdata,int* maxitem,int* itemlist,int* param)
{
	*maxitem=cdata.GetItemMax();

	memset(param,0,sizeof(int)*MAX_ITEM);

	for(int i=0;i<*maxitem;i++){
		CarryItem citem = cdata.GetItem(i);
		itemlist[i]=citem.item;
		param[i] = citem.param * 0x10000 + 1024;
		if(cdata.CheckEquiped(i)){
			param[i]=1;
		}
	}
	int secwep=cdata.GetSecondWeaponNo();
	if(secwep>=0){
		param[secwep]=256;
	}
}
void	CMenuData::InitClosetList(CListWindow* window,int x,int y,int category,int h,bool none,bool pagemove)
{
	int itemno=0;
	int itemlist[MAX_ITEM_NUMBER+1];
	int param[MAX_ITEM_NUMBER+1];

	int min=0;
	int max=MAX_ITEM_NUMBER;
	if(category>=0){
		min=ItemCategory[category].mMin;
		max=ItemCategory[category].mMax;
	}
	if(none){
		itemlist[0]=NO_ITEM;
		itemno++;
	}
	memset(param,0,sizeof(param));

	for(int i=min;i<max;i++){
		if(Flag.mCloset[i]>0){
			itemlist[itemno]=i;
			param[itemno]=32;
			itemno++;
		}
	}
	if(itemno<=0){
		itemlist[0]=NO_ITEM;
		itemno++;
	}
	window->InitListWindow(	x,y,ITEM_LIST_WIDTH,
								(const CElement**)AllItem,itemlist,
								itemno,h,param,pagemove);
}
void	CMenuData::UpdataClosetList(CListWindow* window,int category,bool none)
{
	int itemno=0;
	int itemlist[MAX_ITEM_NUMBER+4];
	int param[MAX_ITEM_NUMBER+4];

	if(none){
		itemlist[0]=NO_ITEM;
		itemno++;
	}
	memset(param,0,sizeof(param));

	for(int cat=0;cat<CATEGORY_MAX;cat++){
		if(!(category & (1<<cat)))	continue;	
		int min=ItemCategory[cat].mMin;
		int max=ItemCategory[cat].mMax;

		for(int i=min;i<max;i++){
			if(Flag.mCloset[i]>0){
				itemlist[itemno]=i;
				param[itemno]=32;
				itemno++;
			}
		}
	}
	if(itemno<=0){
		itemlist[0]=NO_ITEM;
		itemno++;
	}

	window->ResetList((const CElement**)AllItem,itemlist,itemno,param);
}
char*	CMenuData::GetHelpText(const char* fname,int no)
{
const int BUF_SIZE	= 1024;
	static char text[BUF_SIZE];
	FILE *pFile;
	no%=32;
	if(pFile=fopen(fname,"rb")){
		bool repeat=true;
		int n=0;
		int line=0;
		while(1){
			text[n]=getc(pFile);
			if(text[n]==EOF){
				sprintf(text,"\n準備中です。。。");
				break;
			}
			if(text[n]==42){
				if (line==no){
					text[n]=0;
					break;
				} else {
					line++;
					n=0;
				}
			} else {
				n++;
				if(n>=BUF_SIZE){
					sprintf(text,"\n準備中です。。。。");
					break;
				}
			}
		}
		fclose(pFile);
	}else{
		sprintf(text,"\n準備中です。。。。。");
	}
	int size=int(strlen(text));
	char* ret=new char[size+1];
	strncpy(ret,text,size+1);
	return ret;
}
char*	CMenuData::GetHelpTextShort(const char* fname,int no)
{
const int BUF_SIZE	= 1024;
	static char text[BUF_SIZE];
	FILE *pFile;
	no%=32;
	if(pFile=fopen(fname,"r")){
		bool repeat=true;
		int newline=false;
		int n=0;
		int line=0;
		while(1){
			text[n]=getc(pFile);
			if(text[n]==EOF){
				sprintf(text,"\n準備中です。。。");
				break;
			}
			if(text[n]==42){
				if (line==no){
					text[n]=0;
					break;
				} else {
					line++;
					n=0;
				}
			}else if(text[n]=='\n'){
				if (line==no){
					if (newline){
						text[n]=0;
						break;
					} else {
						newline=true;
					}
				}
				n++;
			} else {
				n++;
				if(n>=BUF_SIZE){
					sprintf(text,"\n準備中です。。。。");
					break;
				}
				newline=false;
			}
		}
		fclose(pFile);
	}else{
		sprintf(text,"\n準備中です。。。。。");
	}
	int size=int(strlen(text));
	char* ret=new char[size+1];
	strncpy(ret,text,size+1);
	return ret;
}
char*	CMenuData::GetItemTextShort(int item,bool newline)
{
	if(item==NO_ITEM){
		return NULL;
	}
	if(item>ID_SD)	item=ID_SD;

	char fname[256];
	sprintf(fname,"data\\help\\item%02d",item/32);

	char* text=GetHelpTextShort(fname,item);
	int i=0;
	while(text[i]!=0){
		if(text[i]=='\n' && !newline){
			text[i]=' ';
		}
		i++;
	}
	return text;
}
char*	CMenuData::GetItemText(int no)
{
	if(no>ID_SD)	no=ID_SD;

	char fname[256];
	sprintf(fname,"data\\help\\item%02d",no/32);
	return GetHelpText(fname,no);
}

void	CMenuData::InitReserveCDataWin()
{
	for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
		mCDataWindow[i+MAX_PARTY_NUMBER].InitWindow(650,380,
													Flag.mParty[i+MAX_PARTY_NUMBER],true);
	}
}
void	CMenuData::AllDelete(void)
{
	CMenuData* del;
	CMenuData* mdata=mNext;
	while(mdata){
		del=mdata;
		mdata=mdata->mNext;
		del->Destroy();
	}
}
void	CMenuData::Delete(const type_info& info)
{
	CMenuData* mdata=this;
	while(mdata->mNext){
		if(info == typeid(*(mdata->mNext))){
			mdata->AllDelete();
			return ;
		}
		mdata=mdata->mNext;
	}
}
CMenuData* CMenuData::SearchMenu(const type_info& info)
{
	CMenuData* mdata=this;
	while(mdata){
		if(info == typeid(*(mdata))){
			return mdata;
		}
		mdata=mdata->mNext;
	}
	return NULL;
}


void	CMenuData::SetMenu(EMenuName menu,int param)
{
	PlayWave(SE_YES);

	if(mBGTimer>=MENU_BG_TIME*2){
		mBGTimer=0;
	}else if(mBGTimer>MENU_BG_TIME){
		mBGTimer=MENU_BG_TIME*2-mBGTimer;
	}
 
	CMenuData* mdata=this;
	while(mdata->mNext){
		mdata=mdata->mNext;
	}

	switch(menu){
		case StartMenu:
			mdata->mNext= new CStartMenu(param);				break;
		case ItemMenu:
			mdata->mNext= new CItemMenu(param);					break;
		case ItemSubMenu:
			mdata->mNext= new CItemSubMenu(param);				break;
		case ExchangeItem:
			mdata->mNext= new CExchangeItem(param);				break;
		case DumpItem:
			mdata->mNext= new CDumpItem(param);					break;
		case UISelectCharacter:
			mdata->mNext= new CUIMenuSelectCharacter(param);	break;
		case BirthStoneMenu:
			mdata->mNext= new CBirthStoneMenu(param);			break;
		case LearnSkillMenu:
			mdata->mNext= new CLearnSkillMenu(param);			break;
		case LearnSkillYesNo:
			mdata->mNext= new CLearnSkillYesNo(param);			break;
		case StatusMenu:
			mdata->mNext= new CStatusMenu(param);				break;
		case FormationMenu:
			mdata->mNext= new CFormationMenu(param);			break;
		case BoxStartMenu:
			mdata->mNext= new CBoxStartMenu(param);				break;
		case BoxSelectMenu:
			mdata->mNext= new CBoxSelectMenu(param);			break;
		case BoxOpenMenu:
			mdata->mNext= new CBoxOpenMenu(param);				break;
		case OverItemMenu:
			mdata->mNext= new COverItemMenu(param);				break;
		case ItemHelp:
			mdata->mNext= new CItemHelp(param);					break;
		case EquipmentHelp:
			mdata->mNext= new CEquipmentHelp(param);			break;
		case SkillHelp:
			mdata->mNext= new CSkillHelp(param);				break;
		case ComSkillHelp:
			mdata->mNext= new CComSkillHelp(param);				break;
		case EventSkip:
			mdata->mNext= new CEventSkipMenu(param);			break;
		case ShopMenu:
			mdata->mNext= new CShopMenu(param);					break;
		case FleaMarketMenu:
			mdata->mNext= new CFleaMarketMenu(param);			break;
		case FarmMenu:
			mdata->mNext= new CFarmMenu(param);					break;
		case ReturnVillage:
			mdata->mNext= new CReturnVillage(param);			break;
		case DiaryMenu:
			mdata->mNext= new CDiaryMenu(param);				break;
		case ClosetMenu:
			mdata->mNext= new CClosetMenu(param);				break;
		case MonthStartMenu:
			mdata->mNext= new CMonthStartMenu(param);			break;
		case ChangeMemberMenu:
			mdata->mNext= new CChangeMemberMenu(param);			break;
		case BattleStatusMenu:
			mdata->mNext= new CBattleStatusMenu(param);			break;
		case ConfigMenu:
			mdata->mNext= new CConfigMenu(param);				break;
		case SellMenu:
			mdata->mNext= new CSellMenu(param);					break;
		case SellYesNoMenu:
			mdata->mNext= new CSellYesNoMenu(param);			break;
		case FreeSpaceMenu:
			mdata->mNext= new CFreeSpaceMenu(param);			break;
		case FSBirthStone:
			mdata->mNext= new CFSBirthStoneMenu(param);			break;
		case FSSelectSkill:
			mdata->mNext= new CFSSelectSkillMenu(param);		break;
		case GlobalConfigMenu:
			mdata->mNext= new CGlobalConfigMenu(param);			break;
		case KeyConfigMenu:
			mdata->mNext= new CKeyConfigMenu(param);			break;		
		case SeedMenu:
			mdata->mNext= new CSeedMenu(param);					break;		
		case DInfoMenu:
			mdata->mNext= new CDInfoMenu(param);				break;		
		case BypathMenu:
			mdata->mNext= new CBypathMenu(param);				break;		
		case StatusHelp:
			mdata->mNext= new CStatusHelp(param);				break;		
		case RemoveBirthStone:
			mdata->mNext= new CRemoveBirthStoneMenu(param);		break;		
		case RemoveBSPersonal:
			mdata->mNext= new CRemoveBSPersonal(param);			break;		
		case SelectDifficult:
			mdata->mNext= new CSelectDifficult(param);			break;	
		case ExitYesNo:
			mdata->mNext= new CExitYesNo(param);				break;			
		case SuccessSaveData:
			mdata->mNext= new CSuccessSaveDataMenu(param);		break;			
		case CallPetHelp:
			mdata->mNext= new CCallPetHelp(param);				break;			
		case TutorialMenu:
			mdata->mNext= new CTutorialMenu(param);				break;		
		case SoundTestMenu:
			mdata->mNext= new CSoundTestMenu(param);			break;
		case PreciousItemMenu:
			mdata->mNext= new CPreciousItemMenu(param);			break;
	}	
}
