
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

#ifndef drop_h
#define drop_h

/**
 * \file drop.h
 * \brief Drag'n'drop
 * \author ecrucru
 * \since Version 0.1, November 2015
 *
 * This file contains the drag'n'drop functionality.
 */


//------------------------------------------

/** \class wxRouletteFileDrop
 *  \brief Drag and drop for files.
 *
 * This class catches the files being dropped into the main window.
 */
class wxRouletteFileDrop : public wxFileDropTarget
{
    private:
        wxRouletteFrame *m_frame;

    public:
        /** Constructor. */
        wxRouletteFileDrop()
        {
            m_frame = NULL;
        }

        /**
         * Defines the parent frame to callback the opening of the dropped file.
         * \param pFrame Parent class.
         */
        inline void SetParent(wxRouletteFrame* pFrame) { m_frame = pFrame; }

        /**
         * Event catching the list of files.
         */
        bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
        {
            size_t i;
            wxString file;
            bool b;

            //-- Fetches a compatible file
            file.Empty();
            for (i=0 ; i<filenames.GetCount() ; i++)
            {
                file = filenames.Item(i);
                if (file.EndsWith(roulette_file_ext_dot) || file.EndsWith(roulette_file_ext_inc_dot))
                    break;
            }

            //-- Loads the dropped file
            if ((m_frame != NULL) && !file.IsEmpty())
            {
                b = m_frame->OpenFile(file);
                if (b)
                {
                    m_frame->FocusScript();
                    m_frame->Raise();
                }
                return b;
            }
            else
                return false;
        }
};

#endif
