
#include "eventdata.h"
#include "number.h"
#include "mapconst.h"
#include "flag.h"



const CEventHeaderInfo	EventHeaderInfo[MAX_EVENT_HEADER]={
	{EH_LOAD_MAP	,"LoadMap"},
	{EH_HIT			,"Hit"},
	{EH_TALK		,"Talk"},
	{EH_ENCOUNT		,"Encount"},
	{EH_WALK		,"Walk"},
	{EH_TIME_FLAME	,"TimeFlame"},
	{EH_TIME_SEC	,"TimeSec"},		//未定義
	{EH_FALL		,"Fall"},
	{EH_GOAL		,"Goal"},
};

const CEventDataInfo		EventDataInfo[MAX_EVENT_PATTERN]={
	{EVE_END_EVENT		,"End"			,false	,0,
		"End ;"},

	{EVE_SET_MESSAGE	,"SetMes"		,true	,6,
		"SetMes 本文 番号 X Y Z 基準点 その他のパラメータ ;"},

	{EVE_DELETE_MESSAGE	,"DelMes"		,false	,1,
		"DelMes 番号 ;"},

	{EVE_DEL_MES_QUICK	,"DelMesQ"		,false	,1,
		"DelMesQ 番号 ;"},

	{EVE_STOP			,"Stop"			,false	,1,
		"Stop (解除：0  主人公のみ：1  主人公と正面の人：2  全体：3　主人公と敵：4);"},		

	{EVE_RETURN_VILLAGE	,"ReturnVillage",false	,1,
		"ReturnVillage　(強制しない：0  強制する：1  BGMを切り替えない：3);"},	

	{EVE_OPEN_MENU		,"OpenMenu"		,false	,1,
		"OpenMenu　(入れ替え等許可：0  通常メニュー：1　入れ替えのみ：2);"},

	{EVE_SHOP			,"Shop"			,false	,2,
		"Shop ショップ番号 控えの有無;"},

	{EVE_SHOP_MAN		,"ShopMan"		,false	,9,
		"ShopMan 控えの有無 商品×８(無しなら1000でも指定しといて。);"},

	{EVE_TUTORIAL		,"Tutorial"		,false	,1,
		"Tutorial　チュートリアル番号;"},


	{EVE_FLEA_MARKET	,"FleaMarket"	,false	,1,
		"FleaMarket 控えの有無;"},

	{EVE_FARM			,"Farm"			,false	,1,
		"Farm 控えの有無;"},

	{EVE_SEED			,"Seed"			,false	,1,
		"Seed 控えの有無;"},

	{EVE_SELL			,"Sell"			,false	,1,
		"Sell 控えの有無;"},

	{EVE_NEXT_MONTH		,"NextMonth",	false	,0,
		"NextMonth　;"},

	{EVE_SET_FRIEND		,"SetFriend",	false	,1,
		"SetFriend 番号;"},

	{EVE_SET_EGG_FRIEND	,"SetEggFriend",false	,1,
		"SetEggFriend 番号;"},

	{EVE_SET_PARTY		,"SetParty",	false	,1,
		"SetParty 番号;"},

	{EVE_LEAVE_PARTY	,"LeaveParty",	false	,1,
		"LeaveParty 番号;"},

	{EVE_CHANGE_MAP		,"ChangeMap"	,false	,5,
		"ChangeMap マップ番号 X Y Z 方向;"},

	{EVE_BATTLE			,"Battle"		,false	,1,
		"Battle 番号;"},

	{EVE_ADD_ENEMY_LEVEL,"AddEnemyLv"	,false	,1,
		"AddEnemyLv 上昇レベル;"},

	{EVE_GET_ITEM		,"GetItem"		,false	,1,
		"GetItem アイテム番号;"},

	{EVE_SPEND_ITEM		,"SpendItem"	,false	,1,
		"SpendItem アイテム番号;"},

	{EVE_ADD_MARKET		,"AddMarket"	,false	,1,
		"AddMarket アイテム番号;"},

	{EVE_HEAL_ALL		,"HealAll"		,false	,0,
		"HealAll ;"},

	{EVE_MONTH_TITLE	,"MonthTitle"	,false	,0,
		"MonthTitle ;"},

	{EVE_MAP_TITLE		,"MapTitle"		,false	,0,
		"MapTitle ;"},

	{EVE_CHANGE_MAP_TITLE,"ChangeMapTitle",true,0,
		"ChangeMapTitle マップタイトル;"},

	{EVE_WAIT			,"Wait"			,false	,1,
		"Wait フレーム数 (0でキー入力待ち)"},

	{EVE_START_EVENT_SKIP,"StartEventSkip",false,0,
		"StartEventSkip ;"},

	{EVE_END_EVENT_SKIP	,"EndEventSkip"	,false	,0,
		"EndEventSkip ;"},

	{EVE_PLAY_WAVE		,"PlayWave"		,false	,1,
		"PlayWave 番号"},

	{EVE_PLAY_BGM		,"PlayBGM"		,false	,1,
		"PlayBGM 番号"},

	{EVE_SET_FADE		,"SetFade"		,false	,2,
		"SetFade 時間 目標音量（％）"},
	{EVE_SET_FADE_OUT	,"SetFadeOut"		,false	,1,
		"SetFadeOut 時間"},

	{EVE_MAP_COLOR		,"MapColor"		,false	,4,
		"MapColor A R G B"},

	{EVE_BG_COLOR		,"BGColor"		,false	,1,
		"BGColor 番号(8以降は特殊なエフェクト)"},

	{EVE_SET_CAMERA		,"SetCamera"	,false	,3,
		"SetCamera X Y (瞬間移動なら1、ゆっくりなら0);"},

	{EVE_SET_CAMERA_SPEED,"SetCameraSpeed",false,1,
		"SetCameraSpeed 速さ×10;"},

	{EVE_GET_MAP_CHIP	,"GetMapChip"	,false	,4,
		"GetMapChip 代入先 X Y Z"},

	{EVE_SET_MAP_CHIP	,"SetMapChip"	,false	,4,
		"SetMapChip チップ番号 X Y Z"},

	{EVE_CHANGE_ALL_CHIP,"ChangeAllChip",false,2,
		"ChangeAllChip 変更前のチップ番号 変更後のチップ番号"},

	{EVE_SET_OBJECT		,"SetObj"		,false	,11,
		"SetObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等(BlockShadow等);"},

	{EVE_SET_ANI_OBJECT	,"SetAniObj"	,false	,11,
		"SetAniObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_AUTO_ANI_OBJECT	,"SetAAObj"	,false	,13,
		"SetAAObj 番号 X Y Z 方向 速さ 移動頻度 画像 ｺﾏ数 ｱﾆﾒ速度 当たり判定 重力 表示順位等;"},

	{EVE_SET_HIGH_OBJECT,"SetHighObj"	,false	,11,
		"SetHighObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_WOOD_OBJECT,"SetWoodObj"	,false	,11,
		"SetWoodObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_HIGH_ANI_OBJECT,"SetHAObj"	,false	,11,
		"SetHAObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_BIG_OBJECT	,"SetBigObj"	,false	,11,
		"SetBigObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_BIG_ANI_OBJECT	,"SetBAObj"	,false	,11,
		"SetBAObj 番号 X Y Z 方向 速さ 移動頻度 画像 当たり判定 重力/ジャンプ 表示順位等;"},

	{EVE_SET_PUT_OBJECT	,"SetPutObj"	,false	,7,
		"SetPutObj 番号 X Y Z 速さ 画像 表示順位等;"},

	{EVE_SET_LIFT_OBJECT,"SetLiftObj"	,false	,9,
		"SetLiftObj 番号 X Y Z 方向 速さ 移動頻度 画像 表示順位等;"},

	{EVE_SET_NORMAL_TRAP,"SetNormalTrap",false	,10,
		"SetNormalTrap 番号 X Y Z 画像 周期 開始時刻　威力 効果音 表示順位等;"},


	{EVE_SET_ENEMY		,"SetEne"		,false	,14,
		"SetEne 番号 X Y Z 方向 速さ 移動頻度 索敵方針　索敵範囲 リーダー レベル 人数 地形 特殊条件;"},

	{EVE_SET_ENEMY_LOCKED,"SetEneL"		,false	,10,
		"SetEneL 番号 X Y Z 方向 速さ 移動頻度 索敵方針　索敵範囲 パーティー番号;"},

	{EVE_SET_BOX		,"SetBox"		,false	,6,
		"SetBox 番号 X Y Z 方向  宝物番号;"},

	{EVE_SET_SAVE_POINT	,"SetSavePoint"	,false	,4,
		"SetSavePoint 番号 X Y Z;"},

	{EVE_DELETE_OBJECT	,"DelObj"		,false	,1,
		"DelObj 番号;"},

	{EVE_SET_OBJ_POS	,"SetObjPos"	,false	,4,
		"SetObjPos 番号 X Y Z;"},

	{EVE_SET_OBJ_GOAL	,"SetObjGoal"	,false	,4,
		"SetObjGoal 番号 X Y Z;"},

	{EVE_SET_OBJ_GOAL_DIR	,"SetObjGoalDir"	,false	,3,
		"SetObjGoalDir 番号 方向(左上が０、時計回り) 距離;"},

	{EVE_SET_OBJ_DIR	,"SetObjDir"	,false	,2,
		"SetObjDir 番号 方向(左上が０、時計回り);"},

	{EVE_SET_OBJ_IMG	,"SetObjImg"	,false	,2,
		"SetObjImg 番号 画像;"},

	{EVE_SET_OBJ_COLOR	,"SetObjColor"	,false	,6,
		"SetObjColor 番号 A R G B 変更時間;"},

	{EVE_SET_OBJ_SPEED	,"SetObjSpeed"	,false	,2,
		"SetObjSpeed 番号 速さ;"},

	{EVE_SET_OBJ_GRAV	,"SetObjGrav"	,false	,2,
		"SetObjGrav 番号 重力指定;"},

	{EVE_SET_OBJ_PARAM	,"SetObjParam"	,false	,2,
		"SetObjParam 番号 表示順位、他;"},

	{EVE_SET_OBJ_VIEW	,"SetObjView"	,false	,2,
		"SetObjView 番号 描画するかどうか;"},

	{EVE_SET_OBJ_ZSPEED	,"SetObjZSpd"	,false	,2,
		"SetObjZSpd 番号 Z速度×10;"},

	{EVE_GO_TO			,"Goto"			,false	,1,
		"Goto 移動先"},

	{EVE_NEW_LINE		,"NewLine"		,false	,3,
		"NewLine パラメータA パラメータB 移動先 !Ret イベントID)"},

	{EVE_KILL			,"Kill"			,false	,1,
		"Kill イベントID"},

	{EVE_IF				,"If"			,false	,4,
		"If 数1 数2 比較演算子 移動先"},

	{EVE_IF_BOX			,"IfBox"		,false	,7,
		"IfBox オブジェクト番号 x1 y1 x2 y2 (内部なら1、外なら0) 移動先"},

	{EVE_IF_BOX_3D		,"IfBox3D"		,false	,9,
		"IfBox3D オブジェクト番号 x1 y1 z1 x2 y2 z2(内部なら1、外なら0) 移動先"},

	{EVE_SELECT_2		,"Select2"		,true	,7,
		"Select2 選択文(改行で区切る) キャンセル時の選択(0～1) x y z 基準点 移動先1 移動先2"},

	{EVE_SELECT_3		,"Select3"		,true	,8,
		"Select3 選択文(改行で区切る) キャンセル時の選択(0～2) x y z 基準点 移動先1 移動先2 移動先3"},

	{EVE_SELECT_4		,"Select4"		,true	,9,
		"Select4 選択文(改行で区切る) キャンセル時の選択(0～3) x y z 基準点 移動先1 移動先2 移動先3 移動先4"},

	{EVE_STORE		,"Store"			,false	,2,
		"Store 代入先 代入する数　;"},

	{EVE_KNOCK_BACK	,"KnockBack"		,false	,2,
		"KnockBack 影響を受ける元　効果音;"},

	{EVE_SHOW_IMAGE	,"ShowImage"		,false	,1,
		"ShowImage 画像番号（0で消去）;"},

	{EVE_ENDING		,"Ending"			,false	,0,
		"Ending ;"},

	{EVE_TRUE_ENDING,"TrueEnd"			,false	,0,
		"TrueEnd ;"},

	{EVE_INST		,"Inst"				,false	,1,		//実装やめた。
		"Inst 番号　;"},

///////////////////////////////////////////////////////////////////////////
	{EVE_OPEN_BOX		,"OpenBox"		,false	,0,
		"ユーザーは使わないこと。"},
	{EVE_ENCOUNT		,"Encount"		,false	,0,
		"ユーザーは使わないこと。"},


};

const CSpecialNumberInfo	SpecialNumberInfo[MAX_SNUM_NUMBER]={
	{SNUM_RANDOM		,"Rand"			,false	,0},
	{SNUM_MONEY			,"Money"		,false	,0},
	{SNUM_PLAYER_IMAGE	,"pImage"		,false	,0},
	{SNUM_PARTY_NUMBER	,"pNumber"		,false	,0},
	{SNUM_CURRENT		,"Current"		,false	,0},
	{SNUM_RETURN		,"Ret"			,false	,0},
	{SNUM_PARAMA		,"ParamA"		,false	,0},
	{SNUM_PARAMB		,"ParamB"		,false	,0},
	{SNUM_CURRENT_MONTH	,"CurrentMonth"	,false	,0},
	{SNUM_MONTH			,"Month"		,false	,0},
	{SNUM_MAP_X			,"MapX"			,false	,0},
	{SNUM_MAP_Y			,"MapY"			,false	,0},
	{SNUM_ESCAPE		,"Escape"		,false	,0},
	{SNUM_EVENT_ID		,"EventID"		,false	,0},
	{SNUM_BUSINESS		,"Business"		,false	,0},
	{SNUM_MAP_NO		,"MapNo"		,false	,0},
	{SNUM_START_X		,"StartX"		,false	,0},
	{SNUM_START_Y		,"StartY"		,false	,0},
	{SNUM_START_Z		,"StartZ"		,false	,0},
	{SNUM_START_DIR		,"StartDir"		,false	,0},
	{SNUM_HARD_MODE		,"HardMode"		,false	,0},
	{SNUM_EVENT_SKIP	,"ESkip"		,false	,0},
	{SNUM_KAKKO			,"k"			,true	,0x7fffffff},
	{SNUM_ROOT			,"Root"			,true	,0x7fffffff},
	{SNUM_FIX			,"fix"			,true	,0x7fffffff},
	{SNUM_FLAG			,"Flag"			,true	,FLAG_MAX},
	{SNUM_OBJECT_X		,"ObjX"			,true	,OBJECT_NO},
	{SNUM_OBJECT_Y		,"ObjY"			,true	,OBJECT_NO},
	{SNUM_OBJECT_Z		,"ObjZ"			,true	,OBJECT_NO},
	{SNUM_OBJECT_X_32	,"ObjSX"		,true	,OBJECT_NO},
	{SNUM_OBJECT_Y_32	,"ObjSY"		,true	,OBJECT_NO},
	{SNUM_OBJECT_Z_32	,"ObjSZ"		,true	,OBJECT_NO},
	{SNUM_OBJECT_CAM_X	,"ObjCamX"		,true	,OBJECT_NO},
	{SNUM_OBJECT_CAM_Y	,"ObjCamY"		,true	,OBJECT_NO},
	{SNUM_OBJECT_DIR	,"ObjDir"		,true	,OBJECT_NO},
	{SNUM_OBJECT_GOAL	,"ObjGoal"		,true	,OBJECT_NO},
	{SNUM_MAP_FLAG		,"MFlag"		,true	,MAX_MAP_FLAG},
	{SNUM_ITEM_NO		,"ItemNo"		,true	,MAX_ITEM_NUMBER},
	{SNUM_MEMBER		,"Member"		,true	,MAX_PARTY_NUMBER},
	{SNUM_SHOP_OK		,"ShopOK"		,true	,VILLAGE_SHOP_MAX},
	{SNUM_BOX			,"Box"			,true	,TREASURE_NO},
	{SNUM_IS_MEMBER		,"IsMember"		,true	,MAX_CHARACTER_NUMBER},
};	