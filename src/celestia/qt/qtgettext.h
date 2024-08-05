/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QString>
#include <QTranslator>

#include <celutil/gettext.h>

namespace celestia::qt
{

class CelestiaQTranslator : public QTranslator
{
    inline QString translate(const char* /* context */,
                             const char* msgid,
                             const char* disambiguation = nullptr,
                             int n = -1) const override;
};

QString
CelestiaQTranslator::translate(const char*,
                               const char *msgid,
                               const char *disambiguation,
                               int) const
{
    return disambiguation != nullptr ? CX_(disambiguation, msgid) : _(msgid);
}

} // end namespace celestia::qt
