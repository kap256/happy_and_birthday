
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
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
	CBGMData(
		"�C�x���g�E����",
		"Lecture & Tutorial",1,		//�C�x���g�E�̂ǂ�
		6000,	//���[�v
		EVENT+0		//�ȏ�
	),
	CBGMData(
		"�����ȑ�",
		"Home Town",2,				//�X
		0,		//���[�v
		TOWN+0		//�ȏ�
	),
	CBGMData(
		"����",
		"����",3,					//����
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
//4
	CBGMData(
		"�_���W�����E���A",
		"Dungeon & Caverns",4,			//���A
		4001,	//���[�v
		FIELD+50	//�ȏ�
	),
	CBGMData(
		"�_���W�����E��ՂQ",
		"Ruins & Labylinth",5,		//���
		4365,	//���[�v
		FIELD+70	//�ȏ�
	),
	CBGMData(
		"�_���W�����E�X��",
		"Fairy & Forest",6,			//�d���̐X
		20571,	//���[�v
		FIELD+20	//�ȏ�
	),
	CBGMData(
		"�����̊X",
		"�����̒�",7,				//�����̒�
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
//8
	CBGMData(
		"�o�g���P",
		"Battle 1",8,				//�퓬
		8929,	//���[�v
		BATTLE+0	//�ȏ�
	),
	CBGMData(
		"�V�N��",
		"Happy & Newyear",	9,				//�V�N��
		10591,		//���[�v
		TOWN+10 	//�ȏ�
	),
	CBGMData(
		"",
		"",		10,					//�J�i�����j
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
	CBGMData(
		"�_���W�����E���O",
		"Field & Adventure",11,		//����
		1933,	//���[�v
		FIELD+0		//�ȏ�
	),
//12
	CBGMData(
		"�_���W�����E�C�m",
		"Mermaid & Whirls",12,		//�l��
		10270,	//���[�v
		FIELD+40	//�ȏ�
	),
	CBGMData(
		"�_���W�����E��ՂP",
		"Royal & Graves",	13,		//�s���~�b�h
		3871,	//���[�v
		FIELD+50	//�ȏ�
	),
	CBGMData(
		"��̓�",
		"��̓�",			14,		//��̓�
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
	CBGMData(
		"�C�x���g�E�_��",
		"Ancient & Calling",15,			//����
		8000,		//���[�v
		EVENT+35	//�ȏ�
	),
//16
	CBGMData(
		"�o�g���Q",
		"Battle 2",		16,			//�{�X�o�g��
		0,		//���[�v
		BATTLE+10	//�ȏ�
	),
	CBGMData(
		"",
		"",				17,			//��̉��i�����j
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
	CBGMData(
		"�o�g���R",
		"Last Battle",	18,			//���X�g�o�g��
		23167,		//���[�v
		BATTLE+20	//�ȏ�
	),
	CBGMData(
		"",
		"",				19,			//���i�����j
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
//20
	CBGMData(
		"�C�x���g�E���|",
		"Knocked & Carriage",20,			//�C�x���g�E�s��
		31314,		//���[�v
		EVENT+5	//�ȏ�
	),
	CBGMData(
		"�C�x���g�E�H��",
		"Dusky & Night",21,			//�C�x���g�E�Í�
		4800,		//���[�v
		EVENT+30	//�ȏ�
	),
	CBGMData(
		"�C�x���g�E�߈�",
		"Rainy & Regret",22,			//�C�x���g�E�߂���
		26174,		//���[�v
		EVENT+40	//�ȏ�
	),
	CBGMData(
		"�X�^�b�t���[��",
		"Staff & Roll",23,				//�C�x���g�E�X�^�b�t���[��
		-1,		//���[�v
		ENDING+10	//�ȏ�
	),
//24
	CBGMData(
		"",
		"�C�x���g�E���",24,			//�C�x���g�E���
		0,		//���[�v
		NOT_PLAY	//�ȏ�
	),
	CBGMData(
		"�^�C�g��",
		"Happy & Birthday",25,			//�^�C�g��
		32005,		//���[�v
		1	//�ȏ�
	),
	CBGMData(
		"�C�x���g�E���̗�",
		"Hearty & Courage",26,			//�C�x���g�E���X�{�X���O
		29204,		//���[�v
		EVENT+100	//�ȏ�
	),
	CBGMData(
		"�G���f�B���O",
		"Happy & Birthday (off melody)",27,	//���ʃG���f�B���O
		10667,		//���[�v
		ENDING+500	//�ȏ�
	),
//28
	CBGMData(
		"�C�x���g�E�s��",
		"Wicked & Boding",28,			//�C�x���g�E�s��
		4800,		//���[�v
		EVENT+4	//�ȏ�
	),
	CBGMData(
		"�_���W�����E����",
		"Invasion & Interception",29,			//�_���W�����E����
		9500,		//���[�v
		FIELD+80	//�ȏ�
	),
};