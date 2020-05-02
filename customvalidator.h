// Copyright 2020 José María Castelo Ares

// This file is part of SIRview.

// SIRview is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// SIRview is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with SIRview.  If not, see <https://www.gnu.org/licenses/>.

#ifndef CUSTOMVALIDATOR_H
#define CUSTOMVALIDATOR_H

#include <QDoubleValidator>

class CustomValidator : public QDoubleValidator
{
public:
    CustomValidator(double bottom, double top, int decimals, QObject * parent): QDoubleValidator(bottom, top, decimals, parent){}

    QValidator::State validate(QString &s, int &i) const
    {
        Q_UNUSED(i)

        if (s.isEmpty() || s == "-")
        {
            return QValidator::Intermediate;
        }

        QChar decimalPoint = locale().decimalPoint();

        if(s.indexOf(decimalPoint) != -1)
        {
            int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

            if (charsAfterPoint > decimals())
            {
                return QValidator::Invalid;
            }
        }

        bool ok;
        double d = locale().toDouble(s, &ok);

        if (ok && d >= bottom() && d <= top())
        {
            return QValidator::Acceptable;
        }
        else
        {
            return QValidator::Invalid;
        }
    }
};

#endif // CUSTOMVALIDATOR_H
