
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

#ifndef defs_h
#define defs_h

/**
 * \file defs.h
 * \brief Global definitions
 * \author ecrucru
 * \since Version 0.1, October 2015
 *
 * This file contains some definitions global to the entire application.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/stdpaths.h"
#include "wx/filename.h"
#include "wx/textfile.h"
#include "wx/tokenzr.h"


//------------------------------------------

#define roulette_compile_extended       1                       /**< Option for the compilation : 0=minimal, 1=extended. */
#define roulette_compile_pick_stc       1                       /**< Option for the compilation : 0=don't use STC, 1=use STC. This is the rich text editor with the highlight of the syntax. */
#define roulette_compile_locale         1                       /**< Option for the compilation : 0=don't translate the application, 1=translate the application. */
#define roulette_compile_beta           0                       /**< Option for the compilation : 0=release, 1=cutting-edge version with unstable features. */
#define roulette_compile_msg_sequence   0                       /**< Option for the compilation : 0=disabled, 1=enabled. It helps to reduce the generated system messages when a new sequence is processed. */


//------------------------------------------

#define roulette_zero                   0                       /**< Internal ID of '0'. */
#define roulette_zero_double            37                      /**< Internal ID of '00'. */
#define roulette_zero_triple            38                      /**< Internal ID of '000'. */
#define roulette_max_size               39
#define roulette_invalid_spin           -1                      /**< Internal ID of an invalid number. */

#define roulette_max_includes           100                     /**< Number of includes that cannot be exceeded. This prevents recursive inclusions. */

#define roulette_sect_init              wxT(".init")
#define roulette_sect_sequence          wxT(".sequence")

#define roulette_sect_init_id           3234
#define roulette_sect_sequence_id       4337

#define roulette_dom_game               wxT("game")
#define roulette_dom_sys                wxT("sys")
#define roulette_dom_user               wxT("user")

#define roulette_file_formats           _("Scripted Roulette files (*.sr;*.sri)|*.sr;*.sri")
#define roulette_file_prefix            wxT("scripted_roulette")
#define roulette_file_ext               wxT("sr")
#define roulette_file_ext_inc           wxT("sri")
#define roulette_file_ext_dot           wxT(".sr")
#define roulette_file_ext_inc_dot       wxT(".sri")
#define roulette_file_default           wxT("script.sr")

#define roulette_char_zero              wxChar(0)
#define roulette_char_space             wxChar(0x20)
#define roulette_char_nbspace           wxChar(0xA0)

#define roulette_inst_allocate          wxT("allocate")
#define roulette_inst_append            wxT("append")
#define roulette_inst_assert            wxT("assert")
#define roulette_inst_backup            wxT("backup")
#define roulette_inst_beep              wxT("beep")
#define roulette_inst_bet               wxT("bet")
#define roulette_inst_bits              wxT("bits")
#define roulette_inst_break             wxT("break")
#define roulette_inst_buy               wxT("buy")
#define roulette_inst_cbet              wxT("cbet")
#define roulette_inst_check             wxT("check")
#define roulette_inst_clear             wxT("clear")
#define roulette_inst_confirm           wxT("confirm")
#define roulette_inst_debug             wxT("debug")
#define roulette_inst_dec               wxT("dec")
#define roulette_inst_else              wxT("else")
#define roulette_inst_endif             wxT("endif")
#define roulette_inst_exist             wxT("exist")
#define roulette_inst_goto              wxT("goto")
#define roulette_inst_if                wxT("if")
#define roulette_inst_inc               wxT("inc")
#define roulette_inst_include           wxT("include")
#define roulette_inst_input             wxT("input")
#define roulette_inst_label             wxT("label")
#define roulette_inst_leave             wxT("leave")
#define roulette_inst_pause             wxT("pause")
#define roulette_inst_plot              wxT("plot")
#define roulette_inst_popup             wxT("popup")
#define roulette_inst_progress          wxT("progress")
#define roulette_inst_random            wxT("random")
#define roulette_inst_randomize         wxT("randomize")
#define roulette_inst_refresh           wxT("refresh")
#define roulette_inst_restart           wxT("restart")
#define roulette_inst_rollback          wxT("rollback")
#define roulette_inst_save              wxT("save")
#define roulette_inst_set               wxT("set")
#define roulette_inst_show              wxT("show")
#define roulette_inst_slap              wxT("slap")
#define roulette_inst_spin              wxT("spin")
#define roulette_inst_stat              wxT("stat")
#define roulette_inst_status            wxT("status")
#define roulette_inst_stop              wxT("stop")
#define roulette_inst_time              wxT("time")
#define roulette_inst_unset             wxT("unset")
#define roulette_inst_write             wxT("write")

#define roulette_inst_allocate_id       4592
#define roulette_inst_append_id         4109
#define roulette_inst_assert_id         4238
#define roulette_inst_backup_id         4026
#define roulette_inst_beep_id           3069
#define roulette_inst_bet_id            2502
#define roulette_inst_bits_id           3224
#define roulette_inst_break_id          3618
#define roulette_inst_buy_id            2665
#define roulette_inst_cbet_id           3078
#define roulette_inst_check_id          3559
#define roulette_inst_clear_id          3614
#define roulette_inst_confirm_id        4469
#define roulette_inst_debug_id          3611
#define roulette_inst_dec_id            2401
#define roulette_inst_else_id           3184
#define roulette_inst_endif_id          3629
#define roulette_inst_exist_id          3874
#define roulette_inst_goto_id           3293
#define roulette_inst_if_id             1761
#define roulette_inst_inc_id            2518
#define roulette_inst_include_id        4454
#define roulette_inst_input_id          3891
#define roulette_inst_label_id          3580
#define roulette_inst_leave_id          3672
#define roulette_inst_pause_id          3798
#define roulette_inst_plot_id           3345
#define roulette_inst_popup_id          3942
#define roulette_inst_progress_id       4864
#define roulette_inst_random_id         4163
#define roulette_inst_randomize_id      4823
#define roulette_inst_refresh_id        4509
#define roulette_inst_restart_id        4624
#define roulette_inst_rollback_id       4707
#define roulette_inst_save_id           3243
#define roulette_inst_set_id            2655
#define roulette_inst_show_id           3358
#define roulette_inst_slap_id           3250
#define roulette_inst_spin_id           3326
#define roulette_inst_stat_id           3338
#define roulette_inst_status_id         4383
#define roulette_inst_stop_id           3412
#define roulette_inst_time_id           3253
#define roulette_inst_unset_id          3924
#define roulette_inst_write_id          3919

#define roulette_vars_bet               wxT("game.bet")
#define roulette_vars_betmax            wxT("game.bet.max")
#define roulette_vars_cash              wxT("game.cash")
#define roulette_vars_credit            wxT("game.credit")
#define roulette_vars_gain              wxT("game.gain")
#define roulette_vars_landed_black      wxT("game.landed.black")
#define roulette_vars_landed_column     wxT("game.landed.column")
#define roulette_vars_landed_dozen      wxT("game.landed.dozen")
#define roulette_vars_landed_even       wxT("game.landed.even")
#define roulette_vars_landed_green      wxT("game.landed.green")
#define roulette_vars_landed_half       wxT("game.landed.half")
#define roulette_vars_landed_number     wxT("game.landed.number")
#define roulette_vars_landed_odd        wxT("game.landed.odd")
#define roulette_vars_landed_red        wxT("game.landed.red")
#define roulette_vars_landed_street     wxT("game.landed.street")
#define roulette_vars_landed_win        wxT("game.landed.win")
#define roulette_vars_landed_zero       wxT("game.landed.zero")
#define roulette_vars_net               wxT("game.net")
#define roulette_vars_rounds            wxT("game.rounds")
#define roulette_vars_stats_black       wxT("stat.black")
#define roulette_vars_stats_green       wxT("stat.green")
#define roulette_vars_stats_loss_cnt    wxT("stat.loss.count")
#define roulette_vars_stats_net_max     wxT("stat.net.max")
#define roulette_vars_stats_net_min     wxT("stat.net.min")
#define roulette_vars_stats_net_avg     wxT("stat.net.average")
#define roulette_vars_stats_red         wxT("stat.red")
#define roulette_vars_stats_win_cnt     wxT("stat.win.count")
#define roulette_vars_stats_win_rate    wxT("stat.win.rate")
#define roulette_vars_console           wxT("sys.console")
#define roulette_vars_debug             wxT("sys.debug")
#define roulette_vars_zero_double       wxT("sys.double_zero")
#define roulette_vars_false             wxT("sys.false")
#define roulette_vars_can_bet           wxT("sys.game.can_bet")
#define roulette_vars_stake             wxT("sys.game.stake")
#define roulette_vars_numbers           wxT("sys.game.numbers")
#define roulette_vars_number_min        wxT("sys.game.number.min")
#define roulette_vars_number_max        wxT("sys.game.number.max")
#define roulette_vars_input             wxT("sys.input")
#define roulette_vars_input_length      wxT("sys.input.length")
#define roulette_vars_last_if           wxT("sys.last_if")
#define roulette_vars_plot              wxT("sys.plot")
#define roulette_vars_popup             wxT("sys.popup")
#define roulette_vars_popup_cancel      wxT("sys.popup.cancel")
#define roulette_vars_popup_no          wxT("sys.popup.no")
#define roulette_vars_popup_yes         wxT("sys.popup.yes")
#define roulette_vars_random            wxT("sys.random")
#define roulette_vars_result            wxT("sys.result")
#define roulette_vars_zero_triple       wxT("sys.triple_zero")
#define roulette_vars_true              wxT("sys.true")
#define roulette_vars_version           wxT("sys.version")
#define roulette_vars_zero              wxT("sys.zero")

#define roulette_layout_fr              { 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26 }
#define roulette_layout_us              { 28, 9, 26, 30, 11, 7, 20, 32, 17, 5, 22, 34, 15, 3, 24, 36, 13, 1, 27, 10, 25, 29, 12, 8, 19, 31, 18, 6, 21, 33, 16, 4, 23, 35, 14, 2 }

#define roulette_http_port              80
#define roulette_http_okcode            200
#define roulette_http_buffer_size       (size_t)16384
#define roulette_http_default_number    1
#define roulette_http_lot_size          1000    /* Don't set below 500 please to comply with the terms of service. */
#define roulette_http_delay             5

#define roulette_about_name             wxT("Scripted Roulette")
#define roulette_about_version          wxT("v0.2")
#define roulette_about_version_num      20
#define roulette_about_website          wxT("http://scripted-roulette.sourceforge.net")
#define roulette_about_copyright        wxT("Copyright (c) 2015-2016, ecrucru")
#define roulette_about_desc             wxT("Scripting tool to develop your own betting\nstrategy for the Roulette, a casino game.")


//------------------------------------------

#undef wxUSE_STC
#if roulette_compile_pick_stc == 1
    #ifndef _CONSOLE
        #define wxUSE_STC 1
    #endif
#endif

#if wxCHECK_VERSION(2,9,0)
    #define wxT28(v)    (v)
    #define wxT28_(v)   (v)     //No translation for the command-line (it generates crashes...)
#else
    #define wxT28(v)    wxT(v)
    #define wxT28_(v)   _(v)
#endif


//------------------------------------------

namespace wxRouletteType
{
    /**
     * \enum wxRouletteType
     * Defines the type of the roulette :
     *   - BALANCED : no zero
     *   - FRENCH   : 1 zero
     *   - AMERICAN : 2 zeros
     *   - MEXICAN  : 3 zeros
     *   .
     *
     * \remark The internal value <b>must</b> be the number of zeros.
     */
    enum wxRouletteType         {   BALANCED = 0,
                                    FRENCH,
                                    AMERICAN,
                                    MEXICAN
                                };
}

namespace wxRouletteAlgorithm
{
    /**
     * \enum wxRouletteAlgorithm::wxRouletteAlgorithm
     * Defines the random number generator.
     */
    enum wxRouletteAlgorithm    {   NONE = 0,
                                    RAND,
                                    MT,
                                    TT800,
                                    CLOCK,
                                    STDMIN,
                                    RS,
                                    RANDU,
                                    BITS,
                                    XOR,
                                    LOADED,
                                    LNGEN,
                                    EQUI,
                                #ifdef __WXDEBUG__
                                    HTTP_LOCAL,
                                #endif
                                    HTTP_RANDOM_ORG,
                                    CUSTOM
                                };
}

namespace wxRouletteSign
{
    /**
     * \enum wxRouletteSign
     * Defines the type of sign in a logical statement.
     */
    enum wxRouletteSign         { NONE = 0, EQ, NE, LT, LE, GT, GE };
}

namespace wxRouletteOperator
{
    /**
     * \enum wxRouletteOperator
     * Defines the type of operator in a logical statement.
     */
    enum wxRouletteOperator     { NONE = 0, AND, OR, XOR, NAND, NOR };
}

namespace wxRouletteMessageType
{
    /**
     * \enum wxRouletteMessageType
     * Defines the type of a message stored in the log.
     */
    enum wxRouletteMessageType  {   NONE        = 1 << 0,
                                    INFO_T      = 1 << 1,
                                    WARNING_T   = 1 << 2,
                                    ERROR_T     = 1 << 3,
                                    DEBUG_T     = 1 << 4,
                                    SYSTEM_T    = 1 << 5
                                }; //Without _T ==> Error C2059
}


//------------------------------------------

#include "helper.h"

#endif
