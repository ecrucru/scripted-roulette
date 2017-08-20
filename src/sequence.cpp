
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

#include "sequence.h"


//------------------------------------------

wxRouletteSequence::wxRouletteSequence(wxRoulette *pParent)
{
    Reset();
    SetParent(pParent);
}

wxRouletteSequence::~wxRouletteSequence()
{
    Reset();
}


//------------------------------------------

void wxRouletteSequence::AddInstruction(wxString& pLine)
{
    m_script.Add(pLine);
}

void wxRouletteSequence::Reset()
{
    m_script.Clear();
}

void wxRouletteSequence::DumpScript()
{
    size_t i;

    if (m_script.GetCount() == 0)
    {
        if (m_parent != NULL)
            m_parent->LogMessage(_("No script is loaded for the sequence."));
    }
    else
    {
        if (m_parent != NULL)
            m_parent->LogMessage(_("The sequence contains the following instructions :"));
        for (i=0 ; i<m_script.GetCount() ; i++)
            if (!m_script.Item(i).IsEmpty())
                if (m_parent != NULL)
                    m_parent->LogMessage(wxString::Format(wxT("%.4d: %s"), i+1, m_script.Item(i).uniCStr()).uniCStr());
    }
}

wxArrayString& wxRouletteSequence::GetScript()
{
    return m_script;
}
