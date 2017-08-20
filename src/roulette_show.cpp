
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

case roulette_inst_show_id:
{
    //Checks the parameters
    if (parser.CountParameters() != 1)
    {
        LogError(wxString::Format(_("Only 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Processes the command
    buffer = parser.CommandList.Item(0);
    if (buffer == wxT("memory"))
    {
        DoDumpVariables(parser.NoWarning);
        continue;
    }
    if (buffer == wxT("script"))
    {
        DoDumpScript();
        continue;
    }
    if (buffer == wxT("sequences"))                //No interest for users. It is more a debugging feature
    {
        DoDumpSequences();
        continue;
    }
    if ((buffer == wxT("table")) || (buffer == wxT("bets")))
    {
        m_table.ShowTable();
        continue;
    }
    if (buffer == wxT("money"))
    {
        DoShowMoney();
        continue;
    }
    if (buffer == wxT("layout"))
    {
        m_table.ShowLayout();
        continue;
    }
    if (buffer == wxT("algorithm"))
    {
        m_table.ShowAlgorithm();
        continue;
    }
    if (buffer == wxT("distribution"))
    {
        m_table.ShowDistribution();
        continue;
    }
    if (buffer == wxT("seed"))
    {
        m_table.ShowSeeds();
        continue;
    }

    //Else
    LogError(wxString::Format(_("Unsupported command '%s' for the instruction '%s'."), buffer.Upper().uniCStr(), parser.Instruction.uniCStr()));
    script[i].Empty();
    continue;
}

#endif
