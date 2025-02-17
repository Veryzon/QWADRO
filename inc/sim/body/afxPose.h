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

/// O objeto afxPose � um buffer usado para manter o estado de um afxSkeleton como expressado em sua "pose local". 
/// Aqui, cada articula��o no afxSkeleton � representada por um afxTransform que � relativo a sua articula��o-parente imediata. 

#ifndef ASX_POSE_H
#define ASX_POSE_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxTransform.h"

ASX afxError        AfxAcquirePoses(afxSimulation sim, afxUnit cnt, afxUnit const artCnt[], afxPose lp[]);

ASX afxUnit         AfxGetPoseCapacity(afxPose const lp);

ASX afxTransform*   AfxGetPoseTransform(afxPose const pose, afxUnit artIdx);

ASX void            AfxCopyPose(afxPose pose, afxPose const from);

ASX void            AfxApplyRootMotionVectorsToPose(afxPose pose, afxV3d const translation, afxV3d const rotation);

ASX void            AfxAccumulateLocalTransform(afxPose LocalAttitude, int LocalAttitudeBoneIndex, int SkeletonBoneIndex, float Weight, const afxModel ReferenceSkeleton, afxQuatBlend Mode, const afxTransform *Transform);

#endif//ASX_POSE_H
