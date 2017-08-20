
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

case roulette_inst_exist_id:
{
    //Checks the parameters
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Checks if all the provided variables exist
    b = true;
    buffer.Empty();
    for (j=0 ; j<parser.CommandList.Count() ; j++)
    {
        //Expand the arrays
        if (m_engine.SetFormula(parser.CommandList.Item(j)))
        {
            if (!m_engine.GetCompactExpression(&buffer))
            {
                b = false;
                break;
            }
        }
        else
            buffer = parser.CommandList.Item(j);

        //Checks the variable
        if (!m_engine.HasConstant(buffer))
        {
            b = false;
            break;
        }
    }

    //Saves the result
    m_engine.SetConstant(roulette_vars_result, (b ? 1 : 0));
    continue;
}

#endif
