
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

#ifndef table_h
#define table_h

/**
 * \file table.h
 * \brief Table of the game including the random number generator
 * \author ecrucru
 * \version 0.1
 * \date October 2015
 *
 * This file defines the behaviour of the table.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxEcMath/ec_defs.h"
#include "defs.h"
#include "random.h"
#include "http_prng.h"


//------------------------------------------

class wxRoulette;           //Included at the end of this file


//------------------------------------------

/** \class wxRouletteTable
 *  \brief Table of the game.
 *
 * The class simulates the behaviour of the roulette.
 */
class wxRouletteTable
{
    private:
        wxRouletteType::wxRouletteType  m_type;             /**< Type of the roulette. */
        bool                m_black_first;                  /**< Tells if the first cell of the layout is black. */
        int                 m_nbcell;                       /**< Number of possible numbers. */
        int                 m_layout[roulette_max_size];    /**< Layout of the roulette. */
        wxRoulette          *m_parent_class;                /**< Parent class. */

        wxRouletteAlgorithm::wxRouletteAlgorithm m_rng;     /**< Random number generator to use. */
        wxRouletteRandom    m_rand_ball;                    /**< Random number generator for the ball. */
        wxRouletteRandom    m_rand_wheel;                   /**< Random number generator for the rotating wheel. */
        wxRouletteRandom    m_rand_generator;               /**< Random number generator for miscellaneous uses. */
        wxRouletteRandomHTTP *m_rand_http;                  /**< Online random number generator. */
        bool                m_ball_direction;               /**< Spinning direction of the ball. */
        int                 m_ball_position;                /**< Relative position of the ball to determine the number. */
        int                 m_ball_initial_position;        /**< Initial position of the ball. */

        double              m_bet,                          /**< Amount of the counter. */
                            mt_single[roulette_max_size],   /**< Bets for 1 number. */
                            mt_double_h[2][12],             /**< Bets for 2 numbers horizontally. */
                            mt_double_v[3][11],             /**< Bets for 2 numbers vertically. */
                            mt_triple[12],                  /**< Bets for 3 numbers. */
                            mt_quad[2][11],                 /**< Bets for 4 numbers. */
                            mt_six[11],                     /**< Bets for 6 numbers. */
                            mt_dozen[3],                    /**< Bets for 12 horizontal numbers. */
                            mt_dozen_x2[2],                 /**< Bets for 24 horizontal numbers. */
                            mt_column[3],                   /**< Bets for 12 vertical numbers. */
                            mt_column_x2[2],                /**< Bets for 24 vertical numbers. */
                            mt_half[2],                     /**< Bets for 18 numbers by position. */
                            mt_color[2],                    /**< Bets for 18 numbers by color (0=black, 1=red). */
                            mt_parity[2];                   /**< Bets for 18 numbers by parity (0=even, 1=odd). */

        /**
         * Sets the type of the roulette.
         * \param pType New type of the roulette.
         */
        void DoSetType(wxRouletteType::wxRouletteType pType);
        /**
         * Defines the layout of the roulette. The zeros are automatically defined according to the roulette type.
         * \param pLayout Table of 36 positions.
         * \return \a true if successful, else \a false.
         */
        bool DoSetLayout(int* pLayout);
        /**
         * Updates the internal status of the HTTP interface in order to request the right random numbers.
         * \return \a true if successful, else \a false.
         */
        bool DoUpdateHTTPSettings();
        /**
         * Launches a spin following the expected algorithm.
         * \param pBall Result for spin 1.
         * \param pWheel Result for spin 2.
         * \param pRandom Result to get a random number not related with the game.
         * \remark The spin occurs even if the target variable is NULL.
         */
        void DoGetNumbers(unsigned long *pBall, unsigned long *pWheel, unsigned long *pRandom);

    public:
        /** Constructor. */
        wxRouletteTable();
        /** Destructor. */
        ~wxRouletteTable();
        /**
         * Sets a pointer to the parent class.
         * \param pParent Parent class.
         */
        inline void SetParentClass(wxRoulette* pParent) { m_parent_class = pParent; }
        /**
         * Gets a pointer to the parent class.
         * \return Parent class.
         */
        inline wxRoulette* GetParentClass() { return m_parent_class; }

        bool SetFrenchLayout  (bool pBalanced);                             /**< Defines a French roulette. */
        bool SetAmericanLayout(bool pBalanced);                             /**< Defines an American roulette. */
        bool SetMexicanLayout (bool pBalanced);                             /**< Defines a Mexican roulette. */
        void ShowLayout();                                                  /**< Displays the layout of the roulette. */
        bool SetAlgorithm(wxRouletteAlgorithm::wxRouletteAlgorithm pAlgo);  /**< Sets the algorithm for the random generator. */
        void ShowAlgorithm();                                               /**< Displays the random number generator. */
        void ShowDistribution();                                            /**< Calculates and displays the distribution of the random number generator. */
        /** Save the distribution of the algorithm in a bitmap (*.bmp).
         * \param pFileName Name of the file.
         * \param pSize Size of the bitmap.
         * \param pHotMap \a true for a hotmap, \a false for a distribution.
         * \param pNumPoints Number of points to draw.
         * \return \a true if saved, else \a false.
         */
        bool SaveDistribution(wxString pFileName, int pSize = 256, bool pHotMap = false, unsigned long pNumPoints = 32768);
        void ShowSeeds();                                                   /**< Display the initial seed used by the random number generators. */

        /**
         * Launches the ball and the wheel in reverse direction to get a number.
         * If you use an online service, the random number is get directly.
         * \param pResult Pointer to store the landed number
         * \return \a true if successful, else \a false on error.
         * \see roulette_zero, roulette_zero_double, roulette_zero_triple.
         */
        bool Spin(unsigned long *pResult);
        /**
         * Tells if the spin is valid or not.
         * \param pSpin Landed number to get.
         * \return \a true if valid, else \a false.
         * \remark The validity is linked to the layout of the table.
         */
        inline bool IsValidSpin(int pSpin)
        {
            return (   (wxRouletteHelper::GetMinNumberFromType(m_type) <= pSpin)
                    && (pSpin <= wxRouletteHelper::GetMaxNumberFromType(m_type))
            );
        }
        /** Clears all the bets currently played on the table. */
        void ClearBets();
        /**
         * Bets on the positions given by the array.
         * \param pBets Pointer to an array of positions.
         * \param pNoWarning Prevents warnings to be generated if a wrong bet is submitted.
         * \return \a true if all the entries have been processed, else \a false.
         */
        bool Bet(wxArrayString* pBets, bool pNoWarning);
        /**
         * Calculates the money engaged on the table.
         * \return Total played money.
         */
        double GetStake();
        /**
         * Displays all the saved bets.
         * \return \a true if the bets have been shown, else \a false (no available bet).
         */
        bool ShowTable();
        /**
         * Returns the color of the landed number.
         * \param pLanded ID of the landed number.
         * \return 0 for black, 1 for red, 2 for green.
         */
        int GetColor(int pLanded);
        /**
         * Tells if the landed number is one of the zeros.
         * \param pLanded ID of the landed number.
         * \return \a true if it is the case, else \a false.
         */
        bool IsZero(int pLanded);
        /**
         * Indicates which half of the table has been hit by the landed number.
         * \param pLanded ID of the landed number.
         * \return Value from 0 to 2.
         */
        int GetHalf(int pLanded);
        /**
         * Indicates which dozen of the table has been hit by the landed number.
         * \param pLanded ID of the landed number.
         * \return Value from 0 to 3.
         */
        int GetDozen(int pLanded);
        /**
         * Indicates which column of the table has been hit by the landed number.
         * \param pLanded ID of the landed number.
         * \return Value from 0 to 3.
         */
        int GetColumn(int pLanded);
        /**
         * Indicates which line of the table has been hit by the landed number.
         * \param pLanded ID of the landed number.
         * \return Value from 0 to 12.
         */
        int GetStreet(int pLanded);
        /**
         * Gets the winning amount for a landed number.
         * \param pLanded ID of the landed number.
         * \return Total amount of gains, including the initial bets.
         */
        double GetGain(int pLanded);
        /** Returns the current bet used on the table. */
        inline double GetBet() { return m_bet; }
        /**
         * Sets the played price for one bet.
         * \param pBet Amount in currency to be played.
         * \return \a true if changed, else \a false.
         */
        inline bool SetBet(double pBet)
        {
            if (pBet > 0)
            {
                m_bet = pBet;
                return true;
            }
            else
                return false;
        }
        /**
         * Generates a random number by using the default algorithm.
         * \return A raw random number with no modulo.
         */
        bool GetRandomNumber(unsigned long *pResult)
        {
            //-- Checks
            if (pResult == NULL)
                return false;

            //-- Gets a value
            if (wxRouletteHelper::IsOnlineGenerator(m_rng))
            {
                wxASSERT(m_rand_http != NULL);
                return m_rand_http->PickNumber(pResult);
            }
            else
            {
                DoGetNumbers(NULL, NULL, pResult);
                return true;
            }
        }
        /**
         * Returns the remaining quota available for the random generator.
         * \return A positive value, or (unsigned long)-1 if the quota doesn't apply.
         */
        unsigned long GetRemainingQuota()
        {
            if (wxRouletteHelper::IsOnlineGenerator(m_rng))
                return m_rand_http->GetRemainingQuota();
            else
                return -1;
        }
        /**
         * Randomizes again the plate to set an intial situation that we can
         * recall later with instruction "CLEAR RANDOM".
         * \return \a true if the randomization is supported, else \a false.
         */
        bool Randomize()
        {
            if (wxRouletteHelper::IsOnlineGenerator(m_rng))
                return false;
            else
            {
                m_ball_position = m_ball_initial_position;
                m_ball_direction = true;
                m_rand_ball.Randomize();
                m_rand_wheel.Randomize();
                //m_rand_generator is randomized only once
                return true;
            }
        }
        /**
         * Resets the status used in the random functions in order to regenerate the same numbers.
         * \remark This is useful to simulate various scenarios in the same script.
         * \return \a true if successful, else \a false if not supported.
         */
        bool RestorePRNG()
        {
            if (    (m_rng == wxRouletteAlgorithm::CLOCK) ||
                    (m_rng == wxRouletteAlgorithm::CUSTOM) ||
                    wxRouletteHelper::IsOnlineGenerator(m_rng)
            )
                return false;
            else
            {
                m_ball_position = m_ball_initial_position;
                m_ball_direction = true;
                m_rand_ball.Restore();
                m_rand_wheel.Restore();
                //m_rand_generator is not used for the spins. A reset is not necessary
                //since a random number still needs to be generated at any moment.
                return true;
            }
        }
        /**
         * Returns the active pseudo-random number generator.
         * \return ID of the algorithm.
         */
        inline wxRouletteAlgorithm::wxRouletteAlgorithm GetPRNG() { return m_rng; }
        /**
         * Returns the count of numbers which can be played.
         * \return Up to roulette_max_size.
         */
        int GetCellCount() { return m_nbcell; };
        /**
         * Returns the current type of the roulette.
         * \return ID of the type.
         */
        inline wxRouletteType::wxRouletteType GetRouletteType() { return m_type; }
        /**
         * Indicates if the wheel is balanced.
         * \return \a true if the wheel has no zero, else \a false.
         */
        bool IsBalanced() { return m_type == wxRouletteType::BALANCED; }
};


//------------------------------------------

#include "roulette.h"

#endif
