
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

case roulette_inst_write_id:
//Similar code
case roulette_inst_input_id:
case roulette_inst_popup_id:
case roulette_inst_confirm_id:
case roulette_inst_status_id:
case roulette_inst_append_id:
{
    //Detects an input string
    if ((parser.InstructionID == roulette_inst_input_id) && parser.HasParameters())
    {
        stringmode = (parser.CommandList.Item(0) == wxT("string"));
        if (stringmode)
        {
            parser.CommandList[0] = wxEmptyString;
            parser.RebuildCommandLine();
        }
    }
    else
        stringmode = false;

    //Manages the type of message
    msg_type = wxRouletteMessageType::INFO_T;
    if (parser.HasParameters())
    {
        //Sets the new specified type
        buffer = parser.CommandList.Item(0);
        if (buffer == wxT("warning"))
            msg_type = wxRouletteMessageType::WARNING_T;
        else
            if (buffer == wxT("error"))
                msg_type = wxRouletteMessageType::ERROR_T;
            else
                if (buffer == wxT("debug"))
                    msg_type = wxRouletteMessageType::DEBUG_T;

        //Removes the argument from the list
        if (msg_type != wxRouletteMessageType::INFO_T)
        {
            parser.CommandList.RemoveAt(0);     //Only removes the first occurrence set in the first position
            parser.RebuildCommandLine();
        }
    }

    //Checks
    if (!parser.HasParameters())
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Splits the command into elements to display both texts and calculated values
    list.Clear();
    buffer = parser.Command + wxT(" ");            //The final space triggers the execution of the last argument
    formula.Alloc(buffer.Len());
    formula.Empty();
    message.Alloc(2*buffer.Len());
    message.Empty();
    b = false;                                    //No string activated by default
    textmode = false;
    for (j=0 ; j<buffer.Len() ; j++)
    {
        car = buffer.GetChar(j);

        //Management of the quotes for strings
        if (car == wxT('\''))
        {
            b = !b;
            if (b)
                textmode = true;
            //Case where a double quote <''> becomes a single quote <'>
            if (b && j>0)
                if (buffer.GetChar(j-1) == wxT('\''))
                    formula += wxT('\'');
            continue;
        }

        //End of a formula
        if ((car == wxT(' ')) && !b)
        {
            //Detects the floating output
            if (formula.StartsWith(wxT("f:")))
            {
                format = FLOAT;
                formula = formula.Mid(2, formula.Len());
            }
            else
                if (formula.StartsWith(wxT("c:")))
                {
                    format = CHAR;
                    formula = formula.Mid(2, formula.Len());
                }
                else
                    format = NORMAL;

            //Converts the text
            if (!textmode)
            {
                if (formula == wxT("space"))
                    message.Append(roulette_char_nbspace);
                else if (!m_engine.SetFormula(formula))
                    textmode = true;
                else
                {
                    switch (format)
                    {
                        case FLOAT:
                            fvalue = m_engine.Compute();
                            break;
                        default:
                            lvalue = m_engine.Compute();
                            break;
                    }
                    if (m_engine.GetLastError() == wxECE_NOERROR)           //If no error, we got a calculated formula
                    {
                        //Adds the relevant space
                        if (message.IsEmpty())
                            car = roulette_char_zero;
                        else
                            car = message.Right(1).GetChar(0);
                        if ((car != roulette_char_space) && (car != roulette_char_nbspace))
                            message.Append(wxT(" "));

                        //Formats the data
                        switch (format)
                        {
                            case FLOAT:
                                message.Append(wxString::Format(wxT("%f"), fvalue));
                                break;
                            case CHAR:
                                message.Append(wxString::Format(wxT("%c"), (wxChar)lvalue));
                                break;
                            default:
                                message.Append(wxString::Format(wxT("%d"), lvalue));
                                break;
                        }
                    }
                    else
                        textmode = true;
                }
            }
            if (textmode)
            {
                if (message.IsEmpty())
                    car = roulette_char_zero;
                else
                    car = message.Right(1).GetChar(0);
                if ((car == roulette_char_space) || (car == roulette_char_nbspace)) //Respectively space and non-breaking space
                    message = wxString::Format(wxT("%s%s"), message.uniCStr(), formula.uniCStr());
                else
                    message = wxString::Format(wxT("%s %s"), message.uniCStr(), formula.uniCStr());
            }

            //Resets the buffers
            formula.Empty();
            textmode = false;
            continue;
        }

        //Normal read
        formula += car;
    }
    message = message.Trim(true).Trim(false);


    //------------------------------------------
    //Rules for POPUP
    if (parser.Instruction == roulette_inst_popup)
    {
    #ifdef _CONSOLE
        parser.Instruction = roulette_inst_write;
        m_engine.SetConstant(roulette_vars_popup, wxID_YES);
    #else
        if (!message.IsEmpty())
        {
            switch (msg_type)
            {
                case wxRouletteMessageType::WARNING_T:
                    ulvalue = wxICON_WARNING;
                    break;
                case wxRouletteMessageType::ERROR_T:
                    ulvalue = wxICON_ERROR;
                    break;
                default:
                    ulvalue = wxICON_INFORMATION;
                    break;
            }
            wxMessageDialog(NULL, message, _("Popup message"), wxOK|ulvalue).ShowModal();
        }
        else
            if (!parser.NoWarning)
                LogError(wxString::Format(_("No text has been generated for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        m_engine.SetConstant(roulette_vars_popup, wxID_YES);
        continue;
    #endif
    }


    //------------------------------------------
    //Rules for CONFIRM
    if (parser.Instruction == roulette_inst_confirm)
    {
        if (message.IsEmpty())
        {
            if (!parser.NoWarning)
                LogError(wxString::Format(_("No text has been generated for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
            m_engine.DeleteConstant(roulette_vars_popup);
        }
        else
        {
        #ifdef _CONSOLE
            //Shows the text
            LogMessage(message);
            LogMessage(_("> Possible answers : (Y)es, (N)o, (C)ancel"));

            //Gets an input from the user
            fflush(stdin);
            scanf("%c", &console_input_c);

            //Manages the input
            if ((console_input_c == 'y') || (console_input_c == 'Y'))
                ulvalue = wxID_YES;
            else
                if ((console_input_c == 'n') || (console_input_c == 'N'))
                    ulvalue = wxID_NO;
                else
                    ulvalue = wxID_CANCEL;

            //Stores the final result
            m_engine.SetConstant(roulette_vars_popup, ulvalue);
        #else
            m_engine.SetConstant(roulette_vars_popup, wxMessageDialog(NULL, message, _("Confirmation"), wxYES_NO|wxCANCEL|wxICON_QUESTION).ShowModal());
        #endif
        }
        continue;
    }


    //------------------------------------------
    //Rules for STATUS
    if (parser.Instruction == roulette_inst_status)        //Blank text is allowed
    {
    #ifdef _CONSOLE
        parser.Instruction = roulette_inst_write;
    #else
        if (m_frame != NULL)
        {
            m_frame->SetStatusBarText(message, 2);
            m_frame->Refresh();
        }
        continue;
    #endif
    }


    //------------------------------------------
    //Rules for WRITE
    if (parser.Instruction == roulette_inst_write)
    {
        if (!message.IsEmpty())
        {
            switch (msg_type)
            {
                case wxRouletteMessageType::INFO_T:
                    LogMessage(message);
                    break;
                case wxRouletteMessageType::WARNING_T:
                    LogWarning(message);
                    break;
                case wxRouletteMessageType::ERROR_T:
                    LogError(message);
                    break;
                case wxRouletteMessageType::DEBUG_T:
                    LogDebug(message);
                    break;
                default:
                    wxASSERT(false);
                    break;
            }
        }
        else
            if (!parser.NoWarning)
                LogError(wxString::Format(_("No text has been generated for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        continue;
    }


    //------------------------------------------
    //Rules for APPEND
    if (parser.Instruction == roulette_inst_append)
    {
        if (!message.IsEmpty())
            AppendLogMessage(message);
        else
            if (!parser.NoWarning)
                LogError(wxString::Format(_("No text has been generated for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        continue;
    }


    //------------------------------------------
    //Rules for INPUT
    if (parser.Instruction == roulette_inst_input) 
    {
        //Removes the length of the input
        m_engine.DeleteConstant(roulette_vars_input_length);

    #ifdef _CONSOLE
        //Shows the text
        LogMessage(message);
        LogMessage(_("> An input is required :"));

        //Gets an input from the user
        fflush(stdin);
        scanf("%511s", (char*)(&console_input_s));
        buffer = wxString(console_input_s, wxConvISO8859_1);
    #else
        buffer = wxGetTextFromUser(message, _("Input"), wxEmptyString);    //If you fall in an infinite loop because of the way the script is written, kill the process
    #endif

        //Handles the cancellation
        if (buffer.IsEmpty())
        {
            m_engine.SetConstant(roulette_vars_popup, wxID_CANCEL);
            continue;
        }
        else
            m_engine.SetConstant(roulette_vars_popup, wxID_YES);

        //Stores the string value
        if (stringmode)
        {
            buffer2 = wxString::Format(wxT("%s_*"), roulette_vars_input);
            m_engine.DeleteConstantMask(buffer2);
            for (j=0 ; j<buffer.Len() ; j++)
                m_engine.SetConstant(   wxString::Format(wxT("%s_%d"), roulette_vars_input, j+1),
                                    #if wxMAJOR_VERSION >= 3
                                        buffer.GetChar(j).GetValue()
                                    #else
                                        buffer.GetChar(j)
                                    #endif
                                    );
            m_engine.SetConstant(roulette_vars_input_length, buffer.Len());
        }

        //Stores the numerical value
        else
        {
            //Calculates the value
            b = false;
            if (m_engine.SetFormula(buffer))
            {
                m_engine.Compute();
                if (m_engine.GetLastError() == wxECE_NOERROR)
                {
                    fvalue = m_engine.GetLastResult();
                    b = true;
                }
            }

            //Stores the value
            if (b)
                m_engine.SetConstant(roulette_vars_input, fvalue);
            else
            {
                m_engine.DeleteConstant(roulette_vars_input);
                LogError(wxString::Format(_("'%s' cannot be interpreted as a value."), buffer.uniCStr()));
            }
        }

        continue;
    }


    //------------------------------------------
    wxASSERT(false);
}

#endif
