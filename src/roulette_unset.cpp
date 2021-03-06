
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

case roulette_inst_unset_id:
{
    //Checks the parameters
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Sets the mask to delete all the entries of an array
    parser.ChangeCharacters(wxT("[]"), wxT("_*"));

    //Lists the variables
    m_engine.ListAllConstants(&list, false);

    //Loops on the variables
    b = false;
    for (j=0 ; j<list.GetCount() ; j++)
    {
        buffer = list.Item(j);

        //Some variables are protected
        if (wxRouletteHelper::GetVariableDomain(buffer) == roulette_dom_sys)
            continue;

        //Processes the parameters
        for (k=0 ; k<parser.CommandList.GetCount() ; k++)
        {
            buffer2 = parser.CommandList.Item(k);
            if (buffer2.IsEmpty())
                continue;

            //Keyword to delete all
            if (buffer2 == wxT("all"))
            {
                if (m_engine.DeleteConstant(buffer))
                    b = true;
                break;
            }

            //Evaluates the name of the variable to support the arrays
            if (m_engine.SetFormula(buffer2))
            {
                if (!m_engine.GetCompactExpression(&buffer2))
                    continue;
            }
            else
            {
                if (!parser.NoWarning)
                    LogWarning(wxString::Format(_("The name of the variable '%s' is misspelled."), buffer2.uniCStr()));
                parser.CommandList[k] = wxEmptyString;
                continue;
            }

            //By name or with a pattern
            if (m_engine.DeleteConstant(buffer2))
            {
                b = true;
                break;
            }
            else
            {
                if (buffer.Matches(buffer2) && (m_engine.DeleteConstant(buffer)))
                {
                    b = true;
                    break;
                }
            }
        }
    }

    //Final tasks
    if (!b)
        if (!parser.NoWarning)
            LogWarning(_("No variable has been deleted."));
    DoUpdateDynamicVariables(parser.NoWarning);
    continue;
}

#endif
