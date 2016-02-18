#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rodent.h"
#include "timer.h"
#include "book.h"

void ReadLine(char *str, int n) {
  char *ptr;

  if (fgets(str, n, stdin) == NULL)
    exit(0);
  if ((ptr = strchr(str, '\n')) != NULL)
    *ptr = '\0';
}

char *ParseToken(char *string, char *token) {

  while (*string == ' ')
    string++;
  while (*string != ' ' && *string != '\0')
    *token++ = *string++;
  *token = '\0';
  return string;
}

void UciLoop(void) {

  char command[4096], token[120], *ptr;
  POS p[1];

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  SetPosition(p, START_POS);
  AllocTrans(16);
  for (;;) {
    ReadLine(command, sizeof(command));
    ptr = ParseToken(command, token);

    if (strstr(command, "setoption name UseBook value"))
      use_book = (strstr(command, "value true") != 0);

    if (strcmp(token, "uci") == 0) {
      printf("id name ");
      printf(PROG_NAME);
      printf("\n");
      printf("id author Pawel Koziol (based on Sungorus 1.4 by Pablo Vazquez)\n");
      printf("option name Hash type spin default 16 min 1 max 4096\n");
      printf("option name Clear Hash type button\n");
      if (panel_style == 0) {
        printf("option name PawnValue type spin default %d min 0 max 1200\n", pc_value[P]);
        printf("option name KnightValue type spin default %d min 0 max 1200\n", pc_value[N]);
        printf("option name BishopValue type spin default %d min 0 max 1200\n", pc_value[B]);
        printf("option name RookValue type spin default %d min 0 max 1200\n", pc_value[R]);
        printf("option name QueenValue type spin default %d min 0 max 1200\n", pc_value[Q]);
        printf("option name Material type spin default %d min 0 max 500\n", mat_perc);
		printf("option name PiecePlacement type spin default %d min 0 max 500\n", pst_perc);
        printf("option name KnightLikesClosed type spin default %d min 0 max 10\n", np_bonus);
        printf("option name RookLikesOpen type spin default %d min 0 max 10\n", rp_malus);
        printf("option name OwnAttack type spin default %d min 0 max 500\n", dyn_weights[DF_OWN_ATT]);
        printf("option name OppAttack type spin default %d min 0 max 500\n", dyn_weights[DF_OPP_ATT]);
        printf("option name OwnMobility type spin default %d min 0 max 500\n", dyn_weights[DF_OWN_MOB]);
        printf("option name OppMobility type spin default %d min 0 max 500\n", dyn_weights[DF_OPP_MOB]);
        printf("option name KingTropism type spin default %d min 0 max 500\n", weights[F_TROPISM]);
        printf("option name PiecePressure type spin default %d min 0 max 500\n", weights[F_PRESSURE]);
        printf("option name PassedPawns type spin default %d min 0 max 500\n", weights[F_PASSERS]);
        printf("option name PawnStructure type spin default %d min 0 max 500\n", weights[F_PAWNS]);
        printf("option name Lines type spin default %d min 0 max 500\n", weights[F_LINES]);
        printf("option name Outposts type spin default %d min 0 max 500\n", weights[F_OUTPOST]);
        printf("option name NpsLimit type spin default %d min 0 max 5000000\n", Timer.nps_limit);
        printf("option name EvalBlur type spin default %d min 0 max 5000000\n", eval_blur);
        printf("option name Contempt type spin default %d min -250 max 250\n", draw_score);
        printf("option name UseBook type check default true\n");
        printf("option name GuideBookFile type string default guide.bin\n");
        printf("option name MainBookFile type string default rodent.bin\n");
        printf("option name BookFilter type spin default %d min 0 max 5000000\n", book_filter);
     }
	 if (panel_style == 1) {
        printf("option name PersonalityFile type string default rodent.txt\n");
		printf("option name UseBook type check default true\n");
		printf("option name GuideBookFile type string default guide.bin\n");
		printf("option name MainBookFile type string default rodent.bin\n");
	 }

      printf("uciok\n");
    } else if (strcmp(token, "isready") == 0) {
      printf("readyok\n");
    } else if (strcmp(token, "setoption") == 0) {
      ParseSetoption(ptr);
    } else if (strcmp(token, "position") == 0) {
      ParsePosition(p, ptr);
    } else if (strcmp(token, "perft") == 0) {
      ptr = ParseToken(ptr, token);
    int depth = atoi(token);
    if (depth == 0) depth = 5;
    Timer.SetStartTime();
    nodes = Perft(p, 0, depth);
    printf (" perft %d : %d nodes in %d miliseconds\n", depth, nodes, Timer.GetElapsedTime() );
    } else if (strcmp(token, "print") == 0) {
      PrintBoard(p);
    } else if (strcmp(token, "eval") == 0) {
      PrintEval(p);
    } else if (strcmp(token, "step") == 0) {
      ParseMoves(p, ptr);
    } else if (strcmp(token, "go") == 0) {
      ParseGo(p, ptr);
    } else if (strcmp(token, "bench") == 0) {
      ptr = ParseToken(ptr, token);
      Bench(atoi(token));
    } else if (strcmp(token, "quit") == 0) {
      exit(0);
    }
  }
}

void ParseSetoption(char *ptr) {

  char token[120], name[120], value[120] = "";

  ptr = ParseToken(ptr, token);
  name[0] = '\0';
  for (;;) {
    ptr = ParseToken(ptr, token);
    if (*token == '\0' || strcmp(token, "value") == 0)
      break;
    strcat(name, token);
    strcat(name, " ");
  }
  name[strlen(name) - 1] = '\0';
  if (strcmp(token, "value") == 0) {
    value[0] = '\0';

    for (;;) {
      ptr = ParseToken(ptr, token);
      if (*token == '\0')
        break;
      strcat(value, token);
      strcat(value, " ");
    }
    value[strlen(value) - 1] = '\0';
  }

  if (strcmp(name, "Hash") == 0) {
    AllocTrans(atoi(value));
  } else if (strcmp(name, "Clear Hash") == 0) {
    ResetEngine();
  } else if (strcmp(name, "Material") == 0) {
    mat_perc = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "PiecePlacement") == 0) {
    pst_perc = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "PawnValue") == 0) {
    pc_value[P] = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "KnightValue") == 0) {
    pc_value[N] = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "BishopValue") == 0) {
    pc_value[B] = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "RookValue") == 0) {
    pc_value[R] = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "QueenValue") == 0) {
    pc_value[Q] = atoi(value);
    ResetEngine();
    InitEval();
  } else if (strcmp(name, "KnightLikedClosed") == 0) {
    np_bonus = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "RookLikesOpen") == 0) {
    rp_malus = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "OwnAttack") == 0) {
    dyn_weights[DF_OWN_ATT] = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "OppAttack") == 0) {
    dyn_weights[DF_OPP_ATT] = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "OwnMobility") == 0) {
    dyn_weights[DF_OWN_MOB] = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "OppMobility") == 0) {
    dyn_weights[DF_OPP_MOB] = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "KingTropism") == 0) {
    SetWeight(F_TROPISM, atoi(value));
  } else if (strcmp(name, "PiecePressure") == 0) {
    SetWeight(F_PRESSURE, atoi(value));
  } else if (strcmp(name, "PassedPawns") == 0) {
    SetWeight(F_PASSERS, atoi(value));
  } else if (strcmp(name, "PawnStructure") == 0) {
    SetWeight(F_PAWNS, atoi(value));
  } else if (strcmp(name, "Lines") == 0) {
   SetWeight(F_LINES, atoi(value));
  } else if (strcmp(name, "Outposts") == 0) {
    SetWeight(F_OUTPOST, atoi(value));
  } else if (strcmp(name, "NpsLimit") == 0) {
    Timer.nps_limit = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "EvalBlur") == 0) {
    eval_blur = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "Contempt") == 0) {
    draw_score = atoi(value);
    ResetEngine();
  } else if (strcmp(name, "GuideBookFile") == 0) {
    GuideBook.ClosePolyglot();
    GuideBook.bookName = value;
    GuideBook.OpenPolyglot();
  } else if (strcmp(name, "MainBookFile") == 0) {
    MainBook.ClosePolyglot();
    MainBook.bookName = value;
    MainBook.OpenPolyglot();
  } else if (strcmp(name, "PersonalityFile") == 0) {
    printf("info string reading ");
    printf(value);
    printf("\n");
    ReadPersonality(value);
  } else if (strcmp(name, "BookFilter") == 0) {
    book_filter = atoi(value);;
  }
}

void SetWeight(int weight_name, int value) {

  weights[weight_name] = value;
  ResetEngine();
}

void ParseMoves(POS *p, char *ptr) {
  
  char token[120];
  UNDO u[1];

  for (;;) {

    // Get next move to parse

    ptr = ParseToken(ptr, token);

  // No more moves!

    if (*token == '\0') break;

    p->DoMove(StrToMove(p, token), u);

  // We won't be taking back moves beyond this point:

    if (p->rev_moves == 0) p->head = 0;
  }
}

void ParsePosition(POS *p, char *ptr) {

  char token[120], fen[120];

  ptr = ParseToken(ptr, token);
  if (strcmp(token, "fen") == 0) {
    fen[0] = '\0';
    for (;;) {
      ptr = ParseToken(ptr, token);

      if (*token == '\0' || strcmp(token, "moves") == 0)
        break;

      strcat(fen, token);
      strcat(fen, " ");
    }
    SetPosition(p, fen);
  } else {
    ptr = ParseToken(ptr, token);
    SetPosition(p, START_POS);
  }

  if (strcmp(token, "moves") == 0)
    ParseMoves(p, ptr);
}

void ParseGo(POS *p, char *ptr) {

  char token[120], bestmove_str[6], ponder_str[6];
  int pv[MAX_PLY];

  Timer.Clear();
  pondering = 0;

  for (;;) {
    ptr = ParseToken(ptr, token);
    if (*token == '\0')
      break;
    if (strcmp(token, "ponder") == 0) {
      pondering = 1;
    } else if (strcmp(token, "wtime") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(W_TIME, atoi(token));
    } else if (strcmp(token, "btime") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(B_TIME, atoi(token));
    } else if (strcmp(token, "winc") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(W_INC, atoi(token));
    } else if (strcmp(token, "binc") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(B_INC, atoi(token));
    } else if (strcmp(token, "movestogo") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(MOVES_TO_GO, atoi(token));
    } else if (strcmp(token, "depth") == 0) {
      ptr = ParseToken(ptr, token);
      Timer.SetData(FLAG_INFINITE, 1);
      Timer.SetData(MAX_DEPTH, atoi(token));
    } else if (strcmp(token, "infinite") == 0) {
      Timer.SetData(FLAG_INFINITE, 1);
    }
  }

  Timer.SetSideData(p->side);
  Timer.SetMoveTiming();
  Think(p, pv);
  MoveToStr(pv[0], bestmove_str);
  if (pv[1]) {
    MoveToStr(pv[1], ponder_str);
    printf("bestmove %s ponder %s\n", bestmove_str, ponder_str);
  } else
    printf("bestmove %s\n", bestmove_str);
}

void ResetEngine(void) {

  ClearHist();
  ClearTrans();
  ClearEvalHash();
  ClearPawnHash();
}

void ReadPersonality(char *fileName)
{
  FILE *personalityFile;
  char line[256];
  int lineNo = 0;
  char token[120], *ptr;

  // exit if this personality file doesn't exist
  if ((personalityFile = fopen(fileName, "r")) == NULL)
    return;

  // read options line by line
  while (fgets(line, 256, personalityFile)) {
    ptr = ParseToken(line, token);

    if (strstr(line, "HIDE_OPTIONS")) panel_style = 1;
    if (strstr(line, "SHOW_OPTIONS")) panel_style = 0;

    if (strcmp(token, "setoption") == 0)
      ParseSetoption(ptr);
  }

  fclose(personalityFile);
}

int Perft(POS *p, int ply, int depth) {

	int move = 0;
	int mv_type;
	MOVES m[1];
	UNDO u[1];
	int mv_cnt = 0;

	InitMoves(p, m, 0, 0, ply);

	while (move = NextMove(m, &mv_type)) {

		p->DoMove(move, u);

		if (Illegal(p)) { p->UndoMove(move, u); continue; }

		if (depth == 1) mv_cnt++;
		else            mv_cnt += Perft(p, ply + 1, depth - 1);
		p->UndoMove(move, u);
	}

	return mv_cnt;
}

void PrintBoard(POS *p) {

	char *piece_name[] = { "P ", "p ", "N ", "n ", "B ", "b ", "R ", "r ", "Q ", "q ", "K ", "k ", ". " };

	printf("--------------------------------------------\n");
	for (int sq = 0; sq < 64; sq++) {
		printf(piece_name[p->pc[sq ^ (BC * 56)]]);
		if ((sq + 1) % 8 == 0) printf(" %d\n", 9 - ((sq + 1) / 8));
	}

	printf("\na b c d e f g h\n\n--------------------------------------------\n");
}

void Bench(int depth) {

	POS p[1];
	int pv[MAX_PLY];
	char *test[] = {
		"r1bqkbnr/pp1ppppp/2n5/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq -",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -",
		"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -",
		"4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b - - 7 19",
		"rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w - - 7 14",
		"r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w - - 2 14",
		"r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b - - 2 15",
		"r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w kq - 0 13",
		"r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w - - 1 16",
		"4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w - - 1 17",
		"2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b KQ - 0 11",
		"r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w - - 1 16",
		"3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b - - 6 22",
		"r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w - - 2 18",
		"4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b - - 3 22",
		"3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b - - 4 26",
		NULL
	}; // test positions taken from DiscoCheck by Lucas Braesch

	if (depth == 0) depth = 8; // so that you can call bench without parameters

	printf("Bench test started (depth %d): \n", depth);

	ResetEngine();
	nodes = 0;
	Timer.SetData(MAX_DEPTH, depth);
	Timer.SetData(FLAG_INFINITE, 1);
	Timer.SetStartTime();

	for (int i = 0; test[i]; ++i) {
		printf(test[i]);
		SetPosition(p, test[i]);
		printf("\n");
		Iterate(p, pv);
	}

	int end_time = Timer.GetElapsedTime();
	int nps = (nodes * 1000) / (end_time + 1);

	printf("%llu nodes searched in %d, speed %u nps (Score: %.3f)\n", nodes, end_time, nps, (float)nps / 430914.0);
}