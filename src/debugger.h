
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

#ifndef debugger_h
#define debugger_h

/**
 * \file debugger.h
 * \brief Memory dump
 * \author ecrucru
 * \since Version 0.2, March 2016
 *
 * This file defines a window showing the state of the memory.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "engine.h"


//------------------------------------------

enum {
    ID_DEBUG_CONTINUE = 1000,
    ID_DEBUG_JUMP,
    ID_DEBUG_EXPORT,
    ID_DEBUG_STOP,
    ID_DEBUG_VALIDATE,
    ID_DEBUG_EVAL,

    ID_LIST_ALL,
    ID_LIST_DIFF
};


//------------------------------------------

/** \class wxRouletteDebugger
 *  \brief Memory dump.
 *
 * When the user calls the instruction "DEBUG MEMORY", it is connected to
 * this window showing contextual information.
 */
class wxRouletteDebugger : public wxDialog
{
    private:
        DECLARE_EVENT_TABLE()

        wxStaticText        *m_static_context, *m_static_eval;
        wxComboBox          *m_combo_eval;
        wxListBox           *m_list, *m_list_diff;
        wxButton            *m_button_eval, *m_button_continue, *m_button_export,
                            *m_button_jump, *m_button_validate, *m_button_stop;

        wxArrayString       m_data;
        wxRouletteEngine    *m_engine_ref;

        void DoEvalExpression();
        bool DoExportMemory();

    public:
        /**
         * Constructor.
         * \param pParent Parent window
         */
        wxRouletteDebugger(wxWindow *pParent);
        /**
         * Destructor.
         */
        ~wxRouletteDebugger();

        /**
         * Sets the state.
         * \param pEngine Mathematical parser
         * \param pScript Current processed script
         * \param pPosition Line of the script currently processed
         */
        void SetState(wxRouletteEngine *pEngine, wxArrayString *pScript, unsigned long pPosition);
        /**
         * Resets the debugger.
         */
        void Reset();

        /**
         * Event when clicking on a button.
         */
        void OnButtonClick(wxCommandEvent& event);
        /**
         * Event when double-clicking on a listbox.
         */
        void OnListDoubleClick(wxCommandEvent& event);
};

#endif
