
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

#ifdef roulette_h

case roulette_inst_spin_id:
{
    //Checks if a forced spin is requested
    forced_spin = roulette_invalid_spin;
    if (parser.HasParameters())
    {
        if (m_engine.SetFormula(parser.Command))
        {
            m_engine.Compute();
            if (m_engine.GetLastError() == wxECE_NOERROR)
            {
                forced_spin = m_engine.GetLastResult();
                if (!m_table.IsValidSpin(forced_spin))
                    forced_spin = roulette_invalid_spin;
                else
                    parser.ClearParameters();
            }
        }
    }

    //Checks the parameters
    if (parser.HasParameters())
    {
        if (!parser.NoWarning)
            LogWarning(_("No valid argument has been found for the instruction 'SPIN'."));
        DoClearLandedNumber();
        //Do not remove the instruction here
        continue;
    }

    //Reads the current statistics
    stake = m_table.GetStake();
    cash = GetCash();

    //Checks for "Casino attacks"
    ulvalue = GetMaxBet();
    if (ulvalue > 0)        //A maximal bet has been specified
        if (stake > ulvalue)
        {
            if (!parser.NoWarning)
                LogError(wxString::Format(_("The instruction 'SPIN' is cancelled because the bets have exceeded the limit (%d > %d)."), stake, ulvalue));
            continue;
        }

    //Increases the credit
    credit = GetCredit();
    if (cash < 0)
    {
        credit += stake;
        cash += stake;                //An increase of the credit generates some new money for the player
    }
    else
        if (stake > cash)
            credit += stake - cash;

    //Spins
    if (!DoSpin(forced_spin, &landed))
    {
        LogSystem(wxString::Format(_("No valid random number was generated. Please fix your access to the network and wait %d minutes."), roulette_http_delay));
        DoClearLandedNumber();
        RequestStop();              //The error is fatal
        continue;
    }

    //Reads the statistics
    gain = m_table.GetGain(landed);
    win = (gain > 0);
    relative_gain = gain - stake;
    cash += relative_gain;
    if (cash > 0)                    //Reimburses the credit
    {
        if (credit >= cash)
        {
            credit -= cash;
            cash = 0;
        }
        else
        {
            cash -= credit;
            credit = 0;
        }
    }
    else
    {
        credit -= cash;             //Any negative money is transfered as a credit
        cash = 0;
    }

    //Update the statistics
    m_engine.SetConstant(roulette_vars_gain, relative_gain);
    m_engine.SetConstant(roulette_vars_landed_win, (win ? 1 : 0));
    m_engine.SetConstant(roulette_vars_cash, cash);
    m_engine.SetConstant(roulette_vars_credit, credit);
    m_engine.SetConstant(roulette_vars_net, cash-credit);
    if (stake > 0)
    {
        m_engine.GetConstant(roulette_vars_stats_win_cnt, &fa, 0);
        m_engine.GetConstant(roulette_vars_stats_loss_cnt, &fb, 0);
        if (win)
            fa++;
        else
            fb++;
        m_engine.SetConstant(roulette_vars_stats_win_cnt, fa);
        m_engine.SetConstant(roulette_vars_stats_loss_cnt, fb);
        if (fa + fb != 0)
            m_engine.SetConstant(roulette_vars_stats_win_rate, fa/(fa+fb));
        else
            m_engine.SetConstant(roulette_vars_stats_win_rate, 0);
        m_engine.SetConstant(roulette_vars_rounds, fa+fb);
    }
    if (stake > 0)
    {
        m_engine.GetConstant(roulette_vars_plot, &fvalue, 1);
        if (fvalue == 1)
        {
            m_cash.AddData(wxRealPoint(m_cash.GetCount(), cash));
            m_credit.AddData(wxRealPoint(m_credit.GetCount(), credit));
        }
    }
    DoUpdateDynamicVariables(parser.NoWarning);

    //Store the history of the spins
    history.Reset();
    m_engine.GetConstant(roulette_vars_bet,           &history.Bet,          -1);
    m_engine.GetConstant(roulette_vars_stake,         &history.Stake,        -1);
    m_engine.GetConstant(roulette_vars_landed_number, &history.LandedNumber, -1);
    m_engine.GetConstant(roulette_vars_landed_win,    &history.Win,          -1);
    m_engine.GetConstant(roulette_vars_gain,          &history.Gain,         -1);
    m_engine.GetConstant(roulette_vars_cash,          &history.Cash,         -1);
    m_engine.GetConstant(roulette_vars_credit,        &history.Credit,       -1);
    m_engine.GetConstant(roulette_vars_net,           &history.Net,          -1);
    m_spin_history.AddData(history);

    //Finalization
    m_table.ClearBets();
    continue;
}

#endif
