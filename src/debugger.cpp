
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

#ifndef _CONSOLE

#include "debugger.h"


//------------------------------------------

    BEGIN_EVENT_TABLE(wxRouletteDebugger, wxDialog)
        EVT_BUTTON        (wxID_ANY, wxRouletteDebugger::OnButtonClick)
        EVT_LISTBOX_DCLICK(wxID_ANY, wxRouletteDebugger::OnListDoubleClick)
    END_EVENT_TABLE()


wxRouletteDebugger::wxRouletteDebugger(wxWindow *pParent)
                  : wxDialog(pParent, wxID_ANY, _("Debugger"), wxDefaultPosition, wxSize(450,425), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU)
{
    //-- Layout
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    wxBoxSizer *VSizer = new wxBoxSizer(wxVERTICAL);
        m_static_context = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER);
        m_static_context->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
        VSizer->Add(m_static_context, 0, wxALL|wxEXPAND, 5);

        wxBoxSizer *CSizer = new wxBoxSizer(wxHORIZONTAL);
            m_combo_eval = new wxComboBox(this, wxID_ANY, wxEmptyString);
            CSizer->Add(m_combo_eval, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	        m_static_eval = new wxStaticText(this, wxID_ANY, wxT("= ..."));
	        m_static_eval->Wrap(-1);
	        m_static_eval->SetMinSize(wxSize(100,-1));
            CSizer->Add(m_static_eval, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
            m_button_eval = new wxButton(this, ID_DEBUG_EVAL, _("&Evaluate"));
            CSizer->Add(m_button_eval, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
        VSizer->Add(CSizer, 0, wxEXPAND, 5);

        wxBoxSizer *LSizer = new wxBoxSizer(wxHORIZONTAL);
            m_list = new wxListBox(this, ID_LIST_ALL, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SORT); 
            LSizer->Add(m_list, 1, wxALL|wxEXPAND, 5);
            m_list_diff = new wxListBox(this, ID_LIST_DIFF, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB|wxLB_SORT); 
            LSizer->Add(m_list_diff, 1, wxALL|wxEXPAND, 5);
        VSizer->Add(LSizer, 1, wxEXPAND, 5);

        wxBoxSizer *BSizer = new wxBoxSizer(wxHORIZONTAL);
            m_button_continue = new wxButton(this, ID_DEBUG_CONTINUE, _("&Continue"));
            BSizer->Add(m_button_continue, 0, wxALL, 5);
            m_button_jump = new wxButton(this, ID_DEBUG_JUMP, _("&Jump"));
            BSizer->Add(m_button_jump, 0, wxALL, 5);
            m_button_export = new wxButton(this, ID_DEBUG_EXPORT, _("Ex&port"));
            BSizer->Add(m_button_export, 0, wxALL, 5);
            BSizer->Add(0, 0, 1, wxEXPAND, 5);
            m_button_validate = new wxButton(this, ID_DEBUG_VALIDATE, _("&Validate"));
            BSizer->Add(m_button_validate, 0, wxALL, 5);
            m_button_stop = new wxButton(this, ID_DEBUG_STOP, _("&Stop"));
            BSizer->Add(m_button_stop, 0, wxALL, 5);
        VSizer->Add(BSizer, 0, wxEXPAND, 5);

    //-- Initializes
    m_data.Clear();
    m_engine_ref = NULL;

    //-- Shows
    this->SetSizer(VSizer);
    this->Layout();
    this->Center();
}

wxRouletteDebugger::~wxRouletteDebugger()
{
}


//------------------------------------------

void wxRouletteDebugger::DoEvalExpression()
{
    bool b;
    int position;
    wxString buffer;

    //-- Evaluates the expression
    if (m_engine_ref != NULL)
    {
        buffer = m_combo_eval->GetValue();
        buffer.Replace(wxT(" "), wxEmptyString);    //To reduce the duplicate entries in the MRU list
        b = false;
        if (m_engine_ref->SetFormula(buffer))
        {
            m_engine_ref->Compute();
            if (m_engine_ref->GetLastError() == wxECE_NOERROR)
            {
                m_static_eval->SetLabel(wxString::Format(wxT("= %f"), m_engine_ref->GetLastResult()));
                b = true;
            }
        }
        if (!b)
            m_static_eval->SetLabel(wxT("= ..."));
        else
        {
            //- Updates the drop-down list to put the last used formula in the first place
            position = m_combo_eval->FindString(buffer);
            if (position != wxNOT_FOUND)
                m_combo_eval->Delete(position);
            m_combo_eval->Insert(buffer, 0);
        }
        m_combo_eval->SetFocus();
    }
}

bool wxRouletteDebugger::DoExportMemory()
{
    wxArrayString buffer;
    wxFileDialog *dialog;
    wxString path;

    //-- Gets the data from the listbox
    buffer = m_list->GetStrings();
    if (buffer.Count() == 0)
        return false;

    //-- Prompts for the file name
    dialog = new wxFileDialog(this, _("Save the variables"), wxEmptyString, wxEmptyString, _("Plain text (*.txt)|*.txt"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (dialog->ShowModal() == wxID_OK)
    {
        path = dialog->GetPath();
        wxRouletteHelper::SaveArrayStringToFile(path, &buffer);
    }
    delete dialog;
    return true;
}


//------------------------------------------

void wxRouletteDebugger::SetState(wxRouletteEngine *pEngine, wxArrayString *pScript, unsigned long pPosition)
{
    const int around_line = 4;
    wxArrayString newdata;
    wxString buffer;
    unsigned long i;

    //-- Reference of the engine
    m_engine_ref = pEngine;

    //-- View of the script
    if (pScript == NULL)
        m_static_context->Hide();
    else
    {
        buffer.Empty();
        for (  i = (pPosition>=around_line ? pPosition-around_line : 0) ;
              (i < pPosition+around_line+1) && (i<pScript->Count()) ;
               i++
          )
        {
            if (!buffer.IsEmpty())
                buffer.Append(wxT("\n"));
            if (i == pPosition)
                buffer.Append(wxString::Format(wxT(">>> %s"), pScript->Item(i).uniCStr()));
            else
                buffer.Append(wxString::Format(wxT("    %s"), pScript->Item(i).uniCStr()));
        }
        m_static_context->SetLabel(buffer);
    }

    //-- Variables
    if (pEngine != NULL)
    {
        //- Clears the target components
        m_list->Clear();
        m_list_diff->Clear();

        //- Compares the data
        pEngine->ListAllConstants(&newdata);
        for (i=0 ; i<newdata.Count() ; i++)
            if (m_data.Index(newdata.Item(i)) == wxNOT_FOUND)
                m_list_diff->Append(wxT("+ ") + newdata.Item(i));
        for (i=0 ; i<m_data.Count() ; i++)
            if (newdata.Index(m_data.Item(i)) == wxNOT_FOUND)
                m_list_diff->Append(wxT("- ") + m_data.Item(i));
        m_list->Append(newdata);

        //- Backup
        m_data.Clear();
        for (i=0 ; i<newdata.Count() ; i++)
            m_data.Add(newdata.Item(i));
    }

    //-- Calculates the current expression
    if (!m_combo_eval->GetValue().IsEmpty())
        DoEvalExpression();
}

void wxRouletteDebugger::Reset()
{
    m_static_context->SetLabel(wxEmptyString);
    m_list->Clear();
    m_list_diff->Clear();
}

void wxRouletteDebugger::OnButtonClick(wxCommandEvent& event)
{
    switch (event.GetId())
    {
        //-- Evaluates an expression
        case ID_DEBUG_EVAL:
            DoEvalExpression();
            break;

        //-- Exports the memory
        case ID_DEBUG_EXPORT:
            if (!DoExportMemory())
                wxLogError(_("Nothing has been exported."));
            break;

        //-- Modal results
        default:
            EndModal(event.GetId());
            event.Skip();
            break;
    }
}

void wxRouletteDebugger::OnListDoubleClick(wxCommandEvent& event)
{
    wxString buffer, name;
    int position;
    bool b;

    //-- Processes the event
    switch (event.GetId())
    {
        case ID_LIST_ALL:
            //- Checks
            if (m_engine_ref == NULL)
                return;

            //- Reads the item
            position = event.GetSelection();
            if (position != wxNOT_FOUND)
            {
                buffer = m_list->GetString(position);

                //Reads the name of the variable
                position = buffer.Find(wxT(" "));
                if (position == wxNOT_FOUND)
                {
                    wxASSERT(false);
                }
                else
                {
                    name = buffer.Mid(0, position);
                    wxASSERT(buffer.Len() > 0);

                    //Checks
                    buffer = wxRouletteHelper::GetVariableDomain(name);
                    if (buffer == roulette_dom_sys)
                        return;     //You can't modify system variables

                    //Prompts the user for a value
                    buffer = wxGetTextFromUser(name, _("New value"), wxEmptyString, this);
                    if (!buffer.IsEmpty())
                    {
                        b = false;

                        //Calculates the value
                        if (m_engine_ref->SetFormula(buffer))
                        {
                            m_engine_ref->Compute();
                            if (m_engine_ref->GetLastError() == wxECE_NOERROR)
                            {
                                m_engine_ref->SetConstant(name, m_engine_ref->GetLastResult());
                                b = true;

                                //Updates the user interface
                                m_list->Clear();
                                m_engine_ref->ListAllConstants((wxControlWithItems*)m_list);
                                m_list->SetSelection(event.GetSelection());         //The number of items has not changed
                            }
                        }

                        //Warning if no update occurred
                        if (!b)
                            wxLogWarning(wxString::Format(_("No update occurred for the variable '%s'."), name.uniCStr()));
                    }
                }
            }
            break;

        default:
            event.Skip();
            break;
    }
}

#endif
