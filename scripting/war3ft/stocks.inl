stock get_user_maxhealth( id )
{

	new maxHealth = 100

	static iSkillLevel;
	iSkillLevel = SM_GetSkillLevel( id, SKILL_DEVOTION );

	// Human Devotion Skill
	if ( iSkillLevel > 0 )
	{
		maxHealth += iSkillLevel * p_devotion;
	}


	// Player has a health bonus from the Periapt of Health

	if ( ITEM_Has( id, ITEM_HEALTH ) > ITEM_NONE )
		maxHealth += get_pcvar_num( CVAR_wc3_health );

	return maxHealth
}

// Function checks to see if the weapon is a pistol
stock SHARED_IsSecondaryWeapon( iWeaponID )
{
	// Check for Counter-Strike or Condition Zero
	if ( g_MOD == GAME_CSTRIKE || g_MOD == GAME_CZERO )
	{
		if ( iWeaponID == CSW_ELITE || iWeaponID == CSW_FIVESEVEN || iWeaponID == CSW_USP || iWeaponID == CSW_GLOCK18 || iWeaponID == CSW_DEAGLE || iWeaponID == CSW_P90 )
		{
			return true;
		}
	}
	
	// Check for Day of Defeat
	else if ( g_MOD == GAME_DOD )
	{
    return false;
	}


	return false;
}

// sdemian

stock client_print_color_msg(const id, const input[], any:...) {
	static msg[256]; 
	vformat(msg, 255, input, 3); 
	replace_all(msg, 255, "!g", "^4"); // Green Color 
	replace_all(msg, 255, "!y", "^1"); // Default Color (yellow)
	replace_all(msg, 255, "!t", "^3"); // Team Color
	format(msg, 255, "^x01%s", msg);	
	if ( is_user_connected(id) ) { 
			message_begin(MSG_ONE_UNRELIABLE, gmsgSayText, _, id); 
			write_byte(id); 
			write_string(msg); 
			message_end(); 
	} 
}  
