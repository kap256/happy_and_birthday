#include "mapconst.h"

const D3DCOLOR cMapBGColor[MAX_BG_COLOR][2]={
	{0xff0080ff,0xff00d0ff},
	{0xff404040,0xff000000},
	{0xff000080,0xff000000},
	{0xffffb000,0xffff4000},
	{0xff00d000,0xff004000},
	{0xffd00000,0xff400000},
	{0,0},
	{0xff000000,0xff000000},
};

const int MapChipParam[MAP_CHIP_NO]={
	MCHIP_SYSTEM,
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_MOVE,
	MCHIP_GROUND | MCHIP_WATER | MCHIP_ANIMATION_FAST,
//4
	MCHIP_GROUND,
	MCHIP_GROUND,
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_MOVE,
//8
	MCHIP_GROUND | MCHIP_NO_WALK | MCHIP_ANIMATION_FAST,
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_BORDER | MCHIP_NO_VIEW_OBST,
	MCHIP_NO_BORDER | MCHIP_NO_VIEW_OBST,
//12
	MCHIP_GROUND | MCHIP_ANIMATION_FAST,
	MCHIP_NO_BORDER | MCHIP_NO_VIEW_OBST | MCHIP_FENCE | MCHIP_NO_SHADOW | MCHIP_THRU_SHADOW,
	MCHIP_GROUND | MCHIP_ANIMATION | MCHIP_CONVEYER,
	MCHIP_GROUND | MCHIP_ANIMATION | MCHIP_CONVEYER,
//16
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_MOVE,
	MCHIP_GROUND | MCHIP_NO_BORDER | MCHIP_NO_VIEW_OBST,
	MCHIP_NO_VIEW_OBST | MCHIP_NO_SHADOW | MCHIP_THRU_SHADOW,
//20
	MCHIP_GROUND,
	MCHIP_V_GROUND | MCHIP_NO_VIEW_OBST | MCHIP_NO_BORDER,
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_MOVE,
//24
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_NO_BORDER | MCHIP_GET_SHADOW,
	MCHIP_GROUND | MCHIP_WATER | MCHIP_ANIMATION_FAST,
	MCHIP_GROUND,
//28
	MCHIP_GROUND,
	MCHIP_GROUND | MCHIP_ANIMATION,
};
const int cPlayBGMMap[64]={
	 0, 1, 2,11, 4, 5, 6, 9, 8, 9,
	10,11,12,13,6,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,


//32
	28,		//不穏、一般
	-1,		//不穏、盗賊戦
	1,		//のどか、タネ発見
	-1,		//問題解決、
//36
	20,		//黒衣
	15,		//リティスのテーマ
	6,		//妖精のテーマ
	1,		//ラニーのテーマ
//40
	-1,		//ショック、宝が無かった
	20,		//ディアノのテーマ
	28,		//砂漠の盗賊
	28,		//砂漠の少年
//44
	28,		//悲しみ、人の死
	1,		//水上遺跡、いろいろと発見		
	15,		//聖石のテーマ
	2,		//ミゼリアのテーマ
//48
	28,		//聖石を求めて
	15,		//タマゴのテーマ
	6,		//マリンの墓参り
	0,		//不穏、レーナvsリティス
//52
	26,		//タマゴ起動
	6,		//フィルミナのテーマ
	22,		//墓場
	22,		//悲しみ、レーナのテーマ
//56
	22,		//夢のタマゴ内部
	21,		//黒衣・幹部
	0,
	0,
//60
	0,
	0,
	0,
	0,		//無音にする（0と同義）
//64

/*
//32
	20,		//不穏、１月の商人、
	20,		//不穏、盗賊戦
	1,		//のどか、タネ発見
	24,		//問題解決、
//36
	21,		//黒衣
	15,		//リティスのテーマ
	1,		//妖精のテーマ
	1,		//ラニーのテーマ
//40
	20,		//ショック、宝が無かった
	21,		//ディアノのテーマ
	20,		//砂漠の盗賊
	24,		//砂漠の少年
//44
	22,		//悲しみ、人の死
	1,		//水上遺跡、いろいろと発見		
	15,		//聖石のテーマ
	2,		//ミゼリアのテーマ
//48
	20,		//聖石を求めて
	15,		//タマゴのテーマ
	6,		//マリンの墓参り
	20,		//不穏、レーナvsリティス
//52
	15,		//タマゴ起動
	6,		//フィルミナのテーマ
	19,		//墓場
	22,		//悲しみ、レーナのテーマ
//56
	22,		//夢のタマゴ内部
	0,
	0,
	0,
//60
	0,
	0,
	0,
	0,		//無音にする（0と同義）
//64
*/
};