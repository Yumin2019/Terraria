#include "stdafx.h"
#include "Component.h"




CComponent::CComponent(COMPONENT_TYPE eType) :
	m_eType(eType),
	m_bChanged(true),
	m_bEnable(true)
{
}

CComponent::~CComponent()
{
}
