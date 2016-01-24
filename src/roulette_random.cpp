
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

if (parser.Instruction == roulette_inst_random)
{
    //-- Reads the parameters assigned to the instruction
    b = false;
    if (parser.CountParameters() > 0)
    {
        if (m_engine.SetFormula(parser.Command))
        {
            m_engine.Compute();
            if (m_engine.GetLastError() == wxECE_NOERROR)
                if (m_engine.GetLastResult() > 0)
                    b = true;
        }
    }

    //-- Defines the min-max
    if (b)
    {
        if (wxRouletteHelper::IsOnlineGenerator(m_table.GetPRNG()))
        {
            if (!parser.NoWarning)
                LogWarning(_("The online services won't generate free numbers. Switching to the normal mode."));
            goto random_normal_mode;
        }
        ulmin = 0;
        ulmax = m_engine.GetLastResult();
    }
    else
    {
random_normal_mode:
        ulmin = wxRouletteHelper::GetMinNumberFromType(m_table.GetRouletteType());
        ulmax = wxRouletteHelper::GetMaxNumberFromType(m_table.GetRouletteType());
    }

    //-- Gets a random number
    if (m_table.GetRandomNumber(&ulvalue))
    {
        wxASSERT(ulmin != ulmax);
        ulvalue = ulmin + (ulvalue % (ulmax - ulmin + 1));
        m_engine.SetConstant(roulette_vars_random, ulvalue);
    }
    else
        m_engine.DeleteConstant(roulette_vars_random);
    continue;
}

#endif
