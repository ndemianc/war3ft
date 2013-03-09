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
302400,
402400,
502400,
1002400,
1502400,
2002400,
2102400,
2202400,
2302400,
2402400,
2502400,
2602400,
2702400,
2802400,
2902400,
3002400,
3102400,
6204800,
10240000,
20480000,
40960000,
81920000,
160000000,
200000000,
250000000,
300000000,
350000000,
400000000,
450000000,
500000000
};

// Amount of XP awarded when killing a user of this level
new iXPGivenShortTerm[MAX_LEVELS+1]				= {10,15,25,35,40,50,60,70,80,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245};
// new iXPGivenSaved[MAX_LEVELS+1]					= {6,8,10,12,14,16,18,20,24,28,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92};

new iXPGivenSaved[MAX_LEVELS+1] = {8,
18,
28,
38,
48,
58,
68,
78,
88,
98,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108,
108
};