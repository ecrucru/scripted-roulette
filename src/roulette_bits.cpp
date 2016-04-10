
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

case roulette_inst_bits_id:
{
    //Checks the parameters
    if (parser.CountParameters() < 2)
    {
        LogError(wxString::Format(_("At least 2 arguments are expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }
    parser.RemoveCharacters(wxT("[]"));

    //Gets the name of the target variable
    name = parser.CommandList.Item(0);
    if (wxRouletteHelper::GetVariableDomain(name) != roulette_dom_user)
    {
        LogError(_("Unsupported domain for the variable."));
        script[i].Empty();
        continue;
    }
    buffer = wxRouletteHelper::GetVariableName(name);
    if (    buffer.IsEmpty()
        || (wxRouletteHelper::CountCharInString(buffer, wxT('[')) != 0)
        || (wxRouletteHelper::CountCharInString(buffer, wxT(']')) != 0)
       )
    {
        LogError(_("No name given to the variable."));
        script[i].Empty();
        continue;
    }

    //Evaluate the formula
    parser.CommandList[0] = wxEmptyString;
    parser.RebuildCommandLine();
    b = false;
    if (m_engine.SetFormula(parser.Command))
    {
        m_engine.Compute();
        if (m_engine.GetLastError() == wxECE_NOERROR)
        {
            fvalue = m_engine.GetLastResult();
            b = true;
        }
    }
    if (!b)
        continue;

    //Generates the array: 1=LSB... N=MSB
    ulvalue = (fvalue < 0 ? -fvalue : fvalue);
    ulvalue = wxUINT32_SWAP_ON_BE(ulvalue);
    if (ulvalue == 0)
        m_engine.SetConstant(wxString::Format(wxT("%s_0"), name.uniCStr()), 0.0);
    else
    {
        j = 0;
        while (ulvalue > 0)
        {
            m_engine.SetConstant(   wxString::Format(wxT("%s_%d"), name.uniCStr(), j),
                                    ulvalue & 1
                                );
            ulvalue >>= 1;
            j++;
        }
    }
    continue;
}

#endif
