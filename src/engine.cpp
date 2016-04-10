
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

#include "engine.h"


//------------------------------------------

wxRouletteEngine::wxRouletteEngine()
{
    m_basic_formula.Clear();
}


//------------------------------------------

bool wxRouletteEngine::SetFormula(wxString pExpression)
{
    //-- Checks the input string
    if ( pExpression.IsEmpty()
     || (wxRouletteHelper::CountCharInString(pExpression, wxT('(')) != wxRouletteHelper::CountCharInString(pExpression, wxT(')')))
     || (wxRouletteHelper::CountCharInString(pExpression, wxT('[')) != wxRouletteHelper::CountCharInString(pExpression, wxT(']')))
       )
    {
        m_basic_formula.Empty();
        return false;
    }
    else
    {
        m_basic_formula = pExpression;
        return true;
    }
}

bool wxRouletteEngine::GetCompactExpression(wxString *pExpression)
{
    wxString buffer, arraypos;
    int start, end;
    size_t i;
    bool b;

    //-- Checks
    if ((pExpression == NULL) || (m_basic_formula.IsEmpty()))
        return false;

    //-- Processes the square brackets which defines the arrays
    buffer = m_basic_formula;
reprocess_brackets:
    b = true;
    start = 0;
    //end = buffer.Len();
    for (i=0 ; i<buffer.Len() ; i++)
    {
        //- Finds an opening square bracket
        if (buffer.GetChar(i) == wxT('['))
        {
            start = i;
            continue;
        }

        //- Finds a closing square bracket
        if (buffer.GetChar(i) == wxT(']'))
        {
            end = i;

            //Evaluates the square bracket
            if (!wxEcEngine::SetFormula(buffer.Mid(start+1, end-start-1)))
                b = false;
            else
            {
                wxEcEngine::Compute();
                b = (GetLastError() == wxECE_NOERROR);
            }

            //Determinates the index of the table
            if (b)
                arraypos = wxString::Format(wxT("_%d"), (long)(GetLastResult()));
            else
                arraypos = wxT("_");
            arraypos.Replace(wxT("-"), wxT("0"));

            //Formats the new expression
            buffer = buffer.Mid(0, start) + arraypos + buffer.Mid(end+1, buffer.Len());

            //Recursive lookup
            if (b)
                goto reprocess_brackets;
        }
    }

    //-- Result
    if (b)
        *pExpression = buffer;
    else
        pExpression->Empty();
    return b;
}

double wxRouletteEngine::Compute()
{
    wxString buffer;
    bool b;

    //-- Gets the compact expression without any square bracket
    b = false;
    buffer = m_basic_formula;
    if (this->SetFormula(buffer))
    {
        if (GetCompactExpression(&buffer))
        {
            wxASSERT(wxRouletteHelper::CountCharInString(buffer, wxT('[')) == 0);
            wxASSERT(wxRouletteHelper::CountCharInString(buffer, wxT(']')) == 0);

            //- Jump to the interpreter
            if (wxEcEngine::SetFormula(buffer))
            {
                wxEcEngine::Compute();
                b = (GetLastError() == wxECE_NOERROR);
            }
        }
    }

    //-- Returns the result
    return (b ? GetLastResult() : 0.0);
}

bool wxRouletteEngine::DeleteConstantMask(wxString& pMask)
{
    wxArrayString list;
    int i;
    bool del;

    //-- Retrieve all the constants
    ListAllConstants(&list, false);

    //-- Remove the matching ones
    if (list.Count() == 0)
        return false;
    else
    {
        del = false;
        i = list.Count()-1;
        do {
            if (list.Item(i).Matches(pMask))
            {
                if (DeleteConstant(list.Item(i)))
                    del = true;
            }
            if (i-- == 0)
                break;
        } while (true);
        return del;
    }
}
