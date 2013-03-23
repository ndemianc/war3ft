#define LINE_WITH 80 // Ширина полоски 
// Скилл воскрешение {

public respawn_menu(id) {
    if (g_F_counter[id] < 1) {
        new title_msg[100];
        formatex(title_msg, charsmax(title_msg), "%L", LANG_PLAYER, "NECROMANT_MENU")
        new menu = menu_create(title_msg, "menu_res")
        new players[32], pnum, tempid;
        new szName[32], szTempid[10];
        get_players(players, pnum);

        for (new i; i < pnum; i++) {
            tempid = players[i]

            if (is_user_alive(tempid)) {
                WC3_StatusText(id, 0, "%L", LANG_PLAYER, "WC3_TEMPID_LIVE");
            }

            if (!is_user_alive(tempid) && is_user_alive(id)) {
                get_user_name(tempid, szName, 31)
                num_to_str(tempid, szTempid, 9)
                if (get_user_team(id) == get_user_team(tempid)) {
                    menu_additem(menu, szName, szTempid, 0)
                    g_F_counter[id]++
                }
            }
        }

        menu_display(id, menu, 0)

    }

    return PLUGIN_HANDLED

}

public menu_res(idUser, menu, item) {
    if (item == MENU_EXIT) {
        menu_destroy(menu)
        return PLUGIN_HANDLED
    }

    new data[6], iName[64], playername[33], playername2[33]
    new access, callback
    menu_item_getinfo(menu, item, access, data, 5, iName, 63, callback)

    new tempid = str_to_num(data)
    cs_get_user_team(tempid)

    get_user_name(idUser, playername, 32)
    get_user_name(tempid, playername2, 32)

    static iSkillLevel;

    iSkillLevel = SM_GetSkillLevel(idUser, SKILL_UNHOLY_RESURRECTION);
    if (iSkillLevel > 0) {
        if (!is_user_alive(tempid) && random_float(0.0, 1.0) <= p_unholy_resurrection[iSkillLevel - 1]) {
            // Воскрешаем Игрока
            ExecuteHam(Ham_CS_RoundRespawn, tempid);

            // Воспроизводим музыку воскрешения у воскресителя
            emit_sound(idUser, CHAN_STATIC, g_szSounds[SOUND_RESPAWN], 1.0, ATTN_NORM, 0, PITCH_NORM);

            // Воспроизводим музыку воскрешения у воскрешенного игрока
            emit_sound(tempid, CHAN_STATIC, g_szSounds[SOUND_RESPAWNED], 1.0, ATTN_NORM, 0, PITCH_NORM);

            // Плюсуем денушку воскресителю
            new iNewMoney = SHARED_GetUserMoney(idUser) + 450;
            SHARED_SetUserMoney(idUser, iNewMoney);

            new szMessage[256];
            format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
            LANG_PLAYER, "WC3_BRACKET_LEFT", LANG_PLAYER, "WC3_MODCLIENT", LANG_PLAYER, "WC3_BRACKET_RIGHT",
            LANG_PLAYER, "WC3_RESPAWN_MONEY_PLUS");

            client_print(idUser, print_chat, szMessage);

            // Минусуем денюжку воскресившемуся
            new iStoledMoney = SHARED_GetUserMoney(tempid) - 450;
            SHARED_SetUserMoney(tempid, iStoledMoney);

            format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
            LANG_PLAYER, "WC3_BRACKET_LEFT", LANG_PLAYER, "WC3_MODCLIENT", LANG_PLAYER, "WC3_BRACKET_RIGHT",
            LANG_PLAYER, "WC3_RESPAWN_MONEY_MINUS");

            client_print(tempid, print_chat, szMessage);

        } else {
            if (!is_user_alive(tempid) && random_float(0.0, 1.0) > p_unholy_resurrection[iSkillLevel - 1]) // Больше(float),то выполняем действие
            {
                new szMessage[256];
                format(szMessage, sizeof(szMessage) - 1, "%L%L%L %L",
                LANG_PLAYER, "WC3_BRACKET_LEFT", LANG_PLAYER, "WC3_MODCLIENT", LANG_PLAYER, "WC3_BRACKET_RIGHT",
                LANG_PLAYER, "WC3_RESPAWN_LOW_LEVEL");

                client_print(idUser, print_chat, szMessage);
            }
        }
    }

    menu_destroy(menu)
    return PLUGIN_HANDLED
}

NM_ULT_Brain_Sap(iCaster, iTarget) {
    if (is_user_alive(iCaster) && is_user_alive(iTarget)) {
        new iHealth, iDamage;
        iHealth = get_user_health(iTarget);
        iDamage = 35 // Сколько хп отнимается
        if (iHealth > 0 || iHealth > iDamage) {
            WC3_Damage(iTarget, iCaster, iDamage, CSW_BRAIN_SAP, 0);
            set_user_health(iCaster, get_user_health(iCaster) + iDamage);
            Create_TE_BEAMENTS(iCaster, iTarget, g_iSprites[SPR_LIGHTNING], 0, 15, 10, LINE_WITH, 10, 196, 0, 255, 255, 0);
        } else {
            set_user_health(iTarget, iHealth - iDamage);
            set_user_health(iCaster, get_user_health(iCaster) + iDamage);
        }
    }

    ULT_ResetCooldown(iCaster, get_pcvar_num(CVAR_wc3_ult_cooldown));
}

//Скил 1 уровня - Волна жизни (Heling Wave)
// Helper function to start the healing process
public NM_BlackMana(idUser) {
    if (task_exists(idUser + TASK_BLACK_MANA) == 0) {
        _MN_BlackMana(idUser);
    }
}

// We don't need to ever actually "turn this off" since this task will take care of itself
public _MN_BlackMana(idUser) {
    if (idUser >= TASK_BLACK_MANA) {
        idUser -= TASK_BLACK_MANA;
    }

    // User is no longer connected
    if (!p_data_b[idUser][PB_ISCONNECTED]) {
        return;
    }

    static iSkillLevel;
    iSkillLevel = SM_GetSkillLevel(idUser, SKILL_BLACK_MANA);

    // User doesn't have this skill, so lets return
    //  - Also return if this was called twice
    if (iSkillLevel <= 0) return;

    // Continue healing...
    if (is_user_alive(idUser)) {
        set_task(p_black_mana[iSkillLevel - 1], "_MN_BlackMana", TASK_BLACK_MANA + idUser);
    }

    // Prevent healing if this player is hexed
    if (p_data_b[idUser][PB_HEXED]) {
        return;
    }

    new iMaxMoney = 16000;

    if (cs_get_user_money(idUser) + 30 <= iMaxMoney) {
        cs_set_user_money(idUser, cs_get_user_money(idUser) + 30);
    }

    return;
}

public FlashEvent(id) {
    if (!is_user_alive(id)) {
        return;
    }


    new Duration, HoldTime, Fade, Alpha;
    Duration = read_data(1);
    HoldTime = read_data(2);
    Fade = read_data(3);
    Alpha = read_data(7);
    Alpha = Alpha>50?Alpha-50:50;

    new players[32], numberofplayers, targetid;
    new iTeam = get_user_team(id);

    get_players(players, numberofplayers, "a");

    // Loop through each player and check to see if one is close enough to be healed
    for (new i = 0; i < numberofplayers; i++) {
        targetid = players[i];

        static iSkillLevel;
        iSkillLevel = SM_GetSkillLevel(targetid, SKILL_EYE_OF_INSIGHT);
        if (iSkillLevel > 0 && (p_data[targetid][P_RACE] == RACE_NECROMANCER)) {
            if (p_data_b[targetid][PB_ISCONNECTED] && get_user_team(targetid) != iTeam && (id != targetid)) {
                if (is_user_alive(targetid) && is_user_alive(id)) {
                    if (p_data[id][P_RACE] == RACE_NECROMANCER) {
                        message_begin(MSG_ONE, gmsgScreenFade, {
                            0, 0, 0
                        }, targetid)
                        write_short(0) // Duration
                        write_short(0) // Hold time
                        write_short(0) // Fade type
                        write_byte(0) // Red
                        write_byte(0) // Green
                        write_byte(0) // Blue
                        write_byte(0) // Alpha
                        message_end()

                        message_begin(MSG_ONE, gmsgScreenFade, {
                            0, 0, 0
                        }, id)
                        write_short(Duration) // Duration
                        write_short(HoldTime) // Hold time
                        write_short(Fade) // Fade type
                        write_byte(0) // Red
                        write_byte(255) // Green
                        write_byte(255) // Blue
                        write_byte(Alpha) // Alpha
                        message_end()

                        if (!SHARED_IsPlayerSlowed(id)) {
                            SHARED_GlowShell(id, 0, 255, 255, 2.0, 18);
                            p_data_b[id][PB_STUNNED] = true;
                            SHARED_SetSpeed(id);
                            set_task(1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + id);
                        }
                    } else {
                        message_begin(MSG_ONE, gmsgScreenFade, {
                            0, 0, 0
                        }, id)
                        write_short(0) // Duration
                        write_short(0) // Hold time
                        write_short(0) // Fade type
                        write_byte(0) // Red
                        write_byte(0) // Green
                        write_byte(0) // Blue
                        write_byte(0) // Alpha
                        message_end()

                        if (!SHARED_IsPlayerSlowed(id)) {
                            SHARED_GlowShell(id, 0, 255, 255, 2.0, 18);
                            p_data_b[id][PB_STUNNED] = true;
                            SHARED_SetSpeed(id);
                            set_task(1.0, "SHARED_ResetMaxSpeed", TASK_RESETSPEED + id);
                        }
                        
                        static Float:punchangle[3];
                        pev(id, pev_punchangle, punchangle);
        
                        punchangle[0] += random_float(-5.0, 5.0);
                        punchangle[1] += random_float(-5.0, 5.0);
                        punchangle[2] += random_float(-5.0, 5.0);

                        // this will shake the user's monitor a little bit
                        set_pev(id, pev_punchangle, punchangle);

                        new iDamage, iBonusDamage;
                        iBonusDamage = 110;
                        iDamage = floatround(float(iBonusDamage) * p_eye_of_insight[iSkillLevel - 1]);

                        Create_TE_BEAMFOLLOW(id, g_iSprites[SPR_TRAIL], 20, 5, 10, 255, 255, 255);

                        if (targetid == id || p_data[id][P_RACE] == RACE_NECROMANCER) {
                            WC3_Damage(targetid, id, 0, CSW_EYE_OF_INSIGHT, 0);
                            WC3_Damage(id, targetid, iDamage, CSW_EYE_OF_INSIGHT, 0);
                        } else {
                            WC3_Damage(id, targetid, iDamage, CSW_EYE_OF_INSIGHT, 0);
                        }

                        client_cmd(targetid, "speak %s", g_szSounds[SOUND_IMPALE]);
                        client_cmd(id, "speak %s", g_szSounds[SOUND_IMPALE]);
                    }
                }
            }
        }
    }
}


public fw_emitsound(entity, channel, const sample[], Float: volume, Float: attenuation, fFlags, pitch) {
    if (!equali(sample, "weapons/flashbang-1.wav") && !equali(sample, "weapons/flashbang-2.wav")) return FMRES_IGNORED

    new players[32], numberofplayers, targetid;
    get_players(players, numberofplayers, "a");

    // Loop through each player and check to see if one is close enough to be healed
    for (new i = 0; i < numberofplayers; i++) {
        targetid = players[i];
        if (p_data[targetid][P_RACE] == RACE_NECROMANCER) {
            flashbang_explode(entity);
        }
    }

    return FMRES_IGNORED
}

public flashbang_explode(greindex) {
    if (!pev_valid(greindex)) return

    // get origin of explosion
    new Float: origin[3]
    pev(greindex, pev_origin, origin)

    // get color
    new Red,
    Green,
    Blue;
    Red = 20;
    Green = 70;
    Blue = 120;

    message_begin(MSG_BROADCAST, SVC_TEMPENTITY)
    write_byte(TE_DLIGHT) // 27
    write_coord(floatround(origin[0])) // x
    write_coord(floatround(origin[1])) // y
    write_coord(floatround(origin[2])) // z
    write_byte(50) // radius
    write_byte(Red) // Red
    write_byte(Green) // Green
    write_byte(Blue) // Blue
    write_byte(8) // life
    write_byte(60) // decay rate
    message_end()
}
