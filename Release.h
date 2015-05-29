#ifndef _RELEASE_H_
#define _RELEASE_H_

#define NULL_DELETE(p) { if( p ){ delete p; p = NULL; } }
#define NULL_ARRAY_DELETE(p) { if( p ){ delete[] p; p = NULL; } }
#define SAFE_RELEASE(p) { if( p ){ p->Release(); p = NULL; } }

#endif