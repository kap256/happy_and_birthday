#pragma once

//#define _DEBUG
		//���񂾂�̂킴
//#undef _DEBUG
		//���񂾂�̂킴



//�e��ݒ荀�ځB���J����O�ɂ�������m�F�I/////////////////////

	//#define _NO_USE_GGSOUND_			//GGSound���C���N���[�h���Ȃ��B�i������񉹂͏o�Ȃ��B�j

	#define _PLAY_SOUND_

	const int SOUND_VOLUME	= 7000;		//��{�l�F7000

	const int MONTH_LOCK	= 12;		//��{�l�F12

	//#define _HARD_LOCK_				//��`����Ă���ƁA�n�[�h���[�h���v���C�s�\

	//#define _ADD_DOT_BMP_				//�摜�t�@�C���ǂݍ��݂̖�����.bmp��t��

	//#define _NO_SHOP_					//�V���b�v���g�p�s�\�ɂȂ�B

	//#define _TRIAL_VERSION_			//�̌��Ńf��������B

#ifdef _TRIAL_VERSION_
class CTrialDemo;
extern	CTrialDemo		TrialDemo;
#endif

//�f�o�b�O�p�I�v�V����//////////////////////
#ifdef _DEBUG
	//#define _NO_OPTION_
				//�S�ẴI�v�V�������g��Ȃ�

#ifndef _NO_OPTION_

/*���̑��̃I�v�V����///////////////////*/
	//#define _SET_FPS_	2
				//FPS��n�{

	//#define _SHOW_TUTORIAL
				//�C�x���g�X�L�b�v���ł��`���[�g���A����\��

	//#define _SET_BGM_	1
				//�Đ�����BGM���Œ�

	//#define _GAME_PAD_OFF_
				//�Q�[���p�b�h���g�p�s�\�ɂ���

	#define _ALL_SOUND_TEST_
				//�T�E���h�e�X�g�őS�Ă̋Ȃ��g�p�ł���悤�ɂȂ�B

	//#define _SET_FLAG_	
		#define _SET_FLAG_ID_	56
		#define _SET_FLAG_NO_	60
				//�t���O���Œ�BFlag[ID] = NO; 
/*��ʃf�[�^�m�F�p�I�v�V����///////////////////*/

	//#define _SHOW_FLAG_
		#define _SHOW_FLAG_MIN_ 27
		#define _SHOW_FLAG_MAX_ 27
				//�t���O��\���B

	//#define _DRAW_DATA_
		#define _DRAW_DATA_FRIEND_	CMapMode	//�t�����h�w�肷��ׂ��N���X
		#define _DRAW_DATA_NAME_	MapMode.mBGMWait
		//#define _DRAW_DATA_MIN_		10
		//#define _DRAW_DATA_MAX_		10
					//��ʓI�ȃf�[�^���o�͂���
					//�z��Ŗ����ꍇ�A�ŏ��ƍő���R�����g�A�E�g����΁A�����ŏ�������B
				

	//#define _CHECK_MAPCHIP_
				//�}�b�v�`�b�v���ǂݍ��܂ꂽ���m�F����

/*�O���t�B�b�N�n�I�v�V����///////////////////*/

	//#define _PLAYER_IMAGE_		73
				//�}�b�v��̎�l���̊G

	//#define _BATTLE_IMAGE_		191
				//�퓬���̊G

	//#define _BATTLE_BG_			15
				//�퓬���̔w�i

	#define _SHOW_OBJECT_IMG_NO_
				//�X�y�[�X�L�[�ŁA�摜�ԍ���\���B

/*�퓬�n�I�v�V����///////////////////*/

	//#define _FREE_SKILL_
				//�X�L���g������

	//#define _ALL_SKILL_	
				//�S�X�L���g�p�\ 

	//#define _ALL_SKILL_NO_ 192
				//�K��̃X�L������64���g�p�\�B_ALL_SKILL_���D��

	#define _SET_STATUS_	22
				//�X�e�[�^�X�ُ�������B	

	//#define _ENEMY_LOCK_
		#define _ENEMY_LOCK_TYPE_	71
		#define _ENEMY_LOCK_LV_		4
		#define _ENEMY_LOCK_NO_		4
				//�G�p�[�e�B�[���Œ�B

	//#define	_ENEMY_SKILL_LOCK_		14
				//�G������̃X�L�������g���Ȃ��Ȃ�B

	//#define _BATTLE_FPS_	60
				//�퓬����FPS��ύX

	#define _SUPER_MAN_ 10000
				//�^����_���[�W��n�{�A�󂯂�_���[�W���O

	//#define _POOR_MAN_
				//�^����_���[�W���O

	//#define _LOCK_HP_	1
				//�w�肵��HP�Ő퓬�J�n

	//#define _REFLECT_	
				//�����I�ɔ���

	#define _ESCAPE_	
				//��΂ɓ����ɐ����ł���B

	#define _DELETE_ESCAPE_ENEMY_	
				//������ɓG������

	//#define _ALL_CRITICAL_	
				//��΂ɃN���e�B�J�����o��B

	//#define	_GET_EXP_	50000.0
				//�l���o���ln�{

	#define _MONSTER_KNOWLEDGE_
				//�����w�����K��


/*�}�b�v�n�I�v�V����///////////////////*/

	#define _SHOW_MAP_NO_
				//�}�b�v�ԍ���\���B

	#define _SHOW_PLAYER_POS_
				//��l���̌��ݍ��W��\��

	#define _SHOW_ENEMY_
				//�o������G��\��

	#define _SHOW_OBJECT_NO_
				//�}�b�v�I�u�W�F�N�g�̔ԍ���\���B

	#define _SHOW_TRESURE_NO_
				//�󔠔ԍ���\��

	#define _SHOW_ENEMY_LV_
				//�G�̃��x����\��

	#define _SHOW_OUT_OBJ_
				//�͈͊O�◎���A�����̂Ȃ��ɂ���I�u�W�F�̔ԍ���\��

	//#define _SUPER_JUMP_			20
				//�W�����v�́~n

	//#define _PLAYER_SPEED_		8
				//�}�b�v��̎�l���̑��x�B�ύX���̂݁B

	#define _HAWK_EYE_
				//��̖ڎ�������

	//#define _HAWK_EYE_EVERYTIME_
				//��̖ڏ펞����

	//#define _HIDDEN_PLAYER_
				//�v���C���[��\�����Ȃ�

	//#define _HIDDEN_MESSAGE_
				//���b�Z�[�W��\�����Ȃ�

/*�������n�I�v�V����///////////////////*/
	#define _ALL_ITEM_
				//�q�ɂ��A�C�e���ł����ς��ɂ���B
				//���ԃL�������S���ŏ����璇�ԂɂȂ�B
				//�X�����ׂĎg����l�ɂ���B

	//#define _LEVEL_LOCK_ 60
				//�������x�����Œ肳����B

	//#define _AUTO_LEARN_
				//�a���΂̎����K���B

	//#define _LOCK_FULL_SCREEN_ true
				//�X�N���[�����[�h���Œ�B

/*�f�[�^�o�͌n�I�v�V����///////////////////*/

	//#define _OUTPUT_EXIT_
				//�o�͏I����A���₩�ɏI��

	/*�U�����֘A///////////////////*/
	//#define _OUTPUT_ENEMY_
				//�G�̃p�����[�^���v�Z����HTML�ɓf���܂��B
				//debugoutput�ɃI�v�V��������B

	//#define _OUTPUT_ENEMY_SKILL_
				//�G�̃X�L�����o�͂��܂��B
				//debugoutput�ɃI�v�V��������B
	//#define _OUTPUT_SKILL_USED_
				//���̃X�L���A�{���Ɏg���Ă܂����H


	//#define _OUTPUT_GET_TRESURE_
				//�󔠂̊l���󋵂��A���[�h���ɏo�͂��܂��B

	//#define _OUTPUT_ENEMY_MONTH_
				//���ׂĂ̓G�́A���ʏo���󋵂�HTML�ɓf���܂��B���ƃI�v�V���������p�B
	//#define _OUTPUT_KEY_DATA_
				//�J�M�̓���A��������o�͂��܂��B���ƃI�v�V���������p�B
	//#define _OUTPUT_ALL_ITEM_
				//���ׂĂ̊l���A�C�e���ƁA�l���ł��Ȃ��A�C�e����HTML�ɓf���܂��B���ƃI�v�V���������p�B
	//#define _OUTPUT_TRESURE_
				//���ʂ̊l���ł���󔠂̒��g��HTML�ɓf���܂��B
				//debugoutput�ɃI�v�V��������B

	//#define _OUTPUT_SKILL_
				//�S�L�����̏K���X�L����HTML�ɓf���܂��B

	//#define _OUTPUT_EXP_
				//���x�����ɂ��l���o���l�̔{�����o�͂��܂��B

	//#define _OUTPUT_GROW_PARAM_
				//�e�L�����N�^�[�́A���ʂ̗��_��̍ő�p�����[�^���o�͂��܂��B
		#define _OUTPUT_GROW_PARAM_STEP_ 1


  /*�摜���֘A///////////////////*/
	//#define _OUTPUT_ENEMY_IMG_
				//�}�b�v��ł̓G�L�����N�^�̉摜���o�͂��܂��B

	//#define _OUTPUT_ITEM_IMG_
				//�A�C�e���̃A�C�R�����Ƃ̖��O�Ɛ��������o�͂��܂��B

	//#define _OUTPUT_SKILL_EFFECT_
				//�G�t�F�N�g�摜���ƂɁA�g�p����Ă���Z�E���@���o�͂��܂��B


  /*Wiki�p///////////////////*/
	//#define _OUTPUT_WIKI_ITEM_
				//Wiki�p�A�C�e���ꗗ

	//#define _OUTPUT_WIKI_CSKILL_
				//Wiki�p�b�X�L���ꗗ

	//#define _OUTPUT_WIKI_MONTH_
				//Wiki�p�����ꗗ

	//#define _OUTPUT_WIKI_PLAYER_
				//Wiki�p�v���C���[���ꗗ

  /*�U���p///////////////////*/
	//#define _OUTPUT_MAP_BMP_
				//�U���p�}�b�v�摜
		#define _OUTPUT_MAP_BMP_SIZE_	16

	//#define _OUTPUT_MONTH_ENEMY_REAL_
				//���ڍׂɌ����Ƃ̓G�̏o���󋵂��o��
		//#define _OUTPUT_MONTH_ENEMY_REAL_LD_MODE_	//�n�[�h���X�g�_���W������p���[�h

	#define _OUTPUT_DB_CSKILL_
				//�U���pC�X�L���ꗗ

	#define _OUTPUT_DB_ASKILL_
				//�U���pA�X�L���ꗗ

	//#define _OUTPUT_DB_EQUIP_
				//�U���p�����i�ꗗ

	//#define _OUTPUT_DB_UI_
				//�U���p���Օi�ꗗ

	#define _OUTPUT_DB_PLAYER_
				//�U���p�L�������ꗗ

	#define _OUTPUT_DB_MONTH_
				//�U���p���f�[�^

	#define _OUTPUT_DB_ENEMY_
				//�U���p�G�f�[�^

	#define _OUTPUT_DB_RANKING_
				//�U���p�\�͒l�����L���O

	#define _OUTPUT_DB_SHOP_
				//�U���p�V���b�v�ꗗ

	#define _OUTPUT_DB_BS_
				//�U���p�a���΃��X�g



#endif
#endif

////////////////////////////////////////////////////////////////