
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

#ifndef history_h
#define history_h

/**
 * \file history.h
 * \brief Item for the history of the spins.
 * \author ecrucru
 * \version 0.1
 * \date November 2015
 *
 * This class stores the history of the spins.
 */


//------------------------------------------

/** \class wxRouletteHistory
 *  \brief Item for the history of the spins.
 *
 * This class stores the history of the spins.
 */
class wxRouletteHistory
{
    public:
        double Bet,                 /**< Bet. */
               Stake,               /**< Money on the table. */
               LandedNumber,        /**< Landed number. */
               Win,                 /**< Status for winning. */
               Gain,                /**< Amount of won money. */
               Cash,                /**< Current cash. */
               Credit,              /**< Current credit. */
               Net;                 /**< Net result equal to "Cash - Credit". */

        /** Constructor. */
        wxRouletteHistory()
        {
            Reset();
        }

        /** Resets data. */
        void Reset()
        {
            Bet = 0;
            Stake = 0;
            LandedNumber = 0;
            Win = 0;
            Gain = 0;
            Cash = 0;
            Credit = 0;
            Net = 0;
        }
};

#endif
