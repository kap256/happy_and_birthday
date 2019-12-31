#include "texturelist.h"
#include <string.h>
#include <stdio.h>

CTextureList::CTextureList(void)
{
	lpDirectTexture=NULL;
	mNextList=NULL;
	mNumber=0;
	mBuffer=NULL;
}
CTextureList::~CTextureList(void)
{
	Release();
	ReleaseBuffer();
}
void CTextureList::Release(void)
{
	if(lpDirectTexture){
		lpDirectTexture->Release();
		lpDirectTexture=NULL;
	}
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
		if(texturelist->mNumber<=0){
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
	lpDirectTexture=texture;
	sprintf(mFileName,"%s",filename);
	if(mNumber<=0){
		mNumber=0;
		mWidth=x;
		mHeight=y;
		mColor=color;
		mRenderTarget=rt;
	}
	mNumber++;
}
CTextureList* CTextureList::SeachTexture(const char* filename)
{
	CTextureList* tlist=this;
	while(tlist){
		if(strcmp(filename,tlist->mFileName)==0){
			tlist->mNumber++;
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
	if(texturelist->mNumber<=0){
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
	while(tlist){
		if(tlist->mRenderTarget){
			D3DXSaveTextureToFileInMemory(&(tlist->mBuffer),D3DXIFF_BMP,(tlist->lpDirectTexture),NULL);
		}
		CTextureList* del=tlist;
		tlist=tlist->mNextList;
		del->Release();
	}
}