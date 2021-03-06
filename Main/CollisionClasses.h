#define BIT(x) (1<<(x))

#ifndef _colClasses_
#define _colClasses_
enum collisiontypes{
	COL_NOTHING = 0,
	COL_FLYING_OBJECTS = BIT(0),
	COL_STRUCTURES = BIT(1),
	COL_PLAYER = BIT(2),
	COL_LETTERS = BIT(3),
	COL_WALL = BIT(4),
	COL_SPARTA_POINTS = BIT(5),
	COL_CAKE = BIT(6),
	COL_FINAL_CAKE = BIT(7),
	COL_BOUNDARY = BIT(8),
	COL_CASTLE = BIT(9)

};

#define PLAYER_COL_WITH (COL_FLYING_OBJECTS | COL_STRUCTURES | COL_LETTERS | \
	COL_SPARTA_POINTS | COL_CAKE | COL_FINAL_CAKE)
#define FLYINGOBJECTS_COL_WITH (COL_FLYING_OBJECTS | COL_STRUCTURES | COL_PLAYER | COL_BOUNDARY | \
		COL_CASTLE)
#define STRUCTURES_COL_WITH (COL_FLYING_OBJECTS | COL_STRUCTURES | COL_PLAYER | COL_FINAL_CAKE | \
		COL_CASTLE)
#define WALL_COL_WITH COL_BOUNDARY
#define LETTERS_COL_WITH COL_PLAYER
#define SPARTA_POINTS_COL_WITH COL_PLAYER | COL_BOUNDARY
#define CAKE_COL_WITH COL_PLAYER | COL_BOUNDARY
#define FINAL_CAKE_COL_WITH COL_PLAYER | COL_STRUCTURES | COL_BOUNDARY
#define BOUNDARY_COL_WITH (COL_FLYING_OBJECTS |	COL_WALL | COL_SPARTA_POINTS | COL_CAKE | \
		COL_FINAL_CAKE)
#define CASTLE_COL_WITH (COL_FLYING_OBJECTS | COL_STRUCTURES | COL_CASTLE)

#endif
