
#include "option.h"
#ifdef _DEBUG

#include "main.h"
#include "mapmode.h"
#include "battlemode.h"
#include "village.h"
#include "input.h"
#include "gamewindow.h"
#include "cursor.h"
#include "menudata.h" 
#include "changemode.h"
#include "itemdata.h"
#include "flag.h"
#include "title.h"
#include "itemdata.h"

//�G�̃p�����[�^���v�Z����HTML�ɓf���܂��B
	#define _OUTPUT_ENEMY_MIN_NO_	0
	#define _OUTPUT_ENEMY_MAX_NO_	127
	#define _OUTPUT_ENEMY_MIN_LV_	16
	#define _OUTPUT_ENEMY_MAX_LV_	60
	#define _OUTPUT_ENEMY_LV_STEP_	20

//�G�̃X�L�����o�͂��܂��B
	//�o�͔͈͂́A��̎w��Ƌ���
	//#define _OUTPUT_ENEMY_SKILL_BASE_	ID_AS + 242		//��`����Ă���ꍇ�A���̃X�L�����K�����Ă���G���o��
	#define _OUTPUT_ENEMY_SKILL_SEARCH_					//��`����Ă���ꍇ�A���L�̃X�L���̂ݏo��
		#define _OUTPUT_ENEMY_SKILL_SEARCH_START_	ID_AS + 0
		#define _OUTPUT_ENEMY_SKILL_SEARCH_END_		ID_AS + 0


//���ʂ̊l���ł���󔠂̒��g��HTML�ɓf���܂��B
	#define _OUTPUT_TRESURE_NORMAL_HARD_ 1
	#define _OUTPUT_TRESURE_MAX_MONTH_   12


//�}�b�v�̉摜��f���܂��B
	#define _OUTPUT_MAP_BMP_START_	364
	#define _OUTPUT_MAP_BMP_END_	_OUTPUT_MAP_BMP_START_


	const POINT MonthMapData[MAX_ALL_MONTH][5]={
		{	{0	,30	},},				//1
		{	{36	,49	},	{148,150},},	//2
		{	{120,143},},	//3N
		{	{445,463},},	//3H
		
		{	{58	,65	},	{71	,75	},	{78	,86	},	{394,400},},	//4
		{	{162,191},	{330,337},},	//5N
		{	{271,298},},	//5H
		{	{192,215},	{401,412},},	//6
		
		{	{223,244},},	//7N
		{	{649,672},},	//7H
		{	{88	,117},	{413,419},},	//8
		{	{249,268},},	//9
		
		{	{680,698},},	//10N
		{	{469,492},},	//10H
		{	{303,328},},	//11N
		{	{494,531},},	//11H

		{	{338,392},},	//12N
		{	{428,443},	{534,643},	{673,676},},	//12H
		{	{0	,34	},},				//1
	};

	const POINT LDMapData[MAX_ALL_MONTH][5]={
		{	{546,554},},	//1
		{	{539,545},},	//2
		{	{560,569},},	//3
		{	{555,559},},	//4
		
		{	{570,584},},	//5
		{	{585,596},},	//6
		{	{673,676},},	//7
		{	{597,605},},	//8
		
		{	{606,622},},	//9
		{	{623,631},},	//10
		{	{428,443},},	//11
		{	{632,639},},	//12
	};

	const int MonthNormalHard[MAX_ALL_MONTH]={
		3,//1���F�����ȑ��̐V�N��
		3,//2���F�_��Ǝ�
		1,//3��n
		2,//3��h
		
		3,//4��
		1,//5��n
		2,//5��h
		3,//6��

		1,//7��n
		2,//7��h
		3,//8��
		3,//9��

		1,//10��n
		2,//10��h
		1,//11��n
		2,//11��h

		1,//12��n
		2,//12��h
		2
	};

	const int MonthEnemyData[MAX_ALL_MONTH][ENEMY_NO][10]={
		{//1���F�����ȑ��̐V�N��
			{	0,	0,4,16,20,	-100},
			{	1,	0,4,	-100},
			{	2,	0,4,	-100},
			{	10,	0,	-100},
			{	13,	3,	-100},
			{	14,	3,	-100},
			{	58,	20,	-100},
			{	59,	20,	-100},
			{	60,	22,	-100},
		{-100}},

		{//2���F�_��Ǝ�
			{	0,	0,	-100},
			{	1,	0,	-100},
			{	2,	0,	-100},
			{	5,	0,	-100},
			{	8,	0,	-100},
			{	9,	0,	-100},
			{	10,	0,	-100},
			{	11,	4,	-100},
			{	27,	1,	-100},
			{	28,	1,	-100},
			{	29,	1,	-100},
			{	30,	1,	-100},
			{	6,	3,	-100},
			{	31,	4,	-100},
		{-100}},

		{//3���F�ؘR����̒��̗d��
			{	5,	0,	-100},
			{	8,	0,	-100},
			{	11,	0,	-100},
			{	20,	0,	-100},
			{	21,	0,1,	-100},
			{	22,	0,	-100},
			{	87,	2,	-100},
			{	24,	4,	-100},
			{	25,	4,	-100},
			{	28,	0,	-100},
			{	29,	0,	-100},
			{	30,	0,	-100},
		{-100}},

		{//3���F�����̉�
			{	0,	0,	-100},
			{	2,	0,	-100},
			{	5,	0,	-100},
			{	8,	0,	-100},
			{	10,	0,	-100},
			{	11,	0,4,	-100},
			{	12,	0,	-100},
			{	20,	0,	-100},
			{	65,	0,4,	-100},
			{	71,	0,	-100},
			{	88,	0,16,	-100},
			{	89,	6,	-100},
			{	90,	2,	-100},
		{-100}},

		{//4���F��T���ɍs����
			{	19,	4,	-100},
			{	20,	0,	-100},
			{	1,	0,8,	-100},
			{	2,	0,8,	-100},
			{	3,	20,	-100},
			{	4,	20,	-100},
			{	5,	0,4,8,	-100},
			{	8,	0,4,8,	-100},
			{	9,	0,4,8,	-100},
			{	11,	0,4,8,	-100},
			{	12,	0,4,8,	-100},
			{	13,	20,	-100},
			{	14,	20,	-100},
			{	15,	4,	-100},
			{	37,	0,4,	-100},
			{	75,	20,	-100},
		{-100}},

		{//5���F���̑�n
			{	22,	4,	-100},
			{	23,	4,	-100},
			{	26,	0,	-100},
			{	27,	0,	-100},
			{	33,	0,	-100},
			{	34,	0,	-100},
			{	35,	0,	-100},
			{	36,	0,	-100},
			{	53,	4,	-100},
			{	54,	4,	-100},
			{	56,	0,	-100},
			{	57,	0,	-100},
		{-100}},

		{//5���F�܌��J�̉�
			{	0,	0,	-100},
			{	10,	0,	-100},
			{	23,	0,	-100},
			{	39,	0,	-100},
			{	46,	0,	-100},
			{	59,	0,	-100},
			{	63,	0,	-100},
			{	70,	0,	-100},
			{	72,	0,	-100},
			{	81,	0,	-100},
			{	55,	60,	-100},
			{	91,	60,	-100},
			{	92,	0,	-100},
		{-100}},

		{//6���F�Â�藈�����
			{	10,	0,4,	-100},
			{	12,	0,4,	-100},
			{	28,	0,4,	-100},
			{	38,	0,4,16,20,	-100},
			{	39,	0,4,	-100},
			{	40,	4,16,20,	-100},
			{	41,	4,	-100},
			{	76,	16,20,	-100},
			{	77,	16,20,	-100},
			{	78,	24,	-100},
			{	79,	20,	-100},
		{-100}},

		{//7���F�H���Ȗ�
			{	0,	0,	-100},
			{	7,	0,	-100},
			{	17,	4,	-100},
			{	32,	0,	-100},
			{	42,	60,	-100},
			{	62,	0,	-100},
		{-100}},

		{//7���F���A�R����
			{	7,	0,20,	-100},
			{	10,	0,	-100},
			{	26,	0,20,	-100},
			{	27,	0,20,	-100},
			{	35,	0,20,	-100},
			{	48,	0,20,	-100},
			{	49,	0,20,	-100},
			{	58,	0,20,	-100},
			{	98,	0,	-100},
			{	120,	2,32,	-100},
		{-100}},

		{//8���F�ꂩ��̎莆
			{	5,	0,	-100},
			{	9,	0,	-100},
			{	16,	0,4,8,12,25,28,	-100},
			{	17,	0,4,8,12,25,28,	-100},
			{	18,	0,	-100},
			{	29,	0,	-100},
			{	30,	0,	-100},
			{	43,	5,28,	-100},
			{	47,	0,	-100},
			{	63,	0,	-100},
			{	80,	8,12,25,28,	-100},
			{	81,	8,12,25,28,	-100},
		{-100}},

		{//9���F㟊��ꂽ�Ɏq
			{	0,	0,	-100},
			{	9,	0,	-100},
			{	11,	0,8,	-100},
			{	17,	8,12,	-100},
			{	18,	0,8,	-100},
			{	32,	8,12,	-100},
			{	44,	0,8,	-100},
			{	45,	0,8,12,	-100},
			{	46,	0,8,12,	-100},
			{	47,	0,8,	-100},
			{	64,	0,	-100},
		{-100}},

		{//10���F�v���o�̉ԉ�
			{	0,	0,	-100},
			{	1,	0,	-100},
			{	2,	0,	-100},
			{	11,	0,	-100},
			{	22,	8,	-100},
			{	23,	8,	-100},
			{	48,	0,	-100},
			{	53,	8,	-100},
			{	54,	8,	-100},
			{	58,	0,	-100},
			{	59,	0,	-100},
			{	63,	0,	-100},
			{	64,	0,	-100},
			{	103,	4,	-100},
		{-100}},

		{//10���F�吹���ɂ�
			{	0,	20,	-100},
			{	26,	0,	-100},
			{	36,	0,4,	-100},
			{	56,	0,4,	-100},
			{	57,	0,4,	-100},
			{	65,	0,	-100},
			{	66,	4,	-100},
			{	67,	0,4,	-100},
			{	69,	20,	-100},
			{	90,	0,	-100},
		{-100}},

		{//11���F�Ж���̗��K
			{	22,	0,4,	-100},
			{	23,	0,	-100},
			{	52,	0,	-100},
			{	53,	0,	-100},
			{	54,	0,	-100},
			{	55,	60,	-100},
		{-100}},

		{//11���F����ʖ]��
			{	6,	0,	-100},
			{	23,	20,	-100},
			{	27,	0,	-100},
			{	28,	0,	-100},
			{	29,	0,	-100},
			{	30,	0,	-100},
			{	31,	20,	-100},
			{	46,	0,	-100},
			{	48,	20,	-100},
			{	60,	20,	-100},
			{	66,	20,	-100},
			{	67,	0,	-100},
			{	68,	4,	-100},
			{	91,	20,	-100},
			{	92,	20,	-100},
		{-100}},

		{//12���F�����ȑ��̒a����
			{	0,	0,16,	-100},
			{	1,	0,16,	-100},
			{	2,	0,16,	-100},
			{	5,	0,	-100},
			{	10,	0,	-100},
			{	11,	0,8,	-100},
			{	16,	8,	-100},
			{	17,	8,	-100},
			{	36,	0,8,	-100},
			{	38,	0,4,	-100},
			{	40,	0,4,	-100},
			{	44,	8,	-100},
			{	48,	8,	-100},
			{	56,	0,8,	-100},
			{	57,	0,8,	-100},
			{	60,	8,	-100},
			{	63,	0,4,	-100},
			{	64,	0,4,	-100},
			{	65,	0,8,	-100},
			{	66,	4,	-100},
			{	67,	0,	-100},
			{	68,	4,	-100},
			{	69,	4,	-100},
			{	70,	4,	-100},
			{	71,	4,	-100},
			{	72,	4,	-100},
			{	77,	0,4,	-100},
			{	73,	60,	-100},
			{	74,	60,	-100},
		{-100}},

		{//12���F�����킹�̉ʂĂ�
			{	0,	0,	-100},
			{	1,	0,	-100},
			{	2,	0,	-100},
			{	5,	0,	-100},
			{	6,	-59,	-100},
			{	8,	0,	-100},
			{	9,	0,	-100},
			{	10,	0,	-100},
			{	11,	0,	-100},
			{	12,	0,	-100},
			{	13,	0,	-100},
			{	14,	0,	-100},
			{	15,	0,	-100},
			{	16,	0,	-100},
			{	17,	0,	-100},
			{	18,	0,	-100},
			{	19,	0,	-100},
			{	20,	0,	-100},
			{	21,	0,	-100},
			{	22,	0,	-100},
			{	23,	0,	-100},
			{	24,	4,	-100},
			{	25,	4,	-100},
			{	26,	0,	-100},
			{	27,	0,	-100},
			{	30,	0,	-100},
			{	32,	0,	-100},
			{	35,	0,	-100},
			{	36,	0,	-100},
			{	38,	0,	-100},
			{	39,	0,	-100},
			{	40,	0,	-100},
			{	42,	20,	-100},
			{	46,	0,	-100},
			{	47,	0,	-100},
			{	48,	0,	-100},
			{	49,	0,	-100},
			{	52,	0,	-100},
			{	53,	0,	-100},
			{	54,	0,	-100},
			{	55,	20,	-100},
			{	56,	0,	-100},
			{	57,	0,	-100},
			{	58,	0,	-100},
			{	62,	0,	-100},
			{	63,	0,	-100},
			{	65,	0,	-100},
			{	67,	0,	-100},
			{	73,	20,	-100},
			{	76,	0,	-100},
			{	77,	0,	-100},
			{	80,	0,	-100},
			{	81,	0,	-100},
			{	82,	0,	-100},
			{	83,	0,	-100},
			{	89,	6,	-100},
			{	90,	2,	-100},
			{	92,	0,	-100},
			{	95,	0,	-100},
			{	96,	20,	-100},
			{	97,	20,	-100},
			{	98,	-4,20,	-100},
			{	99,	20,	-100},
			{	100,	20,	-100},
			{	101,	20,	-100},
			{	102,	20,	-100},
			{	103,	0,	-100},
			{	104,	20,	-100},
			{	105,	20,	-100},
			{	106,	20,	-100},
			{	121,	20,	-100},
			{	122,	60,	-100},

			{	107,	60,	-100},
			{	108,	60,	-100},
			{	109,	60,	-100},
			{	110,	60,	-100},
			{	111,	60,	-100},
			{	112,	60,	-100},
			{	113,	60,	-100},
			{	114,	60,	-100},
			{	115,	60,	-100},
			{	116,	60,	-100},
			{	124,	60,	-100},
			{	117,	60,	-100},
			{	118,	60,	-100},
		{-100}},

		{//1���F�����ȑ��̐V�N��
			{	33,	9,	-100},
			{	34,	9,	-100},
			{	87,	24,	-100},
			{	121,	39,	-100},
			{	93,	44,	-100},
			{	123,	60,	-100},
		{-100}},
	};
void OutputDBSkill(FILE *pFile,int skill)
{
	switch(SkillData[skill].mType){
		case SKILL_STATUS:
			fprintf(pFile,"_p'>");	break;
		case SKILL_COMMAND:
			fprintf(pFile,"_c'>");	break;
		case SKILL_AUTO:
			fprintf(pFile,"_a'>");	break;
		case SKILL_FIELD:
			fprintf(pFile,"_f'>");	break;
		default:
			fprintf(pFile,"'>");	break;
	}
	if(SkillData[skill].mType==SKILL_COMMAND){
		fprintf(pFile,"<a href='cskill.html#cs_%d'>",SkillData[skill].mParam);
	}else{
		fprintf(pFile,"<a href='askill.html#as_%d'>",skill);
	}
	fprintf(pFile,"%s</a></td>",SkillData[skill].GetName());
}
void OutputDBParam(FILE *pFile,int param,char* name)
{
	fprintf(pFile,"<td class='param_name'>%s</td><td>",name);
	if(param>0){
		fprintf(pFile,"<span class='red'>");
	}else if(param<0){
		fprintf(pFile,"<span class='blue'>");
	}else{
		fprintf(pFile,"<span class='subtext'>");
	}
	fprintf(pFile,"%+d</span></td>",param);
}
void OutputDBCharaParam(FILE *pFile,int lv,int param[9])
{
	CPlayerData cdata;
	cdata.mStm=param[0];
	cdata.mTech=param[1];
	cdata.mLv=lv;
	cdata.CalculateLP();
	cdata.CalculateSP();

	fprintf(pFile,"<td class='param'>%d</td>",lv);
	fprintf(pFile,"<td class='param'>%d</td><td class='sp'>",cdata.GetMaxLP());
	for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
		fprintf(pFile,"%d/",cdata.GetMaxSP(k));
	fprintf(pFile,"</td>");

	for(int k=2;k<9;k++){
		fprintf(pFile,"<td class='param'>%d</td>",param[k]);
	}
}
void OutputDBFirstEquip(FILE *pFile,int i,int loop,int geta)
{
	bool end=false;
	for(int j=0;j<loop;j++){
		if(j!=0)	fprintf(pFile,"<tr>");
		fprintf(pFile,"<td>");
		int eq=StartItem[i][j+geta];
		if(eq==-1)	end=true;
		if(end)		eq=NO_ITEM;
		if(eq!=NO_ITEM){
			const CEquipmentData &eqd=EquipmentData[eq];
			fprintf(pFile,"<a href='eq.html#eq_%d'>%s</a>",eq,eqd.GetName());
		}else{
			fprintf(pFile,"<span class='subtext'>---</span>");
		}
		fprintf(pFile,"</td></tr>");
	}
}
void AddLv(int lvlist[10],int lv)
{
	for(int i=0;i<10;i++){
		if(lvlist[i]==lv)	return;
		if(lvlist[i]<=-100){
			lvlist[i]=lv;
			return;
		}
	}
	MessageBox(NULL,"error","error",MB_OK);
}
void AddEnemy(int allenemy[ENEMY_NO][10], int enemy,int lv)
{
	for(int i=0;i<10;i++){
		if(allenemy[enemy][i]==lv)	return;
		if(allenemy[enemy][i]<=-100){
			allenemy[enemy][i]=lv;
			return;
		}
	}
}
void AddEnemyDB(int enemy_list[ENEMY_NO][60],int enemy,int lv)
{
	if(lv<0)	lv=0;
	if(lv>60)	lv=60;
	for(int e=0;e<ENEMY_NO;e++){
		if(enemy_list[e][0]<0)	enemy_list[e][0]=enemy;
		if(enemy_list[e][0]==enemy){
			for(int l=1;l<60;l++){
				if(enemy_list[e][l]==lv)	return;
				if(enemy_list[e][l]<0){
					enemy_list[e][l]=lv;
					return;
				}
				if(enemy_list[e][l]>lv){
					for(int i=59;i>l;i--){
						enemy_list[e][i]=enemy_list[e][i-1];
					}
					enemy_list[e][l]=lv;
					return;
				}
			}
			return;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DebugOutput()
{
	
	const char* shopname[MAX_SHOP_NO+1]={
		"��",
		"�@�o�������i�m�[�}���j",
		"�@�o�������i�n�[�h�j",
		"��Ή�",
		"���퉮",
		"�h�",
		"�G�݉�",
		"�G�݉� (����Lv1)",
		"�G�݉� (����Lv2)",
		"�G�݉� (����Lv3)",
		"�X (�d���̐X)",
		"�X (�d���̐X)",
		"�X (�����̒�)",
		"�X (�����̒�)",
		"�X (�����̒�)",
		"�X (�n������)",
	};
	const int shopno[2]={13,7};
	const int shopid[2][MAX_SHOP_NO+1]={
		{0,1,3,4,5,6,7,8,9,11,12,13,14},
		{0,2,3,4,5,6,15}
	};

	const char* monthcolor[MAX_MONTH]={
		"#80ff80",
		"#c0ff80",
		"#ffff80",
		"#ffc080",
		"#ff8080",
		"#ff80c0",
		"#ff80ff",
		"#c080ff",
		"#8080ff",
		"#80c0ff",
		"#80ffff",
		"#80ffc0",
		"#ffffff",
	};
	FILE* pFile;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�@�@�@�U�����֘A�@�@�@�@
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_SKILL_
	pFile=fopen("data\\ �X�L���ꗗ.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			fprintf(pFile,"<table border><tr><td colspan='%d'>",MAX_SKILL_MONTH+1);
			fprintf(pFile,"%s</td></tr>",CharacterBaseData[i].GetName());
			for(int j=0;j<MAX_MONTH-1;j++){
				fprintf(pFile,"<tr><td>%d��</td>",j+1);
				for(int k=0;k<MAX_SKILL_MONTH;k++){
					int skill=CharacterSkill[i][j][k];
					switch(SkillData[skill].mType){
						case SKILL_COMMAND:
							fprintf(pFile,"<td bgcolor=#ffb0b0>");	break;
						case SKILL_AUTO:
							fprintf(pFile,"<td bgcolor=#b0ffb0>");	break;
						case SKILL_FIELD:
							fprintf(pFile,"<td bgcolor=#ffff80>");	break;
						default:
							fprintf(pFile,"<td bgcolor=#80ffff>");	break;
					}

					fprintf(pFile,SkillData[skill].GetName());
					fprintf(pFile,"</td>");
				}
				fprintf(pFile,"</tr>");
			}
			fprintf(pFile,"</table>");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}

	pFile=fopen("data\\ ���a���Α����\.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		fprintf(pFile,"<table border>");
		for(int j=0;j<MAX_MONTH;j++){
			fprintf(pFile,"<td>%d��</td>",j);
		}
		fprintf(pFile,"</tr>");
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			fprintf(pFile,"<tr><td>%s</td>",CharacterBaseData[i].GetName());
			for(int j=0;j<MAX_MONTH-1;j++){
				int skill=CharacterSkill[i][j][MAX_SKILL_MONTH-1];
				switch(SkillData[skill].mType){
					case SKILL_COMMAND:
						fprintf(pFile,"<td bgcolor=#ffb0b0>");	break;
					case SKILL_AUTO:
						fprintf(pFile,"<td bgcolor=#b0ffb0>");	break;
					case SKILL_FIELD:
						fprintf(pFile,"<td bgcolor=#ffff80>");	break;
					default:
						fprintf(pFile,"<td bgcolor=#80ffff>");	break;
				}

				fprintf(pFile,SkillData[skill].GetName());
				fprintf(pFile,"</td>");
			}
			fprintf(pFile,"</tr>");
		}
			fprintf(pFile,"</table>");
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ENEMY_
	pFile=fopen("data\\ �G�̋����ꗗ.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		for(int i=_OUTPUT_ENEMY_MIN_NO_;i<=_OUTPUT_ENEMY_MAX_NO_;i++){
			fprintf(pFile,"<table border><tr><td colspan='8'>");
			fprintf(pFile,"%d�F%s</td></tr>",i,EnemyBaseData[i].mName);
			fprintf(pFile,"<tr><td>Lv</td><td>LP</td><td>SP</td><td>�U��</td><td>�h��</td><td>���U</td><td>���h</td><td>����</td>\n");
			for(int j=0;j*_OUTPUT_ENEMY_LV_STEP_<=_OUTPUT_ENEMY_MAX_LV_ - _OUTPUT_ENEMY_MIN_LV_;j++){
				const char* color;
				int lv=j*_OUTPUT_ENEMY_LV_STEP_+_OUTPUT_ENEMY_MIN_LV_;
				switch(j%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
				}
				fprintf(pFile,"<tr bgcolor='%s'><td>%d</td>",color,lv);
				CEnemyData edata;
				edata.LoadEnemyData(i,lv);
				fprintf(pFile,"<td>�@%d�@(%d)</td>",edata.GetLP(),edata.GetStm());
				fprintf(pFile,"<td>�@");
				for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
					fprintf(pFile,"%d/",edata.GetMaxSP(k));
				fprintf(pFile,"�@(%d)</td>",edata.GetTech());
				fprintf(pFile,"<td>�@%d�@</td>",edata.GetAtk());
				fprintf(pFile,"<td>�@%d�@</td>",edata.GetDef());
				fprintf(pFile,"<td>�@%d�@</td>",edata.GetMagAtk());
				fprintf(pFile,"<td>�@%d�@</td>",edata.GetMagDef());
				fprintf(pFile,"<td>�@%d�@</td></tr>",edata.GetSpeed());
			}
			fprintf(pFile,"</table>");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ENEMY_SKILL_
	pFile=fopen("data\\ �G�̃X�L���ꗗ.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		#ifdef _OUTPUT_ENEMY_SKILL_BASE_
			fprintf(pFile,"<h3>�o�͊�F");
			if(_OUTPUT_ENEMY_SKILL_BASE_<ID_AS){
				fprintf(pFile,ComSkillData[_OUTPUT_ENEMY_SKILL_BASE_].GetName());
			}else{
				fprintf(pFile,SkillData[_OUTPUT_ENEMY_SKILL_BASE_-ID_AS].GetName());
			}
			fprintf(pFile,"</h3>");
		#endif

		for(int i=_OUTPUT_ENEMY_MIN_NO_;i<=_OUTPUT_ENEMY_MAX_NO_;i++){
			#ifdef _OUTPUT_ENEMY_SKILL_BASE_
				bool write=false;
				for(int j=0;j<EnemyBaseData[i].mSkillNo;j++){
					if(EnemyBaseData[i].mSkill[j]==_OUTPUT_ENEMY_SKILL_BASE_){
						write=true;
						break;
					}
				}
				if(!write)	continue;
			#endif
			fprintf(pFile,"<table border><tr><td colspan='2'>");
			fprintf(pFile,"%d�F%s</td></tr>",i,EnemyBaseData[i].mName);
			int sno=0;
			for(int j=0;j<EnemyBaseData[i].mSkillNo;j++){
				int &skill=EnemyBaseData[i].mSkill[j];

				sno++;
				const char* color;
				int lv=j*_OUTPUT_ENEMY_LV_STEP_+_OUTPUT_ENEMY_MIN_LV_;
				switch(sno%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
				}

				#ifdef _OUTPUT_ENEMY_SKILL_SEARCH_
					if(_OUTPUT_ENEMY_SKILL_SEARCH_START_<=skill && skill<= _OUTPUT_ENEMY_SKILL_SEARCH_END_){
						color="#ffd0d0";
					}
				#endif
				fprintf(pFile,"<tr bgcolor='%s'><td>",color);
				if(skill<ID_AS){
					fprintf(pFile,ComSkillData[skill].GetName());
				}else{
					fprintf(pFile,SkillData[skill-ID_AS].GetName());
				}
				fprintf(pFile,"</td><td>Lv.%d</td></tr>",EnemyBaseData[i].mLearnLv[j]);
			}
			fprintf(pFile,"</table>");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_SKILL_USED_
	pFile=fopen("data\\ �X�L�����g���Ă��邩�̊m�F.html","w");
	if(pFile){
		fprintf(pFile,"<html><body><table border>");
		for(int i=0;i<MAX_CSKILL_NUMBER;i++){
			if(strcmp("",ComSkillData[i].GetName())==0)	continue;
			fprintf(pFile,"<tr><td>%s</td><td>",ComSkillData[i].GetName());
			try{
				for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				for(int k=0;k<MAX_MONTH;k++){
				for(int l=0;l<MAX_SKILL_MONTH;l++){
					int sk=CharacterSkill[j][k][l];
					if(SkillData[sk].mType!=SKILL_COMMAND)	continue;
					if(SkillData[sk].mParam==i){
						fprintf(pFile,CharacterBaseData[j].GetName());
						throw 0;
					}
				}}}
				for(int j=0;j<ENEMY_NO;j++){
					const CEnemyBaseData& eb=EnemyBaseData[j];
					if(!(eb.mName))	continue;
					if(strcmp("",eb.mName)==0)	continue;
					for(int k=0;k<eb.mSkillNo;k++){
						int &sk=eb.mSkill[k];
						if(sk==i){
							fprintf(pFile,eb.mName);
							throw 0;
						}
					}
				}
				for(int j=0;j<MAX_PET_TYPE;j++){
					const CPetData &pd=PetData[j];
					for(int k=0;k<pd.mSkillNo;k++){
						int &sk=pd.mSkill[k];
						if(SkillData[sk].mType!=SKILL_COMMAND)	continue;
						if(SkillData[sk].mParam==i){
							fprintf(pFile,pd.GetName());
							throw 0;
						}
					}
				}
				for(int j=0;j<MAX_EQUIPMENT_NUMBER;j++){
					const CEquipmentData &eq=EquipmentData[j];
					for(int k=0;k<eq.mSkillNo;k++){
						const int &sk=eq.mSkill[k];
						if(SkillData[sk].mType!=SKILL_COMMAND)	continue;
						if(SkillData[sk].mParam==i){
							fprintf(pFile,eq.GetName());
							throw 0;
						}
					}
				}
				fprintf(pFile,"<font color='#ff0000'>���g�p�I�I</font></td></tr>");
			}catch(int n){
				fprintf(pFile,"�@��</td></tr>");
			}
		}	
		for(int i=0;i<MAX_SKILL_NUMBER;i++){
			if(strcmp("",SkillData[i].GetName())==0)	continue;
			if(SkillData[i].mType!=SKILL_AUTO)			continue;

			fprintf(pFile,"<tr><td>%s</td><td>",SkillData[i].GetName());
			try{
				for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				for(int k=0;k<MAX_MONTH;k++){
				for(int l=0;l<MAX_SKILL_MONTH;l++){
					int sk=CharacterSkill[j][k][l];
					if(sk==i){
						fprintf(pFile,CharacterBaseData[j].GetName());
						throw 0;
					}
				}}}
				for(int j=0;j<ENEMY_NO;j++){
					const CEnemyBaseData& eb=EnemyBaseData[j];
					if(!(eb.mName))	continue;
					if(strcmp("",eb.mName)==0)	continue;
					for(int k=0;k<eb.mSkillNo;k++){
						int &sk=eb.mSkill[k];
						if(sk-ID_AS==i){
							fprintf(pFile,eb.mName);
							throw 0;
						}
					}
				}
				for(int j=0;j<MAX_PET_TYPE;j++){
					const CPetData &pd=PetData[j];
					for(int k=0;k<pd.mSkillNo;k++){
						int &sk=pd.mSkill[k];
						if(sk==i){
							fprintf(pFile,pd.GetName());
							throw 0;
						}
					}
				}
				for(int j=0;j<MAX_EQUIPMENT_NUMBER;j++){
					const CEquipmentData &eq=EquipmentData[j];
					for(int k=0;k<eq.mSkillNo;k++){
						const int &sk=eq.mSkill[k];
						if(sk==i){
							fprintf(pFile,eq.GetName());
							throw 0;
						}
					}
				}
				fprintf(pFile,"<font color='#ff0000'>���g�p�I�I</font></td></tr>");
			}catch(int n){
				fprintf(pFile,"�@��</td></tr>");
			}
		}

		fprintf(pFile,"</table></body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_TRESURE_
	pFile=fopen("data\\ �󔠓��e�ꗗ.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		int totalitem[MAX_ITEM_NUMBER];
		memset(totalitem,0,sizeof(totalitem));
		for(int i=0;i<_OUTPUT_TRESURE_MAX_MONTH_;i++){
			if(NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]<0)	break;
			const CMonthData& mdata=MonthData[NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]];
			fprintf(pFile,"<table border><tr><td colspan='2'>");
			fprintf(pFile,"%s</td></tr>",mdata.GetName());

			int item[MAX_ITEM_NUMBER];
			memset(item,0,sizeof(item));
			for(int j=0;j<mdata.mTresureNo;j++){
				const CTreasureData &t=TreasureData[mdata.mTresure[j]];
				for(int k=0;k<t.mItemNo;k++){
					item[t.mItem[k]]++;
					totalitem[t.mItem[k]]++;
				}
			}
			int n=0;
			for(int j=0;j<MAX_ITEM_NUMBER;j++){
				if(item[j]<=0)	continue;
				n++;
				const char* color;
				switch(n%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
				}
				if(j>=ID_SD){
					fprintf(pFile,"<tr bgcolor='%s'><td>�^�l�F%s</td>",color,AllItem[j-ID_SD+ID_UI]->GetName());
				}else{
					fprintf(pFile,"<tr bgcolor='%s'><td>%s</td>",color,AllItem[j]->GetName());
				}
				fprintf(pFile,"<td>�@%d�@�R</td></tr>",item[j]);
			}
		}
		int n=0;
		fprintf(pFile,"<table border><tr><td colspan='2'>���v</td></tr>");
		for(int j=0;j<MAX_ITEM_NUMBER;j++){
			if(totalitem[j]<=0)	continue;
			n++;
			const char* color;
			switch(n%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
			}
			if(j>=ID_SD){
				fprintf(pFile,"<tr bgcolor='%s'><td>�^�l�F%s</td>",color,AllItem[j-ID_SD+ID_UI]->GetName());
			}else{
				fprintf(pFile,"<tr bgcolor='%s'><td>%s</td>",color,AllItem[j]->GetName());
			}
			fprintf(pFile,"<td>�@%d�@�R</td></tr>",totalitem[j]);
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ALL_ITEM_
	pFile=fopen("data\\ �A�C�e�����l���ł��邩�ǂ���.html","w");
	if(pFile){
		int item[MAX_SHOP_NO+1][MAX_ITEM_NUMBER];
		memset(item,0x7f,sizeof(item));

		for(int i=0;i<_OUTPUT_TRESURE_MAX_MONTH_;i++){
			if(NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]<0)	break;
			const CMonthData& mdata=MonthData[NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]];
			for(int j=0;j<mdata.mTresureNo;j++){
				const CTreasureData &t=TreasureData[mdata.mTresure[j]];
				for(int k=0;k<t.mItemNo;k++){
					if(item[0][t.mItem[k]]>0xffff)	item[0][t.mItem[k]]=i;
				}
			}
		}

		const int &hard=_OUTPUT_TRESURE_NORMAL_HARD_;
		for(int i=0;i<shopno[hard]-1;i++){
			for(int j=0;j<_OUTPUT_TRESURE_MAX_MONTH_;j++){
				for(int k=0;k<MAX_SHOP_ITEM;k++){
					const int &it = (ShopData[shopid[hard][i+1]-1][j][k]);
					if(it<0)	break;
					if(item[i+1][it]>0xffff)	item[i+1][it]=j;
				}
			}
		}

		fprintf(pFile,"<html><body><table border><tr>");
		for(int k=0;k<shopno[hard];k++){
			fprintf(pFile,"<td>%s</td>",shopname[shopid[hard][k]]);
		}
		fprintf(pFile,"</tr>");
		int n=0;
		for(int j=0;j<MAX_ITEM_NUMBER;j++){
			const char* color;
			switch(j%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
			}

			fprintf(pFile,"<tr bgcolor='%s'>",color);			

			int get=0xfffffff;
			for(int k=0;k<shopno[hard];k++){
				if(item[k][j]>=0xffff){
					fprintf(pFile,"<td>-</td>");
					continue;
				}
				int &month=item[k][j];
				fprintf(pFile,"<td bgcolor='%s'>%d��</td>",monthcolor[month],month+1);
				if((get&0xff)>month)	get=item[k][j] + k*0x100;
			}
			if(get>0xffffff){
				fprintf(pFile,"<td bgcolor='#ff0000'>����s�I</td>");
			}else{
				int month=(get&0xff);
				int shop=get/0x100;
				fprintf(pFile,"<td bgcolor='%s'>%d���E%s</td>",monthcolor[month],month+1,shopname[shopid[hard][shop]]);
			}
			if(j>=ID_SD){
				fprintf(pFile,"<td>�^�l�F%s</td></tr>",AllItem[j-ID_SD+ID_UI]->GetName());
			}else{
				fprintf(pFile,"<td>%s</td></tr>",AllItem[j]->GetName());
			}
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
		
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ENEMY_MONTH_
	pFile=fopen("data\\ �U�R�G�o����.html","w");
	if(pFile){
		bool enemy[ENEMY_NO+1][_OUTPUT_TRESURE_MAX_MONTH_];
		memset(enemy,0,sizeof(enemy));

		for(int i=0;i<_OUTPUT_TRESURE_MAX_MONTH_;i++){
			if(NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]<0)	break;
			const CMonthData& mdata=MonthData[NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]];
			for(int j=0;j<mdata.mEnemyNo;j++){
				enemy[mdata.mEnemy[j]][i]=true;
			}
		}
		fprintf(pFile,"<html><body><table border><tr>");
		for(int k=0;k<_OUTPUT_TRESURE_MAX_MONTH_;k++){
			fprintf(pFile,"<td>%d��</td>",k+1);
		}
		fprintf(pFile,"<td>�ŏI�o��</td><td>�G�L������</td></tr>");
		fprintf(pFile,"</tr>");
		int n=0;
		for(int j=0;j<ENEMY_NO;j++){
			const char* color;
			switch(j%2){
					case 0:		color="#d0d0ff";	break;
					default:	color="#e0e0ff";	break;
			}

			fprintf(pFile,"<tr bgcolor='%s'>",color);			

			int get=-1;
			for(int k=0;k<_OUTPUT_TRESURE_MAX_MONTH_;k++){
				if(!enemy[j][k]){
					fprintf(pFile,"<td>--</td>");
					continue;
				}
				fprintf(pFile,"<td bgcolor='%s'>��</td>",monthcolor[k],k+1);
				if(get<k)	get=k;
			}
			if(get<0){
				fprintf(pFile,"<td bgcolor='#ffffff'>�o�����܂���</td>");
			}else{
				fprintf(pFile,"<td bgcolor='%s'>%d��</td>",monthcolor[get],get+1);
			}
			fprintf(pFile,"<td>%03d�F%s</td></tr>",j,EnemyBaseData[j].mName);
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
		
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_KEY_DATA_
	pFile=fopen("data\\ �J�M�ꗗ.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		int allkey[MAX_KEY_TYPE];
		memset(allkey,0,sizeof(allkey));
		for(int i=0;i<_OUTPUT_TRESURE_MAX_MONTH_;i++){
			if(NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]<0)	break;
			const CMonthData& mdata=MonthData[NormalHard[_OUTPUT_TRESURE_NORMAL_HARD_][i]];
			fprintf(pFile,"<table border><tr bgcolor='#ffffff'><td colspan='%d'>",MAX_KEY_TYPE+1);
			fprintf(pFile,"%s</td></tr>",mdata.GetName());

			int key[MAX_KEY_TYPE];
			int lock[MAX_KEY_TYPE];
			memset(key,0,sizeof(key));
			memset(lock,0,sizeof(lock));
			for(int j=0;j<mdata.mTresureNo;j++){
				const CTreasureData &t=TreasureData[mdata.mTresure[j]];
				for(int k=0;k<t.mItemNo;k++){
					int item=t.mItem[k]-ID_KEY;
					if(0<=item && item<MAX_KEY_TYPE){
						key[item]++;
						allkey[item]++;
					}
				}
				for(int k=0;k<MAX_KEY_TYPE;k++){
					if(t.mKey & (1<<k)){
						lock[k]++;
						allkey[k]--;
					}
				}
			}

			int n=0;
			fprintf(pFile,"<tr bgcolor='#c0c0c0'><td>���</td>");
			for(int j=0;j<MAX_KEY_TYPE;j++){
				char name[32];
				strcpy(name,AllItem[j + ID_KEY]->GetName());
				name[2]='\0';
				fprintf(pFile,"<td>%s�i%d�j</td>",name,j);
			}
			fprintf(pFile,"</tr><tr bgcolor='#e0e0ff'><td>����</td>");
			for(int j=0;j<MAX_KEY_TYPE;j++){
				fprintf(pFile,"<td>�@%d�@</td>",key[j]);
			}
			fprintf(pFile,"</tr><tr bgcolor='#d0d0ff'><td>����</td>");
			for(int j=0;j<MAX_KEY_TYPE;j++){
				fprintf(pFile,"<td>�@%d�@</td>",lock[j]);
			}
			fprintf(pFile,"</tr><tr bgcolor='#ffd0d0'><td>�̂���</td>");
			for(int j=0;j<MAX_KEY_TYPE;j++){
				fprintf(pFile,"<td>�@%d�@</td>",allkey[j]);
			}
			fprintf(pFile,"</tr></table><br>");
		}
		int n=0;

		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_EXP_

	pFile=fopen("data\\ �o���l�̔{��.txt","w");
	if(pFile){
		for(int i=-MAX_LV;i<MAX_LV;i++){
			double exp=CPlayerData::CulcExp(100,0,i,true);
			fprintf(pFile,"Lv.%+d\t\t%lf\n",-i,exp);
		}
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_GROW_PARAM_
	pFile=fopen("data\\ ���_������.html","w");
	if(pFile){
		fprintf(pFile,"<html><body>");
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			fprintf(pFile,"<table border><tr><td colspan='10'>%s</td></tr>",CharacterBaseData[i].GetName());
			fprintf(pFile,"<td></td><td>LP (�̗�)</td><td>SP</td><td>�U��</td><td>�h��</td><td>���U</td><td>���h</td><td>����</td><td>����</td><td>�����i</td></tr>");

			int param[9];
			param[0]=CharacterBaseData[i].mLP;
			param[1]=CharacterBaseData[i].mTech;
			param[2]=CharacterBaseData[i].mAtk;
			param[3]=CharacterBaseData[i].mDef;
			param[4]=CharacterBaseData[i].mMagAtk;
			param[5]=CharacterBaseData[i].mMagDef;
			param[6]=CharacterBaseData[i].mSpeed;
			param[7]=MIN_USE_ITEM;
			param[8]=MIN_ACCESSORY;

			for(int j= -1;j<MAX_MONTH-1;j++){
				if(j>=0){
					for(int k=0;k<MAX_SKILL_MONTH;k++){
						int skill=CharacterSkill[i][j][k];
						if(SkillData[skill].mType==SKILL_STATUS){
							const int &p=SkillData[skill].mParam;
							int	up=p&0xffff;
							for(int l=0;l<9;l++){
								if((p>>(16+l))&0x01){
									param[l]+=up ;
								}
							}
						}
					}
				}
				//�o�͕�
				if(j%_OUTPUT_GROW_PARAM_STEP_ == _OUTPUT_GROW_PARAM_STEP_-1 || j<0){
					int color;
					switch(j%2){
						case 0:		color=0xd0d0ff;	break;
						default:	color=0xe0e0ff;	break;
					}
					if(j>=0){
						fprintf(pFile,"<tr><td>%d��</td>",j+1);
					}else{
						fprintf(pFile,"<tr><td>�����l</td>",j+1);
					}

					CPlayerData cdata;
					cdata.mStm=param[0];
					cdata.mTech=param[1];
					cdata.mLv=(j+1)*5;
					cdata.CalculateLP();
					cdata.CalculateSP();
					fprintf(pFile,"<td bgcolor=#%x>�@%d�@(%d)</td>",color,cdata.mLP,cdata.GetStm());
					fprintf(pFile,"<td bgcolor=#%x>�@",color);
					for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
						fprintf(pFile,"%d/",cdata.GetMaxSP(k));
					fprintf(pFile," (%d)</td>�@",param[1]);

					for(int k=2;k<9;k++){
						fprintf(pFile,"<td bgcolor=#%x>%d</td>",color,param[k]);
					}
					fprintf(pFile,"</tr>");
				}
			}
			fprintf(pFile,"</table>");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�@�@�@�摜���֘A
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ENEMY_IMG_
	pFile=fopen("data\\ �G�̉摜�ꗗ.html","w");
	if(pFile){
		int enemy[ENEMY_NO];
		int enemyimg[ENEMY_NO];
		int i;
		for(i=0;i<ENEMY_NO;i++){
			enemy[i]=i;
			enemyimg[i]=EnemyBaseData[i].mMapImage;
		}
		bool turn=true;
		while(turn){
			turn=false;
			for(int n=1;n<ENEMY_NO;n++){
				if(enemyimg[n]<enemyimg[n-1]){
					turn=true;
					int img=enemyimg[n];
					enemyimg[n]=enemyimg[n-1];
					enemyimg[n-1]=img;
					int e=enemy[n];
					enemy[n]=enemy[n-1];
					enemy[n-1]=e;
				}
			}
		}

		fprintf(pFile,"<html><body>");
		i=0;
		while(1){
			int img=enemyimg[i];
			fprintf(pFile,"<ul><li>%d</li><ul>",img);
			for(;i<ENEMY_NO;i++){
				if(enemyimg[i]!=img)	break;
				fprintf(pFile,"<li>%s</li>",EnemyBaseData[enemy[i]].mName);
			}
			fprintf(pFile,"</ul></ul>");
			if(i>=ENEMY_NO)	break;
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_ITEM_IMG_
	pFile=fopen("data\\ �A�C�e���̃A�C�R���ꗗ.html","w");
	if(pFile){
		int item[MAX_ITEM_NUMBER];
		int itemimg[MAX_ITEM_NUMBER];
		int i;
		for(i=0;i<MAX_ITEM_NUMBER;i++){
			item[i]=i;
			itemimg[i]=AllItem[i]->mIcon;
		}
		bool turn=true;
		while(turn){
			turn=false;
			for(int n=1;n<MAX_ITEM_NUMBER;n++){
				if(itemimg[n]<itemimg[n-1]){
					turn=true;
					int img=itemimg[n];
					itemimg[n]=itemimg[n-1];
					itemimg[n-1]=img;
					int it=item[n];
					item[n]=item[n-1];
					item[n-1]=it;
				}
			}
		}
		fprintf(pFile,"<html><body>");
		fprintf(pFile,"<h1>�A�C�e���̃A�C�R���ꗗ</h1><hr size='16' color='#8080ff'>");
		for(int i=0;i<5;i++){
			fprintf(pFile,"<h3><a href='�A�C�e���̃A�C�R���ꗗ%d.html'>icon%d.bmp</a></h3>",i,i);
		}
		i=0;
		int oldpage=-1;
		int oldy=-1;
		while(1){
			int img=itemimg[i];
			int x=img%8;
			int y=(img/8)%8;
			int page=img/64;
			if(img>500)	break;
			if(page!=oldpage){
				char fname[256];
				fprintf(pFile,"</body></html>");
				fclose(pFile);
				sprintf(fname,"data\\�A�C�e���̃A�C�R���ꗗ%d.html",page);
				pFile=fopen(fname,"w");
				fprintf(pFile,"<html><body>");
				fprintf(pFile,"<hr size='16' color='#8080ff'><h1>icon%d.bmp</h1>",page);
				fprintf(pFile,"<img src='itemicon/icon%d.bmp'>",page);

				int j=i;
				fprintf(pFile,"<table border>");
				for(int y=0;y<8;y++){
					fprintf(pFile,"<tr>");
					for(int x=0;x<8;x++){
						int img=itemimg[j];
						int imgx=img%8;
						int imgy=(img/8)%8;
						if(x==imgx && y==imgy){
							fprintf(pFile,"<td bgcolor='#eeeeaa'>");
							fprintf(pFile,"<a href='#%d-%d'><font size='2'>",x,y);
							for(;j<MAX_ITEM_NUMBER;j++){
								if(itemimg[j]!=img)	break;
								fprintf(pFile,"%s<br>",AllItem[item[j]]->GetName());
							}
							fprintf(pFile,"</font></a>");
							
						}else{
							fprintf(pFile,"<td>");
						}
						fprintf(pFile,"</td>");
					}
					fprintf(pFile,"</tr>");
				}
				fprintf(pFile,"</table>");

				oldpage=page;
				oldy=-1;
			}
			if(y!=oldy){
				fprintf(pFile,"<hr size='8' color='#ff8080'><h2>�ォ��%d�s��</h2>",y+1);
				oldy=y;
			}
			fprintf(pFile,"<a name='%d-%d'><hr size='4' color='#00ff0'><h3>%d�Ԗ�</h3><ul>",x,y,x+1);
			for(;i<MAX_ITEM_NUMBER;i++){
				if(itemimg[i]!=img)	break;
				fprintf(pFile,"<li><b>%s</b></li>",AllItem[item[i]]->GetName());
				fprintf(pFile,"<ul><li>%s</li>",AllItem[item[i]]->GetTypeName());
				const char *help=MenuData.GetItemText(item[i]);
				int len=strlen(help)+2;
				char *nh=new char[len+2];
				strcpy(nh,help);
				fprintf(pFile,"<li>");

				int print=0;
				for(int j=2;j<len;j++){
					if(nh[j]=='\n'){
						nh[j]='\0';
						fprintf(pFile,"%s<br>",&nh[print]);
						print=j+1;
					}else if(nh[j]=='\0'){
						fprintf(pFile,"%s",&nh[print]);
						break;
					}
				}
				fprintf(pFile,"�@</li></ul>",help);
			}
			fprintf(pFile,"</ul>\n");
			if(i>=MAX_ITEM_NUMBER)	break;
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_SKILL_EFFECT_
	pFile=fopen("data\\ �G�t�F�N�g�ʃX�L���ꗗ.txt","w");
	if(pFile){
		const int EFFECT_MAX=256;
		int effect[EFFECT_MAX][MAX_CSKILL_NUMBER];
		memset(effect,-1,sizeof(effect));
		for(int i=0;i<MAX_CSKILL_NUMBER;i++){
			for(int j=0;j<MAX_QUICK_LOAD;j++){
				int e=ComSkillData[i].mQuickLoad[j];
				if(e<0)	break;
				int k=0;
				while(effect[e][k]>=0)	k++;
				effect[e][k]=i;
			}
		}

		for(int i=0;i<EFFECT_MAX;i++){
			if(effect[i][0]<0)	continue;
			fprintf(pFile,"\n//////////////////////////////////////\n!%d\n",i);
			int k=0;
			while(effect[i][k]>=0){
				fprintf(pFile,"*%s\n",ComSkillData[effect[i][k]].GetName());
				k++;
			}
		}
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�@�@�@Wiki�p
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_WIKI_ITEM_
	pFile=fopen("data\\ wiki�p�A�C�e���ꗗ.txt","w");
	if(pFile){
		int ctg=0;
		for(int i=0;i<MAX_EQUIPMENT_NUMBER+MAX_USE_ITEM_NUMBER;i++){
			if(ctg+1<CATEGORY_MAX && ItemCategory[ctg+1].mMin<=i){
				ctg++;
				fprintf(pFile,"!!%s\n",ItemCategory[ctg].GetName());
			}
			if(strcmp(AllItem[i]->GetName(),"")){
				fprintf(pFile,"::%s\n",AllItem[i]->GetName());
				if(i<MAX_EQUIPMENT_NUMBER){
					fprintf(pFile,"*���\\n**");
					const CEquipmentData &eq=EquipmentData[i];
					bool param=false;
					if(eq.mLP)		{fprintf(pFile,"LP%+d�@",eq.mLP);		param=true;}
					if(eq.mAtk)		{fprintf(pFile,"AT%+d�@",eq.mAtk);		param=true;}
					if(eq.mDef)		{fprintf(pFile,"DF%+d�@",eq.mDef);		param=true;}
					if(eq.mMagAtk)	{fprintf(pFile,"MA%+d�@",eq.mMagAtk);	param=true;}
					if(eq.mMagDef)	{fprintf(pFile,"MD%+d�@",eq.mMagDef);	param=true;}
					if(eq.mSpeed)	{fprintf(pFile,"SPD%+d�@",eq.mSpeed);	param=true;}
					if(!param)		{fprintf(pFile,"�C���l����");}

					if(eq.mSkillNo){
						for(int i=0;i<eq.mSkillNo;i++){
							fprintf(pFile,"%s�@",SkillData[eq.mSkill[i]].GetName());
						}
					}
				}
				fprintf(pFile,"\n*�R�����g\n**\n");
			}
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_WIKI_CSKILL_
	pFile=fopen("data\\ wiki�p�b�X�L���ꗗ.txt","w");
	if(pFile){
		int ctg=0;
		for(int i=8;i<128;i++){
			if(strcmp(ComSkillData[i].GetName(),"")){
				const CComSkillData &data=ComSkillData[i];
				fprintf(pFile,"::%s\n*���\\n**",data.GetName());
				
				fprintf(pFile,"SLv.%d�@",data.mSP);
				const char *areaname[10] = {
					"---",
					"�������g",
					"�P��",
					"�P��",
					"�c����",
					"������",
					"�͈�(��)",
					"�͈�(��)",
					"�S��",
					"�G�����S��",
				};

				int area=0;
				for(int i=0;i<8;i++){
					if(data.mRange & (1<<i)){
						area=i+1;
					}
				}
				if(data.mRange==CR_S_ALL){
					fprintf(pFile," �G�����S��");
				}else if(data.mRange==CR_NONE){
					fprintf(pFile," ---");
				}else{
					fprintf(pFile, "%s�E%s",(data.mTarget?"�G":"����"),areaname[area]);
				}
				fprintf(pFile,"\n**");
				char temp[256],*text;
				int index=0;
				sprintf(temp,"data\\help\\comskill%02d",i/32);
				text=CMenuData::GetHelpText(temp,i);

				bool loop=true;

				while(loop){
					int n=0;
					bool read=true;
					while(read){
						switch(text[n+index]){
							case '\0':
								loop=false;
								read=false;
								temp[n]='\0';
								break;
							case '\n':
								read=false;
								temp[n-1]='\0';
								break;
							default:	temp[n]=text[n+index];
						}
						n++;
					}
					index+=n;

					bool write=false;
					while(temp[0]==char(0x81) && temp[1]==char(0x40)){
						write=true;
						strcpy(temp,temp+2);
					}
					if(write){
						if(temp[0]==char(0x84) && (temp[1]==char(0xa4) || temp[1]==char(0xa5))){
							fprintf(pFile,"[%s]",temp+2);
						}else{
							fprintf(pFile,"%s�@",temp);
						}
					}
				}

				delete[] text;
				

				fprintf(pFile,"\n*�R�����g\n**(����)\n");
			}
		}
	}
#endif
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_WIKI_MONTH_
	pFile=fopen("data\\ wiki�p�����ꗗ.txt","w");
	if(pFile){
		int ctg=0;
		for(int i=0;i<MAX_ALL_MONTH;i++){
			if(strcmp(MonthData[i].GetName(),"")){
				fprintf(pFile,"!!%d���@%s\n",MonthData[i].mMonth,MonthData[i].GetName());
				fprintf(pFile,"!�G���\n::�U�R\n");
				for(int j=0;j<MonthData[i].mEnemyNo;j++){
					fprintf(pFile,"*%s\n",EnemyBaseData[MonthData[i].mEnemy[j]].mName);
				}
				fprintf(pFile,"::�{�X\n");
				fprintf(pFile,"::�R�����g\n*(����)\n");
				fprintf(pFile,"!�V�i���I�ɂ���\n*(����)\n");
				fprintf(pFile,"!�_���W�����ɂ���\n*(����)\n");
			}
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_WIKI_PLAYER_
	pFile=fopen("data\\ wiki�p�v���C���[���ꗗ.txt","w");
	if(pFile){
		int final_param[9][MAX_CHARACTER_NUMBER];
		memset(final_param,0,sizeof(final_param));
		int ctg=0;
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			if(strcmp("",CharacterBaseData[i].GetName())==0)	continue;
			fprintf(pFile,"!!%s\n",CharacterBaseData[i].GetName());
			fprintf(pFile,"!�K���X�L��\n");
			fprintf(pFile,",��,,,,,,,,��\n");
			for(int j=0;j<MAX_MONTH-1;j++){
				fprintf(pFile,",%d��",j+1);
				for(int k=0;k<MAX_SKILL_MONTH;k++){
					int skill=CharacterSkill[i][j][k];
					fprintf(pFile,",");
					fprintf(pFile,SkillData[skill].GetName());
				}
				fprintf(pFile,"\n");
			}
			fprintf(pFile,"!�p�����[�^�[���z�l\n");
			fprintf(pFile,",,LP,SP,�U��,�h��,���U,���h,����,����,�����i");

			int param[9];
			param[0]=CharacterBaseData[i].mLP;
			param[1]=CharacterBaseData[i].mTech;
			param[2]=CharacterBaseData[i].mAtk;
			param[3]=CharacterBaseData[i].mDef;
			param[4]=CharacterBaseData[i].mMagAtk;
			param[5]=CharacterBaseData[i].mMagDef;
			param[6]=CharacterBaseData[i].mSpeed;
			param[7]=MIN_USE_ITEM;
			param[8]=MIN_ACCESSORY;

			for(int j= -1;j<MAX_MONTH-1;j++){
				if(j>=0){
					for(int k=0;k<MAX_SKILL_MONTH;k++){
						int skill=CharacterSkill[i][j][k];
						if(SkillData[skill].mType==SKILL_STATUS){
							const int &p=SkillData[skill].mParam;
							int	up=p&0xffff;
							for(int l=0;l<9;l++){
								if((p>>(16+l))&0x01){
									param[l]+=up ;
								}
							}
						}
					}
				}
				//�o�͕�
				if(j%_OUTPUT_GROW_PARAM_STEP_ == _OUTPUT_GROW_PARAM_STEP_-1 || j<0){
					if(j>=0){
						fprintf(pFile,"\n,%d��",j+1);
					}else{
						fprintf(pFile,"\n,�����l",j+1);
					}

					CPlayerData cdata;
					cdata.mStm=param[0];
					cdata.mTech=param[1];
					cdata.mLv=(j+1)*5;
					cdata.CalculateLP();
					cdata.CalculateSP();
					fprintf(pFile,",%d",cdata.mLP);
					fprintf(pFile,",");
					for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
						fprintf(pFile,"%d/",cdata.GetMaxSP(k));

					for(int k=2;k<9;k++){
						fprintf(pFile,",%d",param[k]);
					}
				}
			}
			fprintf(pFile,"\n!�R�����g\n*(����)\n");
			for(int j=0;j<9;j++){
				final_param[j][i]=param[j];
			}
		}
		//�����L���O
		fprintf(pFile,"\n!!!�p�����[�^�ʁE���z�l�����L���O");
		for(int i=0;i<7;i++){
			int param[2][MAX_CHARACTER_NUMBER];
			for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				param[0][j]=j;
			}
			memcpy(param[1],final_param[i],sizeof(int)*MAX_CHARACTER_NUMBER);

			bool change=true;
			while(change){
				change=false;
				for(int j=1;j<MAX_CHARACTER_NUMBER;j++){
					if(param[1][j]>param[1][j-1]){
						int no=param[0][j];
						int p=param[1][j];

						param[0][j]=param[0][j-1];
						param[1][j]=param[1][j-1];

						param[0][j-1]=no;
						param[1][j-1]=p;

						change=true;
					}
				}
			}

			switch(i){
				case 0:	fprintf(pFile,"\n!!LP");	break;
				case 1:	fprintf(pFile,"\n!!SP");	break;
				case 2:	fprintf(pFile,"\n!!�U����");	break;
				case 3:	fprintf(pFile,"\n!!�h���");	break;
				case 4:	fprintf(pFile,"\n!!���@�U��");	break;
				case 5:	fprintf(pFile,"\n!!���@�h��");	break;
				case 6:	fprintf(pFile,"\n!!���΂₳");	break;
			}
			fprintf(pFile,"\n,����,���O,�p�����[�^");
			for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				if(strcmp("",CharacterBaseData[param[0][j]].GetName())==0)	continue;
				CPlayerData cdata;
				fprintf(pFile,"\n,%d,%s,",j+1,CharacterBaseData[param[0][j]].GetName());
				switch(i){
					case 0:
						cdata.mStm=param[1][j];
						cdata.mLv=60;
						cdata.CalculateLP();
						fprintf(pFile,"%d",cdata.mLP);
						break;
						
					case 1:
						cdata.mTech=param[1][j];
						cdata.CalculateSP();
						for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
							fprintf(pFile,"%d/",cdata.GetMaxSP(k));
						break;
					default:
						fprintf(pFile,"%d",param[1][j]);
						break;
				}
			}
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_MAP_BMP_
	{


		for(int i=160;i<=182;i++)
			Flag.mTreasure[i]=true;

		CDTTexture	tex;
		for(int no=_OUTPUT_MAP_BMP_START_;no<=_OUTPUT_MAP_BMP_END_;no++){
			Flag.mFlag[0]=1024;
			Flag.mFlag[2]=1024;
			Flag.mFlag[7]=1024;
			Flag.mFlag[8]=1024;
			Flag.mFlag[9]=1024;
			Flag.mFlag[14]=1024;
			Flag.mFlag[18]=1024;
			Flag.mFlag[20]=1024;
			Flag.mFlag[27]=1024;
			Flag.mFlag[35]=1024;
			Flag.mFlag[37]=1024;
			Flag.mFlag[39]=1024;
			Flag.mFlag[40]=1024;
			Flag.mFlag[41]=1024;
			Flag.mFlag[44]=10;
			
			Flag.mFlag[45]=1024;
			Flag.mFlag[46]=1024;
			Flag.mFlag[47]=1024;
			Flag.mFlag[48]=1024;
			Flag.mFlag[49]=1024;
			Flag.mFlag[50]=1024;

			Flag.mFlag[56]=1024;
			Flag.mFlag[64]=1024;
			Flag.mFlag[65]=1024;
			Flag.mFlag[73]=1024;
			Flag.mFlag[79]=1024;
			Flag.mFlag[83]=1024;
			Flag.mFlag[85]=1024;
			Flag.mFlag[86]=1024;
			Flag.mFlag[87]=1024;
			Flag.mFlag[122]=1024;
			Flag.mFlag[123]=1024;
			Flag.mFlag[125]=1024;
			Flag.mFlag[126]=1024;
			
			
			
			MapMode.LoadMap(no,0,0,0,0);
			int x,y;
			MapMode.GetMapSize(&x,&y);
			int size=x+y;
			DTest.CreateTexture(tex,
				(size*32)*_OUTPUT_MAP_BMP_SIZE_/32,
				(size*16)*_OUTPUT_MAP_BMP_SIZE_/32+480
				);
			DTest.SetRenderTarget(&tex);
			DTest.Clear(0xffffffff);
			MapMode.OnDraw();
			DTest.SetRenderTarget(NULL);
			char fname[32];
			sprintf(fname,"mapimg%04d.png",no);
			tex.SaveTexture(fname);
			tex.Release();
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_MONTH_ENEMY_REAL_
	//MonthMapData
	pFile=fopen("_�U�����\\���ڍ׌��ʓG���.txt","w");
	if(pFile){
		POINT MapData[19][5];
#ifdef _OUTPUT_MONTH_ENEMY_REAL_LD_MODE_
		memcpy(MapData,LDMapData,sizeof(MapData));
#else
		memcpy(MapData,MonthMapData,sizeof(MapData));
#endif

		int allenemy[ENEMY_NO][10];
		for(int month=0;month<MAX_ALL_MONTH;month++){
			const CMonthData &mdata=MonthData[month];
			fprintf(pFile,"/////////////////////////////////////////////////////\n");
			fprintf(pFile,"%d�� %s//////////////////////////////////////////////\n",mdata.mMonth,mdata.GetName());
			fprintf(pFile,"/////////////////////////////////////////////////////\n");
			fprintf(pFile,"**������������G�p�[�e�B�[**\n");

			memset(allenemy,-100,sizeof(allenemy));
			for(int i=0;i<5;i++){
				POINT point=MapData[month][i];
				if(point.x==0 && point.y==0)	break;
				for(int no=point.x;no<=point.y;no++){
					
					memset(Flag.mFlag,0,sizeof(Flag.mFlag));
					bool mapname=false;

					MapMode.LoadMap(no,0,0,0,0);
					int lv[10];
					bool enemy[ENEMY_NO];
					memset(lv,-100,sizeof(lv));
					memset(enemy,0,sizeof(enemy));
					for(int obj=1;obj<OBJECT_NO;obj++){
						if(!MapMode.mObject[obj])	continue;
						if(MapMode.mObject[obj]->GetObjType() == MapObjEnemy){
							CEnemyObject *eobj=(CEnemyObject*)MapMode.mObject[obj];
							if(eobj->mParty<0){
								int l=eobj->mLv/4*4;
								if(eobj->mMember==1){
									AddEnemy(allenemy,eobj->mReader,l);
								}else{
									AddLv(lv,l);
									enemy[eobj->mReader]=true;
								}
								if(eobj->mOption!=0){
									if(!mapname){
										mapname=true;
										fprintf(pFile,Flag.mMapName);
										fprintf(pFile,"\n");
									}
									fprintf(pFile,"�@���[�_�[�F%d %s �I�v�V�����F0x%08x\n",eobj->mReader,EnemyBaseData[eobj->mReader].mName,eobj->mOption);
								}
							}else{
								const CEnemyPartyData &party=EnemyPartyData[eobj->mParty];
								for(int j=0;j<MAX_ENEMY_NUMBER;j++){
									if(party.mEnemy[j]<0)	break;
									AddEnemy(allenemy,party.mEnemy[j],party.mLv[j]);
								}
								if(party.mOption!=0){
									if(!mapname){
										mapname=true;
										fprintf(pFile,Flag.mMapName);
										fprintf(pFile,"\n");
									}
									fprintf(pFile,"�@�Œ�p�[�e�B�[�F%d\n",eobj->mParty);
								}
							}
						}
					}
					CEventData *eve = &(MapMode.mEventData);
					while(eve){
						if(eve->mEvent == EVE_BATTLE){
							int pno=eve->mData[0].GetNumber();
							const CEnemyPartyData &party=EnemyPartyData[pno];
							for(int j=0;j<MAX_ENEMY_NUMBER;j++){
								if(party.mEnemy[j]<0)	break;
								AddEnemy(allenemy,party.mEnemy[j],party.mLv[j]);
							}
							if(party.mOption!=0){
								if(!mapname){
									mapname=true;
									fprintf(pFile,Flag.mMapName);
									fprintf(pFile,"\n");
								}
								fprintf(pFile,"�@�Œ�p�[�e�B�[�F%d\n",pno);
							}
						}
						eve=eve->mNext;
					}

					for(int ene=0;ene<Flag.mEnemyTypeNo;ene++){
						enemy[Flag.mEnemyType[ene]]=true;
					}
					for(int l=0;l<10;l++){
						if(lv[l]<=-100)	break;
						for(int e=0;e<ENEMY_NO;e++){
							if(!enemy[e])	continue;
							AddEnemy(allenemy,e,lv[l]);
						}
					}
				}
			}
			for(int e=0;e<ENEMY_NO;e++){
				//sort
				if(allenemy[e][0]<=-100)	continue;
				bool swap=true;
				while(swap){
					swap=false;
					int no=1;
					while(allenemy[e][no]>-100){
						if(allenemy[e][no]<allenemy[e][no-1]){
							int l=allenemy[e][no];
							allenemy[e][no]=allenemy[e][no-1];
							allenemy[e][no-1]=l;
							swap=true;
						}
						no++;
					}
				}
			}

			fprintf(pFile,"\n**�\�[�X�R�[�h��{�G�ԍ�,Lv,Lv�c},***************\n");
			
			fprintf(pFile,"{//%d���F%s\n",mdata.mMonth,mdata.GetName());
			for(int e=0;e<ENEMY_NO;e++){
				if(allenemy[e][0]<=-100)	continue;
				fprintf(pFile,"\t\t\t{\t%d,\t",e);
				for(int l=0;l<10;l++){
					if(allenemy[e][l]<=-100)	break;
					fprintf(pFile,"%d,",allenemy[e][l]);

				}
				fprintf(pFile,"\t-100},\n");
			}
			fprintf(pFile,"\t\t{-100}},\n");
			
#ifndef _OUTPUT_MONTH_ENEMY_REAL_LD_MODE_
			fprintf(pFile,"\n**����o�^�ς݂̃U�R*************\n");
			for(int i=0;i<mdata.mEnemyNo;i++){
				int e=mdata.mEnemy[i];
				fprintf(pFile,"%d:%s\t\tLv",e,EnemyBaseData[e].mName);
				if(allenemy[e][0]<=-100){
					fprintf(pFile,"!!!!!!!�o�����Ă��Ȃ��I�H!!!!!!\n");
					continue;
				}
				for(int l=0;l<10;l++){
					if(allenemy[e][l]<=-100)	break;
					fprintf(pFile,"%d,",allenemy[e][l]);

				}
				fprintf(pFile,"\n");
				memset(allenemy[e],-100,sizeof(int)*10);
			}
#endif

			fprintf(pFile,"\n**���̑��̓G***************\n");
			for(int e=0;e<ENEMY_NO;e++){
				if(allenemy[e][0]<=-100)	continue;
				fprintf(pFile,"%d:%s\t\tLv",e,EnemyBaseData[e].mName);
				for(int l=0;l<10;l++){
					if(allenemy[e][l]<=-100)	break;
					fprintf(pFile,"%d,",allenemy[e][l]);

				}
				fprintf(pFile,"\n");
			}

			fflush(pFile);
		}
		fclose(pFile);
	}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_CSKILL_
	pFile=fopen("_�U�����\\cskill.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>�Z�E���@�E����@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>�Z�E���@�E����@�ꗗ</h2>");
		int ctg=0;
		const int typemax=18;
		const int type[typemax]={
			CS_SWORD,
			CS_SPEAR,
			CS_AXE,
			CS_BOW,
			CS_SKILL,
			CS_MAGIC_FIRE,
			CS_MAGIC_ICE,
			CS_MAGIC_WIND,
			CS_MAGIC_EARTH,
			CS_MAGIC,
			CS_OTHER_HEAL,
			CS_OTHER_FIELD,
			CS_OTHER_ST,
			CS_OTHER,
			CS_OTHER_PET,
			CS_SKILL | CS_SPEAR |CS_SWORD|CS_AXE|CS_BOW,
			CS_MAGIC,
			CS_OTHER|CS_ENEMY_ITEM
		};
		const char* typen[typemax]={
			"�Z�E���Z",
			"�Z�E���Z",
			"�Z�E���Z",
			"�Z�E�|�Z",
			"�Z�E���̑�",

			"���@�E��",
			"���@�E�X",
			"���@�E��",
			"���@�E�y",
			"���@�E������",

			"����E�񕜌n",
			"����E���E�n",
			"����EST�ُ�n",
			"����E���̑�",
			"����E�y�b�g�Ă�",

			"�G��p�E�Z",
			"�G��p�E���@",
			"�G��p�E����"
		};
		fprintf(pFile,"<ul>");
		for(int c=0;c<typemax;c++){
			fprintf(pFile,"<li><a href='#cs_ctg_%d'>%s</a></li>",c,typen[c]);
			if(c%5==4)	fprintf(pFile,"<hr class='short'>");
		}
		fprintf(pFile,"</ul><hr class='bold'>\n");
		for(int c=0;c<typemax;c++){

			fprintf(pFile,"<a name='cs_ctg_%d'></a><h3>%s</h3>\n",c,typen[c]);
			for(int i=8;i<256;i++){
				int file_top= ftell(pFile);
				if(!strcmp(ComSkillData[i].GetName(),""))	continue;
				const CComSkillData &data=ComSkillData[i];
				if(c<15){
					if((data.mType&0xfffff)!=type[c])	continue;
				}else{
					if(!(data.mType&type[c]))	continue;
					if(i<144)	continue;
				}

				fprintf(pFile,"<a name='cs_%d'></a>",i);
				fprintf(pFile,"<table class='cs'><tr><td rowspan='2' colspan='2' class='sname'>%s</td>",data.GetName());
				
				fprintf(pFile,"<th class='csinfo'>SLv</td><td>%d</td></tr><tr><th class='csinfo'>�Ώ�</td><td>",data.mSP);
				const char *areaname[10] = {
					"---",
					"�������g",
					"�P��",
					"�P��",
					"�c����",
					"������",
					"�͈�(��)",
					"�͈�(��)",
					"�S��",
					"�G�����S��",
				};

				int area=0;
				for(int i=0;i<8;i++){
					if(data.mRange & (1<<i)){
						area=i+1;
					}
				}
				if(data.mRange==CR_S_ALL){
					fprintf(pFile," �G�����S��");
				}else if(data.mRange==CR_NONE){
					fprintf(pFile," ---");
				}else{
					fprintf(pFile, "%s�E%s",(data.mTarget?"�G":"����"),areaname[area]);
				}
				fprintf(pFile,"</td></tr><tr><th class='csinfo'>����</th><td colspan='3'>");
				char temp[256],*text;
				int index=0;
				sprintf(temp,"data\\help\\comskill%02d",i/32);
				text=CMenuData::GetHelpText(temp,i);

				bool loop=true;
				int line=0;
				while(loop){
					int n=0;
					bool read=true;
					while(read){
						switch(text[n+index]){
							case '\0':
								loop=false;
								read=false;
								temp[n]='\0';
								break;
							case '\n':
								read=false;
								temp[n-1]='\0';
								break;
							default:	temp[n]=text[n+index];
						}
						n++;
					}
					index+=n;

					bool write=false;
					line++;
					write=(line==3);
					while(temp[0]==char(0x81) && temp[1]==char(0x40)){
						write=true;
						strcpy(temp,temp+2);
					}
					if(write){
						if(temp[0]==char(0x84) && (temp[1]==char(0xa4) || temp[1]==char(0xa5))){
							fprintf(pFile,"�@<span class='subtext'>[ %s ]</span><br>",temp+2);
						}else{
							fprintf(pFile,"%s<br>",temp);
						}
						if(line==3)	fprintf(pFile,"<br>");
					}
				}
				if(117<=i && i<=126){
					int pno=i-117;
					fprintf(pFile,"<table><tr><td><table class='slim'><tr><td>�p�����[�^�{��</td></tr>");
					fprintf(pFile,"<tr><td class='slim'>LP - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mLP);
					fprintf(pFile,"<tr><td class='slim'>SP - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mTech);
					fprintf(pFile,"<tr><td class='slim'>�U���� - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mAtk);
					fprintf(pFile,"<tr><td class='slim'>�h��� - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mDef);
					fprintf(pFile,"<tr><td class='slim'>���@�U�� - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mMagAtk);
					fprintf(pFile,"<tr><td class='slim'>���@�h�� - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mMagDef);
					fprintf(pFile,"<tr><td class='slim'>���΂₳ - </td><td class='slim'>�@%d ��</td></tr>",PetData[pno].mSpeed);
					fprintf(pFile,"</table></td><td><table class='slim'><tr><td>�K���X�L��</tr></td>");
					for(int s=0;s<PetData[pno].mSkillNo;s++){
						fprintf(pFile,"<tr><td class='skill_c");
						int skill=PetData[pno].mSkill[s];
						OutputDBSkill(pFile,skill);
						fprintf(pFile,"</tr>");
					}
					fprintf(pFile,"</table></td></tr></table>");
				}

				fprintf(pFile,"</td></tr>");

#define	LEARN	{if(!learn){fprintf(pFile,"</td></tr><tr><th class='csinfo'>�K��</th><td colspan='3'><table>");learn=true;}}

				delete[] text;
				bool learn=false;
				bool learn_c=false;
				for(int chara=0;chara<MAX_CHARACTER_NUMBER;chara++){
					for(int month=0;month<MAX_MONTH;month++){
						for(int skill=0;skill<MAX_SKILL_MONTH;skill++){
							const CSkillData &data=SkillData[CharacterSkill[chara][month][skill]];
							if(data.mType!=SKILL_COMMAND)	continue;
							if(data.mParam==i){
								LEARN ;
								if(!learn_c){
									learn_c=true;
									fprintf(pFile,"<tr><td><span class='subtext'>�E�����o�[�@</span></td><td>");

								}
								fprintf(pFile,"<a href='player.html#pl_%d'>%s</a>�E%d��%s<br>",chara,CharacterBaseData[chara].GetName(),month+1,((skill==MAX_SKILL_MONTH-1)?"(��)":""));
							}
						}
					}
				}
				if(learn_c)		fprintf(pFile,"</td></tr>");
				bool learn_e=false;
				for(int eq=0;eq<MAX_EQUIPMENT_NUMBER;eq++){
					const CEquipmentData &eqd=EquipmentData[eq];
					for(int skill=0;skill<eqd.mSkillNo;skill++){
						const CSkillData &data=SkillData[eqd.mSkill[skill]];
						if(data.mType!=SKILL_COMMAND)	continue;
						if(data.mParam==i){
							LEARN ;
							if(!learn_e){
								learn_e=true;
								fprintf(pFile,"<tr><td><span class='subtext'>�E�����i�@</span></td><td>");

							}
							fprintf(pFile,"�y%s�z�@<a href='eq.html#eq_%d'>%s</a><br>",eqd.GetTypeName(),eq,eqd.GetName());
						}
					}
				}
				if(learn_e)		fprintf(pFile,"</td></tr>");


				if(learn)		fprintf(pFile,"</table>");
				fprintf(pFile,"</td></tr></table>\n");
				if(c<15){
					if(!learn)	fseek(pFile,file_top,SEEK_SET);
				}else{
					if(learn)	fseek(pFile,file_top,SEEK_SET);
				}
			}
		}
		int file_c= ftell(pFile);
		fseek(pFile,0,SEEK_END);
		int file_end= ftell(pFile);
		fseek(pFile,file_c,SEEK_SET);
		for(int i=file_c;i<file_end;i++)	putc(' ',pFile);

		fprintf(pFile,"\n</body></html>");
		fclose(pFile);
	}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_ASKILL_
	pFile=fopen("_�U�����\\askill.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>�I�[�g�X�L���@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>�I�[�g�X�L���@�ꗗ</h2>");
		int ctg=0;
		const int typemax=4;
		const int type[typemax]={
			SKILL_AUTO,
			SKILL_FIELD,
			SKILL_STATUS,
			SKILL_AUTO
		};
		const char* typen[typemax]={
			"�I�[�g�X�L��",
			"�t�B�[���h�X�L��",
			"�p�����[�^�X�L��",
			"�G��p�I�[�g�X�L��"
		};

		const int astypemax=15;
		const int astype[astypemax]={
			AS_LP_UP,
			AS_FIRE_GUARD,
			AS_STATUS_GUARD,
			AS_CRT_UP,
			AS_AVOID_UP,
			AS_MONEY_UP,
			AS_DEF_BREAK,
			AS_WEAPON_MASTER,
			AS_DMG_BOOST,
			AS_PET_SKILL,
			AS_FREE_SPACE,
			AS_HP_HEAL_UP,
			AS_ACT_ORDER,

			AS_OPTION,
			0xfffffff
		};
		const char* astypen[astypemax]={
			"�p�����[�^�ύX�n",
			"�����n",
			"ST�ُ�n",
			"�N���e�B�J���n",
			"����n",
			"�l���A�b�v�n",
			"�h�䖳���n",
			"�S���n",
			"�_���[�W�u�[�X�g�n",
			"�y�b�g�n",
			"�t���[�X�y�[�X�n",
			"���ʃA�b�v�n",
			"�s�����n",

			"���̑�",
			""
		};

		fprintf(pFile,"<ul>");
		for(int c=0;c<typemax;c++){
			fprintf(pFile,"<li><a href='#as_ctg_%d'>%s</a></li>",c,typen[c]);
			if(c==0){
				fprintf(pFile,"<ul>");
				for(int ac=0;ac<astypemax-1;ac++){
					fprintf(pFile,"<li><a href='#as_subctg_%d'>%s</a></li>",ac,astypen[ac]);
				}
				fprintf(pFile,"</ul>");
			}
		}
		fprintf(pFile,"</ul><hr class='bold'>\n");

		int slist[MAX_SKILL_NUMBER];
		for(int i=0;i<MAX_SKILL_NUMBER;i++){
			slist[i]=i;
		}
		bool swap=true;
		while(swap){
			swap=false;
			for(int i=1;i<MAX_SKILL_NUMBER;i++){
				if(SkillData[slist[i]].mParam<SkillData[slist[i-1]].mParam){
					int s=slist[i];
					slist[i]=slist[i-1];
					slist[i-1]=s;
					swap=true;
				}
			}
		}

		for(int c=0;c<typemax;c++){

			fprintf(pFile,"<a name='as_ctg_%d'></a><h3>%s</h3>\n",c,typen[c]);
			int oldparam=0;
			for(int j=0;j<MAX_SKILL_NUMBER;j++){
				int i=slist[j];
				 
				int file_top= ftell(pFile);
				int len=strlen(SkillData[i].GetName());
				if(len<6)	continue;
				const CSkillData &data=SkillData[i];
				if((data.mType)!=type[c])	continue;

				if(c==0){
					int j;
					for(j=0;j<astypemax;j++){if(oldparam>=astype[j])	continue;if(oldparam<astype[j])	break;}
					int oldctg=j-1;
					int p=SkillData[i].mParam&0xffff0000;
					for(j=0;j<astypemax;j++){if(p>=astype[j])	continue;if(p<astype[j])	break;}
					int ctg=j-1;

					if(oldparam!=p && p<=AS_FREE_SPACE) fprintf(pFile,"<br>");
					if(ctg!=oldctg)	fprintf(pFile,"<a name='as_subctg_%d'></a> <h4>%s</h4>\n",ctg,astypen[ctg]);
				}
				if(c==2){
					if(oldparam!=(SkillData[i].mParam&0xffff0000)) fprintf(pFile,"<br>");
				}

				fprintf(pFile,"<a name='as_%d'></a>",i);
				fprintf(pFile,"<table class='cs'><tr><td  colspan='4' class='sname'>%s</td></tr>",data.GetName());
				
				fprintf(pFile,"<tr><th class='csinfo'>����</th><td colspan='3'>");
				
				if(c!=2){
					char temp[1024];
					char *text;
					int index=0;
					sprintf(temp,"data\\help\\skill%02d",i/32);
					text=CMenuData::GetHelpText(temp,i);

					bool loop=true;
					int line=0;
					bool end=false;
					while(loop){
						int n=0;
						bool read=true;
						bool write=true;
						while(read){
							switch(text[n+index]){
								case '\0':
									if(line<=1 && n<=1)	end=true;
									loop=false;
									read=false;
									temp[n]='\0';
									break;
								case '\n':
									if(n<=1 && line>0){
										loop=false;
										write=false;
									}
									read=false;
									temp[n-1]='\0';
									break;
								default:	temp[n]=text[n+index];
							}
							n++;
						}
						index+=n;
						if(line!=0 && write)	fprintf(pFile,"%s<br>",temp);
						line++;
					}
					if(end){
						fseek(pFile,file_top,SEEK_SET);
						continue;
					}

					delete[] text;
					bool learn=false;
					bool learn_c=false;
					for(int chara=0;chara<MAX_CHARACTER_NUMBER;chara++){
						for(int month=0;month<MAX_MONTH;month++){
							for(int skill=0;skill<MAX_SKILL_MONTH;skill++){
								if(CharacterSkill[chara][month][skill]==i){
									LEARN ;
									if(!learn_c){
										learn_c=true;
										fprintf(pFile,"<tr><td><span class='subtext'>�E�����o�[�@</span></td><td>");

									}
									fprintf(pFile,"<a href='player.html#pl_%d'>%s</a>�E%d��%s<br>",chara,CharacterBaseData[chara].GetName(),month+1,((skill==MAX_SKILL_MONTH-1)?"(��)":""));
								}
							}
						}
					}
					if(learn_c)		fprintf(pFile,"</td></tr>");
					bool learn_e=false;
					for(int eq=0;eq<MAX_EQUIPMENT_NUMBER;eq++){
						const CEquipmentData &eqd=EquipmentData[eq];
						for(int skill=0;skill<eqd.mSkillNo;skill++){
							if(eqd.mSkill[skill]==i){
								LEARN ;
								if(!learn_e){
									learn_e=true;
									fprintf(pFile,"<tr><td><span class='subtext'>�E�����i�@</span></td><td>");

								}
								fprintf(pFile,"�y%s�z�@<a href='eq.html#eq_%d'>%s</a><br>",eqd.GetTypeName(),eq,eqd.GetName());
							}
						}
					}
					if(learn_e)		fprintf(pFile,"</td></tr>");


					if(learn)		fprintf(pFile,"</table>");
					if(c<3){
						if(!learn){
							fseek(pFile,file_top,SEEK_SET);	
							continue;
						}
					}else{
						if(learn){
							fseek(pFile,file_top,SEEK_SET);
							continue;
						}
					}
				}else{
					int p=SkillData[i].mParam;
					const char* paramname[9]={
						"LP�̊�l",
						"SP�̊�l",
						"�U����",
						"�h���",
						"���@�U��",
						"���@�h��",
						"���΂₳",
						"���Օi�̍ő及����",
						"�A�N�Z�T���̍ő呕����"
					};
					for(int i=0;i<9;i++){
						if((p&0xffff0000)==(0x00010000 << i)){
							fprintf(pFile,paramname[i]);
							break;
						}
					}
					fprintf(pFile,"�� %d �㏸",p&0xffff);
				}
				oldparam=(SkillData[i].mParam&0xffff0000);
				fprintf(pFile,"</td></tr></table>\n");
			}
		}
		int file_c= ftell(pFile);
		fseek(pFile,0,SEEK_END);
		int file_end= ftell(pFile);
		fseek(pFile,file_c,SEEK_SET);
		for(int i=file_c;i<file_end;i++)	putc(' ',pFile);

		fprintf(pFile,"\n</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_EQUIP_
	pFile=fopen("_�U�����\\eq.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>�����i�@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>�����i�@�ꗗ</h2>");

		fprintf(pFile,"<ul>");
		for(int c=1;c<CATEGORY_MAX-2;c++){
			fprintf(pFile,"<li><a href='#eq_ctg_%d'>%s</a></li>",c,ItemCategory[c].GetName());
			if(c==5 || c==6)	fprintf(pFile,"<hr class='short'>");
		}
		fprintf(pFile,"</ul><hr class='bold'>\n");

		int ctg=0;
		for(int i=0;i<MAX_EQUIPMENT_NUMBER;i++){
			if(ctg+1<CATEGORY_MAX && ItemCategory[ctg+1].mMin<=i){
				ctg++;
				fprintf(pFile,"<a name='eq_ctg_%d'></a><h3>%s</h3>\n",ctg,ItemCategory[ctg].GetName());
			}
			if(strcmp(AllItem[i]->GetName(),"")){
				fprintf(pFile,"<a name='eq_%d'></a>",i);
				fprintf(pFile,"<table class='cs'><tr><td colspan='3'  class='sname'>%s</td>",AllItem[i]->GetName());
				fprintf(pFile,"<th>�艿</th><td>%d</td></tr>",AllItem[i]->GetPrice());
				if(i<MAX_EQUIPMENT_NUMBER){
					fprintf(pFile,"<tr><th  class='csinfo' rowspan='3'>���\</th>");
					const CEquipmentData &eq=EquipmentData[i];
					OutputDBParam(pFile,eq.mLP,"�ő�LP");
					OutputDBParam(pFile,eq.mAtk,"�U����");
					fprintf(pFile,"</tr><tr>");
					OutputDBParam(pFile,eq.mDef,"�h���");
					OutputDBParam(pFile,eq.mMagAtk,"���@�U��");
					fprintf(pFile,"</tr><tr>");
					OutputDBParam(pFile,eq.mMagDef,"���@�h��");
					OutputDBParam(pFile,eq.mSpeed,"���΂₳");

					int sn=(eq.mSkillNo+1)/2;
					if(sn<=0)	sn=1;
					fprintf(pFile,"</tr><tr><th rowspan='%d' class='csinfo'>�X�L��</th>",sn);

					if(eq.mSkillNo){
						for(int i=0;i<eq.mSkillNo;i++){
							if(i%2==0 && i>0)	fprintf(pFile,"</tr><tr>");
							fprintf(pFile,"<td colspan='2' class='skill_i");
							OutputDBSkill(pFile,eq.mSkill[i]);
						}
						if(eq.mSkillNo%2==1){
							fprintf(pFile,"<td  colspan='2' class='skill_i_none'> </td>");
						}
					}else{
						fprintf(pFile,"<td class='skill_i' colspan='2'><span class='subtext'>����</span></td><td colspan='2' class='skill_i_none'> </td>");
					}

					fprintf(pFile,"</tr><tr><th rowspan='2'>����</th>");
					for(int hard=0;hard<2;hard++){
						switch(hard){
							case 0:
								fprintf(pFile,"<th>Normal");	break;
							case 1:
								fprintf(pFile,"<th>Hard");	break;
						}
						fprintf(pFile,"</th><td colspan='3'>");


						for(int n=0;n<_OUTPUT_TRESURE_MAX_MONTH_;n++){
							if(NormalHard[hard][n]<0)	break;
							const CMonthData& mdata=MonthData[NormalHard[hard][n]];
							for(int j=0;j<mdata.mTresureNo;j++){
								const CTreasureData &t=TreasureData[mdata.mTresure[j]];
								for(int k=0;k<t.mItemNo;k++){
									if(t.mItem[k]==i){
										fprintf(pFile,"�� %d��<br>",n+1);
									}
								}
							}
						}

						for(int n=0;n<shopno[hard]-1;n++){
							for(int j=0;j<_OUTPUT_TRESURE_MAX_MONTH_;j++){
								for(int k=0;k<MAX_SHOP_ITEM;k++){
									const int &it = (ShopData[shopid[hard][n+1]-1][j][k]);
									if(it<0)	break;
									if(it==i){
										if(shopid[hard][n+1]>10){
											fprintf(pFile,"%s<br>",shopname[shopid[hard][n+1]]);
										}else{
											fprintf(pFile,"%s %d��<br>",shopname[shopid[hard][n+1]],j+1);
										}
									}
								}
							}
						}
						const int member[2][MAX_CHARACTER_NUMBER]={
							{0,1,2,3,4,5,7,8,10,-1},
							{0,1,2,3,4,5,6,7,8,9,10,11,12,-1}
						};
						for(int n=0;n<MAX_CHARACTER_NUMBER;n++){
							const int &mem=member[hard][n];
							if(mem<0)	break;
							for(int j=0;j<MAX_ITEM;j++){
								const int &it=StartItem[mem][j];
								if(it<0)	break;
								if(it==i){
									fprintf(pFile,"���������@<a href='player.html#pl_%d'>%s</a><br>",mem,CharacterBaseData[mem].GetName());
								}
							}
						}


						fprintf(pFile,"</td></tr>");
					}
					fprintf(pFile,"</table>\n");
				}
			}
		}
		fprintf(pFile,"\n</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_UI_
		pFile=fopen("_�U�����\\ui.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>���Օi�@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>���Օi�@�ꗗ</h2>");

		for(int i=ID_UI;i<ID_NSUI;i++){
			if(strcmp(AllItem[i]->GetName(),"")){
				fprintf(pFile,"<a name='ui_%d'></a>",i);
				fprintf(pFile,"<table class='cs'><tr><td colspan='3'  class='sname'>%s</td>",AllItem[i]->GetName());
				fprintf(pFile,"</tr><tr><th  class='csinfo'>����</th><td colspan='2'>");
				char temp[256],*text;
				int index=0;
				sprintf(temp,"data\\help\\item08");
				text=CMenuData::GetHelpText(temp,i);

				bool loop=true;
				int line=0;
				while(loop){
					int n=0;
					bool read=true;
					while(read){
						switch(text[n+index]){
							case '\0':
								loop=false;
								read=false;
								temp[n]='\0';
								break;
							case '\n':
								read=false;
								temp[n-1]='\0';
								break;
							default:	temp[n]=text[n+index];
						}
						n++;
					}
					index+=n;

					bool write=false;
					line++;
					write=(line==2 || line==3);
					while(temp[0]==char(0x81) && temp[1]==char(0x40)){
						write=true;
						strcpy(temp,temp+2);
					}
					if(write){
						if(temp[0]==char(0x84) && (temp[1]==char(0xa4) || temp[1]==char(0xa5))){
							fprintf(pFile,"�@[ %s ]<br>",temp+2);
						}else{
							fprintf(pFile,"%s<br>",temp);
						}
						if(line==3)	fprintf(pFile,"<br>");
					}
				}
				fprintf(pFile,"</td></tr><tr><th  class='csinfo' rowspan='2'>����</th>");
				for(int hard=0;hard<2;hard++){
					switch(hard){
						case 0:
							fprintf(pFile,"<th class='csinfo' >Normal");	break;
						case 1:
							fprintf(pFile,"<th class='csinfo' >Hard");	break;
					}
					fprintf(pFile,"</th><td>");

					int item=ID_SD-ID_UI+i;
					int total=0;

					fprintf(pFile,"<table><tr><td>");
					if(i==ID_UI){
						fprintf(pFile,"�������Y�� - 30 ��<br>");
						total=30;
					}
					for(int n=0;n<_OUTPUT_TRESURE_MAX_MONTH_;n++){
						if(NormalHard[hard][n]<0)	break;
						const CMonthData& mdata=MonthData[NormalHard[hard][n]];
						int no=0;
						for(int j=0;j<mdata.mTresureNo;j++){
							const CTreasureData &t=TreasureData[mdata.mTresure[j]];
							for(int k=0;k<t.mItemNo;k++){
								if(t.mItem[k]==item){
									no++;
								}
							}
						}
						if(no>0){
							fprintf(pFile,"%d�� - %d ��<br>",n+1,no);
							total+=no;
						}
					}
					fprintf(pFile,"</td><td class='total'>�@���v - %d ��</td></tr></table>",total);

					fprintf(pFile,"</td></tr>");
				}
				fprintf(pFile,"</table>\n");
			}
		}
		fprintf(pFile,"\n</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_PLAYER_
	pFile=fopen("_�U�����\\player.html","w");
	if(pFile){
		const int CharacterSort[MAX_CHARACTER_NUMBER]=
		{0,1,2,3,10,7,8,4,5,11,6,9,12,13,14};
		fprintf(pFile,"<html><head><title>�L�����N�^�[�@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>�L�����N�^�[�@�ꗗ</h2>");
		
		fprintf(pFile,"<ul>");
		for(int aaaa=0;aaaa<MAX_CHARACTER_NUMBER-2;aaaa++){
			int i=CharacterSort[aaaa];
			fprintf(pFile,"<li><a href='#pl_%d'>%s</a></li>",i,CharacterBaseData[i].GetName());
		}
		fprintf(pFile,"</ul>\n");

		for(int aaaa=0;aaaa<MAX_CHARACTER_NUMBER;aaaa++){
			int i=CharacterSort[aaaa];
			if(strcmp("",CharacterBaseData[i].GetName())==0)	continue;
			fprintf(pFile,"\n<hr class='bold'><a name='pl_%d'></a><h3>%s</h3>\n",i,CharacterBaseData[i].GetName());

			fprintf(pFile,"<h4>�\�͒l</h4>");
			fprintf(pFile,"<table class='ch'><tr><th></th><th>Lv</th><th>LP</th><th>SP</th><th>�U��</th><th>�h��</th><th>���U</th><th>���h</th><th>����</th><th>����</th><th>����</th></tr>");
			int param[9];
			CPlayerData &pdata=Flag.mPlayerData[i];

			param[0]=pdata.GetBaseStm();
			param[1]=pdata.GetBaseTech();
			param[2]=pdata.GetBaseAtk();
			param[3]=pdata.GetBaseDef();
			param[4]=pdata.GetBaseMagAtk();
			param[5]=pdata.GetBaseMagDef();
			param[6]=pdata.GetBaseSpeed();
			param[7]=pdata.GetUseItemMax();
			param[8]=pdata.GetAccessoryMax();

			fprintf(pFile,"<tr><th>�����l</th>");
			OutputDBCharaParam(pFile,pdata.GetLv(),param);
			fprintf(pFile,"</tr>");

			param[0]=CharacterBaseData[i].mLP;
			param[1]=CharacterBaseData[i].mTech;
			param[2]=CharacterBaseData[i].mAtk;
			param[3]=CharacterBaseData[i].mDef;
			param[4]=CharacterBaseData[i].mMagAtk;
			param[5]=CharacterBaseData[i].mMagDef;
			param[6]=CharacterBaseData[i].mSpeed;
			param[7]=MIN_USE_ITEM;
			param[8]=MIN_ACCESSORY;

			for(int j= -1;j<MAX_MONTH-1;j++){
				if(j>=0){
					for(int k=0;k<MAX_SKILL_MONTH;k++){
						int skill=CharacterSkill[i][j][k];
						if(SkillData[skill].mType==SKILL_STATUS){
							const int &p=SkillData[skill].mParam;
							int	up=p&0xffff;
							for(int l=0;l<9;l++){
								if((p>>(16+l))&0x01){
									param[l]+=up ;
								}
							}
						}
					}
				}
			}
			fprintf(pFile,"<tr><th>���E</th>");

			OutputDBCharaParam(pFile,60,param);
			fprintf(pFile,"</tr>");
			fprintf(pFile,"</table>\n");

			
			fprintf(pFile,"<h4>��������</h4>");
			fprintf(pFile,"<table class='fe'><tr><th rowspan='2'>����</th>");
			OutputDBFirstEquip(pFile,i,2,0);
			
			fprintf(pFile,"<tr><td colspan='2'></td></tr><tr><th rowspan='1'>�h��</th>");
			OutputDBFirstEquip(pFile,i,1,3);
			
			fprintf(pFile,"<tr><td colspan='2'></td></tr><tr><th rowspan='2'>����</th>");
			OutputDBFirstEquip(pFile,i,2,4);

			fprintf(pFile,"</table>");

			fprintf(pFile,"<h4>�K���X�L��</h4><table class='ch'>");
			for(int j=0;j<MAX_MONTH-1;j++){
				fprintf(pFile,"<tr><td colspan='5'></td></tr><tr><th>%d��</th>",j+1);
				for(int k=0;k<MAX_SKILL_MONTH;k++){
					fprintf(pFile,"<td class='skill_c");
					int skill=CharacterSkill[i][j][k];
					OutputDBSkill(pFile,skill);
					if(k==2){
						fprintf(pFile,"<th>��</th></tr><tr>");
					}
				}
				fprintf(pFile,"</tr>");
			}
			fprintf(pFile,"</table>");
		}
		fprintf(pFile,"\n</body></html>");
		fclose(pFile);
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_MONTH_
	for(int month=0;month<MAX_ALL_MONTH;month++){
		const CMonthData &mdata=MonthData[month];
		char fname[256];
		sprintf(fname,"_�U�����\\month%02d.html",month);
		pFile=fopen(fname,"w");
		if(pFile){
			char monthname[256];
			sprintf(monthname,"%d��",mdata.mMonth);
			switch(MonthNormalHard[month]){
				case 1:
					strcat(monthname,"(Normal)�@");	break;
				case 2:
					strcat(monthname,"(Hard)�@");	break;
				case 3:
					strcat(monthname,"(����)�@");	break;
			}
			strcat(monthname,mdata.GetName());

			fprintf(pFile,"<html><head><title>%s</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>%s</h2>",monthname,monthname);

			fprintf(pFile,"<ul>");
			fprintf(pFile,"<li><a href='#map'>�S�̃}�b�v</a></li>");
			fprintf(pFile,"<li><a href='#tresure'>��</a></li>");
			fprintf(pFile,"<li><a href='#enemy'>�o������G</a></li>");
			fprintf(pFile,"<li><a href='#kouryaku'>�ڍ׍U��</a></li>");
			fprintf(pFile,"</ul><hr>\n");
			fprintf(pFile,"<a name='map'></a><h3>�S�̃}�b�v</h3>\n");
			fprintf(pFile,"<a name='tresure'></a><h3>��</h3>");
			fprintf(pFile,"<table  class='ch'><tr><th>no</th><th>���g</th><th>㩁E�J�M</th></tr>\n");
			for(int box=0;box<mdata.mTresureNo;box++){
				const CTreasureData &boxdata=TreasureData[mdata.mTresure[box]];
				fprintf(pFile,"<tr><th>%d</th><td>",box+1);
				for(int item=0;item<boxdata.mItemNo;item++){
					if(!item==0)	fprintf(pFile,"<br>");
					int no=boxdata.mItem[item];
					if(no<ID_UI){
						fprintf(pFile,"<a href='eq.html#eq_%d'>%s</a>",no,AllItem[no]->GetName());
					}else if(ID_SD<=no && no<ID_SD+MAX_SEED_NUMBER){
						int uino=no-ID_SD+ID_UI;
						fprintf(pFile,"<a href='ui.html#ui_%d'>�^�l�F%s</a>",uino,AllItem[uino]->GetName());
					}else{
						fprintf(pFile,AllItem[no]->GetName());
					}
				}
				fprintf(pFile,"</td><td>");
				if(boxdata.mKey){
					for(int k=0;k<8;k++){
						int mask=1<<k;
						if(boxdata.mKey & mask){
							fprintf(pFile,"�v�F%s",AllItem[k+ID_KEY]->GetName());
						}
					}
				}
				if(boxdata.mTrap>=0){
					if(boxdata.mKey)	fprintf(pFile,"<br>");
					fprintf(pFile,"%s",TrapData[boxdata.mTrap]);
					fprintf(pFile,"�@(��x�F%d)",boxdata.mTrapLv);
					
				}
				fprintf(pFile,"</td></tr>\n");
			}
			fprintf(pFile,"</table>\n");

			int baselv=mdata.mBaseLv[0];
			if(MonthNormalHard[month]==2)	baselv=mdata.mBaseLv[1];
			bool used[ENEMY_NO];
			memset(used,0,sizeof(used));
			
			fprintf(pFile,"<a name='enemy'></a><h3>�o������G</h3>\n");
			fprintf(pFile,"<table  class='ch'><tr><th></th><th>Lv</th></tr>\n");
			fprintf(pFile,"<tr><th>�U�R</th><td><hr></td></tr>\n");

			for(int en=0;en<mdata.mEnemyNo;en++){
				int enemy=mdata.mEnemy[en];
				used[enemy]=true;

				int i=0;
				while(MonthEnemyData[month][i][0]>=0){
					if(MonthEnemyData[month][i][0]==enemy){
						fprintf(pFile,"<tr><td><a href='enemy.html#ene_%d'>%s</a></td><td>",enemy,EnemyBaseData[enemy].mName);
						int j=1;
						while(MonthEnemyData[month][i][j]>-100){
							if(j!=1)	fprintf(pFile," ,  ");
							int lv=MonthEnemyData[month][i][j]+baselv;
							if(lv<0)	lv=0;
							if(lv>60)	lv=60;
							fprintf(pFile,"%d",lv);
							j++;
						}
						fprintf(pFile,"</td></tr>\n");
						break;
					}
					i++;
				}
			}
			
			fprintf(pFile,"<tr><th>BOSS</th><td><hr></td></tr>\n");

			int i=0;
			while(MonthEnemyData[month][i][0]>=0){
				const int &enemy=MonthEnemyData[month][i][0];
				if(used[enemy]){
					i++;
					continue;
				}
				fprintf(pFile,"<tr><td><a href='enemy.html#ene_%d'>%s</a></td><td>",enemy,EnemyBaseData[enemy].mName);
				int j=1;
				while(MonthEnemyData[month][i][j]>-100){
					if(j!=1)	fprintf(pFile," ,  ");
					int lv=MonthEnemyData[month][i][j]+baselv;
					if(lv<0)	lv=0;
					if(lv>60)	lv=60;
					fprintf(pFile,"%d",lv);
					j++;
				}
				i++;
				fprintf(pFile,"</td></tr>\n");
			}

			if(MonthNormalHard[month]==3){
				fprintf(pFile,"<tr><th>�n�[�h���[�h</th><td>Lv +%d</td></tr>\n",mdata.mBaseLv[1]-mdata.mBaseLv[0]);
			}
			
			fprintf(pFile,"</table>\n<a name='kouryaku'></a><h3>�ڍ׍U��</h3>\n");
			fprintf(pFile,"</body></html>");
			fclose(pFile);
		}
	}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef _OUTPUT_DB_ENEMY_
	pFile=fopen("_�U�����\\enemy.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>�G�@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body><h2>�G�@�ꗗ</h2>\n");

		int enemy_list[ENEMY_NO][60];
		memset(enemy_list,-100,sizeof(enemy_list));
		for(int m=0;m<MAX_ALL_MONTH;m++){
			int month=m-1;
			if(m==0)	month=0;
			if(m==1)	month=MAX_ALL_MONTH-1;
			const CMonthData &mdata=MonthData[month];
			int baselv=mdata.mBaseLv[0];
			if(MonthNormalHard[month]==2)	baselv=mdata.mBaseLv[1];
			bool used[ENEMY_NO];
			memset(used,0,sizeof(used));

			for(int en=0;en<mdata.mEnemyNo;en++){
				int enemy=mdata.mEnemy[en];
				used[enemy]=true;

				int i=0;
				while(MonthEnemyData[month][i][0]>=0){
					if(MonthEnemyData[month][i][0]==enemy){
						int j=1;
						while(MonthEnemyData[month][i][j]>-100){
							AddEnemyDB(enemy_list,enemy,MonthEnemyData[month][i][j]+baselv);
							if(MonthNormalHard[month]==3)	AddEnemyDB(enemy_list,enemy,MonthEnemyData[month][i][j]+mdata.mBaseLv[1]);
							j++;
						}
					}
					i++;
				}
			}
			int i=0;
			while(MonthEnemyData[month][i][0]>=0){
				const int &enemy=MonthEnemyData[month][i][0];
				if(used[enemy]){
					i++;
					continue;
				}
				int j=1;
				while(MonthEnemyData[month][i][j]>-100){
					AddEnemyDB(enemy_list,enemy,MonthEnemyData[month][i][j]+baselv);
					if(MonthNormalHard[month]==3)	AddEnemyDB(enemy_list,enemy,MonthEnemyData[month][i][j]+mdata.mBaseLv[1]);
					j++;
				}
				i++;
			}
		}
		
		
		for(int i=0;i<ENEMY_NO;i++){
			int skill_list[MAX_CSKILL+MAX_ASKILL][60];
			memset(skill_list,-100,sizeof(skill_list));

			int enemy=enemy_list[i][0];
			if(enemy<0)	break;
			fprintf(pFile,"<a name='ene_%d'></a>",enemy);
			fprintf(pFile,"<table class='en'><tr><td class='ename' colspan='8' >%s</td></tr>",EnemyBaseData[enemy].mName);

			fprintf(pFile,"<tr><th class='ep'>EXP</th><td class='ep'>%d</td><th class='ep'>����</th><td class='ep'>%d</td><th class='ep'>�T�C�Y</th><td class='ep'>",EnemyBaseData[enemy].mExp,EnemyBaseData[enemy].mMoney);
			switch(EnemyBaseData[enemy].mSize){
				case SMALL:
					fprintf(pFile,"��");	break;
				case LARGE:
					fprintf(pFile,"��");	break;
			}
			fprintf(pFile,"</td><th class='tgt'>�D��^�[�Q�b�g</th><td class='tgt'>");

			switch(EnemyBaseData[enemy].mThinkType &0xff0000){
				case THINK_TGT_RANDOM:
					fprintf(pFile,"�Ȃ�");	break;
				case THINK_TGT_MOST_DAMAGE:
					fprintf(pFile,"�_���[�W�𑽂��󂯂Ă���");	break;
				case THINK_TGT_MOST_HP:
					fprintf(pFile,"�g�o������");	break;
				case THINK_TGT_LOW_HP:
					fprintf(pFile,"�g�o���Ⴂ");	break;
				case THINK_TGT_HI_LEVEL:
					fprintf(pFile,"���x��������");	break;
				case THINK_TGT_MULTI_HIT:
					fprintf(pFile,"�͈͍U���ɂ�葽���������߂�");	break;
				case THINK_TGT_MOST_LP:
					fprintf(pFile,"�k�o������");	break;
				case THINK_TGT_FEW_ST:
					fprintf(pFile,"�X�e�[�^�X�ُ�̐������Ȃ�");	break;
				case THINK_TGT_MOST_ST:
					fprintf(pFile,"�X�e�[�^�X�ُ�̐�������");	break;
				case THINK_TGT_HI_ATK:
					fprintf(pFile,"�U���͂�����");	break;
				case THINK_TGT_LOW_FEFFECT:
					fprintf(pFile,"�������n��łȂ�");	break;
				case THINK_TGT_LARGE_SIZE:
					fprintf(pFile,"��^�T�C�Y");	break;
				case THINK_TGT_HI_DEF:
					fprintf(pFile,"�h��͂�����");	break;
				case THINK_TGT_LEFT:
					fprintf(pFile,"��荶�̂ق��ɗ����Ă���");	break;
				case THINK_TGT_SELF:
					fprintf(pFile,"�������g");	break;
			}
			if((EnemyBaseData[enemy].mThinkType &0xff0000)!=THINK_TGT_RANDOM){
				int intel=EnemyBaseData[enemy].mInt;
				if(EnemyBaseData[enemy].mThinkType & THINK_TOPT_HALF_INT)		intel/=2;
				if(EnemyBaseData[enemy].mThinkType & THINK_TOPT_QUARTER_INT)	intel/=4;
				if(intel>100)	intel=100;
				if(intel<0)		intel=0;
				fprintf(pFile,"�@(%d��)",intel);
			}
			fprintf(pFile,"</td><tr><td colspan='8' >");
			fprintf(pFile,"<b>�o��</b>");
			fprintf(pFile,"<table class='ch'><tr><th colspan='2' rowspan='2'></th><th colspan='2'>Lv</th></tr><tr><th class='diff'>Normal</th><th class='diff'>Hard</th></tr>");
			
			for(int m=0;m<MAX_ALL_MONTH;m++){
				int month=m-1;
				if(m==0)	month=0;
				if(m==1)	month=MAX_ALL_MONTH-1;

				int i=0;
				while(MonthEnemyData[month][i][0]>=0){
					if(enemy==MonthEnemyData[month][i][0]){
						fprintf(pFile,"<tr><td>%d��",MonthData[month].mMonth);
						switch(MonthNormalHard[month]){
							case 1:
								fprintf(pFile,"(Normal)");	break;
							case 2:
								fprintf(pFile,"(Hard)");	break;
							case 3:
								fprintf(pFile,"(����)");	break;
						}
						fprintf(pFile,"</td><td>%s</td>",MonthData[month].GetName());
						for(int d=0;d<2;d++){
							if(MonthNormalHard[month] & (1<<d)){
								fprintf(pFile,"<td class='center'>");
								const int &base=MonthData[month].mBaseLv[d];
								int j=1;
								while(MonthEnemyData[month][i][j]>-100){
									if(j!=1)	fprintf(pFile," , ");
									int lv=MonthEnemyData[month][i][j]+base;
									if(lv<0)	lv=0;
									if(lv>60)	lv=60;
									fprintf(pFile,"%d",lv);
									j++;
								}
								fprintf(pFile,"</td>");
							}else{
								fprintf(pFile,"<td class='none'>---</td>");
							}
						}
						
						fprintf(pFile,"</tr>");
					}
					i++;
				}
			}
			fprintf(pFile,"</table><br>");

			fprintf(pFile,"<b>�p�����[�^</b>");
			fprintf(pFile,"<table class='ch'><tr><th class='param'>Lv</th><th>LP</th><th>SP</th><th>�U��</th><th>�h��</th><th>���U</th><th>���h</th><th>����</th></tr>");
			for(int j=1;j<60;j++){
				int lv=enemy_list[i][j];
				if(lv<0)	break;
				CEnemyData edata;
				edata.LoadEnemyData(enemy,lv);
				fprintf(pFile,"<th class='param'>%d</th>",lv);
				fprintf(pFile,"<td class='param'>%d</td>",edata.GetLP());
				fprintf(pFile,"<td class='sp'>�@");
				for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
					fprintf(pFile,"%d/",edata.GetMaxSP(k));
				fprintf(pFile,"</td>");
				fprintf(pFile,"<td class='param'>%d</td>",edata.GetAtk());
				fprintf(pFile,"<td class='param'>%d</td>",edata.GetDef());
				fprintf(pFile,"<td class='param'>%d</td>",edata.GetMagAtk());
				fprintf(pFile,"<td class='param'>%d</td>",edata.GetMagDef());
				fprintf(pFile,"<td class='param'>%d</td></tr>",edata.GetSpeed());

				//�X�L�����X�g�擾
				for(int cs=0;cs<MAX_CSKILL;cs++){
					int s=edata.mCSkill[cs];
					if(s<0)	continue;
					for(int k=0;k<MAX_CSKILL+MAX_ASKILL;k++){
						if(skill_list[k][0]<0)	skill_list[k][0]=s;
						if(skill_list[k][0]==s){
							skill_list[k][j]=1;
							break;
						}
					}
				}
				for(int as=0;as<MAX_ASKILL;as++){
					int s=edata.mASkill[as]+ID_AS;
					if(s<ID_AS)	continue;
					for(int k=0;k<MAX_CSKILL+MAX_ASKILL;k++){
						if(skill_list[k][0]<0)	skill_list[k][0]=s;
						if(skill_list[k][0]==s){
							skill_list[k][j]=1;
							break;
						}
					}
				}
			}
			fprintf(pFile,"</table><br>");
			fprintf(pFile,"<b>�X�L��</b>");
			fprintf(pFile,"<table class='ch'><tr><th>Lv</th>");
			int j;
			for(j=1;j<60;j++){
				int lv=enemy_list[i][j];
				if(lv<0)	break;
				fprintf(pFile,"<th>%d</th>",lv);
			}
			fprintf(pFile,"</tr><tr><td colspan='%d'></td></tr>",j);
			for(int s=0;s<MAX_CSKILL+MAX_ASKILL;s++){
				int skill=skill_list[s][0];
				if(skill<0)	break;
				fprintf(pFile,"<tr>");
				if(skill<ID_AS){
					fprintf(pFile,"<td class='skill_c_c'><a href='cskill.html#cs_%d'>%s</a></td>",skill,ComSkillData[skill].GetName());
				}else{
					fprintf(pFile,"<td class='skill_c_a'><a href='askill.html#as_%d'>%s</a></td>",skill-ID_AS,SkillData[skill-ID_AS].GetName());
				}
				for(j=1;j<60;j++){
					int lv=enemy_list[i][j];
					if(lv<0)	break;
					if(skill_list[s][j]>0){
						fprintf(pFile,"<td class='learn'>��</td>");
					}else{
						fprintf(pFile,"<td>-</td>");
					}
				}

				
				fprintf(pFile,"</tr>");
			}
			fprintf(pFile,"</table>");

			fprintf(pFile,"</td></tr></table>\n");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_RANKING_
	pFile=fopen("_�U�����\\rank.html","w");
	if(pFile){
		int final_param[9][MAX_CHARACTER_NUMBER];
		memset(final_param,0,sizeof(final_param));
		int ctg=0;
		for(int i=0;i<MAX_CHARACTER_NUMBER;i++){
			if(strcmp("",CharacterBaseData[i].GetName())==0)	continue;

			int param[9];
			param[0]=CharacterBaseData[i].mLP;
			param[1]=CharacterBaseData[i].mTech;
			param[2]=CharacterBaseData[i].mAtk;
			param[3]=CharacterBaseData[i].mDef;
			param[4]=CharacterBaseData[i].mMagAtk;
			param[5]=CharacterBaseData[i].mMagDef;
			param[6]=CharacterBaseData[i].mSpeed;
			param[7]=MIN_USE_ITEM;
			param[8]=MIN_ACCESSORY;

			for(int j= -1;j<MAX_MONTH-1;j++){
				if(j>=0){
					for(int k=0;k<MAX_SKILL_MONTH;k++){
						int skill=CharacterSkill[i][j][k];
						if(SkillData[skill].mType==SKILL_STATUS){
							const int &p=SkillData[skill].mParam;
							int	up=p&0xffff;
							for(int l=0;l<9;l++){
								if((p>>(16+l))&0x01){
									param[l]+=up ;
								}
							}
						}
					}
				}
			}
			for(int j=0;j<9;j++){
				final_param[j][i]=param[j];
			}
		}
		//�����L���O
		fprintf(pFile,"<html><head><title>���E�\�͒l�����L���O</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body>\n<h2>���E�\�͒l�����L���O</h2>\n");
		for(int i=0;i<9;i++){
			int param[2][MAX_CHARACTER_NUMBER];
			for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				param[0][j]=j;
			}
			memcpy(param[1],final_param[i],sizeof(int)*MAX_CHARACTER_NUMBER);

			bool change=true;
			while(change){
				change=false;
				for(int j=1;j<MAX_CHARACTER_NUMBER;j++){
					if(param[1][j]>param[1][j-1]){
						int no=param[0][j];
						int p=param[1][j];

						param[0][j]=param[0][j-1];
						param[1][j]=param[1][j-1];

						param[0][j-1]=no;
						param[1][j-1]=p;

						change=true;
					}
				}
			}

			fprintf(pFile,"\n<h4><a name='");
			switch(i){
				case 0:	fprintf(pFile,"lp'>LP");	break;
				case 1:	fprintf(pFile,"sp'>SP");	break;
				case 2:	fprintf(pFile,"at'>�U����");	break;
				case 3:	fprintf(pFile,"df'>�h���");	break;
				case 4:	fprintf(pFile,"ma'>���@�U��");	break;
				case 5:	fprintf(pFile,"md'>���@�h��");	break;
				case 6:	fprintf(pFile,"sd'>���΂₳");	break;
				case 7:	fprintf(pFile,"ui'>�A�C�e��");	break;
				case 8:	fprintf(pFile,"ac'>�A�N�Z�T��");	break;
			}
			fprintf(pFile,"</a></h4>\n");
			fprintf(pFile,"<table class='fe'><tr><th>����</th><th>���O</th><th>�p�����[�^</th></tr>\n");
			for(int j=0;j<MAX_CHARACTER_NUMBER;j++){
				if(strcmp("",CharacterBaseData[param[0][j]].GetName())==0)	continue;
				CPlayerData cdata;
				fprintf(pFile,"<tr><td>%d</td><td>%s</td><td>",j+1,CharacterBaseData[param[0][j]].GetName());
				switch(i){
					case 0:
						cdata.mStm=param[1][j];
						cdata.mLv=60;
						cdata.CalculateLP();
						fprintf(pFile,"%d",cdata.mLP);
						break;
						
					case 1:
						cdata.mTech=param[1][j];
						cdata.CalculateSP();
						for(int k=MAX_CSKILL_LEVEL-1;k>=0;k--)
							fprintf(pFile,"%d/",cdata.GetMaxSP(k));
						break;
					default:
						fprintf(pFile,"%d",param[1][j]);
						break;
				}
				fprintf(pFile,"</td></tr>\n");
			}
			fprintf(pFile,"</table>\n");
		}
		fprintf(pFile,"</body></html>");
		fclose(pFile);

	}
	
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_SHOP_
	pFile=fopen("_�U�����\\shop.html","w");
	if(pFile){
		fprintf(pFile,"<html><head><title>�X�@�ꗗ</title><link rel='stylesheet' href='style.css' type='text/css'></link></head><body>\n<h2>�X�@�ꗗ</h2>\n");
		fprintf(pFile,"<ul>");
		for(int i=0;i<MAX_SHOP_NO;i++){
			fprintf(pFile,"<li><a href='#shop_%d'>%s</a></li>\n",i,shopname[i+1]);
		}
		fprintf(pFile,"</ul>");
		for(int i=0;i<MAX_SHOP_NO;i++){
			fprintf(pFile,"\n<a name='shop_%d'></a><h4>%s</h4>",i,shopname[i+1]);

			fprintf(pFile,"<table class='ch'>");
			for(int month=0;month<MAX_MONTH-1;month++){
				int item;
				for(item=0;item<MAX_SHOP_ITEM;item++){
					int no=ShopData[i][month][item];

					if(no<0){
						break;
					}
					if(item==0){
						fprintf(pFile,"<tr><th>%d��</th><td>",month+1);
					}else{
						fprintf(pFile,"<br>");
					}
					if(no<ID_UI){
						fprintf(pFile,"<a href='eq.html#eq_%d'>%s</a>",no,AllItem[no]->GetName());
					}else if(ID_SD<=no && no<ID_SD+MAX_SEED_NUMBER){
						int uino=no-ID_SD+ID_UI;
						fprintf(pFile,"<a href='ui.html#ui_%d'>�^�l�F%s</a>",uino,AllItem[uino]->GetName());
					}else{
						fprintf(pFile,AllItem[no]->GetName());
					}
				}
				if(item!=0){
					fprintf(pFile,"</td></tr>");
				}
			}
			fprintf(pFile,"</table>");
		}

		fprintf(pFile,"</body></html>");
		fclose(pFile);
	}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _OUTPUT_DB_BS_
	pFile=fopen("_�U�����\\birthstone.html","w");
	if(pFile){
		fprintf(pFile,"<table>");
		for(int month=0;month<MAX_MONTH-1;month++){
			fprintf(pFile,"<tr><th>%d��</th>",month+1);
			static const int lv[MAX_LV]={
				0,	0,	1,	0,	1,	2,	0,	1,	2,	3,
				1,	2,	3,	4,	0,	2,	3,	4,	5,	1,	
				3,	4,	5,	6,	2,	7,	4,	5,	6,	3,	
				8,	7,	6,	5,	4,	9,	8,	7,	6,	10,
				9,	8,	5,	7,	11,	10,	9,	8,	6,	11,
				7,	10,	9,	11,	8,	10,	9,	11,	10,	11
			};
			for(int l=0;l<MAX_LV;l++){
				if(lv[l]==month){
					fprintf(pFile,"<td>%d</td>",l+1);
				}
			}
			
			fprintf(pFile,"</tr>\n",month+1);
		}
		fprintf(pFile,"</table>");

		fclose(pFile);
	}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _OUTPUT_EXIT_
	DTest.Exit();
#endif
}
#endif
