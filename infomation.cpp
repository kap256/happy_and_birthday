
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
	{EH_TIME_SEC	,"TimeSec"},		//����`
	{EH_FALL		,"Fall"},
	{EH_GOAL		,"Goal"},
};

const CEventDataInfo		EventDataInfo[MAX_EVENT_PATTERN]={
	{EVE_END_EVENT		,"End"			,false	,0,
		"End ;"},

	{EVE_SET_MESSAGE	,"SetMes"		,true	,6,
		"SetMes �{�� �ԍ� X Y Z ��_ ���̑��̃p�����[�^ ;"},

	{EVE_DELETE_MESSAGE	,"DelMes"		,false	,1,
		"DelMes �ԍ� ;"},

	{EVE_DEL_MES_QUICK	,"DelMesQ"		,false	,1,
		"DelMesQ �ԍ� ;"},

	{EVE_STOP			,"Stop"			,false	,1,
		"Stop (�����F0  ��l���̂݁F1  ��l���Ɛ��ʂ̐l�F2  �S�́F3�@��l���ƓG�F4);"},		

	{EVE_RETURN_VILLAGE	,"ReturnVillage",false	,1,
		"ReturnVillage�@(�������Ȃ��F0  ��������F1  BGM��؂�ւ��Ȃ��F3);"},	

	{EVE_OPEN_MENU		,"OpenMenu"		,false	,1,
		"OpenMenu�@(����ւ������F0  �ʏ탁�j���[�F1�@����ւ��̂݁F2);"},

	{EVE_SHOP			,"Shop"			,false	,2,
		"Shop �V���b�v�ԍ� �T���̗L��;"},

	{EVE_SHOP_MAN		,"ShopMan"		,false	,9,
		"ShopMan �T���̗L�� ���i�~�W(�����Ȃ�1000�ł��w�肵�Ƃ��āB);"},

	{EVE_TUTORIAL		,"Tutorial"		,false	,1,
		"Tutorial�@�`���[�g���A���ԍ�;"},


	{EVE_FLEA_MARKET	,"FleaMarket"	,false	,1,
		"FleaMarket �T���̗L��;"},

	{EVE_FARM			,"Farm"			,false	,1,
		"Farm �T���̗L��;"},

	{EVE_SEED			,"Seed"			,false	,1,
		"Seed �T���̗L��;"},

	{EVE_SELL			,"Sell"			,false	,1,
		"Sell �T���̗L��;"},

	{EVE_NEXT_MONTH		,"NextMonth",	false	,0,
		"NextMonth�@;"},

	{EVE_SET_FRIEND		,"SetFriend",	false	,1,
		"SetFriend �ԍ�;"},

	{EVE_SET_EGG_FRIEND	,"SetEggFriend",false	,1,
		"SetEggFriend �ԍ�;"},

	{EVE_SET_PARTY		,"SetParty",	false	,1,
		"SetParty �ԍ�;"},

	{EVE_LEAVE_PARTY	,"LeaveParty",	false	,1,
		"LeaveParty �ԍ�;"},

	{EVE_CHANGE_MAP		,"ChangeMap"	,false	,5,
		"ChangeMap �}�b�v�ԍ� X Y Z ����;"},

	{EVE_BATTLE			,"Battle"		,false	,1,
		"Battle �ԍ�;"},

	{EVE_ADD_ENEMY_LEVEL,"AddEnemyLv"	,false	,1,
		"AddEnemyLv �㏸���x��;"},

	{EVE_GET_ITEM		,"GetItem"		,false	,1,
		"GetItem �A�C�e���ԍ�;"},

	{EVE_SPEND_ITEM		,"SpendItem"	,false	,1,
		"SpendItem �A�C�e���ԍ�;"},

	{EVE_ADD_MARKET		,"AddMarket"	,false	,1,
		"AddMarket �A�C�e���ԍ�;"},

	{EVE_HEAL_ALL		,"HealAll"		,false	,0,
		"HealAll ;"},

	{EVE_MONTH_TITLE	,"MonthTitle"	,false	,0,
		"MonthTitle ;"},

	{EVE_MAP_TITLE		,"MapTitle"		,false	,0,
		"MapTitle ;"},

	{EVE_CHANGE_MAP_TITLE,"ChangeMapTitle",true,0,
		"ChangeMapTitle �}�b�v�^�C�g��;"},

	{EVE_WAIT			,"Wait"			,false	,1,
		"Wait �t���[���� (0�ŃL�[���͑҂�)"},

	{EVE_START_EVENT_SKIP,"StartEventSkip",false,0,
		"StartEventSkip ;"},

	{EVE_END_EVENT_SKIP	,"EndEventSkip"	,false	,0,
		"EndEventSkip ;"},

	{EVE_PLAY_WAVE		,"PlayWave"		,false	,1,
		"PlayWave �ԍ�"},

	{EVE_PLAY_BGM		,"PlayBGM"		,false	,1,
		"PlayBGM �ԍ�"},

	{EVE_SET_FADE		,"SetFade"		,false	,2,
		"SetFade ���� �ڕW���ʁi���j"},
	{EVE_SET_FADE_OUT	,"SetFadeOut"		,false	,1,
		"SetFadeOut ����"},

	{EVE_MAP_COLOR		,"MapColor"		,false	,4,
		"MapColor A R G B"},

	{EVE_BG_COLOR		,"BGColor"		,false	,1,
		"BGColor �ԍ�(8�ȍ~�͓���ȃG�t�F�N�g)"},

	{EVE_SET_CAMERA		,"SetCamera"	,false	,3,
		"SetCamera X Y (�u�Ԉړ��Ȃ�1�A�������Ȃ�0);"},

	{EVE_SET_CAMERA_SPEED,"SetCameraSpeed",false,1,
		"SetCameraSpeed �����~10;"},

	{EVE_GET_MAP_CHIP	,"GetMapChip"	,false	,4,
		"GetMapChip ����� X Y Z"},

	{EVE_SET_MAP_CHIP	,"SetMapChip"	,false	,4,
		"SetMapChip �`�b�v�ԍ� X Y Z"},

	{EVE_CHANGE_ALL_CHIP,"ChangeAllChip",false,2,
		"ChangeAllChip �ύX�O�̃`�b�v�ԍ� �ύX��̃`�b�v�ԍ�"},

	{EVE_SET_OBJECT		,"SetObj"		,false	,11,
		"SetObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�(BlockShadow��);"},

	{EVE_SET_ANI_OBJECT	,"SetAniObj"	,false	,11,
		"SetAniObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_AUTO_ANI_OBJECT	,"SetAAObj"	,false	,13,
		"SetAAObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �ϐ� ��ґ��x �����蔻�� �d�� �\�����ʓ�;"},

	{EVE_SET_HIGH_OBJECT,"SetHighObj"	,false	,11,
		"SetHighObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_WOOD_OBJECT,"SetWoodObj"	,false	,11,
		"SetWoodObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_HIGH_ANI_OBJECT,"SetHAObj"	,false	,11,
		"SetHAObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_BIG_OBJECT	,"SetBigObj"	,false	,11,
		"SetBigObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_BIG_ANI_OBJECT	,"SetBAObj"	,false	,11,
		"SetBAObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �����蔻�� �d��/�W�����v �\�����ʓ�;"},

	{EVE_SET_PUT_OBJECT	,"SetPutObj"	,false	,7,
		"SetPutObj �ԍ� X Y Z ���� �摜 �\�����ʓ�;"},

	{EVE_SET_LIFT_OBJECT,"SetLiftObj"	,false	,9,
		"SetLiftObj �ԍ� X Y Z ���� ���� �ړ��p�x �摜 �\�����ʓ�;"},

	{EVE_SET_NORMAL_TRAP,"SetNormalTrap",false	,10,
		"SetNormalTrap �ԍ� X Y Z �摜 ���� �J�n�����@�З� ���ʉ� �\�����ʓ�;"},


	{EVE_SET_ENEMY		,"SetEne"		,false	,14,
		"SetEne �ԍ� X Y Z ���� ���� �ړ��p�x ���G���j�@���G�͈� ���[�_�[ ���x�� �l�� �n�` �������;"},

	{EVE_SET_ENEMY_LOCKED,"SetEneL"		,false	,10,
		"SetEneL �ԍ� X Y Z ���� ���� �ړ��p�x ���G���j�@���G�͈� �p�[�e�B�[�ԍ�;"},

	{EVE_SET_BOX		,"SetBox"		,false	,6,
		"SetBox �ԍ� X Y Z ����  �󕨔ԍ�;"},

	{EVE_SET_SAVE_POINT	,"SetSavePoint"	,false	,4,
		"SetSavePoint �ԍ� X Y Z;"},

	{EVE_DELETE_OBJECT	,"DelObj"		,false	,1,
		"DelObj �ԍ�;"},

	{EVE_SET_OBJ_POS	,"SetObjPos"	,false	,4,
		"SetObjPos �ԍ� X Y Z;"},

	{EVE_SET_OBJ_GOAL	,"SetObjGoal"	,false	,4,
		"SetObjGoal �ԍ� X Y Z;"},

	{EVE_SET_OBJ_GOAL_DIR	,"SetObjGoalDir"	,false	,3,
		"SetObjGoalDir �ԍ� ����(���オ�O�A���v���) ����;"},

	{EVE_SET_OBJ_DIR	,"SetObjDir"	,false	,2,
		"SetObjDir �ԍ� ����(���オ�O�A���v���);"},

	{EVE_SET_OBJ_IMG	,"SetObjImg"	,false	,2,
		"SetObjImg �ԍ� �摜;"},

	{EVE_SET_OBJ_COLOR	,"SetObjColor"	,false	,6,
		"SetObjColor �ԍ� A R G B �ύX����;"},

	{EVE_SET_OBJ_SPEED	,"SetObjSpeed"	,false	,2,
		"SetObjSpeed �ԍ� ����;"},

	{EVE_SET_OBJ_GRAV	,"SetObjGrav"	,false	,2,
		"SetObjGrav �ԍ� �d�͎w��;"},

	{EVE_SET_OBJ_PARAM	,"SetObjParam"	,false	,2,
		"SetObjParam �ԍ� �\�����ʁA��;"},

	{EVE_SET_OBJ_VIEW	,"SetObjView"	,false	,2,
		"SetObjView �ԍ� �`�悷�邩�ǂ���;"},

	{EVE_SET_OBJ_ZSPEED	,"SetObjZSpd"	,false	,2,
		"SetObjZSpd �ԍ� Z���x�~10;"},

	{EVE_GO_TO			,"Goto"			,false	,1,
		"Goto �ړ���"},

	{EVE_NEW_LINE		,"NewLine"		,false	,3,
		"NewLine �p�����[�^A �p�����[�^B �ړ��� !Ret �C�x���gID)"},

	{EVE_KILL			,"Kill"			,false	,1,
		"Kill �C�x���gID"},

	{EVE_IF				,"If"			,false	,4,
		"If ��1 ��2 ��r���Z�q �ړ���"},

	{EVE_IF_BOX			,"IfBox"		,false	,7,
		"IfBox �I�u�W�F�N�g�ԍ� x1 y1 x2 y2 (�����Ȃ�1�A�O�Ȃ�0) �ړ���"},

	{EVE_IF_BOX_3D		,"IfBox3D"		,false	,9,
		"IfBox3D �I�u�W�F�N�g�ԍ� x1 y1 z1 x2 y2 z2(�����Ȃ�1�A�O�Ȃ�0) �ړ���"},

	{EVE_SELECT_2		,"Select2"		,true	,7,
		"Select2 �I��(���s�ŋ�؂�) �L�����Z�����̑I��(0�`1) x y z ��_ �ړ���1 �ړ���2"},

	{EVE_SELECT_3		,"Select3"		,true	,8,
		"Select3 �I��(���s�ŋ�؂�) �L�����Z�����̑I��(0�`2) x y z ��_ �ړ���1 �ړ���2 �ړ���3"},

	{EVE_SELECT_4		,"Select4"		,true	,9,
		"Select4 �I��(���s�ŋ�؂�) �L�����Z�����̑I��(0�`3) x y z ��_ �ړ���1 �ړ���2 �ړ���3 �ړ���4"},

	{EVE_STORE		,"Store"			,false	,2,
		"Store ����� ������鐔�@;"},

	{EVE_KNOCK_BACK	,"KnockBack"		,false	,2,
		"KnockBack �e�����󂯂錳�@���ʉ�;"},

	{EVE_SHOW_IMAGE	,"ShowImage"		,false	,1,
		"ShowImage �摜�ԍ��i0�ŏ����j;"},

	{EVE_ENDING		,"Ending"			,false	,0,
		"Ending ;"},

	{EVE_TRUE_ENDING,"TrueEnd"			,false	,0,
		"TrueEnd ;"},

	{EVE_INST		,"Inst"				,false	,1,		//������߂��B
		"Inst �ԍ��@;"},

///////////////////////////////////////////////////////////////////////////
	{EVE_OPEN_BOX		,"OpenBox"		,false	,0,
		"���[�U�[�͎g��Ȃ����ƁB"},
	{EVE_ENCOUNT		,"Encount"		,false	,0,
		"���[�U�[�͎g��Ȃ����ƁB"},


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