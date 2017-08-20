
/*  Scripted Roulette - version 0.2.1
 *  Copyright (C) 2015-2017, http://scripted-roulette.sourceforge.net
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/cmdline.h"
#include "wx/gdicmn.h"

#define main_cpp

#include "defs.h"
#include "roulette.h"


//------------------------------------------

static const wxCmdLineEntryDesc roulette_cmdline[] =
{
    { wxCMD_LINE_SWITCH, wxT28("h"), wxT28("help"), wxT28_("Display the help"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
#if roulette_compile_locale == 1
    { wxCMD_LINE_OPTION, wxT28("l"), wxT28("locale"), wxT28_("Force the language (cf. norm ISO 3166-1)"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
#ifndef _CONSOLE
    { wxCMD_LINE_SWITCH, wxT28("s"), NULL, wxT28_("Execute the script after it is loaded"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
#endif
    { wxCMD_LINE_SWITCH, wxT28("v"), wxT28("version"), wxT28_("Show the version"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
#ifdef _CONSOLE
    { wxCMD_LINE_PARAM, NULL, NULL, wxT28_("Script(s) to execute"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL|wxCMD_LINE_PARAM_MULTIPLE },
#else
    { wxCMD_LINE_PARAM, NULL, NULL, wxT28_("Script(s) to execute"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
    { wxCMD_LINE_NONE }
};


//------------------------------------------

#if roulette_compile_locale == 1
bool ManageLocale(wxCmdLineParser *pParser, wxLocale *pLocale)
{
    wxString buffer;
    const wxLanguageInfo *langinfo;
    long language;

    //-- Checks
    if ((pParser == NULL) || (pLocale == NULL))
        return false;

    //-- Initializes
    buffer.Empty();
    language = wxLANGUAGE_DEFAULT;

    //-- Gets the forced locale
    if (pParser->Found(wxT("l"), &buffer))
    {
        langinfo = pLocale->FindLanguageInfo(buffer);
        if ((langinfo != NULL) /*&& pLocale->IsAvailable(langinfo->Language)*/)     //The availability is not checked because on Linux, even if the locale is not enabled globally, the translations can still be found
            language = langinfo->Language;
        else
            wxLogError(wxString::Format(wxT("The locale '%s' doesn't exist."), buffer.uniCStr()));
    }

    //-- Applies the locale
#ifdef __WXGTK__
    pLocale->AddCatalogLookupPathPrefix(wxT("/usr"));
    pLocale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
    pLocale->AddCatalogLookupPathPrefix(wxStandardPaths::Get().GetInstallPrefix());
#endif
    pLocale->AddCatalogLookupPathPrefix(wxRouletteHelper::GetApplicationPath() + wxT("locales"));
    if ((language == wxLANGUAGE_DEFAULT) || (language == wxLANGUAGE_UNKNOWN))
        pLocale->Init();
    else
        pLocale->Init(language);                        //With wxGTK, if the locale cannot be loaded, execute "dpkg-reconfigure locales" to activate it also at the level of your system
    pLocale->AddCatalog(wxT("scripted-roulette"));      //Name of the MO files
    return true;
}
#endif


//------------------------------------------

#ifdef _CONSOLE
    #include "main_console.cpp"
#else
    #include "main_win.cpp"
#endif
