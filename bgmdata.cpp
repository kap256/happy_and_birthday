
#include "bgmdata.h"


CBGMData::CBGMData()
{
	mName="";
	mID=0;
	mLoopStart=0;
	mSort=NOT_PLAY;
}
CBGMData::CBGMData(char *name, char *subname, int id, int loopstart, int sort)
{
	mName=subname;
	mID=id;
	mLoopStart=loopstart;
	mSort=sort;
}
const CBGMData BGMData[BGM_MAX]=
{
	CBGMData(
		" - - - ",
		" - - - ",0,
		0,		//ループ
		NOT_PLAY	//曲順
	),
	CBGMData(
		"イベント・日常",
		"Lecture & Tutorial",1,		//イベント・のどか
		6000,	//ループ
		EVENT+0		//曲順
	),
	CBGMData(
		"小さな村",
		"Home Town",2,				//街
		0,		//ループ
		TOWN+0		//曲順
	),
	CBGMData(
		"砂漠",
		"砂漠",3,					//砂漠
		0,		//ループ
		NOT_PLAY	//曲順
	),
//4
	CBGMData(
		"ダンジョン・洞窟",
		"Dungeon & Caverns",4,			//洞窟
		4001,	//ループ
		FIELD+50	//曲順
	),
	CBGMData(
		"ダンジョン・遺跡２",
		"Ruins & Labylinth",5,		//遺跡
		4365,	//ループ
		FIELD+70	//曲順
	),
	CBGMData(
		"ダンジョン・森林",
		"Fairy & Forest",6,			//妖精の森
		20571,	//ループ
		FIELD+20	//曲順
	),
	CBGMData(
		"砂漠の街",
		"砂漠の町",7,				//砂漠の町
		0,		//ループ
		NOT_PLAY	//曲順
	),
//8
	CBGMData(
		"バトル１",
		"Battle 1",8,				//戦闘
		8929,	//ループ
		BATTLE+0	//曲順
	),
	CBGMData(
		"新年祭",
		"Happy & Newyear",	9,				//新年祭
		10591,		//ループ
		TOWN+10 	//曲順
	),
	CBGMData(
		"",
		"",		10,					//雨（環境音）
		0,		//ループ
		NOT_PLAY	//曲順
	),
	CBGMData(
		"ダンジョン・屋外",
		"Field & Adventure",11,		//草原
		1933,	//ループ
		FIELD+0		//曲順
	),
//12
	CBGMData(
		"ダンジョン・海洋",
		"Mermaid & Whirls",12,		//人魚
		10270,	//ループ
		FIELD+40	//曲順
	),
	CBGMData(
		"ダンジョン・遺跡１",
		"Royal & Graves",	13,		//ピラミッド
		3871,	//ループ
		FIELD+50	//曲順
	),
	CBGMData(
		"南の島",
		"南の島",			14,		//南の島
		0,		//ループ
		NOT_PLAY	//曲順
	),
	CBGMData(
		"イベント・神聖",
		"Ancient & Calling",15,			//教会
		8000,		//ループ
		EVENT+35	//曲順
	),
//16
	CBGMData(
		"バトル２",
		"Battle 2",		16,			//ボスバトル
		0,		//ループ
		BATTLE+10	//曲順
	),
	CBGMData(
		"",
		"",				17,			//夜の音（環境音）
		0,		//ループ
		NOT_PLAY	//曲順
	),
	CBGMData(
		"バトル３",
		"Last Battle",	18,			//ラストバトル
		23167,		//ループ
		BATTLE+20	//曲順
	),
	CBGMData(
		"",
		"",				19,			//風（環境音）
		0,		//ループ
		NOT_PLAY	//曲順
	),
//20
	CBGMData(
		"イベント・恐怖",
		"Knocked & Carriage",20,			//イベント・不穏
		31314,		//ループ
		EVENT+5	//曲順
	),
	CBGMData(
		"イベント・幽夜",
		"Dusky & Night",21,			//イベント・暗黒
		4800,		//ループ
		EVENT+30	//曲順
	),
	CBGMData(
		"イベント・悲哀",
		"Rainy & Regret",22,			//イベント・悲しい
		26174,		//ループ
		EVENT+40	//曲順
	),
	CBGMData(
		"スタッフロール",
		"Staff & Roll",23,				//イベント・スタッフロール
		-1,		//ループ
		ENDING+10	//曲順
	),
//24
	CBGMData(
		"",
		"イベント・喜び",24,			//イベント・喜び
		0,		//ループ
		NOT_PLAY	//曲順
	),
	CBGMData(
		"タイトル",
		"Happy & Birthday",25,			//タイトル
		32005,		//ループ
		1	//曲順
	),
	CBGMData(
		"イベント・夢の卵",
		"Hearty & Courage",26,			//イベント・ラスボス直前
		29204,		//ループ
		EVENT+100	//曲順
	),
	CBGMData(
		"エンディング",
		"Happy & Birthday (off melody)",27,	//共通エンディング
		10667,		//ループ
		ENDING+500	//曲順
	),
//28
	CBGMData(
		"イベント・不穏",
		"Wicked & Boding",28,			//イベント・不穏
		4800,		//ループ
		EVENT+4	//曲順
	),
	CBGMData(
		"ダンジョン・村落",
		"Invasion & Interception",29,			//ダンジョン・村落
		9500,		//ループ
		FIELD+80	//曲順
	),
};