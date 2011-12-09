/*
 * GrandOrgue - free pipe organ simulator
 *
 * Copyright 2006 Milan Digital Audio LLC
 * Copyright 2009-2011 GrandOrgue contributors (see AUTHORS)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * MA 02111-1307, USA.
 */


#include "IniFileConfig.h"
#include <wx/fileconf.h>

IniFileConfig::IniFileConfig(wxFileConfig& odf_ini_file) :
	m_ODFIni (odf_ini_file)
{

}

wxString IniFileConfig::ReadString(wxString group, wxString key, unsigned nmax, bool required, wxString defaultValue)
{
	wxString value;

	m_ODFIni.SetPath(wxT("/"));
	if (!m_ODFIni.HasGroup(group))
	{
		if (group.length() >= 6 && !group.Mid(0, 6).CmpNoCase(wxT("Setter")))	// Setter groups aren't required.
			return defaultValue;
		if (group.length() >= 5 && !group.Mid(0, 5).CmpNoCase(wxT("Panel")))
			if (group.length() >= 14 && !group.Mid(8, 6).CmpNoCase(wxT("Setter")))	// Setter groups aren't required.
				return defaultValue;

		if (group.length() >= 12 && !group.Mid(0, 12).CmpNoCase(wxT("FrameGeneral")))	// FrameGeneral groups aren't required.
			return defaultValue;

		if (required)
		{
			wxString error;
			error.Printf(_("Missing required group '/%s'"), group.c_str());
			throw error;
		}
	}

	m_ODFIni.SetPath(wxT("/") + group);

	if (!m_ODFIni.Read(key, &value))
	{
		if (required)
		{
			if (group.length() >= 6 && !group.Mid(0, 6).CmpNoCase(wxT("Setter")))	// Setter groups aren't required.
				return defaultValue;
			if (group.length() >= 5 && !group.Mid(0, 5).CmpNoCase(wxT("Panel")))
				if (group.length() >= 14 && !group.Mid(8, 6).CmpNoCase(wxT("Setter")))	// Setter groups aren't required.
					return defaultValue;

			wxString error;
			error.Printf(_("Missing required value '/%s/%s'"), group.c_str(), key.c_str());
			throw error;
		}
		else
			return defaultValue;
	}

	value.Trim();
	if (value.length() > nmax)
	{
		wxString error;
		error.Printf(_("Value too long: '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
		throw error;
	}

	return value;
}

bool IniFileConfig::ReadBoolean(wxString group, wxString key, bool required)
{
	return ReadBoolean(group, key, required, false);
}

bool IniFileConfig::ReadBoolean(wxString group, wxString key, bool required, bool defaultValue)
{
	wxString value = ReadString(group, key, 255, required, defaultValue ? wxT("Y") : wxT("N"));
	value.MakeUpper();
	value.Trim();

	if (value.Length() && value[0] == wxT('Y'))
		return true;
	else if (value.Length() && value[0] == wxT('N'))
		return false;

	wxString error;
	error.Printf(_("Invalid boolean value '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
	throw error;
}

wxColour IniFileConfig::ReadColor(wxString group, wxString key, bool required)
{
	return ReadColor(group, key, required, wxT("BLACK"));
}

wxColour IniFileConfig::ReadColor(wxString group, wxString key, bool required, wxString defaultValue)
{
	wxString value = ReadString(group, key, 255, required, defaultValue);

	value.MakeUpper();
	value.Trim();

	if (value == wxT("BLACK"))
		return wxColour(0x00, 0x00, 0x00);
	else if (value == wxT("BLUE"))
		return wxColour(0x00, 0x00, 0xFF);
	else if (value == wxT("DARK BLUE"))
		return wxColour(0x00, 0x00, 0x80);
	else if (value == wxT("GREEN"))
		return wxColour(0x00, 0xFF, 0x00);
	else if (value == wxT("DARK GREEN"))
		return wxColour(0x00, 0x80, 0x00);
	else if (value == wxT("CYAN"))
		return wxColour(0x00, 0xFF, 0xFF);
	else if (value == wxT("DARK CYAN"))
		return wxColour(0x00, 0x80, 0x80);
	else if (value == wxT("RED"))
		return wxColour(0xFF, 0x00, 0x00);
	else if (value == wxT("DARK RED"))
		return wxColour(0x80, 0x00, 0x00);
	else if (value == wxT("MAGENTA"))
		return wxColour(0xFF, 0x00, 0xFF);
	else if (value == wxT("DARK MAGENTA"))
		return wxColour(0x80, 0x00, 0x80);
	else if (value == wxT("YELLOW"))
		return wxColour(0xFF, 0xFF, 0x00);
	else if (value == wxT("DARK YELLOW"))
		return wxColour(0x80, 0x80, 0x00);
	else if (value == wxT("LIGHT GREY"))
		return wxColour(0xC0, 0xC0, 0xC0);
	else if (value == wxT("DARK GREY"))
		return wxColour(0x80, 0x80, 0x80);
	else if (value == wxT("WHITE"))
		return wxColour(0xFF, 0xFF, 0xFF);

	wxColour colour;
	if (colour.Set(value))
		return colour;
	
	wxString error;
	error.Printf(_("Invalid color '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
	throw error;
}

wxString IniFileConfig::ReadString(wxString group, wxString key, unsigned nmax, bool required)
{
	return ReadString(group, key, nmax, required, wxT(""));
}

int IniFileConfig::ReadInteger(wxString group, wxString key, int nmin, int nmax, bool required)
{
	return ReadInteger(group, key, nmin, nmax, required, nmin);
}

int IniFileConfig::ReadInteger(wxString group, wxString key, int nmin, int nmax, bool required, int defaultValue)
{
	wxString value = ReadString(group, key, 255, required, wxString::Format(wxT("%d"), defaultValue));

	if (value.IsEmpty())
	{
		if (required)
		{
			wxString error;
			error.Printf(_("Missing required value '/%s/%s'"), group.c_str(), key.c_str());
			throw error;
		}
		else
			return defaultValue;
	}

	if (!::wxIsdigit(value[0]) && value[0] != wxT('+') && value[0] != wxT('-') && value.CmpNoCase(wxT("none")))
	{
		wxString error;
		error.Printf(_("Invalid integer value '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
		throw error;
	}

	int retval = wxAtoi(value);

	if (nmin <= retval && retval <= nmax)
		return retval;

	wxString error;
	error.Printf(_("Out of range value '/%s/%s': %d"), group.c_str(), key.c_str(), retval);
	throw error;
}

int IniFileConfig::ReadLong(wxString group, wxString key, int nmin, int nmax, bool required)
{
	return ReadInteger(group, key, nmin, nmax, required);
}

int IniFileConfig::ReadLong(wxString group, wxString key, int nmin, int nmax, bool required, int defaultValue)
{
	return ReadInteger(group, key, nmin, nmax, required, defaultValue);
}

double IniFileConfig::ReadFloat(wxString group, wxString key, double nmin, double nmax, bool required)
{
	return ReadFloat(group, key, nmin, nmax, required, nmin);
}

double IniFileConfig::ReadFloat(wxString group, wxString key, double nmin, double nmax, bool required, double defaultValue)
{
	wxString value = ReadString(group, key, 255, required, wxString::Format(wxT("%e"), defaultValue));

	if (value.IsEmpty())
	{
		if (required)
		{
			wxString error;
			error.Printf(_("Missing required value '/%s/%s'"), group.c_str(), key.c_str());
			throw error;
		}
		else
			return defaultValue;
	}

	if (!::wxIsdigit(value[0]) && value[0] != wxT('+') && value[0] != wxT('-') && value.CmpNoCase(wxT("none")))
	{
		wxString error;
		error.Printf(_("Invalid float value '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
		throw error;
	}

	double retval = wxAtof(value);

	if (nmin <= retval && retval <= nmax)
		return retval;

	wxString error;
	error.Printf(_("Out of range value '/%s/%s': %d"), group.c_str(), key.c_str(), retval);
	throw error;
}

unsigned IniFileConfig::ReadSize(wxString group, wxString key, unsigned type, bool required)
{
	return ReadSize(group, key, type, required, wxT("SMALL"));
}

unsigned IniFileConfig::ReadSize(wxString group, wxString key, unsigned type, bool required, wxString defaultValue)
{
	static const int sizes[2][4] = {{800, 1007, 1263, 1583}, {500, 663, 855, 1095}};

	wxString value = ReadString(group, key, 255, required, defaultValue);

	value.MakeUpper();
	value.Trim();

	if (value.IsEmpty() && !required)
		value = defaultValue;

	if (value == wxT("SMALL"))
		return sizes[type][0];
	else if (value == wxT("MEDIUM"))
		return sizes[type][1];
	else if (value == wxT("MEDIUM LARGE"))
		return sizes[type][2];
	else if (value == wxT("LARGE"))
		return sizes[type][3];

	if (::wxIsdigit(value[0]))
	{
		int size = wxAtoi(value);
		if (100 <= size && size <= 4000)
			return size;
	}
	
	wxString error;
	error.Printf(_("Invalid size '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
	throw error;
}

unsigned IniFileConfig::ReadFontSize(wxString group, wxString key, bool required)
{
	return ReadFontSize(group, key, required, wxT("NORMAL"));
}

unsigned IniFileConfig::ReadFontSize(wxString group, wxString key, bool required, wxString defaultValue)
{
	wxString value = ReadString(group, key, 255, required, defaultValue);

	value.MakeUpper();
	value.Trim();

	if (value.IsEmpty() && !required)
		value = defaultValue;

	if (value == wxT("SMALL"))
		return 6;
	else if (value == wxT("NORMAL"))
		return 7;
	else if (value == wxT("LARGE"))
		return 10;

	int val = wxAtoi(value);
	if (val > 1)
		return val;

	wxString error;
	error.Printf(_("Invalid font size '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
	throw error;
}

int IniFileConfig::ReadEnum(wxString group, wxString key, const struct IniFileEnumEntry* entry, unsigned count, bool required, int defaultValue)
{
	wxString value = ReadString(group, key, 255, required);
	for (unsigned i = 0; i < count; i++)
		if (entry[i].name == value)
			return entry[i].value;
	if (required || !value.IsEmpty())
	{
		wxString error;
		error.Printf(_("Invalid enum value '/%s/%s': %s"), group.c_str(), key.c_str(), value.c_str());
		throw error;
	}
	return defaultValue;
}

int IniFileConfig::ReadEnum(wxString group, wxString key, const struct IniFileEnumEntry* entry, unsigned count, bool required)
{
	return ReadEnum(group, key, entry, count, required, entry[0].value);
}


void IniFileConfig::SaveHelper(bool prefix, wxString group, wxString key, wxString value)
{
	wxString str = group + wxT('/') + key;
	if (m_ODFIni.Read(str) != value)
	{
		if (prefix)
		{
			m_ODFIni.Write(wxT('_') + str, value);
		}
		else
			m_ODFIni.Write(str, value);
	}
}

void IniFileConfig::SaveHelper( bool prefix, wxString group, wxString key, int value, bool sign, bool force)
{
	wxString str;
	if (force)
		str.Printf(wxT("%03d"), value);
	else if (sign)
	{
		if (value >= 0)
			str.Printf(wxT("+%03d"), value);
		else
			str.Printf(wxT("-%03d"), -value);
	}
	else
		str.Printf(wxT("%d"), value);
	SaveHelper(prefix, group, key, str);
}

void IniFileConfig::SaveHelper( bool prefix, wxString group, wxString key, float value)
{
	wxString str = wxString::Format(wxT("%f"), value);
	SaveHelper(prefix, group, key, str);
}

void IniFileConfig::SaveHelper(bool prefix, wxString group, wxString key, int value, const struct IniFileEnumEntry* entry, unsigned count)
{
	for (unsigned i = 0; i < count; i++)
		if (entry[i].value == value)
		{
			SaveHelper(prefix, group, key, entry[i].name);
			return;
		}
	wxLogError(_("Invalid enum value for /%s/%s: %d"), group.c_str(), key.c_str(), value);
}
