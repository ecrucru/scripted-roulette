
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

#ifdef main_cpp

int main(int argc, char* argv[])
{
#if wxCHECK_VERSION(2,9,1)
    wxDISABLE_DEBUG_SUPPORT()
#endif

    wxRoulette roulette;
    wxCmdLineParser cmdline(roulette_cmdline, argc, argv);
    wxLocale m_locale;
    wxString filename;
    size_t i;

    //-- Handler for some extensions
#if roulette_compile_extended == 1
    wxImage::AddHandler(new wxBMPHandler);                        //Not required in GUI mode
#endif

    //-- Parses the command line to file the files to process
    wxMessageOutput::Set(new wxMessageOutputStderr);            //cf. https://forums.wxwidgets.org/viewtopic.php?t=21840
    if (cmdline.Parse(false) != 0)
    {
        roulette.LogError(_("Unable to interpret the command line. Use the option -h for help."));
        return 0;
    }
#if roulette_compile_locale == 1
    ManageLocale(&cmdline, &m_locale);                          //i18n
#endif
    if (cmdline.Found(wxT("v")))
    {
        //- About Scripted Roulette
        roulette.LogMessage(wxString::Format(wxT("%s %s"), roulette_about_name, roulette_about_version));
        roulette.LogMessage(roulette_about_website);
        roulette.LogMessage(wxT(" "));

        //- About the compilation
        roulette.LogMessage(wxString::Format(wxT("wxWidgets : %d.%d.%d"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER));
        roulette.LogMessage(wxString::Format(_("Compiled with : %s"), wxRouletteHelper::GetCompiler()));
    #ifdef __WXDEBUG__
        roulette.LogMessage(_("Compilation : debug"));
    #else
        roulette.LogMessage(_("Compilation : release"));
    #endif
    #ifdef wxUSE_UNICODE
        roulette.LogMessage(_("Unicode : yes"));
    #else
        roulette.LogMessage(_("Unicode : no"));
    #endif
    #if roulette_compile_extended == 1
        roulette.LogMessage(_("Extended features : yes"));
    #else
        roulette.LogMessage(_("Extended features : no"));
    #endif
    #if roulette_compile_locale == 1
        roulette.LogMessage(_("Translations : yes"));
    #else
        roulette.LogMessage(_("Translations : no"));
    #endif
    }
    if (cmdline.Found(wxT("h")))
    {
        cmdline.Usage();
        return 0;
    }

    //-- Warning about the instability
#if roulette_compile_beta == 1
    roulette.LogMessage(wxT("Warning : this version is unstable. Please report any bug you may face."));
#endif

    //-- Processing the files
    for (i=0 ; i<cmdline.GetParamCount() ; i++)
    {
        //- Gets the filename
        filename = cmdline.GetParam(i);
        if (wxRouletteHelper::GetFileExtension(filename).IsEmpty())
            filename.Append(roulette_file_ext_dot);

        //- Processes the script
        roulette.LogMessage(wxString::Format(_(">> Start of processing for '%s'..."), filename.uniCStr()));
        roulette.StartFromFile(filename, true);
        roulette.LogMessage(wxString::Format(_("<< End of processing for '%s'"), filename.uniCStr()));
        roulette.LogMessage(wxT("\n"));
    }
    return 0;
}

#endif
