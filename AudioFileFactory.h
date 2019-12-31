#pragma once

/*
 ファイルを判別し、適切なCTIAudioFileインスタンスを返す
 拡張子で判別すればいいか。
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
	// 拡張子とCTIAudioFileを結びつける　引数の拡張子はピリオド不要
	void AddExtention(char* ext, CTIAudioFile* audio);
	// 指定した拡張子と結びついているAudioFileのクローンを返す
	CTIAudioFile* CreateFromExtention(char* ext);

	// ファイル名から拡張子を取り出す
	void SplitExtention(const char* filename, char* buffer, size_t size);

private:
	typedef map<string, CTIAudioFile*> ExtentionMap;
	ExtentionMap m_ExtMap;
};