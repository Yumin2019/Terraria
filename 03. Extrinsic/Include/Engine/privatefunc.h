#pragma once

#include "Component.h"

void ComponentSetChange(CComponent* pCom, bool bChange)
{
	pCom->m_bChanged = bChange;
}