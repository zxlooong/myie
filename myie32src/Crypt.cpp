// Crypt.cpp: implementation of the CCrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myie.h"
#include "Crypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#pragma optimize( "s", on)

char InnKey[9]="7eOp@m0D";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CCrypt::CCrypt()
{

}

CCrypt::~CCrypt()
{

}
*/

void Encoding(LPCSTR src, CString& dest)
{
	int l=0;
	char tmp;
	BYTE t, t2;
	dest.Empty();
	for(UINT i = 0; i<strlen(src); i++)
	{
		t = src[i] ^ InnKey[l];
		t2 = t & 0xF;
		t = t >> 4;
		tmp = 'A' + t;
		dest += tmp;
		tmp = 'A' + t2;
		dest += tmp;

		l++;
		l=l%8;
	}
}

void Decoding(LPCSTR src, CString& dest)
{
	int l=0, v;
	BYTE t,	c=0;
	dest.Empty();
	for(UINT i=0; i<strlen(src); i++)
	{
		t=src[i];
		if(t>='A' && t<='Q')
			v = t-'A';
		else
			v = 0;
	
		c = c << 4;
		c |= v;
		if(i%2==1)
		{
			c = c ^ InnKey[l];
			dest += (char)c;
			c=0;
			l++;
			l = l%8;
		}
	}
}


//#pragma optimize( "s", off)

