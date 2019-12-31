#pragma once

/*
 �t�@�C���𔻕ʂ��A�K�؂�CTIAudioFile�C���X�^���X��Ԃ�
 �g���q�Ŕ��ʂ���΂������B
*/

#include "AudioFile.h"
#include <map>
#include <string>
using namespace std;

class CAudioFileFactory
{
private:
	CAudioFileFactory();
public:
	static CAudioFileFactory* Instance(void)
	{
		static CAudioFileFactory s_Ins;
		return &s_Ins;
	};
	virtual ~CAudioFileFactory();

public:
	// �g���q��CTIAudioFile�����т���@�����̊g���q�̓s���I�h�s�v
	void AddExtention(char* ext, CTIAudioFile* audio);
	// �w�肵���g���q�ƌ��т��Ă���AudioFile�̃N���[����Ԃ�
	CTIAudioFile* CreateFromExtention(char* ext);

	// �t�@�C��������g���q�����o��
	void SplitExtention(const char* filename, char* buffer, size_t size);

private:
	typedef map<string, CTIAudioFile*> ExtentionMap;
	ExtentionMap m_ExtMap;
};