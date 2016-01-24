
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

#ifndef sequence_h
#define sequence_h

/**
 * \file sequence.h
 * \brief Individual sequence to be processed
 * \author ecrucru
 * \version 0.1
 * \date October 2015
 *
 * A sequence is an individual phase of the game described by a script.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxEcMath/ec_defs.h"
#include "defs.h"


//------------------------------------------

class wxRoulette;           //Included at the end of this file


//------------------------------------------

/** \class wxRouletteSequence
 *  \brief Sequence defining a phase of the game.
 *
 * A sequence contains the strategy of the current phase of the game.
 */
class wxRouletteSequence
{
    private:
        wxArrayString   m_script;           /**< Script of the sequence. */
        wxRoulette      *m_parent;          /**< Parent class. */

    public:
        /** Constructor. */
        wxRouletteSequence(wxRoulette *pParent);
        /** Destructor. */
        ~wxRouletteSequence();
        /**
         * Sets a pointer to the parent class.
         * \param pParent Parent class.
         */
        inline void SetParent(wxRoulette *pParent) { m_parent = pParent; }

        /**
         * Add an instruction to the script.
         * \param pLine One complete line of script
         */
        void AddInstruction(wxString& pLine);
        void Reset();                       /**< Resets the sequence to its default empty content. */
        void DumpScript();                  /**< Displays the script of the sequence. */
        wxArrayString& GetScript();         /**< Return the script. */
};


//------------------------------------------

#include "roulette.h"

#endif
