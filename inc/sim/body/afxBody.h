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

/// O objeto afxBody aloja o estado de um afxModel no tempo de execu��o.
/// O afxModel � apenas dados estruturados para um modelo particular --- isto �, este n�o h� qualquer conceito de onde este est�, qual estado este est� em, ou quais anima��es est�o interagindo em si.
/// O afxBody mant�m rastreio disto e prov�m um n�mero de fun��es �teis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades id�nticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam id�nticos, cada um deveria ainda haver seu pr�prio afxBody.
/// Dito isto porque os dados do afxBody s�o todos din�micos, e n�o s�o compartilhados atrav�s de m�ltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto � comum colocar a hierarquia de estruturas do afxModel no cache e instanci�-las m�ltiplas vezes, voc� nunca deveria colocar objetos afxBody no cache e reus�-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da exist�ncia das entidades que eles representam na sua aplica��o.

/// Note, no entanto, que estes s�o simplesmente ponteiros para os dados originais que voc� passou durante a instancia��o.
/// O afxBody n�o mant�m c�pias dos dados (para minimizar uso de mem�ria), ent�o voc� n�o pode liberar os dados originais do modelo e esperar obter ponteiros v�lidos de volta aqui.
/// Para aquilo que importa, se voc� liberar os dados originais do modelo, nenhuma das chamadas para afxBody funcionar�o, desde que elas todas dependem daqueles dados, como voc� esperaria.

#ifndef ASX_BODY_H
#define ASX_BODY_H

#include "qwadro/inc/sim/io/afxModel.h"
#include "qwadro/inc/sim/afxCapstan.h"

ASX afxBool     AfxGetBodyModel(afxBody bod, afxModel* model);
    
ASX void        AsxResetBodyClock(afxBody bod);

ASX void        AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
ASX void        AfxUpdateBodyMotives(afxBody bod, afxReal newClock);

ASX void        AfxPurgeTerminatedMotives(afxBody bod);

AFX_DEFINE_STRUCT(afxAnimSampleContext)
{
    afxBool         accelerated;
    afxUnit         firstPivot;
    afxUnit         pivotCnt;
    afxCapstan        moto;
    afxPose         pose;
    afxPlacement    posb;
    afxReal         allowedErr;
    afxM4d const    displacement;
    afxUnit const*  sparseBoneArray;
};

ASX afxBool     AfxSampleBodyAnimations(afxBody bod, afxAnimSampleContext const* ctx);

ASX void        AmxCmdBindAttitude(afxDrawContext dctx, afxPose pose);
ASX void        AmxCmdBindPosture(afxDrawContext dctx, afxPlacement wp);
ASX void        AmxCmdBindSparsePivotMap(afxDrawContext dctx, afxUnit const* sparseBoneArray);
ASX void        AmxCmdSetAllowedSamplingError(afxDrawContext dctx, afxReal allowedErr);
ASX void        AmxCmdSampleBodyMotions(afxDrawContext dctx, afxBody bod, afxUnit basePivot, afxUnit pivotCnt);

ASX void        AfxSetBodyMass(afxBody bod, afxV3d mass);

ASX void    AfxDoBodyDynamics(afxBody bod, afxReal dt);
ASX void    AfxApplyForceAndTorque(afxBody bod, afxV3d const force, afxV3d const torque);

ASX afxBool     AfxGetBodyPose(afxBody bod, afxPose* pose);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxSpawnBodies(afxModel proto, afxUnit cnt, afxBody bod[]);

ASX afxError    AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[]);

ASX afxUnit     AfxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[]);

#endif//ASX_BODY_H
