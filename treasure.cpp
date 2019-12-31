#include "treasure.h"
#include "flag.h"

const int K0 = 1;
const int K1 = 2;
const int K2 = 4;
const int K3 = 8;
const int K4 = 16;
const int K5 = 32;
const int K6 = 64;
const int K7 = 128;

CTreasureData::CTreasureData(void)
{
	mItem=NULL;
}
CTreasureData::CTreasureData(int key,int trap,int traplv,int explv,int item,...)
{
	mKey=key;
	mTrap=trap;
	mTrapLv=traplv;
	mExpLv=explv;

	static int allitem[16];
	int i=0;
	va_list marker;

	va_start( marker, item ); 
	while(item>=0 && i<TEMP_ITEM_MAX)
	{
		allitem[i]=item;
		i++;
		item = va_arg( marker, int);
	}
	va_end( marker );

	mItemNo=i;
	mItem		= new int[i];

	memcpy(mItem,allitem,sizeof(int)*i);
}
CTreasureData::~CTreasureData(void)
{
	delete[] mItem;
}
int	CTreasureData::GetTrapLv(void)const
{
	int lv=mTrapLv;
	switch(TrapData[mTrap].mParam){
		case TRAP_DEF:
		case TRAP_MDEF:
			lv=mTrapLv*2;
		default:
			if(lv<10)		return 1;
			if(lv<20)		return 2;
			if(lv<40)		return 3;
			if(lv<60)		return 4;
			if(lv<100)		return 5;
			if(lv<150)		return 6;
			if(lv<200)		return 7;
			if(lv<300)		return 8;
			if(lv<500)		return 9;
			return 10;

		case TRAP_SPD:
			if(lv<20)		return 1;
			if(lv<30)		return 2;
			if(lv<40)		return 3;
			if(lv<60)		return 4;
			if(lv<80)		return 5;
			if(lv<100)		return 6;
			if(lv<120)		return 7;
			if(lv<150)		return 8;
			if(lv<200)		return 9;
			return 10;

		case TRAP_TECH:
			if(lv<3)		return 1;
			if(lv<6)		return 2;
			if(lv<12)		return 3;
			if(lv<20)		return 4;
			if(lv<30)		return 5;
			if(lv<50)		return 6;
			if(lv<80)		return 7;
			if(lv<100)		return 8;
			if(lv<130)		return 9;
			return 10;

		case TRAP_LP:
			if(lv<50)		return 1;
			if(lv<100)		return 2;
			if(lv<200)		return 3;
			if(lv<400)		return 4;
			if(lv<800)		return 5;
			if(lv<1600)		return 6;
			if(lv<3200)		return 7;
			if(lv<4800)		return 8;
			if(lv<6400)		return 9;
			return 10;

		case TRAP_IMP:
			return 10;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
CTrapData::CTrapData()
{
	mName=NULL;
	mHelp=NULL;
}
CTrapData::CTrapData(char* name,int param,bool item,char* help)
{
	mName=name;
	mHelp=help;
	mParam=param;
	mGetItem=item;
}

/////////////////////////////////////////////////////////////////////////////
const CTrapData		TrapData[TRAP_NO]={
	CTrapData("弓矢",
				TRAP_SPD,	true,
				"機械仕掛けの弓矢。\n開けた者のLPを、現在値の 10% 減少させる。"),
	CTrapData("強化弓",
				TRAP_SPD,	true,
				"強力な機械仕掛けの弓矢。\n開けた者のLPを、現在値の 50% 減少させる。"),
	CTrapData("爆弾",
				TRAP_TECH,	true,
				"動作感知型の爆弾。\n全員のLPを、現在値の 5% 減少させる。"),
	CTrapData("強化爆弾",
				TRAP_TECH,	true,
				"強力な爆弾。\n全員のLPを、現在値の 30% 減少させる。"),
//4
	CTrapData("テレポーター（宝）",
				TRAP_MATK,	false,
				"対物型の転送装置。\n中の宝をどこかへ転送させる。"),
	CTrapData("テレポーター（人）",
				TRAP_MDEF,	false,
				"対人型の転送装置。\n村に強制送還させられる。"),
	CTrapData("鈍足ガス",
				TRAP_TECH,	true,
				"神経に作用するガス。\nマップが変わるまで移動速度が半減する。"),
	CTrapData("混乱ガス",
				TRAP_TECH,	true,
				"神経に作用するガス。\nマップが変わるまで移動方向が 180° 回転する。"),
//8
	CTrapData("トラバサミ",
				TRAP_DEF,	true,
				"腕をつかんで離さない罠。\n 6 秒間、移動不能になる。"),
	CTrapData("金食い虫",
				TRAP_ATK,	true,
				"お金を好む虫。\n所持金の半分を奪われる。"),
	CTrapData("電気ショック",
				TRAP_LP,	true,
				"宝箱に流れる強力な電気。\n開けたもののSPを、3/4にする。"),
	CTrapData("高圧電流",
				TRAP_LP,	true,
				"宝箱に流れる壮絶な電気。\n開けたもののSPを、半分にする。"),
//12
};
/////////////////////////////////////////////////////////////////////////////
const CTreasureData TreasureData[TREASURE_NO]={
//ラクリー街道
	//0
	CTreasureData(	1,-1,0,18,
					ID_MED+1,ID_BS+3,ID_BS+2,-1),		//ラクリア山岳
	CTreasureData(	0,8,40,18,
					ID_HED+3,ID_BS+3,-1),				//ラクリア山岳
	CTreasureData(	1,-1,0,2,
					ID_SHE,ID_BS,ID_BS,-1),
	CTreasureData(	0,0,150,5,
					ITEM_KEY,-1),
	CTreasureData(	0,0,10,4,
					ID_GLV+0,ID_BS,-1),
	//5
	CTreasureData(	0,-1,0,3,
					ID_SLD+1,ID_BS,-1),		
	CTreasureData(	1,-1,0,3,
					ID_AXE+WLV_0+1,-1),					
	CTreasureData(	0,-1,0,3,
					ITEM_KEY,ID_BS,-1),
	CTreasureData(	0,-1,0,4,
					ID_HED+0,ID_BS,-1),
	CTreasureData(	0,-1,0,4,
					ID_MED+0,ID_BS,-1),
	//10
	CTreasureData(	0,-1,0,5,
					ID_BS,ID_BS,-1),
	CTreasureData(	0,-1,0,5,
					ID_AMR+ALV_0+1,-1),
//村近くの洞窟
	CTreasureData(	0,-1,0,8,
					ID_BS+13,ID_BS+0,ID_BS+0,-1),	
	CTreasureData(	0,-1,0,4,
					ID_BS+1,ID_BS+1,-1),	
	CTreasureData(	0,0,40,4,
					ID_BS+1,ID_BS+0,ID_BS+0,-1),						
	//15
	CTreasureData(	0,-1,0,5,
					ID_KEY+1,ID_BS+1,ID_BS+1,-1),					
	CTreasureData(	0,-1,0,5,
					ID_HED+1,ID_BS+1,ID_BS+0,ID_BS+0,-1),			
	CTreasureData(	K1,6,4,5,
					ID_GLV+3,ID_BS+1,-1),						
	CTreasureData(	0,2,50,6,
					ID_WND+WLV_0+1,ID_BS+1,ID_BS+0,ID_BS+0,-1),	
	CTreasureData(	0,0,100,6,
					ID_KEY+1,ID_BS+1,ID_BS+1,-1),
	//20
	CTreasureData(	0,-1,30,6,
					ID_SD+20,-1),										
	CTreasureData(	0,-1,0,10,
					ID_BS+14,ID_BS+1,ID_BS+1,-1),	
	CTreasureData(	0,6,6,8,
					ID_GLV+1,ID_BS+1,ID_BS+0,ID_BS+0,-1),	
	CTreasureData(	0,-1,50,10,
					ID_SD+2,-1),								
	CTreasureData(	0,-1,0,15,
					ID_BS+3,ID_BS+2,-1),				//ラクリア山岳
	//25
	CTreasureData(	K1,-1,0,5,							
					ID_AME+5,ID_BS+1,ID_BS+0,-1),								
	CTreasureData(	0,-1,0,6,
					ID_SD+12,ID_SD+12,-1),						
//ラクリア山岳
	CTreasureData(	K0,-1,0,12,
					ID_BS+3,ID_BS+2,ID_BS+1,ID_BS,-1),
	CTreasureData(	K2,6,12,12,
					ID_AME+1,ID_BS+3,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	K1,-1,0,12,
					ID_AME+11,ID_AME+14,ID_BS+3,ID_BS+2,-1),
	//30
	CTreasureData(	0,0,30,12,
					ID_SD+1,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	0,-1,0,12,
					ID_SD+4,ID_SD+6,ID_SD+6,-1),
	CTreasureData(	0,-1,0,12,
					ID_GLV+2,-1),	
	CTreasureData(	K1,2,15,12,
					ID_KEY+2,ID_BS+3,ID_BS+3,-1),
	CTreasureData(	K0,0,80,12,
					ID_SLD+6,ID_BS+3,ID_BS+3,-1),
	//35
	CTreasureData(	0,0,30,18,
					ID_MED+0,ID_SD+13,ID_SD+6,-1),
	CTreasureData(	K0,1,40,18,
					ID_SLD+7,ID_BS+2,ID_BS+2,ID_BS+1,ID_BS+1,-1),
	CTreasureData(	K1,3,1000,18,
					ID_BS+16,ID_BS+3,ID_BS+3,-1),
	CTreasureData(	K0,-1,0,12,
					ID_SD+13,ID_SD+6,ID_SD+6,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	K2,2,6,12,
					ID_AMR+ALV_0+3,ID_BS+3,ID_BS+2,ID_BS+2,-1),
	//40
	CTreasureData(	0,-1,0,25,
					ID_SPR+WLV_3+2,ID_SD+7,-1),
	CTreasureData(	1,4,40,18,
					ID_AME+3,ID_BS+2,ID_BS+1,ID_BS+0,-1),
	CTreasureData(	1,2,15,18,
					ID_MED+0,ID_SD+8,-1),
	CTreasureData(	0,6,8,18,
					ID_BS+15,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	1,6,12,18,
					ID_SWD+WLV_1+2,ID_BS+1,ID_BS+1,-1),
	//45
//地下帝国
	CTreasureData(	0,3,200,30,
					ID_AME+9,ID_AME+10,ID_BS+7,ID_BS+7,-1),
	CTreasureData(	K4,-1,15,30,
					ID_SPR+WLV_3+1,ID_BS+7,ID_SD+25,-1),
	CTreasureData(	K4,-1,15,30,
					ID_SWD+WLV_3+2,ID_BS+7,ID_SD+4,-1),//
	CTreasureData(	0,-1,15,30,
					ID_BOW+WLV_3+2,ID_BS+7,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,-1,15,30,
					ID_SLD+9,ID_BS+7,ID_BS+6,ID_BS+5,-1),
	//50
	CTreasureData(	0,2,35,30,
					ID_BS+19,ID_BS+6,-1),
	CTreasureData(	0,-1,15,40,
					ID_KEY+4,ID_BS+4,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	0,-1,15,30,
					ID_HED+9,ID_BS+4,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	0,-1,15,30,
					ID_SD+26,ID_SD+2,ID_SD+8,-1),
	CTreasureData(	0,2,30,30,
					ID_MED+2,ID_SD+7,ID_SD+10,-1),
	//55
	CTreasureData(	0,6,5,30,
					ID_SHE+8,-1),
	CTreasureData(	0,0,5,30,
					ID_BS+7,ID_BS+6,ID_BS+5,ID_BS+4,-1),
	CTreasureData(	0,-1,80,30,
					ID_AMR+ALV_2+5,ID_BS+7,ID_BS+6,ID_BS+5,ID_BS+4,-1),
	CTreasureData(	0,-1,0,40,
					ID_SHE+10,-1),
	CTreasureData(	0,2,30,30,
					ID_SD+2,ID_SD+4,ID_BS+7,ID_BS+7,ID_BS+6,-1),
	//60
	CTreasureData(	0,1,120,30,
					ID_SD+7,-1),
	CTreasureData(	0,1,80,30,
					ID_BS+20,ID_BS+7,ID_BS+7,-1),
	CTreasureData(	0,-1,0,30,
					ID_MED+2,ID_SD+3,-1),
	CTreasureData(	0,-1,0,30,
					ID_AMR+ALV_3+4,ID_BS+6,ID_BS+6,ID_BS+5,ID_BS+5,-1),
//妖精の森
	CTreasureData(	0,-1,0,8,
					ID_AME+13,ID_BS+2,ID_BS+2,ID_BS+1,ID_BS+1,ID_BS+1,-1),
	//65
	CTreasureData(	0,2,8,8,
					ID_KEY+1,ID_BS+14,ID_BS+1,ID_BS+1,-1),
	CTreasureData(	0,6,30,8,
					ID_WND+WLV_1+2,ID_BS+2,ID_BS+1,ID_BS+0,-1),		
	CTreasureData(	0,-1,20,8,
					ID_SD+28,ID_SD+26,ID_SD+1,ID_SD+8,-1),
	CTreasureData(	0,-1,20,10,
					ID_AXE+WLV_1+3,-1),
	CTreasureData(	0,2,10,8,
					ID_HED+2,ID_BS+2,ID_BS+2,-1),	
	//70
	CTreasureData(	0,-1,20,8,
					ID_BOW+WLV_1+3,ID_BS+2,ID_BS+2,-1),		
	CTreasureData(	0,-1,0,20,												 //1月
					ID_SWD+WLV_2+2,ID_SD+1,ID_SD+1,-1),
	CTreasureData(	0,-1,20,10,
					ID_BS+15,ID_BS+2,-1),
	CTreasureData(	0,10,120,8,
					ID_SD+20,ID_SD+8,ID_SD+12,ID_BS+2,-1),
	CTreasureData(	0,10,150,10,
					ID_GLV+4,ID_BS+2,ID_BS+2,-1),
	//75
	CTreasureData(	0,-1,20,8,
					ID_KEY+2,ID_KEY+1,-1),
	CTreasureData(	K1,-1,20,10,
					ID_SPR+WLV_1+2,ID_BS+1,ID_BS+0,ID_BS+0,-1),
	CTreasureData(	K1,-1,20,10,
					ID_SLD+2,ID_BS+2,ID_BS+1,ID_BS+1,-1),
//砂漠とピラミッド
	CTreasureData(	0,-1,0,18,
					ID_KEY+3,ID_BS+4,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	0,-1,0,18,
					ID_SD+21,ID_SD+1,ID_BS+4,ID_BS+3,ID_BS+2,-1),
	//80
	CTreasureData(	0,-1,0,18,
					ID_SD+21,ID_BS+2,ID_BS+1,ID_BS+0,-1),
	CTreasureData(	0,-1,0,18,
					ID_AME+2,ID_BS+4,ID_BS+3,ID_BS+2,-1),
	CTreasureData(	0,-1,0,18,
					ID_HED+7,ID_BS+4,ID_BS+3,-1),
		//ここまで砂漠にランダムで配置。
	CTreasureData(	0,-1,0,25,
					ID_SLD+13,-1),
	CTreasureData(	0,6,15,20,
					ID_KEY+3,ID_BS+2,ID_BS+1,ID_BS+0,-1),
	//85
	CTreasureData(	K3,0,40,20,
					ID_AME+4,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,0,20,
					ID_SD+21,ID_SD+21,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,0,20,
					ID_HED+6,ID_SD+28,ID_SD+1,-1),
	CTreasureData(	0,0,60,20,
					ID_BS+16,ID_BS+3,ID_BS+3,-1),
	CTreasureData(	0,0,80,20,
					ID_SPR+WLV_2+0,ID_BS+4,ID_BS+3,-1),
	//90
	CTreasureData(	0,-1,60,22,
					ID_SD+1,ID_SD+1,ID_SD+9,-1),
	CTreasureData(	K3,2,100,60,
					ID_BS+17,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	K3,0,60,20,
					ID_GLV+10,ID_BS+4,ID_BS+3,-1),
//南の島
	CTreasureData(	0,-1,0,20,
					ID_BS+5,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,60,25,
					ID_WND+WLV_2+1,-1),
	//95
	CTreasureData(	K3,5,30,22,
					ID_BS+17,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	K3,-1,60,22,
					ID_GLV+11,ID_BS+4,ID_BS+3,ID_BS+2,ID_BS+1,-1),
	CTreasureData(	0,-1,0,20,
					ID_AME+0,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,1,80,24,
					ID_AXE+WLV_2+2,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,10,600,24,
					ID_KEY+3,ID_SD+25,ID_SD+4,-1),
	//100
	CTreasureData(	0,-1,0,22,
					ID_SD+22,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,0,22,
					ID_SD+25,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,0,22,
					ID_HED+8,ID_BS+3,ID_BS+2,ID_BS+1,-1),
	CTreasureData(	0,6,15,24,
					ID_SD+7,ID_SD+13,-1),
	CTreasureData(	0,6,15,24,
					ID_GLV+7,ID_BS+5,ID_BS+5,-1),
	//105
	CTreasureData(	0,0,70,24,
					ID_SPR+WLV_2+2,ID_BS+5,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,3,50,26,
					ID_BS+18,ID_BS+5,ID_BS+5,-1),
//からくり屋敷
	CTreasureData(	0,-1,0,26,
					ID_BS+6,ID_BS+5,ID_BS+5,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,0,26,
					ID_BS+3,ID_BS+3,ID_BS+2,ID_BS+2,ID_BS+1,ID_BS+1,ID_BS+0,ID_BS+0,-1),
	CTreasureData(	0,0,80,26,
					ID_BS+4,ID_BS+4,ID_BS+3,ID_BS+3,ID_BS+2,ID_BS+2,ID_BS+1,ID_BS+1,-1),
	//110
	CTreasureData(	K1,5,30,26,
					ID_MED+2,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,-1,100,26,
					ID_BS+18,ID_BS+5,ID_BS+5,-1),
	CTreasureData(	0,1,50,26,
					ID_MED+1,ID_SD+2,ID_SD+4,ID_SD+14,-1),
	CTreasureData(	0,2,30,26,
					ID_GLV+8,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,0,30,26,
					ID_SD+9,ID_SD+6,ID_SD+6,ID_BS+6,-1),
	//115
	CTreasureData(	0,-1,0,28,
					ID_BS+19,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,28,
					ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,-1,0,28,
					ID_KEY+4,ID_KEY+5,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,10,20,28,
					ID_AXE+WLV_2+3,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,10,2500,28,
					ID_HED+5,ID_BS+6,ID_BS+5,-1),
	//120
	CTreasureData(	K1,0,60,30,
					ID_MED+2,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,-1,0,30,
					ID_SD+2,ID_SD+26,ID_SD+26,-1),
//ガラス鉱山
	CTreasureData(	0,-1,0,34,
					ID_BS+8,ID_BS+7,ID_BS+6,-1),
	CTreasureData(	K5,5,10,34,
					ID_SWD+WLV_3+1,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,0,80,34,
					ID_SD+2,ID_SD+2,ID_BS+8,ID_BS+7,-1),
	//125
	CTreasureData(	0,-1,100,34,
					ID_MED+2,ID_BS+8,ID_BS+8,-1),
	CTreasureData(	0,1,50,34,
					ID_HED+12,ID_MED+2,-1),
	CTreasureData(	0,-1,100,34,
					ID_SD+23,ID_SD+14,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,2,25,34,
					ID_GLV+13,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,100,40,
					ID_BS+20,ID_BS+7,-1),
	//130
	CTreasureData(	0,-1,100,34,
					ID_AXE+WLV_3+3,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,100,40,
					ID_BS+8,ID_BS+7,ID_BS+6,-1),
	CTreasureData(	0,10,1500,34,
					ID_WND+WLV_3+2,ID_BS+8,-1),
	CTreasureData(	0,-1,0,34,
					ID_SD+5,ID_SD+9,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,34,
					ID_KEY+5,ID_BS+8,ID_BS+7,-1),
	//135
	CTreasureData(	0,-1,0,34,
					ID_SLD+10,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,0,150,34,
					ID_AME+1,ID_AME+2,ID_BS+8,-1),
	CTreasureData(	0,2,60,34,
					ID_AME+3,ID_AME+4,ID_BS+8,-1),
	CTreasureData(	0,-1,0,40,
					ID_BS+21,ID_BS+8,ID_BS+8,-1),			
//地下水道
	CTreasureData(	0,-1,0,25,
					ID_BS+0,ID_BS+0,ID_BS+1,ID_BS+1,ID_BS+4,-1),
	//140
	CTreasureData(	0,-1,0,25,
					ID_BS+2,ID_BS+2,ID_BS+4,-1),
	CTreasureData(	0,-1,10,25,
					ID_BS+2,ID_BS+2,ID_BS+3,ID_BS+3,ID_BS+4,-1),
	CTreasureData(	0,-1,30,25,
					ID_WND+WLV_2+3,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,3,10,25,
					ID_BS+16,ID_BS+3,ID_BS+3,-1),
	CTreasureData(	0,1,30,25,
					ID_MED+1,ID_BS+0,ID_BS+0,ID_BS+1,ID_BS+1,-1),
	//145
	CTreasureData(	0,-1,50,25,
					ID_BS+0,ID_BS+1,ID_BS+2,-1),
	CTreasureData(	0,7,30,25,
					ID_SD+13,ID_SD+21,ID_SD+22,ID_BS+4,-1),
	CTreasureData(	0,6,30,25,
					ID_SHE+5,ID_BS+4,-1),
	CTreasureData(	0,-1,50,25,
					ID_HED+2,ID_BS+3,-1),
	CTreasureData(	0,0,50,25,
					ID_SWD+WLV_2+1,ID_BS+4,-1),
	//150
	CTreasureData(	0,2,50,25,
					ID_AME+2,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,50,25,
					ID_BS+0,ID_BS+1,ID_BS+2,ID_BS+3,-1),
	CTreasureData(	0,-1,50,25,
					ID_AME+4,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,50,28,
					ID_BS+17,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,50,28,
					ID_HED+11,-1),
	//155
	CTreasureData(	0,-1,50,30,
					ID_MED+2,ID_MED+1,ID_MED+0,-1),
	CTreasureData(	0,-1,50,30,
					ID_KEY+2,ID_KEY+3,ID_KEY+6,-1),
	CTreasureData(	0,-1,50,30,
					ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,3,200,30,
					ID_SD+9,ID_SD+1,ID_SD+1,ID_SD+1,-1),
	CTreasureData(	0,0,100,28,
					ID_GLV+9,ID_MED+1,ID_MED+1,-1),
	//160
//フィーレス村
	CTreasureData(	0,-1,0,40,
					ID_AMR+ALV_3+0,ID_AMR+ALV_3+1,ID_AMR+ALV_3+2,ID_AMR+ALV_3+3,ID_BS+9,ID_BS+8,-1),
	CTreasureData(	0,-1,0,40,
					ID_SWD+WLV_3+0,ID_SPR+WLV_3+0,ID_AXE+WLV_3+0,ID_BOW+WLV_3+0,ID_WND+WLV_3+0,ID_BS+9,ID_BS+8,-1),
	CTreasureData(	0,-1,0,42,
					ID_BS+22,ID_BS+9,ID_BS+9,ID_BS+7,ID_BS+7,-1),
	CTreasureData(	0,-1,0,42,
					ID_BS+23,ID_BS+10,ID_BS+10,ID_BS+8,ID_BS+8,-1),
	CTreasureData(	0,-1,0,40,
					ID_BS+10,ID_BS+10,-1),
	//165
	CTreasureData(	0,-1,0,42,
					ID_MED+3,ID_MED+1,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,42,
					ID_MED+2,ID_MED+0,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,40,
					ID_AME+18,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,40,
					ID_AMR+ALV_4+5,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,42,
					ID_BS+10,ID_BS+9,ID_BS+8,-1),
	//170
	CTreasureData(	0,-1,0,42,
					ID_SLD+14,ID_HED+11,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,40,
					ID_GLV+44,ID_SHE+11,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,40,
					ID_KEY+6,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,40,
					ID_AME+14,ID_AME+15,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,42,
					ID_AXE+WLV_4+1,ID_BS+8,ID_BS+7,-1),
	//175
	CTreasureData(	0,-1,0,42,
					ID_WND+WLV_4+1,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,40,
					ID_SD+3,ID_SD+10,ID_SD+15,-1),
	CTreasureData(	0,-1,0,40,
					ID_AMR+ALV_3+7,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,42,
					ID_AMR+ALV_4+1,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,42,
					ID_AMR+ALV_4+3,ID_BS+8,ID_BS+7,-1),
	//180
	CTreasureData(	0,-1,0,42,
					ID_SD+23,ID_SD+3,ID_SD+11,ID_SD+15,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,42,
					ID_SD+22,ID_SD+5,ID_SD+7,ID_SD+6,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,50,
					ID_BS+10,ID_BS+10,ID_BS+9,ID_BS+9,ID_BS+8,ID_BS+8,-1),
//クロボーの巣					
	CTreasureData(	0,-1,0,20,												 //1月
					ID_SPR+WLV_2+1,ID_MED+1,ID_MED+1,-1),
	CTreasureData(	0,-1,0,20,												 //1月
					ID_NSUI+24,ID_SD+14,-1),
	//185
//ピラミッド深部	
	CTreasureData(	0,-1,0,20,												 //5月
					ID_SWD+WLV_2+3,ID_KEY+3,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,0,20,												 //5月
					ID_WND+WLV_2+3,ID_KEY+3,ID_BS+4,ID_BS+3,-1),
//フィーレス地下遺跡・隠し部屋
	CTreasureData(	0,-1,0,32,												 //8月
					ID_SHE+15,-1),
//ノーマル・ラストダンジョン
	CTreasureData(	0,-1,0,46,
					ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,7,20,46,
					ID_SD+3,ID_BS+11,ID_BS+10,-1),
	//190
	CTreasureData(	0,0,50,46,
					ID_BS+9,ID_BS+8,ID_BS+7,ID_BS+6,-1),
	CTreasureData(	0,-1,0,46,
					ID_HED+15,ID_BS+11,-1),
	CTreasureData(	0,9,300,46,
					ID_SHE+13,ID_BS+11,-1),
	CTreasureData(	0,7,20,46,
					ID_BS+11,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,46,
					ID_SWD+WLV_4+1,-1),
	//195
	CTreasureData(	0,5,50,46,
					ID_MED+3,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,46,
					ID_SD+20,ID_SD+23,-1),
	CTreasureData(	0,-1,0,46,
					ID_SPR+WLV_4+1,-1),
	CTreasureData(	0,0,60,46,
					ID_MED+3,ID_MED+3,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,46,
					ID_GLV+15,-1),
	//200
	CTreasureData(	0,10,1000,46,
					ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,0,50,46,
					ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,0,50,55,
					ID_BS+23,-1),
	//ここから第二層
	CTreasureData(	0,0,100,50,
					ID_SD+3,ID_SD+15,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,3,1,50,
					ID_MED+3,ID_MED+3,ID_BS+11,ID_BS+11,-1),
	//205
	CTreasureData(	0,-1,50,50,
					ID_SLD+15,ID_BS+11,-1),
	CTreasureData(	0,-1,50,50,
					ID_SHE+14,ID_BS+11,-1),
	CTreasureData(	0,-1,50,50,	
					ID_WND+WLV_4+3,ID_BS+11,-1),	
	CTreasureData(	0,-1,50,50,
					ID_SPR+WLV_4+2,ID_BS+11,-1),	
	CTreasureData(	0,-1,50,50,
					ID_BOW+WLV_4+2,ID_BS+11,-1),	
	//210
	CTreasureData(	0,-1,50,50,	
					ID_AXE+WLV_4+3,ID_BS+11,-1),	
	CTreasureData(	0,-1,50,55,	
					ID_NSUI+24,ID_BS+11,ID_BS+11,-1),	
	//ここから第三層
	CTreasureData(	0,0,90,55,	
					ID_SD+7,-1),	
	CTreasureData(	0,2,50,55,	
					ID_BS+11,-1),	
	CTreasureData(	0,10,2000,55,	
					ID_MED+3,-1),	
	//215
	CTreasureData(	0,0,30,55,	
					ID_AME+5,-1),	
	CTreasureData(	0,2,30,55,	
					ID_BS+11,-1),	
	CTreasureData(	0,6,30,55,	
					ID_BS+11,-1),	
	CTreasureData(	0,7,30,55,	
					ID_AME+6,-1),	
	CTreasureData(	0,-1,0,55,	
					ID_SD+5,-1),	
	//220
	CTreasureData(	0,0,50,55,	
					ID_AMR+ALV_4+1,-1),	
	CTreasureData(	0,9,300,55,	
					ID_MED+3,-1),	
	CTreasureData(	0,0,60,55,	
					ID_BS+11,-1),	
	CTreasureData(	0,1,10,55,	
					ID_AMR+ALV_4+3,-1),	
//盗賊のアジト
	CTreasureData(	0,-1,10,25,	
					ID_MED+0,-1),	
	//225
	CTreasureData(	0,-1,10,25,	
					ID_BOW+WLV_2+3,ID_MED+1,-1),	
	CTreasureData(	0,-1,10,25,	
					ID_AMR+ALV_2+3,ID_MED+2,-1),	
	CTreasureData(	0,-1,10,25,	
					ID_WND+WLV_4+2,ID_MED+3,-1),	
//人魚
	CTreasureData(	0,-1,10,32,	
					ID_HED+14,-1),	
	CTreasureData(	0,-1,10,32,	
					ID_AMR+ALV_3+5,-1),	
	//230
	CTreasureData(	0,-1,10,36,	
					ID_AXE+WLV_3+2,-1),	
	CTreasureData(	0,-1,10,36,	
					ID_BOW+WLV_3+3,-1),	
//フィーレス地下遺跡・隠し部屋
	CTreasureData(	0,-1,0,32,												 //8月
					ID_SWD+WLV_3+3,-1),
	CTreasureData(	0,-1,0,32,												 //8月
					ID_SLD+12,-1),
	CTreasureData(	0,-1,0,45,												 //8月
					ID_AMR+ALV_4+4,-1),
	//235
	CTreasureData(	0,-1,0,45,												 //8月
					ID_SWD+WLV_4+2,-1),
//秋の山
	CTreasureData(	0,0,30,40,	
					ID_BS+9,ID_BS+9,-1),	
	CTreasureData(	K4,2,60,40,	
					ID_BOW+WLV_4+3,ID_BS+9,ID_BS+8,-1),	
	CTreasureData(	0,-1,30,40,	
					ID_BS+9,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,11,1500,40,	
					ID_HED+13,ID_BS+9,ID_BS+8,-1),
	//240
	CTreasureData(	0,-1,30,40,	
					ID_BS+9,ID_BS+9,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,30,40,	
					ID_GLV+14,ID_BS+9,ID_BS+9,-1),	
	CTreasureData(	0,-1,30,40,	
					ID_KEY+5,ID_BS+9,ID_BS+9,-1),	
	CTreasureData(	0,-1,30,40,	
					ID_AMR+ALV_3+6,ID_BS+9,ID_BS+8,-1),	
	CTreasureData(	0,-1,30,40,	
					ID_SD+3,ID_SD+11,-1),		
	//245
	CTreasureData(	0,-1,30,40,
					ID_BS+22,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	K5,-1,30,40,
					ID_SLD+11,-1),	
	CTreasureData(	K5,-1,30,40,
					ID_BOW+WLV_4+1,-1),	
	CTreasureData(	0,-1,30,40,	
					ID_BS+21,ID_BS+8,ID_BS+8,-1),
//南の森
	CTreasureData(	0,-1,0,15,	
					ID_SD+8,ID_SD+26,ID_SD+28,-1),	
	//250
	CTreasureData(	0,-1,0,15,	
					ID_BS+2,ID_BS+2,-1),	
	CTreasureData(	0,10,200,15,	
					ID_MED+0,ID_MED+0,ID_MED+0,-1),	
	CTreasureData(	0,-1,0,15,	
					ID_BS+2,ID_BS+2,-1),	
	CTreasureData(	0,-1,0,15,	
					ID_SD+21,-1),
	CTreasureData(	0,-1,0,25,	
					ID_SWP+0,-1),	
	//255
	CTreasureData(	0,-1,0,15,	
					ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,15,	
					ID_SD+12,ID_SD+12,-1),	
	CTreasureData(	0,-1,0,15,	
					ID_BOW+WLV_1+3,ID_BS+2,-1),
	CTreasureData(	0,-1,0,15,	
					ID_BS+14,ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1,-1),
	CTreasureData(	0,-1,0,15,	
					ID_SD+8,ID_SD+8,ID_BS+0,ID_BS+0,-1),	
	//260
	CTreasureData(	0,2,20,15,	
					ID_AME+6,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,15,	
					ID_SHE+4,ID_BS+2,ID_BS+2,-1),			//要きこりの斧
	CTreasureData(	0,-1,0,15,	
					ID_MED+0,ID_MED+0,ID_MED+0,-1),	
	CTreasureData(	0,-1,0,15,	
					ID_BS+15,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,15,	
					ID_HED+7,ID_BS+2,ID_BS+2,-1),			//要きこりの斧
	//265
	CTreasureData(	0,-1,0,15,	
					ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,15,	
					ID_SWD+WLV_1+3,ID_BS+2,-1),			//武器
	CTreasureData(	0,-1,0,15,	
					ID_SPR+WLV_1+1,ID_BS+2,-1),			//武器
	CTreasureData(	0,9,50,15,	
					ID_AXE+WLV_1+1,-1),			//斧
//地下墓所
	CTreasureData(	K6,-1,0,55,	
					ID_BOW+WLV_4+1,ID_BS+9,ID_BS+9,-1),
	//270
	CTreasureData(	K6,-1,0,55,	
					ID_SPR+WLV_4+2,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,55,
					ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,55,	
					ID_BS+21,ID_BS+8,ID_BS+8,-1),
	CTreasureData(	0,-1,0,55,	
					ID_AMR+ALV_4+3,ID_BS+9,ID_BS+8,ID_BS+7,-1),	//
	CTreasureData(	0,-1,0,55,	
					ID_AXE+WLV_4+2,ID_BS+9,ID_BS+8,ID_BS+7,-1),	//
	//275
	CTreasureData(	0,-1,0,60,	
					ID_SHE+13,-1),//
	CTreasureData(	0,-1,0,60,	
					ID_SWD+WLV_4+1,-1),
	CTreasureData(	0,-1,0,60,	
					ID_MED+3,ID_MED+3,ID_MED+3,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SD+15,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SLD+13,ID_BS+9,ID_BS+8,ID_BS+7,-1),
	//280
	CTreasureData(	0,-1,0,60,	
					ID_WND+WLV_4+3,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_AMR+ALV_4+5,-1),
	CTreasureData(	0,-1,0,60,	
					ID_KEY+4,ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9,-1),
//11月Ｈ
	CTreasureData(	0,-1,0,60,	
					ID_BS+10,ID_BS+10,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+8,ID_BS+8,ID_BS+7,ID_BS+7,-1),
	//285
	CTreasureData(	0,-1,0,60,	
					ID_BS+6,ID_BS+6,ID_BS+5,ID_BS+5,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+10,ID_BS+9,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,60,	
					ID_GLV+14,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+10,ID_BS+10,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+8,ID_BS+8,ID_BS+7,ID_BS+7,-1),
	//290
	CTreasureData(	0,-1,0,60,	
					ID_BS+22,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_HED+13,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_WND+WLV_1+2,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SLD+11,ID_BS+10,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SHE+11,ID_BS+10,ID_BS+9,-1),
	//295
	CTreasureData(	0,-1,0,60,	
					ID_MED+3,ID_MED+3,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_AMR+ALV_4+1,ID_BS+10,ID_BS+9,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+23,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BOW+WLV_4+2,-1),
	CTreasureData(	0,-1,0,60,	
					ID_WND+WLV_4+1,-1),
	//300
	CTreasureData(	0,-1,0,60,	
					ID_AXE+WLV_4+3,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SPR+WLV_4+1,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_AMR+ALV_4+2,-1),
//12月Ｈ
	CTreasureData(	0,-1,0,60,	
					ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1),
	//305
	CTreasureData(	0,-1,0,60,	
					ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+24,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+23,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+14,ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+14,ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1,ID_BS+1,-1),
	//310
	CTreasureData(	0,-1,0,60,	
					ID_BS+13,ID_BS+0,ID_BS+0,ID_BS+0,ID_BS+0,ID_BS+0,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SLD+15,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+13,ID_BS+0,ID_BS+0,ID_BS+0,ID_BS+0,ID_BS+0,-1),
	CTreasureData(	0,-1,0,60,	
					ID_HED+5,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+16,ID_BS+3,ID_BS+3,ID_BS+3,ID_BS+3,ID_BS+3,-1),
	//315
	CTreasureData(	0,-1,0,60,	
					ID_BS+16,ID_BS+3,ID_BS+3,ID_BS+3,ID_BS+3,ID_BS+3,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+15,ID_BS+2,ID_BS+2,ID_BS+2,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+15,ID_BS+2,ID_BS+2,ID_BS+2,ID_BS+2,ID_BS+2,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SD+3,ID_SD+11,ID_SD+15,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+17,ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4,-1),
	//320
	CTreasureData(	0,-1,0,60,	
					ID_BS+17,ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4,ID_BS+4,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+18,ID_BS+5,ID_BS+5,ID_BS+5,ID_BS+5,ID_BS+5,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+18,ID_BS+5,ID_BS+5,ID_BS+5,ID_BS+5,ID_BS+5,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+20,ID_BS+7,ID_BS+7,ID_BS+7,ID_BS+7,ID_BS+7,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+20,ID_BS+7,ID_BS+7,ID_BS+7,ID_BS+7,ID_BS+7,-1),
	//325
	CTreasureData(	0,-1,0,60,	
					ID_SHE+12,ID_MED+3,ID_MED+3,ID_MED+3,ID_MED+3,ID_MED+3,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+21,ID_BS+8,ID_BS+8,ID_BS+8,ID_BS+8,ID_BS+8,-1),
	CTreasureData(	0,-1,0,60,	
					ID_OTH+7,-1),
	CTreasureData(	0,-1,0,60,	
					ID_OTH+8,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SHE+14,-1),
	//330
	CTreasureData(	0,-1,0,60,	
					ID_OTH+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+21,ID_BS+8,ID_BS+8,ID_BS+8,ID_BS+8,ID_BS+8,-1),
	CTreasureData(	0,-1,0,60,	
					ID_MED+3,ID_MED+3,ID_MED+3,ID_MED+3,ID_MED+3,-1),
	CTreasureData(	0,-1,0,60,	
					ID_OTH+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+22,ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9,-1),
	//335
	CTreasureData(	0,-1,0,60,	
					ID_BS+22,ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SLD+14,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+24,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+24,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,ID_BS+11,-1),
	CTreasureData(	0,-1,0,60,	
					ID_SWP+3,-1),
//7月Ｈ
	//340
	CTreasureData(	0,-1,0,30,	
					ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_AMR+ALV_2+4,ID_BS+6,ID_BS+5,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BS+18,ID_BS+5,ID_BS+5,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BS+6,ID_BS+5,ID_BS+4,ID_BS+3,-1),
	CTreasureData(	0,-1,0,30,	
					ID_SLD+5,ID_BS+6,ID_BS+6,-1),
	//345
	CTreasureData(	0,-1,0,30,	
					ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BS+6,ID_BS+5,ID_BS+4,-1),
	CTreasureData(	0,-1,0,30,	
					ID_SPR+WLV_2+3,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_KEY+6,ID_BS+6,ID_BS+6,-1),
	//350
	CTreasureData(	0,-1,0,30,	
					ID_SD+9,ID_SD+14,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_GLV+10,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BOW+WLV_2+2,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_SD+21,ID_SD+14,ID_BS+6,-1),
	CTreasureData(	0,-1,0,30,	
					ID_BS+19,ID_BS+6,ID_BS+6,-1),
	//355
	CTreasureData(	0,5,250,30,	
					ID_MED+3,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,60,	
					ID_AXE+WLV_4+1,-1),
	CTreasureData(	0,-1,0,60,	
					ID_MED+3,ID_SD+5,ID_SD+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_AMR+ALV_3+7,-1),
//12月Ｈ
	CTreasureData(	0,-1,0,60,	
					ID_BS+19,ID_BS+6,ID_BS+6,ID_BS+6,ID_BS+6,ID_BS+6,-1),
	//360
	CTreasureData(	0,-1,0,60,	
					ID_BS+19,ID_BS+6,ID_BS+6,ID_BS+6,ID_BS+6,ID_BS+6,-1),
	CTreasureData(	0,-1,0,60,	
					ID_GLV+15,-1),
//1月Ｈ・上級者の館
	CTreasureData(	0,-1,0,15,	
					ID_AXE+WLV_1+3,ID_MED+0,-1),
	CTreasureData(	0,-1,0,30,	
					ID_AMR+ALV_3+6,ID_MED+1,-1),
	CTreasureData(	0,-1,0,45,	
					ID_GLV+13,ID_MED+2,-1),
	//365
	CTreasureData(	0,-1,0,60,	
					ID_SPR+WLV_4+3,ID_MED+3,-1),
//12月Ｈ
	CTreasureData(	0,-1,0,60,	
					ID_BS+23,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_BS+23,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,ID_BS+10,-1),
	CTreasureData(	0,-1,0,60,	
					ID_HED+15,-1),
//秋の山その２
	CTreasureData(	K6,4,160,60,	
					ID_SPR+WLV_4+3,-1),
	//370
	CTreasureData(	0,-1,0,40,	
					ID_MED+3,ID_BS+9,ID_BS+9,-1),
	CTreasureData(	0,-1,0,40,	
					ID_AMR+ALV_4+2,-1),
	CTreasureData(	0,-1,0,40,	
					ID_BS+9,ID_BS+8,ID_BS+7,-1),
	CTreasureData(	0,-1,0,45,	
					ID_BS+9,ID_BS+9,-1),
};





