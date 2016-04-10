
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

case roulette_inst_goto_id:
{
    //Checks the parameters
    if (parser.CountParameters() < 1)
    {
        LogError(wxString::Format(_("At least 1 argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Goes to
    buffer = wxString::Format(wxT("label %s"), parser.Command.uniCStr());
    for (j=0 ; j<=line_max ; j++)
    {
        if (script.Item(j) == buffer)
        {
            //Finds the depth of the target instruction
            if_look_depth = 0;
            for (k=0 ; k<j ; k++)
            {
                buffer = script.Item(k);
                if (wxRouletteHelper::GetInstructionName(buffer) == roulette_inst_if)
                    if_look_depth++;
                if (wxRouletteHelper::GetInstructionName(buffer) == roulette_inst_endif)
                    if_look_depth--;
            }
            //Warns if we jump to a deeper instruction
            if (if_look_depth > if_depth)
                if (!parser.NoWarning)
                    LogWarning(_("Jumping to a deeper instruction may be unsafe."));
            //Adapts the IF before jumping to another instruction
            i = j;
            if_depth = if_look_depth;
            goto current_statement;
        }
    }

    //Target instruction not found
    LogError(_("Target label not found for the instruction 'GOTO'."));
    script[i].Empty();
    continue;
}

#endif
