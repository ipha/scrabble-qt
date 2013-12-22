#ifndef SCRABBLERULES_H
#define SCRABBLETULES_H

#define SCRABBLE 0
#define WORDSWITHFRIENDS 1

const int letter_mult[2][15][15] = 
{{ // Scrabble letter multiplier
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1},
	{1,1,1,1,1,3,1,1,1,3,1,1,1,1,1},
	{1,1,1,1,1,1,2,1,2,1,1,1,1,1,1},
	{2,1,1,1,1,1,1,2,1,1,1,1,1,1,2},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,3,1,1,1,3,1,1,1,3,1,1,1,3,1},
	{1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1},
	{1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
	{1,3,1,1,1,3,1,1,1,3,1,1,1,3,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{2,1,1,1,1,1,1,2,1,1,1,1,1,1,2},
	{1,1,1,1,1,1,2,1,2,1,1,1,1,1,1},
	{1,1,1,1,1,3,1,1,1,3,1,1,1,1,1},
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1}
},
{ // Words with Friends letter multiplier
	{1,1,1,1,1,1,3,1,3,1,1,1,1,1,1},
	{1,1,2,1,1,1,1,1,1,1,1,1,2,1,1},
	{1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
	{1,1,1,3,1,1,1,1,1,1,1,3,1,1,1},
	{1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
	{1,1,1,1,1,3,1,1,1,3,1,1,1,1,1},
	{3,1,1,1,2,1,1,1,1,1,2,1,1,1,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{3,1,1,1,2,1,1,1,1,1,2,1,1,1,3},
	{1,1,1,1,1,3,1,1,1,3,1,1,1,1,1},
	{1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
	{1,1,1,3,1,1,1,1,1,1,1,3,1,1,1},
	{1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
	{1,1,2,1,1,1,1,1,1,1,1,1,2,1,1},
	{1,1,1,1,1,1,3,1,3,1,1,1,1,1,1}
}};

const int word_mult[2][15][15] =
{{ // Scrabble word multiplier
	{3,1,1,1,1,1,1,3,1,1,1,1,1,1,3},
	{1,2,1,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,1,2,1,1,1,1,1,1,1,1,1,2,1,1},
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1},
	{1,1,1,1,2,1,1,1,1,1,2,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{3,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,2,1,1,1,1,1,2,1,1,1,1},
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1},
	{1,1,2,1,1,1,1,1,1,1,1,1,2,1,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,1,2,1},
	{3,1,1,1,1,1,1,3,1,1,1,1,1,1,3}
},
{ // Words with Friends word multiplier
	{1,1,1,3,1,1,1,1,1,1,1,3,1,1,1},
	{1,1,1,1,1,2,1,1,1,2,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{3,1,1,1,1,1,1,2,1,1,1,1,1,1,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,2,1,1,1,1,1,1,1,2,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,2,1,1,1,1,1,1,1,1,1,1,1,2,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{3,1,1,1,1,1,1,2,1,1,1,1,1,1,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,2,1,1,1,2,1,1,1,1,1},
	{1,1,1,3,1,1,1,1,1,1,1,3,1,1,1}
}};

const int all_tiles_bonus[2] = 
{ // Scrabble
	50,
  // Words with Friends
	35
};

const int letter_value[2][26] = 
{{ // Scrabble letter values
  //a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p   q  r  s  t  u  v  w  x  y   z
	1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10
},
{ // Words with Friends letter values
  //a  b  c  d  e  f  g  h  i   j  k  l  m  n  o  p   q  r  s  t  u  v  w  x  y   z
	1, 4, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
}};

#endif // SCRABBLERULES_H
