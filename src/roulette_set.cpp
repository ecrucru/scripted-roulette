
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

case roulette_inst_set_id:
{
    //Checks the parameters
    parser.RemoveParameter(wxT("="));
    if (parser.CountParameters() < 2)
    {
        LogError(wxString::Format(_("At least 2 arguments are expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Evaluates the name of the variable to support the arrays
    b = false;
    if (m_engine.SetFormula(parser.CommandList.Item(0)))
    {
        if (m_engine.GetCompactExpression(&buffer))
        {
            parser.CommandList[0] = buffer;
            parser.RebuildCommandLine();
            b = true;
        }
    }
    if (!b)
    {
        LogError(wxString::Format(_("The name of the variable '%s' is misspelled."), parser.CommandList.Item(0).uniCStr()));
        script[i].Empty();
        continue;
    }

    //Checks the syntax of the variable
    domain = wxRouletteHelper::GetVariableDomain(parser.CommandList.Item(0));
    name = wxRouletteHelper::GetVariableName(parser.CommandList.Item(0));
    if ((domain != roulette_dom_sys) && (domain != roulette_dom_user) && (domain != roulette_dom_game))
    {
        LogError(_("Unsupported domain for the variable."));
        script[i].Empty();
        continue;
    }
    if (name.IsEmpty())
    {
        LogError(_("No name given to the variable."));
        script[i].Empty();
        continue;
    }
    if ((domain == roulette_dom_sys) && !pInit)
    {
        LogError(_("You cannot change a system variable in a sequence."));
        script[i].Empty();
        continue;
    }

    //Management of the system variables
    if (domain == roulette_dom_sys)
    {
        //Type of roulette
        if (name == wxT("type"))
        {
            //Checks
            if ((parser.CountParameters() != 2) && (parser.CountParameters() != 3))
            {
            get_invalid_sys_type:
                LogError(wxString::Format(_("Incorrect number of parameters to define '%s'."), wxT("sys.type")));
                script[i].Empty();
                continue;
            }
            //Checks if the roulette needed to be balanced
            b = (parser.CommandList.Item(1) == wxT("balanced"));
            if (b && (parser.CountParameters() != 3))
                goto get_invalid_sys_type;
            if (!b && (parser.CountParameters() != 2))
                goto get_invalid_sys_type;
            //Updates
            if (b)
                buffer = parser.CommandList.Item(2);
            else
                buffer = parser.CommandList.Item(1);
            //Sets the layout
            if (buffer == wxT("french"))
                m_table.SetFrenchLayout(b);
            else
                if (buffer == wxT("american"))
                    m_table.SetAmericanLayout(b);
                else
                    if (buffer == wxT("mexican"))
                    {
                        if (b && !parser.NoWarning)
                            LogWarning(_("A balanced mexican roulette will adopt the balanced american layout."));
                        m_table.SetMexicanLayout(b);
                    }
                    else
                    {
                        LogError(wxString::Format(_("Unrecognized roulette type ('%s')."), buffer.Upper().uniCStr()));
                        script[i].Empty();
                    }
            //Updates the variable telling how many numbers are on the table
            m_engine.SetConstant(roulette_vars_numbers,    m_table.GetCellCount());
            m_engine.SetConstant(roulette_vars_number_min, wxRouletteHelper::GetMinNumberFromType(m_table.GetRouletteType()));
            m_engine.SetConstant(roulette_vars_number_max, wxRouletteHelper::GetMaxNumberFromType(m_table.GetRouletteType()));
            continue;
        }

        //Type of the algorithm
        if (name == wxT("algorithm"))
        {
            //Checks
            if (parser.CountParameters() != 2)
            {
                LogError(wxString::Format(_("Incorrect number of parameters to define '%s'."), wxT("sys.algorithm")));
                script[i].Empty();
                continue;
            }
            //Sets the algorithm
            buffer = parser.CommandList.Item(1);
            algo = wxRouletteHelper::AlgorithmToID(buffer);
            if (algo != wxRouletteAlgorithm::NONE)
                m_table.SetAlgorithm(algo);
            else
            {
                LogError(wxString::Format(_("'%s' is a not a supported random number generator."), buffer.Upper().uniCStr()));
                script[i].Empty();
                RequestStop();
            }
            continue;
        }

        //Unknown
        LogError(wxString::Format(_("'%s.%s' is not a system variable that you can change or create."), domain.uniCStr(), name.uniCStr()));
        script[i].Empty();
        continue;
    }

    //Sets the value of all other authorized names
    position = parser.Command.Find(wxT(" "));
    wxASSERT(position != wxNOT_FOUND);
    name = parser.Command.Mid(0, position);
    formula = parser.Command.Mid(position+1, parser.Command.Len());
    if (!m_engine.SetFormula(formula))
    {
        LogError(wxString::Format(_("Wrong syntax encountered when processing the definition of '%s'."), name.uniCStr()));
        script[i].Empty();
        continue;
    }
    fvalue = m_engine.Compute();
    if (m_engine.GetLastError() != wxECE_NOERROR)
    {
        LogError(wxString::Format(_("Error when processing '%s' (%s) at '%s'.")
                        , formula.uniCStr()
                        , m_engine.TranslateError(m_engine.GetLastError()).uniCStr()
                        , m_engine.GetIndicator().uniCStr()
                    ));
        script[i].Empty();
        continue;
    }
    m_engine.SetConstant(name, /*(long)*/ fvalue);
    DoUpdateDynamicVariables(parser.NoWarning);
    continue;
}

#endif
