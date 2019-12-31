#include "skilldata.h"
#include "main.h"
#include "stdarg.h"
#include "gamewindow.h"
#include "menudata.h"
#include "battleconst.h"
#include "main.h"

const CSkillData 		NoneSkillData("－－－",-1,SKILL_NONE,0);

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
//param&1	習得済みスキル
//param&2	特別スキル
//param&4	詳細不明スキル
//param&16	小さく表示 
	if(param & 1){
		window->DrawBox(x,y+8,999,16,0x40ff80ff);
		window->DrawText(0,"習得済！",10,y+6,0xffff0000,DT_RIGHT);
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
		window->DrawText(font,"？？？",x+namex,y+namey);
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
			window->DrawText(2,"Special！",x+20,y+namey+16,0xffff0000,DT_LEFT);
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
		sprintf(name,"【C】%s",ComSkillData[mParam].GetName());
	}else{
		switch(mType){
			case SKILL_STATUS:
				sprintf(name,"【P】%s",mName);	break;
			case SKILL_AUTO:
				sprintf(name,"【A】%s",mName);	break;
			case SKILL_FIELD:
				sprintf(name,"【F】%s",mName);	break;
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
//param&1 使用不能スキル
//param&2 特別スキル
//param&4 詳細不明スキル
//param&8 レベルを表示しない
//param&16 小さく表示

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
		window->DrawText(font,"？？？",x+namex,y,textcolor);
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
	CComSkillData(	"叩く"				,0		,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"斬る"				,1		,CS_SWORD		|CS_MOVE		,0	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"突く"				,2		,CS_SPEAR		|CS_MOVE		,0	,true	,CR_ONE_M		,2,-1),
	CComSkillData(	"叩っ切る"			,3		,CS_AXE			|CS_MOVE		,0	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"射る"				,4		,CS_BOW			|CS_NO_MOVE		,0	,true	,CR_ONE_M		,4,-1),//剣の舞が依存
	CComSkillData(	"レーザー"			,5		,CS_SKILL		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,-1),
	CComSkillData(	"失敗！"			,6		,0				|CS_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	"ＳＰが足りない！"	,7		,0				|CS_MOVE		,0	,true	,CR_ONE			,-1),
	//以上、特殊用途
	//以下、習得用スキル。40を足す。
		//剣技
	CComSkillData(	"水平斬り"			,8		,CS_SWORD		|CS_MOVE		,0	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"ダブルスラッシュ"	,9		,CS_SWORD		|CS_MOVE		,1	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"瞬撃"				,10		,CS_SWORD		|CS_QUICK_MOVE	,1	,true	,CR_ONE_M		,0,-1),//発動が早い
	CComSkillData(	"回転切り"			,11		,CS_SWORD		|CS_MOVE		,1	,true	,CR_CIRCLE		,16,-1),
	CComSkillData(	"トライスラスト"	,12		,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,0,7,-1),
	CComSkillData(	"閃光の太刀"		,13		,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,32,-1),//クリティカルアップ
	CComSkillData(	"ブレードカルテット",14		,CS_SWORD		|CS_MOVE		,3	,true	,CR_ONE_M		,0,-1),
	CComSkillData(	"剣の舞"			,15		,CS_SWORD		|CS_NO_MOVE		,3	,true	,CR_ALL			,17,-1),//10回攻撃
	CComSkillData(	"セブンスターズ"	,16		,CS_SWORD		|CS_MOVE		,4	,true	,CR_ONE_M		,17,-1),
	CComSkillData(	"ルナティックブラッド",17	,CS_SWORD		|CS_MOVE		,4	,true	,CR_ONE_M		,66,-1),
		//槍技
	CComSkillData(	"振り回し"			,18		,CS_SPEAR		|CS_MOVE		,0	,true	,CR_CIRCLE		,16,-1),
	CComSkillData(	"なぎ払い"			,19		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_VLINE		,17,-1),
	CComSkillData(	"一閃"				,20		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_HLINE		,17,-1),
	CComSkillData(	"足払い"			,21		,CS_SPEAR		|CS_MOVE		,1	,true	,CR_ONE_M		,24,-1),//スタン付与
	CComSkillData(	"風刃裂"			,22		,CS_SPEAR		|CS_NO_MOVE		,1	,true	,CR_ONE_M		,14,-1),//遠隔攻撃
	CComSkillData(	"大車輪"			,23		,CS_SPEAR		|CS_MOVE		,2	,true	,CR_BIG_CIRCLE	,16,-1),
	CComSkillData(	"不動突き"			,24		,CS_SPEAR		|CS_SLOW_NO_WALK,2	,true	,CR_ONE_M		,-1),//ため攻撃
	CComSkillData(	"驚天動地"			,25		,CS_SPEAR		|CS_NO_MOVE		,3	,true	,CR_ALL			,28,22,-1),
	CComSkillData(	"カミカゼストライク",26		,CS_SPEAR		|CS_QUICK_NO_WALK,3	,true	,CR_ONE_M		,12,61,-1),//特攻
	CComSkillData(	"無限螺旋"			,27		,CS_SPEAR		|CS_MOVE		,4	,true	,CR_ONE_M		,17,-1),
		//斧技
	CComSkillData(	"丸太切り"			,28		,CS_AXE			|CS_MOVE		,0	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"ヘビーアタック"	,29		,CS_AXE			|CS_MOVE		,1	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"衝撃波"			,30		,CS_AXE			|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,22,-1),
	CComSkillData(	"吹き飛ばし"		,31		,CS_AXE	|CS_MOVE |CS_TLOCK_HARD	,1	,true	,CR_ONE_M		,1,-1),//吹き飛ばす
	CComSkillData(	"スピニングブレイク",32		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"矛砕き"			,33		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,44,-1),
	CComSkillData(	"盾砕き"			,34		,CS_AXE			|CS_MOVE		,2	,true	,CR_ONE_M		,3,45,-1),
	CComSkillData(	"スカイダイビング"	,35		,CS_AXE			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,3,-1),
	CComSkillData(	"ライフブレイク"	,36		,CS_AXE			|CS_MOVE		,3	,true	,CR_ONE_M		,3,26,34,-1),
	CComSkillData(	"ファーマークラッシュ",37	,CS_AXE			|CS_MOVE_NO_WALK,4	,true	,CR_CIRCLE		,22,49,60,-1),
		//弓技
	CComSkillData(	"ゼロ距離射撃"		,38		,CS_BOW			|CS_MOVE		,0	,true	,CR_ONE_M		,4,-1),
	CComSkillData(	"バードショット"	,39		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,19,-1),
	CComSkillData(	"毒矢"				,40		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,4,21,-1),//毒付与
	CComSkillData(	"マルチプルショット",41		,CS_BOW			|CS_NO_MOVE		,1	,true	,CR_ONE_M		,4,-1),//必中
	CComSkillData(	"スターダストアロー",42		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ONE_M		,36,-1),
	CComSkillData(	"影縫い"			,43		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ONE_M		,43,-1),//麻痺付与
	CComSkillData(	"アローレイン"		,44		,CS_BOW			|CS_NO_MOVE		,2	,true	,CR_ALL			,4,43,-1),//魔法攻撃
	CComSkillData(	"シュート･ザ･ムーン",45		,CS_BOW			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,4,52,-1),
	CComSkillData(	"愛の天使"			,46		,CS_BOW			|CS_NO_MOVE		,3	,true	,CR_ONE_M		,26,34,43,63,-1),//混乱付与
	CComSkillData(	"ソウルブレイク"	,47		,CS_BOW			|CS_NO_MOVE		,4	,true	,CR_ONE_M		,43,-1),//即死効果
		//格闘技
	CComSkillData(	"強打"				,48		,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"復讐の一撃"		,49		,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE			,-1),
	CComSkillData(	"ボコボコ"			,50		,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	""					,51		,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,-1),
	CComSkillData(	""					,52		,CS_SKILL		|CS_MOVE		,3	,true	,CR_ONE_M		,-1),
		//魔法
	CComSkillData(	"マジックシュート"	,53		,CS_MAGIC		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,5,-1),
		//炎魔法
	CComSkillData(	"フレイムボール"	,54		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,9,10,-1),//ファイアレインが依存
	CComSkillData(	"シャイニング"		,55		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_ALL			,-1),//広範囲をスタン
	CComSkillData(	"ヒートアタック"	,56		,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//攻撃・魔攻アップ
	CComSkillData(	"ファイアウォール"	,57		,CS_MAGIC_FIRE	|CS_NO_MOVE		,2	,true	,CR_VLINE		,9,10,-1),
	CComSkillData(	"バーンブースター"	,58		,CS_MAGIC_FIRE	|CS_QUICK		,2	,false	,CR_ALL			,10,-1),//メージ2倍(1ターン)
	CComSkillData(	"エクスプロージョン",59		,CS_MAGIC_FIRE	|CS_NO_MOVE		,3	,true	,CR_BIG_CIRCLE	,10,50,-1),
	CComSkillData(	"フェニックス"		,60		,CS_MAGIC_FIRE	|CS_NO_MOVE		,3	,false	,CR_ONE			,57,-1),//リヴァイヴァ
	CComSkillData(	"セイントスター"	,61		,CS_MAGIC_FIRE	|CS_NO_MOVE		,4	,true	,CR_CIRCLE		,10,-1),
		//氷魔法
	CComSkillData(	"アイスニードル"	,62		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,true	,CR_ONE_M		,11,12,-1),
	CComSkillData(	"フロストバイト"	,63		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,true	,CR_ONE			,12,-1),
	CComSkillData(	"クールバリア"		,64		,CS_MAGIC_ICE	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//魔防アップ
	CComSkillData(	"フローズンソード"	,65		,CS_MAGIC_ICE	|CS_NO_MOVE		,2	,true	,CR_HLINE		,38,-1),
	CComSkillData(	"コールドスリープ"	,66		,CS_MAGIC_ICE	|CS_NO_MOVE		,2	,true	,CR_S_ALL		,12,-1),//眠り追加
	CComSkillData(	"アイシクルスラッサー",67	,CS_MAGIC_ICE	|CS_NO_MOVE		,3	,true	,CR_ONE_M		,12,53,54,-1),
	CComSkillData(	"ヒールレイン"		,68		,CS_MAGIC_ICE	|CS_NO_MOVE		,3	,false	,CR_ALL			,51,-1),//自動回復(永続)
	CComSkillData(	"スノーマンラッシュ",69		,CS_MAGIC_ICE	|CS_NO_MOVE		,4	,true	,CR_ONE_M		,12,54,72,73,-1),
		//風魔法
	CComSkillData(	"ウインドカッター"	,70		,CS_MAGIC_WIND	|CS_NO_MOVE		,1	,true	,CR_HLINE		,13,14,-1),
	CComSkillData(	"ガストウインド"	,71		,CS_MAGIC_WIND	|CS_NO_MOVE		,1	,true	,CR_ALL		,30,-1),//敵を吹き飛ばす
	CComSkillData(	"ゲイルスピード"	,72		,CS_MAGIC_WIND|CS_NO_MOVE|CS_TLOCK_VHARD,1,false,CR_CIRCLE	,18,-1),//すばやさアップ
	CComSkillData(	"エアースラッシュ"	,73		,CS_MAGIC_WIND	|CS_NO_MOVE		,2	,true	,CR_ALL			,28,31,-1),
	CComSkillData(	"スカイウォーカー"	,74		,CS_MAGIC_WIND	|CS_NO_MOVE		,2	,false	,CR_ONE			,56,-1),//対象に飛行を付与
	CComSkillData(	"カッティングストーム",75	,CS_MAGIC_WIND	|CS_NO_MOVE		,3	,true	,CR_ALL			,14,-1),
	CComSkillData(	"ラピッドラビット"	,76		,CS_MAGIC_WIND|CS_NO_MOVE|CS_TLOCK_VHARD,3,false,CR_ALL		,39,-1),//味方全員が即座に行動
	CComSkillData(	"トルネード"		,77		,CS_MAGIC_WIND	|CS_NO_MOVE		,4	,true	,CR_ALL			,30,28,-1),
		//土魔法
	CComSkillData(	"ダートランス"		,78		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,true	,CR_ONE_M		,15,-1),
	CComSkillData(	"グラビティー"		,79		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,35,-1),//敵を重くする
	CComSkillData(	"ストーンシールド"	,80		,CS_MAGIC_EARTH	|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,18,-1),//防御力アップ
	CComSkillData(	"ロックプレス"		,81		,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_ONE_M		,35,37,-1),
	CComSkillData(	"グランドウォール"	,82		,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_NONE		,48,49,-1),//敵と味方の間にカベを作る
	CComSkillData(	"アースクエイク"	,83		,CS_MAGIC_EARTH	|CS_NO_MOVE		,3	,true	,CR_S_ALL		,15,49,-1),
	CComSkillData(	"モールダイブ"		,84		,CS_MAGIC_EARTH	|CS_QUICK		,3	,false	,CR_ALL			,49,-1),//地面に潜って1ターンやり過ごす
	CComSkillData(	"メテオスウォーム"	,85		,CS_MAGIC_EARTH	|CS_NO_MOVE		,4	,true	,CR_ALL			,49,50,62,-1),
		//特殊魔法
	CComSkillData(	"インパクト"		,86		,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,21,22,-1),
	CComSkillData(	"レイ"				,87		,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"ブラックホール"	,88		,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_BIG_CIRCLE	,58,-1),
	CComSkillData(	"リフレクション"	,89		,CS_MAGIC		|CS_NO_MOVE		,2	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	""					,90		,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
	CComSkillData(	"エンシャントルーン",91		,CS_MAGIC|CS_QUICK|CS_TLOCK_VHARD,4	,false	,CR_ALL			,26,74,-1),
		//以下、特殊
	CComSkillData(	"応急処置"			,92		,CS_OTHER_HEAL	|CS_MOVE		,1	,false	,CR_ONE			,6,-1),
	CComSkillData(	"祝福"				,93		,CS_OTHER_HEAL	|CS_NO_MOVE		,2	,false	,CR_BIG_CIRCLE	,26,41,-1),
	CComSkillData(	"治療"				,94		,CS_OTHER_HEAL|CS_NO_MOVE|CS_TLOCK_HARD,1,false	,CR_CIRCLE	,25,26,-1),
	CComSkillData(	"生命の息吹"		,95		,CS_OTHER_HEAL	|CS_RIVIVAL		,2	,false	,CR_ONE			,75,-1),
	CComSkillData(	"炎の結界"			,96		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"氷の結界"			,97		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"風の結界"			,98		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"土の結界"			,99		,CS_OTHER_FIELD	|CS_QUICK		,1	,false	,CR_NONE		,-1),
	CComSkillData(	"力の結界"			,100	,CS_OTHER_FIELD	|CS_QUICK		,2	,false	,CR_NONE		,-1),
	CComSkillData(	"魔の結界"			,101	,CS_OTHER_FIELD	|CS_QUICK		,2	,false	,CR_NONE		,-1),
	CComSkillData(	"どくどくガス"		,102	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,20,21,-1),
	CComSkillData(	"催眠術"			,103	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,23,-1),
	CComSkillData(	"誘惑"				,104	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,26,34,-1),
	CComSkillData(	"電気ショック"		,105	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"封印"				,106	,CS_OTHER_ST	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,77,-1),
	CComSkillData(	"狂戦士のツボ"		,107	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_CIRCLE		,65,-1),
	CComSkillData(	"腐敗ガス"			,108	,CS_OTHER_ST	|CS_MOVE		,1	,true	,CR_BIG_CIRCLE	,20,21,-1),
	CComSkillData(	""					,109	,CS_OTHER		|CS_MOVE		,2	,true	,CR_ONE			,-1),
	CComSkillData(	""					,110	,CS_OTHER		|CS_MOVE		,2	,true	,CR_ONE			,-1),
	CComSkillData(	"癒しの光"			,111	,CS_OTHER_HEAL	|CS_NO_MOVE		,3	,false	,CR_ALL			,6,26,-1),
	CComSkillData(	"命の輝き"			,112	,CS_OTHER_HEAL	|CS_RIVIVAL		,4	,false	,CR_ONE			,78,41,20,-1),
	CComSkillData(	""					,113	,CS_OTHER		|CS_MOVE		,1	,true	,CR_ONE			,-1),
	CComSkillData(	"防御"				,114	,CS_OTHER		|CS_QUICK		,0	,false	,CR_SELF		,18,-1),
	CComSkillData(	"挑発"				,115	,CS_OTHER		|CS_QUICK		,1	,true	,CR_ALL			,42,-1),
	CComSkillData(	"ためる"			,116	,CS_OTHER		|CS_NO_MOVE		,0	,false	,CR_SELF		,18,-1),
	CComSkillData(	"クロボー"			,117	,CS_OTHER_PET	|CS_CALL_PET	,0	,true	,CR_NONE		,-1),
	CComSkillData(	"魔法ぼうや"		,118	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"岩男"				,119	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"癒しキノコ"		,120	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"ボムボムボーイ"	,121	,CS_OTHER_PET	|CS_CALL_PET	,1	,true	,CR_NONE		,-1),
	CComSkillData(	"ビーストウォーリア",122	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"ウッドマン"		,123	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"いたずら妖精"		,124	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"タコキング"		,125	,CS_OTHER_PET	|CS_CALL_PET	,2	,true	,CR_NONE		,-1),
	CComSkillData(	"ワイバーン"		,126	,CS_OTHER_PET	|CS_CALL_PET	,3	,true	,CR_NONE		,-1),
	CComSkillData(	""					,127	,CS_OTHER		|CS_NO_MOVE		,4	,true	,CR_NONE		,-1),
		//以下、アイテム。
	CComSkillData(	"ライフリカバリー"	,128	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,6,-1),
	CComSkillData(	"チャージスキル"	,129	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,6,-1),
	CComSkillData(	"プラントボム・Ｓ"	,130	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,10,-1),
	CComSkillData(	"プラントボム・Ｌ"	,131	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"ヒーリング"		,132	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_BIG_CIRCLE	,29,-1),
	CComSkillData(	"クリアランス"		,133	,CS_ITEM|CS_NO_MOVE|CS_TLOCK_HARD,0	,false	,CR_BIG_CIRCLE	,25,26,-1),
	CComSkillData(	"リバイバル"		,134	,CS_ITEM		|CS_RIVIVAL		,0	,false	,CR_ONE			,75,-1),
	CComSkillData(	"インスタントバリア",135	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	"カウンターニードル",136	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,47,-1),
	CComSkillData(	"ダブル"			,137	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,-1),
	CComSkillData(	"リフレクション"	,138	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"エスケープ"		,139	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_NONE		,-1),
	CComSkillData(	"ガードブレイク"	,140	,CS_ITEM		|CS_NO_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	""					,141	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
	CComSkillData(	""					,142	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
	CComSkillData(	""					,143	,CS_ITEM		|CS_NO_MOVE		,0	,false	,CR_ONE			,-1),
		//以下、敵専用。
	CComSkillData(	"体当たり"			,144	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,1,-1),
	CComSkillData(	"噛み付く"			,145	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,8,-1),
	CComSkillData(	"自爆"				,146	,CS_OTHER		|CS_MOVE		,0	,true	,CR_BIG_CIRCLE	,10,-1),
	CComSkillData(	"吸血"				,147	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,21,-1),
	CComSkillData(	"リザレクション"	,148	,CS_MAGIC		|CS_RIVIVAL		,3	,false	,CR_ALL			,75,-1),
	CComSkillData(	"インスタントバリア",149	,CS_MAGIC		|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	"養分吸収"			,150	,CS_SKILL		|CS_NO_MOVE		,1	,true	,CR_ONE_M		,33,-1),
	CComSkillData(	"まやかしの魔術師"	,151	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_NONE		,-1),
	CComSkillData(	"サソリの毒針"		,152	,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,2,-1),
	CComSkillData(	"すみ"				,153	,CS_OTHER		|CS_NO_MOVE		,1	,true	,CR_ONE			,46,-1),
	CComSkillData(	"突撃"				,154	,CS_SKILL		|CS_MOVE		,2	,true	,CR_HLINE		,-1),
	CComSkillData(	"リフレクション"	,155	,CS_MAGIC		|CS_NO_MOVE		,1	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"マグマフィールド"	,156	,CS_MAGIC_FIRE	|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,10,49,59,-1),
	CComSkillData(	"サンダーストーム"	,157	,CS_MAGIC_WIND	|CS_NO_MOVE		,4	,true	,CR_ALL			,-1),
	CComSkillData(	"ロトンスポア"		,158	,CS_MAGIC_EARTH	|CS_NO_MOVE		,4	,true	,CR_ALL			,64,-1),
	CComSkillData(	"呼び出し"			,159	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_NONE		,-1),
		//ここだけ暴走攻撃
	CComSkillData(	"暴走攻撃・Lv0"		,160	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"暴走攻撃・Lv1"		,161	,CS_SKILL		|CS_MOVE		,1	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"暴走攻撃・Lv2"		,162	,CS_SKILL		|CS_MOVE		,2	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"暴走攻撃・Lv3"		,163	,CS_SKILL		|CS_MOVE		,3	,true	,CR_ONE_M		,1,-1),
	CComSkillData(	"暴走攻撃・Lv4"		,164	,CS_SKILL		|CS_MOVE		,4	,true	,CR_ONE_M		,1,-1),
		//暴走攻撃おわり
	CComSkillData(	""					,165	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	""					,166	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"マルチ・レイ"		,167	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ALL			,-1),
	CComSkillData(	"デスサークル"		,168	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ONE			,-1),
	CComSkillData(	"フォーススフィア"	,169	,CS_MAGIC		|CS_SUPER_SLOW	,3	,true	,CR_NONE		,-1),
	CComSkillData(	"フロートブレイド"	,170	,CS_SWORD		|CS_QUICK		,2	,true	,CR_ALL			,68,-1),
	CComSkillData(	""					,171	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
	CComSkillData(	"闇の波動"			,172	,CS_MAGIC		|CS_NO_MOVE		,4	,true	,CR_ALL			,20,-1),
	CComSkillData(	"鉄球投げ"			,173	,CS_SKILL		|CS_NO_MOVE		,0	,true	,CR_ONE_M		,69,70,-1),
	CComSkillData(	""					,174	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ONE_M		,57,-1),
	CComSkillData(	"子分呼び"			,175	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_NONE		,-1),
	CComSkillData(	"誘惑の歌"			,176	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"狂騒の歌"			,177	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"死神の歌"			,178	,CS_OTHER_ST	|CS_NO_MOVE		,2	,true	,CR_ALL			,71,-1),
	CComSkillData(	"癒しの歌"			,179	,CS_OTHER_ST	|CS_NO_MOVE		,2	,false	,CR_ALL			,71,-1),
	CComSkillData(	"火炎剣"			,180	,CS_SWORD		|CS_MOVE		,2	,true	,CR_HLINE		,10,-1),
	CComSkillData(	"氷雪剣"			,181	,CS_SWORD		|CS_MOVE		,2	,true	,CR_CIRCLE		,54,11,-1),
	CComSkillData(	"風陣剣"			,182	,CS_SWORD		|CS_MOVE		,2	,true	,CR_ONE_M		,27,28,-1),
	CComSkillData(	"地隆剣"			,183	,CS_SWORD		|CS_MOVE		,2	,true	,CR_ALL			,21,-1),
	CComSkillData(	""/*ターゲッティング*/	,184	,CS_OTHER		|CS_SLOW_NO_WALK,0	,true	,CR_ONE			,76,-1),
	CComSkillData(	""					,185	,CS_SKILL		|CS_MOVE		,0	,true	,CR_ONE			,-1),
	CComSkillData(	"養分吸収"			,186	,CS_SKILL		|CS_NO_MOVE		,1	,true	,CR_ALL			,33,-1),
	CComSkillData(	"乱れ墨"			,187	,CS_OTHER		|CS_NO_MOVE		,2	,true	,CR_ALL			,46,-1),
	CComSkillData(	""/*100カウントバリア*/	,188	,CS_EITEM|0x1000*20|CS_NO_MOVE	,0	,false	,CR_ONE			,55,-1),
	CComSkillData(	"ヒーリング"		,189	,CS_EITEM|0x1000*5 |CS_NO_MOVE	,0	,false	,CR_BIG_CIRCLE	,29,-1),
	CComSkillData(	"プラントボム・Ｌ"	,190	,CS_EITEM|0x1000*14|CS_NO_MOVE	,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"ダブル"			,191	,CS_EITEM|0x1000*22|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,-1),
	CComSkillData(	"プラントボム・Ｓ"	,192	,CS_EITEM|0x1000*12|CS_NO_MOVE	,0	,true	,CR_CIRCLE		,10,-1),
	CComSkillData(	"カウンターニードル",193	,CS_EITEM|0x1000*21|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,10,-1),
	CComSkillData(	"スキルドレインLv.1",194	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"スキルドレインLv.2",195	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"スキルドレインLv.3",196	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"スキルドレインLv.4",197	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"レーザーカノン"	,198	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ONE			,21,79,-1),
	CComSkillData(	"アイテムスティール",199	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,-1),
	CComSkillData(	"小さな村の優しい人々",200	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"支えてくれた仲間達",201	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"ほのかな愛と恋人"	,202	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"名前の無い赤子"	,203	,CS_OTHER		|CS_SUPER_SLOW	,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"夢崩し"			,204	,CS_OTHER		|CS_SUPER_QUICK	,0	,false	,CR_S_ALL		,-1),
	CComSkillData(	"プラントボム・Ｌ"	,205	,CS_EITEM|0x1000*14|CS_NO_MOVE	,0	,true	,CR_BIG_CIRCLE	,50,-1),
	CComSkillData(	"夢の終わり"		,206	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	"スキルドレインLv.1",207	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"スキルドレインLv.2",208	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_CIRCLE		,6,21,54,-1),
	CComSkillData(	"死の言葉"			,209	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・毒"	,210	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・眠り"	,211	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・麻痺"	,212	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・封印"	,213	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・魅了"	,214	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・暴走"	,215	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"呪いの言葉・腐敗"	,216	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,74,-1),
	CComSkillData(	"インスタントバリア",217	,CS_EITEM|0x1000*20|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,40,-1),
	CComSkillData(	""/*リフレクション*/	,218	,CS_EITEM|0x1000*23|CS_NO_MOVE	,0	,false	,CR_CIRCLE		,55,-1),
	CComSkillData(	"リバイバル"		,219	,CS_EITEM|0x1000*7 |CS_RIVIVAL	,0	,false	,CR_ONE			,75,-1),
	CComSkillData(	""/*ステージ全体汎用*/,220	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_S_ALL		,-1),
	CComSkillData(	""/*全体攻撃汎用*/	,221	,CS_OTHER		|CS_NO_MOVE		,0	,true	,CR_ALL			,-1),
	CComSkillData(	""					,222	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,80,-1),
	CComSkillData(	"グラビティー・オール",223	,CS_MAGIC_EARTH	|CS_NO_MOVE		,2	,true	,CR_ALL			,35,-1),

	CComSkillData(	""/*ブラントウェポン*/	,224	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	""/*ルストアーマー*/	,225	,CS_MAGIC		|CS_NO_MOVE		,1	,true	,CR_CIRCLE		,-1),
	CComSkillData(	"黒死鳥"			,226	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ONE_M		,57,-1),
	CComSkillData(	"ディスペル"		,227	,CS_MAGIC		|CS_NO_MOVE		,2	,true	,CR_ALL			,80,-1),
	CComSkillData(	"サイレントワールド",228	,CS_MAGIC		|CS_NO_MOVE		,3	,true	,CR_ALL			,-1),
};

const CSkillData SkillData[MAX_SKILL_NUMBER]={
	CSkillData(		"LPアップ Lv. 1"	,0		,SKILL_STATUS	,1*0x10000	+20),
	CSkillData(		"LPアップ Lv. 2"	,1		,SKILL_STATUS	,1*0x10000	+30),
	CSkillData(		"LPアップ Lv. 3"	,2		,SKILL_STATUS	,1*0x10000	+50),
	CSkillData(		"LPアップ Lv. 4"	,3		,SKILL_STATUS	,1*0x10000	+70),
	CSkillData(		"LPアップ Lv. 5"	,4		,SKILL_STATUS	,1*0x10000	+90),
	CSkillData(		"LPアップ Lv. 6"	,5		,SKILL_STATUS	,1*0x10000	+120),
	CSkillData(		"LPアップ Lv. 7"	,6		,SKILL_STATUS	,1*0x10000	+150),
	CSkillData(		"SPアップ Lv. 1"	,7		,SKILL_STATUS	,2*0x10000	+3),
	CSkillData(		"SPアップ Lv. 2"	,8		,SKILL_STATUS	,2*0x10000	+5),
	CSkillData(		"SPアップ Lv. 3"	,9		,SKILL_STATUS	,2*0x10000	+7),
	CSkillData(		"SPアップ Lv. 4"	,10		,SKILL_STATUS	,2*0x10000	+10),
	CSkillData(		"SPアップ Lv. 5"	,11		,SKILL_STATUS	,2*0x10000	+15),
	CSkillData(		"SPアップ Lv. 6"	,12		,SKILL_STATUS	,2*0x10000	+20),
	CSkillData(		"攻撃力アップ Lv. 1",13		,SKILL_STATUS	,4*0x10000	+10),
	CSkillData(		"攻撃力アップ Lv. 2",14		,SKILL_STATUS	,4*0x10000	+15),
	CSkillData(		"攻撃力アップ Lv. 3",15		,SKILL_STATUS	,4*0x10000	+20),
	CSkillData(		"攻撃力アップ Lv. 4",16		,SKILL_STATUS	,4*0x10000	+25),
	CSkillData(		"攻撃力アップ Lv. 5",17		,SKILL_STATUS	,4*0x10000	+30),
	CSkillData(		"攻撃力アップ Lv. 6",18		,SKILL_STATUS	,4*0x10000	+40),
	CSkillData(		"攻撃力アップ Lv. 7",19		,SKILL_STATUS	,4*0x10000	+50),
	CSkillData(		"防御力アップ Lv. 1",20		,SKILL_STATUS	,8*0x10000	+5),
	CSkillData(		"防御力アップ Lv. 2",21		,SKILL_STATUS	,8*0x10000	+10),
	CSkillData(		"防御力アップ Lv. 3",22		,SKILL_STATUS	,8*0x10000	+15),
	CSkillData(		"防御力アップ Lv. 4",23		,SKILL_STATUS	,8*0x10000	+20),
	CSkillData(		"防御力アップ Lv. 5",24		,SKILL_STATUS	,8*0x10000	+25),
	CSkillData(		"防御力アップ Lv. 6",25		,SKILL_STATUS	,8*0x10000	+30),
	CSkillData(		"防御力アップ Lv. 7",26		,SKILL_STATUS	,8*0x10000	+40),
	CSkillData(		"魔法攻撃アップ Lv. 1",27	,SKILL_STATUS	,16*0x10000	+10),
	CSkillData(		"魔法攻撃アップ Lv. 2",28	,SKILL_STATUS	,16*0x10000	+15),
	CSkillData(		"魔法攻撃アップ Lv. 3",29	,SKILL_STATUS	,16*0x10000	+20),
	CSkillData(		"魔法攻撃アップ Lv. 4",30	,SKILL_STATUS	,16*0x10000	+25),
	CSkillData(		"魔法攻撃アップ Lv. 5",31	,SKILL_STATUS	,16*0x10000	+30),
	CSkillData(		"魔法攻撃アップ Lv. 6",32	,SKILL_STATUS	,16*0x10000	+40),
	CSkillData(		"魔法攻撃アップ Lv. 7",33	,SKILL_STATUS	,16*0x10000	+50),
	CSkillData(		"魔法防御アップ Lv. 1",34	,SKILL_STATUS	,32*0x10000	+5),
	CSkillData(		"魔法防御アップ Lv. 2",35	,SKILL_STATUS	,32*0x10000	+10),
	CSkillData(		"魔法防御アップ Lv. 3",36	,SKILL_STATUS	,32*0x10000	+15),
	CSkillData(		"魔法防御アップ Lv. 4",37	,SKILL_STATUS	,32*0x10000	+20),
	CSkillData(		"魔法防御アップ Lv. 5",38	,SKILL_STATUS	,32*0x10000	+25),
	CSkillData(		"魔法防御アップ Lv. 6",39	,SKILL_STATUS	,32*0x10000	+30),
	CSkillData(		"魔法防御アップ Lv. 7",40	,SKILL_STATUS	,32*0x10000	+40),
	CSkillData(		"すばやさ + 10"		,41		,SKILL_STATUS	,64*0x10000	+10),
	CSkillData(		"すばやさ + 5"		,42		,SKILL_STATUS	,64*0x10000	+5),
	CSkillData(		"すばやさ + 20"		,43		,SKILL_STATUS	,64*0x10000	+20),
	CSkillData(		"SPアップ Lv. 7"	,44		,SKILL_STATUS	,2*0x10000	+25),
	CSkillData(		"アクセサリー + 1"	,45		,SKILL_STATUS	,256*0x10000+1),
	CSkillData(		"アイテム + 2"		,46		,SKILL_STATUS	,128*0x10000+2),
	CSkillData(		"アイテム + 4"		,47		,SKILL_STATUS	,128*0x10000+4),
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
	CSkillData(		"LP +15％"			,168	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +15),
	CSkillData(		"LP +25％"			,169	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +25),
	CSkillData(		"LP +30％"			,170	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +30),
	CSkillData(		"LP +50％"			,171	,SKILL_AUTO		,AS_LP_UP	+ 0x100 +50),
	CSkillData(		"攻撃力 +15％"		,172	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +15),
	CSkillData(		"攻撃力 +25％"		,173	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +25),
	CSkillData(		"攻撃力 +30％"		,174	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +30),
	CSkillData(		"攻撃力 +50％"		,175	,SKILL_AUTO		,AS_ATK_UP	+ 0x100 +50),
	CSkillData(		"防御力 +15％"		,176	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +15),
	CSkillData(		"防御力 +25％"		,177	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +25),
	CSkillData(		"防御力 +30％"		,178	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +30),
	CSkillData(		"防御力 +50％"		,179	,SKILL_AUTO		,AS_DEF_UP	+ 0x100 +50),
	CSkillData(		"魔法攻撃 +15％"	,180	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +15),
	CSkillData(		"魔法攻撃 +25％"	,181	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +25),
	CSkillData(		"魔法攻撃 +30％"	,182	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +30),
	CSkillData(		"魔法攻撃 +50％"	,183	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 +50),
	CSkillData(		"魔法防御 +15％"	,184	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +15),
	CSkillData(		"魔法防御 +25％"	,185	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +25),
	CSkillData(		"魔法防御 +30％"	,186	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +30),
	CSkillData(		"魔法防御 +50％"	,187	,SKILL_AUTO		,AS_MDEF_UP	+ 0x100 +50),
	CSkillData(		""/*すばやさ +15％*/	,188	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +15),
	CSkillData(		"すばやさ +25％"	,189	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +25),
	CSkillData(		"すばやさ +30％"	,190	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +30),
	CSkillData(		"すばやさ +50％"	,191	,SKILL_AUTO		,AS_SPD_UP	+ 0x100 +50),
	CSkillData(		""/*SP +15％*/			,192	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +15),
	CSkillData(		"SP +25％"			,193	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +25),
	CSkillData(		"SP +30％"			,194	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +30),
	CSkillData(		"SP +50％"			,195	,SKILL_AUTO		,AS_TECH_UP	+ 0x100 +50),
	CSkillData(		"炎ダメージ -50％"	,196	,SKILL_AUTO		,AS_FIRE_GUARD	+50),
	CSkillData(		"炎ダメージ -90％"	,197	,SKILL_AUTO		,AS_FIRE_GUARD	+90),
	CSkillData(		"炎ダメージ +50％"	,198	,SKILL_AUTO		,AS_FIRE_GUARD	+0xffff-50),
	CSkillData(		"炎ダメージ +100％"	,199	,SKILL_AUTO		,AS_FIRE_GUARD	+0xffff-100),
	CSkillData(		"氷ダメージ -50％"	,200	,SKILL_AUTO		,AS_ICE_GUARD	+50),
	CSkillData(		"氷ダメージ -90％"	,201	,SKILL_AUTO		,AS_ICE_GUARD	+90),
	CSkillData(		"氷ダメージ +50％"	,202	,SKILL_AUTO		,AS_ICE_GUARD	+0xffff-50),
	CSkillData(		"氷ダメージ +100％"	,203	,SKILL_AUTO		,AS_ICE_GUARD	+0xffff-100),
	CSkillData(		"風ダメージ -50％"	,204	,SKILL_AUTO		,AS_WIND_GUARD	+50),
	CSkillData(		"風ダメージ -90％"	,205	,SKILL_AUTO		,AS_WIND_GUARD	+90),
	CSkillData(		"風ダメージ +50％"	,206	,SKILL_AUTO		,AS_WIND_GUARD	+0xffff-50),
	CSkillData(		"風ダメージ +100％"	,207	,SKILL_AUTO		,AS_WIND_GUARD	+0xffff-100),
	CSkillData(		"土ダメージ -50％"	,208	,SKILL_AUTO		,AS_EARTH_GUARD	+50),
	CSkillData(		"土ダメージ -90％"	,209	,SKILL_AUTO		,AS_EARTH_GUARD	+90),
	CSkillData(		"土ダメージ +50％"	,210	,SKILL_AUTO		,AS_EARTH_GUARD	+0xffff-50),
	CSkillData(		"土ダメージ +100％"	,211	,SKILL_AUTO		,AS_EARTH_GUARD	+0xffff-100),
	CSkillData(		"ST防御・毒"		,212	,SKILL_AUTO		,AS_STATUS_GUARD	+1),
	CSkillData(		"ST防御・眠り"		,213	,SKILL_AUTO		,AS_STATUS_GUARD	+2),
	CSkillData(		"ST防御・魅了"		,214	,SKILL_AUTO		,AS_STATUS_GUARD	+4),
	CSkillData(		"ST防御・麻痺"		,215	,SKILL_AUTO		,AS_STATUS_GUARD	+8),
	CSkillData(		"ST防御・封印"		,216	,SKILL_AUTO		,AS_STATUS_GUARD	+16),
	CSkillData(		"ST防御・暴走"		,217	,SKILL_AUTO		,AS_STATUS_GUARD	+32),
	CSkillData(		"ST防御・腐敗"		,218	,SKILL_AUTO		,AS_STATUS_GUARD	+64),
	CSkillData(		"ST防御・即死"		,219	,SKILL_AUTO		,AS_STATUS_GUARD	+128),
	CSkillData(		"追加効果・毒"		,220	,SKILL_AUTO		,AS_ADD_STATUS	+1),
	CSkillData(		"追加効果・眠り"	,221	,SKILL_AUTO		,AS_ADD_STATUS	+2),
	CSkillData(		"追加効果・魅了"	,222	,SKILL_AUTO		,AS_ADD_STATUS	+4),
	CSkillData(		"追加効果・麻痺"	,223	,SKILL_AUTO		,AS_ADD_STATUS	+8),
	CSkillData(		"追加効果・封印"	,224	,SKILL_AUTO		,AS_ADD_STATUS	+16),
	CSkillData(		"追加効果・暴走"	,225	,SKILL_AUTO		,AS_ADD_STATUS	+32),
	CSkillData(		"追加効果・腐敗"	,226	,SKILL_AUTO		,AS_ADD_STATUS	+64),
	CSkillData(		"追加効果・即死"	,227	,SKILL_AUTO		,AS_ADD_STATUS	+128),
	CSkillData(		"クリティカル +5％"	,228	,SKILL_AUTO		,AS_CRT_UP		+5),
	CSkillData(		"クリティカル +10％",229	,SKILL_AUTO		,AS_CRT_UP		+10),
	CSkillData(		"クリティカル +15％",230	,SKILL_AUTO		,AS_CRT_UP		+15),
	CSkillData(		"回避 +10％"		,231	,SKILL_AUTO		,AS_AVOID_UP	+10),
	CSkillData(		"回避 +15％"		,232	,SKILL_AUTO		,AS_AVOID_UP	+15),
	CSkillData(		"回避 +20％"		,233	,SKILL_AUTO		,AS_AVOID_UP	+20),
	CSkillData(		"獲得金額 +30％"	,234	,SKILL_AUTO		,AS_MONEY_UP	+30),
	CSkillData(		"獲得金額 +50％"	,235	,SKILL_AUTO		,AS_MONEY_UP	+50),
	CSkillData(		"獲得金額 +70％"	,236	,SKILL_AUTO		,AS_MONEY_UP	+70),
	CSkillData(		"獲得金額 +100％"	,237	,SKILL_AUTO		,AS_MONEY_UP	+100),
	CSkillData(		"獲得経験値 +50％"	,238	,SKILL_AUTO		,AS_EXP_UP		+50),
	CSkillData(		"獲得経験値 +100％"	,239	,SKILL_AUTO		,AS_EXP_UP		+100),
	CSkillData(		"獲得経験値 +150％"	,240	,SKILL_AUTO		,AS_EXP_UP		+150),
	CSkillData(		"獲得経験値 +200％"	,241	,SKILL_AUTO		,AS_EXP_UP		+200),
	CSkillData(		"飛行"				,242	,SKILL_AUTO		,AS_MOVE+1),
	CSkillData(		""/*テレポート*/		,243	,SKILL_AUTO		,AS_MOVE+1),
	CSkillData(		"防御力半減"		,244	,SKILL_AUTO		,AS_DEF_BREAK		+50),
	CSkillData(		"防御力無視"		,245	,SKILL_AUTO		,AS_DEF_BREAK		+100),
	CSkillData(		"魔法防御半減"		,246	,SKILL_AUTO		,AS_MDEF_BREAK		+50),
	CSkillData(		"魔法防御無視"		,247	,SKILL_AUTO		,AS_MDEF_BREAK		+100),
	CSkillData(		""/*属性防御半減*/		,248	,SKILL_AUTO		,AS_ELEMENT_BREAK	+50),	//スキル番号も使ってるので注意！
	CSkillData(		"属性防御無視"		,249	,SKILL_AUTO		,AS_ELEMENT_BREAK	+100),	//スキル番号も使ってるので注意！
	CSkillData(		"精密動作"			,250	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"HP自動回復"		,251	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"フリースペース"	,252	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"魔剣の加護"		,253	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*魔盾の加護*/		,254	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ダメージブースター",255	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"属性付与・炎"		,256	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_FIRE),
	CSkillData(		"属性付与・氷"		,257	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_ICE),
	CSkillData(		"属性付与・風"		,258	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_WIND),
	CSkillData(		"属性付与・土"		,259	,SKILL_AUTO		,AS_ADD_ELEMENT	+AT_ELE_EARTH),
	CSkillData(		""/*HP回復量 +50％*/	,260	,SKILL_AUTO		,AS_HP_HEAL_UP	+50),
	CSkillData(		"聖女の祈り"		,261	,SKILL_AUTO		,AS_HP_HEAL_UP	+50),
	CSkillData(		" --- "/*習得不能*/	,262	,SKILL_NONE		,AS_OTHER),
	CSkillData(		""/*LPダメージ -20％*/	,263	,SKILL_AUTO		,AS_LP_GUARD	+20),
	CSkillData(		""/*LPダメージ -40％*/	,264	,SKILL_AUTO		,AS_LP_GUARD	+40),
	CSkillData(		""/*LPダメージ -60％*/	,265	,SKILL_AUTO		,AS_LP_GUARD	+60),
	CSkillData(		""/*LPダメージ -80％*/	,266	,SKILL_AUTO		,AS_LP_GUARD	+80),
	CSkillData(		"鋼の守り"			,267	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"韋駄天"			,268	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		""/*くすねる*/			,269	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		""					,270	,SKILL_FIELD	,95),
	CSkillData(		"植物学"			,271	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"脱兎"				,272	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"セカンドスペース"	,273	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"鷹の目"			,274	,SKILL_FIELD	,10),
	CSkillData(		"早歩き"			,275	,SKILL_FIELD	,20),
	CSkillData(		"ハイジャンプ"		,276	,SKILL_FIELD	,30),
	CSkillData(		"スーパーダッシュ"	,277	,SKILL_FIELD	,40),
	CSkillData(		"カギ開け・紫-青"	,278	,SKILL_FIELD	,50),
	CSkillData(		"カギ開け・水-緑-黄",279	,SKILL_FIELD	,60),
	CSkillData(		"カギ開け・橙-赤"	,280	,SKILL_FIELD	,70),
	CSkillData(		"空中浮遊"			,281	,SKILL_FIELD	,35),
	CSkillData(		"探索者の直感"		,282	,SKILL_FIELD	,90),
	CSkillData(		"トラップ解除"		,283	,SKILL_FIELD	,100),
	CSkillData(		"忍び足"			,284	,SKILL_FIELD	,25),
	CSkillData(		"すり抜け"			,285	,SKILL_FIELD	,15),
	CSkillData(		"危険予知"			,286	,SKILL_FIELD	,120),
	CSkillData(		"がまん"			,287	,SKILL_FIELD	,130),
	CSkillData(		"投石"				,288	,SKILL_FIELD	,140),
	CSkillData(		"二刀流"			,289	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"巻き込み"			,290	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"生物学"			,291	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"足止め"			,292	,SKILL_AUTO		,AS_ADD_STATUS_OT),
	CSkillData(		"ダッシュ不能"		,293	,SKILL_FIELD	,45),
	CSkillData(		""		,298	,SKILL_FIELD	,170),
	CSkillData(		""		,299	,SKILL_FIELD	,180),
	CSkillData(		"ブロック押し"		,296	,SKILL_FIELD	,150),
	CSkillData(		"ブロック押し ＋2"	,297	,SKILL_FIELD	,160),
	CSkillData(		"炎ダメージ -40％"	,298	,SKILL_AUTO		,AS_FIRE_GUARD	+40),
	CSkillData(		"氷ダメージ -40％"	,299	,SKILL_AUTO		,AS_ICE_GUARD	+40),
	CSkillData(		"風ダメージ -40％"	,300	,SKILL_AUTO		,AS_WIND_GUARD	+40),
	CSkillData(		"土ダメージ -40％"	,301	,SKILL_AUTO		,AS_EARTH_GUARD	+40),
	CSkillData(		"木こり"			,302	,SKILL_FIELD	,190),
	CSkillData(		"攻撃力固定"		,303	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ダブルヒット"		,304	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"正体不明"			,305	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"まやかし"			,306	,SKILL_AUTO		,AS_OPTION+1),
	CSkillData(		"ダメージレデューサー",307	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"ライフリカバリー"	,308	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"オールラウンド"	,309	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*魔術師の心得*/		,310	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		""/*戦士の心得*/		,311	,SKILL_AUTO		,AS_PARAM_UP_OTHER),
	CSkillData(		"スキルマスター"	,312	,SKILL_AUTO		,AS_FS_OTHER),
	CSkillData(		"先見の明"			,313	,SKILL_FIELD	,990),
	CSkillData(		"土産話"			,314	,SKILL_AUTO		,AS_EXP_UP_OTHER),
	CSkillData(		"初心戻し"			,315	,SKILL_FIELD	,1000),
	CSkillData(		"元気印"			,316	,SKILL_AUTO		,AS_ST_OTHER),
	CSkillData(		"商才＋1"			,317	,SKILL_FIELD	,1010),
	CSkillData(		"商才＋2"			,318	,SKILL_FIELD	,1020),
	CSkillData(		"土地勘・春"		,319	,SKILL_FIELD	,980),
	CSkillData(		"促成栽培"			,320	,SKILL_FIELD	,1030),
	CSkillData(		""				,321	,SKILL_AUTO		,AS_BANSEI+1),
	CSkillData(		""				,322	,SKILL_AUTO		,AS_BANSEI+2),
	CSkillData(		""				,323	,SKILL_AUTO		,AS_BANSEI+3),
	CSkillData(		""				,324	,SKILL_AUTO		,AS_BANSEI+4),
	CSkillData(		""				,325	,SKILL_AUTO		,AS_BANSEI+5),
	CSkillData(		""				,326	,SKILL_AUTO		,AS_BANSEI+6),
	CSkillData(		""				,327	,SKILL_AUTO		,AS_BANSEI+7),
	CSkillData(		""/*魔法攻撃固定*/		,328	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"追加効果・攻撃↓"	,329	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_ATK),
	CSkillData(		"追加効果・防御↓"	,330	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_DEF),
	CSkillData(		"追加効果・魔攻↓"	,331	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_MATK),
	CSkillData(		"追加効果・魔防↓"	,332	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_MDEF),
	CSkillData(		"追加効果・早さ↓"	,333	,SKILL_AUTO		,AS_ADD_ST_DOWN	+UP_SPD),
	CSkillData(		"サボテンの針"		,334	,SKILL_AUTO		,AS_COUNTER + AT_ELE_NONE),
	CSkillData(		""/*ド根性*/			,335	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"パラメータ低下防御",336	,SKILL_AUTO		,AS_SPECIAL_GUARD),//パラメータが下がらない。
	CSkillData(		"対空攻撃"			,337	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"土地勘・夏"		,338	,SKILL_FIELD	,981),
	CSkillData(		"土地勘・秋"		,339	,SKILL_FIELD	,982),
	CSkillData(		"土地勘・冬"		,340	,SKILL_FIELD	,983),
	CSkillData(		"水上移動"			,341	,SKILL_FIELD	,33),
	CSkillData(		"スタートダッシュ"	,342	,SKILL_AUTO		,AS_DMG_BOOST),
	CSkillData(		"鉄壁"				,343	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"雷神の術"			,344	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"樹霊の術"			,345	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"属性効果アップ"	,346	,SKILL_AUTO		,AS_ELE_POW_UP +100),
	CSkillData(		"対空×"			,347	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"マジッククリティカル",348	,SKILL_AUTO		,AS_CRT_OTHER),
	CSkillData(		"エレメントチェンジ",349	,SKILL_AUTO		,AS_ELE_OTHER),
	CSkillData(		"マジックリフレクト",350	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"刃こぼれ"			,351	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"クリティカル +25％",352	,SKILL_AUTO		,AS_CRT_UP		+25),
	CSkillData(		""/*ギャンブラー*/		,353	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"技魔法反転"		,354	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"抜け道探索"		,355	,SKILL_FIELD	,985),
	CSkillData(		"剣技の心得"		,356	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_SWORD),
	CSkillData(		"槍技の心得"		,357	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_SPEAR),
	CSkillData(		"斧技の心得"		,358	,SKILL_AUTO		,AS_WEAPON_MASTER + CS_AXE),
	CSkillData(		"魔法攻撃 -50％"	,359	,SKILL_AUTO		,AS_MATK_UP	+ 0x100 -50),
	CSkillData(		"不器用"			,360	,SKILL_FIELD	,102),
	CSkillData(		"クリティカル防御"	,361	,SKILL_AUTO		,AS_SPECIAL_GUARD),
	CSkillData(		"キャンセル防御"	,362	,SKILL_AUTO		,AS_SPECIAL_GUARD),
	CSkillData(		"対巨攻撃"			,363	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"クリティカル +3％"	,364	,SKILL_AUTO		,AS_CRT_UP		+3),
	CSkillData(		"アンデッド"		,365	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"回避 +3％"			,366	,SKILL_AUTO		,AS_AVOID_UP	+3),
	CSkillData(		"回避 +5％"			,367	,SKILL_AUTO		,AS_AVOID_UP	+5),
	CSkillData(		"対不死攻撃"		,368	,SKILL_AUTO		,AS_ANTI_ATTACK),
	CSkillData(		"重装備"			,369	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ST異常回避 30％"	,370	,SKILL_AUTO		,AS_ANTI_STATUS	+30),
	CSkillData(		"ST異常回避 50％"	,371	,SKILL_AUTO		,AS_ANTI_STATUS	+50),
	CSkillData(		"ST異常回避 80％"	,372	,SKILL_AUTO		,AS_ANTI_STATUS	+80),
	CSkillData(		"ST異常回避 100％"	,373	,SKILL_AUTO		,AS_ANTI_STATUS	+100),
	CSkillData(		"アイテム投げ"		,374	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		"ST異常回避 20％"	,375	,SKILL_AUTO		,AS_ANTI_STATUS	+20),
	CSkillData(		"ST異常回避 40％"	,376	,SKILL_AUTO		,AS_ANTI_STATUS	+40),
	CSkillData(		"クリティカル +100％",377	,SKILL_AUTO		,AS_CRT_UP		+100),
	CSkillData(		"奇術師の秘術"		,378	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"二回行動"			,379	,SKILL_AUTO		,AS_MULTI_ACTION+2),
	CSkillData(		"三回行動"			,380	,SKILL_AUTO		,AS_MULTI_ACTION+3),
	CSkillData(		"四回行動"			,381	,SKILL_AUTO		,AS_MULTI_ACTION+4),
	CSkillData(		"裏切り"			,382	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"光学迷彩"			,383	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"無属性ダメージ -50％",384	,SKILL_AUTO		,AS_NO_ELE_GUARD	+50),
	CSkillData(		"執念"				,385	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		""/*誘惑の歌*/		,386	,SKILL_COMMAND	,176),
	CSkillData(		""/*炎の剣*/		,387	,SKILL_COMMAND	,180),
	CSkillData(		""/*氷の剣*/		,388	,SKILL_COMMAND	,181),
	CSkillData(		""/*風の剣*/		,389	,SKILL_COMMAND	,182),
	CSkillData(		""/*土の剣*/		,390	,SKILL_COMMAND	,183),
	CSkillData(		"ガラスの体"		,391	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ＳＰ消費０"		,392	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"死霊術師の秘儀"	,393	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"LP -50％"			,394	,SKILL_AUTO		,AS_LP_UP	+ 0x100 -50),
	CSkillData(		"鈍足"				,395	,SKILL_AUTO		,AS_ACT_ORDER),
	CSkillData(		"炎ダメージ吸収"	,396	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_FIRE),
	CSkillData(		"氷ダメージ吸収"	,397	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_ICE),
	CSkillData(		"風ダメージ吸収"	,398	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_WIND),
	CSkillData(		"土ダメージ吸収"	,399	,SKILL_AUTO		,AS_ELE_HEAL+AT_ELE_EARTH),
	CSkillData(		"ST防御無視"		,400	,SKILL_AUTO		,AS_ST_GUARD_BREAK),
	CSkillData(		"サードスペース"	,401	,SKILL_AUTO		,AS_FREE_SPACE +1),
	CSkillData(		"全体化"			,402	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ひとりぼっちの夢"	,403	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"博愛"				,404	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"爪とぎ"			,405	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"毛づくろい"		,406	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"お散歩"			,407	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		"クイック呼び出し"	,408	,SKILL_AUTO		,AS_PET_SKILL),
	CSkillData(		""/*サンダースト*/	,409	,SKILL_COMMAND	,157),
	CSkillData(		""/*ロトンスポア*/	,410	,SKILL_COMMAND	,158),
	CSkillData(		""/*スキルドレ１*/	,411	,SKILL_COMMAND	,207),
	CSkillData(		""/*スキルドレ２*/	,412	,SKILL_COMMAND	,208),
	CSkillData(		""/*養分吸収Ａ*/	,413	,SKILL_COMMAND	,150),
	CSkillData(		""/*養分吸収Ｂ*/	,414	,SKILL_COMMAND	,186),
	CSkillData(		""/*すみ*/			,415	,SKILL_COMMAND	,153),
	CSkillData(		""/*乱れすみ*/		,416	,SKILL_COMMAND	,187),
	CSkillData(		""/*自爆*/			,417	,SKILL_COMMAND	,146),
	CSkillData(		""/*体当たり*/		,418	,SKILL_COMMAND	,144),
	CSkillData(		"大吸収"			,419	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"ST異常回避 99％"	,420	,SKILL_AUTO		,AS_ANTI_STATUS	+99),
	CSkillData(		"高熱体"			,421	,SKILL_AUTO		,AS_COUNTER + AT_ELE_FIRE),
	CSkillData(		"回避 +100％"		,422	,SKILL_AUTO		,AS_AVOID_UP	+100),
	CSkillData(		"ゴールデンパワー"	,423	,SKILL_AUTO		,AS_OTHER),
	CSkillData(		"氷結の針"			,424	,SKILL_AUTO		,AS_COUNTER + AT_ELE_ICE),
	CSkillData(		"詩人の囁き"		,425	,SKILL_AUTO		,AS_SPECIAL_UP),
	CSkillData(		"パーマネンス"		,426	,SKILL_AUTO		,AS_ST_OTHER),
	CSkillData(		""/*強打*/			,427	,SKILL_COMMAND	,48),
	CSkillData(		""/*復讐の一撃*/	,428	,SKILL_COMMAND	,49),
	CSkillData(		""/*ボコボコ*/		,429	,SKILL_COMMAND	,50),
	CSkillData(		""/*黒死鳥*/		,430	,SKILL_COMMAND	,226),
	CSkillData(		""/*ディスペル*/	,431	,SKILL_COMMAND	,227),
	CSkillData(		""/*Ｓワールド*/	,432	,SKILL_COMMAND	,228),
};
const CStatusData	StatusData[MAX_STATUS_NUMBER]=
{
	CStatusData("毒"			,0,	"POISON"	,true	,false	,255	,0xff40b040,SE_POISON),
	CStatusData("眠り"			,1,	"SLEEP"		,true	,false	,4		,0xffb0b0ff,SE_SLEEP),
	CStatusData("魅了"			,2,	"CHARM"		,true	,false	,4		,0xffff8080,SE_CHARM),
	CStatusData("麻痺"			,3,	"PARALYSE"	,true	,false	,8		,0xffffff00,SE_PARALYSE),
	CStatusData("封印"			,4,	"SEAL"		,true	,false	,8		,0xff4040ff,SE_SEAL),
	CStatusData("暴走"			,5,	"BERSERK"	,true	,false	,8		,0xffff0000,SE_BERSERK),
	CStatusData("腐敗"			,6,	"ROT"		,true	,false	,255	,0xffff00ff,SE_ROT),
	CStatusData("即死"			,7,	"DEATH"		,true	,false	,255	,0xff808080,-1),
	CStatusData("ためる"		,8,	"CHARGE!"	,false	,false	,2		,0xffff8000,SE_PARAM_UP),
	CStatusData("重力"			,9,	"GRAVITY"	,false	,true	,3		,0xff8080b0,SE_GRAVITY),
	CStatusData("ブースター"	,10,"BOOST!"	,false	,false	,1		,0xffff8000,SE_FIRE),
	CStatusData("ラピッド"		,11,"RAPID!"	,false	,false	,1		,0xff00ff00,SE_PARAM_UP),
	CStatusData("オートヒール"	,12,"HEAL!"		,false	,true	,5		,0xff0080ff,SE_HEAL),
	CStatusData("スカイウォーク",13,"SKYWALK!"	,false	,false	,255	,0xff00ff00,SE_HEAL),
	CStatusData("モール"		,14,"DIVE!"		,false	,false	,100	,0xffff8000,SE_DIVE),
	CStatusData("防御"			,15,"GUARD!"	,false	,false	,1		,0xffff8000,SE_PARAM_UP),
	CStatusData("バリア"		,16,"BARRIER!"	,false	,true	,1		,0xff80ff80,SE_MAGIC),
	CStatusData("サボテンの針"	,17,"NEEDLE!"	,false	,true	,255	,0xffffb000,SE_HARD_SLASH),
	CStatusData("すみ"			,18,"INK"		,true	,false	,255	,0xff808080,SE_INK),
	CStatusData("分身"			,19,"DOUBLE!"	,false	,true	,1		,0xffffffff,SE_MAGIC),
	CStatusData("反射"			,20,"REFLECT!"	,false	,true	,1		,0xff00ffff,SE_MAGIC),
	CStatusData("挑発"			,21,"!!!"		,true	,false	,1		,0xffff0000,-1),
	CStatusData("フェニックス"	,22,"PHOENIX!"	,false	,false	,1		,0xffffff40,SE_HEAL),
	CStatusData("不死"			,23,"UNDEAD..."	,false	,false	,1		,0xff202020,SE_ROT),
	CStatusData("必殺"			,24,"CRITICAL!"	,false	,false	,1		,0xffffff00,SE_PARAM_UP),
	CStatusData(""				,25,"TARGET!"	,false	,false	,2		,0xffff0000,-1),
	CStatusData("気絶"			,26,"STUN!"		,true	,false	,1		,0xffff8000,SE_HIT),
};