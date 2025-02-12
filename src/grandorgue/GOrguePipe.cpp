/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrguePipe.h"

#include "GOrgueRank.h"
#include "GrandOrgueFile.h"
#include <wx/intl.h>

GOrguePipe::GOrguePipe (GrandOrgueFile* organfile, GOrgueRank* rank, unsigned midi_key_number) :
	m_Velocity(0),
	m_Velocities(1),
	m_organfile(organfile),
	m_Rank(rank),
	m_MidiKeyNumber(midi_key_number)
{
	m_organfile->RegisterPlaybackStateHandler(this);
}

GOrguePipe::~GOrguePipe()
{
}

unsigned GOrguePipe::RegisterReference(GOrguePipe* pipe)
{
	unsigned id = m_Velocities.size();
	m_Velocities.resize(id + 1);
	return id;
}

void GOrguePipe::AbortPlayback()
{
}

void GOrguePipe::PreparePlayback()
{
	m_Velocity = 0;
	for(unsigned i = 0; i < m_Velocities.size(); i++)
		m_Velocities[i] = 0;
}

void GOrguePipe::StartPlayback()
{
}

void GOrguePipe::PrepareRecording()
{
}

void GOrguePipe::SetTemperament(const GOrgueTemperament& temperament)
{
}

void GOrguePipe::Set(unsigned velocity, unsigned referenceID)
{
	if (m_Velocities[referenceID] <= velocity && velocity <= m_Velocity)
	{
		m_Velocities[referenceID] = velocity;
		return;
	}
	unsigned last_velocity = m_Velocity;
	if (velocity >= m_Velocity)
	{
		m_Velocities[referenceID] = velocity;
		m_Velocity = velocity;
	}
	else
	{
		m_Velocities[referenceID] = velocity;
		m_Velocity = m_Velocities[0];
		for(unsigned i = 1; i < m_Velocities.size(); i++)
			if (m_Velocity < m_Velocities[i])
				m_Velocity = m_Velocities[i];
	}

	m_Rank->SendKey(m_MidiKeyNumber, velocity);
	Change(m_Velocity, last_velocity);
}
