/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2019 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#ifndef GORGUECACHECLEANER_H
#define GORGUECACHECLEANER_H

class GOrgueSettings;
class wxArrayString;

class GOrgueCacheCleaner
{
private:
	GOrgueSettings& m_settings;

	wxArrayString GetOrganIDList();
	wxArrayString GetArchiveIDList();
	
public:
	GOrgueCacheCleaner(GOrgueSettings& settings);
	~GOrgueCacheCleaner();

	void Cleanup();
};

#endif
