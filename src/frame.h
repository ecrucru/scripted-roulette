
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

#ifndef frame_h
#define frame_h

/**
 * \file frame.h
 * \brief User interface
 * \author ecrucru
 * \version 0.1
 * \date October 2015
 *
 * This file contains the user interface.
 *
 * \todo C++ code generator (the mapping with wxRouletteTable is tough)
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/aboutdlg.h"
#include "wx/dnd.h"
#include "wx/grid.h"
#include "wx/notebook.h"
#include "wx/statline.h"
#include "wx/tglbtn.h"
#include "wx/clipbrd.h"
//_include "wx/mimetype.h"

#include "wxEcMath/ec_defs.h"
#include "wxEcMath/ec_plot.h"

#include "defs.h"
#include "roulette.h"

#ifdef wxUSE_STC
    #include "wx/stc/stc.h"         //Define after "defs.h"
#endif


//------------------------------------------

/** \class wxRouletteFrame
 *  \brief User interface.
 *
 * The user interface allows the user to maintain its script and execute it in live.
 */
class wxRouletteFrame : public wxFrame
{
	private:
        //-- Events
        DECLARE_EVENT_TABLE()
		enum
        {
            ID_MENU_NEW = wxID_HIGHEST + 1,
			ID_MENU_OPEN,
			ID_MENU_SAVE,
            ID_MENU_SAVEAS,
            ID_MENU_EXPORT,

			ID_MENU_PRETTY,
			ID_MENU_RESET,
			ID_MENU_EXECUTE,
            ID_MENU_STOP,

			ID_MENU_QUIT,

			ID_MENU_HELP,
			ID_MENU_ABOUT,

            ID_BUTTON_TOGGLE_ALL,
            ID_BUTTON_TOGGLE,

            ID_POPUP_COPY
		};

        //-- UI
		wxNotebook      *m_notebook;
		wxPanel         *m_tab_script, *m_tab_memory, *m_tab_console, *m_tab_money;
    #ifdef wxUSE_STC
        wxStyledTextCtrl *m_script_view;
    #else
		wxTextCtrl      *m_script_view;
    #endif
        wxListBox       *m_listbox_variables;
        wxGrid          *m_grid;
        wxToggleButton  *m_toggle_all, *m_toggle_info, *m_toggle_warning, *m_toggle_error,
                        *m_toggle_debug, *m_toggle_system;
		wxEcPlot        *m_plot;
        wxGauge         *m_gauge;

        //-- Toolbar
        wxToolBar       *m_toolbar;

        //-- Menus
		wxMenuBar       *m_menubar;
		wxMenu          *m_menu_file, *m_menu_help;
        wxMenu          *m_popup_grid;

        //-- Statusbar
        wxStatusBar     *m_statusbar;

        //-- Internal variables
        wxString        m_current_file;
        time_t          m_current_file_mod_time;
        wxRoulette      m_roulette;
        unsigned char   m_log_filter;

        //-- Private methods
        bool DoNew();                                       /**< Method associated to "New". */
        bool DoSave();                                      /**< Method associated to "Save". */
        bool DoSaveAs();                                    /**< Method associated to "Save as...". */
        void DoExport();                                    /**< Method associated to "Export". */
        void DoPrettyScript();                              /**< Method associated to "Pretty script". */
        void DoReset();                                     /**< Method associated to "Reset". */
        bool DoFitAxis(wxEcAxis* pAxis);                    /**< Method to fit the layout of an axis. */
        void DoQuit();                                      /**< Method associated to "Quit". */
        bool DoOpenPDFFile(wxString pFullName);             /**< Method to open a found PDF file. */
        bool DoFindPdfHelp(wxString pFileName);             /**< Method to search for the help file. */
        void DoHelp();                                      /**< Method associated to "Help". */
        bool DoCopyGrid();                                  /**< Method associated to "Copy the selected cells". */
        void DoToggleLogMessages();                         /**< Method associated to the buttons switching the log. */

        //-- Editor
        bool m_editor_status;                               /**< Status of the editor. */
        bool DoEditorIsModified();                          /**< Method to check is the editor is modified. */
        void DoEditorSetModified(bool pStatus);             /**< Method to define the status of the editor. */
        wxString DoEditorGetText();                         /**< Method to get the content of the script. */
        void DoEditorSetText(wxString pText);               /**< Method to set the content of the script. */
        void DoEditorClear();                               /**< Method to clear the content of the script. */

	public:
        /** Constructor */
		wxRouletteFrame(wxWindow *pParent, wxWindowID pId, bool pDefaultSample);
        /** Destructor */
		~wxRouletteFrame();

        /**
         * Updates the title of the main frame.
         * \param pRunningForced Tells that the script is about to start or is started, whatever
         *                       the real status of the execution
         */
        void UpdateTitle(bool pRunningForced = false);
        /**
         * Updates the fold level of each line of the script.
         * \return \a true if successful, else \a false.
         * \see http://www.scintilla.org/ScintillaDoc.html#Folding
         */
        bool UpdateFolding();
        /** Updates the log displayed in a grid. */
        void UpdateLog();
        /** Shows the message types by count. */
        void ShowMessageTypeCount();
        /** Updates the variables showing the memory. */
        void UpdateMemory();
        /** Sets the view of the script editor. */
        void FocusScript();
        /** Resets the plot showing the evolution of the money. */
        void ResetPlot();
        /** Resets the texts of the status bar. */
        void ClearStatusBar();
        /**
         * Sets a text in the status bar at the given position.
         * \param pText Text to display.
         * \param pIndex ID of the column.
         * \return \a true if the text has changed, else \a false.
         */
        bool SetStatusBarText(wxString pText, int pIndex);
        /**
         * Sets the progression of the status bar.
         * \param pPercent Percentage of progress.
         */
        void SetProgression(unsigned char pPercent);

        /**
         * Opens a file into the editor.
         * \param pFileName Name of the file to load.
         * \return \a true on success, else \a false.
         */
        bool OpenFile(wxString pFileName = wxEmptyString);
        /**
         * Executes the loaded script.
         */
        void Execute();
        /**
         * Tells if the current script contains instructions able to cause infinite loops.
         * \return \a true if the script contains possible loops, else \a false.
         */
        bool HasLoop();
        /**
         * Shows the informations about the software.
         */
        static void About();

        /** Event handling for a menu item. */
        void OnMenuClick(wxCommandEvent& event);
        /** Event handling in a wxGrid. */
        void OnGridCellRightClick(wxGridEvent& event);
        /** Event handling for the activation (focus) of the main window. */
        void OnActivate(wxActivateEvent& event);
        /** Event handling for the closure of the main window. */
        void OnClose(wxCloseEvent& event);
        /** Event handling for a toggle button. */
        void OnToggle(wxCommandEvent& event);
    #ifdef wxUSE_STC
        /** Event handling for the editor in a wxStyledTextCtrl. */
        void OnStyledText(wxStyledTextEvent& event);
        /** Event handling for the margin in a wxStyledTextCtrl. */
        void OnMarginClick(wxStyledTextEvent& event);
    #else
        /** Event handling for edition in a wxTextCtrl. */
        void OnText(wxCommandEvent& event);
    #endif
        /** Mouse event on the wxEcPlot. */
        void OnMouseEvent(wxMouseEvent& event);
};


//------------------------------------------

#include "drop.h"


#endif
