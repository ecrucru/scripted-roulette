
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

#ifndef _CONSOLE

#include "frame.h"


//------------------------------------------
// Icons

#ifndef __WXMSW__
#include "res/mainicon.xpm"
#endif

#include "res/new.xpm"
#include "res/open.xpm"
#include "res/save.xpm"
#include "res/export.xpm"
#include "res/pretty.xpm"
#include "res/reset.xpm"
#include "res/execute.xpm"
#include "res/stop.xpm"


//------------------------------------------

    BEGIN_EVENT_TABLE(wxRouletteFrame, wxFrame)
        EVT_MENU                 (wxID_ANY, wxRouletteFrame::OnMenuClick)
        EVT_GRID_CELL_RIGHT_CLICK(          wxRouletteFrame::OnGridCellRightClick)
        EVT_ACTIVATE             (          wxRouletteFrame::OnActivate)
        EVT_CLOSE                (          wxRouletteFrame::OnClose)
        EVT_TOGGLEBUTTON         (wxID_ANY, wxRouletteFrame::OnToggle)
    #ifdef wxUSE_STC
        EVT_STC_CHANGE           (wxID_ANY, wxRouletteFrame::OnStyledText)
        EVT_STC_MARGINCLICK      (wxID_ANY, wxRouletteFrame::OnMarginClick)
    #else
        EVT_TEXT                 (wxID_ANY, wxRouletteFrame::OnText)
    #endif
    END_EVENT_TABLE()


wxRouletteFrame::wxRouletteFrame(wxWindow *pParent, wxWindowID pId, bool pDefaultSample)
               : wxFrame(pParent, pId, wxEmptyString, wxDefaultPosition, wxSize(600, 400), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
    wxString buffer;

	//-- Layout
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	wxBoxSizer *VSizer = new wxBoxSizer(wxVERTICAL);
		m_notebook = new wxNotebook(this, wxID_ANY);
			m_tab_script = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
				wxBoxSizer *SSizer = new wxBoxSizer(wxVERTICAL);
                #ifdef wxUSE_STC
                    m_script_view = new wxStyledTextCtrl(m_tab_script, wxID_ANY);

                    //- Display of line numbers
                    m_script_view->SetMarginMask(0, wxSTC_STYLE_LINENUMBER);
                    m_script_view->SetMarginWidth(0, 32);

                    //- Activates folding
                    //Options
                    m_script_view->SetProperty(wxT("fold"), wxT("1"));
                    m_script_view->SetProperty(wxT("fold.compact"), wxT("0"));
                #ifdef __WXDEBUG__
                    #if wxMAJOR_VERSION >= 3
                        m_script_view->SetFoldFlags(16|wxSTC_FOLDFLAG_LINEBEFORE_EXPANDED|wxSTC_FOLDFLAG_LINEAFTER_EXPANDED);
                    #else
                        m_script_view->SetFoldFlags(16|wxSTC_FOLDFLAG_BOX);
                    #endif
                #else
                    m_script_view->SetFoldFlags(16);                    //To have a line at the bottom of a folder
                #endif
                    //Margin
                    m_script_view->SetMarginType(1, wxSTC_MARGIN_SYMBOL);
                    m_script_view->SetMarginMask(1, wxSTC_MASK_FOLDERS);
                    m_script_view->SetMarginWidth(1, 20);
                    m_script_view->SetMarginSensitive(1, true); //see also wxRouletteFrame::OnMarginClick
                    //Icons
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_PLUS);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_EMPTY);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_MINUS);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_EMPTY);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
                    m_script_view->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);

                    //- Visual styles
                    m_script_view->SetLexer(wxSTC_LEX_SMALLTALK);
                    //General options
                    m_script_view->SetBackgroundColour(*wxWHITE);
                    m_script_view->SetCaretForeground(*wxBLACK);
                    m_script_view->SetCaretLineVisible(true);
                    m_script_view->SetCaretLineBackground(wxColour(224, 224, 224));
                    //Default
                    for (int i=wxSTC_ST_DEFAULT ; i<=wxSTC_ST_SPEC_SEL ; i++)
                    {
                        m_script_view->StyleSetForeground(i, *wxBLACK);
                        m_script_view->StyleSetBackground(i, *wxWHITE);
                        m_script_view->StyleSetBold      (i, false);
                        m_script_view->StyleSetItalic    (i, false);
                        m_script_view->StyleSetSize      (i, 10);
                        m_script_view->StyleSetFaceName  (i, wxT("Courier New"));
                    }
                    //Strings
                    m_script_view->StyleSetForeground(wxSTC_ST_STRING, *wxRED);
                    //Numbers
                    m_script_view->StyleSetForeground(wxSTC_ST_NUMBER, *wxBLUE);
                    //Comments
                    m_script_view->StyleSetForeground(wxSTC_ST_COMMENT, wxColour(0, 128, 0));
                    m_script_view->StyleSetItalic    (wxSTC_ST_COMMENT, true);
                    //Operators
                    m_script_view->StyleSetForeground(wxSTC_ST_BINARY, wxColour(192, 0, 0));
                    //Keywords
                    m_script_view->StyleSetForeground(wxSTC_ST_GLOBAL, wxColour(128, 0, 128));
                    m_script_view->StyleSetBold      (wxSTC_ST_GLOBAL, true);
                #else
					m_script_view = new wxTextCtrl(m_tab_script, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_PROCESS_TAB|wxTE_MULTILINE|wxTE_NOHIDESEL);
					m_script_view->SetFont(wxFont(9, 76, 90, 90, false, wxEmptyString));
                #endif
					SSizer->Add(m_script_view, 1, wxALL|wxEXPAND, 5);
					m_tab_script->SetSizer(SSizer);
					m_tab_script->Layout();
				SSizer->Fit(m_tab_script);
			m_notebook->AddPage(m_tab_script, _("Script"), true);

            m_tab_memory = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
                wxBoxSizer *ASizer = new wxBoxSizer(wxVERTICAL);
                    m_listbox_variables = new wxListBox(m_tab_memory, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SORT);
                    m_listbox_variables->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
                    ASizer->Add(m_listbox_variables, 1, wxALL|wxEXPAND, 5);
                    m_tab_memory->SetSizer(ASizer);
                    m_tab_memory->Layout();
                ASizer->Fit(m_tab_memory);
            m_notebook->AddPage(m_tab_memory, _("Memory"), false);

            m_tab_console = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	            wxBoxSizer *CSizer = new wxBoxSizer(wxVERTICAL);
		            m_grid = new wxGrid(m_tab_console, wxID_ANY);
		            // Grid
		            m_grid->CreateGrid(1, 2);
		            m_grid->EnableEditing(false);
		            m_grid->EnableGridLines(true);
		            m_grid->EnableDragGridSize(false);
		            m_grid->SetMargins(0, 0);
                    m_grid->SetSelectionMode(wxGrid::wxGridSelectRows);
		            // Columns
		            m_grid->AutoSizeColumns();
		            m_grid->EnableDragColMove(false);
		            m_grid->EnableDragColSize(true);
		            m_grid->SetColLabelSize(25);
		            m_grid->SetColLabelValue(0, _("Type"));
		            m_grid->SetColLabelValue(1, _("Message"));
		            m_grid->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
                    m_grid->AutoSizeColumns();
		            // Rows
		            m_grid->SetRowSize(0, 0);
		            m_grid->AutoSizeRows();
		            m_grid->EnableDragRowSize(true);
		            m_grid->SetRowLabelSize(50);
		            m_grid->SetRowLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
		            // Cell Defaults
		            m_grid->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_TOP);
	            CSizer->Add(m_grid, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5);

                wxBoxSizer *CTSizer = new wxBoxSizer(wxHORIZONTAL);
	                m_toggle_all = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE_ALL, _("All"));
	                m_toggle_all->SetValue(true);
	                CTSizer->Add(m_toggle_all, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT|wxTOP, 5);
	                m_toggle_info = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE, _("Information"));
	                m_toggle_info->SetValue(true);
	                CTSizer->Add(m_toggle_info, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	                m_toggle_warning = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE, _("Warning"));
	                m_toggle_warning->SetValue(true);
	                CTSizer->Add(m_toggle_warning, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	                m_toggle_error = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE, _("Error"));
	                m_toggle_error->SetValue(true);
	                CTSizer->Add(m_toggle_error, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	                m_toggle_debug = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE, _("Debug"));
	                m_toggle_debug->SetValue(true);
	                CTSizer->Add(m_toggle_debug, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	                m_toggle_system = new wxToggleButton(m_tab_console, ID_BUTTON_TOGGLE, _("System"));
	                m_toggle_system->SetValue(true);
	                CTSizer->Add(m_toggle_system, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
                CSizer->Add(CTSizer, 0, wxEXPAND, 5);

	            m_tab_console->SetSizer(CSizer);
	            m_tab_console->Layout();
	            CSizer->Fit(m_tab_console);
            m_notebook->AddPage(m_tab_console, _("Console"), false);

			m_tab_money = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
				wxBoxSizer *MSizer = new wxBoxSizer(wxVERTICAL);
					m_plot = new wxEcPlot(m_tab_money, wxID_ANY);
					MSizer->Add(m_plot, 1, wxALL|wxEXPAND, 5);
					m_tab_money->SetSizer(MSizer);
					m_tab_money->Layout();
				MSizer->Fit(m_tab_money);
			m_notebook->AddPage(m_tab_money, _("Money"), false);
		VSizer->Add(m_notebook, 1, wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5);

        m_gauge = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxSize(-1,10), wxGA_HORIZONTAL);
        VSizer->Add(m_gauge, 0, wxALL|wxEXPAND, 5);

    //-- Toolbar
    m_toolbar = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);
#ifdef __WXMSW__
    m_toolbar->AddSeparator();
#endif
    m_toolbar->AddTool(ID_MENU_NEW, wxEmptyString, wxBitmap(icon_new_xpm), _("New"));
    m_toolbar->AddTool(ID_MENU_OPEN, wxEmptyString, wxBitmap(icon_open_xpm), _("Open"));
    m_toolbar->AddTool(ID_MENU_SAVE, wxEmptyString, wxBitmap(icon_save_xpm), _("Save"));
    m_toolbar->AddTool(ID_MENU_EXPORT, wxEmptyString, wxBitmap(icon_export_xpm), _("Export the log"));
    m_toolbar->AddSeparator();
    m_toolbar->AddTool(ID_MENU_PRETTY, wxEmptyString, wxBitmap(icon_pretty_xpm), _("Pretty script"));
    m_toolbar->AddTool(ID_MENU_RESET, wxEmptyString, wxBitmap(icon_reset_xpm), _("Reset"));
    m_toolbar->AddSeparator();
    m_toolbar->AddTool(ID_MENU_EXECUTE, wxEmptyString, wxBitmap(icon_execute_xpm), _("Execute"));
    m_toolbar->AddTool(ID_MENU_STOP, wxEmptyString, wxBitmap(icon_stop_xpm), _("Stop"));
    m_toolbar->Realize();

	//-- Menubar
	m_menubar = new wxMenuBar(0);
	m_menu_file = new wxMenu();
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_NEW, _("&New\tCtrl+N")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_OPEN, _("&Open\tCtrl+O")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_SAVE, _("&Save\tCtrl+S")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_SAVEAS, _("S&ave as...")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_EXPORT, _("E&xport the log")));
		m_menu_file->AppendSeparator();
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_PRETTY, _("&Pretty script")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_RESET, _("&Reset")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_EXECUTE, _("&Execute\tF5")));
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_STOP, _("S&top\tF6")));
		m_menu_file->AppendSeparator();
		m_menu_file->Append(new wxMenuItem(m_menu_file, ID_MENU_QUIT, _("&Quit")));
	m_menubar->Append(m_menu_file, _("&File"));

	m_menu_help = new wxMenu();
		m_menu_help->Append(new wxMenuItem(m_menu_help, ID_MENU_HELP, _("&Help\tF1")));
		m_menu_help->AppendSeparator();
		m_menu_help->Append(new wxMenuItem(m_menu_help, ID_MENU_ABOUT, _("&About")));
	m_menubar->Append(m_menu_help, _("&?"));
	this->SetMenuBar(m_menubar);

    //-- Popup menu
    m_popup_grid = new wxMenu();
    m_popup_grid->Append(new wxMenuItem(m_popup_grid, ID_POPUP_COPY, _("&Copy the selected lines")));

    //-- Status bar
    m_statusbar = this->CreateStatusBar(3, wxST_SIZEGRIP, wxID_ANY);
#ifdef __WXGTK__
    const int status_widths[3] = {300, 125, -1};
#else
    const int status_widths[3] = {200, 100, -1};
#endif
    m_statusbar->SetStatusWidths(3, &(status_widths[0]));

	//-- Final layout
#ifdef __WXMSW__
    SetIcon(wxIcon(wxT("MAINICON")));
#else
    SetIcon(wxIcon(icon_mainicon_xpm));
#endif
	this->SetSizer(VSizer);
	this->Layout();
    m_script_view->SetFocus();
	this->Center();

    //-- Extra features
    wxRouletteFileDrop* drop = new wxRouletteFileDrop();
    drop->SetParent(this);
    this->SetDropTarget(drop);
    m_plot->Connect(wxEVT_MOTION, wxMouseEventHandler(wxRouletteFrame::OnMouseEvent), NULL, this);

    //-- Initialization
    m_log_filter = 0xFF;
    m_current_file.Empty();
    m_current_file_mod_time = (time_t)-1;
    DoReset();
    UpdateMemory();
    DoEditorSetModified(false);

    //-- Open the default file
    if (pDefaultSample)
    {
        buffer = wxRouletteHelper::GetApplicationPath() + roulette_file_default;
        if (wxFileExists(buffer))
            OpenFile(buffer);
    }
}

wxRouletteFrame::~wxRouletteFrame()
{
    m_plot->Disconnect(wxEVT_MOTION, wxMouseEventHandler(wxRouletteFrame::OnMouseEvent), NULL, this);
}

void wxRouletteFrame::UpdateTitle(bool pRunningForced)
{
    wxString buffer;

    //-- Title
    if (m_current_file.IsEmpty())
        buffer = roulette_about_name;
    else
        buffer = wxString::Format(wxT("%s - %s")
                                        , roulette_about_name
                                        , wxFileName(m_current_file).GetFullName().uniCStr()
                                );

    //-- Status of modification
    if (DoEditorIsModified())
        buffer.Append(wxT(" *"));

    //-- Status of the thread
    if (m_roulette.IsRunning() || pRunningForced)
        buffer.Append(_(" (Running)"));

    //-- Final status
    this->SetTitle(buffer);
}

bool wxRouletteFrame::UpdateFolding()
{
#ifndef wxUSE_STC
    return false;
#else
    wxRouletteInstruction parser;
    int i, level, level_mask;

    //-- Determines the fold level of each line
    level = wxSTC_FOLDLEVELBASE;
    for (i=0 ; i<m_script_view->GetLineCount() ; i++)
    {
        //- Reads the line
        if (!parser.Parse(m_script_view->GetLine(i)))
            parser.Reset();

        //- Resets the level for new sections
        if ((parser.Instruction == roulette_sect_init) || (parser.Instruction == roulette_sect_sequence))
            level = wxSTC_FOLDLEVELBASE;

        //- Sets the fold mask
        if ((parser.Instruction == roulette_sect_init) ||
            (parser.Instruction == roulette_sect_sequence) ||
            (parser.Instruction == roulette_inst_if)
        )
            level_mask = wxSTC_FOLDLEVELHEADERFLAG;
        else
            level_mask = wxSTC_FOLDLEVELWHITEFLAG;

        //- Applies the fold level
        m_script_view->SetFoldLevel(i, level|level_mask);

        //- Changes the fold level for the next instructions
        if ((parser.Instruction == roulette_sect_init) || (parser.Instruction == roulette_sect_sequence))
            level++;
        else
            if (parser.Instruction == roulette_inst_if)
                level = (level < wxSTC_FOLDLEVELNUMBERMASK ? level+1 : level);
            else
                if (parser.Instruction == roulette_inst_endif)
                    level = (level > wxSTC_FOLDLEVELBASE ? level-1 : level);
    }
    return true;
#endif
}

void wxRouletteFrame::UpdateLog()
{
    unsigned long i, skipped;
    wxColour *col, col_white, col_yellow, col_red, col_purple, col_blue;
    wxRouletteMessageType::wxRouletteMessageType msg_type;
    wxArrayString *logas;
    wxString buffer, category;

    //-- Clears the table
    m_grid->ClearGrid();
    if (m_grid->GetNumberRows() > 0)
        m_grid->DeleteRows(0, m_grid->GetNumberRows());

    //-- Sets the text
    logas = m_roulette.GetLog();
    if ((logas != NULL) && (logas->Count() > 0) && (m_log_filter != 0))
    {
        //- Opens the grid
        m_grid->BeginBatch();
        m_grid->AppendRows(logas->Count()/*-1*/);

        //- Prepare the color set
        col_white = wxColour(255, 255, 255);
        col_yellow = wxColour(255, 255, 192);
        col_red = wxColour(255, 192, 192);
        col_purple = wxColour(255, 192, 255);
        col_blue = wxColour(192, 192, 255);

        //- Loops on each message
        skipped = 0;
        col = NULL;
        for (i=0 ; i<logas->Count() ; i++)
        {
            //Reads the item
            buffer = logas->Item(i);
            wxASSERT(!buffer.IsEmpty());

            //Gets the type of the message and its corresponding value
        #if wxMAJOR_VERSION >= 3
            switch (buffer.GetChar(0).GetValue())
        #else
            switch (buffer.GetChar(0)/*.GetValue()*/)       //Uncomment if you have issues during the compilation
        #endif
            {
                case wxT('W'):
                    msg_type = wxRouletteMessageType::WARNING_T;
                    col = &col_yellow;
                    category = _("Warning");
                    break;
                case wxT('E'):
                    msg_type = wxRouletteMessageType::ERROR_T;
                    col = &col_red;
                    category = _("Error");
                    break;
                case wxT('D'):
                    msg_type = wxRouletteMessageType::DEBUG_T;
                    col = &col_purple;
                    category = _("Debug");
                    break;
                case wxT('S'):
                    msg_type = wxRouletteMessageType::SYSTEM_T;
                    col = &col_blue;
                    category = _("System");
                    break;
                default:
                    wxASSERT(false);
                case wxT('I'):
                    msg_type = wxRouletteMessageType::INFO_T;
                    col = &col_white;
                    category = _("Information");
                    break;
            }

            //Filters the message type if not relevant for output
            if ((m_log_filter & msg_type) != msg_type)
            {
                skipped++;
                continue;
            }

            //Adds the new line
            m_grid->SetCellValue(i-skipped, 0, category);
            m_grid->SetCellBackgroundColour(i-skipped, 0, *col);
            if (msg_type == wxRouletteMessageType::SYSTEM_T)
                m_grid->SetCellBackgroundColour(i-skipped, 1, *col);
            m_grid->SetCellValue(i-skipped, 1, buffer.SubString(1, buffer.Len()));
        }

        //- Remove the skipped lines
        if (skipped > 0)
            m_grid->DeleteRows(m_grid->GetNumberRows()-skipped, skipped);

        //- Fits the columns
        m_grid->AutoSizeColumns();
        m_grid->EndBatch();
    }
}

void wxRouletteFrame::ShowMessageTypeCount()
{
    wxArrayString *log;
    size_t i;
    wxChar car;
    unsigned long info, warning, error, other;
    wxString buffer;

    //-- Initialization
    log = m_roulette.GetLog();
    info = 0;
    warning = 0;
    error = 0;
    other = 0;

    //-- Counts the messages by type
    for (i=0 ; i<log->Count() ; i++)
    {
        car = log->Item(i).GetChar(0);
        if (car == wxRouletteHelper::MessageTypeToChar(wxRouletteMessageType::INFO_T))
            info++;
        else
            if (car == wxRouletteHelper::MessageTypeToChar(wxRouletteMessageType::WARNING_T))
                warning++;
            else
                if (car == wxRouletteHelper::MessageTypeToChar(wxRouletteMessageType::ERROR_T))
                    error++;
                else
                    other++;
    }

    //-- Builds the message
    buffer.Clear();
    buffer.Alloc(1024);
    //Info
    if (info > 0)
    {
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" "));
        if (info > 1)
            buffer.Append(wxString::Format(_("%d infos."), info));
        else
            buffer.Append(wxString::Format(_("%d info."), info));
    }
    if (warning > 0)
    {
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" "));
        if (warning > 1)
            buffer.Append(wxString::Format(_("%d warnings."), warning));
        else
            buffer.Append(wxString::Format(_("%d warning."), warning));
    }
    if (error > 0)
    {
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" "));
        if (error > 1)
            buffer.Append(wxString::Format(_("%d errors."), error));
        else
            buffer.Append(wxString::Format(_("%d error."), error));
    }
    if (other > 0)
    {
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" "));
        if (other > 1)
            buffer.Append(wxString::Format(_("%d other messages."), other));
        else
            buffer.Append(wxString::Format(_("%d other message."), other));
    }
    this->SetStatusBarText(buffer, 2);
}

void wxRouletteFrame::UpdateMemory()
{
    m_roulette.GetMathematicalParser()->ListAllConstants((wxControlWithItems*)m_listbox_variables);
}

void wxRouletteFrame::FocusScript()
{
    m_notebook->ChangeSelection(0);
    m_script_view->SetFocus();
}

void wxRouletteFrame::ResetPlot()
{
    m_plot->RemoveAllCurves();
    m_plot->SetDefaultGrid();
    m_plot->Redraw();
}

void wxRouletteFrame::ClearStatusBar()
{
    m_statusbar->SetStatusText(wxEmptyString, 0);
    m_statusbar->SetStatusText(wxEmptyString, 1);
    m_statusbar->SetStatusText(wxEmptyString, 2);
}

bool wxRouletteFrame::SetStatusBarText(wxString pText, int pIndex)
{
    if (m_statusbar->GetStatusText(pIndex) != pText)
    {
        m_statusbar->SetStatusText(pText, pIndex);
        m_statusbar->Refresh();
        return true;
    }
    else
        return false;
}

void wxRouletteFrame::SetProgression(unsigned char pPercent)
{
    static unsigned char last_percent = 0;

    //-- Aligns the parameter
    if (pPercent > 100)
        pPercent = 100;
    if (pPercent == last_percent)
        return;
    m_gauge->SetValue(pPercent);
    last_percent = pPercent;

    //-- Refreshes the progress bar
    m_gauge->Refresh();
    if (pPercent % 5 == 0)
        wxSafeYield(this);      //Every 5%. Refer to wxApp::Yield() for the technical reasons.
}

bool wxRouletteFrame::OpenFile(wxString pFileName)
{
    wxFileDialog *m_filedlg;
    bool result = false;
    int code;
    wxString buffer;

    //-- Verifies if the current document has been modified
    if (DoEditorIsModified())
    {
        code = wxMessageDialog(this, _("The current document has been modified. Do you want to save it first ?"), _("Confirmation"), wxICON_QUESTION|wxYES|wxNO|wxCANCEL).ShowModal();
        if (code == wxID_CANCEL)
            return false;
        if (code == wxID_YES)
            if (!DoSave())
                return false;
    }

    //-- Loads a file from the parameter
    if (!pFileName.IsEmpty())
    {
        if (!wxFileExists(pFileName))
            result = false;
        else
            if (m_script_view->LoadFile(pFileName))
            {
                m_current_file = pFileName;
                result = true;
            }
    }

    //-- Loads a file from a dialog box
    else
    {
        m_filedlg = new wxFileDialog(this, _("Open a scripted roulette"), wxEmptyString, wxEmptyString, roulette_file_formats, wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        if (m_filedlg->ShowModal() == wxID_OK)
        {
            if (m_script_view->LoadFile(m_filedlg->GetPath()))
            {
                m_current_file = m_filedlg->GetPath();
                result = true;
            }
        }
        delete m_filedlg;
    }

    //-- Final
    if (result)
    {
        DoReset();
        DoEditorSetModified(false);
        m_current_file_mod_time = wxFileModificationTime(m_current_file);
        wxRouletteHelper::SetWorkingDirectory(wxFileName(m_current_file).GetPathWithSep());
        m_script_view->SetFocus();
    }
    UpdateTitle();
    return result;
}

void wxRouletteFrame::Execute()
{
    unsigned long count;

    //-- Checks for includes
    if (!m_current_file.IsEmpty())
    {
        if (wxRouletteHelper::GetFileExtension(m_current_file) == roulette_file_ext_inc)
        {
            wxLogError(_("The include files cannot be processed directly.\n\nPlease execute a script having the right file extension."));
            return;
        }
        if (wxRouletteHelper::GetFileExtension(m_current_file) != roulette_file_ext)
        {
            wxLogError(_("You can't execute a script not related to Scripted Roulette."));
            return;
        }
    }

    //-- Checks if saved (in case of infinite loops, the work is lost)
    if (DoEditorIsModified() && HasLoop())
    {
        switch (wxMessageDialog(NULL, _("Your scripted roulette uses loops.\n\nIn order to not lose your script, do you want to save it before it is executed ?"), _("Confirmation"), wxYES|wxNO|wxCANCEL|wxICON_QUESTION).ShowModal())
        {
            case wxID_YES:
                if (!DoSave())
                    return;
                break;
            case wxID_CANCEL:
                return;
        }
    }

    //-- Executes
    DoReset();
    UpdateTitle(true);
    m_roulette.StartFromInput(DoEditorGetText(), true);
    UpdateTitle();

    //-- Leaves the application if requested
    if (m_roulette.HasToLeave())
    {
        this->Close();
        return;
    }

    //-- Update the log
    UpdateLog();
    ShowMessageTypeCount();

    //-- Updates the curve displaying the money
    count = m_roulette.GetCreditCount();
    m_plot->AddCurve(   new wxEcCurve(  wxEmptyString,
                                        wxEmptyString,
                                        wxECT_CLOUD,
                                        *wxRED,
                                        1,
                                        false,
                                        false,
                                        0,
                                        count,
                                        count,
                                        m_roulette.GetCreditEvolution()
                        )
                    );
    count = m_roulette.GetCashCount();
    m_plot->AddCurve(   new wxEcCurve(  wxEmptyString,
                                        wxEmptyString,
                                        wxECT_CLOUD,
                                        *wxBLUE,
                                        1,
                                        false,
                                        false,
                                        0,
                                        count,
                                        count,
                                        m_roulette.GetCashEvolution()
                        )
                    );
    m_plot->SetAxis(wxEcAxis(   0,
                                (count > 0 ? count : 1),
                                1,
                                *wxRED,
                                true,
                                true
                            )
                    , true);
    DoFitAxis(m_plot->GetAxis(true));
    m_plot->Redraw();
    m_plot->GetAxis(false)->MaxValue = m_plot->GetYMax();
    DoFitAxis(m_plot->GetAxis(false));
    m_plot->Refresh();
}

bool wxRouletteFrame::HasLoop()
{
    wxRoulette script_container;
    wxArrayString *script;
    size_t i;
    wxString buffer;

    //-- Loads the script
    if (!script_container.LoadFromInput(DoEditorGetText()))
        return false;

    //-- Gets the loaded script
    script = script_container.GetScript();
    wxASSERT(script != NULL);

    //-- Looks for the instructions able to cause loops
    for (i=0 ; i<script->GetCount() ; i++)
    {
        buffer = wxRouletteHelper::GetInstructionName(script->Item(i));
        if ((buffer == roulette_inst_goto) || (buffer == roulette_inst_restart))
            return true;
    }
    return false;
}

void wxRouletteFrame::About()
{
    wxAboutDialogInfo info;
#ifdef __WXMSW__
    info.SetIcon(wxIcon(wxT("MAINICON")));
#else
    info.SetIcon(wxIcon(icon_mainicon_xpm));
#endif
    info.SetName(roulette_about_name);
    info.SetVersion(roulette_about_version);
    info.SetWebSite(roulette_about_website);
    info.SetCopyright(roulette_about_copyright);
    info.SetDescription(roulette_about_desc);
    info.AddDeveloper(wxString::Format(_("Compiled with %s"), wxRouletteHelper::GetCompiler().uniCStr()));
    info.AddDeveloper(wxString::Format(wxT("%s (wxWindows Library License v3.1) at http://www.wxwidgets.org"), wxVERSION_STRING));
    info.AddDeveloper(wxString::Format(wxT("%s %s (GPL v2) at %s"), wxECD_SOFTWARE, wxECD_VERSION, wxECD_URL));
#if wxUSE_STC
    #if wxMAJOR_VERSION >= 3
        info.AddDeveloper(wxT("Scintilla 3.2.1 (specific license, refer to the documentation) at http://www.scintilla.org"));   //Please update the version of Scintilla if wxWidgets 3.x is shipped with a newer version
    #else
        info.AddDeveloper(wxT("Scintilla 1.7 (specific license, refer to the documentation) at http://www.scintilla.org"));
    #endif
#endif
    info.AddDeveloper(wxT("Parts of 'mt19937ar.c' (specific license, refer to the documentation) at http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c"));
    info.AddDeveloper(wxT("Parts of 'tt800.c' (specific license, refer to the documentation) at http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/VERSIONS/C-LANG/tt800.c"));
    info.SetLicense(/* GPL v2 */ wxT("This program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful, but\nWITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\nSee the GNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program; if not, write to the Free Software\nFoundation, Inc., 51 Franklin Street, Fifth Floor, Boston,\nMA 02110-1301 USA."));
    wxAboutBox(info);
}


//------------------------------------------

void wxRouletteFrame::OnMenuClick(wxCommandEvent& event)
{
    //-- Checks
    if (m_roulette.IsRunning())
    {
        if (event.GetId() == ID_MENU_STOP)
        {
            if (m_roulette.RequestStop())
                wxBell();
        }
        else
        {
            //The roulette is not in a separate thread, so this popup should never occur unless wxYield() is called somewhere
            wxMessageDialog(this, _("The script is still being executed. Please wait until it ends."), _("Command not available"), wxOK|wxICON_ERROR).ShowModal();
            event.Skip(false);
            return;
        }
    }

    //-- Normal processing
    switch (event.GetId())
    {
        //- Menu File
        case ID_MENU_NEW:
            DoNew();
            break;
        case ID_MENU_OPEN:
            OpenFile();
            break;
        case ID_MENU_SAVE:
            DoSave();
            break;
        case ID_MENU_SAVEAS:
            DoSaveAs();
            break;
        case ID_MENU_EXPORT:
            DoExport();
            break;
        case ID_MENU_PRETTY:
            DoPrettyScript();
            break;
        case ID_MENU_RESET:
            DoReset();
            break;
        case ID_MENU_EXECUTE:
            Execute();
            break;
        case ID_MENU_QUIT:
            DoQuit();
            break;
        case ID_MENU_STOP:
            //Nothing (see above)
            break;

        //- Menu Help
        case ID_MENU_HELP:
            DoHelp();
            break;
        case ID_MENU_ABOUT:
            About();
            break;

        //-- Popup menus
        case ID_POPUP_COPY:
            if (!DoCopyGrid())
                wxLogWarning(_("Nothing has been copied. Please select the lines with the numbered column."));
            break;

        //- Other cases
        default:
            wxASSERT(false);
            event.Skip();
            break;
    }
    UpdateMemory();
}

void wxRouletteFrame::OnGridCellRightClick(wxGridEvent& event)
{
    m_grid->PopupMenu(m_popup_grid, event.GetPosition());
}

void wxRouletteFrame::OnToggle(wxCommandEvent& event)
{
    //-- No update if the script is running
    if (m_roulette.IsRunning())
        return;

    //-- Processes the event
    switch (event.GetId())
    {
        case ID_BUTTON_TOGGLE:
            DoToggleLogMessages();
            break;

        case ID_BUTTON_TOGGLE_ALL:
            m_toggle_info   ->SetValue(m_toggle_all->GetValue());   //No event is generated
            m_toggle_warning->SetValue(m_toggle_all->GetValue());
            m_toggle_error  ->SetValue(m_toggle_all->GetValue());
            m_toggle_debug  ->SetValue(m_toggle_all->GetValue());
            m_toggle_system ->SetValue(m_toggle_all->GetValue());
            DoToggleLogMessages();
            break;

        default:
            wxASSERT(false);
            event.Skip();
            break;
    }
}

void wxRouletteFrame::OnActivate(wxActivateEvent& event)
{
    static bool lock = false;
    time_t ftime;
    wxString message;

    //-- Checks if the current file has not been modified externally by another program
    if (event.GetActive() && !m_current_file.IsEmpty() && !m_roulette.IsRunning() && !lock)
    {
        lock = true;                                        //The event must be locally locked in order that it doesn't generate multiple popup windows
        if (!wxFileExists(m_current_file))                  //It will not display the wxLogError in case the file doesn't exist
            ftime = (time_t)-1;
        else
        {
            ftime = wxFileModificationTime(m_current_file);
            if (ftime != m_current_file_mod_time)
            {
                message = _("The script has been modified externally. Do you want to reload the script ?");
                if (DoEditorIsModified())
                {
                    message.Append(wxT("\n\n"));
                    message.Append(_("Please note that your current script is not saved yet. Any reload will discard your recent changes."));
                }
                if (wxMessageDialog(this, message, _("Confirmation"), wxICON_QUESTION|wxYES|wxNO).ShowModal() == wxID_YES)
                {
                    if (!OpenFile(m_current_file))
                        wxLogError(_("The current file cannot be reloaded."));
                }
                UpdateTitle();
            }
        }
        m_current_file_mod_time = ftime;                    //This will cancel the next warning whatever the file exists, is changed or not
        lock = false;
    }

    //-- Propagates
    event.Skip();
}

void wxRouletteFrame::OnClose(wxCloseEvent& event)
{
    int code;

    /** \todo In some circumstances, closing the window with wxMSW 3.0.0 leads to a crash.
              The cause is not yet identified since the issue doesn't occur in debug mode. */

    //-- Impossible to leave if the script is running
    if (m_roulette.IsRunning())
    {
        wxMessageDialog(this, _("The script is still being executed. Please wait until it ends."), _("Command not available"), wxOK|wxICON_ERROR).ShowModal();
        event.Veto(true);
        return;
    }

    //-- Verifies if the current document has been modified
    if (DoEditorIsModified())
    {
        code = wxMessageDialog(this, _("The current document has been modified. Do you want to save it before to leave ?"), _("Confirmation"), wxICON_WARNING|wxYES|wxNO|wxCANCEL).ShowModal();
        if (code == wxID_CANCEL)
            goto veto;
        if (code == wxID_YES)
            if (!DoSave())
            {
veto:
                event.Veto(true);
                return;
            }
    }

    //-- Quits
    this->Destroy();
    event.Veto(false);
}

#ifdef wxUSE_STC
    void wxRouletteFrame::OnStyledText(wxStyledTextEvent& event)
    {
        event.Skip();
        m_editor_status = true;
        UpdateTitle();
        UpdateFolding();
    }

    void wxRouletteFrame::OnMarginClick(wxStyledTextEvent& event)
    {
        //-- Verifies the value of the folding levels (now visual)
        /*  wxString buffer;
            for (int i=0 ; i<m_script_view->GetLineCount() ; i++)
                buffer.Append(wxString::Format(wxT("%d, "), m_script_view->GetFoldLevel(i)&wxSTC_FOLDLEVELNUMBERMASK));
            wxLogMessage(buffer);
        */

        //-- Processes the event
        if (event.GetMargin() == 1)
        {
            m_script_view->ToggleFold(m_script_view->LineFromPosition(event.GetPosition()));
            /** \todo The folding is strange sometimes */
        }
        else
            event.Skip();
    }
#else
    void wxRouletteFrame::OnText(wxCommandEvent& event)
    {
        event.Skip();
        m_editor_status = true;
        UpdateTitle();
    }
#endif

void wxRouletteFrame::OnMouseEvent(wxMouseEvent& event)
{
    double x, y;

    //-- Calculates the position of the mouse
    x = m_plot->XToValue(event.GetX());
    y = m_plot->YToValue(event.GetY());
    m_statusbar->SetStatusText(wxString::Format(_("Round %.0f for %.1f units of money"), x, y), 0);
    event.Skip();
}


//------------------------------------------

bool wxRouletteFrame::DoNew()
{
    int code;

    //-- Verifies if the current document has been modified
    if (DoEditorIsModified())
    {
        code = wxMessageDialog(this, _("The current document has been modified. Do you want to save it first ?"), _("Confirmation"), wxICON_QUESTION|wxYES|wxNO|wxCANCEL).ShowModal();
        if (code == wxID_CANCEL)
            return false;
        if (code == wxID_YES)
            if (!DoSave())
                return false;
    }

    //-- Resets the controls
    m_current_file.Empty();
    m_current_file_mod_time = (time_t)-1;
    m_notebook->ChangeSelection(0);
    DoEditorClear();
    DoEditorSetModified(false);
    DoReset();
    m_script_view->SetFocus();
    return true;
}

bool wxRouletteFrame::DoSave()
{
    bool result = false;
    wxString buffer;

    //-- Saves a file
    if (m_current_file.IsEmpty())
    {
        wxFileDialog *m_filedlg = new wxFileDialog(this, _("Save a scripted roulette"), wxEmptyString, wxEmptyString, roulette_file_formats, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
        if (m_filedlg->ShowModal() == wxID_OK)
        {
            if (m_script_view->SaveFile(m_filedlg->GetPath()))
            {
                m_current_file = m_filedlg->GetPath();
                wxRouletteHelper::SetWorkingDirectory(m_current_file);
                result = true;
            }
        }
        delete m_filedlg;
    }
    else
        result = m_script_view->SaveFile(m_current_file);

    //-- Flags the modification
    if (result)
    {
        DoEditorSetModified(false);
        m_current_file_mod_time = wxFileModificationTime(m_current_file);
    }
    UpdateTitle();
    return result;
}

bool wxRouletteFrame::DoSaveAs()
{
    wxString backup = m_current_file;
    m_current_file.Empty();
    if (!DoSave())
    {
        m_current_file = backup;
        UpdateTitle();
        return false;
    }
    else
        return true;
}

void wxRouletteFrame::DoExport()
{
    wxArrayString *log;
    bool result;

    //-- Checks
    log = m_roulette.GetLog();
    if ((log != NULL) && (log->GetCount() == 0))
    {
        wxLogError(_("There is nothing to export."));
        return;
    }

    //-- Saves a file
    wxFileDialog *m_filedlg = new wxFileDialog(this, _("Save the log"), wxEmptyString, wxEmptyString, _("CSV file (*.csv)|*.csv"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (m_filedlg->ShowModal() == wxID_OK)
        result = m_roulette.SaveLogHistory(m_filedlg->GetPath());
    else
        result = true;
    delete m_filedlg;

    //-- Final
    if (!result)
        wxLogError(_("The log has not been saved."));
}

void wxRouletteFrame::DoPrettyScript()
{
    wxArrayString current_script, new_script;
    wxString buffer, str, instruction, line, comment;
    wxRouletteInstruction parser;
    size_t i, j;
    unsigned long depth;
    bool inc_depth, last_line_blank;

    //-- Loads the script in an array
    buffer = DoEditorGetText();
    buffer.Replace(wxT("\r\n"), wxT("\n"), true);
    buffer.Replace(wxT("\r"), wxEmptyString, true);
    while (buffer.Replace(wxT("\n\n\n"), wxT("\n\n"), true) > 0);       //To clear some extra lines in advance
    if (!wxRouletteHelper::StringToScript(buffer, &current_script))
        return;
    new_script.Clear();

    //-- Processes the pretty printing
    depth = 0;
    last_line_blank = false;
    for (i=0 ; i<current_script.GetCount() ; i++)
    {
        inc_depth = false;

        //- Gets data individually
        buffer = current_script.Item(i);
        if (buffer.IsEmpty())
        {
            if (!last_line_blank)
                new_script.Add(wxEmptyString);
            last_line_blank = true;
            continue;
        }
        instruction = wxRouletteHelper::GetInstructionName(buffer);
        line = wxRouletteHelper::FormatInstruction(buffer);
        comment = wxRouletteHelper::GetComment(buffer);

        //- Section
        if (instruction.StartsWith(wxT(".")))
        {
            if (!last_line_blank)
                new_script.Add(wxEmptyString);
            depth = 0;
            inc_depth = true;
        }
        if ((instruction == roulette_inst_if) || (instruction == roulette_inst_assert))
        {
            if (instruction == roulette_inst_if)
                inc_depth = true;
            if (parser.Parse(line))     //To put the logical operators in upper case
            {
                parser.ChangeParameter(wxT("not"),  wxT("NOT"));
                parser.ChangeParameter(wxT("and"),  wxT("AND"));
                parser.ChangeParameter(wxT("or"),   wxT("OR"));
                parser.ChangeParameter(wxT("xor"),  wxT("XOR"));
                parser.ChangeParameter(wxT("nand"), wxT("NAND"));
                parser.ChangeParameter(wxT("nor"),  wxT("NOR"));
                line = wxString::Format(wxT("%s %s"), instruction.uniCStr(), parser.Command.uniCStr());
            }
        }
        if (instruction == roulette_inst_else)
        {
            if (depth > 0)
                depth--;
            inc_depth = true;
        }
        if (instruction == roulette_inst_endif)
        {
            if (depth > 0)
                depth--;
        }
        if (instruction == roulette_inst_plot)
        {
            if (parser.Parse(line))     //To put the arbitrary arguments in upper case
            {
                parser.ChangeParameter(wxT("on"),   wxT("ON"));
                parser.ChangeParameter(wxT("off"),  wxT("OFF"));
                line = wxString::Format(wxT("%s %s"), instruction.uniCStr(), parser.Command.uniCStr());
            }
        }
        if (instruction == roulette_inst_restart)
        {
            if (parser.Parse(line))     //To put the arbitrary arguments in upper case
            {
                parser.ChangeParameter(wxT("sequence"), wxT("SEQUENCE"));
                line = wxString::Format(wxT("%s %s"), instruction.uniCStr(), parser.Command.uniCStr());
            }
        }

        //- Format the new line
        buffer.Empty();

        //> Indent
        if (!instruction.IsEmpty() || !comment.IsEmpty())
            for (j=0 ; j<depth ; j++)
                buffer.Append(wxT("    "));

        //> Instruction
        if (!instruction.IsEmpty())
        {
            buffer.Append(instruction.Upper());

            //Parameters
            if ((instruction == roulette_inst_show) || (instruction == roulette_inst_save) || (instruction == roulette_inst_clear))
                buffer.Append(line.SubString(instruction.Len(), line.Len()).Upper());
            else
            {
                str = line.SubString(instruction.Len(), line.Len());
                if (!str.IsEmpty())
                {
                    if (!str.StartsWith(wxT(" ")))          //To manage: WRITE'Missing space'
                        buffer.Append(wxT(" "));
                    buffer.Append(str);
                }
            }
        }

        //> Comment
        if (!comment.IsEmpty())
        {
            if (!buffer.IsEmpty() && !buffer.EndsWith(wxT(" ")))
                buffer.Append(wxT(" "));
            buffer.Append(wxT("\""));
            buffer.Append(comment);
            if (wxRouletteHelper::CountCharInString(buffer, wxT('"')) % 2 != 0)
                buffer.Append(wxT("\""));               //An even number of quotes is needed for the highlighter
        }
        //> Line
        new_script.Add(buffer.Trim(true));

        //- Updates the flags
        if (inc_depth)
            depth++;
        last_line_blank = (buffer.Trim(true).Trim(false).IsEmpty());
    }

    //-- Builds back a string from the array
    buffer.Alloc(10240);    //10 kB
    buffer.Empty();
    for (i=0 ; i<new_script.GetCount() ; i++)
    {
        buffer.Append(new_script.Item(i));
    #ifdef __WXMSW__
        buffer.Append(wxT("\r\n"));
    #else
        buffer.Append(wxT("\n"));
    #endif
    }

    //-- Updates the view
    DoEditorSetText(buffer);
    UpdateTitle();
    buffer.Clear();
}

void wxRouletteFrame::DoReset()
{
    m_roulette.Reset();
#ifndef _CONSOLE
    m_roulette.SetParent(this);
#endif
    m_plot->Reset();
    m_plot->Redraw();
    UpdateLog();
    UpdateTitle();
    ClearStatusBar();
    m_gauge->SetValue(0);
}

bool wxRouletteFrame::DoFitAxis(wxEcAxis* pAxis)
{
    unsigned long base;
    double target;

    //-- Checks
    if (pAxis == NULL)
        return false;

    //-- Finds the best scale
    pAxis->Validate();
    target = pAxis->MaxValue;
    //For multiples of 5
    base = 5;
    while (10 * base < target)
        base += 5;

    //-- Applies the base
    pAxis->MaxValue = 10 * base;
    pAxis->MinValue = -(double)base;
    pAxis->StepValue = base;
    return true;
}

void wxRouletteFrame::DoQuit()
{
    this->Close();
}

bool wxRouletteFrame::DoOpenPDFFile(wxString pFullName)
{
    return false;

/* (Disabled)
    wxMimeTypesManager manager;
    wxFileType *file_type;
    wxString mime, command;

    //-- Checks
    wxASSERT(wxFileExists(pFullName));

    //-- Loads the command to open the PDF file
    file_type = manager.GetFileTypeFromExtension(wxT("pdf"));
    if (file_type == NULL)
        return false;

    //-- Builds and executes the command
    if (!file_type->GetMimeType(&mime))
        return false;
    if (file_type->GetOpenCommand(&command, wxFileType::MessageParameters(pFullName, mime)))
    {
        wxExecute(command);
        return true;            //The return code is expected to be all right
    }
    else
        return false;
*/
}

bool wxRouletteFrame::DoFindPdfHelp(wxString pFileName)
{
    return false;

/* (Disabled)
    wxString exe_path, file_path;

    //-- Gets the root directory of the application
    exe_path = wxRouletteHelper::GetApplicationPath();

    //-- Finds the help file
    if (wxFileExists(pFileName))
        return DoOpenPDFFile(pFileName);
    else
    {
        file_path = exe_path + pFileName;
        if (wxFileExists(file_path))
            return DoOpenPDFFile(file_path);
        else
        {
            file_path = exe_path + wxT("docs") + wxFileName::GetPathSeparator() + pFileName;
            if (wxFileExists(file_path))
                return DoOpenPDFFile(file_path);
            else
            {
                file_path = exe_path + wxT("..") + wxFileName::GetPathSeparator() + wxT("docs") + wxFileName::GetPathSeparator() + pFileName;
                if (wxFileExists(file_path))
                    return DoOpenPDFFile(file_path);
                else
                    return false;
            }
        }
    }
*/
}

void wxRouletteFrame::DoHelp()
{
    wxLaunchDefaultBrowser(wxString::Format(wxT("%s/docs/"), roulette_about_website));
}

void wxRouletteFrame::DoToggleLogMessages()
{
    //-- Sets the flag
    m_log_filter = 0xFF;
    if (!m_toggle_info->GetValue())
        m_log_filter &= ~wxRouletteMessageType::INFO_T;
    if (!m_toggle_warning->GetValue())
        m_log_filter &= ~wxRouletteMessageType::WARNING_T;
    if (!m_toggle_error->GetValue())
        m_log_filter &= ~wxRouletteMessageType::ERROR_T;
    if (!m_toggle_debug->GetValue())
        m_log_filter &= ~wxRouletteMessageType::DEBUG_T;
    if (!m_toggle_system->GetValue())
        m_log_filter &= ~wxRouletteMessageType::SYSTEM_T;
    m_toggle_all->SetValue(m_log_filter == 0xFF);

    //-- Refresh the log
    UpdateLog();
}

bool wxRouletteFrame::DoCopyGrid()
{
    wxArrayInt rows;
    wxString output, buffer;
    size_t i;
    int y;
    bool ret;

    //-- Gets the cells
    rows = m_grid->GetSelectedRows();
    if (rows.Count() == 0)
    {
        m_grid->SelectRow(m_grid->GetGridCursorRow());
        rows = m_grid->GetSelectedRows();                   //Retry
    }

    //-- Builds the buffer
    output.Empty();
    output.Alloc(128*rows.Count());
    for (i=0 ; i<rows.Count() ; i++)
    {
        y = rows.Item(i);
        buffer = wxString::Format(wxT("%s\t%s")
                                        , m_grid->GetCellValue(y, 0).uniCStr()
                                        , m_grid->GetCellValue(y, 1).uniCStr()
                                );
        if (!output.IsEmpty())
        {
        #ifdef __WXMSW__
            output.Append(wxT("\r\n"));
        #else
            output.Append(wxT("\n"));
        #endif
        }
        output.Append(buffer);
    }

    //-- Stores to the clipboard
    if (output.IsEmpty())
        return false;
    if (wxTheClipboard->Open())
    {
        ret = wxTheClipboard->SetData(new wxTextDataObject(output));
        wxTheClipboard->Close();
        return ret;
    }
    else
        return false;
}

wxString wxRouletteFrame::DoEditorGetText()
{
#ifdef wxUSE_STC
    return m_script_view->GetText();
#else
    return m_script_view->GetValue();
#endif
}

void wxRouletteFrame::DoEditorSetText(wxString pText)
{
#ifdef wxUSE_STC
    m_script_view->SetText(pText);
#else
    m_script_view->SetValue(pText);
#endif
    DoEditorSetModified(true);
}

void wxRouletteFrame::DoEditorClear()
{
#ifdef wxUSE_STC
    m_script_view->ClearAll();
#else
    m_script_view->Clear();
#endif
}

bool wxRouletteFrame::DoEditorIsModified()
{
#ifndef wxUSE_STC
    return m_script_view->IsModified();
#else
    #if wxMAJOR_VERSION >= 3
        return m_script_view->IsModified();
    #else
        return m_editor_status;
    #endif
#endif
}

void wxRouletteFrame::DoEditorSetModified(bool pStatus)
{
#ifndef wxUSE_STC
    m_script_view->SetModified(pStatus);
#else
    #if wxMAJOR_VERSION >= 3
        if (!pStatus)
            m_script_view->SetModified(pStatus);
        //else is not implemented by wxWidgets
    #else
        m_editor_status = pStatus;
    #endif
#endif
}

#endif //_CONSOLE
