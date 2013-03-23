/*
*	XP Header File
*/

// Objective Modifiers
#define DEFUSING_BOMB		10			// XP awarded when the user starts to defuse the bomb
#define DEFUSED_BOMB		10			// XP awarded when the user defuses the bomb
#define PLANTING_BOMB		10			// XP awarded when the user starts planting the bomb
#define PLANT_BOMB			10			// XP awarded when the user plants the bomb
#define SPAWN_BOMB			10			// XP awarded when the user spawns with the bomb
#define BOMB_PICKUP			10			// XP awarded when the user picks up the bomb
#define TOUCH_HOSTAGE		10			// XP awarded when the user touches a hostage
#define RESCUE_HOSTAGE		10			// XP awarded when the user rescues the hostage
#define KILL_HOSTAGE		10			// XP lost when killing a hostage
#define SPAWN_VIP			10			// XP awarded for spawning as the VIP
#define ESCAPE_VIP			10			// XP awarded for escaping as the VIP
#define OBJ_RADIUS			500			// Nearby radius to award XP for helping complete objectives

// Kill modifiers
#define KILL_HEADSHOT		10			// XP awarded for getting a headshot
#define KILL_HOSTAGE_SAVER	10			// XP awarded for killing the hostage saver
#define	KILL_DEFUSER		10			// XP awarded for killing the defuser
#define KILL_PLANTER		10			// XP awarded for killing the planter
#define KILL_BOMB_CARRIER	10			// XP awarded for killing the bomb carrier
#define KILL_VIP			10			// XP awarded for killing the VIP
#define KILL_RADIUS			250			// Nearby radius to award XP

#define WIN_ROUND			10			// XP awarded for winning the round


// Holds information about the player
enum
{
	PLR_BOMB_DEFUSER = 1,
	PLR_BOMB_PLANTER,
	PLR_HOSTAGE_RESCUER,
	PLR_VIP,
	PLR_BOMB_CARRIER,
};

new g_iPlayerRole[33];
new bool:bHasBegunPlantingOrDefusing[33];

// Holds the XP Multipliers per weapon
new Float:fWpnXPMultiplier[CSW_WAR3_MAX+1] =		{1.0};

// Amount of XP needed to gain a level
new iXPLevelShortTerm[MAX_LEVELS+1]				= {0,150,300,600,1000,1500,2100,2800,3400,4500,5500,7500,9500,11500,13500,15500,17500,19500,21500,23500,25500,27500,29500,31500,33500,35500,37500,39500,41500,43500,45500,47500,47500,51500,53500,55500,57500,59500,61500,63500,65500};
// new iXPLevelSaved[MAX_LEVELS+1]					= {0,100,200,400,800,1600,3200,6400,12800,25600,51200,101200,151200,201200,251200,301200,351200,401200,451200,501200,551200,601200,651200,701200,751200,801200,851200,901200,951200,1001200,1051200,1101200,1151200,1201200,1251200,1301200,1351200,1401200,1451200,1501200,1551200};
new iXPLevelSaved[MAX_LEVELS+1] = {0,
200,
400,
800,
1600,
3200,
6400,
12800,
25600,
51200,
102400,
204800,
409600,
614400,
921600,
1382400,
2073600,
3110400,
4665600,
5598720,
6718464,
8062156,
9674588,
11609505,
13931406,
16717688,
20061226,
24073471,
28888165,
34665798,
41598958,
49918749,
59902499,
71882999,
86259599,
103511519,
124213823,
149056588,
178867906,
214641487,
257569784
};

// Amount of XP awarded when killing a user of this level
new iXPGivenShortTerm[MAX_LEVELS+1]				= {10,15,25,35,40,50,60,70,80,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
// new iXPGivenSaved[MAX_LEVELS+1]					= {6,8,10,12,14,16,18,20,24,28,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92};

new iXPGivenSaved[MAX_LEVELS+1] = {8,
9,
11,
13,
16,
19,
23,
28,
34,
41,
49,
59,
71,
106,
160,
240,
360,
540,
810,
972,
1166,
1399,
1679,
2014,
2416,
2900,
3480,
4176,
5011,
5512,
6061,
6669,
7336,
8070,
8877,
9765,
10741,
10741,
10741,
10741,
10741
};