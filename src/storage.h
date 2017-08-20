
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

#ifndef storage_h
#define storage_h

/**
 * \file storage.h
 * \brief Data storage.
 * \author ecrucru
 * \since Version 0.1, November 2015
 *
 * This class stores various data in an array.
 *
 * \remark For unknown reasons, this class doesn't like &lt;class T&gt;
 * if it contains a wxString.
 */


//------------------------------------------

template <class T>

/** \class wxRouletteStorage
 *  \brief Data storage.
 *
 * This class manages the storage of data of multiple types.
 * As it uses generic types, wxEcPlot only supports type wxRealPoint.
 *
 * \remark There is no use of wxList.
 */
class wxRouletteStorage
{
    private:
        T*              m_data;
        unsigned long   m_size;
        unsigned long   m_count;

    public:
        /** Constructor. */
        wxRouletteStorage(unsigned long pSize = 0)
        {
            m_data = NULL;
            m_size = 0;
            m_count = 0;
            AllocateData(pSize);
        }

        /** Destructor. */
        ~wxRouletteStorage()
        {
            FreeData();
        }

        /** Allocates data to store data. */
        bool AllocateData(unsigned long pSize)
        {
            T *ptr_tmp;

            //-- Checks
            if (pSize == m_size)
                return true;

            //-- A null size is equivalent to a free
            if (pSize == 0)
            {
                FreeData();
                return true;
            }

            //-- (Re)allocates data
            if (m_data == NULL)
            {
                m_data = (T*) malloc(pSize * sizeof(T));
                if (m_data != NULL)
                {
                    m_size = pSize;
                    return true;
                }
                else
                    return false;
            }
            else
            {
                ptr_tmp = (T*) realloc(m_data, pSize * sizeof(T));
                if (ptr_tmp == NULL)
                    return false;
                else
                {
                    m_data = ptr_tmp;
                    m_size = pSize;
                    return true;
                }
            }
        }

        /** Clears the data but not the allocated memory. */
        void EmptyData()
        {
            m_count = 0;
        }

        /** Deletes data. */
        void FreeData()
        {
            free(m_data);
            m_data = NULL;
            m_size = 0;
            m_count = 0;
        }

        /**
         * Gets the data source.
         * \remark You are not expected to manipulate the pointer. You rather
         *         have to use the other methods embedded in the class.
         */
        T* GetData()
        {
            return m_data;
        }

        /**
         * Returns an item of the table provided it is included in the range.
         * \return Pointer to an item, or \a NULL if not successful.
         */
        T* GetItem(unsigned long pPosition)
        {
            if (pPosition + 1 <= m_count)
                return &(m_data[pPosition]);
            else
                return NULL;
        }

        /**
         * Adds some data in the array.
         * \param pValue New object in the chosen format.
         * \return \a true on success, else \a false.
         */
        bool AddData(T pValue)
        {
            //-- Expands the memory space
            if (m_size == 0)
            {
                if (!AllocateData(100))     //100 entries by default
                    return false;
            }
            else
                if (m_count >= m_size)
                    if (!AllocateData(2*m_size))
                        return false;
            wxASSERT(m_data != NULL);

            //-- Add a new point
            m_data[m_count] = pValue;
            m_count++;
            return true;
        }

        /**
         * Removes the last item of the list.
         * \return \a true on success, else \a false.
         */
        bool DeleteLastEntry()
        {
            //-- Checks
            if (m_count == 0)
                return false;

            //-- Frees the item
            m_count--;
            return true;
        }

        /** Returns the number of entries in the table. */
        inline unsigned long GetCount()
        {
            return m_count;
        }

        /** Tells if the array contains data or not. */
        inline bool IsEmpty()
        {
            return (m_count == 0);
        }
};

#endif
