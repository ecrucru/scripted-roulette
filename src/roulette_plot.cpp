
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

if (parser.Instruction == roulette_inst_plot)
{
    //Checks the parameters
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Change the keywords
    parser.ChangeParameter(wxT("on"), roulette_vars_true);
    parser.ChangeParameter(wxT("off"), roulette_vars_false);

    //Translate the parameters
    b = false;
    if (m_engine.SetFormula(parser.Command))
    {
        m_engine.Compute();
        if (m_engine.GetLastError() == wxECE_NOERROR)
        {
            ulvalue = m_engine.GetLastResult();
            b = true;
        }
    }
    if (!b)
    {
        LogWarning(wxT("Unable to interpret the argument for instruction PLOT."));
        continue;
    }

    //Changes the flag for the update of the plot
    m_engine.SetConstant(roulette_vars_plot, (ulvalue == 1 ? 1 : 0));
    continue;
}

#endif
