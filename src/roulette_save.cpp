
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

case roulette_inst_save_id:
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
    if (buffer == wxT("distribution"))
    {
        buffer = wxT("scripted_roulette_distribution.bmp");
        if (!m_table.SaveDistribution(buffer, 256, false))
            if (!parser.NoWarning)
                LogWarning(wxString::Format(_("The distribution has not been saved into file '%s'."), buffer.uniCStr()));
        continue;
    }

    if (buffer == wxT("hotmap"))
    {
        buffer = wxT("scripted_roulette_hotmap.bmp");
        if (!m_table.SaveDistribution(buffer, 256, true))
            if (!parser.NoWarning)
                LogWarning(wxString::Format(_("The distribution has not been saved into file '%s'."), buffer.uniCStr()));
        continue;
    }

    if (buffer == wxT("history"))
    {
        buffer = wxT("scripted_roulette_history.csv");
        if (!DoSaveSpinHistory(buffer))
        {
            if (!parser.NoWarning)
                LogWarning(wxString::Format(_("The history of the spins has not been saved into the file '%s'."), buffer.uniCStr()));
        }
        else
        {
            if (!parser.NoWarning)
                LogMessage(wxString::Format(_("The history of the spins is saved as '%s'."), buffer.uniCStr()));
        }
        continue;
    }

    if (buffer == wxT("log"))
    {
        buffer = wxT("scripted_roulette_log.csv");
        if (!DoSaveLogHistory(buffer))
        {
            if (!parser.NoWarning)
                LogWarning(wxString::Format(_("The history of the spins has not been saved into the file '%s'."), buffer.uniCStr()));
        }
        else
        {
            if (!parser.NoWarning)
                LogMessage(wxString::Format(_("The history of the spins is saved as '%s'."), buffer.uniCStr()));
        }
        continue;
    }

    //Unrecognized command
    LogError(wxString::Format(_("Unsupported command '%s' for the instruction '%s'."), buffer.Upper().uniCStr(), parser.Instruction.uniCStr()));
    script[i].Empty();
    continue;
}

#endif
