#include "texturelist.h"
#include <string.h>
#include <stdio.h>

CTextureList::CTextureList(void)
{
	lpDirectTexture=NULL;
	mNextList=NULL;
	mNumber=0;
	mBuffer=NULL;
	
	InitializeCriticalSection(&mCS);
}
CTextureList::~CTextureList(void)
{
	Release();
	ReleaseBuffer();
	DeleteCriticalSection(&mCS);
}
void CTextureList::Release(void)
{
	EnterCriticalSection(&mCS);
	if(lpDirectTexture){
		lpDirectTexture->Release();
		lpDirectTexture=NULL;
	}
	LeaveCriticalSection(&mCS);
}
void CTextureList::ReleaseBuffer(void)
{
	if(mBuffer){
		mBuffer->Release();
		mBuffer=NULL;
	}
}
CTextureList* CTextureList::SetTexture(CTextureList* texturelist,LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color)
{
	while(texturelist){
		if(texturelist->GetNumber()<=0){
			texturelist->CreateTexture(texture,filename,x,y,rt,color);
			return texturelist;
		}else if(!texturelist->mNextList){
			texturelist->mNextList=new CTextureList;
			texturelist->mNextList->CreateTexture(texture,filename,x,y,rt,color);
			return texturelist->mNextList;
		}
		texturelist=texturelist->mNextList;;
	}
	return NULL;
}
void CTextureList::CreateTexture(LPDIRECT3DTEXTURE9 texture,const char* filename,int x,int y,bool rt,D3DCOLOR color)
{
	Release();
	EnterCriticalSection(&mCS);
	lpDirectTexture=texture;
	sprintf(mFileName,"%s",filename);
	if(GetNumber()<=0){
		SetNumber(0);
		mWidth=x;
		mHeight=y;
		mColor=color;
		mRenderTarget=rt;
	}
	mNumber++;
	LeaveCriticalSection(&mCS);
}
CTextureList* CTextureList::SeachTexture(const char* filename)
{
	CTextureList* tlist=this;
	while(tlist){
		if(strcmp(filename,tlist->mFileName)==0){
			tlist->ChangeNumber(+1);
			return tlist;
		}
		tlist=tlist->mNextList;
	}

	return NULL;
}
CTextureList* CTextureList::CheckDelete(CTextureList* texturelist)
{
	if(!texturelist)	return NULL;
	texturelist->mNextList=CheckDelete(texturelist->mNextList);
	if(texturelist->GetNumber()<=0){
		CTextureList *list=texturelist->mNextList;
		delete texturelist;
		return list;
	}
	return texturelist;
}
void CTextureList::AllDelete()
{
	CTextureList* tlist=this;
	if(tlist){
		CTextureList* next=tlist->mNextList;
		delete tlist;
		tlist=next;
	}
}
void CTextureList::AllEscape()
{
	CTextureList* tlist=this;
	EnterCriticalSection(&mCS);
	while(tlist){
		if(tlist->mRenderTarget){
			D3DXSaveTextureToFileInMemory(&(tlist->mBuffer),D3DXIFF_BMP,(tlist->lpDirectTexture),NULL);
		}
		CTextureList* del=tlist;
		tlist=tlist->mNextList;
		del->Release();
	}
	LeaveCriticalSection(&mCS);

}

void  CTextureList::ChangeNumber(int i)	
{
	EnterCriticalSection(&mCS);
	mNumber+=i;
	LeaveCriticalSection(&mCS);
}
void  CTextureList::SetNumber(int i)	
{
	EnterCriticalSection(&mCS);
	mNumber=i;
	LeaveCriticalSection(&mCS);
}
int  CTextureList::GetNumber(void)	
{
	EnterCriticalSection(&mCS);
	int ret=mNumber;
	LeaveCriticalSection(&mCS);
	return ret;
}
LPDIRECT3DTEXTURE9	CTextureList::GetTexture(void)
{
	EnterCriticalSection(&mCS);
	LPDIRECT3DTEXTURE9 ret=lpDirectTexture;
	LeaveCriticalSection(&mCS);
	return ret;
}
void CTextureList::SetTexture(LPDIRECT3DTEXTURE9 tex)
{
	EnterCriticalSection(&mCS);
	lpDirectTexture=tex;
	LeaveCriticalSection(&mCS);
}

