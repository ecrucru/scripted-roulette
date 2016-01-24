
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

if (parser.Instruction == roulette_inst_buy)
{
    //Gets the amount to be bought
    fvalue = 0;
    if (parser.CountParameters() > 0)
    {
        if (m_engine.SetFormula(parser.Command))
        {
            m_engine.Compute();
            if (m_engine.GetLastError() == wxECE_NOERROR)
                fvalue = m_engine.GetLastResult();
        }
    }

    //Checks
    if (fvalue <= 0)
    {
        if (!parser.NoWarning)
            LogError(_("You can't buy with a negative or null value."));
        continue;
    }

    //Applies the amount
    if (!DoBuyStakes(fvalue))
        LogError(wxString::Format(_("Your amount of %.2f has been rejected."), fvalue));
    else
        if (!parser.NoWarning)
            LogMessage(wxString::Format(_("You have bought new stakes for %.2f units of money."), fvalue));
    continue;
}

#endif
