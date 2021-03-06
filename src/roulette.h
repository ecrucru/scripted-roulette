
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

#ifndef roulette_h
#define roulette_h

/**
 * \file roulette.h
 * \brief Processor for the roulette
 * \author ecrucru
 * \since Version 0.1, October 2015
 *
 * This file contains all the algorithms to execute a simulation for the roulette.
 *
 * \todo Mini-roulette
 * \todo Instruction POINT
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/gdicmn.h"
#include "wx/filename.h"

#include "defs.h"
#ifndef _CONSOLE
    #include "debugger.h"
#endif
#include "engine.h"
#include "instruction.h"
#include "sequence.h"
#include "table.h"
#include "history.h"
#include "storage.h"


//------------------------------------------

#ifndef _CONSOLE
    class wxRouletteFrame;          //Included at the end of this file
#endif


//------------------------------------------

/** \class wxRoulette
 *  \brief Core class.
 *
 * The class pilots the execution of the script.
 */
class wxRoulette
{
    private:
        wxRouletteEngine                m_engine,               /**< Interpreter for any formula. */
                                        m_backup;               /**< Backup of the variables */
        wxArrayString                   m_script;               /**< Content of the script to execute. */
        bool                            m_script_running;       /**< Status telling if a script is running. */
        bool                            m_stop_requested;       /**< Flag to request a stop of the script. */
        bool                            m_leave_requested;      /**< Flag to request the leave of the application. */
        bool                            m_restart_requested;    /**< Flag to request a restart of the script. */
        unsigned long                   m_sequence_count;       /**< Number of sequences contained in the table. */
        wxRouletteSequence              **m_sequences;          /**< Tables containing the sequences. */
        wxRouletteTable                 m_table;                /**< Gameplay. */
        wxRouletteStorage<wxRealPoint>  m_cash;                 /**< Evolution of the cash owned by the player. */
        wxRouletteStorage<wxRealPoint>  m_credit;               /**< Evolution of the credit when you have no more cash. */
        wxRouletteStorage<wxRouletteHistory> m_spin_history;    /**< History of the spins. */
        wxArrayString                   m_log;                  /**< History of the messages. */
        wxDateTime                      m_time_last;            /**< Last value of the time stamp. */
        bool                            m_time_set;             /**< Flag to tell if the last time stamp is set. */
        bool                            m_debugger_skip;        /**< Flag to ignore the debugger until the end of the execution of the script. */
    #ifndef _CONSOLE
        wxRouletteDebugger              *m_debugger;            /**< Debugger. */
        bool                            m_debugger_enabled;     /**< Flag for the use of the debugger. */
        wxRouletteFrame                 *m_frame;               /**< Parent frame. */
    #endif

        /**
         * Remove all the sequences.
         * \param pDestroy Set to true if you want to destroy all the data,
         *                 else \a false if you expect to reuse the object for new tasks
         */
        void DoFreeSequences(bool pDestroy);
        void DoResetConstants();                        /**< Resets the memory to the initial constants. */
                                                        /**  Performs a copy of the variables from one engine to another one. */
        bool DoCopyVariables(wxRouletteEngine *pFrom, wxRouletteEngine *pTo);
        bool DoLoad(bool *pVersionMatch);               /**< Format the script in the right format. */
        void DoDumpScript();                            /**< Displays the processed script. */
        bool DoInitSequences();                         /**< Splits the script into processable parts. */
        /**
         * Adds a new sequence to the loaded ones.
         * \param pSequence Pointer to a sequence generated by the calling program.
         * \param pForInit  Tells if the sequence is used for the initialization.
         * \return \a true if successful, else \a false.
         * \remark It is not possible to have multiple initializations.
         */
        bool DoAddSequence(wxRouletteSequence *pSequence, bool pForInit);
        void DoDumpSequences();                         /**< Displays the script for each loaded sequences. */
        /**
         * Executes a sequence.
         * \param pSequence Sequence.
         * \param pInit     Tells if it is the sequence for the initialization.
         * \return \a true if successful, else \a false.
         */
        bool DoExecuteSequence(wxRouletteSequence *pSequence, bool pInit);
        /**
         * Displays all the variables set in the roulette. It helps to identify which values can be changed.
         * \param pNoWarning Pilots the display of warning messages.
         * \return \a true if something has been displayed, else \a false.
         */
        bool DoDumpVariables(bool pNoWarning);
        /**
         * Spins the game and updates the internal memory.
         * \param pForcedSpin Force the spin by setting a value different from 'roulette_invalid_spin'.
         * \param pResult Pointer receiving the landed number.
         * \return \a true if pResult is meaningful, else \a false.
         */
        bool DoSpin(int pForcedSpin, int *pResult);
        /**
         * Saves the history of the spins into a file.
         * \param pFileName Target file name.
         * \return \a true on success, else \a false.
         */
        bool DoSaveSpinHistory(wxString pFileName);
        /**
         * Saves the log into a file.
         * \param pFileName Target file name.
         * \return \a true on success, else \a false.
         */
        bool DoSaveLogHistory(wxString pFileName);
        /**
         * Updates the internal variables helping the user to make decisions.
         * \param pNoWarning Flag to deactivate warnings.
         */
        void DoUpdateDynamicVariables(bool pNoWarning);
        /**
         * Updates the statistics of the game.
         * \return \a true on success, else \a false.
         */
        bool DoUpdateStatistics();
        /** Clears the information related to the landed number. */
        void DoClearLandedNumber();
        /** Clears the statistics of the game. */
        void DoClearStats();
        /** Displays information related to the money. */
        void DoShowMoney();
        /**
         * Spend money to buy new cash and reduce the credit.
         * \remark Precisely, if you have a credit, you are moneyless. However if someone gives
         *         you some money, or if you win a jackpot elsewhere, you can refund yourself.
         *         That's what this method allows to do.
         */
        bool DoBuyStakes(double pAmount);
        /** Clears the logged messages. */
        void DoClearLog();
        /**
         * Inserts a message into the log.
         * \return \a true on success, else \a false.
         */
        bool DoLogMessage(wxRouletteMessageType::wxRouletteMessageType pType, wxString pMsg);

    public:
        /** Constructor. */
        wxRoulette();
        /** Destructor. */
        ~wxRoulette();

        /** Resets the roulette to its initial state. */
        void Reset();
    #ifndef _CONSOLE
        /** Sets the parent frame. */
        inline void SetParent(wxRouletteFrame *pParent) { m_frame = pParent; }
    #endif
        /**
         * Loads a scripted roulette from a file and processes it.
         * \param pFileName File name to load.
         * \param pStart Starts the script at the end of the load.
         * \return \a true if successful, else \a false.
         */
        bool StartFromFile(wxString pFileName, bool pStart);
        /**
         * Loads a script from file without starting it.
         * \param pFileName File name to load.
         * \return \a true if successful, else \a false.
         */
        inline bool LoadFromFile(wxString pFileName) { return StartFromFile(pFileName, false); }
        /**
         * Loads a scripted roulette from an input string and processes it.
         * \param pInput Input to load.
         * \param pStart Starts the script at the end of the load.
         * \return \a true if successful, else \a false.
         */
        bool StartFromInput(wxString pInput, bool pStart);
        /**
         * Loads a scripted roulette from an input string without processing it.
         * \param pInput Input to load.
         * \return \a true if successful, else \a false.
         */
        inline bool LoadFromInput(wxString pInput) { return StartFromInput(pInput, false); }
        /** Executes the scripted roulette. */
        void Execute();
        /** Tells if a script is running. */
        inline bool IsRunning() { return m_script_running; }
        /** Tells if the application must be terminated after a call to instruction LEAVE. */
        inline bool HasToLeave() { return m_leave_requested; }
        /** Requests to stop the execution of the script. */
        bool RequestStop();
        /** Requests to leave the application. */
        bool RequestLeave();
        /**
         * Saves the log into a file.
         * \param pFileName Target file name.
         * \return \a true on success, else \a false.
         */
        inline bool SaveLogHistory(wxString pFileName) { return DoSaveLogHistory(pFileName); }

        /**
         * Returns a pointer of the mathematical parser embedded in the roulette.
         * \remark This function is mostly used to access the internal variables of the game.
         */
        inline wxRouletteEngine* GetMathematicalParser() { return &m_engine; }
        /**
         * Returns a pointer of the table of game.
         * \remark This function is mostly used to access the internal variables of the game.
         */
        inline wxRouletteTable* GetTable() { return &m_table; }
        /**
         * Returns a pointer of the script loaded in the roulette.
         * \remark This function is mostly used to access the internal variables of the game.
         */
        inline wxArrayString* GetScript() { return &m_script; }
        /** Returns the number of loaded sequences. */
        unsigned long CountSequences();
        /** Retrieves the total cash owned by the player. */
        double GetCash();
        /** Retrieves the credit given to the player in case he bets with a negative amount of cash. */
        double GetCredit();
        /** Retrieves the array of the evolution of the cash owned by the player. */
        inline wxRealPoint* GetCashEvolution() { return m_cash.GetData(); }
        /** Retrieves the array of the evolution of the credit owned by the player. */
        inline wxRealPoint* GetCreditEvolution() { return m_credit.GetData(); }
        /** Returns the number of points which describe the evolution of the cash owned by the player. */
        inline unsigned long GetCashCount() { return m_cash.GetCount(); }
        /** Returns the number of points which describe the evolution of the credit owned by the player. */
        inline unsigned long GetCreditCount() { return m_credit.GetCount(); }
        /** Retrieves the amount of the unitary bet. */
        double GetBet();
        /** Retrieves the amount of the maximal allowed bet that mitigate attacks against the casino. */
        double GetMaxBet();

        /**
         * Return the list containing all the messages.
         * \remark The pointer is private and expected to be used in read-only.
         */
        inline wxArrayString* GetLog() { return &m_log; }
        /**
         * Saves a simple message in the log.
         * \param pMsg Message
         */
        inline void LogMessage(wxString pMsg) { DoLogMessage(wxRouletteMessageType::INFO_T, pMsg); }
        /**
         * Saves a warning message in the log.
         * \param pMsg Message
         */
        inline void LogWarning(wxString pMsg) { DoLogMessage(wxRouletteMessageType::WARNING_T, pMsg); }
        /**
         * Saves an error message in the log.
         * \param pMsg Message
         */
        inline void LogError(wxString pMsg) { DoLogMessage(wxRouletteMessageType::ERROR_T, pMsg); }
        /**
         * Saves a debug message in the log.
         * \param pMsg Message
         */
        inline void LogDebug(wxString pMsg) { DoLogMessage(wxRouletteMessageType::DEBUG_T, pMsg); }
        /**
         * Saves a system message in the log.
         * \param pMsg Message
         */
        inline void LogSystem(wxString pMsg) { DoLogMessage(wxRouletteMessageType::SYSTEM_T, pMsg); }
        /**
         * Appends a text to the last generated message.
         * \param pText Text to append.
         * \return \a true if successful, else \a false.
         */
        bool AppendLogMessage(wxString& pText);
};


//------------------------------------------

#ifndef _CONSOLE
    #include "frame.h"
#endif
    #include "table.h"

#endif
