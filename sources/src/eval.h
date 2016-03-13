/*
Rodent, a UCI chess playing engine derived from Sungorus 1.4
Copyright (C) 2009-2011 Pablo Vazquez (Sungorus author)
Copyright (C) 2011-2016 Pawel Koziol

Rodent is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version.

Rodent is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Piece/square tables are taken from Toga Log User Manual 
// (http://members.aon.at/josefd/Toga%20LOG.html)
// and modified slightly.

#pragma once

static const int pstKnightOutpost[64] =
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   2,   3,   6,   6,   3,   2,   0,
  0,   2,   6,   9,   9,   6,   2,   0,
  0,   3,   9,  12,  12,   9,   3,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0
};

static const int pstBishopOutpost[64] =
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   1,   2,   4,   4,   2,   1,   0,
  0,   1,   4,   6,   6,   4,   1,   0,
  0,   2,   6,   8,   8,   6,   2,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0
};

static const int passed_bonus_mg[2][8] = {
  { 0, 12, 12, 30, 50, 80, 130, 0 },
  { 0, 120, 80, 50, 30, 12, 12, 0 }
};

const int passed_bonus_eg[2][8] = {
  { 0, 16, 16, 39, 65, 104, 156, 0 },
  { 0, 156, 104, 65, 39, 16, 16, 0 }
};

static const int phalanx_bonus[2][8] = {
  { 0, 0, 0, 2, 3, 4, 5, 0 },
  { 0, 5, 4, 3, 2, 0, 0, 0 }
};

struct sEvalHashEntry {
  U64 key;
  int score;
};

struct sPawnHashEntry {
  U64 key;
  int mg_pawns;
  int eg_pawns;
};

extern int mg[2][N_OF_FACTORS];
extern int eg[2][N_OF_FACTORS];

#define EVAL_HASH_SIZE 512*512
#define PAWN_HASH_SIZE 512*512
extern sEvalHashEntry EvalTT[EVAL_HASH_SIZE];
extern sPawnHashEntry PawnTT[PAWN_HASH_SIZE];

static const int file_bonus[8] = { -3,  -1,   1,   3,   3,   1,  -1,  -3 };

// mobility parameters [130]
// (could be decreased to 16)

static const int n_mob_mg[9] =  { -16, -12,  -8,  -4,  +0,  +4,  +8, +12, +16 };
static const int n_mob_eg[9] =  { -16, -12,  -8,  -4,  +0,  +4,  +8, +12, +16 };
static const int b_mob_mg[14] = { -35, -30, -25, -20, -15, -10,  -5,  +0,  +5, +10, +15, +20, +25, +30 };
static const int b_mob_eg[14] = { -35, -30, -25, -20, -15, -10,  -5,  +0,  +5, +10, +15, +20, +25, +30 };
static const int r_mob_mg[15] = { -14, -12, -10,  -8,  -6,  -4,  -2,  +0,  +2,  +4,  +6,  +8, +10, +12, +14 };
static const int r_mob_eg[15] = { -28, -24, -20, -16, -12,  -8,  -4,  +0,  +4,  +8, +12, +16, +20, +24, +28 };
static const int q_mob_mg[28] = { -14, -13, -12, -11, -10,  -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -1,  +0, 
                                   +1,  +2,  +3,  +4,  +5,  +6,  +7,  +8,  +9, +10, +11, +12, +13 };
static const int q_mob_eg[28] = { -28, -26, -24, -22, -20, -18, -16, -14, -12, -10,  -8,  -6,  -4,  -2,  +0,
                                   +2,  +4,  +6,  +8, +10, +12, +14, +16, +18, +20, +22, +24, +26 };
								   
// parameters for evaluating material imbalance [5+9+2]

#define A  80 // advantage in both major and minor pieces
#define Rk 50 // advantage in major pieces only
#define Nt 40 // advantage in minor pieces only
#define Ex 25 // exchange disadvantage [20..25..?]
#define Mm 60 // two minors for a rook 

static const int adj[9] = { -4, -3, -2, -1, 0,  1,  2,  3,  4 };

static const int imbalance[9][9] = {
    /* n=-4  n=-3  n=-2  n=-1  n=0   n=+1  n=+2  n=+3  n=+4 */
    {  -A,   -A,   -A,   -A,  -Rk,    0,    0,    0,    0 }, // R = -4
    {  -A,   -A,   -A,   -A,  -Rk,    0,    0,    0,    0 }, // R = -3
    {  -A,   -A,   -A,   -A,  -Rk,    0,    0,    0,    0 }, // R = -2
    {  -A,   -A,   -A,   -A,  -Rk,  -Ex,   Mm,    0,    0 }, // R = -1
    { -Nt,   -Nt, -Nt,  -Nt,    0,   Nt,   Nt,   Nt,   Nt }, // R =  0
    {   0,    0,  -Mm,   Ex,   Rk,    A,    A,    A,    A }, // R = +1
    {   0,    0,    0,    0,   Rk,    A,    A,    A,    A }, // R = +2
    {   0,    0,    0,    0,   Rk,    A,    A,    A,    A }, // R = +3
    {   0,    0,    0,    0,   Rk,    A,    A,    A,    A }  // R = +4
};

// parameters for initializing danger table [4]

static const int maxAttUnit = 399;
static const double maxAttStep = 8.0;
static const double maxAttScore = 1280;
static const double attCurveMult = 0.027;

// parameters for evaluating king safety [10]

//                                 P   N   B   R   Q
static const int king_att  [7] = { 0,  6,  6,  9, 15,  0,  0 };
static const int chk_threat[7] = { 0,  4,  3,  9, 12,  0,  0 };
static const int q_contact_check = 36; // [24..36..?]
static const int r_contact_check = 24; // [16..24..?]

// parameters for piece/king tropism [8], taken from GambitFruit

//                                 P   N   B   R   Q
static const int tropism_mg[7] = { 0,  3,  2,  2,  2,  0,  0 };
static const int tropism_eg[7] = { 0,  3,  1,  1,  4,  0,  0 };

extern U64 bbPawnTakes[2];

/**/
static const int pstPawnMg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
-15,  -5,   5,   5,   5,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,  10,  25,  25,  10,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
-15,  -5,   5,  15,  15,   5,  -5, -15,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
 };

static const int pstPawnEg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  3,   1,  -1,  -3,  -3,  -1,   1,   3,
  0,   0,   0,   0,   0,   0,   0,   0
//A8                                H8
 };

static const int pstKnightMg[64] = {
//A1                                H1
-50, -40, -30, -20, -20, -30, -40, -50,
-35, -25, -15,   0,   0, -15, -25, -35,
-20, -10,   0,  10,  10,   0, -10, -20,
-10,   0,  10,  20,  20,  10,   0, -10,
-5,    5,  15,  25,  25,  15,   5,  -5,
-5,    5,  15,  25,  25,  15,   5,  -5,
-20, -10,   0,  10,  10,   0, -10, -20,
-135,-25, -15,  -5,  -5, -15, -25, -135
//A8                                H8
};

static const int pstKnightEg[64] = {
//A1                                H1
-40, -30, -20, -10, -10, -20, -30, -40,
-30, -20, -10,   0,   0, -10, -20, -30,
-20, -10,   0,  10,  10,   0, -10, -20,
-10,   0,  10,  20,  20,  10,   0, -10,
-10,   0,  10,  20,  20,  10,   0, -10,
-20, -10,   0,  10,  10,   0, -10, -20,
-30, -20, -10,   0,   0, -10, -20, -30,
-40, -30, -20, -10, -10, -20, -30, -40
//A8                                H8
};

static const int pstBishopMg[64] = {
//A1                                H1
 -2,  -5, -16,  -5,  -5, -16,  -5,  -2,
 -5,   6,   3,   6,   6,   3,   6,  -5,
 -5,   5,   8,  10,  10,   8,   5,  -5,
 -5,   0,  10,  13,  13,  10,   0,  -5,
 -5,   0,   5,  13,  13,   5,   0,  -5,
 -5,   0,   5,   5,   5,   5,   0,  -5,
 -5,   0,   0,   0,   0,   0,   0,  -5,
 -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5
//A8                                H8
};

static const int pstBishopEg[64] = {
//A1                                H1
-15, -10,  -8,  -5,  -5,  -8, -10, -15,
-10,  -8,   0,   5,   5,   0,  -8, -10,
 -5,   0,   5,  10,  10,   5,   0,  -8,
 -5,   5,  10,  15,  15,  10,   5,  -5,
 -5,   5,  10,  15,  15,  10,   5,  -5,
 -8,   0,   5,  10,  10,   5,   0,  -8,
 10,  -8,   0,   5,   5,   0,  -8, -10,
-15, -10,  -8,  -5,  -5,  -8, -10, -15
//A8                                H8
};

static const int pstRookMg[64] = {
//A1                                H1
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 -3,  -1,   1,   3,   3,   1,  -1,  -3,
 //A8                                H8
 };

static const int pstRookEg[64] = {
//A1                                H1
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0
 //A8                                H8
 };

static const int pstQueenMg[64] = {
//A1                                H1
-5,   -5,  -5,  -5,  -5,  -5,  -5,  -5,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0,
 0,    0,   0,   0,   0,   0,   0,   0
//A8                                H8
};

static const int pstQueenEg[64] = {
//A1                                H1
-24, -16, -12,  -8,  -8, -12, -16, -24,
-16,  -8,  -4,   0,   0,  -4,  -8, -16,
-12,  -4,   0,   4,   4,   0,  -4, -12,
 -8,   0,   4,   8,   8,   4,   0,  -8,
 -8,   0,   4,   8,   8,   4,   0,  -8,
-12,  -4,   0,   4,   4,   0,  -4, -12,
-16,  -8,  -4,   0,   0,  -4,  -8, -16,
-24, -16, -12,  -8,  -8, -12, -16, -24
//A8                                H8
};

static const int pstKingMg[64] = {
//A1                                H1
 40,  50,  30,  10,  10,  30,  50,  40,
 30,  40,  20,   0,   0,  20,  40,  30,
 10,  20,   0, -20, -20,   0,  20,  10,
  0,  10, -10, -30, -30, -10,  10,   0,
-10,   0, -20, -40, -40, -20,   0, -10,
-20, -10, -30, -50, -50, -30, -10, -20,
-30, -20, -40, -60, -60, -40, -20, -30,
-40, -30, -50, -70, -70, -50, -30, -40
//A8                                H8
};

static const int pstKingEg[64] = {
//A1                                H1
-72, -48, -36, -24, -24, -36, -48, -72,
-48, -24, -12,   0,   0, -12, -24, -48,
-36, -12,   0,  12,  12,   0, -12, -36,
-24,   0,  12,  24,  24,  12,   0, -24,
-24,   0,  12,  24,  24,  12,   0, -24,
-36, -12,   0,  12,  12,   0, -12, -36,
-48, -24, -12,   0,   0, -12, -24, -48,
-72, -48, -36, -24, -24, -36, -48, -72
//A8                                H8
};
/**/

// alternative pst to test:
/**
static const int pstPawnMg[64]=
{
     0,   0,   0,   0,   0,   0,   0,   0,   
   -23, -11,  -5,   2,   2,  -5, -11, -23,
   -22, -10,  -4,  12,  12,  -4, -10, -22,
   -21,  -9,  12,  24,  24,  12,  -9, -21,
   -19,  -7,  -1,   6,   6,  -1,  -7, -19,
   -18,  -6,   0,   7,   7,   0,  -6, -18,
   -17,  -5,   1,   8,   8,   1,  -5, -17,
     0,   0,   0,   0,   0,   0,   0,   0
};

static const int pstPawnEg[64]  =
{
   0,   0,   0,   0,   0,   0,   0,   0,
   -2,  -4,  -6,  -8,  -8,  -6,  -4,  -2,
   -4,  -6,  -8, -10, -10,  -8,  -6,  -4,
   -5,  -7,  -9, -11, -11,  -9,  -7,  -5,
   -6,  -8, -10, -12, -12, -10,  -8,  -6,
   -7,  -9, -11, -13, -13, -11,  -9,  -7,
   -7,  -9, -11, -13, -13, -11,  -9,  -7,
   0,   0,   0,   0,   0,   0,   0,   0
};

static const int pstKnightMg[64] =
{
	-58, -42, -31, -27, -27, -31, -42, -58,
	-36, -20, -9, -5, -5, -9, -20, -36,
	-20, -4, 7, 11, 11, 7, -4, -20,
	-11, 5, 16, 20, 20, 16, 5, -11,
	-5, 11, 22, 26, 26, 22, 11, -5,
	-7, 9, 20, 24, 24, 20, 9, -7,
	-16, 10, 11, 15, 15, 11, 10, -16,
	-120, -21, -10, -6, -6, -10, -21, -120
};

static const int pstKnightEg[64] =
{
	-22, -17, -12, -9, -9, -12, -17, -22,
	-15, -8, -4, -2, -2, -4, -8, -15,
	-10, -4, 1, 3, 3, 1, -4, -10,
	-6, -1, 4, 8, 8, 4, -1, -6,
	-4, 1, 6, 10, 10, 6, 1, -4,
	-3, 3, 8, 10, 10, 8, 3, -3,
	-8, -1, 3, 5, 5, 3, -1, -8,
	-15, -10, -5, -2, -2, -5, -10, -15
};

static const int pstBishopMg[64] =
{
	-7, -8, -11, -13, -13, -11, -8, -7,
	-3, 3, 0, -2, -2, 0, 3, -3,
	-6, 0, 7, 6, 6, 7, 0, -6,
	-8, -2, 6, 15, 15, 6, -2, -8,
	-8, -2, 6, 15, 15, 6, -2, -8,
	-6, 0, 7, 6, 6, 7, 0, -6,
	-3, 3, 0, -2, -2, 0, 3, -3,
	-2, -3, -6, -8, -8, -6, -3, -2
};


static const int pstBishopEg[64] =
{
	0, -1, -2, -2, -2, -2, -1, 0,
	-1, 1, 0, 0, 0, 0, 1, -1,
	-2, 0, 3, 2, 2, 3, 0, -2,
	-2, 0, 2, 5, 5, 2, 0, -2,
	-2, 0, 2, 5, 5, 2, 0, -2,
	-2, 0, 3, 2, 2, 3, 0, -2,
	-1, 1, 0, 0, 0, 0, 1, -1,
	0, -1, -2, -2, -2, -2, -1, 0
};
static const int pstRookMg[64] =
{
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4,
	-4, 0, 4, 8, 8, 4, 0, -4
};

static const int pstRookEg[64] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	-2, -2, -2, -2, -2, -2, -2, -2
};

static const int pstQueenMg[64] =
{
  -16, -12, -9, -7, -7, -9, -12, -16,
   -7,  -1,  1, 3, 3, 1, -1, -7,
   -4,   1,  5, 6, 6, 5, 1, -4,
   -2,   3,  6, 9, 9, 6, 3, -2,
   -2,   3,  6, 9, 9, 6, 3, -2,
   -4,   1,  5, 6, 6, 5, 1, -4,
   -7,  -1,  1, 3, 3, 1, -1, -7,
   -11, -7, -4, -2, -2, -4, -7, -11
};

static const int pstQueenEg[64] =
{
  -15, -10,  -8,  -7,  -7,  -8, -10, -15,
  -10,  -5,  -3,  -2,  -2,  -3,  -5, -10,
   -8,  -3,   0,   2,   2,   0,  -3,  -8,
   -7,  -2,   2,   5,   5,   2,  -2,  -7,
   -7,  -2,   2,   5,   5,   2,  -2,  -7,
   -8,  -3,   0,   2,   2,   0,  -3,  -8,
  -10,  -5,  -3,  -2,  -2,  -3,  -5, -10,
  -15, -10,  -8,  -7,  -7,  -8, -10, -15,
};

static const int pstKingMg[64] = {
//A1                                H1
 40,  50,  30,  10,  10,  30,  50,  40,
 30,  40,  20,   0,   0,  20,  40,  30,
 10,  20,   0, -20, -20,   0,  20,  10,
  0,  10, -10, -30, -30, -10,  10,   0,
-10,   0, -20, -40, -40, -20,   0, -10,
-20, -10, -30, -50, -50, -30, -10, -20,
-30, -20, -40, -60, -60, -40, -20, -30,
-40, -30, -50, -70, -70, -50, -30, -40
//A8                                H8
};

static const int pstKingEg[64] =
{
  -73, -50, -34, -28, -28, -34, -50, -73,
  -40, -15,  -3,   3,   3,  -3, -15, -40,
  -29,  -8,   7,  13,  13,   7, -8,  -29,
  -23,  -2,  13,  22,  22,  13, -2,  -23,
  -18,   3,  18,  27,  27,  18,  3,  -18,
  -24,  -3,  12,  18,  18,  12, -3,  -24,
  -35, -10,   2,   8,   8,   2, -10, -35,
  -53, -30, -14,  -8,  -8, -14, -30, -53
};
/**/