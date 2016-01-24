
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

#ifndef http_prng_h
#define http_prng_h

/**
 * \file http_prng.h
 * \brief HTTP client to access some online services
 * \author ecrucru
 * \version 0.1
 * \date December 2015
 *
 * This file provides classes to access remote services which generate
 * random numbers.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/filename.h"
#include "wx/url.h"
#include "wx/sstream.h"
#include "wx/protocol/http.h"
#include "wx/utils.h"
#if wxMAJOR_VERSION < 3
    #include "wx/process.h"
#endif

#include "defs.h"
#include "wxEcMath/ec_defs.h"
#include "storage.h"


//------------------------------------------

/** \class wxRouletteRandomHTTP
 *  \brief Get random numbers from Internet.
 *
 * Abstract class to access remote services.
 */
class wxRouletteRandomHTTP
{
    private:
        int                                 m_http_last_response;

    protected:
        bool                                m_quota_active;
        unsigned long                       m_quota;
        wxRouletteStorage<unsigned long>    m_data;
        long                                m_data_cursor;
        wxDateTime                          m_data_stamp;
        unsigned int                        m_option_num;
        long                                m_option_min, m_option_max;
        bool                                m_options_set;

    public:
        //-- Object handling
        /** Constructor. */
        wxRouletteRandomHTTP();
        /** Destructor. */
        virtual ~wxRouletteRandomHTTP();

        //-- Abstract methods
        /** Retrieves the quota. */
        virtual bool GetQuota() = 0;
        /** Retrieves the generated numbers from the online service. */
        virtual bool GetNumbers() = 0;

        //-- Methods
        /** Resets the status of the class. */
        void Reset();
        /** Resets the numbers set in cache. */
        void ResetCache();
        /**
         * Downloads a file from the given unsecured location.
         * \param pPort Port to connect: http=80...
         * \param pURL URL to get.
         * \param pOutput Output buffer.
         * \return \a true if successful, else \a false.
         * \remark Don't provide a link with HTTPS as wxWidgets doesn't embbed an SSL library.
         */
        bool DownloadContentIPv4(unsigned short pPort, wxString pURL, wxString *pOutput);
        /**
         * Downloads a file from the given location with the help of an external downloadeder
         * named WGET.
         * \param pURL URL to get.
         * \param pOutput Output buffer.
         * \return \a true if successful, else \a false.
         * \remark It aims to get files over HTTPS, but it works with many protocols.
         */
        bool DownloadSecureContent(wxString pURL, wxString *pOutput);
        /**
         * Returns the last encountered HTTP status.
         * \remark roulette_http_okcode means OK.
         * \see https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
         */
        inline int GetLastHTTPResponse() { return m_http_last_response; }
        /** Tells if there is some remaining quota. */
        bool HasQuota();
        /**
         * Returns the remaining quota.
         */
        unsigned long GetRemainingQuota();
        /**
         * Counts the number of bits needed to describe the number.
         * \param pValue Numbers to analyze.
         * \return Number of bits.
         * \remark This function can be overridden by sub-classes.
         */
        virtual unsigned char CountBits(unsigned long pValue);
        /**
         * Sets the options used in method GetNumbers().
         * \param pNum Count of numbers to get.
         * \param pMin Minimal number allowed.
         * \param pMax Maximal number allowed.
         * \remark You must call this function to be allowed to pick a random number.
         */
        void SetOptions(unsigned int pNum, long pMin, long pMax);
        /**
         * Picks a random number from the cache.
         * \param pResult Pointer to the variable which stores the output.
         * \return \a true on success, else \a false.
         */
        bool PickNumber(unsigned long *pResult);
};


//------------------------------------------

#ifdef __WXDEBUG__
    /** \class wxRouletteRandomHTTP_Local
     *  \brief Get random numbers from LocalHost.
     *
     * This class manages the local emulation of wxRouletteRandomHTTP_RandomOrg.
     *
     * Refer to the detailed documentation to know how to install the local
     * server which mimics www.random.org.
     */
    class wxRouletteRandomHTTP_Local : public wxRouletteRandomHTTP
    {
        public:
            /** Retrieves the quota. */
            bool GetQuota();
            /** Retrieves the generated numbers from the online service. */
            bool GetNumbers();
    };
#endif //__WXDEBUG__


//------------------------------------------

/** \class wxRouletteRandomHTTP_Random_Org
 *  \brief Get random numbers from Random.org.
 *
 * This class retrieves random numbers from online services.
 */
class wxRouletteRandomHTTP_Random_Org : public wxRouletteRandomHTTP
{
    public:
        /** Retrieves the quota. */
        bool GetQuota();
        /** Retrieves the generated numbers from the online service. */
        bool GetNumbers();
        /**
         * Counts the number of bits needed to describe the number.
         * It is not trivial, so the result is a large average.
         * \see https://www.random.org/faq/#Q4.2
         */
        inline unsigned char CountBits(unsigned long pValue) { return 12; }
};

#endif
