/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2008, 2013, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Eitan Marder-Eppstein
 *         David V. Lu!!
 *********************************************************************/
#include <global_planner/grid_path.h>
#include <algorithm>
#include <stdio.h>
namespace global_planner {

// [ TODO ] NOT considering about lethal_cost. Is it required here ?

bool GridPath::getPath(float* potential, 
                       double start_x, double start_y, 
                       double end_x,   double end_y, 
                       std::vector<std::pair<float, float> >& path) {
    std::pair<float, float> current;
    current.first = end_x;
    current.second = end_y;

    int start_index = getIndex(start_x, start_y);

    path.push_back(current);
    int c = 0;
    int ns = xs_ * ys_;
    
    float min_val = 1e10;
    int min_x = 0, min_y = 0;
    int x = 0, y = 0;
    int index = 0;
    //int xd, yd;
    int idir = 0;

    // 8 directions
    static const int NUM_DIR = 8;
    static const int dir_table[NUM_DIR][2] = {{ 1, 1},
                                              { 1, 0},
                                              { 1,-1},
                                              { 0,-1},
                                              {-1,-1},
                                              {-1, 0},
                                              {-1, 1},
                                              { 0, 1}};
    
    while (getIndex(current.first, current.second) != start_index) {
        min_val = 1e10;
        min_x = -1;
        min_y = -1;
        //for (xd = -1; xd <= 1; xd++) {
        //    for (yd = -1; yd <= 1; yd++) {
        for(idir = 0; idir < NUM_DIR ; idir++) {
            //if (xd == 0 && yd == 0)
            //        continue;
            x = current.first  + dir_table[idir][0];//xd;
            y = current.second + dir_table[idir][1];//yd;
            index = getIndex(x, y);
            if( x < 0 || x >= xs_ ||
                y < 0 || y >= ys_ ) {
              continue;
            }
            if (potential[index] < min_val) {
              min_val = potential[index];
              min_x = x;
              min_y = y;
            }
            //}
        }

        if (min_x == -1 && min_y == -1) {
            return false;
        }

        current.first  = min_x;
        current.second = min_y;
        path.push_back(current);
        
        if(c++>ns*4){
            return false;
        }

    }
    return true;
}

} //end namespace global_planner

