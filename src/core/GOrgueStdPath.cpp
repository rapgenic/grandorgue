/*
* Copyright 2006 Milan Digital Audio LLC
* Copyright 2009-2021 GrandOrgue contributors (see AUTHORS)
* License GPL-2.0 or later (https://www.gnu.org/licenses/old-licenses/gpl-2.0.html).
*/

#include "GOrgueStdPath.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>

wxString GOrgueStdPath::GetBaseDir()
{
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	fn = fn.GetPath();
	return fn.GetPath();
}

void GOrgueStdPath::InitLocaleDir()
{
#ifndef __WXMAC__
	wxLocale::AddCatalogLookupPathPrefix(GetBaseDir() + wxFILE_SEP_PATH +  wxT("share") + wxFILE_SEP_PATH + wxT("locale"));
#endif
#if wxCHECK_VERSION(3, 1, 1)
	wxStandardPaths::Get().UseAppInfo(wxStandardPaths::AppInfo_AppName);
	wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout_XDG);
#endif
}

wxString GOrgueStdPath::GetResourceDir()
{
#ifdef __WXMAC__
	return wxStandardPaths::Get().GetResourcesDir();
#else
	return GetBaseDir() + wxFILE_SEP_PATH +  wxT("share") + wxFILE_SEP_PATH + wxT("GrandOrgue");
#endif
}

wxString GOrgueStdPath::GetConfigDir()
{
	return wxStandardPaths::Get().GetUserConfigDir();
}

wxString GOrgueStdPath::GetDocumentDir()
{
	return wxStandardPaths::Get().GetDocumentsDir();
}

wxString GOrgueStdPath::GetCacheDir()
{
#if wxCHECK_VERSION(3, 1, 1)
	return wxStandardPaths::Get().GetUserDir(wxStandardPaths::Dir_Cache);
#else
	return wxStandardPaths::Get().GetUserConfigDir();
#endif
}
