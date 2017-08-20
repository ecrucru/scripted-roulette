
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

#ifdef roulette_h

case roulette_inst_restart_id:
{
    //Checks the parameters
    if (parser.HasParameters())
    {
        if ((parser.CountParameters() == 1) && (parser.Command == wxT("sequence")))
        {
            i = 0;
            if_depth = 0;
        #ifndef _CONSOLE
            wxSafeYield(m_frame);           //Give the user the possibility to stop. It causes a lower performance but it will avoid infinite loops
        #endif
            goto current_statement;
        }
        else
            if (!parser.NoWarning)
                LogWarning(wxString::Format(_("Incorrect parameters for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
    }

    //Restarts the script
    m_restart_requested = true;
#ifndef _CONSOLE
    wxSafeYield(m_frame);                   //Give the user the possibility to stop
#endif
    continue;
}

#endif
