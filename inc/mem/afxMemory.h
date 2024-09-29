/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Memory Management Unit                                                   //
//////////////////////////////////////////////////////////////////////////////

/// MEMORY LAYOUT
/// size is the raw lenght of a type in bytes.
/// alignment is an additional size used to pad elements in memory.
/// stride is the sum of size with alignment.

#ifndef AFX_MEMORY_H
#define AFX_MEMORY_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/exec/afxSlock.h"
#include "qwadro/inc/io/afxUri.h"

typedef enum afxAllocationFlags
{
    AFX_ALL_FLAG_RESIZABLE  = (1 << 8),
    AFX_ALL_FLAG_ZEROED     = (1 << 9),
} afxAllocationFlags;

typedef enum afxAllocationDuration
{
    AFX_ALL_DUR_TEMPORARY, // to be used just inside "this" function.
    AFX_ALL_DUR_TRANSIENT, // to be used across functions, as example signaling objects about an event occurance.
    AFX_ALL_DUR_PERMANENT, // to be used across the entire system and or subsystem, 
} afxAllocationDuration;

#define AfxFlagsGetDuration(dur_) (dur_ & 0x00000003)

#define AFX_ALL_EXTRA_DATA 256

AFX_DEFINE_STRUCT(afxAllocationStrategy)
{
    afxSize                 size;    // size of an fixed-size allocation or size of afxByte for arbitrary allocations.
    afxSize                 align;  // useful when element requires alignment, such as 128-bit alignment for SIMD instructions. If zero, Qwadro will align to 16 bytes.
    afxSize                 cap;    // useful to restrict space to a certain capacity or, when elemSiz is equal to afxByte it will be the max totality of bytes for arbitrary allocations.
    afxSize                 stock;  // useful to preallocate space for fixed-size allocations.
    afxAllocationDuration   duration;   // the expected duration of allocations.
    afxBool                 resizable;  // flag to indicate if allocations may be required to be resized.    
};

typedef enum afxMemFlags
{
    afxMemFlag_ZEROED       = AFX_BIT(0),
    afxMemFlag_RESIZABLE    = AFX_BIT(1),

    afxMemFlag_TEMPORARY    = AFX_BIT(8), // to be used just inside "this" function.
    afxMemFlag_TRANSIENT    = AFX_BIT(9), // to be used across functions, as example signaling objects about an event occurance.
    afxMemFlag_PERMANENT    = AFX_BIT(10), // to be used across the entire system and or subsystem, 
    afxMemFlag_DURATION_MASK= afxMemFlag_TEMPORARY | afxMemFlag_TRANSIENT | afxMemFlag_PERMANENT,

    AFX_MEM_PROP_DEVICE_LOCAL, // specifies that memory allocated with this type is the most efficient for device access. This property will be set if and only if the memory type belongs to a heap with the VK_MEMORY_HEAP_DEVICE_LOCAL_BIT set.
    AFX_MEM_PROP_HOST_VISIBLE, // specifies that memory allocated with this type can be mapped for host access using vkMapMemory.
    AFX_MEM_PROP_HOST_COHERENT, // specifies that the host cache management commands vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges are not needed to flush host writes to the device or make device writes visible to the host, respectively.
    AFX_MEM_PROP_HOST_CACHED, // specifies that memory allocated with this type is cached on the host.Host memory accesses to uncached memory are slower than to cached memory, however uncached memory is always host coherent.
    AFX_MEM_PROP_LAZILY_ALLOCATED // specifies that the memory type only allows device access to the memory.
} afxMemFlags;

typedef void*(*afxAllocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxHere const hint);
typedef void*(*afxReallocCallback)(afxMmu mmu, void* p, afxSize cnt, afxSize siz, afxSize align, afxHere const hint);
typedef void(*afxDeallocCallback)(afxMmu mmu, void* p);
typedef void(*afxNotifyAllocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint);
typedef void(*afxNotifyDeallocCallback)(afxMmu mmu, afxSize cnt, afxSize siz, afxHere const hint);

typedef afxMemory(*afxAllocCallback2)(afxMmu mmu, afxSize cnt, afxSize siz, afxSize align, afxMemFlags flags, afxHere const hint);
typedef afxError(*afxResizeCallback2)(afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);
typedef afxError(*afxDeallocCallback2)(afxMemory mem, afxHere const hint);
typedef void(*afxNotifyAllocCallback2)(afxMmu mmu, afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);
typedef void(*afxNotifyDeallocCallback2)(afxMmu mmu, afxMemory mem, afxSize cnt, afxSize siz, afxHere const hint);

AFX_DEFINE_STRUCT(afxAllocator)
{
    afxAllocCallback            alloc;
    afxReallocCallback          realloc;
    afxDeallocCallback          dealloc;
    afxNotifyAllocCallback      allocNot;
    afxNotifyDeallocCallback    deallocNot;
};

typedef void*(*ator)(afxMmu mmu, void* old, afxSize siz, afxSize align, afxHere const hint); // if siz is zero, memory is freed

#ifdef _AFX_MMU_C
AFX_OBJECT(afxMmu)
{
    afxAllocCallback            allocCb;
    afxReallocCallback          reallocCb;
    afxDeallocCallback          deallocCb;
    afxNotifyAllocCallback      allocNotCb;
    afxNotifyDeallocCallback    deallocNotCb;

    afxAllocCallback2           allocCb2;
    afxResizeCallback2          reallocCb2;
    afxDeallocCallback2         deallocCb2;
    afxNotifyAllocCallback2     allocNotCb2;
    afxNotifyDeallocCallback2   deallocNotCb2;

    afxSlock                    memSlock;
    afxChain                    memChain;
    afxSize                     defAlign;

// debug
    afxNat                      dbgLevel; // 0 - nothing, 1 - mechanism activity, 2 - block activity, 3 - portion (de)allocations, 4 - [implementation-dependent]
    afxChar const*              func;
    afxChar const*              fname;
    afxSize                     fline;
};
#endif//_AFX_MMU_C

typedef enum afxMemoryFlag
{
    afxMemoryFlag_R = AFX_BIT(0),
    afxMemoryFlag_W = AFX_BIT(1),
    afxMemoryFlag_X = AFX_BIT(2),


} afxMemoryFlags;

AFX afxError                AfxOpenMemory(afxMemoryFlags flags, afxUri const* uri, afxHere const hint, afxMemory* memory);
AFX afxError                AfxAcquireMemory(afxMemoryFlags flags, afxSize siz, afxUri const* uri, afxHere const hint, afxMemory* memory);

AFX afxError                AfxMapMemory(afxMemory mem, afxSize offset, afxNat range, afxFlags flags, void** var);
AFX afxError                AfxUnmapMemory(afxMemory mem, afxSize offset, afxNat range);

AFX afxError                AfxMemoryEnableDebugging(afxMmu mmu, afxNat level);
AFX afxSize                 AfxMemoryGetDefaultAlignment(afxMmu mmu);

AFX afxError    AfxAllocate2(void** var, afxSize siz, afxNat align, afxHere const hint);
AFX afxError    AfxReallocate2(void** var, afxSize siz, afxNat align, afxHere const hint);
AFX afxError    AfxCoallocate2(void** var, afxSize siz, afxNat cnt, afxNat align, afxHere const hint);
AFX afxError    AfxDeallocate2(void** var, afxHere const hint);

AFX void*                   AfxAllocate(afxSize cnt, afxSize siz, afxNat align, afxHere const hint);
AFX void*                   AfxCoallocate(afxSize siz, afxSize cnt, afxNat align, afxHere const hint);
AFX void*                   AfxReallocate(void *p, afxSize siz, afxSize cnt, afxNat align, afxHere const hint);
AFX void                    AfxDeallocate(void *p);

#define                     AfxStream(cnt_,srcStride_,dstStride_,src_,dst_) AfxStream2(cnt_,src_,srcStride_,dst_,dstStride_)
AFX void                    AfxStream2(afxNat cnt, void const* src, afxSize srcStride, void* dst, afxNat dstStride);
AFX void                    AfxStream3(afxNat cnt, void const* src, afxNat srcOffset, afxSize srcStride, void* dst, afxNat dstOffset, afxNat dstStride);

#define AFX_ALIGNED_SIZEOF(operand_,alignment_) ((operand_ + (alignment_ - 1)) & ~(alignment_ - 1))


AFX afxInt          AfxMemcmp(void const* buf1, void const* buf2, afxSize siz);
AFX void const*     AfxMemchr(void const* buf, afxInt val, afxNat cap);

AFX void            AfxCopy(void* dst, void const* src, afxSize siz);
AFX void            AfxCopy2(void* dst, void const* src, afxSize siz, afxSize cnt);
AFX void*           AfxMemcpy(void* dst, void const* src, afxSize siz);

AFX void            AfxFill(void* p, void const* value, afxSize siz);
AFX void            AfxFill2(void* p, void const* value, afxSize siz, afxSize cnt);
AFX void*           AfxMemset(void* dst, afxInt val, afxSize siz);

AFX void            AfxZero(void* p, afxSize siz);
AFX void            AfxZero2(void* p, afxSize siz, afxSize cnt);

AFX void            AfxFree(void* block);
AFX void*           AfxMalloc(afxSize siz);
AFX void*           AfxCalloc(afxSize cnt, afxSize siz);
AFX void*           AfxRealloc(void* block, afxSize siz);

AFX void            AfxFreeAligned(void* block);
AFX void*           AfxMallocAligned(afxSize siz, afxSize align);
AFX void*           AfxReallocAligned(void* block, afxSize siz, afxSize align);
AFX void*           AfxRecallocAligned(void* block, afxSize cnt, afxSize siz, afxSize align);


AFX afxError _AfxInitMmu(afxThread thr);
AFX afxError _AfxDeinitMmu(afxThread thr);

#endif//AFX_MEMORY_H
