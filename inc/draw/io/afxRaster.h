/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 //// SIGMA GL/2                                                           ////
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/*
    A list of function declarations related to raster operations in a graphics or rendering API, 
    likely related to the management and manipulation of raster data (such as textures or images) in a drawing system.

    These functions are used to interact with raster data in a graphics system, likely within a GPU or rendering context. 
    They include operations for testing usage and flags, querying properties, manipulating textures (or images), 
    and transferring data between the system and the GPU or storage.
*/

#ifndef AVX_RASTER_H
#define AVX_RASTER_H

#include "qwadro/inc/draw/io/afxPixel.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/math/afxWhd.h"
#include "qwadro/inc/draw/op/avxRasterOps.h"
 
typedef enum afxRasterUsage
{
    afxRasterUsage_SRC      = AFX_BIT(0), /// The texture can be used as the source of a copy operation.
    afxRasterUsage_DST      = AFX_BIT(1), /// The texture can be used as the destination of a copy or write operation.
    afxRasterUsage_TRANSFER = (afxRasterUsage_SRC | afxRasterUsage_DST),
    afxRasterUsage_SAMPLING = AFX_BIT(2), /// The texture can be bound for use as a sampled texture in a shader.
    afxRasterUsage_STORAGE  = AFX_BIT(3), /// The texture can be bound for use as a storage texture in a shader.
    afxRasterUsage_DRAW     = AFX_BIT(4), /// The texture can be used as a color or depth/stencil attachment in a render pass.
    afxRasterUsage_VIDEO    = (afxRasterUsage_SAMPLING | afxRasterUsage_DRAW),

    afxRasterUsage_ALL      = afxRasterUsage_TRANSFER | afxRasterUsage_SAMPLING | afxRasterUsage_STORAGE | afxRasterUsage_DRAW,
} afxRasterUsage;

typedef enum afxRasterFlag
{
    afxRasterFlag_MIPMAP    = AFX_BIT(0), // has more than 1 LOD, which are mipmaps.
    afxRasterFlag_MULTIMAP  = AFX_BIT(1), // has more than 1 layer/slice (or a set of them, in case of cubemap).
    afxRasterFlag_RESAMPLED = AFX_BIT(2), // has more than 1 LOD, which are supersamples from base image.
    afxRasterFlag_CUBEMAP   = AFX_BIT(3), // is one or more maps of a cubemapping texture.
    
    // autogenerated by acquisition if omitted.
    afxRasterFlag_1D        = AFX_BIT(4), // expands only into its width as a linear buffer.
    afxRasterFlag_2D        = AFX_BIT(5), // a common rectangular image.
    afxRasterFlag_3D        = AFX_BIT(6), // depth is not layer but volumetric extent.

#if !0
    afxRasterFlag_LINEAR_TILING = AFX_BIT(9), /// specifies linear tiling (texels are laid out in memory in row-major order, possibly with some padding on each row).
    //afxRasterFlag_OPTIMAL = AFX_BIT(10), /// specifies optimal tiling (texels are laid out in an implementation-dependent arrangement, for more efficient memory access).
    //afxRasterFlag         = afxRasterFlag_LINEAR | afxRasterFlag_OPTIMAL,
#endif
} afxRasterFlags;

AFX_DEFINE_STRUCT(afxRasterLayout)
/// Information about the layout of the raster subresource.
{
    afxSize         offset; /// the byte offset from the start of the raster or the plane where the raster subresource begins.
    afxUnit         siz; /// the size in bytes of the raster subresource. siz includes any extra memory that is required based on rowStride.
    afxUnit         rowStride; /// the number of bytes between each row of texels in an raster.
    afxUnit         depthStride; ///  the number of bytes between each layer or (3D) slice of an raster.
};

AFX_DEFINE_STRUCT(afxRasterInfo)
{
    avxFormat       fmt;
    afxWhd          extent;
    afxUnit         lodCnt; // mipmaps or supersamples, depending on flags. Default is mipmap.
    afxRasterUsage  usage;
    afxRasterFlags  flags;
    void*           udd;
};

// LOD is mip level or sample level, depending on raster

/*
    The AfxTestRasterUsage() function tests the usage flags of a raster and 
    returns whether the raster is used in specific ways defined by the mask.
*/

AVX afxRasterUsage  AfxTestRasterUsage(afxRaster ras, afxRasterUsage mask);

/*
    The AfxTestRasterFlags() function tests specific flags (probably for state or attributes) of the raster and 
    returns whether those flags are set.
*/

AVX afxRasterFlags  AfxTestRasterFlags(afxRaster ras, afxRasterFlags mask);

/*
    The AfxDescribeRaster() function provides detailed information about the raster, 
    storing the result in the provided afxRasterInfo structure.
*/

AVX void AfxDescribeRaster(afxRaster ras, afxRasterInfo* desc);

/*
    The AfxGetRasterFormat() function returns the format of the raster (e.g., pixel format, bit depth).
*/

AVX avxFormat AfxGetRasterFormat(afxRaster ras);

/*
    The AfxGetRasterExtent() function retrieves the extent (size or dimensions) of a raster at a particular 
    level of detail (LOD), specified by lodIdx.
*/

AVX afxWhd AfxGetRasterExtent(afxRaster ras, afxUnit lodIdx);

/*
    The AfxGetRasterSwizzling() function gets the color swizzling for a given sub-index of a raster (typically for GPU color format mapping).
*/

AVX void AfxGetRasterSwizzling(afxRaster ras, afxUnit subIdx, avxColorSwizzling* csw);

/*
    The AfxQueryRasterLayout() function queries the layout of a raster at a given LOD and layer index, potentially for more detailed memory access patterns.
*/

AVX afxBool AfxQueryRasterLayout(afxRaster ras, afxUnit lodIdx, afxUnit layerIdx, afxRasterLayout* layout);

  //////////////////////////////////////////////////////////////////////////////
 // RASTER INPUT/OUTPUT METHODS                                              //
//////////////////////////////////////////////////////////////////////////////

/*
    The AfxPackRaster() function packs raster data into a buffer for efficient memory transfer to device-local memory.
*/

// Pack/unpack raster data to/from device-local memory buffer.
AVX afxError AfxPackRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, afxBuffer buf);

/*
    The AfxUnpackRaster() function unpacks raster data from a device-local memory buffer back into the appropriate format for use.
*/

AVX afxError AfxUnpackRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, afxBuffer buf);

/*
    The AfxDumpRaster() function dumps raster data into a raw destination (possibly a host memory region), typically for copying purposes.
*/

AVX afxError AfxDumpRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, void* dst);

/*
    The AfxUpdateRaster() function updates raster data from a source buffer or memory location.
*/

AVX afxError AfxUpdateRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, void const* src);

/*
    The AfxUploadRaster() function streams raster data into the system or device from an external stream (likely for loading).
*/

AVX afxError AfxUploadRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, afxStream in);

/*
    The AfxDownloadRaster() function downloads raster data from the system or device to an external stream.
*/

AVX afxError AfxDownloadRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit exuIdx, afxStream out);

/*
    The AfxPrintRaster() function outputs raster data to a file or URI, likely for saving or exporting the raster.
*/

AVX afxError AfxPrintRaster(afxRaster ras, afxRasterIo const* iop, afxUnit lodCnt, afxUnit exuIdx, afxUri const* uri);

/*
    The AfxFetchRaster() function reloads or fetches raster data from a specified URI (possibly fetching a texture or other raster asset).
*/

AVX afxError AfxFetchRaster(afxRaster ras, afxUnit opCnt, afxRasterIo const ops[], afxUnit lodCnt, afxUnit exuIdx, afxUri const* uri); // reload

////////////////////////////////////////////////////////////////////////////////

/*
    The AfxAcquireRasters() function acquires multiple rasters (textures or images) for use in a drawing system, based on the provided afxRasterInfo.
*/

AVX afxError AfxAcquireRasters(afxDrawSystem dsys, afxUnit cnt, afxRasterInfo const info[], afxRaster rasters[]);

/*
    The AfxLoadRasters() function loads multiple rasters from specified URIs into a drawing system.
*/

AVX afxError AfxLoadRasters(afxDrawSystem dsys, afxUnit cnt, afxRasterInfo const info[], afxUri const uri[], afxRaster rasters[]);

/*
    The AfxLoadCubemapRaster() function loads a cubemap (six textures for the six sides of a cube) into the drawing system from the given URIs.
*/

AVX afxRaster AfxLoadCubemapRaster(afxDrawSystem dsys, afxRasterInfo const* info, afxUri const* dir, afxUri const faces[6]);

#endif//AVX_RASTER_H
