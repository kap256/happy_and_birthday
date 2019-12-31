
#include "animationdata.h"

CAnimationData::CAnimationData()
{
	mView=false;
}
CAnimationData::CAnimationData(int x,int y,int deg,bool top,bool turn)
{
	mX=x%64;
	mY=y%64;
	mRad=deg*3.141592/180.0;
	mTop=top;
	mView=true;
	mTurn=turn;
}

typedef CAnimationData CAni;

const CAnimationData AnimationData[MAX_CHARACTER_NUMBER*2][16*ANI_TYPE]={
	{	//�J���X
		CAni(36,35,240,false,false),
		CAni(36,35,240,false,false),
		CAni(36,35,240,false,false),
		CAni(36,35,240,false,false),

		CAni(98,36,310,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(163,36,120,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(163,36,120,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(37,35,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(37,35,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(155,111,270,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(200,110,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(35,153,45,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(107,159,30,true,false),		//�U�艺�낵�E�J�n
		CAni(107,159,75,true,true),			//�U��グ�E�I���
		CAni(107,159,270,false,false),		//�˂��E�J�n
		CAni(89,163,340,false,false),		//�|�E�I���

		CAni(152,170,250,false,false),		//�U�艺�낵�E�I���
		CAni(152,170,220,false,true),		//�U��グ�E�J�n
		CAni(155,164,310,true,false),		//�A�C�e������
		CAni(),

		CAni(204,159,270,false,false),		//�U�艺�낵�E�r��
		CAni(204,159,270,false,true),		//�U��グ�E�r��
		CAni(204,159,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(24,223,0,false,false),			//�|�E�J�n
		CAni(30,223,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(99,223,310,false,false),		//���@����
	},
	{	//���[�i
		CAni(29,35,220,false,false),
		CAni(29,35,220,false,false),
		CAni(29,35,220,false,false),
		CAni(29,35,220,false,false),

		CAni(100,35,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(157,106,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(216,105,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,163,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(102,157,60,true,false),
		CAni(102,157,75,true,true),
		CAni(102,157,270,false,false),
		CAni(91,166,300,true,true),			//�|�E�I���

		CAni(153,165,220,true,false),
		CAni(153,165,70,true,true),
		CAni(153,165,220,true,false),//�A�C�e������
		CAni(),

		CAni(203,160,270,false,false),
		CAni(203,160,270,false,true),
		CAni(203,160,270,false,false),
		CAni(),

		CAni(20,226,0,false,false),			//�|�E�J�n
		CAni(30,221,270,false,false),
		CAni(),
		CAni(),

		CAni(87,218,290,false,false),		//���@����
	},

	{	//�z���B
		CAni(28,36,240,false,false),
		CAni(28,36,240,false,false),
		CAni(28,36,240,false,false),
		CAni(28,36,240,false,false),

		CAni(92,35,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,37,120,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,37,120,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,99,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,99,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(155,106,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(204,105,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,163,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(103,157,75,true,false),
		CAni(103,157,75,true,true),
		CAni(103,157,270,false,false),
		CAni(91,162,340,false,false),		//�|�E�I���

		CAni(153,166,220,false,false),
		CAni(153,166,210,false,false),
		CAni(157,165,230,true,false),		//�A�C�e������
		CAni(),

		CAni(212,161,300,false,false),
		CAni(212,161,250,false,true),
		CAni(212,161,270,false,false),
		CAni(),

		CAni(25,226,0,false,false),			//�|�E�J�n
		CAni(29,227,270,false,false),
		CAni(),
		CAni(),

		CAni(89,227,230,false,false),		//���@����
	},
	{	//�g���K��
		CAni(31,39,240,false,false),
		CAni(31,39,240,false,false),
		CAni(31,39,240,false,false),
		CAni(31,39,240,false,false),

		CAni(100,38,130,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(171,34,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(171,34,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(34,101,50,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(34,101,50,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(157,108,260,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(200,110,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(35,164,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(105,167,30,true,false),		//�U�艺�낵�E�J�n
		CAni(105,167,40,true,true),			//�U��グ�E�I���
		CAni(105,167,310,false,false),		//�˂��E�J�n
		CAni(86,163,30,false,false),		//�|�E�I���

		CAni(164,170,250,false,false),		//�U�艺�낵�E�I���
		CAni(164,170,220,false,true),		//�U��グ�E�J�n
		CAni(158,170,310,false,false),		//�A�C�e������
		CAni(),

		CAni(214,163,270,false,false),		//�U�艺�낵�E�r��
		CAni(214,163,270,false,true),		//�U��グ�E�r��
		CAni(214,163,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(23,228,0,false,false),			//�|�E�J�n
		CAni(31,228,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(93,229,320,false,false),		//���@����
	},
	{	//�{���e
		CAni(27,27,60,false,false),
		CAni(27,27,60,false,false),
		CAni(27,27,60,false,false),
		CAni(27,27,60,false,false),

		CAni(97,35,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(168,33,120,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(168,33,120,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,98,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,98,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(158,107,270,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(201,104,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(23,161,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(107,162,70,true,false),		//�U�艺�낵�E�J�n
		CAni(107,162,110,true,true),		//�U��グ�E�I���
		CAni(107,162,290,false,false),		//�˂��E�J�n
		CAni(86,166,20,true,true),			//�|�E�I���

		CAni(158,166,220,true,false),		//�U�艺�낵�E�I���
		CAni(158,166,80,true,true),			//�U��グ�E�J�n
		CAni(162,158,50,true,false),		//�A�C�e������
		CAni(),

		CAni(208,159,270,false,false),		//�U�艺�낵�E�r��
		CAni(208,159,270,false,true),		//�U��グ�E�r��
		CAni(208,159,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(18,225,0,false,false),			//�|�E�J�n
		CAni(32,159,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(90,228,225,false,false),		//���@����
	},
	{	//�~�[���A
		CAni(28,33,120,false,true),
		CAni(28,33,120,false,true),
		CAni(28,33,120,false,true),
		CAni(28,33,120,false,true),

		CAni(102,31,320,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,34,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,34,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,99,60,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,99,60,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(155,104,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(217,107,70,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(23,161,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(102,153,30,false,false),		//�U�艺�낵�E�J�n
		CAni(102,153,30,true,false),		//�U��グ�E�I���
		CAni(102,153,270,false,false),		//�˂��E�J�n
		CAni(91,162,320,true,false),		//�|�E�I���

		CAni(154,166,220,true,false),		//�U�艺�낵�E�I���
		CAni(154,166,80,true,true),			//�U��グ�E�J�n
		CAni(158,162,320,true,false),		//�A�C�e������
		CAni(),

		CAni(212,160,300,false,false),		//�U�艺�낵�E�r��
		CAni(212,160,240,false,true),		//�U��グ�E�r��
		CAni(212,160,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(25,223,0,false,false),			//�|�E�J�n
		CAni(33,220,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(101,223,40,false,false),		//���@����
	},
	{	//���E�F�C��
		CAni(28,33,45,true,false),
		CAni(28,33,45,true,false),
		CAni(28,33,45,true,false),
		CAni(28,33,45,true,false),

		CAni(93,37,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(159,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(159,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,96,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,96,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(153,104,95,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(219,103,85,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,162,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(105,161,70,true,false),		//�U�艺�낵�E�J�n
		CAni(105,161,80,true,true),		//�U��グ�E�I���
		CAni(105,161,270,false,false),		//�˂��E�J�n
		CAni(89,162,0,false,false),		//�|�E�I���

		CAni(158,168,240,false,false),		//�U�艺�낵�E�I���
		CAni(158,168,240,false,true),		//�U��グ�E�J�n
		CAni(169,164,200,true,false),		//�A�C�e������
		CAni(),

		CAni(211,160,270,false,false),		//�U�艺�낵�E�r��
		CAni(211,160,270,false,true),		//�U��グ�E�r��
		CAni(211,160,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(23,226,0,false,false),			//�|�E�J�n
		CAni(31,223,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(92,227,300,false,false),		//���@����
	},
	{	//���j�[
		CAni(28,36,290,false,false),
		CAni(28,36,290,false,false),
		CAni(28,36,290,false,false),
		CAni(28,36,290,false,false),

		CAni(97,37,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(157,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(157,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(91,98,300,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(91,98,300,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(157,105,270,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(211,103,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,163,310,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(104,156,50,true,false),		//�U�艺�낵�E�J�n
		CAni(104,156,70,true,true),			//�U��グ�E�I���
		CAni(104,156,270,false,false),		//�˂��E�J�n
		CAni(101,167,220,true,false),		//�|�E�I���

		CAni(161,167,240,false,false),		//�U�艺�낵�E�I���
		CAni(161,167,220,false,true),		//�U��グ�E�J�n
		CAni(166,165,200,true,false),		//�A�C�e������
		CAni(),

		CAni(213,160,270,false,false),		//�U�艺�낵�E�r��
		CAni(213,160,270,false,true),		//�U��グ�E�r��
		CAni(213,160,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(26,226,0,false,false),			//�|�E�J�n
		CAni(32,222,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(90,227,320,false,false),		//���@����
	},
	{	//���e�B�X
		CAni(28,33,30,false,false),
		CAni(28,33,30,false,false),
		CAni(28,33,30,false,false),
		CAni(28,33,30,false,false),

		CAni(94,35,60,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,36,320,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,36,320,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(96,98,50,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(96,98,50,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(159,110,110,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(223,114,200,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(33,159,30,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(106,155,60,true,false),		//�U�艺�낵�E�J�n
		CAni(106,155,45,true,true),			//�U��グ�E�I���
		CAni(106,155,270,false,false),		//�˂��E�J�n
		CAni(89,162,0,false,false),			//�|�E�I���

		CAni(157,169,240,true,false),		//�U�艺�낵�E�I���
		CAni(157,169,220,true,true),		//�U��グ�E�J�n
		CAni(161,170,130,true,false),		//�A�C�e������
		CAni(),

		CAni(210,162,270,false,false),		//�U�艺�낵�E�r��
		CAni(210,162,270,false,true),		//�U��グ�E�r��
		CAni(210,162,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(22,226,0,false,false),			//�|�E�J�n
		CAni(34,224,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(91,227,45,true,false),		//���@����
	},
	{	//�f�B�A�m
		CAni(31,35,225,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(101,36,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(160,109,270,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(216,105,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,164,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(103,157,60,true,false),
		CAni(103,157,75,true,true),
		CAni(103,157,270,false,false),
		CAni(92,166,300,true,true),			//�|�E�I���

		CAni(156,167,220,true,false),
		CAni(156,167,70,true,true),
		CAni(156,167,220,true,false),		//�A�C�e������
		CAni(),

		CAni(203,161,270,false,false),
		CAni(203,161,270,false,true),
		CAni(203,161,270,false,false),
		CAni(),

		CAni(20,226,0,false,false),			//�|�E�J�n
		CAni(30,221,270,false,false),
		CAni(),
		CAni(),

		CAni(86,222,270,false,false),		//���@����
	},
	{	//�N���b�g
		CAni(27,35,110,false,false),
		CAni(27,35,110,false,false),
		CAni(27,35,110,false,false),
		CAni(27,35,110,false,false),

		CAni(90,34,70,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(160,28,60,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(160,28,60,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(36,100,320,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(36,100,320,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(154,103,95,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(212,122,280,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(32,160,30,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(107,156,30,false,false),		//�U�艺�낵�E�J�n
		CAni(107,156,40,false,true),		//�U��グ�E�I���
		CAni(107,156,310,true,false),		//�˂��E�J�n
		CAni(87,166,320,false,false),		//�|�E�I���

		CAni(156,165,240,false,false),		//�U�艺�낵�E�I���
		CAni(156,165,110,false,true),		//�U��グ�E�J�n
		CAni(168,162,220,true,false),		//�A�C�e������
		CAni(),

		CAni(205,160,270,false,false),		//�U�艺�낵�E�r��
		CAni(205,160,270,false,true),		//�U��グ�E�r��
		CAni(205,160,270,true,false),		//�˂��E�I���
		CAni(),

		CAni(20,224,0,false,false),			//�|�E�J�n
		CAni(33,225,270,true,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(90,227,320,false,false),		//���@����
	},
	{	//�t�B���~�i
		CAni(32,34,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(95,36,320,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,34,50,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(156,34,50,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,101,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,101,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(154,106,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(217,107,70,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,165,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(105,158,60,true,false),		//�U�艺�낵�E�J�n
		CAni(105,158,60,true,true),		//�U��グ�E�I���
		CAni(105,158,270,false,false),		//�˂��E�J�n
		CAni(89,157,0,false,false),			//�|�E�I���

		CAni(157,167,120,true,false),		//�U�艺�낵�E�I���
		CAni(157,167,80,true,true),			//�U��グ�E�J�n
		CAni(166,165,220,true,false),		//�A�C�e������
		CAni(),

		CAni(212,160,300,false,false),		//�U�艺�낵�E�r��
		CAni(212,160,240,false,true),		//�U��グ�E�r��
		CAni(212,160,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(24,222,0,false,false),			//�|�E�J�n
		CAni(33,220,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(91,228,220,false,false),		//���@����
	},
	{	//���[�i�i�ߋ��j
		CAni(28,34,120,false,true),
		CAni(28,34,120,false,true),
		CAni(28,34,120,false,true),
		CAni(28,34,120,false,true),

		CAni(92,32,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,35,200,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,35,200,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,100,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,100,290,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(154,106,280,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(223,102,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,163,45,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(105,159,30,true,false),		//�U�艺�낵�E�J�n
		CAni(105,159,75,true,true),			//�U��グ�E�I���
		CAni(105,159,270,false,false),		//�˂��E�J�n
		CAni(89,159,0,false,false),			//�|�E�I���

		CAni(158,167,220,true,false),		//�U�艺�낵�E�I���
		CAni(152,170,60,true,true),		//�U��グ�E�J�n
		CAni(160,161,30,true,false),		//�A�C�e������
		CAni(),

		CAni(212,160,270,false,false),		//�U�艺�낵�E�r��
		CAni(212,160,270,false,true),		//�U��グ�E�r��
		CAni(212,160,270,false,false),		//�˂��E�I���
		CAni(),

		CAni(24,223,0,false,false),			//�|�E�J�n
		CAni(32,221,270,false,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(87,217,30,false,false),		//���@����
	},
	{},
	{	//�f�B�A�m
		CAni(31,35,225,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(101,36,250,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(161,36,220,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(26,97,300,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(160,109,270,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(216,105,260,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(28,164,230,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(103,157,60,true,false),
		CAni(103,157,75,true,true),
		CAni(103,157,270,false,false),
		CAni(92,166,300,true,true),			//�|�E�I���

		CAni(156,167,220,true,false),
		CAni(156,167,70,true,true),
		CAni(156,167,220,true,false),		//�A�C�e������
		CAni(),

		CAni(203,161,270,false,false),
		CAni(203,161,270,false,true),
		CAni(203,161,270,false,false),
		CAni(),

		CAni(20,226,0,false,false),			//�|�E�J�n
		CAni(30,221,270,false,false),
		CAni(),
		CAni(),

		CAni(86,222,270,false,false),		//���@����
	},	//�J���X�񓁗�
	{	CAni(30,37,240,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(91,33,70,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(146,33,80,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(146,33,80,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,100,220,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(25,100,220,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(163,107,270,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(201,119,320,false,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(30,153,320,true,false),
		CAni(),
		CAni(),
		CAni(),

		CAni(89,163,60,true,false),		//�U�艺�낵�E�J�n
		CAni(89,163,60,true,true),		//�U��グ�E�I���
		CAni(89,163,60,true,false),		//�˂��E�J�n
		CAni(89,163,60,true,true),		//�|�E�I���

		CAni(154,164,300,true,false),		//�U�艺�낵�E�I���
		CAni(154,164,300,true,true),		//�U��グ�E�J�n
		CAni(154,164,300,true,false),//�A�C�e������
		CAni(),

		CAni(218,162,0,true,false),		//�U�艺�낵�E�r��
		CAni(218,162,0,true,true),		//�U��グ�E�r��
		CAni(218,162,0,true,false),		//�˂��E�I���
		CAni(),

		CAni(33,223,320,false,false),			//�|�E�J�n
		CAni(24,224,30,true,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(101,213,80,true,false),		//���@����
	},
	//���[�i�񓁗�
	{	CAni(38,36,130,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(86,38,120,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(154,37,70,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(154,37,70,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(34,97,220,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(34,97,220,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(165,107,80,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(223,119,120,false,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(35,167,130,true,true),
		CAni(),
		CAni(),
		CAni(),

		CAni(94,166,30,true,true),		//�U�艺�낵�E�J�n
		CAni(94,166,30,true,false),		//�U��グ�E�I���
		CAni(94,166,30,true,true),		//�˂��E�J�n
		CAni(94,166,30,true,true),		//�|�E�I���

		CAni(147,158,250,false,true),		//�U�艺�낵�E�I���
		CAni(147,159,240,false,false),		//�U��グ�E�J�n
		CAni(156,167,240,true,false),//�A�C�e������
		CAni(),

		CAni(213,166,140,true,true),		//�U�艺�낵�E�r��
		CAni(213,166,140,true,false),		//�U��グ�E�r��
		CAni(213,166,140,true,true),		//�˂��E�I���
		CAni(),

		CAni(35,221,90,false,true),			//�|�E�J�n
		CAni(21,229,130,true,false),		//�˂��E�r��
		CAni(),
		CAni(),

		CAni(105,228,70,false,true),		//���@����
	},


};