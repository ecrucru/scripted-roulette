
/*  Scripted Roulette - version 0.1
 *  Copyright (C) 2015-2016, http://scripted-roulette.sourceforge.net
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

#include "frame.h"
#include "http_prng.h"


//------------------------------------------

/**
 * \class wxRouletteApp
 * \brief Launcher for the application
 */
class wxRouletteApp: public wxApp
{
    private:
        wxRouletteFrame *m_mainframe;
    #if roulette_compile_locale == 1
        wxLocale        m_locale;           //To be destroyed at the end
    #endif

    public:
        /** Initializes the application. */
        virtual bool OnInit();
};


//------------------------------------------

    DECLARE_APP(wxRouletteApp)
    IMPLEMENT_APP(wxRouletteApp)

    bool wxRouletteApp::OnInit()
    {
        wxCmdLineParser cmdline;
        wxString filename;
        bool autostart;

        //-- Super
    #ifndef wxUSE_CMDLINE_PARSER
        if (!wxApp::OnInit())
            return false;
    #endif

        //-- Processes the command line
        cmdline.SetDesc(roulette_cmdline);
        cmdline.SetCmdLine(wxTheApp->argc, wxTheApp->argv);
        filename.Empty();
        autostart = false;
        if (cmdline.Parse(false) == 0)
        {
        #if roulette_compile_locale == 1
            ManageLocale(&cmdline, &m_locale);                  //i18n
        #endif
            if (cmdline.Found(wxT("v")))
            {
                wxRouletteFrame::About();
                return false;
            }
            if (cmdline.Found(wxT("s")))
                autostart = true;
            if (cmdline.GetParamCount() == 1)
            {
                //- Gets the filename
                filename = cmdline.GetParam(0);
                if (wxRouletteHelper::GetFileExtension(filename).IsEmpty())
                    filename.Append(roulette_file_ext_dot);
            }
        }

        //-- Loads the application
        m_mainframe = new wxRouletteFrame(NULL, wxID_ANY, filename.IsEmpty());
        if (!autostart)
            m_mainframe->Raise();
        SetTopWindow(m_mainframe);
        m_mainframe->Show();

        //-- Opens the default file if needed
        if (!filename.IsEmpty())
            if (m_mainframe->OpenFile(filename))
                if (autostart)
                    m_mainframe->Execute();

        //-- Final
        return true;
    }

#endif
