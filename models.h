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
    double P0;

    SIR(double p0): P0(p0){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = -P0 * x[0] * x[1];
        dxdt[1] = (P0 * x[0] - 1) * x[1];
        dxdt[2] = x[1];
    }
};

class SIRVitalDynamics
{
public:
    double P0, P1;

    SIRVitalDynamics(double p0, double p1): P0(p0), P1(p1){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = P1 * (1 - x[0]) - P0 * x[0] * x[1];
        dxdt[1] = (P0 * x[0] - 1 - P1) * x[1];
        dxdt[2] = x[1] - P1 * x[2];
    }
};

class SIRS
{
public:
    double P0, P1;

    SIRS(double p0, double p1): P0(p0), P1(p1){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = P1 * x[2] - P0 * x[0] * x[1];
        dxdt[1] = (P0 * x[0] - 1) * x[1];
        dxdt[2] = x[1] - P1 * x[2];
    }
};

class SIRSVitalDynamics
{
public:
    double P0, P1, P2;

    SIRSVitalDynamics(double p0, double p1, double p2): P0(p0), P1(p1), P2(p2){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = P1 * x[2] + P2 * (1 - x[0]) - P0 * x[0] * x[1];
        dxdt[1] = (P0 * x[0] - 1 - P2) * x[1];
        dxdt[2] = x[1] - (P1 + P2) * x[2];
    }
};

class SIRA
{
public:
    double P0, P1;

    SIRA(double p0, double p1): P0(p0), P1(p1){}

    void operator()(const state_type &x, state_type &dxdt, const double)
    {
        dxdt[0] = -P0 * (x[1] + P1 * x[3]) * x[0];
        dxdt[1] = (P0 * x[0] - 1) * x[1];
        dxdt[2] = x[1] + x[3];
        dxdt[3] = (P0 * P1 * x[0] - 1) * x[3];
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
