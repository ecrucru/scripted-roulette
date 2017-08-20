
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

#include "table.h"


//------------------------------------------

void wxRouletteTable::DoSetType(wxRouletteType::wxRouletteType pType)
{
    m_type = pType;
    m_nbcell = 36 + (int) m_type;
    m_ball_position = m_rand_wheel.GetRand() % (m_nbcell+1);
    m_ball_initial_position = m_ball_position;
    m_ball_direction = true;
}

bool wxRouletteTable::DoSetLayout(int* pLayout)
{
    int i, offset;

    //-- Checks
    if (pLayout == NULL)
        return SetFrenchLayout(false);

    //-- Inits to zero by default
    for (i=0 ; i<roulette_max_size ; i++)
        m_layout[i] = 0;

    //-- Assigns data
    offset = 0;
    for (i=0 ; i<36 ; i++)
    {
        //- Fills the zeros
        if ((i == 0) && (m_type != wxRouletteType::BALANCED))
        {
            m_layout[i+offset] = roulette_zero;
            offset++;
        }
        if ((i == 18) && (m_type == wxRouletteType::AMERICAN))
        {
            m_layout[i+offset] = roulette_zero_double;
            offset++;
        }
        if ((i == 12) && (m_type == wxRouletteType::MEXICAN))
        {
            m_layout[i+offset] = roulette_zero_double;
            offset++;
        }
        if ((i == 24) && (m_type == wxRouletteType::MEXICAN))
        {
            m_layout[i+offset] = roulette_zero_triple;
            offset++;
        }

        //- Fills the normal values.
        m_layout[i+offset] = pLayout[i];          /** \remark There is no check against duplicate numbers */
    }

    //-- Finalization
    DoUpdateHTTPSettings();
    return true;
}

bool wxRouletteTable::DoUpdateHTTPSettings()
{
    //-- Checks
    if (m_rand_http == NULL)
        return false;

    //-- Sets the options
    m_rand_http->SetOptions(roulette_http_lot_size,
                            wxRouletteHelper::GetMinNumberFromType(m_type),
                            wxRouletteHelper::GetMaxNumberFromType(m_type)
                        );
    return true;
}

void wxRouletteTable::DoGetNumbers(unsigned long *pBall, unsigned long *pWheel, unsigned long *pRandom)
{
    //-- Gets randoms numbers depending on the available algorithm
    switch (m_rng)
    {
        case wxRouletteAlgorithm::RAND:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetRand();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetRand();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetRand();
            break;

        case wxRouletteAlgorithm::MT:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetMersenneTwister();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetMersenneTwister();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetMersenneTwister();
            break;

        case wxRouletteAlgorithm::TT800:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetTT800();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetTT800();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetTT800();
            break;

        case wxRouletteAlgorithm::CLOCK:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetComputerClock();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetComputerClock();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetComputerClock();
            break;

        case wxRouletteAlgorithm::STDMIN:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetStandardMinimal();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetStandardMinimal();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetStandardMinimal();
            break;

        case wxRouletteAlgorithm::RS:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetRS();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetRS();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetRS();
            break;

        case wxRouletteAlgorithm::RANDU:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetRandu();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetRandu();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetRandu();
            break;

        case wxRouletteAlgorithm::BITS:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetBits();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetBits();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetBits();
            break;

        case wxRouletteAlgorithm::XOR:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetXorShift();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetXorShift();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetXorShift();
            break;

        case wxRouletteAlgorithm::LOADED:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetLoaded();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetLoaded();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetLoaded();
            break;

        case wxRouletteAlgorithm::LNGEN:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetLnGen();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetLnGen();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetLnGen();
            break;

        case wxRouletteAlgorithm::EQUI:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetEqui();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetEqui();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetEqui();
            break;

        //HTTP services are not managed by this method.

        case wxRouletteAlgorithm::CUSTOM:
            if (pBall   != NULL) *pBall   = m_rand_ball.GetCustom();
            if (pWheel  != NULL) *pWheel  = m_rand_wheel.GetCustom();
            if (pRandom != NULL) *pRandom = m_rand_generator.GetCustom();
            break;

        default:
            wxASSERT(false);        //HTTP services are not expected to fail here
            break;
    }
}


//------------------------------------------

wxRouletteTable::wxRouletteTable()
{
    m_parent_class = NULL;
    m_rand_http = NULL;
    m_ball_direction = true;
    SetFrenchLayout(false);
    SetAlgorithm(wxRouletteAlgorithm::TT800);       //Default algorithm
    ClearBets();
    m_bet = 5;
}

wxRouletteTable::~wxRouletteTable()
{
    wxDELETE(m_rand_http);
}

bool wxRouletteTable::SetFrenchLayout(bool pBalanced)
{
    const int default_roulette[36] = roulette_layout_fr;
    DoSetType(pBalanced ? wxRouletteType::BALANCED : wxRouletteType::FRENCH);
    m_black_first = false;
    return DoSetLayout((int*)&(default_roulette[0]));
}

bool wxRouletteTable::SetAmericanLayout(bool pBalanced)
{
    const int default_roulette[36] = roulette_layout_us;
    DoSetType(pBalanced ? wxRouletteType::BALANCED : wxRouletteType::AMERICAN);
    m_black_first = true;
    return DoSetLayout((int*)&(default_roulette[0]));
}

bool wxRouletteTable::SetMexicanLayout(bool pBalanced)
{
    const int default_roulette[36] = roulette_layout_us;    //Same layout than American
    DoSetType(pBalanced ? wxRouletteType::BALANCED : wxRouletteType::MEXICAN);
    m_black_first = true;
    return DoSetLayout((int*)&(default_roulette[0]));
}

void wxRouletteTable::ShowLayout()
{
    wxString buffer;
    int i;

    //-- Buffer
    buffer.Alloc(500);
    buffer.Empty();
    i = 0;
    do {
        //- Separator
        if (!buffer.IsEmpty())
            buffer.Append(wxT(", "));

        //- Format
        buffer.Append(wxRouletteHelper::FormatNumber(m_layout[i]));

        //- Next
        i++;
    } while (i<roulette_max_size && i<m_nbcell);

    //-- Output
    if (m_parent_class != NULL)
        m_parent_class->LogMessage(buffer.uniCStr());
}

bool wxRouletteTable::SetAlgorithm(wxRouletteAlgorithm::wxRouletteAlgorithm pAlgo)
{
    if (pAlgo == wxRouletteAlgorithm::NONE)
        return false;
    else
    {
        if (m_rng != pAlgo)     //The instance is reinstanciated only if required (to save some quota)
        {
            m_rng = pAlgo;
            if (wxRouletteHelper::IsOnlineGenerator(m_rng))
            {
                wxDELETE(m_rand_http);
                switch (m_rng)
                {
                #ifdef __WXDEBUG__
                    case wxRouletteAlgorithm::HTTP_LOCAL:
                        m_rand_http = new wxRouletteRandomHTTP_Local();
                        break;
                #endif

                    case wxRouletteAlgorithm::HTTP_RANDOM_ORG:
                        m_rand_http = new wxRouletteRandomHTTP_Random_Org();
                        if (m_parent_class != NULL)
                            m_parent_class->LogWarning(_("To use Random.org, your script needs to be in accordance with the terms of service available at https://www.random.org/terms/"));
                        break;

                    default:
                        wxASSERT(false);
                        break;
                }
                m_rand_http->GetQuota();
                DoUpdateHTTPSettings();
            }
        }
        return true;
    }
}

void wxRouletteTable::ShowAlgorithm()
{
    wxString rng;

    //-- Finds the name of the algorithm
    switch (m_rng)
    {
        case wxRouletteAlgorithm::RAND:
            rng = _("C++ Rand()");
            break;

        case wxRouletteAlgorithm::MT:
            rng = _("Mersenne-Twister");
            break;

        case wxRouletteAlgorithm::TT800:
            rng = _("TT800");
            break;

        case wxRouletteAlgorithm::CLOCK:
            rng = _("Computer clock");
            break;

        case wxRouletteAlgorithm::STDMIN:
            rng = _("Standard Minimal (linear congruential generator)");
            break;

        case wxRouletteAlgorithm::RS:
            rng = _("Robert Sedgewick's linear congruential generator");
            break;

        case wxRouletteAlgorithm::RANDU:
            rng = _("RANDU (linear congruential generator)");
            break;

        case wxRouletteAlgorithm::BITS:
            rng = _("Bit generator");
            break;

        case wxRouletteAlgorithm::XOR:
            rng = _("XorShift");
            break;

        case wxRouletteAlgorithm::LOADED:
            rng = _("Algorithm with deviation, based on a modified version of XorShift128+");
            break;

        case wxRouletteAlgorithm::LNGEN:
            rng = _("Natural logarithm");
            break;

        case wxRouletteAlgorithm::EQUI:
            rng = _("Home-made equi-distribution (French roulette only)");
            break;

    #ifdef __WXDEBUG__
        case wxRouletteAlgorithm::HTTP_LOCAL:
            rng = _("Local web-service emulating www.random.org (for debugging)");
            break;
    #endif

        case wxRouletteAlgorithm::HTTP_RANDOM_ORG:
            rng = _("www.random.org");
            break;

        case wxRouletteAlgorithm::CUSTOM:
            rng = _("Custom algorithm");
            break;

        default:
            wxASSERT(false);
            return;
    }

    //-- Output message
    if (m_parent_class != NULL)
        m_parent_class->LogMessage(wxString::Format(_("The random number generator is '%s'."), rng.uniCStr()));
}

void wxRouletteTable::ShowDistribution()
{
    unsigned long landed, total[roulette_max_size];
    int i, imin, imax, n;
    double average, deviation;
    bool status;

    //-- Checks
    if (wxRouletteHelper::IsOnlineGenerator(m_rng))
    {
        if (m_parent_class != NULL)
            m_parent_class->LogError(_("You can't create statistics about the online services. There is a risk for an overload and an exhaustion of your quota."));
        return;
    }

    //-- Resets
    for (i=0 ; i<roulette_max_size ; i++)
        total[i] = 0;

    //-- Spins
#ifdef __WXMSW__
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);         //It should not take a lot of time
#endif
    status = true;
    for (i=0 ; i<500000*m_nbcell ; i++)
    {
        if (!Spin(&landed))
        {
            status = false;
            break;
        }
        else
        {
            wxASSERT(landed < roulette_max_size);
            total[landed]++;
        }
    }
#ifdef __WXMSW__
    SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
#endif
    if (!status)
    {
        if (m_parent_class != NULL)
            m_parent_class->LogError(_("Unable to show the distribution for the current algorithm."));
        return;
    }

    //-- Deviation
    n = 0;
    average = 0;
    imin = wxRouletteHelper::GetMinNumberFromType(m_type);
    imax = wxRouletteHelper::GetMaxNumberFromType(m_type);
    wxASSERT(imax-imin+1 == m_nbcell);
    for (i=imin ; i<=imax ; i++)
    {
        n++;
        average += total[i];
    }
    average /= n;
    deviation = 0;
    for (i=imin ; i<=imax ; i++)
        deviation += ((double)total[i]-average)*((double)total[i]-average) / (double)n;
    deviation = (deviation <= 0 ? -1 : sqrt(deviation));

    //-- Output
    if (m_parent_class != NULL)
    {
        m_parent_class->LogMessage(_("Distribution of the current algorithm :"));
        for (i=imin ; i<=imax ; i++)
        {
            if (i < 10)
                m_parent_class->LogMessage(wxString::Format(wxT("   %s = %d"), wxRouletteHelper::FormatNumber(i).uniCStr(), total[i]));
            else
                m_parent_class->LogMessage(wxString::Format(wxT("  %s = %d"), wxRouletteHelper::FormatNumber(i).uniCStr(), total[i]));
        }
        m_parent_class->LogMessage(wxString::Format(_("  Deviation = %f"), deviation));
    }
}

bool wxRouletteTable::SaveDistribution(wxString pFileName, int pSize, bool pHotMap, unsigned long pNumPoints)
{
#if roulette_compile_extended != 1
    if (m_parent_class != NULL)
        m_parent_class->LogError(wxString::Format(_("The method '%s' is not enabled."), wxT("wxRouletteTable::SaveDistribution")));
    return false;
#else

    unsigned long i, spin, spin2, addr;
    int x, y;
    float total;
    bool split;
    unsigned char *data;
    wxMemoryDC dc;

    //-- Checks
    if (pSize < 64)
        return false;
    if (wxRouletteHelper::IsOnlineGenerator(m_rng))
    {
        if (m_parent_class != NULL)
            m_parent_class->LogError(_("You can't create statistics about the online services. There is a risk for an overload and an exhaustion of your quota."));
        return false;
    }

    //-- Detection of the average value of the algorithm
    total = 0;
    for (i=0 ; i<25 ; i++)
    {
        DoGetNumbers(&spin, NULL, NULL);
        total += spin;
    }
    split = ((unsigned long)(total/25) < (unsigned long)(pSize*pSize));

    //-- Classic map with few spins
    if (!pHotMap)
    {
        wxBitmap bmp(pSize, pSize);

        //- Black square
        dc.SelectObject(bmp);
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.DrawRectangle(0, 0, pSize, pSize);

        //- Draws points
        dc.SetPen(*wxWHITE_PEN);
        for (i=0 ; i<pNumPoints ; i++)
        {
            if (split)
            {
                DoGetNumbers(&spin, &spin2, NULL);
                x = spin  % pSize;
                y = spin2 % pSize;
            }
            else
            {
                DoGetNumbers(&spin, NULL, NULL);
                spin = spin % (pSize * pSize);
                y = spin / pSize;
                x = spin - y * pSize;
            }
            dc.DrawPoint(x, y);
        }
        return bmp.ConvertToImage().SaveFile(pFileName);
    }

    //-- Hot map with a lot of spins
    else
    {
        pNumPoints = 128 * pSize * pSize;       //We want a certain average level for each pixel of the map

        //- In direct update
        wxImage pict(pSize, pSize, true);
        data = pict.GetData();
        for (i=0 ; i<pNumPoints ; i++)
        {
            //Calculates the coordinates
            if (split)
            {
                DoGetNumbers(&spin, &spin2, NULL);
                x = spin  % pSize;
                y = spin2 % pSize;
            }
            else
            {
                DoGetNumbers(&spin, NULL, NULL);
                spin = spin % (pSize * pSize);
                y = spin / pSize;
                x = spin - y * pSize;
            }
            addr = 3 * (y * pSize + x);

            //Increase the level of the pixel (gray scale)
            if (data[addr] < 255)
            {
                data[addr  ]++;
                data[addr+1]++;
                data[addr+2]++;
            }
        }
        return pict.SaveFile(pFileName);
    }
#endif
}

void wxRouletteTable::ShowSeeds()
{
    if (m_parent_class != NULL)
        m_parent_class->LogMessage(wxString::Format(_("The seeds are %d and %d."), m_rand_ball.GetInitialSeed(), m_rand_wheel.GetInitialSeed()));
}

bool wxRouletteTable::Spin(unsigned long *pResult)
{
    unsigned long ball, wheel;

    //-- Checks
    if (pResult == NULL)
        return false;

    //-- Gets directly an online spin
    if (wxRouletteHelper::IsOnlineGenerator(m_rng))
    {
        wxASSERT(m_rand_http != NULL);
        if (m_rand_http->PickNumber(&wheel))
        {
            *pResult = wheel;
            return true;
        }
        else
        {
            *pResult = roulette_http_default_number;    //This number is the default value in case of various errors have occurred in the HTTP interface
            return false;
        }
    }

    //-- Else, gets a spin from a local launch
    DoGetNumbers(&ball, &wheel, NULL);
    ball  %= m_nbcell;
    wheel %= m_nbcell;
    m_ball_direction = !m_ball_direction;
    m_ball_position = ( m_ball_position
                        + m_nbcell                                  //To make a positive modulo
                        - (m_ball_direction ? 1 : -1) * wheel
                        + (m_ball_direction ? 1 : -1) * ball
                      ) % m_nbcell;
    *pResult = m_layout[m_ball_position];
    return true;
}

void wxRouletteTable::ClearBets()
{
    int i, j;

    //-- Resets
    for (i=0 ; i<roulette_max_size ; i++)
        mt_single[i] = 0;
    for (i=0 ; i<2 ; i++)
        for (j=0 ; j<12 ; j++)
            mt_double_h[i][j] = 0;
    for (i=0 ; i<2 ; i++)
        for (j=0 ; j<11 ; j++)
            mt_quad[i][j] = 0;
    for (i=0 ; i<3 ; i++)
        for (j=0 ; j<11 ; j++)
            mt_double_v[i][j] = 0;
    for (i=0 ; i<12 ; i++)
        mt_triple[i] = 0;
    for (i=0 ; i<11 ; i++)
        mt_six[i] = 0;
    for (i=0 ; i<3 ; i++)
    {
        mt_dozen[i] = 0;
        mt_column[i] = 0;
    }
    for (i=0 ; i<2 ; i++)
    {
        mt_dozen_x2[i] = 0;
        mt_column_x2[i] = 0;
        mt_half[i] = 0;
        mt_color[i] = 0;
        mt_parity[i] = 0;
    }
}

bool wxRouletteTable::Bet(wxArrayString* pBets, bool pNoWarning)
{
    size_t i;
    int j, x, y;
    wxString buffer;
    bool result;

    //-- Returns
    if ((pBets == NULL) || (m_bet == 0))
        return false;

    //-- Processes the bets
    result = true;
    for (i=0 ; i<pBets->GetCount() ; i++)
    {
        buffer = pBets->Item(i);
        if (buffer.IsEmpty())
            continue;

        //- Color
        if (buffer == wxT("black")) { mt_color[0] += m_bet; continue; }
        if (buffer == wxT("red"))   { mt_color[1] += m_bet; continue; }

        //- Parity
        if (buffer == wxT("even"))  { mt_parity[0] += m_bet; continue; }
        if (buffer == wxT("odd"))   { mt_parity[1] += m_bet; continue; }

        //- Half
        if ((buffer == wxT("low"))  || (buffer == wxT("1-18")))  { mt_half[0] += m_bet; continue; }
        if ((buffer == wxT("high")) || (buffer == wxT("19-36"))) { mt_half[1] += m_bet; continue; }

        //- Column
        if ((buffer == wxT("column_1"))  || (buffer == wxT("1-34"))) { mt_column[0]    += m_bet; continue; }
        if ((buffer == wxT("column_2"))  || (buffer == wxT("2-35"))) { mt_column[1]    += m_bet; continue; }
        if ((buffer == wxT("column_3"))  || (buffer == wxT("3-36"))) { mt_column[2]    += m_bet; continue; }
        if ((buffer == wxT("column_12")) || (buffer == wxT("1-35"))) { mt_column_x2[0] += m_bet; continue; }
        if ((buffer == wxT("column_23")) || (buffer == wxT("2-36"))) { mt_column_x2[1] += m_bet; continue; }

        //- Dozen
        if ((buffer == wxT("dozen_1"))  || (buffer == wxT("1-12")))  { mt_dozen[0]    += m_bet; continue; }
        if ((buffer == wxT("dozen_2"))  || (buffer == wxT("13-24"))) { mt_dozen[1]    += m_bet; continue; }
        if ((buffer == wxT("dozen_3"))  || (buffer == wxT("25-36"))) { mt_dozen[2]    += m_bet; continue; }
        if ((buffer == wxT("dozen_12")) || (buffer == wxT("1-24")))  { mt_dozen_x2[0] += m_bet; continue; }
        if ((buffer == wxT("dozen_23")) || (buffer == wxT("13-36"))) { mt_dozen_x2[1] += m_bet; continue; }

        //- Line
        for (j=0 ; j<11 ; j++)
        {
            if (buffer == wxString::Format(wxT("%d-%d"), 3*j+1, (j+2)*3))
            {
                mt_six[j] += m_bet;
                goto next_bet;
            }
        }

        //- Corner
        for (x=0 ; x<2 ; x++)
            for (y=0 ; y<11 ; y++)
            {
                if (buffer == wxString::Format(wxT("%d-%d"), 3*y+x+1, 3*y+x+5))
                {
                    mt_quad[x][y] += m_bet;
                    goto next_bet;
                }
            }

        //- Street
        for (j=0 ; j<12 ; j++)
        {
            if (buffer == wxString::Format(wxT("%d-%d"), 3*j+1, (j+1)*3))
            {
                mt_triple[j] += m_bet;
                goto next_bet;
            }
        }

        //- Split (horizontal)
        for (x=0 ; x<2 ; x++)
            for (y=0 ; y<12 ; y++)
            {
                if (buffer == wxString::Format(wxT("%d-%d"), 3*y+x+1, 3*y+x+2))
                {
                    mt_double_h[x][y] += m_bet;
                    goto next_bet;
                }
            }

        //- Split (vertical)
        for (x=0 ; x<3 ; x++)
            for (y=0 ; y<11 ; y++)
            {
                if (buffer == wxString::Format(wxT("%d-%d"), 3*y+x+1, 3*y+x+4))
                {
                    mt_double_v[x][y] += m_bet;
                    goto next_bet;
                }
            }

        //- Straight
        wxASSERT((0<=m_type) && (m_type<=3));
        for (j=(m_type==wxRouletteType::BALANCED?1:0) ; j<roulette_max_size-(3-m_type) ; j++)
        {
            if (buffer == wxString::Format(wxT("%d"), j))
            {
                mt_single[j] += m_bet;
                goto next_bet;
            }
        }

        //- Not found
        result = false;

        //- Next
next_bet:
        continue;
    }

    //-- Result
    if (!result && !pNoWarning && (m_parent_class != NULL))
        m_parent_class->LogWarning(_("Some bets were not processed."));
    return result;
}

double wxRouletteTable::GetStake()
{
    int i, j;
    double total = 0;

    //-- Performs a sum
    for (i=0 ; i<roulette_max_size ; i++)
        total += mt_single[i];
    for (i=0 ; i<2 ; i++)
        for (j=0 ; j<12 ; j++)
            total += mt_double_h[i][j];
    for (i=0 ; i<2 ; i++)
        for (j=0 ; j<11 ; j++)
            total += mt_quad[i][j];
    for (i=0 ; i<3 ; i++)
        for (j=0 ; j<11 ; j++)
            total += mt_double_v[i][j];
    for (i=0 ; i<12 ; i++)
        total += mt_triple[i];
    for (i=0 ; i<11 ; i++)
        total += mt_six[i];
    for (i=0 ; i<3 ; i++)
    {
        total += mt_dozen[i];
        total += mt_column[i];
    }
    for (i=0 ; i<2 ; i++)
    {
        total += mt_dozen_x2[i];
        total += mt_column_x2[i];
        total += mt_half[i];
        total += mt_color[i];
        total += mt_parity[i];
    }
    return total;
}

bool wxRouletteTable::ShowTable()
{
    wxString buffer;
    int x, y;

    //-- Inits
    buffer.Alloc(1024);
    if (GetStake() == 0)
    {
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(_("There is no stake on the table."));
        return false;
    }
    else
    {
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(_("The table contains the following stakes :"));
    }

    //-- Straight (1)
    buffer.Empty();
    for (x=0 ; x<roulette_max_size ; x++)
    {
        if (mt_single[x] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %s"), mt_single[x], wxRouletteHelper::FormatNumber(x).uniCStr()));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Straight : %s"), buffer.uniCStr()));

    //-- Split (2)
    buffer.Empty();
    for (x=0 ; x<2 ; x++)               //Horizontal
        for (y=0 ; y<12 ; y++)
        {
            if (mt_double_h[x][y] == 0)
                continue;
            if (!buffer.IsEmpty())
                buffer.Append(wxT(" / "));
            buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_double_h[x][y], 3*y+x+1, 3*y+x+2));
        }
    for (x=0 ; x<3 ; x++)               //Vertical
        for (y=0 ; y<11 ; y++)
        {
            if (mt_double_v[x][y] == 0)
                continue;
            if (!buffer.IsEmpty())
                buffer.Append(wxT(" / "));
            buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_double_v[x][y], 3*y+x+1, 3*y+x+4));
        }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Split : %s"), buffer.uniCStr()));

    //-- Street (3)
    buffer.Empty();
    for (y=0 ; y<12 ; y++)
    {
        if (mt_triple[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_triple[y], 3*y+1, 3*(y+1)));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Street : %s"), buffer.uniCStr()));

    //-- Corner (4)
    buffer.Empty();
    for (x=0 ; x<2 ; x++)
        for (y=0 ; y<11 ; y++)
        {
            if (mt_quad[x][y] == 0)
                continue;
            if (!buffer.IsEmpty())
                buffer.Append(wxT(" / "));
            buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_quad[x][y], 3*y+x+1, 3*y+x+5));
        }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Corner : %s"), buffer.uniCStr()));

    //-- Line (6)
    buffer.Empty();
    for (y=0 ; y<11 ; y++)
    {
        if (mt_six[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_six[y], 3*y+1, 3*(y+2)));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Line : %s"), buffer.uniCStr()));

    //-- Dozen (12)
    buffer.Empty();
    for (y=0 ; y<3 ; y++)
    {
        if (mt_dozen[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_dozen[y], 12*y+1, 12*(y+1)));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Dozen : %s"), buffer.uniCStr()));

    //-- Dozens (24)
    buffer.Empty();
    for (y=0 ; y<2 ; y++)
    {
        if (mt_dozen_x2[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_dozen_x2[y], 12*y+1, 12*(y+2)));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Dozens : %s"), buffer.uniCStr()));

    //-- Column (12)
    buffer.Empty();
    for (x=0 ; x<3 ; x++)
    {
        if (mt_column[x] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_column[x], 1+x, 34+x));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Column : %s"), buffer.uniCStr()));

    //-- Columns (24)
    buffer.Empty();
    for (x=0 ; x<2 ; x++)
    {
        if (mt_column_x2[x] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_column_x2[x], 1+x, 35+x));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Columns : %s"), buffer.uniCStr()));

    //-- Columns (24)
    buffer.Empty();
    for (x=0 ; x<2 ; x++)
    {
        if (mt_half[x] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %d-%d"), mt_half[x], 18*x+1, 18*(x+1)));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Half : %s"), buffer.uniCStr()));

    //-- Color
    buffer.Empty();
    for (y=0 ; y<2 ; y++)
    {
        if (mt_color[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %s"), mt_color[y], (y==0 ? _("Black") : _("Red"))));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Color : %s"), buffer.uniCStr()));

    //-- Parity
    buffer.Empty();
    for (y=0 ; y<2 ; y++)
    {
        if (mt_parity[y] == 0)
            continue;
        if (!buffer.IsEmpty())
            buffer.Append(wxT(" / "));
        buffer.Append(wxString::Format(_("%.2f on %s"), mt_parity[y], (y==0 ? _("Even") : _("Odd"))));
    }
    if (!buffer.IsEmpty())
        if (m_parent_class != NULL)
            m_parent_class->LogMessage(wxString::Format(_("   - Parity : %s"), buffer.uniCStr()));

    //-- Final
    return true;
}

int wxRouletteTable::GetColor(int pLanded)
{
    int i;
    bool color;

    //-- Green
    if ((pLanded == roulette_zero) || (pLanded == roulette_zero_double) || (pLanded == roulette_zero_triple))
        return 2;

    //-- Finds the red or black
    color = m_black_first;
    for (i=0 ; i<roulette_max_size ; i++)
    {
        if ((m_layout[i] == roulette_zero) || (m_layout[i] == roulette_zero_double) || (m_layout[i] == roulette_zero_triple))
            continue;
        if (m_layout[i] == pLanded)
            break;
        else
            color = !color;
    }
    return (color ? 0 : 1);
}

bool wxRouletteTable::IsZero(int pLanded)
{
    return ((pLanded==roulette_zero) || (pLanded==roulette_zero_double) || (pLanded==roulette_zero_triple));
}

int wxRouletteTable::GetHalf(int pLanded)
{
    return (IsZero(pLanded) ? 0 : (unsigned long)(floor((pLanded-1)/18)+1));
}

int wxRouletteTable::GetDozen(int pLanded)
{
    return (IsZero(pLanded) ? 0 : (unsigned long)(floor((pLanded-1)/12)+1));
}

int wxRouletteTable::GetColumn(int pLanded)
{
    return (IsZero(pLanded) ? 0 : ((pLanded-1) % 3)+1);
}

int wxRouletteTable::GetStreet(int pLanded)
{
    return (IsZero(pLanded) ? 0 : floor((pLanded-1)/3)+1);
}

double wxRouletteTable::GetGain(int pLanded)
{
    double gain;
    int i, j, n, x, y;

    //-- Checks
    if ((pLanded < 0) || (pLanded > m_nbcell))
        return 0;

    //-- Calculates the gains
    gain = 0;
    if (IsZero(pLanded))
        gain += mt_single[pLanded];
    else
    {
        //- Absolute position of the landed number
        y = floor((pLanded-1) / 3);
        x = (pLanded - 1) % 3;

        //- Single (35 to 1) -- Straight
        gain += 36 * mt_single[pLanded];

        //- Double horizontally (17 to 1) -- Split H
        if ((x == 0) || (x == 1))
            gain += 18 * mt_double_h[0][y];
        if ((x == 1) || (x == 2))
            gain += 18 * mt_double_h[1][y];

        //- Double vertically (17 to 1) -- Split V
        if (y == 0)
            gain += 18 * mt_double_v[x][y];
        else
            if (y == 11)
                gain += 18 * mt_double_v[x][y-1];
            else
                gain += 18 * (mt_double_v[x][y-1] + mt_double_v[x][y]);

        //- Triple (11 to 1) -- Street
        gain += 12 * mt_triple[y];

        //- Quad (8 to 1) -- Corner
        for (i=0 ; i<2 ; i++)
            for (j=0 ; j<11 ; j++)
            {
                n = 3*j + i + 1;
                if ((pLanded == n  ) ||
                    (pLanded == n+1) ||
                    (pLanded == n+3) ||
                    (pLanded == n+4)
                )
                    gain += 9 * mt_quad[i][j];
            }

        //- Six (5 to 1) -- Line
        if (y == 0)
            gain += 6 * mt_six[y];
        else
            if (y == 11)
                gain += 6 * mt_six[y-1];
            else
                gain += 6 * (mt_six[y-1] + mt_six[y]);

        //- Dozen (2 to 1)
        j = floor((pLanded-1) / 12);
        gain += 3 * mt_dozen[j];
        if (j <= 1)
            gain += 1.5 * mt_dozen_x2[0];
        if (j >= 1)
            gain += 1.5 * mt_dozen_x2[1];

        //- Column (2 to 1)
        i = (pLanded-1) % 3;
        gain += 3 * mt_column[i];
        if (i <= 1)
            gain += 1.5 * mt_column_x2[0];
        if (i >= 1)
            gain += 1.5 * mt_column_x2[1];

        //- Half (1 to 1)
        j = floor((pLanded-1)/18);
        gain += 2 * mt_half[j];

        //- Color (1 to 1)
        gain += 2 * mt_color[GetColor(pLanded)];

        //- Parity (1 to 1)
        gain += 2 * mt_parity[pLanded % 2];
    }

    return gain;
}
