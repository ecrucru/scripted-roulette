
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

#include "http_prng.h"


//------------------------------------------

wxRouletteRandomHTTP::wxRouletteRandomHTTP()
{
    Reset();
}

wxRouletteRandomHTTP::~wxRouletteRandomHTTP()
{
}

void wxRouletteRandomHTTP::Reset()
{
    m_http_last_response = 0;
    m_quota = 0;
    m_quota_active = false;
    m_options_set = false;
    ResetCache();
}

void wxRouletteRandomHTTP::ResetCache()
{
    m_data.FreeData();
    m_data_cursor = wxNOT_FOUND;
    m_data_stamp.Set(1, wxDateTime::Jan, 1970, 0, 0, 0, 0);     //The time zero of computer scientists
}

bool wxRouletteRandomHTTP::DownloadContentIPv4(unsigned short pPort, wxString pURL, wxString *pOutput)
{
    wxHTTP http;
    wxIPV4address ipv4;
    wxInputStream *input;
    wxString buffer, content;

    //-- Checks
    if (pOutput == NULL)
        return false;

    //-- Connects to the remove server
    if (!ipv4.Hostname(wxURI(pURL).GetServer()))
        return false;
    ipv4.Service(pPort);
    if (!http.Connect(ipv4, true))
        return false;

    //-- Gets the content
    content.Empty();
    buffer.Empty();
    buffer.Alloc(roulette_http_buffer_size);
    http.SetHeader(wxT("X-SR-Client-Version"), roulette_about_version);
    input = http.GetInputStream(pURL);
    if ((input != NULL) && input->IsOk())
    {
        wxStringOutputStream stream(&buffer);
        while (input->Read(stream).LastRead() >= roulette_http_buffer_size)
        {
            content.Append(buffer);
            buffer.Empty();
        }
        content.Append(buffer);
    }
    delete input;

    //-- Deals with the response code
    m_http_last_response = http.GetResponse();
    if (m_http_last_response != roulette_http_okcode)
        content.Empty();
    *pOutput = content;
    return true;
}

bool wxRouletteRandomHTTP::DownloadSecureContent(wxString pURL, wxString *pOutput)
{
    wxString tmp_filename, cmd_file, command;
    wxArrayString file_content;
    size_t i;
    bool result;

    //-- Builds the command line and the temporary files
    m_http_last_response = roulette_http_okcode;            //Not managed
    tmp_filename = wxFileName::CreateTempFileName(roulette_file_prefix, (wxFile*)NULL);
    if (tmp_filename.IsEmpty())
        return false;
    if ((pURL.Find(wxT("\"")) != wxNOT_FOUND) || pURL.EndsWith(wxT("\\")))
        return false;
    cmd_file = tmp_filename;
    cmd_file.Replace(wxT("\\"), wxT("/"));
    cmd_file.Replace(wxT("\""), wxT("\\\""));
    command = wxString::Format(wxT("wget --quiet --no-verbose --tries=3 --timeout=300 --no-cache --ignore-length --no-http-keep-alive --user-agent=\"\" --header=\"X-SR-Client-Version: %s\" --no-check-certificate -O \"%s\" \"%s\"")
                                        , roulette_about_version
                                        , cmd_file.uniCStr()
                                        , pURL.uniCStr()
                                );
    //The mask (with 2 parameters) for CURL is as follows but it works not so good after some tests with wxGTK :
    //  curl --silent --fail --retry 3 --connect-timeout 300 --ignore-content-length --no-keep-alive --user-agent "" --insecure --output "%s" "%s"

    //-- Deletes the temporary file to avoid any problem when it needs to be rewritten
    if (::wxFileExists(tmp_filename))
        wxRemoveFile(tmp_filename);

    //-- Downloads the remote document
#if wxMAJOR_VERSION >= 3
    if (wxExecute(command, wxEXEC_BLOCK|wxEXEC_HIDE_CONSOLE) != 0)
#else
    wxProcess dummy;            //Trick to hide the console when WGET is launched
    dummy.Redirect();           //Remark: Not tested with wxGTK 2.8.12
    if (wxExecute(command, wxEXEC_SYNC&~wxEXEC_NOHIDE, &dummy) != 0)
#endif
        return false;
    result = true;

    //-- Gets the content of the downloaded file
    wxRouletteHelper::LoadFileToArrayString(tmp_filename, &file_content);
    if (file_content.IsEmpty())
    {
        result = false;
        goto clear;
    }

    //-- Builds the content
    if (pOutput != NULL)
    {
        pOutput->Empty();
        pOutput->Alloc(10*file_content.Count());
        for (i=0 ; i<file_content.Count() ; i++)
        {
            if (!pOutput->IsEmpty())
                pOutput->Append(wxT("\n"));
            pOutput->Append(file_content.Item(i));
        }
    }

    //-- Deletes the temporary file and returns the final status
clear:
    if (::wxFileExists(tmp_filename))
        wxRemoveFile(tmp_filename);
    return result;
}

bool wxRouletteRandomHTTP::HasQuota()
{ 
    return ((m_quota_active && (m_quota > 0)) || !m_quota_active);
}

unsigned long wxRouletteRandomHTTP::GetRemainingQuota()
{
    return (m_quota_active ? m_quota : 0);
}

unsigned char wxRouletteRandomHTTP::CountBits(unsigned long pValue)
{
    unsigned char i, num;

    //-- Defines the number of bits used by the number
    num = 1;
    for (i=1 ; i<=32 ; i++)
    {
        if ((pValue & 1) == 1)
            num = i;
        pValue >>= 1;
    }
    return num;
}

void wxRouletteRandomHTTP::SetOptions(unsigned int pNum, long pMin, long pMax)
{
    //-- Clears the data if the options are changed
    if (m_options_set)
        if ((pNum != m_option_num) || (pMin != m_option_min) || (pMax != m_option_max))
            ResetCache();

    //-- Stores the settings
    m_option_num = pNum;
    m_option_min = pMin;
    m_option_max = pMax;

    //-- Indicates that the settings are loaded
    m_options_set = true;
}

bool wxRouletteRandomHTTP::PickNumber(unsigned long *pResult)
{
    //-- Checks
    if (pResult == NULL)
        return false;

    //-- Refreshes the cache
    if ((m_data.GetCount() == 0) || ((m_data_cursor != wxNOT_FOUND) && ((unsigned long)m_data_cursor >= m_data.GetCount()-1)))
    {
        if (!HasQuota())            //The quota is refreshed at the startup of the application
            return false;
        if (!GetNumbers())
            return false;
        wxASSERT(m_data_cursor == wxNOT_FOUND);
    }

    //-- Picks a number
    wxASSERT(m_data.GetCount() > 0);
    if (m_data_cursor == wxNOT_FOUND)
        m_data_cursor = 0;
    else
        m_data_cursor++;
    *pResult = *(m_data.GetItem(m_data_cursor));
    return true;
}


//------------------------------------------

#ifdef __WXDEBUG__

    bool wxRouletteRandomHTTP_Local::GetQuota()    
    {
        wxString content;

        //-- Downloads the quota
        if (!DownloadContentIPv4(roulette_http_port, wxT("http://localhost/random_server/quota/?format=plain"), &content))
            content.Empty();

        //-- Analyzes the return code
        if (GetLastHTTPResponse() != roulette_http_okcode)
            return false;
        if (content.IsEmpty() || content.StartsWith(wxT("-")))
        {
            m_quota_active = false;
            m_quota = 0;
        }
        else
            m_quota_active = content.ToULong(&m_quota);
        return true;
    }

    bool wxRouletteRandomHTTP_Local::GetNumbers()
    {
        static bool last_failed = false;
        wxDateTime now;
        wxString uri, content;
        wxStringTokenizer tokenizer;
        wxString token;
        unsigned long value;
        unsigned char nbits;
        bool status;

        //-- Checks
        if (!m_options_set || !HasQuota())
            return false;

        //-- Temporization after last failure
        now = wxDateTime::Now();
        if (last_failed)
            if (now.Subtract(m_data_stamp).GetMinutes() < roulette_http_delay)
                return false;

        //-- Downloads the numbers
        uri = wxString::Format(wxT("http://localhost/random_server/integers/?num=%d&min=%d&max=%d&col=1&base=10&format=plain&rnd=new")
                                    , m_option_num
                                    , m_option_min
                                    , m_option_max
                                );
        if (!DownloadContentIPv4(roulette_http_port, uri, &content))
            content.Empty();

        //-- Analyzes the return code
        ResetCache();
        m_data_stamp = now;
        if ((GetLastHTTPResponse() != roulette_http_okcode) || content.IsEmpty())
        {
            last_failed = true;
            return false;
        }
        else
            last_failed = false;

        //-- Stores the new values
        status = false;
        tokenizer.SetString(content, wxT("\n"), wxTOKEN_STRTOK);
        while (tokenizer.HasMoreTokens())
        {
            token = tokenizer.GetNextToken();
            if (token.ToULong(&value))
            {
                m_data.AddData(value);
                status = true;

                //- Reduces the quota
                if (m_quota_active)
                {
                    nbits = CountBits(value);
                    if (nbits > m_quota)
                        m_quota = 0;
                    else
                        m_quota -= nbits;
                }
            }
        }
        return status;      //At least 1 value must have been loaded
    }

#endif //__WXDEBUG__


//------------------------------------------

bool wxRouletteRandomHTTP_Random_Org::GetQuota()    
{
    wxString content;

    //-- Downloads the quota
    if (!DownloadSecureContent(wxT("https://random.org/quota/?format=plain"), &content))
        content.Empty();

    //-- Analyzes the return code
    if (GetLastHTTPResponse() != roulette_http_okcode)
        return false;
    if (content.IsEmpty() || content.StartsWith(wxT("-")))
    {
        m_quota_active = false;
        m_quota = 0;
    }
    else
        m_quota_active = content.ToULong(&m_quota);
    return true;
}

bool wxRouletteRandomHTTP_Random_Org::GetNumbers()
{
    static bool last_failed = false;
    wxDateTime now;
    wxString uri, content;
    wxStringTokenizer tokenizer;
    wxString token;
    unsigned long value, nbits;
    bool status;

    //-- Checks
    if (!m_options_set || !HasQuota())
        return false;

    //-- Temporization after last failure
    now = wxDateTime::Now();
    if (last_failed)
        if (now.Subtract(m_data_stamp).GetMinutes() < roulette_http_delay)
            return false;

    //-- Downloads the numbers
    uri = wxString::Format(wxT("https://random.org/integers/?num=%d&min=%d&max=%d&col=1&base=10&format=plain&rnd=new")
                                , m_option_num
                                , m_option_min
                                , m_option_max
                            );
    if (!DownloadSecureContent(uri, &content))
        content.Empty();

    //-- Analyzes the return code
    ResetCache();
    m_data_stamp = now;
    if ((GetLastHTTPResponse() != roulette_http_okcode) || content.IsEmpty())
    {
        last_failed = true;
        return false;
    }
    else
        last_failed = false;

    //-- Stores the new values
    status = false;
    tokenizer.SetString(content, wxT("\n"), wxTOKEN_STRTOK);
    while (tokenizer.HasMoreTokens())
    {
        token = tokenizer.GetNextToken();
        if (token.ToULong(&value))
        {
            m_data.AddData(value);
            status = true;

            //- Reduces the quota
            if (m_quota_active)
            {
                nbits = CountBits(value);
                if (nbits > m_quota)
                    m_quota = 0;
                else
                    m_quota -= nbits;
            }
        }
    }
    return status;      //At least 1 value must have been loaded
}
