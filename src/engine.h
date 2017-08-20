
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

#ifndef engine_h
#define engine_h

/**
 * \file engine.h
 * \brief Enhanced mathematical engine
 * \author ecrucru
 * \since Version 0.2, February 2016
 *
 * This class enhances the syntax of the mathematical engine wxEcMath
 * to support the arrays.
 */

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxEcMath/ec_defs.h"
#include "wxEcMath/ec_engine.h"
#include "defs.h"
#include "helper.h"


//------------------------------------------

/** \class wxRouletteEngine
 *  \brief Enhanced mathematical engine.
 *
 * The class enhances the syntax of the mathematical engine wxEcMath
 * to support the arrays.
 *
 * \remark Designed for wxEcMath version 0.6.5. Earlier versions will not work.
 */
class wxRouletteEngine : public wxEcEngine
{
    private:
        wxString    m_basic_formula;    /**< Formula used when the calculation is started. */

    public:
        //-- Constructor
        /** Constructor */
        wxRouletteEngine();

        //-- Overridden and new methods
        /**
         * Defines the expression to evaluate with Compute().
         * \remark It is not possible here to do an enhanced check with the normal function because the
         *         square brackets will be replaced by parentheses. The real check will be done when the
         *         method Compute() is called.
         */
        bool SetFormula(wxString pExpression); //Overridden
        /**
         * Evaluates an expression to remove the dimensions of the arrays. The output is compatible
         * with the standard syntax supported by wxEcMath.
         * \param pExpression Pointer to a string receiving the result.
         * \return \a true if no problem occurred, else \a false.
         */
        bool GetCompactExpression(wxString *pExpression); //New
        /**
         * Evaluates the formula defined by the method SetFormula().
         * The errors are fetched with GetLastError().
         * \return The result of the calculation.
         */
        double Compute(); //Overridden
        /**
         * Removes a variable based on a mask.
         * \param pMask Mask of the variable to delete.
         * \return \a true if successful, else \a false.
         */
        bool DeleteConstantMask(wxString& pMask); //New
};

#endif
