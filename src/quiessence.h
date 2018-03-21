/*
 * quiessence.h
 *
 *  Created on: Feb 26, 2018
 *      Author: henry
 */

#ifndef SRC_QUIESSENCE_H_
#define SRC_QUIESSENCE_H_

#include "eval.h"

void printQCT();
Value quiessence(Board &b, Value alpha, Value beta, int ply, int &nodect);

#endif /* SRC_QUIESSENCE_H_ */
