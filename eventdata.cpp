#include "eventdata.h"
#include "string.h"



////////////////////////////////////////////////////////////////////////////////

CEventData::CEventData()
{
	memset(this,0,sizeof(CEventData));
}
CEventData::~CEventData()
{
	if(mText)	delete[] mText;
	for(int i=0;i<MAX_EVENT_DATA;i++){
		mData[i].AllDelete();
	}
}
void CEventData::AllDelete()
{
	CEventData *ed,*del;
	ed=mNext;
	if(ed){
		del=ed;
		ed=ed->mNext;
		delete del;
	}
	mNext=NULL;
}

////////////////////////////////////////////////////////////////////////////

CEventHeader::CEventHeader()
{
	mCurrent=NULL;
	mNext=NULL;
}
void CEventHeader::AllDelete()
{
	CEventHeader *ed,*del;
	ed=mNext;
	if(ed){
		del=ed;
		ed=ed->mNext;
		delete del;
	}
	mNext=NULL;
}

////////////////////////////////////////////////////////////////////


CEvent::CEvent()
{
	mCurrent=NULL;
	mNext=NULL;
	mWaitTime=-1;
	mKill=false;
}
void CEvent::AllDelete()
{
	CEvent *ed,*del;
	ed=mNext;
	if(ed){
		del=ed;
		ed=ed->mNext;
		delete del;
	}
	mNext=NULL;
}
