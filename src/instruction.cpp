
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

#include "instruction.h"


//------------------------------------------

wxRouletteInstruction::wxRouletteInstruction()
{
    Reset();
}


//------------------------------------------

void wxRouletteInstruction::Reset()
{
    NoWarning = false;
    Instruction.Empty();
    Command.Empty();
    CommandList.Clear();
}

wxString wxRouletteInstruction::GetAsString()
{
    wxString buffer = wxString::Format(wxT("%s %s"), Instruction.uniCStr(), Command.uniCStr()).Trim(true);
    if (NoWarning)
        return wxT("@") + buffer;
    else
        return buffer;
}

bool wxRouletteInstruction::Parse(wxString pInstruction)
{
    wxStringTokenizer list;

    //-- Initializes
    Reset();

    //-- Checks
    if (pInstruction.IsEmpty())
        return false;

    //-- Instruction name
    Instruction = wxRouletteHelper::GetInstructionName(pInstruction);
    if (Instruction.Len() == pInstruction.Len())
        goto FinalizeInstruction;
    else
        pInstruction = pInstruction.SubString(Instruction.Len(), pInstruction.Len()).Trim(false);

    //-- Command line
    Command = pInstruction;

    //-- Command line in parts
    list.SetString(pInstruction, wxT(" "));
    while (list.HasMoreTokens())
        CommandList.Add(list.GetNextToken());

    //-- Final result
FinalizeInstruction:
    NoWarning = Instruction.StartsWith(wxT("@"));
    if (NoWarning)
        Instruction = Instruction.SubString(1, Instruction.Len());
    return !Instruction.IsEmpty();
}

bool wxRouletteInstruction::HasParameters()
{
    return (CountParameters() > 0);
}

size_t wxRouletteInstruction::CountParameters()
{
    if (Instruction.IsEmpty())
        return 0;
    else
        return CommandList.GetCount();
}

void wxRouletteInstruction::AddParameter(wxString pString)
{
    CommandList.Add(pString);
    RebuildCommandLine();
}

bool wxRouletteInstruction::ChangeParameter(wxString pOld, wxString pNew)
{
    size_t i;
    bool changed;

    //-- Checks
    if (pOld.IsEmpty() && pNew.IsEmpty())
        return false;

    //-- Updates
    changed = false;
    if (CommandList.GetCount() > 0)
        for (i=CommandList.GetCount() ; i>0 ; i--)
            if (CommandList.Item(i-1) == pOld)
            {
                CommandList[i-1] = pNew;
                changed = true;
            }

    //-- Rebuilds the command line
    if (changed)
        RebuildCommandLine();

    //-- Result
    return changed;
}

bool wxRouletteInstruction::RemoveParameter(wxString pString)
{
    size_t i;
    bool changed;

    //-- Initializes
    changed = false;

    //-- Removes the matching entries
    if (CommandList.GetCount() > 0)
        for (i=CommandList.GetCount() ; i>0 ; i--)
            if (CommandList.Item(i-1) == pString)
            {
                CommandList.RemoveAt(i-1);
                changed = true;
            }

    //-- Rebuilds the command line
    if (changed)
        RebuildCommandLine();

    //-- Result
    return changed;
}

bool wxRouletteInstruction::ChangeCharacters(wxString pOld, wxString pNew)
{
    size_t i;
    bool changed;

    //-- Edits the matching entries
    changed = false;
    for (i=0 ; i<CommandList.GetCount() ; i++)
        if (CommandList.Item(i).Find(pOld) != wxNOT_FOUND)
        {
            CommandList.Item(i).Replace(pOld, pNew);
            changed = true;
        }

    //-- Rebuilds the command line
    if (changed)
        RebuildCommandLine();

    //-- Result
    return changed;
}

bool wxRouletteInstruction::RemoveCharacters(wxString pChars)
{
    return ChangeCharacters(pChars, wxEmptyString);
}

void wxRouletteInstruction::ClearParameters()
{
    Command.Empty();
    CommandList.Clear();
}

void wxRouletteInstruction::RebuildCommandLine()
{
    size_t i;

    //-- Rebuilds
    Command.Empty();
    for (i=0 ; i<CommandList.GetCount() ; i++)
    {
        if (!Command.IsEmpty())
            Command.Append(wxT(" "));
        Command.Append(CommandList.Item(i));
    }
}
