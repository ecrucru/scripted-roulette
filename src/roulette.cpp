
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

#include "roulette.h"


//------------------------------------------

void wxRoulette::DoFreeSequences(bool pDestroy)
{
    unsigned long i;

    //-- Free all
    if ((m_sequence_count > 0) && (m_sequences != NULL))
    {
        for (i=0 ; i<m_sequence_count ; i++)
        {
            wxDELETE(m_sequences[i]);
        }
        m_sequence_count = 0;
        m_sequences = NULL;
    }

    //-- Let 1 cell for the initialization
    if (!pDestroy)
    {
        m_sequence_count = 1;
        m_sequences = (wxRouletteSequence**) malloc(sizeof(wxRouletteSequence*));
        m_sequences[0] = NULL;
    }
}

void wxRoulette::DoResetConstants()
{
    m_engine.ResetConstants(false);

    //-- Defines the global and default variables
#ifdef _CONSOLE
    m_engine.SetConstant(roulette_vars_console,         1);
#else
    m_engine.SetConstant(roulette_vars_console,         0);
#endif
#ifdef __WXDEBUG__
    m_engine.SetConstant(roulette_vars_debug,           1);
#else
    m_engine.SetConstant(roulette_vars_debug,           0);
#endif
    m_engine.SetConstant(roulette_vars_version,         roulette_about_version_num);
    m_engine.SetConstant(roulette_vars_true,            1);
    m_engine.SetConstant(roulette_vars_false,           0);
    m_engine.SetConstant(roulette_vars_zero,            roulette_zero);
    m_engine.SetConstant(roulette_vars_zero_double,     roulette_zero_double);
    m_engine.SetConstant(roulette_vars_zero_triple,     roulette_zero_triple);
    m_engine.SetConstant(roulette_vars_plot,            1);
    m_engine.SetConstant(roulette_vars_popup_yes,       wxID_YES);
    m_engine.SetConstant(roulette_vars_popup_no,        wxID_NO);
    m_engine.SetConstant(roulette_vars_popup_cancel,    wxID_CANCEL);
}

bool wxRoulette::DoCopyVariables(wxRouletteEngine *pFrom, wxRouletteEngine *pTo)
{
    wxArrayString list;
    size_t i;
    double value;
    bool b;

    //-- Checks
    if ((pFrom == NULL) || (pTo == NULL))
        return false;
    if (pFrom->GetConstantCount() == 0)
        return false;

    //-- Copies the variables
    pTo->ResetConstants(false);
    pFrom->ListAllConstants(&list, false);
    for (i=0 ; i<list.Count() ; i++)
    {
        b = pFrom->GetConstant(list.Item(i), &value);
        if (b)
            pTo->SetConstant(list.Item(i), value);
        else
        {
            wxASSERT(false);
        }
    }
    return true;
}

bool wxRoulette::DoLoad(bool *pVersionMatch)
{
    wxRouletteInstruction parser;
    wxArrayString content;
    wxString buffer, comment, name, subfile;
    size_t i, j, pos;
    int nb_includes = 0;                    //Prevents infinite inclusions
    bool skip_bits = false;
    unsigned long ulvalue;

    //-- By default the version is compatible
    if (pVersionMatch != NULL)
        *pVersionMatch = true;

    //-- Simplifies the file to make it readable by the application
    for (i=0 ; i<m_script.GetCount() ; i++)
    {
reprocess_code:
        buffer = wxRouletteHelper::FormatInstruction(m_script.Item(i));

        //- Processing of special commands
        if (parser.Parse(buffer, true))
        {
            //Includes
            if (parser.InstructionID == roulette_inst_include_id)
            {
                if (nb_includes >= roulette_max_includes)
                    LogError(wxString::Format(_("Having more than %d includes is not accepted."), roulette_max_includes));
                else
                {
                    subfile = wxString::Format(wxT("%s.sri"), parser.Command.uniCStr());
                    if (wxRouletteHelper::LoadFileToArrayString(subfile, &content))
                    {
                        if (content.GetCount() > 0)     //Empty includes to not count in the maximal total
                        {
                            nb_includes++;
                            for (j=0 ; j<content.GetCount() ; j++)
                                m_script.Insert(content.Item(j), i+1+j);
                        }
                    }
                    else
                        LogError(wxString::Format(_("Unable to include '%s'."), subfile.uniCStr()));
                }
                buffer.Empty();         //We don't remove the line for now to not desynchronize the loop
            }

            //Macros - Assertion
            if (parser.InstructionID == roulette_inst_assert_id)
            {
                m_script[i] = wxString::Format(wxT("IF %s"), parser.Command.uniCStr());
                m_script.Insert(wxT("ENDIF"), i+1);
                m_script.Insert(wxT("STOP"), i+1);
                m_script.Insert(wxString::Format(wxT("WRITE error '%s: %s'"), wxRouletteHelper::EscapeQuote(wxString(_("Assertion failed"))).uniCStr(), wxRouletteHelper::EscapeQuote(buffer).uniCStr()), i+1);
                m_script.Insert(wxString::Format(wxT("IF NOT %s"), wxString(roulette_vars_last_if).uniCStr()), i+1);
                m_script.Insert(wxT("ENDIF"), i+1);
                goto reprocess_code;
            }

            //Macros - Check
            if (parser.InstructionID == roulette_inst_check_id)
            {
                m_script[i] = wxString::Format(wxT("IF %s"), parser.Command.uniCStr());
                m_script.Insert(wxT("ENDIF"), i+1);
                m_script.Insert(wxT("BREAK"), i+1);
                m_script.Insert(wxString::Format(wxT("IF NOT %s"), wxString(roulette_vars_last_if).uniCStr()), i+1);
                m_script.Insert(wxT("ENDIF"), i+1);
                goto reprocess_code;
            }

            //Macros - Decrementation
            if (parser.InstructionID == roulette_inst_dec_id)
            {
                if (parser.CountParameters() != 1)
                {
                    LogError(wxString::Format(_("Only 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
                    buffer.Empty();
                }
                else
                {
                    m_script[i] = wxString::Format(wxT("SET %s = %s - 1"), parser.Command.uniCStr(), parser.Command.uniCStr());
                    goto reprocess_code;
                }
            }

            //Macros - Incrementation
            if (parser.InstructionID == roulette_inst_inc_id)
            {
                if (parser.CountParameters() != 1)
                {
                    LogError(wxString::Format(_("Only 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
                    buffer.Empty();
                }
                else
                {
                    m_script[i] = wxString::Format(wxT("SET %s = %s + 1"), parser.Command.uniCStr(), parser.Command.uniCStr());
                    goto reprocess_code;
                }
            }

            //Macros - Conditional bet
            if (parser.InstructionID == roulette_inst_cbet_id)
            {
                m_script[i] = wxString::Format(wxT("BET %s"), parser.Command.uniCStr());
                m_script.Insert(wxT("ENDIF"), i+1);
                m_script.Insert(wxString::Format(wxT("IF %s"), roulette_vars_can_bet), i);
                goto reprocess_code;
            }

            //Extended code - Translation to an array of bits
            if (parser.InstructionID == roulette_inst_bits_id)
            {
                if (skip_bits)
                    skip_bits = false;
                else
                {
                    if (parser.CommandList.GetCount() >= 2)
                    {
                        name = parser.CommandList.Item(0);
                        if (   ( wxRouletteHelper::GetVariableDomain(name) == roulette_dom_user)
                            && (!wxRouletteHelper::GetVariableName(name).IsEmpty())
                            && ( wxRouletteHelper::CountCharInString(name, wxT('[')) == 0)
                            && ( wxRouletteHelper::CountCharInString(name, wxT(']')) == 0)
                          )
                        {
                            m_script.Insert(wxString::Format(wxT("@UNSET %s[]"), name.uniCStr()), i);
                            skip_bits = true;
                            goto reprocess_code;
                        }
                    }
                }
            }

            //Simplified syntax - The spaces are allowed for the first argument of the instruction SET if the symbol "=" is used
            if (parser.InstructionID == roulette_inst_set_id)
            {
                pos = parser.Command.Find(wxT('='));
                if (pos != wxNOT_FOUND)
                {
                    buffer = parser.Command.Mid(0, pos-1);
                    buffer.Replace(wxT(" "), wxEmptyString);
                    buffer.Append(wxT(" "));
                    buffer.Append(parser.Command.Mid(pos, parser.Command.Len()));
                    buffer = wxString::Format(wxT("%s %s"), parser.Instruction.uniCStr(), buffer.uniCStr());
                }
            }
        }

        //- Preprocessing definitions
        comment = wxRouletteHelper::GetComment(m_script.Item(i));
        if ((pVersionMatch != NULL) && parser.Instruction.IsEmpty() && comment.StartsWith(wxT("#version-min:")))
        {
            comment = comment.Mid(13, comment.Len()).Trim(true).Trim(false);
            comment.Replace(wxT("\""), wxEmptyString, true);
            comment = comment.Trim(true).Trim(false);
            if (comment.ToULong(&ulvalue))
                *pVersionMatch = (roulette_about_version_num >= ulvalue);
            else
                *pVersionMatch = false;
        }

        //- Return the result
        m_script[i] = buffer;
    }

    //-- Removes the blank lines
    if (m_script.GetCount() > 0)
        for (i=m_script.GetCount() ; i>0 ; i--)
            if (m_script.Item(i-1).IsEmpty())
                m_script.RemoveAt(i-1);

    //-- The file is loaded
    return true;
}

void wxRoulette::DoDumpScript()
{
    size_t i;
    wxString mask;

    //-- Dumps the script
    i = m_script.GetCount();
    if (i == 0)
        LogMessage(_("No script is loaded."));
    else
    {
        //- Finds the most appropriated mask
        i = (int)ceil(log((double)i)/log(10.));
        mask = wxString::Format(wxT("%%.%dd: %%s"), i);

        //- Output
        LogMessage(_("The script contains the following content :"));
        for (i=0 ; i<m_script.GetCount() ; i++)
            if (!m_script.Item(i).IsEmpty())
                LogMessage(wxString::Format(mask, i+1, m_script.Item(i).uniCStr()).uniCStr());
    }
}

bool wxRoulette::DoInitSequences()
{
    size_t i;
    bool hasinit, b;
    wxString buffer;
    wxRouletteInstruction parser;
    wxRouletteSequence *sequence;

    //-- Initialization
    hasinit = false;
    sequence = NULL;
    if (m_script.GetCount() == 0)
        return false;

    //-- Split
    for (i=0 ; i<m_script.GetCount() ; i++)
    {
        //- Reads the instruction
        buffer = m_script.Item(i);
        b = parser.Parse(buffer, true);
        if (!b)
        {
            wxASSERT(false);
            LogError(wxString::Format(_("Unable to parse the instruction '%s'."), buffer.uniCStr()));
            goto FatalSplit;
        }

        //- Stores the initialization block
        if (parser.InstructionID == roulette_sect_init_id)
        {
            if (hasinit)
            {
                LogError(_("Multiple sections .INIT are not allowed."));
                goto FatalSplit;
            }
            else
            {
                if (parser.HasParameters())
                    LogWarning(wxString::Format(_("No argument is expected after the section '%s'."), parser.Instruction.uniCStr()));
                sequence = new wxRouletteSequence(this);        //The deletion is handled elsewhere
                wxASSERT(sequence != NULL);
                DoAddSequence(sequence, true);
                hasinit = true;
                continue;
            }
        }

        //- Stores a new sequence
        if (parser.InstructionID == roulette_sect_sequence_id)
        {
            if (parser.HasParameters())
                LogWarning(wxString::Format(_("No argument is expected after the section '%s'."), parser.Instruction.uniCStr()));
            sequence = new wxRouletteSequence(this);
            wxASSERT(sequence != NULL);
            DoAddSequence(sequence, false);
            continue;
        }

        //- Stores a new line of script into the current sequence
        if (sequence != NULL)
            sequence->AddInstruction(buffer);
        else
            LogWarning(wxString::Format(_("Ignored instruction : '%s'"), buffer.uniCStr()));
    }

    //-- Results
    //sequence = NULL;
    return true;

FatalSplit:
    //sequence = NULL;
    DoFreeSequences(false);
    return false;
}

bool wxRoulette::DoAddSequence(wxRouletteSequence *pSequence, bool pForInit)
{
    //-- Checks
    if (pSequence == NULL)
        return false;

    //-- Adds the sequence
    if (pForInit)
    {
        //- Opens the memory for the initial sequence
        if (m_sequence_count == 0)
            DoFreeSequences(false);

        //- Clears the previous initial sequence if available
        if (m_sequences[0] != NULL)
        {
            wxDELETE(m_sequences[0]);
        }

        //- Stores the new initial sequence
        m_sequences[0] = pSequence;
    }
    else
    {
        //- Opens the memory for the initial sequence
        if (m_sequence_count == 0)
            DoFreeSequences(false);

        //- Reallocation of the memory
        m_sequence_count++;
        m_sequences = (wxRouletteSequence**) realloc((void*)m_sequences, m_sequence_count*sizeof(wxRouletteSequence*));
        wxASSERT(m_sequences != NULL);

        //- Stores the new sequence
        m_sequences[m_sequence_count-1] = pSequence;
    }
    return true;
}

void wxRoulette::DoDumpSequences()
{
    unsigned long num_seq;
    size_t i;

    //-- Global stats
    num_seq = CountSequences();
    if (num_seq > 1)
        LogMessage(wxString::Format(_("There are %d sequences."), num_seq));
    else
        LogMessage(wxString::Format(_("There is %d sequence."), num_seq));
    if (num_seq == 0)
        return;

    //-- Display the sequences
    for (i=0 ; i<m_sequence_count ; i++)
    {
        if (m_sequences[i] == NULL)
            continue;
        m_sequences[i]->DumpScript();
    }
}

bool wxRoulette::DoExecuteSequence(wxRouletteSequence *pSequence, bool pInit)
{
    //-- Generic variables
    wxArrayString script, list;
    wxStringTokenizer tokenizer;
    wxRouletteInstruction parser;
    wxString buffer, buffer2, domain, name, formula, message;
#ifdef _CONSOLE
    char console_input_c, console_input_s[512];
#endif
    wxChar car;
    unsigned long i, j, k, line_max;
    bool b, win, textmode, stringmode;
    enum { NORMAL, FLOAT, CHAR } format;
    int position, initial_depth, forced_spin;
    long lvalue;
    unsigned long ulvalue, ulmin, ulmax, *ul_alloc_size, *ul_alloc_loop;
    int landed;
    double fvalue, fa, fb, cash, gain, relative_gain, credit, stake;
    wxDateTime stamp;
    wxTimeSpan timespan;
    wxRouletteAlgorithm::wxRouletteAlgorithm algo;
    wxRouletteHistory history;
    wxRouletteMessageType::wxRouletteMessageType msg_type;

    //-- Variables to manage the conditions
    int if_depth, if_look_depth;
    wxRouletteSign::wxRouletteSign if_sign;
    wxRouletteOperator::wxRouletteOperator if_operator;
    wxString if_left, if_right;
    double if_left_val, if_right_val;
    bool if_not_left, if_not_right, if_temp, if_result, if_error;
    size_t if_max;

    //-- Checks
    if (pSequence == NULL)
        return false;

    //-- Loads the script
    m_table.ClearBets();
    if_depth = 0;
    script = pSequence->GetScript();
    line_max = script.GetCount();
    if (line_max == 0)                      //A blank sequence must trigger a spin in any case
        return true;
    if (line_max > 0)
        line_max--;

    //-- Preprocesses the instructions LABEL
    for (i=0 ; i<=line_max ; i++)
    {
        //- Reads the item
        if (!parser.Parse(script.Item(i), true))
            continue;
        if (parser.InstructionID != roulette_inst_label_id)
            continue;

        //- Detects duplicate labels
        for (j=0 ; j<=line_max ; j++)
        {
            if (j == i)
                continue;
            if (script.Item(i) == script.Item(j))
            {
                LogError(wxString::Format(_("Duplicate instructions 'LABEL' are not authorized (%s)."), parser.Command.uniCStr()));
                script[j].Empty();
            }
        }
    }

    //-- Executes the script
    for (i=0 ; i<=line_max || m_stop_requested ; i++)       //The OR aims the display the warning just below
    {
current_statement:
        if (m_restart_requested)
            break;
        if (m_stop_requested)
        {
		    LogWarning(_("The execution of the script has been requested to stop."));
            break;
        }
        if (i > line_max)                   //Some IF ends a script
            break;
        if (!parser.Parse(script.Item(i), true))
            continue;

        //- Controls the instruction set when we are in init
        if (pInit && (     (parser.InstructionID == roulette_inst_bet_id)
                        || (parser.InstructionID == roulette_inst_clear_id)
                        || (parser.InstructionID == roulette_inst_debug_id)
                        || (parser.InstructionID == roulette_inst_restart_id)
                        || (parser.InstructionID == roulette_inst_save_id)
                        || (parser.InstructionID == roulette_inst_spin_id)
                        || (parser.InstructionID == roulette_inst_stat_id)
                    )
            )
        {
            LogError(wxString::Format(_("The instruction '%s' is not authorized in the section .INIT."), parser.Instruction.Upper().uniCStr()));
            script[i].Empty();
            continue;
        }
        if (!pInit && (parser.InstructionID == roulette_inst_randomize_id) && !parser.NoWarning)
        {
            LogWarning(_("The use of the instruction 'RANDOMIZE' in a sequence is not recommended."));
            continue;
        }

        //- Calls the debugger
    #ifndef _CONSOLE
        if (m_debugger_enabled && !m_debugger_skip)
        {
            wxASSERT(m_debugger!= NULL);
            m_debugger->SetState(&m_engine, &script, i);
            switch (m_debugger->ShowModal())
            {
                case ID_DEBUG_CONTINUE:
                    m_debugger_enabled = false;
                    break;
                case ID_DEBUG_JUMP:
                    //Nothing
                    break;
                case ID_DEBUG_VALIDATE:
                    m_debugger_enabled = false;
                    m_debugger_skip = true;
                    break;
                case ID_DEBUG_STOP:
                    m_debugger_enabled = false;
                    RequestStop();
                    goto current_statement;
                default:
                    wxASSERT(false);
                    break;
            }
        }
    #endif

        //- Processes the instructions in a predefined order
        parser.Checksum();
        switch (parser.InstructionID)
        {
            #include "roulette_label.cpp"           //Control flow
            #include "roulette_goto.cpp"
            #include "roulette_break.cpp"
            #include "roulette_restart.cpp"
            #include "roulette_stop.cpp"
            #include "roulette_leave.cpp"
            #include "roulette_if.cpp"
            #include "roulette_else.cpp"
            #include "roulette_endif.cpp"
            #include "roulette_pause.cpp"

            #include "roulette_debug.cpp"           //Technical
            #include "roulette_refresh.cpp"

            #include "roulette_set.cpp"             //Memory management
            #include "roulette_unset.cpp"
            #include "roulette_exist.cpp"
            #include "roulette_allocate.cpp"
            #include "roulette_clear.cpp"
            #include "roulette_stat.cpp"
            #include "roulette_bits.cpp"
            #include "roulette_backup.cpp"
            #include "roulette_rollback.cpp"

            #include "roulette_spin.cpp"            //Game play
            #include "roulette_bet.cpp"
            #include "roulette_buy.cpp"
            #include "roulette_random.cpp"
            #include "roulette_randomize.cpp"

            #include "roulette_plot.cpp"            //Verbosity
            #include "roulette_show.cpp"
            #include "roulette_write.cpp"           //Includes: status, popup, confirm, input
            #include "roulette_progress.cpp"
            #include "roulette_save.cpp"
            #include "roulette_time.cpp"
            #include "roulette_beep.cpp"
            #include "roulette_slap.cpp"
        }

        //- Other unknown commands
        LogError(wxString::Format(_("Unknown instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
    }
break_sequence:
    return true;
}

bool wxRoulette::DoDumpVariables(bool pNoWarning)
{
    wxArrayString list;
    size_t i;

    //-- Gets data
    m_engine.ListAllConstants(&list);
    if (list.GetCount() == 0)
    {
        if (!pNoWarning)
            LogMessage(_("There is no defined variable."));
        return false;
    }
    list.Sort();

    //-- Shows data
    LogMessage(_("The available variables are :"));
    for (i=0 ; i<list.GetCount() ; i++)
        LogMessage(wxString::Format(wxT("   %s"), list.Item(i).uniCStr()));
    return true;
}

bool wxRoulette::DoSpin(int pForcedSpin, int *pResult)
{
    bool status;
    unsigned long ulvalue;
    int landed, color;
    bool zero;

    //-- Checks
    if (pResult == NULL)
        return false;
    if (!m_table.IsValidSpin(pForcedSpin))
        pForcedSpin = roulette_invalid_spin;

    //-- Spins
    status = m_table.Spin(&ulvalue);            //Even if the spin is forced, the wheel is launched because the forced spin is considered as an overridden value
    if (!status)
        return false;
    landed = ulvalue;                           //The intervals are compatible
    if (pForcedSpin != roulette_invalid_spin)
        landed = pForcedSpin;
    color = m_table.GetColor(landed);
    zero = m_table.IsZero(landed);

    //-- Updates the internal variables
    m_engine.SetConstant(roulette_vars_landed_number, landed);
    m_engine.SetConstant(roulette_vars_landed_black,  (color == 0 ? 1 : 0));
    m_engine.SetConstant(roulette_vars_landed_red,    (color == 1 ? 1 : 0));
    m_engine.SetConstant(roulette_vars_landed_green,  (color == 2 ? 1 : 0));
    m_engine.SetConstant(roulette_vars_landed_zero,   (landed == roulette_zero ? 1 : (landed == roulette_zero_double ? 2 : (landed == roulette_zero_triple ? 3 : 0))));
    m_engine.SetConstant(roulette_vars_landed_odd,    (!zero && (landed % 2 == 1) ? 1 : 0));
    m_engine.SetConstant(roulette_vars_landed_even,   (!zero && (landed % 2 == 0) ? 1 : 0));
    m_engine.SetConstant(roulette_vars_landed_street, m_table.GetStreet(landed));
    m_engine.SetConstant(roulette_vars_landed_column, m_table.GetColumn(landed));
    m_engine.SetConstant(roulette_vars_landed_dozen,  m_table.GetDozen(landed));
    m_engine.SetConstant(roulette_vars_landed_half,   m_table.GetHalf(landed));

    //-- Final result
    *pResult = landed;
    return true;
}

bool wxRoulette::DoSaveSpinHistory(wxString pFileName)
{
    wxArrayString content;
    wxRouletteHistory *item;
    unsigned long i;

    //-- Checks
    if ((m_spin_history.GetCount() == 0) || pFileName.IsEmpty())
        return false;

    //-- Creates the content
    content.Clear();
    content.Add(wxT("Spin;Bet;Stake;Landed number;Win;Gain;Cash;Credit;Net result"));     //Don't write "ID" instead of "Spin"
    for (i=0 ; i<m_spin_history.GetCount() ; i++)
    {
        item = m_spin_history.GetItem(i);
        content.Add(wxString::Format(wxT("%d;%f;%f;%f;%f;%f;%f;%f;%f"), i+1, item->Bet, item->Stake, item->LandedNumber, item->Win, item->Gain, item->Cash, item->Credit, item->Net));
    }

    //-- Saves the content
    return wxRouletteHelper::SaveArrayStringToFile(pFileName, &content);
}

bool wxRoulette::DoSaveLogHistory(wxString pFileName)
{
    wxArrayString content;
    unsigned long i;
    wxString type, buffer;

    //-- Checks
    if ((m_log.GetCount() == 0) || pFileName.IsEmpty())
        return false;

    //-- Creates the content
    buffer.Alloc(512);
    content.Clear();
    content.Add(wxT("Type;Message"));
    for (i=0 ; i<m_log.GetCount() ; i++)
    {
        //- Splits into a readable CSV
        type = m_log.Item(i).GetChar(0);
        buffer = m_log.Item(i).Mid(1, m_log.Item(i).Len());
        if ((buffer.Find(wxT(';')) != wxNOT_FOUND) || (buffer.Find(wxT('"')) != wxNOT_FOUND))
        {
            buffer.Replace(wxT("\""), wxT("\"\""));
            buffer = wxString::Format(wxT("\"%s\""), buffer.uniCStr());
        }

        //- Writes
        content.Add(wxString::Format(wxT("%s;%s")
                            , type.uniCStr()
                            , buffer.uniCStr()
                    ));
    }

    //-- Saves the content
    return wxRouletteHelper::SaveArrayStringToFile(pFileName, &content);
}

void wxRoulette::DoUpdateDynamicVariables(bool pNoWarning)
{
    double bet, stake, cash, credit, maxbet;

    //-- After SET for example, the bet needs to be updated
    if (m_engine.GetConstant(roulette_vars_bet, &bet))
		if (!m_table.SetBet(bet))
			if (!pNoWarning)
				LogWarning(_("You can't bet with null or negative amounts."));
    bet = m_table.GetBet();
    m_engine.SetConstant(roulette_vars_bet, bet);

    //-- Updates the cash/credit on hand which cannot be undefined
    if (!m_engine.HasConstant(roulette_vars_cash))
        m_engine.SetConstant(roulette_vars_cash, 0);
    if (!m_engine.HasConstant(roulette_vars_credit))
        m_engine.SetConstant(roulette_vars_credit, 0);

    //-- Updates the stake and the status telling if we can bet more
    stake = m_table.GetStake();
	m_engine.SetConstant(roulette_vars_stake, stake);
    m_engine.GetConstant(roulette_vars_cash, &cash, 0);
    m_engine.GetConstant(roulette_vars_credit, &credit, 0);
    maxbet = GetMaxBet();
    if ((credit > 0) || (stake + bet > cash))
        m_engine.SetConstant(roulette_vars_can_bet, 0);
    else
        if (maxbet == 0)
            m_engine.SetConstant(roulette_vars_can_bet, 1);
        else
            m_engine.SetConstant(roulette_vars_can_bet, (stake + bet > maxbet ? 0 : 1));
}

bool wxRoulette::DoUpdateStatistics()
{
    wxRouletteHistory *history;
    unsigned long i, j, start, end;
    unsigned long green, black, red;
    double fvalue, min, max, average;
    unsigned long tmp, numbers[roulette_max_size], numbers_id[roulette_max_size];
    wxString buffer;

    //-- Checks
    //DoClearStats();                       //Not all stats are calculated here
    if (m_spin_history.GetCount() == 0)
        return false;

    //-- Initializes
    green = 0;
    black = 0;
    red = 0;
    min = 0;
    max = 0;
    average = 0;
    for (i=0 ; i<roulette_max_size ; i++)
    {
        numbers[i] = 0;
        numbers_id[i] = i;
    }

    //-- Processes data
    for (i=0 ; i<m_spin_history.GetCount() ; i++)
    {
        //- Reads the history
        history = m_spin_history.GetItem(i);
        wxASSERT(history != NULL);

        //- Color
        switch (m_table.GetColor(history->LandedNumber))
        {
            case 0:  black++;         break;
            case 1:  red++;           break;
            case 2:  green++;         break;
            default: wxASSERT(false); break;
        }

        //- Min/Max/Average cash
        fvalue = history->Cash - history->Credit;
        if (i == 0)
        {
            min = fvalue;
            max = fvalue;
        }
        else
        {
            if (fvalue < min)
                min = fvalue;
            if (fvalue > max)
                max = fvalue;
        }
        average += fvalue;

        //- Counts Hot/Cold numbers
        wxASSERT((0 <= history->LandedNumber) && (history->LandedNumber < roulette_max_size));
        numbers[(unsigned long)(history->LandedNumber)]++;
    }

    //-- Sets data
    m_engine.SetConstant(roulette_vars_stats_green, (double)green/(green + black + red));
    m_engine.SetConstant(roulette_vars_stats_black, (double)black/(green + black + red));
    m_engine.SetConstant(roulette_vars_stats_red,   (double)red  /(green + black + red));

    m_engine.SetConstant(roulette_vars_stats_net_max, max);
    m_engine.SetConstant(roulette_vars_stats_net_min, min);
    m_engine.SetConstant(roulette_vars_stats_net_avg, average/m_spin_history.GetCount());

    //-- Hot/Cold numbers
    //Sorts data
    start = (m_table.IsBalanced() ? 1 : 0);
    end = start + m_table.GetCellCount() - 1;
    for (i=start ; i<=end-1 ; i++)
        for (j=i+1 ; j<=end ; j++)
        {
            if (numbers[j] >= numbers[i])
            {
                tmp = numbers[j];
                numbers[j] = numbers[i];
                numbers[i] = tmp;

                tmp = numbers_id[j];
                numbers_id[j] = numbers_id[i];
                numbers_id[i] = tmp;
            }
        }
    //Stores data
    for (i=0 ; i<5 ; i++)   //TOP 5
    {
        //Hot
        buffer = wxString::Format(wxT("stat.hot%d"), i+1);
        if (numbers[start+i] == 0)
            m_engine.DeleteConstant(buffer);
        else
            m_engine.SetConstant(buffer, numbers_id[start+i]);

        //Cold
        buffer = wxString::Format(wxT("stat.cold%d"), i+1);
        if (numbers[end-i] == 0)
            m_engine.DeleteConstant(buffer);
        else
            m_engine.SetConstant(buffer, numbers_id[end-i]);
    }

    return true;
}

void wxRoulette::DoClearLandedNumber()
{
    wxArrayString list;
    wxString buffer;
    unsigned long j;

    //-- Clears the variables
    m_engine.ListAllConstants(&list, false);
    for (j=0 ; j<list.GetCount() ; j++)
    {
        buffer = list.Item(j);
        if (buffer.StartsWith(wxT("game.landed.")))
            m_engine.DeleteConstant(buffer);
    }
}

void wxRoulette::DoClearStats()
{
    wxArrayString list;
    wxString buffer;
    unsigned long j;

    //-- Removes the corresponding variables
    m_engine.ListAllConstants(&list, false);
    for (j=0 ; j<list.GetCount() ; j++)
    {
        buffer = list.Item(j);
        if (buffer.StartsWith(wxT("stat.")))
            m_engine.DeleteConstant(buffer);
    }
}

void wxRoulette::DoShowMoney()
{
    LogMessage(_("Statistics :"));
    LogMessage(wxString::Format(_("   - Cash : %.2f"), GetCash()));
    LogMessage(wxString::Format(_("   - Credit : %.2f"), GetCredit()));
    LogMessage(wxString::Format(_("   - Net result : %.2f"), GetCash()-GetCredit()));
    LogMessage(wxString::Format(_("   - Stake : %.2f"), m_table.GetStake()));
    LogMessage(wxString::Format(_("   - Current bet : %.2f"), GetBet()));
}

bool wxRoulette::DoBuyStakes(double pAmount)
{
    double cash, credit;

    //-- Checks
    if (pAmount < 0)
        return false;
    if (pAmount == 0)
        return true;

    //-- Gets data
    cash = GetCash();
    credit = GetCredit();

    //-- Calculates the new values
    credit -= pAmount;              //The credit is reimbursed first
    if (credit < 0)
    {
        cash += credit;
        credit = 0;
    }
   
    //-- Applies the amount
    m_engine.SetConstant(roulette_vars_cash, cash);
    m_engine.SetConstant(roulette_vars_credit, credit);
    //The history will be updated after the next spin
    return true;
}

void wxRoulette::DoClearLog()
{
    //-- Clears the memory
    m_log.Clear();

   //-- Clears the console
#ifdef _CONSOLE
    #ifdef __WXMSW__
        system("cls");
    #else
        system("clear");
    #endif
#endif
}

bool wxRoulette::DoLogMessage(wxRouletteMessageType::wxRouletteMessageType pType, wxString pMsg)
{
    wxString buffer;

    //-- Checks
    if ((pType == wxRouletteMessageType::NONE) || pMsg.IsEmpty())
        return false;

    //-- Shows the message in the console
#ifdef _CONSOLE
    static bool write_done = false;
    switch (pType)
    {
        case wxRouletteMessageType::WARNING_T:  buffer = _("Warning :");   break;
        case wxRouletteMessageType::ERROR_T:    buffer = _("Error :");     break;
        case wxRouletteMessageType::DEBUG_T:    buffer = _("Debug :");     break;
        case wxRouletteMessageType::SYSTEM_T:   buffer = _("System :");    break;
        default:                                buffer.Empty();            break;
    }
    if (!buffer.IsEmpty())
        buffer.Append(wxT(" "));        //It will facilitate the translations of the messages above
    buffer.Append(pMsg);
    #if wxUSE_UNICODE
        if (write_done)
            wprintf(wxT("\n"));
        wprintf(buffer.uniCStr());
    #else
        if (write_done)
            printf(wxT("\n"));
        printf(buffer.uniCStr());
    #endif
    write_done = true;
#endif

    //-- Adds the message
    buffer = wxRouletteHelper::MessageTypeToChar(pType) + pMsg;
    m_log.Add(buffer);
    return true;
}


//------------------------------------------

wxRoulette::wxRoulette()
{
    m_sequence_count = 0;
    m_sequences = NULL;
    m_script_running = false;
    m_stop_requested = false;
    m_leave_requested = false;
    m_restart_requested = false;
#ifndef _CONSOLE
    m_debugger = NULL;
    m_frame = NULL;
#endif
    m_table.SetParentClass(this);
    Reset();
}

wxRoulette::~wxRoulette()
{
#ifndef _CONSOLE
    wxDELETE(m_debugger);
#endif
    DoFreeSequences(true);
}

void wxRoulette::Reset()
{
    DoFreeSequences(false);
    DoResetConstants();
    DoUpdateDynamicVariables(true);
    m_stop_requested = false;
    m_leave_requested = false;
    m_restart_requested = false;
    m_spin_history.EmptyData();
    m_log.Clear();
    m_time_set = false;
}

bool wxRoulette::StartFromFile(wxString pFileName, bool pStart)
{
    wxString buffer;
    bool compatible;

    //-- Checks
    if (pFileName.IsEmpty())
        return false;
    if (wxRouletteHelper::GetFileExtension(pFileName) != roulette_file_ext)
        return false;

    //-- Sets the current directory
    wxRouletteHelper::SetWorkingDirectory(pFileName);

    //-- Loads the script from a file
    if (!wxRouletteHelper::LoadFileToArrayString(pFileName, &m_script))
    {
	    LogError(wxString::Format(_("The input file '%s' is not loaded."), pFileName.uniCStr()));
	    return false;
    }

    //-- Loads the script
    if (!DoLoad(&compatible))
        return false;
    if (!compatible)
    {
        LogSystem(_("The current script requires a newer version of the interpreter."));
        return false;
    }
    DoInitSequences();

    //-- Executes
    if (pStart)
        Execute();
    return true;
}

bool wxRoulette::StartFromInput(wxString pInput, bool pStart)
{
    wxString buffer;
    bool compatible;

    //-- Checks the format
    if (pInput.IsEmpty())
        return false;

    //-- Splits the input string
    if (!wxRouletteHelper::StringToScript(pInput, &m_script, false))
        return false;

    //-- Loads the script
    if (!DoLoad(&compatible))
        return false;
    if (!compatible)
    {
        LogSystem(_("The current script requires a newer version of the interpreter."));
        return false;
    }
    DoInitSequences();

    //-- Executes
    if (pStart)
        Execute();
    return true;
}

void wxRoulette::Execute()
{
    unsigned long seq_id, quota;
    wxRouletteSequence *sequence;

    //-- Checks
    wxASSERT(!m_script_running);

    //-- Reduces the priority of the main thread
#ifdef __WXMSW__
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
#endif

    //-- Initializes
    DoResetConstants();
    DoUpdateDynamicVariables(true);
    m_cash.EmptyData();
    m_credit.EmptyData();
    m_spin_history.EmptyData();
    m_backup.ResetConstants(false);
    m_log.Clear();
    m_time_set = false;
    m_script_running = true;
    m_leave_requested = false;
#ifndef _CONSOLE
    m_debugger_enabled = false;
#endif
    m_debugger_skip = false;

    //-- Processes each sequence
    for (seq_id=0 ; seq_id<m_sequence_count || m_restart_requested ; seq_id++)
    {
        //- Stops if requested
        if (m_stop_requested)
            break;

        //- Restarts
        if (m_restart_requested)
        {
            seq_id = 1;                         //RESTART never reprocesses section .INIT
            if (seq_id >= m_sequence_count)
                break;
            m_restart_requested = false;
        }

        //- Reads the sequence if it is loaded
        sequence = m_sequences[seq_id];
        if (sequence == NULL)
            continue;

        //- Executes the sequence
#if roulette_compile_msg_sequence == 1          //Too many messages are generated and it is worse for the performances
        LogSystem(wxString::Format(_("Starting sequence %d"), seq_id));
    #ifndef _CONSOLE
        if (m_frame != NULL)
            m_frame->SetStatusBarText(wxString::Format(_("Sequence %d"), seq_id), 1);
    #endif
#endif
        DoExecuteSequence(sequence, seq_id==0);
    }

    //-- Closes the debugger
#ifndef _CONSOLE
    if (m_debugger != NULL)
    {
        m_debugger->Destroy();
        wxDELETE(m_debugger);
        m_debugger = NULL;
    }
#endif

    //-- Resets some flags
    m_script_running = false;
    m_stop_requested = false;
    m_restart_requested = false;

    //-- Frees some data
    m_spin_history.EmptyData();

    //-- Controls the quota
    quota = GetTable()->GetRemainingQuota();
    if (quota == 0)
        LogError(_("You ran out of quota. Please change your pseudo-random number generator, or retry later."));
    else
        if (quota != (unsigned long)-1)
            LogSystem(wxString::Format(_("Online PRNG : your remaining quota is %d."), quota));

    //-- Resets the priority of the main thread
#ifdef __WXMSW__
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
#endif
}

bool wxRoulette::RequestStop()
{
    if (m_script_running)
    {
        m_stop_requested = true;
        return true;
    }
    else
        return false;
}

bool wxRoulette::RequestLeave()
{
    if (m_script_running)
    {
        RequestStop();
        m_leave_requested = true;
        return true;
    }
    else
        return false;
}

unsigned long wxRoulette::CountSequences()
{
    unsigned long result = m_sequence_count;
    if (result > 0)
        if (m_sequences[0] == NULL)
            result--;
    return result;
}

double wxRoulette::GetCash()
{
    double fvalue;
    m_engine.GetConstant(roulette_vars_cash, &fvalue, 0);
    return fvalue;
}

double wxRoulette::GetCredit()
{
    double fvalue;
    m_engine.GetConstant(roulette_vars_credit, &fvalue, 0);
    return fvalue;
}

double wxRoulette::GetBet()
{
    return m_table.GetBet();
}

double wxRoulette::GetMaxBet()
{
    double fvalue;
    if (!m_engine.GetConstant(roulette_vars_betmax, &fvalue))
        fvalue = 0;
    if (fvalue < 0)
        fvalue = 0;
    return fvalue;
}

bool wxRoulette::AppendLogMessage(wxString& pText)
{
    size_t maxid;

    //-- Checks
    if (pText.IsEmpty())
        return false;

    //-- Appends the text
    maxid = m_log.GetCount();
    if (maxid == 0)
        return DoLogMessage(wxRouletteMessageType::INFO_T, pText);
    else
    {
        //- Writes in the console
#ifdef _CONSOLE
    #if wxUSE_UNICODE
        wprintf(pText.uniCStr());
    #else
        printf(pText.uniCStr());
    #endif
#endif
        //- Stores the message
        m_log[maxid-1] += pText;
        return true;
    }
}
