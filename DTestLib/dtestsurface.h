#ifndef _DTESTSURFACE_
#define _DTESTSURFACE_

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

class CDTSurface{
	LPDIRECT3DSURFACE9 lpDirectSurface;
public:
	int mWidth;
	int mHeight;

	CDTSurface(void);
	~CDTSurface(void);
	bool Release(void);

	void SetSurface(LPDIRECT3DSURFACE9 surface);

	LPDIRECT3DSURFACE9 GetSurface(void);
};

#endif