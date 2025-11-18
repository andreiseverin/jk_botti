//
// JK_Botti - be more human!
//
// bot_skill.cpp
//

#define BOTSKILL

#ifndef _WIN32
#include <string.h>
#endif

#include <extdll.h>
#include <dllapi.h>
#include <h_export.h>
#include <meta_api.h>

#include "bot.h"
#include "bot_func.h"
#include "waypoint.h"
#include "bot_weapons.h"

#include "bot_skill.h"

#include <ctype.h>

#if 0
COPY+PASTE from bot_skill.h, for explaning values in following table:
typedef struct
{

0:
   int pause_frequency; // how often (out of 1000 times) the bot will pause, based on bot skill
   float pause_time_min; // how long bot pauses (min, max)
   float pause_time_max; 
   
   float normal_strafe; // how much bot strafes when walking around
   float battle_strafe; // how much bot strafes when attacking enemy
  
   int keep_optimal_dist; // how often bot (out of 100 times) the bot try to keep at optimum distance of weapon when attacking

6: 
   float respawn_react_delay; // delay on players after respawn
   float react_delay_min; // reaction delay settings (first is for bot_reaction 1, second for 2, etc)
   float react_delay_max;

9:
   float weaponchange_rate_min; // how fast changing weapons (min, max)
   float weaponchange_rate_max; 

11: 
   float shootcone_diameter; // bot tries to fire when aim line is less than [diameter / 2] apart from target 
   float shootcone_minangle; // OR angle between bot aim line and line to target is less than angle set here

13:   
   float turn_skill; // BotAim turn_skill, how good bot is at aiming on enemy origin.
   float turn_slowness; // Is bot's aim in slow motion?
   float updown_turn_ration; // how much slower bots aims up and down than side ways?

16:
   // Bot doesn't use real origin of target player but instead use ping emulation based on recorded old position data of player. 
   // These settings specify ammount of latency and randomness used at different skill levels.
   float ping_emu_latency; // ping emulation in seconds
   float ping_emu_speed_varitation; // percent
   float ping_emu_position_varitation; // units from target center

19:
   qboolean can_longjump; // and can longjump.
   
   int random_jump_frequency; // how often (out of 100 times) the bot will do random jump
   int random_jump_duck_frequency; // how often (out of 100 times) the bot will do random duck when random jumping
   int random_duck_frequency; // how often (out of 100 times) the bot will do random duck jumping in combat mode
   int random_longjump_frequency; // how often (out of 100 times) the bot will do random longjump instead of random jump


24:
   qboolean can_taujump; // can tau jump? (waypoint taujump, attack/flee taujump)


   int attack_taujump_frequency; // how often (out of 100 times) the bot will do tau jump at far away enemy
   int flee_taujump_frequency; // how often (out of 100 times) the bot will taujump away from enemy
   
   float attack_taujump_distance; // how far enemy have to be to bot to use tau jump
   float flee_taujump_distance; // max distance to flee enemy from
   float flee_taujump_health; // how much bot has health left when tries to escape
   float flee_taujump_escape_distance; // how long way bot tries to move away

   qboolean can_shoot_through_walls; // can shoot through walls by sound
   int wallshoot_frequency; // how often (out of 100 times) the bot will try attack enemy behind wall


33:
   float hearing_sensitivity; // how well bot hears sounds
   float track_sound_time_min; // how long bot tracks one sound
   float track_sound_time_max;

} bot_skill_settings_t;
#endif

bot_skill_settings_t default_skill_settings[5] = {
   // **** best skill (lvl1) ****
   {
//0:
     1, 0.05, 0.30, 10.0, 50.0, 80, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//6:
     0.6, 0.08, 0.12, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//9:
     0.1, 0.3, // weapon change rate min, weapon change rate max
//11:
     150.0, 12.5, // shoot cone diameter, shoot cone min angle
//13:
     4.0, 1.0, 2.0, // turn skill, turn slowness, updown turn ratio
//16:
     0.060, 0.03, 2.0, // ping latency, ping speed variation, ping position variation
//19:
     TRUE, 50, 75, 50, 100,  // Enable longjump for lvl1
//24:
     TRUE, 100, 100, 1000.0, 400.0, 20.0, 1000.0, // can longjump, random jump frequency, random jump duck frequency, random duck frequency, random longjump frequency
//33:
     1.5, 20.0, 40.0 }, // hearing sensitivity, track sound time min, track sound time max

   // **** lvl2 ****
   {
//0:
     2, 0.10, 0.60, 9.5, 30.0, 60, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//6:
     0.8, 0.12, 0.18, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//9:
     0.2, 0.5, // weapon change rate min, weapon change rate max
//11:
     175.0, 20.0, // shoot cone diameter, shoot cone min angle
//13:
     3.0, 1.25, 2.25, // turn skill, turn slowness, updown turn ratio
//16:
     0.120, 0.04, 3.0, // ping latency, ping speed variation, ping position variation
//19:
     TRUE, 50, 75, 50, 100,  // Enable longjump for lvl2
//24:
     TRUE, 100, 100, 1000.0, 400.0, 20.0, 1000.0, // can longjump, random jump frequency, random jump duck frequency, random duck frequency, random longjump frequency
//33:
     1.25, 15.0, 30.0 }, // hearing sensitivity, track sound time min, track sound time max

   // **** lvl3 ****
   {
//0:
     3, 0.15, 0.80, 8.0, 15.0, 40, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//6:
     1.0, 0.16, 0.24, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//9:
     0.3, 0.7, // weapon change rate min, weapon change rate max
//11:
     200.0, 25.0, // shoot cone diameter, shoot cone min angle
//13:
     2.0, 1.5, 2.50, // turn skill, turn slowness, updown turn ratio
//16:
     0.180, 0.05, 4.0, // ping latency, ping speed variation, ping position variation          
//19:
     TRUE, 50, 75, 50, 100,  // Enable longjump for lvl3
//24:
     TRUE, 100, 100, 1000.0, 400.0, 20.0, 1000.0, // can longjump, random jump frequency, random jump duck frequency, random duck frequency, random longjump frequency
//33:
     1.0, 10.0, 20.0 }, // hearing sensitivity, track sound time min, track sound time max

   // **** lvl4 ****
   {
//0:
     4, 0.20, 0.90, 7.5, 7.5, 25, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//6:
     1.20, 0.2, 0.3, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//9:
     0.6, 1.4, // weapon change rate min, weapon change rate max
//11:
     250.0, 30.0, // shoot cone diameter, shoot cone min angle
//13:
     1.25, 1.75, 2.75, // turn skill, turn slowness, updown turn ratio
//16:
     0.240, 0.075, 6.0, // ping latency, ping speed variation, ping position variation          
//19:
     TRUE, 50, 75, 50, 100,  // Enable longjump for lvl4
//24:
     TRUE, 100, 100, 1000.0, 400.0, 20.0, 1000.0, // can longjump, random jump frequency, random jump duck frequency, random duck frequency, random longjump frequency
//33:
     0.75, 7.5, 15.0 }, // hearing sensitivity, track sound time min, track sound time max

   // **** worst skill (lvl5) ****
   {
//0:
     5, 0.25, 1.00, 7.0, 1.0, 15, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//6:
     1.4, 0.24, 0.36, // pause frequency, pause time min, pause time max, normal strafe, battle strafe, keep optimal distance
//9:
     1.2, 2.8, // weapon change rate min, weapon change rate max
//11:
     300.0, 35.0, // shoot cone diameter, shoot cone min angle
//13:
     0.75, 2.0, 3.0, // turn skill, turn slowness, updown turn ratio
//16:
     0.300, 0.10, 8.0, // ping latency, ping speed variation, ping position variation          
//19:
     TRUE, 50, 75, 50, 100,  // Enable longjump for lvl5
//24:
     TRUE, 100, 100, 1000.0, 400.0, 20.0, 1000.0, // can longjump, random jump frequency, random jump duck frequency, random duck frequency, random longjump frequency
//33:
     0.5, 5.0, 10.0 },
};

bot_skill_settings_t skill_settings[5];

// new: load default skill table from cfg file (named fields: name[level]=value)
static qboolean ParseBoolToken(const char *tok)
{
    if (!tok) return FALSE;
    if (tok[0] == '1') return TRUE;
    if (tok[0] == '0') return FALSE;
    if (strcasecmp(tok, "true") == 0) return TRUE;
    if (strcasecmp(tok, "false") == 0) return FALSE;
    return FALSE;
}

static char *TrimWhitespace(char *s)
{
    char *end;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) *end-- = '\0';
    return s;
}

static void SetSkillField(bot_skill_settings_t *s, const char *name, const char *valstr)
{
    if (strcmp(name, "pause_frequency") == 0) s->pause_frequency = atoi(valstr);
    else if (strcmp(name, "pause_time_min") == 0) s->pause_time_min = (float)atof(valstr);
    else if (strcmp(name, "pause_time_max") == 0) s->pause_time_max = (float)atof(valstr);
    else if (strcmp(name, "normal_strafe") == 0) s->normal_strafe = (float)atof(valstr);
    else if (strcmp(name, "battle_strafe") == 0) s->battle_strafe = (float)atof(valstr);
    else if (strcmp(name, "keep_optimal_dist") == 0) s->keep_optimal_dist = atoi(valstr);

    else if (strcmp(name, "respawn_react_delay") == 0) s->respawn_react_delay = (float)atof(valstr);
    else if (strcmp(name, "react_delay_min") == 0) s->react_delay_min = (float)atof(valstr);
    else if (strcmp(name, "react_delay_max") == 0) s->react_delay_max = (float)atof(valstr);

    else if (strcmp(name, "weaponchange_rate_min") == 0) s->weaponchange_rate_min = (float)atof(valstr);
    else if (strcmp(name, "weaponchange_rate_max") == 0) s->weaponchange_rate_max = (float)atof(valstr);

    else if (strcmp(name, "shootcone_diameter") == 0) s->shootcone_diameter = (float)atof(valstr);
    else if (strcmp(name, "shootcone_minangle") == 0) s->shootcone_minangle = (float)atof(valstr);

    else if (strcmp(name, "turn_skill") == 0) s->turn_skill = (float)atof(valstr);
    else if (strcmp(name, "turn_slowness") == 0) s->turn_slowness = (float)atof(valstr);
    else if (strcmp(name, "updown_turn_ration") == 0) s->updown_turn_ration = (float)atof(valstr);

    else if (strcmp(name, "ping_emu_latency") == 0) s->ping_emu_latency = (float)atof(valstr);
    else if (strcmp(name, "ping_emu_speed_varitation") == 0) s->ping_emu_speed_varitation = (float)atof(valstr);
    else if (strcmp(name, "ping_emu_position_varitation") == 0) s->ping_emu_position_varitation = (float)atof(valstr);

    else if (strcmp(name, "can_longjump") == 0) s->can_longjump = ParseBoolToken(valstr);
    else if (strcmp(name, "random_jump_frequency") == 0) s->random_jump_frequency = atoi(valstr);
    else if (strcmp(name, "random_jump_duck_frequency") == 0) s->random_jump_duck_frequency = atoi(valstr);
    else if (strcmp(name, "random_duck_frequency") == 0) s->random_duck_frequency = atoi(valstr);
    else if (strcmp(name, "random_longjump_frequency") == 0) s->random_longjump_frequency = atoi(valstr);

    else if (strcmp(name, "can_taujump") == 0) s->can_taujump = ParseBoolToken(valstr);
    else if (strcmp(name, "attack_taujump_frequency") == 0) s->attack_taujump_frequency = atoi(valstr);
    else if (strcmp(name, "flee_taujump_frequency") == 0) s->flee_taujump_frequency = atoi(valstr);
    else if (strcmp(name, "attack_taujump_distance") == 0) s->attack_taujump_distance = (float)atof(valstr);
    else if (strcmp(name, "flee_taujump_distance") == 0) s->flee_taujump_distance = (float)atof(valstr);
    else if (strcmp(name, "flee_taujump_health") == 0) s->flee_taujump_health = (float)atof(valstr);
    else if (strcmp(name, "flee_taujump_escape_distance") == 0) s->flee_taujump_escape_distance = (float)atof(valstr);

    else if (strcmp(name, "hearing_sensitivity") == 0) s->hearing_sensitivity = (float)atof(valstr);
    else if (strcmp(name, "track_sound_time_min") == 0) s->track_sound_time_min = (float)atof(valstr);
    else if (strcmp(name, "track_sound_time_max") == 0) s->track_sound_time_max = (float)atof(valstr);
    // unknown names are ignored silently
}

static qboolean LoadDefaultSkillSettingsFromFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return FALSE;

    char line[512];
    qboolean any_set = FALSE;

    while (fgets(line, sizeof(line), fp)) {
        char *s = TrimWhitespace(line);
        if (!*s) continue;
        if (s[0] == '#' || s[0] == ';') continue;
        if (s[0] == '/' && s[1] == '/') continue;

        // find '='
        char *eq = strchr(s, '=');
        if (!eq) continue;
        *eq = '\0';
        char *lhs = TrimWhitespace(s);
        char *rhs = TrimWhitespace(eq + 1);

        // lhs expected: name[index]  (e.g. pause_frequency[1])
        char fieldname[128];
        int level = 0;
        char *br = strchr(lhs, '[');
        char *brc = br ? strchr(lhs, ']') : NULL;
        if (!br || !brc) continue;
        size_t namelen = br - lhs;
        if (namelen >= sizeof(fieldname)) namelen = sizeof(fieldname) - 1;
        memcpy(fieldname, lhs, namelen);
        fieldname[namelen] = '\0';
        fieldname[namelen] = '\0';
        level = atoi(br + 1); // user provides 1..5

        if (level < 1 || level > 5) continue;
        int idx = level - 1;

        TrimWhitespace(fieldname);
        // set value on a copy of defaults: but skill_settings already initialized by caller
        SetSkillField(&skill_settings[idx], fieldname, rhs);
        any_set = TRUE;
    }

    fclose(fp);
    return any_set;
}

// modify ResetSkillsToDefault to copy compiled defaults then override from cfg if present
void ResetSkillsToDefault(void)
{
    // copy compiled defaults first
    memcpy(skill_settings, default_skill_settings, sizeof(skill_settings));

    char cfg_path[256];
    UTIL_BuildFileName_N(cfg_path, sizeof(cfg_path), "addons/jk_botti/bot_skill.cfg", NULL);

    // if file exists, override fields that are present. keep compiled defaults for missing fields.
    LoadDefaultSkillSettingsFromFile(cfg_path);
}
