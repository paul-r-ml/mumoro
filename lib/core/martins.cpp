/*    This file is part of Mumoro.

    Mumoro is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mumoro is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mumoro.  If not, see <http://www.gnu.org/licenses/>.

    © Université de Toulouse 1 2010
    Author: Tristram Gräbener*/

#include "martins_impl.h"
#include "martins.h"

float Edge::* get_objective(Objective o)
{
    switch(o)
    {
        case dist: return &Edge::distance; break;
        case elevation: return &Edge::elevation; break;
        case mode_change: return &Edge::mode_change; break;
        case cost: return &Edge::cost; break;
        case line_change: return &Edge::line_change; break;
        case co2: return &Edge::co2; break;
        default: std::cerr << "Unknown objective..." << std::endl; break;
    }
}

vector<Path> martins(int start_node, int dest_node, Graph & g, int start_time, int day)
{
    vector<float Edge::*> objectives;
    return martins<1>(start_node, dest_node, g, start_time, day, objectives, Dominates<1>());
}

vector<Path> martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    return martins<2>(start_node, dest_node, g, start_time, day, objectives, Dominates<2>());
}

vector<Path> martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1, Objective o2)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    objectives.push_back(get_objective(o2));
    return martins<3>(start_node, dest_node, g, start_time, day, objectives, Dominates<3>());
}


vector<Path> martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1, Objective o2, Objective o3)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    objectives.push_back(get_objective(o2));
    objectives.push_back(get_objective(o3));
    return martins<4>(start_node, dest_node, g, start_time, day, objectives, Dominates<4>());
}

std::vector<Path> relaxed_martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1, float r1)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    return martins<2>(start_node, dest_node, g, start_time, day, objectives, Relaxed_dominates<2>(r1));
}

std::vector<Path> relaxed_martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1, float r1, Objective o2, float r2)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    objectives.push_back(get_objective(o2));
    return martins<3>(start_node, dest_node, g, start_time, day, objectives, Relaxed_dominates<3>(r1, r2));
}

std::vector<Path> relaxed_martins(int start_node, int dest_node, Graph & g, int start_time, int day, Objective o1, float r1, Objective o2, float r2, Objective o3, float r3)
{
    vector<float Edge::*> objectives;
    objectives.push_back(get_objective(o1));
    objectives.push_back(get_objective(o2));
    objectives.push_back(get_objective(o3));
    return martins<4>(start_node, dest_node, g, start_time, day, objectives, Relaxed_dominates<4>(r1, r2, r3));
}

float delta(float a, float b)
{
    return a < b ? b - a : b - a;
}
