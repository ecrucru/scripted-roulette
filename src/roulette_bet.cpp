
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

#ifdef roulette_h

if (parser.Instruction == roulette_inst_bet)
{
    //Simplifies the parameters
    parser.RemoveParameter(wxT("straight"));        //1
    parser.RemoveParameter(wxT("single"));
    parser.RemoveParameter(wxT("mono"));
    parser.RemoveParameter(wxT("solo"));
    parser.RemoveParameter(wxT("one"));
    parser.RemoveParameter(wxT("split"));           //2
    parser.RemoveParameter(wxT("double"));
    parser.RemoveParameter(wxT("duo"));
    parser.RemoveParameter(wxT("two"));
    parser.RemoveParameter(wxT("street"));          //3
    parser.RemoveParameter(wxT("trio"));
    parser.RemoveParameter(wxT("three"));
    parser.RemoveParameter(wxT("corner"));          //4
    parser.RemoveParameter(wxT("square"));
    parser.RemoveParameter(wxT("four"));
    parser.RemoveParameter(wxT("line"));            //6
    parser.RemoveParameter(wxT("six"));
    parser.RemoveParameter(wxT("dozen"));           //12
    parser.RemoveParameter(wxT("twelve"));
    parser.RemoveParameter(wxT("column"));
    parser.RemoveParameter(wxT("half"));            //18
    parser.RemoveParameter(wxT("eighteen"));
    parser.RemoveParameter(wxT("dozens"));          //24
    parser.RemoveParameter(wxT("columns"));
    parser.RemoveParameter(wxT("twenty-four"));

    //Translates the variables
    if (parser.HasParameters())
    {
        j = parser.CommandList.GetCount();
        do {
            j--;
            buffer = parser.CommandList.Item(j);
            if (buffer.Find(wxT('.')) == wxNOT_FOUND)
                continue;

            /** \todo Enhanced interpretation with '|' */

            //Having a dot is a sign that a variable needs to be converted (example: "BET SINGLE game.landed.number" to play again the last number)
            b = false;
            if (m_engine.SetFormula(buffer))
            {
                m_engine.Compute();
                if (m_engine.GetLastError() == wxECE_NOERROR)
                {
                    parser.CommandList[j] = wxString::Format(wxT("%d"), (unsigned long)m_engine.GetLastResult());
                    b = true;
                }
            }
            if (!b)
                parser.CommandList.RemoveAt(j);
        } while (j > 0);
        parser.RebuildCommandLine();        //To rebuild the inline command since we are not expected to edit CommandList directly
    }

    //Checks
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Bets and updates the statistics
    m_table.Bet(&(parser.CommandList), parser.NoWarning);
    DoUpdateDynamicVariables(parser.NoWarning);
    continue;
}

#endif
