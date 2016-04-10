
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

#ifndef helper_h
#define helper_h

/**
 * \file helper.h
 * \brief Helper for simple tasks.
 * \author ecrucru
 * \since Version 0.1, October 2015
 *
 * This file collects simple and static functions to perform simple actions.
 */


//------------------------------------------

/** \class wxRouletteHelper
 *  \brief Static features.
 *
 * The class delivers some help to perform simple actions or conversions.
 */
class wxRouletteHelper
{
    public:
        //===== File functions
        /**
         * Extracts the extension of the provided file name.
         * \param pFileName File name.
         * \return Extension of the file name in lowercase.
         */
        static wxString GetFileExtension(wxString& pFileName)
        {
            return wxFileName(pFileName).GetExt().Lower().Trim(false).Trim(true);
        }

        /**
         * Returns the path of the application.
         * \return The full path with the separator at the end.
         */
        static inline wxString GetApplicationPath()
        {
            return wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPathWithSep();
        }

        /**
         * Returns the full name of the application.
         * \return The full name with the extension.
         */
        static inline wxString GetApplicationFile()
        {
            return wxStandardPaths::Get().GetExecutablePath();
        }

        /**
         * Sets the working directory with no error message.
         * \param pInput A string to process
         * \return \a true if successful, else \a false
         */
        static bool SetWorkingDirectory(wxString& pInput)
        {
            wxString buffer;

            //-- Checks
            if (pInput.IsEmpty())
                return false;

            //-- Gets the path of the file
            if (::wxFileExists(pInput))
                buffer = wxFileName(pInput).GetPathWithSep();
            else
                buffer = pInput;

            //-- Sets the path
            if (::wxIsAbsolutePath(buffer))
            {
                wxSetWorkingDirectory(buffer);
                return true;
            }
            else
                return false;
        }

        /**
         * Loads the content of a file into an array of string.
         * \param pFileName Name of the file to load.
         * \param pArray Pointer to the receiving object.
         * \return \a true if successful, else \a false.
         */
        static bool LoadFileToArrayString(wxString& pFileName, wxArrayString* pArray)
        {
            wxTextFile finput;
            wxString buffer;

            //-- Checks
            if (pArray == NULL)
                return false;

            //-- Loads the file into an internal list
            pArray->Clear();
            finput.Open(pFileName, wxConvISO8859_1);
            if (finput.IsOpened())
            {
                for (buffer = finput.GetFirstLine() ; !finput.Eof() ; buffer = finput.GetNextLine() )
                    pArray->Add(buffer);
                finput.Close();
                return true;
            }
            else
                return false;
        }

        /**
         * Saves an array into a file.
         * \param pFileName Target file name.
         * \param pData Pointer to the content.
         * \return \a true if successful, else \a false.
         */
        static bool SaveArrayStringToFile(wxString& pFileName, wxArrayString* pData)
        {
            wxTextFile file;
            size_t i;

            //-- Checks
            if ((pData == NULL) || (pFileName.IsEmpty()))
                return false;

            //-- Deletes the target file
            if (::wxFileExists(pFileName))
                if (!::wxRemoveFile(pFileName))
                    return false;

            //-- Creates the new file
            if (file.Create(pFileName))
            {
                for (i=0 ; i<pData->Count() ; i++)
                    file.AddLine(pData->Item(i));
            #ifdef __WXMSW__
                file.Write(wxTextFileType_Dos);
            #else
                file.Write(wxTextFileType_Unix);
            #endif
                file.Close();
                return true;
            }
            else
                return false;
        }


        //===== Management of the instructions
        /**
         * Formats a line of script to make it readable by the parser.
         * \param pInstruction Line of script.
         * \return The modified version of the instruction.
         */
        static wxString FormatInstruction(wxString& pInstruction)
        {
            wxString from, to;
            wxChar car;
            bool string_on;
            size_t i;

            //-- Aligns
            from = pInstruction.Trim(true).Trim(false);

            //-- Simplifies the spaces
            from.Replace(wxT("\t"), wxT(" "), true);
            from.Replace(wxString(roulette_char_nbspace), wxT(" "), true);
            while (from.Replace(wxT("  "), wxT(" "), true) > 0);

            //-- Transforms the input
            to.Alloc(from.Len());
            string_on = false;
            for (i=0 ; i<from.Len() ; i++)
            {
                car = from.GetChar(i);

                //- Manages the comments
                if ((car == wxT('"')) && !string_on)
                    break;

                //- Manages the strings
                if (car == wxT('\''))
                    string_on = !string_on;

                //- Manages the normal case
                if (string_on || (car == wxT('\'')))
                    to.Append(car);
                else
                    to.Append(wxString(car).Lower());
            }
            return to.Trim(true).Trim(false);
        }

        /**
         * Returns the instruction code from the input line.
         * \param pInput Processed line of script.
         * \return The instruction.
         */
        static wxString GetInstructionName(wxString& pInput)
        {
            wxString buffer;
            int position, pos_temp;

            //-- Aligns
            buffer = pInput.Trim(false).Trim(true).Lower();

            //-- Finds the position of the breaking character
            position = buffer.Len();
            pos_temp = buffer.Find(wxT(" "));                           //WRITE 'Demo'
            if ((pos_temp != wxNOT_FOUND) && (pos_temp < position))
                position = pos_temp;
            pos_temp = buffer.Find(wxT("\""));                          //WRITE"No space before the comment
            if ((pos_temp != wxNOT_FOUND) && (pos_temp < position))
                position = pos_temp;
            pos_temp = buffer.Find(wxT("'"));                           //WRITE'Misspelled instruction' "Comment
            if ((pos_temp != wxNOT_FOUND) && (pos_temp < position))
                position = pos_temp;

            //-- Result
            return buffer.Mid(0, position).Trim(false).Trim(true);
        }

        /**
         * Returns the comment used in an instruction.
         * \param pInstruction Line of script.
         * \return A comment in an original format.
         */
        static wxString GetComment(wxString& pInstruction)
        {
            wxString from;
            wxChar car;
            bool string_on, found;
            size_t i;

            //-- Aligns
            from = pInstruction.Trim(true).Trim(false);

            //-- Simplifies the spaces
            from.Replace(wxT("\t"), wxT(" "), true);
            from.Replace(wxString(roulette_char_nbspace), wxT(" "), true);
            while (from.Replace(wxT("  "), wxT(" "), true) > 0);

            //-- Transforms the input
            string_on = false;
            found = false;
            for (i=0 ; i<from.Len() ; i++)
            {
                car = from.GetChar(i);

                //- Manages the comments
                if ((car == wxT('"')) && !string_on)
                {
                    found = true;
                    break;
                }

                //- Manages the strings
                if (car == wxT('\''))
                    string_on = !string_on;
            }

            //-- Result
            if (found)
                return from.Mid(i+1, from.Len());
            else
                return wxEmptyString;
        }


		//===== Memory management
        /**
         * Loads a string into a script.
         * \param pInput Input string.
         * \param pArray Target array of strings.
         * \param pKeepLineID Don't remove the blank lines in excess.
         * \return \a true if successful, else \a false.
         * \remark It is just a technical conversion from wxString to wxArrayString.
         * \see ScriptToString
         */
        static bool StringToScript(wxString& pInput, wxArrayString* pArray, bool pKeepLineID)
        {
            wxString buffer;

            //-- Checks
            if (pArray == NULL)
                return false;
            buffer = pInput;

            //-- Processes
            pArray->Clear();
            buffer.Replace(wxString(roulette_char_nbspace), wxString(roulette_char_space), true);   //The special spaces can be generated with the keyword 'space' only
            buffer.Replace(wxT("\r\n"), wxT("\n"), true);                           //Normally not needed but safer
            buffer.Replace(wxT("\r"), wxEmptyString, true);
            if (!pKeepLineID)
                while (buffer.Replace(wxT("\n\n\n"), wxT("\n\n"), true) > 0);       //To clear some extra lines in advance
            wxStringTokenizer tokenizer(buffer, wxT("\n"), wxTOKEN_RET_EMPTY);
            while (tokenizer.HasMoreTokens())
                pArray->Add(tokenizer.GetNextToken());
            return true;
        }
        /**
         * Converts an array of strings into a string delimited with '\n'.
         * \param pArray Source array of strings.
         * \return The concatened result.
         * \remark It is just a technical conversion from wxArrayString to wxString.
         * \see StringToScript
         */
        static wxString ScriptToString(wxArrayString& pArray)
        {
            wxString buffer;
            size_t i;

            //-- Converts the table
            buffer.Empty();
            for (i=0 ; i<pArray.Count() ; i++)
            {
                if (!buffer.IsEmpty())
                    buffer.Append(wxT("\n"));
                buffer.Append(pArray.Item(i));
            }
            return buffer;
        }
        /**
         * A variable is expected to be named like "domain.name(.subname(.(...)))".
         * The function retrieves the "domain".
         * \return A string corresponding to the domain of the variable.
         */
        static wxString GetVariableDomain(wxString& pString)
        {
            int position = pString.Find(wxT("."));
            if (position == wxNOT_FOUND)
                return wxEmptyString;
            else
                return pString.Mid(0, position);
        }

        /**
         * A variable is expected to be named like "domain.name(.subname(.(...)))".
         * The function retrieves the "name" and eventual subnames.
         * \return A string corresponding to the full name of the variable.
         */
        static wxString GetVariableName(wxString& pString)
        {
            int position = pString.Find(wxT("."));
            if (position == wxNOT_FOUND)
                return wxEmptyString;
            else
                return pString.Mid(position+1, pString.Len());
        }


		//===== Numbers
        /**
         * Tells if a string is fully numeric.
         * \param pInput Input string.
         * \return \a true if the string is numeric, else \a false.
         * \remark The negative numbers are not supported.
         */
        static bool IsNumber(wxString& pInput)
        {
            size_t i, j;
            wxChar car;

            //-- Checks
            j = pInput.Len();
            if (j == 0)
                return false;

            //-- Compares
            for (i=0 ; i<j ; i++)
            {
                car = pInput[i];
                if ((car < wxT('0')) || (car > wxT('9')))
                    return false;
            }
            return true;
        }

        /**
         * Formats the number to be displayed.
         * \param pNumber Internal ID of the number.
         * \return A string representing the landed number.
         * \remark The zeros are managed correctly through this function.
         */
        static wxString FormatNumber(int pNumber)
        {
            if (pNumber == roulette_zero_double)
                return wxT("00");
            else
                if (pNumber == roulette_zero_triple)
                    return wxT("000");
                else
                    return wxString::Format(wxT("%d"), pNumber);
        }


		//===== Strings
        /**
         * Counts the number of times a character exists in a string.
         * \param pString Input string.
         * \param pCar Character to look for.
         * \return Number of occurences.
         */
        static unsigned long CountCharInString(wxString& pString, wxChar pCar)
        {
            unsigned long count, i;

            //-- Counts
            count = 0;
            for (i=0 ; i<pString.Len() ; i++)
                if (pString.GetChar(i) == pCar)
                    count++;
            return count;
        }

        /**
         * Converts some special HTML chars to render correctely in a file.
         * \param pInput Input string.
         * \return The converted string.
         */
        static wxString HtmlSpecialChars(wxString pInput)
        {
            pInput.Replace(wxT("&"), wxT("&amp;"));
            pInput.Replace(wxT("<"), wxT("&lt;"));
            pInput.Replace(wxT(">"), wxT("&gt;"));
            return pInput;
        }

        /**
         * Converts a quote into a double quote.
         * \param pInput Input string.
         * \return The converted string.
         */
        static wxString EscapeQuote(wxString pInput)
        {
            pInput.Replace(wxT("'"), wxT("''"), true);
            return pInput;
        }


		//===== Converters
        /**
         * Translates the algorithm as a string into an internal ID.
         * \param pInput Input string.
         * \return The algorithm ID or \a NONE if it is not recognized.
         */
        static wxRouletteAlgorithm::wxRouletteAlgorithm AlgorithmToID(wxString& pInput)
        {
            if (pInput == wxT("rand"))
                return wxRouletteAlgorithm::RAND;
            if (pInput == wxT("mt"))
                return wxRouletteAlgorithm::MT;
            if (pInput == wxT("tt800"))
                return wxRouletteAlgorithm::TT800;
            if (pInput == wxT("clock"))
                return wxRouletteAlgorithm::CLOCK;
            if (pInput == wxT("stdmin"))
                return wxRouletteAlgorithm::STDMIN;
            if (pInput == wxT("rs"))
                return wxRouletteAlgorithm::RS;
            if (pInput == wxT("randu"))
                return wxRouletteAlgorithm::RANDU;
            if (pInput == wxT("bits"))
                return wxRouletteAlgorithm::BITS;
            if (pInput == wxT("xor"))
                return wxRouletteAlgorithm::XOR;
            if (pInput == wxT("loaded"))
                return wxRouletteAlgorithm::LOADED;
            if (pInput == wxT("lngen"))
                return wxRouletteAlgorithm::LNGEN;
            if (pInput == wxT("equi"))
                return wxRouletteAlgorithm::EQUI;
        #ifdef __WXDEBUG__
            if (pInput == wxT("http_local"))
                return wxRouletteAlgorithm::HTTP_LOCAL;
        #endif
            if (pInput == wxT("http_random_org"))
                return wxRouletteAlgorithm::HTTP_RANDOM_ORG;
            if (pInput == wxT("custom"))
                return wxRouletteAlgorithm::CUSTOM;
            return wxRouletteAlgorithm::NONE;
        }

        /**
         * Indicates if the given algorithm is accessible online.
         * \return \a true if HTTP requests are necessary, or \a false.
         */
        static bool IsOnlineGenerator(wxRouletteAlgorithm::wxRouletteAlgorithm pAlgorithm)
        {
            return   (pAlgorithm == wxRouletteAlgorithm::HTTP_RANDOM_ORG)
                #ifdef __WXDEBUG__
                  || (pAlgorithm == wxRouletteAlgorithm::HTTP_LOCAL)
                #endif
            ;
        }
	
        /**
         * Indicates if the input is a sign.
         * \param pInput Input string.
         * \return \a true if it is a sign, else \a false.
         */
        static bool IsSign(wxString& pInput)
        {
            return  (pInput == wxT("="))  || (pInput == wxT("==")) ||
                    (pInput == wxT("!=")) || (pInput == wxT("<>")) ||
                    (pInput == wxT("<"))  || (pInput == wxT("<=")) || (pInput == wxT("=<")) ||
                    (pInput == wxT(">"))  || (pInput == wxT(">=")) || (pInput == wxT("=>")) ;
        }

        /**
         * Translates the sign as string into an internal ID.
         * \param pInput Input string.
         * \return The sign ID or \a NONE if it is not recognized.
         */
        static wxRouletteSign::wxRouletteSign SignToID(wxString& pInput)
        {
            if ((pInput == wxT("=")) || (pInput == wxT("==")))
                return wxRouletteSign::EQ;
            if ((pInput == wxT("!=")) || (pInput == wxT("<>")))
                return wxRouletteSign::NE;
            if ((pInput == wxT("<")))
                return wxRouletteSign::LT;
            if ((pInput == wxT("<=")) || (pInput == wxT("=<")))
                return wxRouletteSign::LE;
            if ((pInput == wxT(">")))
                return wxRouletteSign::GT;
            if ((pInput == wxT(">=")) || (pInput == wxT("=>")))
                return wxRouletteSign::GE;
            return wxRouletteSign::NONE;
        }

        /**
         * Indicates if the input is a logical operator.
         * \param pInput Input string <b>provided in lowercase</b>.
         * \return \a true if it is an operator, else \a false.
         */
        static bool IsOperator(wxString& pInput)
        {
            return  (pInput == wxT("and"))  ||
                    (pInput == wxT("or"))   ||
                    (pInput == wxT("xor"))  ||
                    (pInput == wxT("nand")) ||
                    (pInput == wxT("nor"));
        }

        /**
         * Translates the operator as string into an internal ID.
         * \param pInput Input string <b>provided in lowercase</b>.
         * \return The operator ID or \a NONE if it is not recognized.
         */
        static wxRouletteOperator::wxRouletteOperator OperatorToID(wxString& pInput)
        {
            if (pInput == wxT("and"))
                return wxRouletteOperator::AND;
            if (pInput == wxT("or"))
                return wxRouletteOperator::OR;
            if (pInput == wxT("xor"))
                return wxRouletteOperator::XOR;
            if (pInput == wxT("nand"))
                return wxRouletteOperator::NAND;
            if (pInput == wxT("nor"))
                return wxRouletteOperator::NOR;
            return wxRouletteOperator::NONE;
        }

        /**
         * Translates the message type into a character.
         * \param pInput Input type.
         * \return An equivalent character.
         */
        static wxString MessageTypeToChar(wxRouletteMessageType::wxRouletteMessageType pInput)
        {
            switch (pInput)
            {
                case wxRouletteMessageType::NONE:
                    return wxT(" ");
                case wxRouletteMessageType::INFO_T:
                    return wxT("I");
                case wxRouletteMessageType::WARNING_T:
                    return wxT("W");
                case wxRouletteMessageType::ERROR_T:
                    return wxT("E");
                case wxRouletteMessageType::DEBUG_T:
                    return wxT("D");
                case wxRouletteMessageType::SYSTEM_T:
                    return wxT("S");
                default:
                    wxASSERT(false);
                    return wxEmptyString;
            }
        }

        /**
         * Returns the minimal ID of the playable number.
         * \param pType Type of the roulette.
         * \return The ID of the expected number.
         * \remark This method serves to loop on the arrays.
         */
        static int GetMinNumberFromType(wxRouletteType::wxRouletteType pType)
        {
            if (pType == wxRouletteType::BALANCED)
                return 1;
            else
                return 0;
        }

        /**
         * Returns the maximal ID of the playable number.
         * \param pType Type of the roulette.
         * \return The ID of the expected number.
         * \remark This method serves to loop on the arrays.
         */
        static int GetMaxNumberFromType(wxRouletteType::wxRouletteType pType)
        {
            switch (pType)
            {
                case wxRouletteType::AMERICAN: return roulette_zero_double;
                case wxRouletteType::MEXICAN:  return roulette_zero_triple;
                default:                       return 36;
            }
        }


		//===== Miscellaneous
        /**
         * Gets the name of the C++ compiler based on the predefined constants of wxWidgets.
         * \return Name of the C++ compiler
         */
        static wxString GetCompiler()
        {
        #if defined __BORLANDC__
            return wxString::Format(wxT("Borland (R) C++ Compiler %d"), __BORLANDC__);
        #elif defined __clang__
            return wxT("CLang");
        #elif defined __VISUALC__
            return wxString::Format(wxT("Microsoft Visual C++ (R) %d"), __VISUALC__);
        #elif defined __MINGW32__
            return wxString::Format(wxT("MinGW %d"), __MINGW32__);
        #elif defined __GNUWIN32__
            return wxT("GNU-Win32 compiler");
        #elif defined __GNUG__
            return wxT("GNU C++");
        #else
            return wxT("Unnamed C++ compiler");
        #endif
        }
};

#endif
