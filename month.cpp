
#include "month.h"
#include "gamewindow.h"
#include "flag.h"
#include "menudata.h"

CMonthData::CMonthData()
{
	mTresure=NULL;
}

CMonthData::CMonthData(const char* name,int id,int month,const char* title,int normallv,int hardlv,int startmap,int x,int y,int z,
				int pa,int pb,int pc,int pd,int ima,int imb,int imc,int imd,int tresure, ...)
{
	mName=name;
	mTitle=title;
	mID=id;
	mMonth=month;
	mBaseLv[0]=normallv;
	mBaseLv[1]=hardlv;

	mStartMap=startmap;
	mX=x;
	mY=y;
	mZ=z;

	mParty[0]=pa;
	mParty[1]=pb;
	mParty[2]=pc;
	mParty[3]=pd;

	mImpossible[0]=ima;
	mImpossible[1]=imb;
	mImpossible[2]=imc;
	mImpossible[3]=imd;

	va_list marker;
	va_start( marker, tresure ); 

	static int list[256];
	mTresureNo=0;
	while(tresure>=0 && mTresureNo<256)
	{
		list[mTresureNo]=tresure;
		tresure = va_arg( marker, int);
		mTresureNo++;
	}
	mTresure=new int[mTresureNo];
	memcpy(mTresure,list,sizeof(int)*mTresureNo);


	int enemy=va_arg( marker, int);
	mEnemyNo=0;
	while(enemy>=0 && mEnemyNo<256)
	{
		list[mEnemyNo]=enemy;
		enemy = va_arg( marker, int);
		mEnemyNo++;
	}
	mEnemy=new int[mEnemyNo];
	memcpy(mEnemy,list,sizeof(int)*mEnemyNo);


	va_end( marker );
}

CMonthData::~CMonthData()
{
	delete[] mTresure;
}
void	CMonthData::Draw(CPlaneWindow* window,int x,int y,int param,D3DCOLOR color) const
{
//param&0xffff 宝箱情報
	static char text[32];
	sprintf(text,"%d月",mMonth);
	window->DrawText(0,text,x,y+6,0);
	window->DrawText(5,mName,x+50,y+6,0);
	
	int box=param&0xffff;
	if(box>0){
		window->DrawTexture(&SystemTex,0,96,112,32,32,x+220,y);
		y+=8;
		window->DrawText(0,"/",x+290,y);
		if((1<<mMonth)&box){
			int tresure=0;
			for(int i=0;i<mTresureNo;i++){
				if(Flag.mTreasure[mTresure[i]])	tresure++;
			}
			sprintf(text,"%d",tresure);
			D3DCOLOR color=0xff000000;
			if(tresure>=mTresureNo)	color=0xffff0000;
			window->DrawText(0,text,x+270,y,color);
			sprintf(text,"%d",mTresureNo);
			window->DrawText(0,text,x+300,y);
		}else{
			window->DrawText(0,"??",x+270,y);
			window->DrawText(0,"??",x+300,y);
		}
	}
}
void CMonthData::Help(int param)const
{
	int box=param&0xffff;
	if((1<<mMonth)&box){
		MenuData.SetMenu(DInfoMenu,mID);
	}
}
int		CMonthData::GetBaseLv(void)
{
	switch(Flag.mHardMode){
		case 0:
		case 1:
			return mBaseLv[Flag.mHardMode];
		default:
			return 60;
	}
}

CMonthData MonthData[MAX_ALL_MONTH]=
{
	CMonthData("小さな村の新年祭"	,0	,1	,
				"小さな村の　　\n新年祭",
		/* ノーマル用、ハードはもっと下 */
				2	,6	,			// enemylv
				2,//303	,				// mapno
				4	,5	,3	,		// startpoint
				0	,1	,2	,3	,	// member
				-1	,-1	,-1	,-1	,
				2,3,4,5,6,7,8,9,10,11,71,183,184,-1,
				0,1,2,10,-1),
	CMonthData("農場と種"			,1	,2	,
				"農場と種　"			,
				5	,12	,			//enemylv
				145	,
				6	,2	,2	,
				0	,1	,-2	,-1	,
				-1	,-1	,-1	,-1	,
				12,13,14,15,16,17,18,19,20,21,22,23,25,26,-1,
				0,1,2,5,8,9,10,-1),
	CMonthData("木漏れ日の中の妖精"	,2	,3	,
				"木漏れ日の　　\n中の　\n妖精",
				8	,14	,			//enemylv
				120	,
				2	,6	,2	,
				0	,-2	,-1	,-1	,
				1	,8	,-1	,-1	,
				64,65,66,67,68,69,70,72,73,74,75,76,77,-1,
				5,8,11,20,21,28,29,30,-1),
	CMonthData("名無の花"			,3	,3	,
				"名無の花　"			,
				8	,14	,			//enemylv
				57,
				5	,5	,2	,
				2	,-2	,-1	,-1	,
				0	,1	,11	,-1	,
				249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,-1,
				0,2,5,8,10,12,20, 65,88,-1),
	CMonthData("宝探しに行こう"		,4	,4	,
				"宝探しに　　\n行こう！"		,
				12	,20	,			//enemylv
				57	,
				6	,9	,10	,
				0	,1	,7	,-1	,
				-1	,-1	,-1	,-1	,
				0,1,24,27,28,29,30,31,32,33,34,35,36,37,38,39,41,42,43,44,	40,224,225,226,227,-1,
				1,2,5,8,9,11,12,15,37,13,14,-1),
	CMonthData("砂の大地"			,5	,5	,
				"砂の大地　"			,
				16	,22	,			//enemylv
				153	,
				2	,6	,2	,
				0	,8	,-2	,-1	,
				0	,1	,4  ,-1	,
				78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,185,186,-1,
				26,27,33,34,35,36,56,57,-1),
	CMonthData("五月雨の下"			,6	,5	,
				"五月雨の下　"			,
				0	,28	,			//enemylv
				298	,
				5	,5	,3	,
				9	,12	,-2	,-2	,
				-1	,-1	,-1	,-1	,
				139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,-1,
				0,10, 39,46,63, 59,81, 92, -1),
	CMonthData("古より来たりて"		,7	,6	,
				"古より　　\n来たりて"		,
				22	,32	,			//enemylv
				57	,
				5	,5	,2	,
				0	,1	,-1	,-1	,
				0	,1	,-1	,-1	,
				93,94,95,96,97,98,99,100,101,102,103,104,105,106,228,229,230,231,-1,
				10,12,28,38,39,40,76,77,-1),
	CMonthData("幽かな夜"			,8	,7	,
				"幽かな夜　"			,
				26	,36	,			//enemylv
				220	,
				3	,3	,2	,
				0	,5	,-1	,-1	,
				1	,3	,4	,8	,
				107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,-1,
				0,7,32,62,17,-1),
	CMonthData("炎、燃える"			,9	,7	,
				"炎　\n\n燃える　",			
				0	,38	,			//enemylv
				302	,
				5	,5	,2	,
				0	,11	,-2	,-1	,
				0	,1	,6	,-1	,
				340,341,342,343,344,345,346,347,348,349,350,351,352,353,354 ,355,356,357,358,-1,
				7,10,26,27,35,49,58,98,48,-1),
	CMonthData("母からの手紙"		,10	,8	,
				"母からの　　\n手紙"		,
				30	,42	,			//enemylv
				220	,
				3	,3	,2	,
				0	,1	,8	,-1	,
				-1	,-1	,-1	,-1	,
				45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,187,232,233,234,235,-1,
				9,16,17,18,29,63, 5,47,30, 80,81,-1),
	CMonthData("罅割れた硝子"		,11	,9	,
				"罅割れた　\n硝子　"		,
				34	,48	,			//enemylv
				246	,
				5	,5	,2	,
				0	,1	,-1	,-1	,
				-1	,-1	,-1	,-1	,
				122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,-1,
				0,11,44,45,46,64,9,18,47,17,32,-1),
	CMonthData("思い出の花園"		,12	,10	,
				"思い出の　　\n花園"		,
				36	,54	,			//enemylv
				678	,
				5	,8	,10	,
				0	,4	,8	,-1	,
				1	,8	,-1	,-1	,
				236,237,238,239,240,241,242,243,244,245,246,247,248,369,370,371,372,373,-1,
				1,2,11,63,64, 0,58,59, 48, -1),
	CMonthData("大聖堂にて"			,13	,10	,
				"　\n\n\n大聖堂にて..."			,
				36	,52	,			//enemylv
				220	,
				5	,5	,2	,
				0	,-2	,-2	,-2	,
				-1	,-1	,-1	,-1	,
				269,270,271,272,273,274,275,276,277,278,279,280,281,282,-1,
				36,56,57,-1),
	CMonthData("災厄たちの来訪"		,14	,11	,
				"災厄たちの　　\n来訪"		,
				42	,60	,			//enemylv
				246	,
				//707,
				5	,5	,2	,
				0	,8	,-1	,-1	,
				1	,-1	,-1	,-1	,
				160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,
				177,178,179,180,181,182,-1,
				22,23,52,53,54,-1),
	CMonthData("叶わぬ望み"			,15,11	,
				"　\n\n\n叶わぬ望み"			,
				42	,60	,			//enemylv
				300	,
				5	,5	,2	,
				11	,-2	,-1	,-1	,
				0	,1	,-1	,-1	,
				283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,-1,
				27,28,29,30, 46,				-1),
	CMonthData("小さな村の誕生会"	,16	,12	,
				"小さな村の　　\n誕生会"	,
				46	,60	,			//enemylv
				338	,
				3	,19	,13	,
				0	,-1	,-1	,-1	,
				1	,8	,-1	,-1	,
				188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,//第一層
				203,204,205,206,207,208,209,210,211,//第2層
				212,213,214,215,216,217,218,219,220,221,222,//第3層
				-1,

				0,1,2,5,10,11, 36,56,57,65, 38,63,64,40,77, 16,17,44,48, -1),
	CMonthData("しあわせの果てに"	,17	,12	,
				"　\nしあわせの　　\n果てに"	,
				0	,60	,			//enemylv
				393	,
				5	,5	,2	,
				0	,-1	,-1	,-1	,
				-1	,-1	,-1	,-1	,
			/*宝箱*/
				304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,
				330,331,332,333,334,335,336,337,338,339,359,360,361,366,367,368,-1,
			/*敵*/
				0,5,17,30,47,/*洞窟*/2,9,11,16,21,/*草原*/20,8,12,15,92,26,/*森林*/13,14,22,23,52,53,54,/*村落*/
				76,77,38,39,40/*鍾乳洞*/,27,35,49,48,58,/*溶岩流*/18,32,62,80,81,95/*遺跡*/,36,56,57,65,67,/*廃墟*/
				1,46,82,83/*図書館*/,10,19,63/*空中庭園*/,
				-1),
	CMonthData("小さな村の新年祭"	,18	,1	,
				"小さな村の　　\n新年祭",
		/*ハードモードなので、上級者の館の宝箱を配置*/
				2	,6	,			// enemylv
				2	,				// mapno
				4	,5	,3	,		// startpoint
				0	,1	,2	,3	,	// member
				-1	,-1	,-1	,-1	,
				2,3,4,5,6,7,8,9,10,11,71,183,184,362,363,364,365,-1,
				0,1,2,10,-1),
};

int	NormalHard[3][MAX_ALL_MONTH]=
{
	{0,1,2,4,5,7,8,10,11,12,14,16,-1,0,0,0,0,0},
	{18,1,3,4,6,7,9,10,11,13,15,17,-1,0,0,0,0,0},
	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18},
};
const char*	MonthEnglish[MAX_MONTH]={
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};