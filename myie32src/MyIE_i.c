/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Dec 06 15:57:37 2001
 */
/* Compiler settings for E:\y2001\Myie3.2-476\MyIE.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IMyIENSHandle = {0x25E81C73,0xA6C1,0x46B2,{0xA8,0x3B,0xC0,0x55,0x65,0x64,0x30,0x62}};


const IID LIBID_MyIELib = {0x133F1EF9,0xAE92,0x4C44,{0xAE,0x6B,0xDE,0x5B,0x07,0x01,0xE5,0x9B}};


const CLSID CLSID_MyIENSHandle = {0xB96C53BD,0xFADC,0x45C7,{0xA7,0x2D,0xEB,0x52,0x30,0x43,0x3A,0x46}};


#ifdef __cplusplus
}
#endif

