/*
 * trsolver.h - transient solver class definitions
 *
 * Copyright (C) 2004 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: trsolver.h,v 1.10 2004-10-07 19:49:35 ela Exp $
 *
 */

#ifndef __TRSOLVER_H__
#define __TRSOLVER_H__

#include "nasolver.h"
#include "states.h"

class sweep;

class trsolver : public nasolver<nr_double_t>, public states<nr_double_t>
{
 public:
  trsolver ();
  trsolver (char *);
  trsolver (trsolver &);
  ~trsolver ();
  void solve (void);
  int  predictor (void);
  int  corrector (void);
  void nextStates (void);
  void fillStates (void);
  void adjustDelta (void);
  void adjustOrder (int&, int);
  void initTR (void);
  static void calcTR (trsolver *);
  void initDC (void);
  static void calcDC (trsolver *);
  void initSteps (void);
  void saveAllResults (nr_double_t);
  nr_double_t checkDelta (void);
  void updateCoefficients (nr_double_t, int next = 0);
  void predictBashford (void);
  void predictEuler (void);

 private:
  sweep * swp;
  nr_double_t predCoeff[8];
  nr_double_t corrCoeff[8];
  nr_double_t deltas[8];
  nr_double_t delta;
  nr_double_t deltaMax;
  nr_double_t deltaMin;
  nr_double_t deltaOld;
  int CMethod;
  int PMethod;
  int corrOrder;
  int predOrder;
  int chargeCoeffs;
  int rejected;
  tmatrix<nr_double_t> * solution[8];
  nr_double_t current;
};

#endif /* __TRSOLVER_H__ */
