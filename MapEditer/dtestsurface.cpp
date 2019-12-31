#include "dtestsurface.h"


CDTSurface::CDTSurface()
{
	lpDirectSurface=NULL;
}
CDTSurface::~CDTSurface()
{
	Release();
}
bool CDTSurface::Release()
{
	if(lpDirectSurface){
		lpDirectSurface->Release();
		lpDirectSurface=NULL;
	}

	return true;
}
void CDTSurface::SetSurface(LPDIRECT3DSURFACE9 surface)
{
	lpDirectSurface=surface;
}
LPDIRECT3DSURFACE9 CDTSurface::GetSurface(void)
{
	return lpDirectSurface;
}

