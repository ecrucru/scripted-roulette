
/*  Scripted Roulette - version 0.2
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

#ifdef roulette_h

case roulette_inst_pause_id:
{
    //Checks the parameters
    if (parser.HasParameters())
        if (!parser.NoWarning)
            LogWarning(wxString::Format(_("No argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));

    //Action
    #ifdef _CONSOLE
        #ifdef __WXMSW__
            system("pause");
        #else
            LogMessage(_("Press Return to continue..."));
            getchar();
        #endif
    #else
        wxMessageDialog(NULL, _("Press OK to continue..."), _("Pause"), wxOK|wxICON_INFORMATION).ShowModal();
    #endif

    continue;
}

#endif
