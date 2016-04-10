
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

case roulette_inst_clear_id:
{
    //Checks the parameters
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Processes the command
    buffer = parser.Command;
    if (buffer == wxT("bets"))
    {
        m_table.ClearBets();
        DoUpdateDynamicVariables(parser.NoWarning);
        continue;
    }

    if (buffer == wxT("history"))
    {
        m_spin_history.FreeData();
        DoClearStats();
        continue;
    }

    if (buffer == wxT("landed number"))
    {
        DoClearLandedNumber();
        continue;
    }

    if (buffer == wxT("last history"))
    {
        m_spin_history.DeleteLastEntry();
        DoClearStats();
        continue;
    }

    if ((buffer == wxT("log")) || (buffer == wxT("console")))
    {
        DoClearLog();
        continue;
    }

    if (buffer == wxT("money"))
    {
        m_cash.EmptyData();
        m_credit.EmptyData();
        continue;
    }

    if (buffer == wxT("random"))
    {
        if (!m_table.RestorePRNG())
            if (!parser.NoWarning)
                LogWarning(_("The algorithm doesn't allow to restore the initial status of the randomization."));
        continue;
    }

    if (buffer == wxT("time"))
    {
        m_time_last = false;
        continue;
    }

    if (buffer == wxT("stat"))
    {
        DoClearStats();
        continue;
    }

    if (buffer == wxT("backup"))
    {
        m_backup.ResetConstants(false);
        continue;
    }

    //Unsupported cases
    LogError(wxString::Format(_("Unsupported command '%s' for the instruction '%s'."), buffer.Upper().uniCStr(), parser.Instruction.uniCStr()));
    script[i].Empty();
    continue;
}

#endif
