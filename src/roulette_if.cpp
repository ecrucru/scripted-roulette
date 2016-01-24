
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

if (parser.Instruction == roulette_inst_if)
{
    if_depth++;

    //Cleans the input from the unsupported brackets
    if (parser.Command.StartsWith(wxT("(")) || (parser.Command.Find(wxT(" (")) != wxNOT_FOUND))
    {
        script[i] = wxString::Format(wxT("if %s"), roulette_vars_false);    //The error will not occur again
        if (!parser.NoWarning)
            LogWarning(_("The priorities in conditional statements are not supported. Refer to the documentation."));
        goto current_statement;
    }
    if (wxRouletteHelper::CountCharInString(parser.Command, wxT('(')) != wxRouletteHelper::CountCharInString(parser.Command, wxT(')')))
    {
        script[i] = wxString::Format(wxT("if %s"), roulette_vars_false);    //The error will not occur again
        if (!parser.NoWarning)
            LogWarning(_("Incorrect number of parentheses in a statement 'IF'."));
        goto current_statement;
    }

    //Checks the parameters
    if (!parser.HasParameters())
    {
        parser.AddParameter(roulette_vars_true);
        if (!parser.NoWarning)
            LogWarning(_("Empty statements in an instruction 'IF' are not expected."));
    }

    //Analyzes the list of arguments
    if_not_left = false;
    if_not_right = false;
    if_result = true;                               //Useless but secure
    if_error = false;
    if_sign = wxRouletteSign::NONE;
    if_operator = wxRouletteOperator::NONE;
    if_left.Empty();
    if_right.Empty();
    if_max = parser.CommandList.Count();
    if (if_max > 0)
        if_max--;
    for (j=0 ; j<parser.CommandList.Count() ; j++)
    {
        buffer = parser.CommandList.Item(j);

        //Operator NOT
        if (buffer == wxT("not"))
        {
            if ((if_sign == wxRouletteSign::NONE) && (if_left.IsEmpty()) && !if_not_left)
            {
                if_not_left = true;
                continue;
            }
            if ((if_sign != wxRouletteSign::NONE) && (if_right.IsEmpty()) && !if_not_right)
            {
                if_not_right = true;
                continue;
            }
            if (!parser.NoWarning)
                LogWarning(_("The logical operator NOT is expected to be at the beginning of a statement."));
        }

        //Signs (=, <=...)
        if (wxRouletteHelper::IsSign(buffer))
        {
            if_sign = wxRouletteHelper::SignToID(buffer);
            continue;
        }

        //Normal term of the condition
        if (!wxRouletteHelper::IsOperator(buffer))
        {
            if (if_sign == wxRouletteSign::NONE)
                if_left += buffer;
            else
                if_right += buffer;
            //Don't add "continue" here, else you can't parse statements containing no operator
        }

        //Operators (AND, OR...)
        if (wxRouletteHelper::IsOperator(buffer) || (j == if_max))
        {
            //Correction of missing statements
            if (if_left.IsEmpty())
                if_left = roulette_vars_true;
            if (if_right.IsEmpty())
            {
                if (if_sign == wxRouletteSign::NONE)
                    if_sign = wxRouletteSign::EQ;
                if_right = roulette_vars_true;
            }

            //Calculation of the left part of the formula
            if_left_val = 0;
            if (m_engine.SetFormula(if_left))
            {
                m_engine.Compute();
                switch (m_engine.GetLastError())
                {
                    case wxECE_NOERROR:
                        if_left_val = m_engine.GetLastResult();
                        break;

                    case wxECE_UNDEFCONSTANT:
                        if_error = true;
                        if (!parser.NoWarning)
                            LogWarning(wxString::Format(_("Attempt to access an undefined variable ('%s') in a statement 'IF'."), m_engine.GetIndicator().uniCStr()));
                        break;

                    default:
                        if_error = true;
                        if (!parser.NoWarning)
                            LogError(wxString::Format(_("Error in the logical expression (%s)."), m_engine.TranslateError(m_engine.GetLastError()).uniCStr()));
                        break;
                }
                if (if_error)
                    break;
            }
            if (if_not_left)
            {
                if_left_val = (if_left_val == 1 ? 0 : (if_left_val == 0 ? 1 : if_left_val));
                if_not_left = false;
            }

            //Calculation of the right part of the formula
            if_right_val = 0;
            if (m_engine.SetFormula(if_right))
            {
                m_engine.Compute();
                switch (m_engine.GetLastError())
                {
                    case wxECE_NOERROR:
                        if_right_val = m_engine.GetLastResult();
                        break;

                    case wxECE_UNDEFCONSTANT:
                        if_error = true;
                        if (!parser.NoWarning)
                            LogWarning(wxString::Format(_("Attempt to access an undefined variable ('%s') in a statement 'IF'."), m_engine.GetIndicator().uniCStr()));
                        break;

                    default:
                        if_error = true;
                        if (!parser.NoWarning)
                            LogError(wxString::Format(_("Error in the logical expression (%s)."), m_engine.TranslateError(m_engine.GetLastError()).uniCStr()));
                        break;
                }
                if (if_error)
                    break;
            }
            if (if_not_right)
            {
                if_right_val = (if_right_val == 1 ? 0 : (if_right_val == 0 ? 1 : if_right_val));
                if_not_right = false;
            }

            //Comparison of the results
            switch (if_sign)
            {
                case wxRouletteSign::EQ: if_temp = (if_left_val == if_right_val); break;
                case wxRouletteSign::NE: if_temp = (if_left_val != if_right_val); break;
                case wxRouletteSign::LT: if_temp = (if_left_val <  if_right_val); break;
                case wxRouletteSign::LE: if_temp = (if_left_val <= if_right_val); break;
                case wxRouletteSign::GT: if_temp = (if_left_val >  if_right_val); break;
                case wxRouletteSign::GE: if_temp = (if_left_val >= if_right_val); break;
                //...
                case wxRouletteSign::NONE:
                default:
                    wxASSERT(false);            //Impossible cases
                    if_temp = false;            //Warning GCC
                    break;
            }

            //Debug step
            /* LogDebug(wxString::Format(wxT("%s (%f) %s (%f) // %d = %d"),
                        if_left.uniCStr(),
                        if_left_val,
                        if_right.uniCStr(),
                        if_right_val,
                        if_sign,
                        (if_temp ? 1 : 0)
                ));
            */

            //Apply the logical result
            switch (if_operator)
            {
                case wxRouletteOperator::AND:
                    if_result = if_result && if_temp;
                    break;
                case wxRouletteOperator::OR:
                    if_result = if_result || if_temp;
                    break;
                case wxRouletteOperator::XOR:
                    if_result = if_result ^ if_temp;
                    break;
                case wxRouletteOperator::NAND:
                    if_result = !(if_result && if_temp);
                    break;
                case wxRouletteOperator::NOR:
                    if_result = !(if_result || if_temp);
                    break;
                case wxRouletteOperator::NONE:
                    if_result = if_temp;
                    break;
                default:
                    wxASSERT(false);        //There is no other operator
                    if_result = false;      //Warning GCC
                    break;
            }

            //Reset the buffers
            if_sign = wxRouletteSign::NONE;
            if_operator = wxRouletteHelper::OperatorToID(buffer);       //It can be NONE during the last loop
            if_left.Empty();
            if_right.Empty();
            continue;
        }
    }

    //Result of the condition
    if (if_error)
        if_result = false;
    m_engine.SetConstant(roulette_vars_last_if, (if_result ? 1 : 0));    //Also used in ASSERT

    //Go to the next statement (ELSE or ENDIF)
    if (if_result)
        continue;               //It will not generate the errors later
    else
    {
        initial_depth = if_depth;
        for (k=i+1 ; k<=line_max ; k++)
        {
            buffer = wxRouletteHelper::GetInstructionName(script.Item(k));

            //IF: it increases the depth
            if (buffer == roulette_inst_if)
            {
                if_depth++;
                continue;
            }

            //ELSE: it is part of the current or sub-level condition
            if (buffer == roulette_inst_else)
            {
                if (if_depth == initial_depth)
                {
                    i = k+1;
                    //LogDebug(wxString::Format(wxT("--> Jump (1) to %d [%s]"), i, script.Item(i).uniCStr()));
                    goto current_statement;
                }
                else
                    continue;
            }

            //ENDIF:
            if (buffer == roulette_inst_endif)
            {
                if (if_depth == initial_depth)
                {
                    i = k+1;
                    //LogDebug(wxString::Format(wxT("--> Jump (2) to %d [%s]"), i, script.Item(i).uniCStr()));
                    if_depth--;
                    goto current_statement;
                }
                else
                {
                    if (if_depth == 0)
                    {
                        LogError(wxString::Format(_("No opened instruction 'IF' for the instruction '%s'."), buffer.Upper().uniCStr()));
                        script[k].Empty();
                        continue;
                    }
                    else
                        if_depth--;
                    continue;
                }
            }
        }
    }

    //Syntax error
    if (!if_error)
        LogError(_("Missing instruction 'ENDIF' at the end of the sequence."));
    break;          //End of the sequence
}

#endif
