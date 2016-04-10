
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

#include "random.h"


//------------------------------------------

void wxRouletteRandom::Randomize()
{
    unsigned long seed = 1;

    //-- Rand
    m_initial_seed_time = time(NULL);
    srand(m_initial_seed_time);

    //-- Mersenne-Twister
#ifdef __BORLANDC__
    seed = bcc_randomize();

#elif defined(__VISUALC__)
    __asm                                       //Home-made seed generator
    {
        RDTSC
        ROL     EDX, 5
        ROR     EAX, 3
        XOR     EAX, EDX
        MOV     DWORD PTR [seed], EAX
    }

#else //GCC for example
    __asm__ /*__volatile__*/
    (   "RDTSC                 \n\t"
        "ROL    $5, %%EDX      \n\t"
        "ROR    $3, %%EAX      \n\t"
        "XOR    %%EDX, %%EAX   \n\t"
        "MOVL   %%EAX, %0          "
        : "=m" (seed)
    );
#endif
    if (seed == 0)
        seed++;
    m_initial_seed = seed;

    //-- Mersenne-Twister
    mt_initialize(seed);

    //-- TT800
    tt_initialize(seed);

    //-- Computer clock : nothing needed

    //-- Linear congruential generators
    lcg_key = seed;

    //-- Polynomial bits
    pb_status = seed;

    //-- XorShift
    xor_seed = seed;

    //-- Loaded
    m_initial_l_tab[0] = seed;
    m_initial_l_tab[1] = rand() + 1;            //No zero allowed
    l_tab[0] = m_initial_l_tab[0];
    l_tab[1] = m_initial_l_tab[1];

    //-- LnGen
    if (seed > 0)
        ln_seed = log(seed/M_PI + 2);
    else
        ln_seed = log(m_initial_seed_time/M_PI + 2);
    wxASSERT(ln_seed != 1);
}

unsigned long wxRouletteRandom::GetComputerClock()
{
    unsigned long result = 1;   //Will remove the warning when EAX is directly filled in as a result

    //-- Fetches a number
#ifdef __BORLANDC__
    result = bcc_getcomputerclock();

#elif defined(__VISUALC__)
    __asm
    {
        RDTSC
        ROL     EDX, 6
        NOT     EDX
        ROR     EAX, 4
        XCHG    DH,  AL
        IMUL    EAX, EDX
        XOR     EAX, EDX
        ROR     EAX, 3
        MOV     DWORD PTR [result], EAX
    }

#else //GCC for example
    __asm__ /*__volatile__*/
    (
        "RDTSC                  \n\t"
        "ROL    $6, %%EDX       \n\t"
        "NOT    %%EDX           \n\t"
        "ROR    $4, %%EAX       \n\t"
        "XCHG   %%AL, %%DH      \n\t"
        "IMUL   %%EDX, %%EAX    \n\t"
        "XOR    %%EDX, %%EAX    \n\t"
        "ROR    $3, %%EAX       \n\t"
        "MOVL   %%EAX, %0       "
        : "=m" (result)
    );
#endif

    //-- Turns off the warning
    return result;
}
