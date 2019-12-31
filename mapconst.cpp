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
	28,		//�s���A���
	-1,		//�s���A������
	1,		//�̂ǂ��A�^�l����
	-1,		//�������A
//36
	20,		//����
	15,		//���e�B�X�̃e�[�}
	6,		//�d���̃e�[�}
	1,		//���j�[�̃e�[�}
//40
	-1,		//�V���b�N�A�󂪖�������
	20,		//�f�B�A�m�̃e�[�}
	28,		//�����̓���
	28,		//�����̏��N
//44
	28,		//�߂��݁A�l�̎�
	1,		//�����ՁA���낢��Ɣ���		
	15,		//���΂̃e�[�}
	2,		//�~�[���A�̃e�[�}
//48
	28,		//���΂����߂�
	15,		//�^�}�S�̃e�[�}
	6,		//�}�����̕�Q��
	0,		//�s���A���[�ivs���e�B�X
//52
	26,		//�^�}�S�N��
	6,		//�t�B���~�i�̃e�[�}
	22,		//���
	22,		//�߂��݁A���[�i�̃e�[�}
//56
	22,		//���̃^�}�S����
	21,		//���߁E����
	0,
	0,
//60
	0,
	0,
	0,
	0,		//�����ɂ���i0�Ɠ��`�j
//64

/*
//32
	20,		//�s���A�P���̏��l�A
	20,		//�s���A������
	1,		//�̂ǂ��A�^�l����
	24,		//�������A
//36
	21,		//����
	15,		//���e�B�X�̃e�[�}
	1,		//�d���̃e�[�}
	1,		//���j�[�̃e�[�}
//40
	20,		//�V���b�N�A�󂪖�������
	21,		//�f�B�A�m�̃e�[�}
	20,		//�����̓���
	24,		//�����̏��N
//44
	22,		//�߂��݁A�l�̎�
	1,		//�����ՁA���낢��Ɣ���		
	15,		//���΂̃e�[�}
	2,		//�~�[���A�̃e�[�}
//48
	20,		//���΂����߂�
	15,		//�^�}�S�̃e�[�}
	6,		//�}�����̕�Q��
	20,		//�s���A���[�ivs���e�B�X
//52
	15,		//�^�}�S�N��
	6,		//�t�B���~�i�̃e�[�}
	19,		//���
	22,		//�߂��݁A���[�i�̃e�[�}
//56
	22,		//���̃^�}�S����
	0,
	0,
	0,
//60
	0,
	0,
	0,
	0,		//�����ɂ���i0�Ɠ��`�j
//64
*/
};