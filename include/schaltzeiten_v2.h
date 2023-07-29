/*
  schaltzeiten_v2.h  vom 07.10.22
  für Sonnenauf- / -untergang
*/

typedef struct {
  int stundeMorgen; int minuteMorgen; int stundeAbend; int minuteAbend;
} SchaltzeitenAstro;

SchaltzeitenAstro schaltzeitTag = { 1, 2, 3, 4 };
SchaltzeitenAstro schaltzeitTagRolladeZentral = { 0, 0, 0, 0 };
SchaltzeitenAstro schaltzeitTagAussenleuchte = { 0, 0, 0, 0 };
SchaltzeitenAstro schaltzeitOffsetRolladeZentral = { 0, 0, 0, 0 };
SchaltzeitenAstro schaltzeitOffsetAussenleuchte = { 0, 0, 0, 0 };
SchaltzeitenAstro schaltzeitOffsetArray[] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

SchaltzeitenAstro MorgenAbend[365] = {
  // Januar
  {8, 36, 16, 29 },
  {8, 35, 16, 30 },
  {8, 35, 16, 32 },
  {8, 35, 16, 33 },
  {8, 35, 16, 34 },
  {8, 34, 16, 35 },
  {8, 34, 16, 36 },
  {8, 34, 16, 38 },
  {8, 33, 16, 39 },
  {8, 32, 16, 41 },
  {8, 32, 16, 42 },
  {8, 31,	16, 43	},
  {8, 30,	16, 45	},
  {8, 30,	16, 46	},
  {8, 29,	16, 48	},
  {8, 28,	16, 50	},
  {8, 27,	16, 51	},
  {8, 26,	16, 53	},
  {8, 25,	16, 54	},
  {8, 24,	16, 56	},
  {8, 23,	16, 58	},
  {8, 22,	16, 59	},
  {8, 21,	17, 1	},
  {8, 19,	17, 3	},
  {8, 18,	17, 5	},
  {8, 17,	17, 6	},
  {8, 16,	17, 8	},
  {8, 14,	17, 10	},
  {8, 13,	17, 12	},
  {8, 11,	17, 14	},
  {8, 10,	17, 15	},

  // Februar

  {8, 8 , 17, 17  },
  {8, 7	, 17, 19  },
  {8, 5	, 17, 21  },
  {8, 4	, 17, 23  },
  {8, 2	, 17, 24  },
  {8, 0	, 17, 26  },
  {7, 58	, 17, 28  },
  {7, 57	, 17, 30  },
  {7, 55	, 17, 32  },
  {7, 53	, 17, 34  },
  {7, 51	, 17, 35  },
  {7, 50	, 17, 37  },
  {7, 48	, 17, 39  },
  {7, 46	, 17, 41  },
  {7, 44	, 17, 43  },
  {7, 42	, 17, 45  },
  {7, 40	, 17, 46  },
  {7, 38	, 17, 48  },
  {7, 36	, 17, 50  },
  {7, 34	, 17, 52  },
  {7, 32	, 17, 54  },
  {7, 30	, 17, 55  },
  {7, 28	, 17, 57  },
  {7, 26	, 17, 59  },
  {7, 24	, 18, 1  },
  {7, 22	, 18, 3  },
  {7, 20	, 18, 4  },
  {7, 17	, 18, 6  },



  // März
  {7, 15,	18, 8 },
  {7, 13,	18, 10 },
  {7, 11,	18, 11 },
  {7, 9,	18, 13 },
  {7, 7,	18, 15 },
  {7, 4,	18, 17 },
  {7, 2,	18, 18 },
  {7, 0,	18, 20 },
  {6, 58,	18, 22 },
  {6, 55,	18, 24 },
  {6, 53,	18, 25 },
  {6, 51,	18, 27 },
  {6, 49,	18, 29 },
  {6, 46,	18, 31 },
  {6, 44,	18, 32 },
  {6, 42,	18, 34 },
  {6, 40,	18, 36 },
  {6, 37,	18, 37 },
  {6, 35,	18, 39 },
  {6, 33,	18, 41 },
  {6, 31,	18, 42 },
  {6, 28,	18, 44 },
  {6, 26,	18, 46 },
  {6, 24,	18, 48 },
  {6, 21,	18, 49 },
  {6, 19,	18, 51 },
  {7, 17,	19, 53 },
  {7, 15,	19, 54 },
  {7, 12,	19, 56 },
  {7, 10,	19, 58 },
  {7, 8,	19, 59 },


  //	April
  {7, 5,	20, 1 },
  {7, 3,	20, 3 },
  {7, 1,	20, 4 },
  {6, 59,	20, 6 },
  {6, 56,	20, 8 },
  {6, 54,	20, 9 },
  {6, 52,	20, 11 },
  {6, 50,	20, 13 },
  {6, 48,	20, 14 },
  {6, 45,	20, 16 },
  {6, 43,	20, 18 },
  {6, 41,	20, 19 },
  {6, 39,	20, 21 },
  {6, 37,	20, 23 },
  {6, 34,	20, 24 },
  {6, 32,	20, 26 },
  {6, 30,	20, 28 },
  {6, 28,	20, 29 },
  {6, 26,	20, 31 },
  {6, 24,	20, 33 },
  {6, 22,	20, 35 },
  {6, 20,	20, 36 },
  {6, 18,	20, 38 },
  {6, 16,	20, 40 },
  {6, 14,	20, 41 },
  {6, 12,	20, 43 },
  {6, 10,	20, 44 },
  {6, 8,	20, 46 },
  {6, 6,	20, 48 },
  {6, 4,	20, 49 },


  // Mai
  {6, 2,	20, 51 },
  {6, 0,	20, 53 },
  {5, 58,	20, 54 },
  {5, 56,	20, 56 },
  {5, 55,	20, 58 },
  {5, 53,	20, 59 },
  {5, 51,	21, 1 },
  {5, 49,	21, 2 },
  {5, 48,	21, 4 },
  {5, 46,	21, 6 },
  {5, 44,	21, 7 },
  {5, 43,	21, 9 },
  {5, 41,	21, 10 },
  {5, 40,	21, 12 },
  {5, 38,	21, 13 },
  {5, 37,	21, 15 },
  {5, 35,	21, 16 },
  {5, 34,	21, 18 },
  {5, 32,	21, 19 },
  {5, 31,	21, 21 },
  {5, 30,	21, 22 },
  {5, 29,	21, 23 },
  {5, 27,	21, 25 },
  {5, 26,	21, 26 },
  {5, 25,	21, 27 },
  {5, 24,	21, 29 },
  {5, 23,	21, 30 },
  {5, 22,	21, 31 },
  {5, 21,	21, 32 },
  {5, 20,	21, 33 },
  {5, 19,	21, 35 },

  //  Juni
  {5, 18,	21, 36 },
  {5, 18,	21, 37 },
  {5, 17,	21, 38 },
  {5, 16,	21, 39 },
  {5, 15,	21, 40 },
  {5, 15,	21, 41 },
  {5, 14,	21, 42 },
  {5, 14,	21, 42 },
  {5, 13,	21, 43 },
  {5, 13,	21, 44 },
  {5, 13,	21, 45 },
  {5, 12,	21, 45 },
  {5, 12,	21, 46 },
  {5, 12,	21, 46 },
  {5, 12,	21, 47 },
  {5, 12,	21, 47 },
  {5, 12,	21, 48 },
  {5, 12,	21, 48 },
  {5, 12,	21, 49 },
  {5, 12,	21, 49 },
  {5, 12,	21, 49 },
  {5, 12,	21, 49 },
  {5, 13,	21, 49 },
  {5, 13,	21, 49 },
  {5, 13,	21, 49 },
  {5, 14,	21, 49 },
  {5, 14,	21, 49 },
  {5, 15,	21, 49 },
  {5, 15,	21, 49 },
  {5, 16,	21, 49 },


  //  Juli
  {5, 17,	21, 48 },
  {5, 17,	21, 48 },
  {5, 18,	21, 48 },
  {5, 19,	21, 47 },
  {5, 20,	21, 47 },
  {5, 21,	21, 46 },
  {5, 22,	21, 46 },
  {5, 22,	21, 45 },
  {5, 23,	21, 44 },
  {5, 24,	21, 43 },
  {5, 26,	21, 43 },
  {5, 27,	21, 42 },
  {5, 28,	21, 41 },
  {5, 29,	21, 40 },
  {5, 30,	21, 39 },
  {5, 31,	21, 38 },
  {5, 32,	21, 37 },
  {5, 34,	21, 36 },
  {5, 35,	21, 35 },
  {5, 36,	21, 33 },
  {5, 38,	21, 32 },
  {5, 39,	21, 31 },
  {5, 40,	21, 30 },
  {5, 42,	21, 28 },
  {5, 43,	21, 27 },
  {5, 44,	21, 25 },
  {5, 46,	21, 24 },
  {5, 47,	21, 22 },
  {5, 49,	21, 21 },
  {5, 50,	21, 19 },
  {5, 52,	21, 18 },

  //  August

  {5, 53,	21, 16},
  {5, 55,	21, 14},
  {5, 56,	21, 13},
  {5, 58,	21, 11},
  {5, 59,	21, 9},
  {6, 1,	21, 8},
  {6, 2,	21, 6},
  {6, 4,	21, 4},
  {6, 6,	21, 2},
  {6, 7,	21, 0},
  {6, 9,	20, 58},
  {6, 10,	20, 56},
  {6, 12,	20, 54},
  {6, 13,	20, 53},
  {6, 15,	20, 51},
  {6, 17,	20, 49},
  {6, 18,	20, 47},
  {6, 20,	20, 44},
  {6, 21,	20, 42},
  {6, 23,	20, 40},
  {6, 25,	20, 38},
  {6, 26,	20, 36},
  {6, 28,	20, 34},
  {6, 29,	20, 32},
  {6, 31,	20, 30},
  {6, 33,	20, 28},
  {6, 34,	20, 25},
  {6, 36,	20, 23},
  {6, 37,	20, 21},
  {6, 39,	20, 19},
  {6, 41,	20, 17},

  //  September
  {6, 42,	20, 14},
  {6, 44,	20, 12},
  {6, 45,	20, 10},
  {6, 47,	20, 8},
  {6, 49,	20, 5},
  {6, 50,	20, 3},
  {6, 52,	20, 1},
  {6, 53,	19, 59},
  {6, 55,	19, 56},
  {6, 57,	19, 54},
  {6, 58,	19, 52},
  {7, 0,	19, 50},
  {7, 1,	19, 47},
  {7, 3,	19, 45},
  {7, 4,	19, 43},
  {7, 6,	19, 40},
  {7, 8,	19, 38},
  {7, 9,	19, 36},
  {7, 11,	19, 33},
  {7, 12,	19, 31},
  {7, 14,	19, 29},
  {7, 16,	19, 26},
  {7, 17,	19, 24},
  {7, 19,	19, 22},
  {7, 21,	19, 20},
  {7, 22,	19, 17},
  {7, 24,	19, 15},
  {7, 25,	19, 13},
  {7, 27,	19, 10},
  {7, 29,	19, 8},

  //  Oktober
  {7, 30,	19, 6},
  {7, 32,	19, 4},
  {7, 34,	19, 1},
  {7, 35,	18, 59},
  {7, 37,	18, 57},
  {7, 38,	18, 55},
  {7, 40,	18, 52},
  {7, 42,	18, 50},
  {7, 43,	18, 48},
  {7, 45,	18, 46},
  {7, 47,	18, 43},
  {7, 49,	18, 41},
  {7, 50,	18, 39},
  {7, 52,	18, 37},
  {7, 54,	18, 35},
  {7, 55,	18, 33},
  {7, 57,	18, 31},
  {7, 59,	18, 28},
  {8, 0,	18, 26},
  {8, 2,	18, 24},
  {8, 4,	18, 22},
  {8, 6,	18, 20},
  {8, 7,	18, 18},
  {8, 9,	18, 16},
  {8, 11,	18, 14},
  {8, 13,	18, 12},
  {8, 14,	18, 10},
  {8, 16,	18, 8},
  {8, 18,	18, 6},
  {7, 20,	17, 5},
  {7, 21,	17, 3},

  //  November

  {7, 23,	17, 1},
  {7, 25,	16, 59},
  {7, 27,	16, 57},
  {7, 29,	16, 56},
  {7, 30,	16, 54},
  {7, 32,	16, 52},
  {7, 34,	16, 51},
  {7, 36,	16, 49},
  {7, 37,	16, 47},
  {7, 39,	16, 46},
  {7, 41,	16, 44},
  {7, 43,	16, 43},
  {7, 44,	16, 41},
  {7, 46,	16, 40},
  {7, 48,	16, 39},
  {7, 49,	16, 37},
  {7, 51,	16, 36},
  {7, 53,	16, 35},
  {7, 55,	16, 33},
  {7, 56,	16, 32},
  {7, 58,	16, 31},
  {7, 59,	16, 30},
  {8, 1,	16, 29},
  {8, 3,	16, 28},
  {8, 4,	16, 27},
  {8, 6,	16, 26},
  {8, 7,	16, 25},
  {8, 9,	16, 24},
  {8, 10,	16, 24},
  {8, 12,	16, 23},

  //  Dezember
  {8, 13,	16, 22},
  {8, 14,	16, 22},
  {8, 16,	16, 21},
  {8, 17,	16, 21},
  {8, 18,	16, 20},
  {8, 20,	16, 20},
  {8, 21,	16, 19},
  {8, 22,	16, 19},
  {8, 23,	16, 19},
  {8, 24,	16, 19},
  {8, 25,	16, 19},
  {8, 26,	16, 19},
  {8, 27,	16, 19},
  {8, 28,	16, 19},
  {8, 29,	16, 19},
  {8, 30,	16, 19},
  {8, 31,	16, 19},
  {8, 31,	16, 19},
  {8, 32,	16, 20},
  {8, 33,	16, 20},
  {8, 33,	16, 21},
  {8, 34,	16, 21},
  {8, 34,	16, 22},
  {8, 34,	16, 22},
  {8, 35,	16, 23},
  {8, 35,	16, 24},
  {8, 35,	16, 24},
  {8, 35,	16, 25},
  {8, 36,	16, 26},
  {8, 36,	16, 27},
  {8, 36,	16, 28}

};   // Ende SchaltzeitenAstroMorgenAbend[365]
