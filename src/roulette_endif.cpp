
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

case roulette_inst_endif_id:
{
    //Checks the parameters
    if (parser.HasParameters())
        if (!parser.NoWarning)
            LogWarning(wxString::Format(_("No argument is expected for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));

    //Controls of the depth
    if (if_depth == 0)
    {
        LogError(wxString::Format(_("No opened instruction 'IF' for the instruction '%s'."), parser.Instruction.Upper().uniCStr()));
        script[i].Empty();
        continue;
    }

    //Closure of the IF
    if_depth--;
    continue;
}

#endif
