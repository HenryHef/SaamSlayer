/*
 * search.h
 *
 *  Created on: Feb 25, 2018
 *      Author: henry
 */

#ifndef SRC_SEARCH_BASE_H_
#define SRC_SEARCH_BASE_H_

#include "eval.h"

struct Move;

struct Board;

void searchProgressive(Board &b, int min_depth, double min_time, Move& re);//using negamax
Value searchRootNode(Board &b, int depth, Move &m, int &nodect);//using negamax
Value searchN(Board &b, Value alpha, Value beta, int depth, int ply, bool isPV, bool can_null, int &nodect);//using negamax


#endif /* SRC_SEARCH_BASE_H_ */
