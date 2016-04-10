
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

case roulette_inst_allocate_id:
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

    //Preparation
    parser.CommandList[0] = wxEmptyString;
    parser.RebuildCommandLine();

    //Evaluates each term of the formula
    tokenizer.SetString(parser.Command, wxT(" "));
    ulmax = tokenizer.CountTokens();
    if (ulmax < 1)
    {
        wxASSERT(false);
        continue;
    }
    ul_alloc_size = (unsigned long*) malloc(ulmax * sizeof(unsigned long));
    ul_alloc_loop = (unsigned long*) malloc(ulmax * sizeof(unsigned long));
    b = (ul_alloc_size != NULL) && (ul_alloc_loop != NULL);
    if (b)
    {
        j = 0;
        while (tokenizer.HasMoreTokens())
        {
            b = false;
            if (m_engine.SetFormula(tokenizer.GetNextToken()))
            {
                m_engine.Compute();
                if (m_engine.GetLastError() == wxECE_NOERROR)
                {
                    fvalue = m_engine.GetLastResult();
                    ulvalue = (fvalue < 0 ? 0 : fvalue);    //Cast
                    if (ulvalue > 0)                        //An array must have a correct positive size
                    {
                        b = true;
                        ul_alloc_size[j] = ulvalue;
                        ul_alloc_loop[j] = ulvalue;
                    }
                }
                j++;
            }
            if (!b)
                break;
        }
    }
    if (!b)
    {
        LogError(_("The array cannot be allocated."));
        script[i].Empty();
        goto allocate_free_memory;
    }

    //Verifies the size of the array to allocate
    ulvalue = 1;
    for (j=0 ; j<ulmax ; j++)
        ulvalue *= ul_alloc_size[j];
    if ((ulvalue > 0x4000) && !parser.NoWarning)
    {
        LogWarning(_("The allocation of the array is aborted because of its size. Use the sign '@' to force anyway."));
        script[i].Empty();
        goto allocate_free_memory;
    }

    //Allocates the multi-dimensional array
    while (true)
    {
        j = ulmax-1;

        //- Sets the initial value of the variable
        buffer = name;
        b = true;
        for (k=0 ; k<ulmax ; k++)
        {
            if (ul_alloc_loop[k] == 0)      //The index is from 1 to N, but we are looping from 0 to N, so we have to skip the indexes equal to 0
            {
                b = false;
                break;
            }
            else
                buffer.Append(wxString::Format(wxT("_%d"), ul_alloc_loop[k]));
        }
        if (b)
            m_engine.SetConstant(buffer, 0.0);

        //Loop
        if (ul_alloc_loop[j] > 0)
            ul_alloc_loop[j]--;
        else
        {
            if (j == 0)
                break;

            k = j-1;
            do {
                if (ul_alloc_loop[k] > 0)
                {
                    ul_alloc_loop[k]--;
                    break;
                }
                else
                {
                    if (k == 0)
                        goto allocate_free_memory;
                    else
                        k--;
                }
            } while (true);

            for (k++ ; k<=j ; k++)
                ul_alloc_loop[k] = ul_alloc_size[k];
        }
    }

    //Frees the memory
allocate_free_memory:
    free(ul_alloc_size);
    free(ul_alloc_loop);
    continue;
}

#endif
