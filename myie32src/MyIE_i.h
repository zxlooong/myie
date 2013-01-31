/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Dec 06 15:57:37 2001
 */
/* Compiler settings for E:\y2001\Myie3.2-476\MyIE.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MyIE_i_h__
#define __MyIE_i_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMyIENSHandle_FWD_DEFINED__
#define __IMyIENSHandle_FWD_DEFINED__
typedef interface IMyIENSHandle IMyIENSHandle;
#endif 	/* __IMyIENSHandle_FWD_DEFINED__ */


#ifndef __MyIENSHandle_FWD_DEFINED__
#define __MyIENSHandle_FWD_DEFINED__

#ifdef __cplusplus
typedef class MyIENSHandle MyIENSHandle;
#else
typedef struct MyIENSHandle MyIENSHandle;
#endif /* __cplusplus */

#endif 	/* __MyIENSHandle_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMyIENSHandle_INTERFACE_DEFINED__
#define __IMyIENSHandle_INTERFACE_DEFINED__

/* interface IMyIENSHandle */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMyIENSHandle;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("25E81C73-A6C1-46B2-A83B-C05565643062")
    IMyIENSHandle : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMyIENSHandleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMyIENSHandle __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMyIENSHandle __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMyIENSHandle __RPC_FAR * This);
        
        END_INTERFACE
    } IMyIENSHandleVtbl;

    interface IMyIENSHandle
    {
        CONST_VTBL struct IMyIENSHandleVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMyIENSHandle_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMyIENSHandle_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMyIENSHandle_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMyIENSHandle_INTERFACE_DEFINED__ */



#ifndef __MyIELib_LIBRARY_DEFINED__
#define __MyIELib_LIBRARY_DEFINED__

/* library MyIELib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MyIELib;

EXTERN_C const CLSID CLSID_MyIENSHandle;

#ifdef __cplusplus

class DECLSPEC_UUID("B96C53BD-FADC-45C7-A72D-EB5230433A46")
MyIENSHandle;
#endif
#endif /* __MyIELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
