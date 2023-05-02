// anim.c
//

#include "stdafx.h"

#include "anim.h"
#include "cartdir.h"

#ifdef WIN32
#include "utils.h"
#define frame_increment (1.0)
#else
#include "tengine.h"
#include "adpcm2.h"
#endif

// CGameAnimateState implementation
/////////////////////////////////////////////////////////////////////////////

// CGameAnimateState
/////////////////////////////////////////////////////////////////////////////

void CGameAnimateState__Construct(CGameAnimateState *pThis)
{
	pThis->m_nAnim = pThis->m_nDesiredAnim	= 0;
	pThis->m_cFrame 					 			= 1;
	pThis->m_nFrames 					 			= 0;
	pThis->m_pceAnim 					 			= NULL;
	pThis->m_CycleCompleted						= FALSE;
}

int CGameAnimateState__GetTransitionAnim(CGameAnimateState *pThis)
{
	CIndexedSet 			isAnim,
								isTransitionTable;
	void						*pbTransitionTable,
								*pbTransitionEntries;
	CUnindexedSet			usTableEntries;
	int						deliveredAnim,
								cEntry, nEntries;
	CROMTransitionEntry  *pTransitionEntries,
								*pEntry, *pCurrentEntry;

	ASSERT((DWORD) pThis >= 0x80000000);
	ASSERT((DWORD) pThis <= 0x84000000);
	ASSERT(pThis->m_nDesiredAnim >= 0);

				// is a transition requested?
   //if (pThis->m_nDesiredAnim == pThis->m_nAnim)
	//{
	//	// no
	//	deliveredAnim = -1;
	//}
	//else
	if (TRUE)
   {
		ASSERT(pThis->m_pceAnim);

		// attatch isAnim
		CIndexedSet__ConstructFromRawData(&isAnim,
													 CCacheEntry__GetData(pThis->m_pceAnim),
													 FALSE);

		// get transition table block
		pbTransitionTable = CIndexedSet__GetBlock(&isAnim, CART_ANIM_isTransitionTable);
		CIndexedSet__ConstructFromRawData(&isTransitionTable, pbTransitionTable, FALSE);

		pbTransitionEntries = CIndexedSet__GetBlock(&isTransitionTable, CART_TRANTABLE_usTableEntries);
		CUnindexedSet__ConstructFromRawData(&usTableEntries, pbTransitionEntries, FALSE);

		pEntry = NULL;

		pTransitionEntries = (CROMTransitionEntry*) CUnindexedSet__GetBasePtr(&usTableEntries);
		nEntries = CUnindexedSet__GetBlockCount(&usTableEntries);
		for (cEntry=0; cEntry<nEntries; cEntry++)
		{
			pCurrentEntry = &pTransitionEntries[cEntry];

			if (pCurrentEntry->m_nRequestedAnim == pThis->m_nDesiredAnim)
			{
				pEntry = pCurrentEntry;
				break;
			}
		}

      if (pEntry)
      {
         // found matching entry, wait for frame
			if ((pEntry->m_nWaitForFrame == ((DWORD) -1))
				 || ((pThis->m_cFrame >= pEntry->m_nWaitForFrame)
				    && (pThis->m_cFrame < (pEntry->m_nWaitForFrame + frame_increment))) )
         {
            // do transition
            deliveredAnim = pEntry->m_nDeliveredAnim;

				//TRACE5("nAnim:%d, cFrame:%f, desired:%d, waitFrame:%d, delivered:%d\r\n",
				//		pThis->m_nAnim,
				//		pThis->m_cFrame,
				//		pThis->m_nDesiredAnim,
				//		pEntry->m_nWaitForFrame,
				//		deliveredAnim);
         }
			else
			{
				deliveredAnim = -1;
			}
      }
      else
		{
			if (pThis->m_nDesiredAnim == pThis->m_nAnim)
			{
				// no transition
				deliveredAnim = -1;
			}
			else
			{
				// matching entry not found, do transition immediately
				deliveredAnim = pThis->m_nDesiredAnim;
				
				ASSERT(pThis->m_nDesiredAnim >= 0);
			}
		}

		CIndexedSet__Destruct(&isAnim);
		CIndexedSet__Destruct(&isTransitionTable);
		CUnindexedSet__Destruct(&usTableEntries);
   }

	return deliveredAnim;
}

int CGameAnimateState__GetNextFrame(CGameAnimateState *pThis)
{
	CIndexedSet 		isAnim,
							isTransitionTable;
	void					*pbTransitionTable;
	CROMTransition		*pTransition;
	int					cNextFrame;

	// increment frame counter
	cNextFrame = ((int) pThis->m_cFrame) + 1;

	if (cNextFrame >= pThis->m_nFrames)
	{
		// if past end, go to followup frame

		ASSERT(pThis->m_pceAnim);

		// attatch isAnim
		CIndexedSet__ConstructFromRawData(&isAnim,
													 CCacheEntry__GetData(pThis->m_pceAnim),
													 FALSE);

		// get transition table block
		pbTransitionTable = CIndexedSet__GetBlock(&isAnim, CART_ANIM_isTransitionTable);
		CIndexedSet__ConstructFromRawData(&isTransitionTable, pbTransitionTable, FALSE);

		pTransition = (CROMTransition*) CIndexedSet__GetBlock(&isTransitionTable, CART_TRANTABLE_transition);

		cNextFrame = pTransition->m_nExitToFrame;

		CIndexedSet__Destruct(&isAnim);
		CIndexedSet__Destruct(&isTransitionTable);
	}

	ASSERT(cNextFrame >= 0);
	return cNextFrame;
}

void CGameAnimateState__Advance(CGameAnimateState *pThis)
{
	CIndexedSet 		isAnim,
							isTransitionTable;
	void					*pbTransitionTable;
	CROMTransition		*pTransition;

	// increment frame counter
	pThis->m_cFrame += frame_increment;

	if (pThis->m_cFrame >= pThis->m_nFrames)
	{
		// if past end, go to followup frame

		ASSERT(pThis->m_pceAnim);

		// attatch isAnim
		CIndexedSet__ConstructFromRawData(&isAnim,
													 CCacheEntry__GetData(pThis->m_pceAnim),
													 FALSE);

		// get transition table block
		pbTransitionTable = CIndexedSet__GetBlock(&isAnim, CART_ANIM_isTransitionTable);
		CIndexedSet__ConstructFromRawData(&isTransitionTable, pbTransitionTable, FALSE);

		pTransition = (CROMTransition*) CIndexedSet__GetBlock(&isTransitionTable, CART_TRANTABLE_transition);

		pThis->m_cFrame = pTransition->m_nExitToFrame + pThis->m_cFrame - pThis->m_nFrames;
		if (pThis->m_cFrame >= pThis->m_nFrames)
			pThis->m_cFrame = pTransition->m_nExitToFrame;

		pThis->m_CycleCompleted = TRUE;

		CIndexedSet__Destruct(&isAnim);
		CIndexedSet__Destruct(&isTransitionTable);
	}
}


// CROMTransition
/////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
void CROMTransition__TakeFromTransitionTable(CROMTransition *pThis, CTransitionTable *pTable)
{
	pThis->m_BlendFromLength 	= ORDERBYTES((WORD) pTable->m_BlendFromLength);
	pThis->m_nExitToFrame		= ORDERBYTES((WORD) pTable->m_nExitToFrame);
	pThis->m_BlendStart			= ORDERBYTES((WORD) pTable->m_BlendStart);
	pThis->m_BlendFinish			= ORDERBYTES((WORD) pTable->m_BlendStart);
}
#endif


// CROMTransitionEntry
/////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
void CROMTransitionEntry__TakeFromTransitionEntry(CROMTransitionEntry *pThis, CTransitionEntry *pEntry,
																  int AnimIndices[], int nAnims)
{
	int cAnim;
	for (cAnim=0; cAnim<nAnims; cAnim++)
		if (AnimIndices[cAnim] == pEntry->m_nRequestedAnim)
			pThis->m_nRequestedAnim = ORDERBYTES((DWORD) cAnim);

	pThis->m_nWaitForFrame 	= ORDERBYTES((DWORD) pEntry->m_nFrame);

	for (cAnim=0; cAnim<nAnims; cAnim++)
		if (AnimIndices[cAnim] == pEntry->m_nDeliveredAnim)
			pThis->m_nDeliveredAnim = ORDERBYTES((DWORD) cAnim);
}
#endif


// CROMEventEntry
/////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
void CROMEventEntry__TakeFromEventEntry(CROMEventEntry *pThis, CEventEntry *pSource, float Scaler)
{
	pThis->m_nFrame  = ORDERBYTES((unsigned short) pSource->m_nFrame);
	pThis->m_nEvent  = ORDERBYTES((unsigned short) pSource->m_nEvent);
	pThis->m_Value   = ORDERBYTES(pSource->m_Value);

	// convert vOffset to 3D Studio coordinate system so it can use
	// the same orientation matrix as the model
	CVector3 v3ds(pSource->m_vOffset.x, pSource->m_vOffset.z, pSource->m_vOffset.y);

	CVector3 vOffset = v3ds * Scaler;
	TruncateFloat2(&vOffset.x);
	TruncateFloat2(&vOffset.y);
	TruncateFloat2(&vOffset.z);

	pThis->m_vOffset = ORDERBYTES(vOffset);
}
#endif


// CGameObjectInstance
/////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
void CGameObjectInstance__DecompressAnim(CGameObjectInstance *pThis, CCacheEntry **ppceTarget)
{
	CIndexedSet				isAnim, isNewAnim;
	CUnindexedSet			usYRots,
								usTranslationSets, usNewTranslationSets,
								usRotationSets, usNewRotationSets,
								usTranslationSet, usRotationSet,
								usEvents;
	int						allocSize,
								cFrame, nFrames,
								cTranslationSet, nTranslationSets,
								cRotationSet, nRotationSets,
								offset, translationSetsOffset, rotationSetsOffset,
								scratchAreaSize;
	void						*pbYRots, *pbNewYRots,
								*pbNodeAnimIndices, *pbNewNodeAnimIndices,
								*pbInitialOrients, *pbNewInitialOrients,
								*pbTranslationSets, *pbNewTranslationSets,
								*pbTranslationSet, *pbNewTranslationSet,
								*pbRotationSets, *pbNewRotationSets,
								*pbRotationSet, *pbNewRotationSet,
								*pbEvents, *pbNewEvents,
								*pbTransitionTable, *pbNewTransitionTable,
								*pbNewAnim;
	DWORD						yRotsSize,
								nodeAnimIndicesSize,
								initialOrientsSize,
								transitionTableSize,
								eventsSize,
								newTranslationSetSize,
								newRotationSetSize;
	BYTE						*pBytes,
								*pScratchArea;
	short						*rawBuffers[4];
	CRotFrame				*rotFrames, *pRotFrame;
	CPosFrame				*posFrames, *pPosFrame;
	CTranslationOffset	*pTransOffset;
#ifdef IG_DEBUG
	OSTime			startTime, endTime, diffTime;
#endif

	ASSERT(ppceTarget == &pThis->m_asCurrent.m_pceAnim);

	TRACE0("CGameObjectInstance: Decompressing animation data.\r\n");
#ifdef IG_DEBUG
	startTime = osGetTime();
#endif

	if (!CCacheEntry__DecompressTemp(pThis->m_asCurrent.m_pceAnim))
		return;

	// calculate allocation size
	allocSize = 0;

	CIndexedSet__ConstructFromRawData(&isAnim,
												 CCacheEntry__GetData(pThis->m_asCurrent.m_pceAnim),
												 FALSE);
	allocSize += IS_INDEX_SIZE(CART_ANIM_nItems);

	pbYRots = CIndexedSet__GetBlockAndSize(&isAnim, CART_ANIM_usYRotations, &yRotsSize);
	allocSize += yRotsSize;

	// use usYRots to get number of frames
	CUnindexedSet__ConstructFromRawData(&usYRots, pbYRots, FALSE);
	nFrames = CUnindexedSet__GetBlockCount(&usYRots);

	pbNodeAnimIndices = CIndexedSet__GetBlockAndSize(&isAnim, CART_ANIM_usNodeAnimIndices, &nodeAnimIndicesSize);
	allocSize += nodeAnimIndicesSize;

	pbInitialOrients = CIndexedSet__GetBlockAndSize(&isAnim, CART_ANIM_usInitialOrients, &initialOrientsSize);
	allocSize += initialOrientsSize;

	// translation sets
	pbTranslationSets = CIndexedSet__GetBlock(&isAnim, CART_ANIM_usTranslationSets);
	CUnindexedSet__ConstructFromRawData(&usTranslationSets, pbTranslationSets, FALSE);
	nTranslationSets = CUnindexedSet__GetBlockCount(&usTranslationSets);
	allocSize += FixAlignment(US_TOTAL_SIZE(US_TOTAL_SIZE(sizeof(CPosFrame), nFrames), nTranslationSets), IS_ALIGNMENT);

	// rotation sets
	pbRotationSets = CIndexedSet__GetBlock(&isAnim, CART_ANIM_usRotationSets);
	CUnindexedSet__ConstructFromRawData(&usRotationSets, pbRotationSets, FALSE);
	nRotationSets = CUnindexedSet__GetBlockCount(&usRotationSets);
	allocSize += FixAlignment(US_TOTAL_SIZE(US_TOTAL_SIZE(sizeof(CRotFrame), nFrames), nRotationSets), IS_ALIGNMENT);


	// transition table
	pbTransitionTable = CIndexedSet__GetBlockAndSize(&isAnim, CART_ANIM_isTransitionTable, &transitionTableSize);
	allocSize += transitionTableSize;

	pbEvents = CIndexedSet__GetBlockAndSize(&isAnim, CART_ANIM_usEvents, &eventsSize);
	allocSize += eventsSize;

	// allocate scratch area in animation block if global scratch area is too small
	scratchAreaSize = nFrames*4*sizeof(short);
	if (scratchAreaSize <= scratch_area_size)
		pScratchArea = scratch_area;
	else
		allocSize += scratchAreaSize;

	// allocate block for decompressed data
	pBytes = CCacheEntry__AllocDataForReplacement(pThis->m_asCurrent.m_pceAnim, allocSize);
	if (pBytes)
	{
		if (scratchAreaSize > scratch_area_size)
			pScratchArea = pBytes + allocSize - scratchAreaSize;
		
		// isNewAnim
		pbNewAnim = pBytes;
		CIndexedSet__ConstructFromRawData(&isNewAnim, pbNewAnim, FALSE);

		// isNewAnim index
		offset = IS_INDEX_SIZE(CART_ANIM_nItems);
		CIndexedSet__SetBlockCount(&isNewAnim, 0);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);

		// usNewYRots
		pbNewYRots = pBytes + offset;
		memcpy(pbNewYRots, pbYRots, yRotsSize);

		offset += yRotsSize;
		CIndexedSet__SetBlockCount(&isNewAnim, 1);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);

		// usNewNodeAnimIndices
		pbNewNodeAnimIndices = pBytes + offset;
		memcpy(pbNewNodeAnimIndices, pbNodeAnimIndices, nodeAnimIndicesSize);

		offset += nodeAnimIndicesSize;
		CIndexedSet__SetBlockCount(&isNewAnim, 2);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);

		// usNewInitialOrients
		pbNewInitialOrients = pBytes + offset;
		memcpy(pbNewInitialOrients, pbInitialOrients, initialOrientsSize);

		offset += initialOrientsSize;
		CIndexedSet__SetBlockCount(&isNewAnim, 3);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);


		// usNewTranslationSets
		pbNewTranslationSets = pBytes + offset;
		CUnindexedSet__ConstructWithAllocatedBlock(&usNewTranslationSets, pbNewTranslationSets,
																 US_TOTAL_SIZE(sizeof(CPosFrame), nFrames), nTranslationSets,
																 FALSE);

		translationSetsOffset = US_HEADER_SIZE;
		newTranslationSetSize = US_HEADER_SIZE + nFrames*sizeof(CPosFrame);

		for (cTranslationSet=0; cTranslationSet<nTranslationSets; cTranslationSet++)
		{
			// usTranslationSet
			pbTranslationSet = CUnindexedSet__GetBlock(&usTranslationSets, cTranslationSet);
			pbNewTranslationSet = pBytes + offset + translationSetsOffset;

			CUnindexedSet__ConstructWithAllocatedBlock(&usTranslationSet, pbNewTranslationSet,
																	 sizeof(CPosFrame), nFrames,
																	 FALSE);

			posFrames = (CPosFrame*) CUnindexedSet__GetBasePtr(&usTranslationSet);

			pTransOffset = (CTranslationOffset*) pbTranslationSet;
			if (pTransOffset->m_Compressed)
			{
				rawBuffers[0] = (short*) (pScratchArea + 0*sizeof(short)*nFrames);
				rawBuffers[1] = (short*) (pScratchArea + 1*sizeof(short)*nFrames);
				rawBuffers[2] = (short*) (pScratchArea + 2*sizeof(short)*nFrames);

				adpcmDecode(rawBuffers, ((BYTE*) pbTranslationSet) + sizeof(CTranslationOffset), 3);

				for (cFrame=0; cFrame<nFrames; cFrame++)
				{
					pPosFrame = &posFrames[cFrame];

					pPosFrame->m_vPos.x = INT162FLOAT(rawBuffers[0][cFrame], pTransOffset->m_Scale) + pTransOffset->m_vOffset.x;
					pPosFrame->m_vPos.y = INT162FLOAT(rawBuffers[1][cFrame], pTransOffset->m_Scale) + pTransOffset->m_vOffset.y;
					pPosFrame->m_vPos.z = INT162FLOAT(rawBuffers[2][cFrame], pTransOffset->m_Scale) + pTransOffset->m_vOffset.z;
				}
			}
			else
			{
				memcpy(posFrames, ((BYTE*) pbTranslationSet) + sizeof(CTranslationOffset), nFrames*sizeof(CPosFrame));
			}

			translationSetsOffset += newTranslationSetSize;
			CUnindexedSet__Destruct(&usTranslationSet);
		}

		offset += translationSetsOffset;
		CIndexedSet__SetBlockCount(&isNewAnim, 4);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);


		// usNewRotationSets
		pbNewRotationSets = pBytes + offset;
		CUnindexedSet__ConstructWithAllocatedBlock(&usNewRotationSets, pbNewRotationSets,
																 US_TOTAL_SIZE(sizeof(CRotFrame), nFrames), nRotationSets,
																 FALSE);

		rotationSetsOffset = US_HEADER_SIZE;
		newRotationSetSize = US_HEADER_SIZE + nFrames*sizeof(CRotFrame);

#define COMPRESS_ANIM
#ifdef COMPRESS_ANIM
		for (cRotationSet=0; cRotationSet<nRotationSets; cRotationSet++)
		{
			// usRotationSet
			pbRotationSet = CUnindexedSet__GetBlock(&usRotationSets, cRotationSet);
			pbNewRotationSet = pBytes + offset + rotationSetsOffset;

			rawBuffers[0] = (short*) (pScratchArea + 0*sizeof(short)*nFrames);
			rawBuffers[1] = (short*) (pScratchArea + 1*sizeof(short)*nFrames);
			rawBuffers[2] = (short*) (pScratchArea + 2*sizeof(short)*nFrames);
			rawBuffers[3] = (short*) (pScratchArea + 3*sizeof(short)*nFrames);

			adpcmDecode(rawBuffers, (BYTE*) pbRotationSet, 4);

			CUnindexedSet__ConstructWithAllocatedBlock(&usRotationSet, pbNewRotationSet,
																	 sizeof(CRotFrame), nFrames,
																	 FALSE);

			rotFrames = (CRotFrame*) CUnindexedSet__GetBasePtr(&usRotationSet);

			for (cFrame=0; cFrame<nFrames; cFrame++)
			{
				pRotFrame = &rotFrames[cFrame];

				//pRotFrame->m_qRot.x = INT162FLOAT(rawBuffers[0][cFrame], 1);
				//pRotFrame->m_qRot.y = INT162FLOAT(rawBuffers[1][cFrame], 1);
				//pRotFrame->m_qRot.z = INT162FLOAT(rawBuffers[2][cFrame], 1);
				//pRotFrame->m_qRot.t = INT162FLOAT(rawBuffers[3][cFrame], 1);
				
				//CQuatern__Normalize(&pRotFrame->m_qRot);

				pRotFrame->x = rawBuffers[0][cFrame];
				pRotFrame->y = rawBuffers[1][cFrame];
				pRotFrame->z = rawBuffers[2][cFrame];
				pRotFrame->t = rawBuffers[3][cFrame];
			}

			rotationSetsOffset += newRotationSetSize;

			CUnindexedSet__Destruct(&usRotationSet);
		}
#else
		for (cRotationSet=0; cRotationSet<nRotationSets; cRotationSet++)
		{
			// usRotationSet
			pbRotationSet = CUnindexedSet__GetBlock(&usRotationSets, cRotationSet);
			pbNewRotationSet = pBytes + offset + rotationSetsOffset;
			memcpy(pbNewRotationSet, pbRotationSet, newRotationSetSize);

			rotationSetsOffset += newRotationSetSize;
		}
#endif

		offset += rotationSetsOffset;
		CIndexedSet__SetBlockCount(&isNewAnim, 5);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);


		// isNewTransitionTable
		pbNewTransitionTable = pBytes + offset;
		memcpy(pbNewTransitionTable, pbTransitionTable, transitionTableSize);

		offset += transitionTableSize;
		CIndexedSet__SetBlockCount(&isNewAnim, 6);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);

		// usNewEvents
		CUnindexedSet__ConstructFromRawData(&usEvents, pbEvents, FALSE);

		pbNewEvents = pBytes + offset;
		CUnindexedSet__DeinterleaveCopy(&usEvents, pbNewEvents, FALSE);

		offset += eventsSize;
		CIndexedSet__SetBlockCount(&isNewAnim, 7);
		CIndexedSet__SetNextOffset(&isNewAnim, offset);

		
		CIndexedSet__Destruct(&isNewAnim);
		CIndexedSet__Destruct(&isNewTranslationSets);
		CIndexedSet__Destruct(&isNewRotationSets);
		CUnindexedSet__Destruct(&usEvents);
	}
	else
	{
		TRACE("CGameObjectInstance: WARNING - could not allocate %x bytes for animation.\r\n", allocSize);
	}

	CCacheEntry__DeallocAndReplaceData(pThis->m_asCurrent.m_pceAnim, pBytes, allocSize);

#ifdef IG_DEBUG
	endTime = osGetTime();
	diffTime = endTime - startTime;
#endif
	TRACE("...Finished animation decompression in %8.2f msec.\r\n",
			((float)(CYCLES_TO_NSEC(diffTime)))/1000000);

	CIndexedSet__Destruct(&isAnim);
	CUnindexedSet__Destruct(&usYRots);
	CUnindexedSet__Destruct(&usTranslationSets);
	CUnindexedSet__Destruct(&usRotationSets);
}
#endif
