/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueCombinationDefinition.h"

#include "GOrgueCoupler.h"
#include "GOrgueDivisionalCoupler.h"
#include "GOrgueManual.h"
#include "GOrgueStop.h"
#include "GOrgueSwitch.h"
#include "GOrgueTremulant.h"
#include "GrandOrgueFile.h"

GOrgueCombinationDefinition::GOrgueCombinationDefinition(GrandOrgueFile* organfile) :
	m_organfile(organfile),
	m_Content(0)
{
}

GOrgueCombinationDefinition::~GOrgueCombinationDefinition()
{
}

void GOrgueCombinationDefinition::AddGeneral(GOrgueDrawstop* control, CombinationType type, int manual, unsigned index)
{
	Add(control, type, manual, index, control->GetStoreGeneral());
}

void GOrgueCombinationDefinition::AddDivisional(GOrgueDrawstop* control, CombinationType type, int manual, unsigned index)
{
	Add(control, type, manual, index, control->GetStoreDivisional());
}

void GOrgueCombinationDefinition::Add(GOrgueDrawstop* control, CombinationType type, int manual, unsigned index, bool store_unconditional)
{
	if (control->IsReadOnly())
		return;
	CombinationSlot def;
	def.type = type;
	def.manual = manual;
	def.index = index;
	def.store_unconditional = store_unconditional;
	def.control = control;
	if (manual == -1)
		def.group = wxEmptyString;
	else
		def.group = m_organfile->GetManual(manual)->GetName();
	m_Content.push_back(def);
}

void GOrgueCombinationDefinition::InitGeneral()
{
	m_Content.resize(0);

	for (unsigned j = m_organfile->GetFirstManualIndex(); j <= m_organfile->GetManualAndPedalCount(); j++)
	{
		for (unsigned i = 0; i < m_organfile->GetManual(j)->GetStopCount(); i++)
			AddGeneral(m_organfile->GetManual(j)->GetStop(i), COMBINATION_STOP, j, i + 1);
	}
	
	for (unsigned j = m_organfile->GetFirstManualIndex(); j <= m_organfile->GetManualAndPedalCount(); j++)
	{
		for (unsigned i = 0; i < m_organfile->GetManual(j)->GetCouplerCount(); i++)
			AddGeneral(m_organfile->GetManual(j)->GetCoupler(i), COMBINATION_COUPLER, j, i + 1);
	}

	for (unsigned i = 0; i < m_organfile->GetTremulantCount(); i++)
		AddGeneral(m_organfile->GetTremulant(i), COMBINATION_TREMULANT, -1, i + 1);

	for (unsigned i = 0; i < m_organfile->GetSwitchCount(); i++)
		AddGeneral(m_organfile->GetSwitch(i), COMBINATION_SWITCH, -1, i + 1);

	for (unsigned i = 0; i < m_organfile->GetDivisionalCouplerCount(); i++)
		AddGeneral(m_organfile->GetDivisionalCoupler(i), COMBINATION_DIVISIONALCOUPLER, -1, i + 1);
}

void GOrgueCombinationDefinition::InitDivisional(unsigned manual_number)
{
	GOrgueManual* associatedManual = m_organfile->GetManual(manual_number);
	m_Content.resize(0);

	for (unsigned i = 0; i < associatedManual->GetStopCount(); i++)
		AddDivisional(associatedManual->GetStop(i), COMBINATION_STOP, manual_number, i + 1);

	for (unsigned i = 0; i < associatedManual->GetCouplerCount(); i++)
		AddDivisional(associatedManual->GetCoupler(i), COMBINATION_COUPLER, manual_number, i + 1);

	for (unsigned i = 0; i < associatedManual->GetTremulantCount(); i++)
		AddDivisional(associatedManual->GetTremulant(i), COMBINATION_TREMULANT, manual_number, i + 1);

	for (unsigned i = 0; i < associatedManual->GetSwitchCount(); i++)
		AddDivisional(associatedManual->GetSwitch(i), COMBINATION_SWITCH, manual_number, i + 1);
}

const std::vector<GOrgueCombinationDefinition::CombinationSlot>& GOrgueCombinationDefinition::GetCombinationElements()
{
	return m_Content;
}

int GOrgueCombinationDefinition::findEntry(CombinationType type, int manual, unsigned index)
{
	for(unsigned i = 0; i < m_Content.size(); i++)
	{
		if (m_Content[i].type == type && m_Content[i].manual == manual && m_Content[i].index == index)
			return i;
	}
	return -1;
}
