
/*  Copyright (C) 1996-2016, Multiple authors
 *
 *  ---------------------------------------------------------------------------
 *  TT800:
 *
 *     Copyright (C) 1996, Makoto Matsumoto
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. The names of its contributors may not be used to endorse or promote
 *        products derived from this software without specific prior written 
 *        permission.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *     IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *     THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *     PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *     EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *     PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *     PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *     LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ---------------------------------------------------------------------------
 *  Mersenne-Twister:
 *
 *     Copyright (C) 1997-2002, Makoto Matsumoto and Takuji Nishimura,
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. The names of its contributors may not be used to endorse or promote
 *        products derived from this software without specific prior written
 *        permission.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *     IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *     THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *     PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *     EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *     PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *     PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *     LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  ---------------------------------------------------------------------------
 *  Other algorithms:
 *
 *     Scripted Roulette - version 0.1
 *     Copyright (C) 2015-2016, http://scripted-roulette.sourceforge.net
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License along
 *     with this program; if not, write to the Free Software Foundation, Inc.,
 *     51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef random_h
#define random_h

/**
 * \file random.h
 * \brief Random number generator
 * \author Various authors
 * \version 0.1
 * \date October 2015
 *
 * This file defines some random number generators picked from various sources.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#ifdef __BORLANDC__
    #include "sr_tools/sr_tools.h"
#endif


//------------------------------------------

/** \class wxRouletteRandom
 *  \brief Random number generator.
 *
 * The class generates numbers for the roulette.
 */
class wxRouletteRandom
{
    private:
        //-- Initial seeds
        unsigned long m_initial_seed;
        long m_initial_seed_time;
        unsigned long m_initial_l_tab[2];

        //-- Mersenne-Twister
        enum { mt_n = 624, mt_m = 397};
        unsigned long mt_mtab_init[mt_n], mt_mtab[mt_n];
        int mt_mti;

        /** Initializes the generator Mersenne-Twister with a seed. */
        void mt_initialize(unsigned long pSeed)
        {
            static unsigned long mt_numbers[mt_n] = {
                #include "random_init_mt.cpp"
            };
            unsigned long i;

            //-- Initializes the table
            mt_mti = 0;
            for (i=0 ; i<mt_n ; i++)
            {
                mt_mtab_init[i] = mt_numbers[i] ^ pSeed;
                mt_mtab[i] = mt_mtab_init[i] & 0xffffffffUL; //32 bits
            }
        }

        //-- TT800
        enum { tt_n = 25, tt_m = 7 };
        unsigned long tt_x_init[tt_n], tt_x[tt_n];
        int tt_k;

        /** Initializes the generator TT800 with a seed. */
        void tt_initialize(unsigned long pSeed)
        {
            static unsigned long tt_numbers[tt_n] = {
                #include "random_init_tt800.cpp"
            };
            unsigned long i;

            //-- Initializes the table
            tt_k = 0;
            for (i=0 ; i<tt_n ; i++)
            {
                tt_x_init[i] = tt_numbers[i] ^ pSeed;
                tt_x[i] = tt_x_init[i] & 0xffffffffUL; //32 bits
            }
        }

        //-- Linear congruential generator
        enum { lcg_a_sm = 16807UL,    lcg_b_sm = 0UL, lcg_c_sm = 2147483647UL };    //Standard minimal
        enum { lcg_a_rs = 31415821UL, lcg_b_rs = 1UL, lcg_c_rs = 100000000UL };     //Sedgewick
        enum { lcg_a_ru = 65539UL,    lcg_b_ru = 0UL, lcg_c_ru = 2147483648UL };    //RANDU
        unsigned long lcg_key;

        //-- Polynomial bits
        unsigned long pb_status;
        char pb_getbit(void)
        {
            int x = pb_status & 0x4001;
            pb_status = (pb_status << 1) & 0x7fff;
            if ((x == 0) || (x == 0x4001))
            {
                pb_status |= 0;
                return 0;
            }
            else
            {
                pb_status |= 1;
                return 1;
            }
        }

        //-- XorShift
        unsigned long xor_seed;

        //-- Loaded
        unsigned long l_tab[2];

        //-- LnGen
        double ln_seed;

    public:
        /** Constructor. */
        wxRouletteRandom()
        {
            Randomize();
        }

        /** Randomizes all the different algorithms. */
        void Randomize();

        /** Returns the initial seed of the class. */
        inline unsigned long GetInitialSeed() { return m_initial_seed; }
        /** Restores the initial status of the randomization. */
        void Restore()
        {
            //-- Unapplicable : CLOCK, CUSTOM

            //-- Rand
            srand(m_initial_seed_time);

            //-- Mersenne-Twister
            mt_initialize(m_initial_seed);

            //-- TT800
            tt_initialize(m_initial_seed);

            //-- Linear congruential generators
            lcg_key = m_initial_seed;

            //-- Polynomial bits
            pb_status = m_initial_seed;

            //-- XorShift
            xor_seed = m_initial_seed;

            //-- Loaded
            l_tab[0] = m_initial_l_tab[0];
            l_tab[1] = m_initial_l_tab[1];

            //-- LnGen
            if (m_initial_seed > 0)
                ln_seed = log(m_initial_seed/M_PI + 2);
            else
                ln_seed = log(m_initial_seed_time/M_PI + 2);
        }

        /**
         * Returns a random number by using the default and simple C++ generator.
         * \remark This is a system function. Therefore, the random generator for the ball
         *         and the wheel are the same. With the other function, the ball and the
         *         wheel have they own assigned generator.
         */
        int GetRand()
        {
            return rand();
        }

        /** Returns a random number by using the algorithm Mersenne-Twister. */
        unsigned long GetMersenneTwister()
        {
            static unsigned long magic[2] = { 0, 0x9908b0dfUL };
            unsigned long y;
            int kk;

            //-- Generate N words at one time
            if (mt_mti >= mt_n)
            {
                for (kk = 0 ; kk < mt_n-mt_m ; kk++)
                {
                    y = (mt_mtab[kk] & 0x80000000UL) | (mt_mtab[kk+1] & 0x7fffffffUL);
                    mt_mtab[kk] = mt_mtab[kk+mt_m] ^ (y >> 1) ^ magic[y & 0x1UL];
                }
                for ( ; kk < mt_n-1 ; kk++)
                {
                    y = (mt_mtab[kk] & 0x80000000UL) | (mt_mtab[kk+1] & 0x7fffffffUL);
                    mt_mtab[kk] = mt_mtab[kk+(mt_m-mt_n)] ^ (y >> 1) ^ magic[y & 0x1UL];
                }
                y = (mt_mtab[mt_n-1] & 0x80000000UL) | (mt_mtab[0] & 0x7fffffffUL);
                mt_mtab[mt_n-1] = mt_mtab[mt_m-1] ^ (y >> 1) ^ magic[y & 0x1UL];
                mt_mti = 0;
            }

            //-- Tempering
            y = mt_mtab[mt_mti++];
            y ^= (y >> 11);
            y ^= (y <<  7) & 0x9d2c5680UL;
            y ^= (y << 15) & 0xefc60000UL;
            y ^= (y >> 18);
            return y;
        }

        /** Returns a random number by using the algorithm TT800. */
        unsigned long GetTT800()
        {
            unsigned long y;
            static unsigned long magic[2] = { 0x0, 0x8ebfd028 };

            if (tt_k == tt_n)
            {
                int kk;
                for (kk = 0 ; kk < tt_n - tt_m ; kk++)
	                tt_x[kk] = tt_x[kk + tt_m] ^ (tt_x[kk] >> 1) ^ magic[tt_x[kk] % 2];
                for ( ; kk < tt_n ; kk++)
	                tt_x[kk] = tt_x[kk + (tt_m - tt_n)] ^ (tt_x[kk] >> 1) ^ magic[tt_x[kk] % 2];
                tt_k = 0;
            }
            y = tt_x[tt_k];
            y ^= (y << 7) & 0x2b5b2500;
            y ^= (y << 15) & 0xdb8b0000;
            y &= 0xffffffff;
            y ^= (y >> 16);
            tt_k++;
            return y;
        }

        /** Returns a random number by using the clock of the computer. */
        unsigned long GetComputerClock();

        /** Returns a random number by using a linear congruential formula
         *  with special parameters known as "Standard Minimal".
         */
        unsigned long GetStandardMinimal()
        {
            return (lcg_key = (lcg_a_sm * lcg_key + lcg_b_sm) % lcg_c_sm ) >> 16;
        }

        /** Returns a random number by using a linear congruential formula with
         *  special parameters given by Robert Sedgewick. This algorithm generates
         *  every number in a random order.
         */
        unsigned long GetRS()
        {
            return (lcg_key = (lcg_a_rs * lcg_key + lcg_b_rs) % lcg_c_rs) >> 16;
        }

        /** Returns a random number by using a linear congruential formula with
         *  special parameters known as "RANDU". This algorithm is not reliable.
         */
        unsigned long GetRandu()
        {
            return (lcg_key = (lcg_a_ru * lcg_key + lcg_b_ru) % lcg_c_ru);
        }

        /** Returns a random number by using a bit generator. */
        unsigned long GetBits()
        {
            unsigned int i, value = 0;
            for (i=0 ; i<5 ; i++)
                value = (value << 1) + pb_getbit();
            return value;
        }

        /** Returns a random number by using XorShift. */
        unsigned long GetXorShift()
        {
            xor_seed ^= xor_seed << 13;
            xor_seed ^= xor_seed >> 17;
            xor_seed ^= xor_seed << 5;
            return xor_seed;
        }

        /** Returns a random number by using a wrongly adapted version of XorShift128+ from 64 bits to 32 bits.
         *  The distribution has a deviation making some numbers to appear much more or less often.
         */
        unsigned long GetLoaded()
        {
            unsigned long x = l_tab[0];
            unsigned long y = l_tab[1];
            l_tab[0] = y;
            x ^= x << 23;
            x ^= x >> 17;
            x ^= y ^ (y >> 26);
            l_tab[1] = x;
            return x + y;
        }

        /** Returns a random number by using the natural logarithm. */
        unsigned long GetLnGen()
        {
            double local;

            //-- Generates the new number
            if (ln_seed == 0)
            {
                wxASSERT(false);
                ln_seed = log(m_initial_seed);
            }
            else
                if (ln_seed > 1)
                    ln_seed = log(ln_seed);
                else
                    if (ln_seed < -1)
                        ln_seed = log(-ln_seed);
                    else
                        if (ln_seed < 0)
                            ln_seed = log(-1/ln_seed);
                        else
                            ln_seed = log(1/ln_seed);

            //-- Deals with the decimal part of the number to give the final number
            local = (ln_seed < 0 ? -ln_seed : ln_seed);
            return 2349088560UL * (local - floor(local));   //2349088560UL = 39!/33! < 2^32
        }

        /** Returns a random number by using a home-made algorithm having a very low deviation.
         *  This algorithm has no known mathematical background.
         *  \remark It only works with the French roulette.
         */
        unsigned long GetEqui()
        {
            return (xor_seed = xor_seed ^ (xor_seed << 3) ^ (xor_seed >> 5));
        }

        /** Returns a random number by using your own algorithm.
         *  \remark The algorithm always return 1 in the official release.
         */
        unsigned long GetCustom()
        {
            //Write your own code here !
            return 1;
        }
};

#endif
