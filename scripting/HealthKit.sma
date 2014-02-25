#include < amxmodx >
#include < hamsandwich >
#include < fakemeta >


#define MAX_CLIENTS				32

#define MODEL_HEALTHKIT			"models/fl0wer/Medkit.mdl"
#define SOUND_HEALTHKIT			"items/smallmedkit1.wav"


new g_iszModelIndexSmoke;


public plugin_init( )
{
	register_plugin( "HealthKit ( From CS: Source )", "Best", "fl0wer" );

	RegisterHam( Ham_Killed, "player", "CPlayer__Killed_Post", .Post = 1 );

	RegisterHam( Ham_Think, "info_target", "CInfoTarget__Think_Post", .Post = 1 );
	RegisterHam( Ham_Touch, "info_target", "CInfoTarget__Touch_Post", .Post = 1 );
}

public plugin_precache( )
{
	precache_model( MODEL_HEALTHKIT );
	precache_sound( SOUND_HEALTHKIT );

	g_iszModelIndexSmoke = precache_model( "sprites/steam1.spr" );
}

public CPlayer__Killed_Post( iVictim, iAttacker, iGib )
{
	if( iVictim == iAttacker || !is_user_connected( iAttacker ) )
		return;

	CHealthKit__Create( iVictim );
}

public CInfoTarget__Think_Post( iEntity )
{
	if( !pev_valid( iEntity ) )
		return;

	static szClassName[ 10 ];
	pev( iEntity, pev_classname, szClassName, 9 );

	if( !equal( szClassName, "medkit" ) )
		return;

	if( pev( iEntity, pev_iuser1 ) )
	{
		new Float: flRenderAmt;
		pev( iEntity, pev_renderamt, flRenderAmt );

		if( flRenderAmt > 15.0 )
		{
			set_pev( iEntity, pev_renderamt, flRenderAmt - 15.0 );
			set_pev( iEntity, pev_nextthink, get_gametime( ) + 0.05 );
		}
		else
		{
			set_pev( iEntity, pev_renderamt, 0.0 );
			set_pev( iEntity, pev_nextthink, get_gametime( ) + 0.2 );

			set_pev( iEntity, pev_flags, FL_KILLME );
		}
	}
	else
	{
		new Float: vecOrigin[ 3 ];
		pev( iEntity, pev_origin, vecOrigin );

		engfunc(EngFunc_MessageBegin, MSG_BROADCAST, SVC_TEMPENTITY, vecOrigin, 0 );
		write_byte( TE_SMOKE );
		engfunc( EngFunc_WriteCoord, vecOrigin[ 0 ] );
		engfunc( EngFunc_WriteCoord, vecOrigin[ 1 ] );
		engfunc( EngFunc_WriteCoord, vecOrigin[ 2 ] - 24.0 );
		write_short( g_iszModelIndexSmoke );
		write_byte( 5 );
		write_byte( 8 );
		message_end( );

		set_pev( iEntity, pev_iuser1, 1 );
		set_pev( iEntity, pev_skin, 1 );

		set_pev( iEntity, pev_renderamt, 255.0 );
		set_pev( iEntity, pev_rendermode, kRenderTransTexture );
		set_pev( iEntity, pev_nextthink, get_gametime( ) + 1.5 );
	}
}

public CInfoTarget__Touch_Post( iEntity, iOther )
{
	if( !pev_valid( iEntity ) || !pev_valid( iOther ) )
		return;

	static szClassName[ 10 ];
	pev( iEntity, pev_classname, szClassName, 9 );

	if( !equal( szClassName, "medkit" ) )
		return;

	if( iOther < 1 || iOther > MAX_CLIENTS )
		return;

	if( ExecuteHam( Ham_TakeHealth, iOther, 20.0, DMG_GENERIC ) )
	{
		message_begin( MSG_ONE, 93, _, iOther );
		write_string( "item_healthkit" );
		message_end( );

		emit_sound( iOther, 3, SOUND_HEALTHKIT, 0.2, 0.8, 0, 100 );
	}

	set_pev( iEntity, pev_flags, FL_KILLME );
}

CHealthKit__Create( iPlayer )
{
	new iEntity;
	static iszAllocStringCached;

	if( iszAllocStringCached || ( iszAllocStringCached = engfunc( EngFunc_AllocString, "info_target" ) ) )
		iEntity = engfunc( EngFunc_CreateNamedEntity, iszAllocStringCached );

	if( !pev_valid( iEntity ) )
		return;

	set_pev( iEntity, pev_classname, "medkit" );
	set_pev( iEntity, pev_owner, iPlayer );
	set_pev( iEntity, pev_solid, SOLID_TRIGGER );
	set_pev( iEntity, pev_movetype, MOVETYPE_TOSS );
	set_pev( iEntity, pev_nextthink, get_gametime() + 10.0 );

	engfunc( EngFunc_SetModel, iEntity, MODEL_HEALTHKIT );
	engfunc( EngFunc_SetSize, iEntity, Float: { -8.57, -6.9, 0.0 }, Float: { 8.64, 7.36, 6.11 } );

	new Float: vecOrigin[ 3 ];
	new Float: vecViewOfs[ 3 ];
	new Float: vecVelocity[ 3 ];

	pev( iPlayer, pev_origin, vecOrigin );
	pev( iPlayer, pev_view_ofs, vecViewOfs );

	vecOrigin[ 0 ] += vecViewOfs[ 0 ];
	vecOrigin[ 1 ] += vecViewOfs[ 1 ];
	vecOrigin[ 2 ] += vecViewOfs[ 2 ];

	vecVelocity[ 0 ] = random_float( -200.0, 200.0 );
	vecVelocity[ 1 ] = random_float( -200.0, 200.0 );
	vecVelocity[ 2 ] = random_float( 1.0, 200.0 );

	set_pev( iEntity, pev_origin, vecOrigin );
	set_pev( iEntity, pev_velocity, vecVelocity );
}
