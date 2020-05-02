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

#ifndef MODELS_H
#define MODELS_H

#include <vector>

typedef std::vector<double> state_type;

class SIR
{
public:
    double R0;

    SIR(double r0): R0(r0){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = -R0 * x[0] * x[1];
        dxdt[1] = (R0 * x[0] - 1) * x[1];
        dxdt[2] = x[1];
    }
};

class SIRS
{
public:
    double R0, R1;

    SIRS(double r0, double r1): R0(r0), R1(r1){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = -R0 * x[0] * x[1] + R1 * x[2];
        dxdt[1] = (R0 * x[0] - 1) * x[1];
        dxdt[2] = x[1] - R1 * x[2];
    }
};

struct push_back_state_and_time
{
    std::vector<state_type> &states;
    std::vector<double> &times;

    push_back_state_and_time(std::vector<state_type> &xs, std::vector<double> &ts): states(xs), times(ts){}

    void operator()(const state_type &x, double t)
    {
        states.push_back(x);
        times.push_back(t);
    }
};

#endif // MODELS_H
