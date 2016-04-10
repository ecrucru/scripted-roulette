
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

#ifndef instruction_h
#define instruction_h

/**
 * \file instruction.h
 * \brief Parser for 1 line of script
 * \author ecrucru
 * \since Version 0.1, October 2015
 *
 * This file parses the script to help its execution.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/tokenzr.h"

#include "wxEcMath/ec_defs.h"
#include "defs.h"


//------------------------------------------

/** \class wxRouletteInstruction
 *  \brief Parser and cutter for the incoming lines of the script.
 *
 * The parsing cuts a line of script into individual readable elements.
 */
class wxRouletteInstruction
{
    public:
        bool          NoWarning;                    /**< An instruction can prevent some warnings to be generated. */
        wxString      Instruction;                  /**< Instruction. */
        unsigned long InstructionID;                /**< Integer representation of the instruction. */
        wxString      Command;                      /**< Parameters for the instruction on a single line. */
        wxArrayString CommandList;                  /**< All the individual parameters separated in a list. */

        /** Constructor. */
        wxRouletteInstruction();

        /** Resets the internal variables of the class. */
        void Reset();
        /**
         * Returns the instruction and its parameters after the changes you made.
         * \return Full instruction.
         */
        wxString GetAsString();

        /** Parses and cuts the instruction. The result are accessible in the public variables.
         * \param pInstruction Instruction to parse and cut into individual elements.
         * \param pChecksum Indicates if the internal representation of the instruction should be calculated.
         * \return \a true if successful, else \a false.
         */
        bool Parse(wxString pInstruction, bool pChecksum);

        /** Tells if there are existing parameters for the instruction.
         * \return \a true if there are parameters in line or in the list, else \a false.
         */
        bool HasParameters();

        /** Tells how many existing parameters are available for the instruction. */
        size_t CountParameters();

        /**
         * This function adds a parameter to the command line.
         * \param pString parameter.
         */
        void AddParameter(wxString pString);
        /**
         * This function changes parameters associated to a command.
         * \param pOld Old value.
         * \param pNew New value.
         * \return \a true if some values have been edited, else \a false.
         */
        bool ChangeParameter(wxString pOld, wxString pNew);
        /**
         * In few instructions, some values are simply visual. This function allows to remove them.
         * \param pString Values to remove.
         * \return \a true if some values have been removed, else \a false.
         */
        bool RemoveParameter(wxString pString);
        /**
         * This function replaces one or several characters from the arguments by new ones.
         * \param pOld Old characters.
         * \param pNew New characters.
         * \return \a true if a replacement occurred, else \a false.
         */
        bool ChangeCharacters(wxString pOld, wxString pNew);
        /**
         * This function removes one or several characters from the arguments.
         * \param pChars Several characters to be replaced.
         * \return \a true if a replacement occurred, else \a false.
         */
        bool RemoveCharacters(wxString pChars);
        /**
         * Removes all the parameters associated to the instruction.
         */
        void ClearParameters();
        /**
         * After the list of arguments is edited, it is often necessary to rebuild
         * its representation as a string.
         * \see CommandList
         */
        void RebuildCommandLine();
        /**
         * Converts the instruction into an integer. The formula is simple because
         * there is not a lot of instructions. Each position is multiplied by the
         * character code and finally summed.
         * \return Identifier of the instruction.
         */
        unsigned long Checksum();

};

#endif
