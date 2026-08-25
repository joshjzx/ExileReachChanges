/*
 * Copyright (c) Fluxurion - 2023
 *
 * Begin in 2021, rewritten in 2022, re-rewritten in 2023.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

 /*
 @TODO: Use conversations instead of scheduled talk
 */

#include "exiles_reach.h"
#include "MovementPackets.h"
#include "ScriptMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

class exiles_reach_general_playerscript : public PlayerScript
{
public:
    exiles_reach_general_playerscript() : PlayerScript("exiles_reach_general_playerscript") {}

    // Undead mowed down by the giant boar (spawnIds), so they can be respawned
    // if the ride quest is abandoned.
    static std::unordered_set<ObjectGuid::LowType> s_mowedDownUndead;
    // Players currently on the boar ride, so an abandon can be detected (the
    // OnQuestStatusChange hook does NOT fire for quest abandon).
    static std::unordered_set<ObjectGuid> s_rideActivePlayers;

    void CheckRideAbandon(Player* player)
    {
        ObjectGuid const guid = player->GetGUID();

        bool active   = false;
        bool rewarded = false;
        for (uint32 questId : { QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR, QUEST_H_THE_REDEATHER })
        {
            QuestStatus const status = player->GetQuestStatus(questId);
            if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE)
                active = true;
            else if (status == QUEST_STATUS_REWARDED)
                rewarded = true;
        }

        if (active)
            s_rideActivePlayers.insert(guid);
        else if (s_rideActivePlayers.erase(guid))
        {
            // Was riding and no longer is: abandoned (respawn the undead) or
            // completed (let them stay mowed down).
            if (!rewarded)
            {
                TC_LOG_ERROR("playerbot.v2", "[ExilesReach] ride abandoned; respawning {} tracked undead", s_mowedDownUndead.size());
                if (Map* map = player->GetMap())
                    for (ObjectGuid::LowType spawnId : s_mowedDownUndead)
                    {
                        TC_LOG_ERROR("playerbot.v2", "[ExilesReach]   respawn spawnId={}", spawnId);
                        map->Respawn(SPAWN_TYPE_CREATURE, spawnId);
                    }
            }
            s_mowedDownUndead.clear();
        }
    }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        if (player->GetMapId() != MAP_NPE)
            return;

        // Set player sanctuary in exile's reach
        player->SetPvpFlag(UNIT_BYTE2_FLAG_SANCTUARY);
    }

    void OnPlayerWorldObjectUpdate(Player* player, uint32 /*diff*/) override
    {
        CheckExilesReachSceneSpells(player);
        CheckRideAbandon(player);
    }

    void OnSceneTriggerEvent(Player* player, uint32 sceneInstanceID, std::string sceneEvent) override
    {
        if (player->GetMapId() != MAP_EXILES_REACH)
            return;

        if (sceneEvent == "Big Kill Credit")
        {
            player->CastSpell(player, SPELL_CADAVER_KILL_CREDIT);

            if (player->GetQuestObjectiveProgress(QUEST_H_THE_REDEATHER, 2) == 8)
            {
                player->CastSpell(player, SPELL_CADAVER_CREDIT_CHECK_CHOPPY_MALFUNCTION, true);
            }

            if (player->GetQuestObjectiveProgress(QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR, 2) == 8)
            {
                player->CastSpell(player, SPELL_GIANT_BOAR_PING_VEHICLE, true);
            }

            // The boar mowed the undead down: despawn the hostile mobs around
            // the rider so the trampled army actually vanishes (and stops
            // attacking). Tied to the reliable scene credit rather than the
            // trample's own grid search.
            std::list<Creature*> mobs;
            Trinity::AnyUnitInObjectRangeCheck u_check(player, 50.0f);
            Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(player, mobs, u_check);
            Cell::VisitGridObjects(player, searcher, 50.0f);
            for (Creature* mob : mobs)
            {
                if (!player->IsHostileTo(mob))
                    continue;
                if (mob->GetEntry() == NPC_OGRE_RUINS_TORGOK)
                    continue; // leave the boss for the fight
                // Track so the undead can be respawned if the quest is abandoned.
                s_mowedDownUndead.insert(mob->GetSpawnId());
                // Long respawn delay so the mowed-down undead stay dead
                // through the rest of the ride instead of popping back.
                mob->DespawnOrUnsummon(0s, 1h);
            }
        }

        if (sceneEvent == "Teleport")
        {
            // tele after zombie kill scene horde
            if (QuestAccepted(player, QUEST_H_THE_REDEATHER))
            {
                player->CastSpell(player, SPELL_CADAVER_KILL_CREDIT);
                player->RemoveAura(SPELL_CADAVER_CREDIT_CHECK_CHOPPY_MALFUNCTION);
                player->CastSpell(player, SPELL_ROUGH_LANDING, true);
            }

            //  tele out from hrun's barrow after scene
            if (QuestAccepted(player, QUEST_WHO_LURKS_IN_THE_PIT) || QuestAccepted(player, QUEST_H_WHO_LURKS_IN_THE_PIT))
            {
                player->CastSpell(player, SPELL_TELEPORT_AFTER_HRUN, true);
                if (player->HasAura(SPELL_BURNING_ESCAPE))
                    player->RemoveAura(SPELL_BURNING_ESCAPE);
            }

            SceneTemplate const* sceneTemplate = player->GetSceneMgr().GetSceneTemplateFromInstanceId(sceneInstanceID);
            if (!sceneTemplate)
                return;

            // Alliance player finished exile's reach
            if (sceneTemplate->SceneId == SCENE_FLY_TO_STORMWIND)
            {
                player->CastSpell(player, SPELL_STORMWIND_TELEPORT, true);
            }
            // Horde player finished exile's reach
            if (sceneTemplate->SceneId == SCENE_FLY_TO_ORGRIMMAR)
            {
                player->CastSpell(player, SPELL_ORGRIMMAR_TELEPORT, true);
            }
        }

        if (sceneEvent == "Knockback")
        {
            player->CastSpell(player, SPELL_GIANT_BOAR_RIDING_KNOCKBACK, true);
        }

        if (sceneEvent == "Trampling Time")
        {
            if (Creature* giantBoarVehicle = player->GetVehicleCreatureBase())
            {
                Position targetLoc = giantBoarVehicle->GetFirstCollisionPosition(20.f, 0.f);
                giantBoarVehicle->GetMotionMaster()->MoveCharge(targetLoc.GetPositionX(), targetLoc.GetPositionY(), targetLoc.GetPositionZ());
                player->CastSpell(player, SPELL_GIANT_BOAR_TRAMPLE, true);
            }
        }
    }

    void OnUpdateArea(Player* player, uint32 newArea, uint32 /*oldArea*/) override
    {
        // Entered Darkmaul Citadel Dungeon
        if (newArea == AREA_DARKMAUL_CITADEL_DUNGEON)
        {
            CancelExilesReachSceneAuras(player);
        }

        // Alliance welcome to Stormwind
        if (newArea == AREA_SW_VALLEY_OF_HEROES)
        {
            if (player->HasAura(SPELL_STORMWIND_TELEPORT))
            {
                player->CastSpell(player, SPELL_CREATE_SCENE_STORMWIND_TELEPORT, true);                
                player->CastSpell(player, SPELL_CREATE_SCENE_ARRIVED_TO_STORMWIND, true);
            }

            CancelExilesReachSceneAuras(player);
        }

        // Horde welcome to Orgrimmar
        if (newArea == AREA_OG_GATES_OF_ORGRIMMAR)
        {
            if (player->HasAura(SPELL_ORGRIMMAR_TELEPORT))
            {
                player->CastSpell(player, SPELL_CREATE_SCENE_OGRIMMAR_TELEPORT, true);
                // no need for other spells at horde side
            }

            CancelExilesReachSceneAuras(player);
        }
    }

    void OnQuestStatusChange(Player* player, uint32 questId) override
    {
        if (questId == QUEST_DUNGEON_DARKMAUL_CITADEL || questId == QUEST_H_DUNGEON_DARKMAUL_CITADEL)
            CancelExilesReachSceneAuras(player);

        if (questId == QUEST_AN_END_TO_BEGINNINGS || questId == QUEST_H_AN_END_TO_BEGINNINGS)
            CancelExilesReachSceneAuras(player);

        // Ride of the Scientifically Enhanced Boar: clear the mowed-down tracking
        // when the ride is completed (the mobs stay mowed down). Abandon is
        // detected in CheckRideAbandon (OnQuestStatusChange doesn't fire for
        // quest abandon).
        if (questId == QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR || questId == QUEST_H_THE_REDEATHER)
            if (player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
                s_mowedDownUndead.clear();

        if (questId == QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR && player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
        {
            player->CastSpell(player, SPELL_REUNION_SCENE_A, true);
            Conversation::CreateConversation(CONVERSATION_OGRE_RUINS_REUNION, player, *player, player->GetGUID(), nullptr);
        }
        else if (questId == QUEST_H_THE_REDEATHER && player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
            player->CastSpell(player, SPELL_REUNION_SCENE_H, true);

        // After the ride is handed in, the camp at the ogre ruins populates:
        // Jaina near Austin Huxworth by the rock edging (grants Stocking Up /
        // Repair Yourself), Captain Garrick by the campfire.
        if (questId == QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR && player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
        {
            if (!player->FindNearestCreature(NPC_OGRE_RUINS_POST_TORGOK_JAINA, 50.0f, true))
                if (Creature* jaina = player->SummonCreature(NPC_OGRE_RUINS_POST_TORGOK_JAINA,
                    Position(186.572f, -2289.42f, 81.6354f, 2.31301f),
                    TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                    jaina->SetNpcFlag(UNIT_NPC_FLAG_QUESTGIVER);

            if (!player->FindNearestCreature(NPC_OGRE_RUINS_POST_TORGOK_GARRICK, 50.0f, true))
                if (Creature* garrick = player->SummonCreature(NPC_OGRE_RUINS_POST_TORGOK_GARRICK,
                    Position(180.8f, -2286.0f, 81.6f, 0.0f),
                    TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                    garrick->SetNpcFlag(UNIT_NPC_FLAG_QUESTGIVER);
        }
    }
};

std::unordered_set<ObjectGuid::LowType> exiles_reach_general_playerscript::s_mowedDownUndead;
std::unordered_set<ObjectGuid> exiles_reach_general_playerscript::s_rideActivePlayers;

class map_exiles_reach : public WorldMapScript
{
public:
    map_exiles_reach() : WorldMapScript("map_exiles_reach", MAP_EXILES_REACH) { }

    void OnPlayerEnter(Map* /*map*/, Player* player) override
    {
        CancelExilesReachSceneAuras(player);
    }

    void OnPlayerLeave(Map* /*map*/, Player* player) override
    {
        CancelExilesReachSceneAuras(player);
    }
};

/* ## SPELL SCRIPTS > ## */

// ID - 325406 Necrotic Ritual
class spell_necrotic_ritual_325406 : public SpellScriptLoader
{
public:
    spell_necrotic_ritual_325406() : SpellScriptLoader("spell_necrotic_ritual_325406") {}

    class spell_necrotic_ritual_325406_SpellScript : public SpellScript
    {
        void SelectTarget(WorldObject*& target)
        {
            uint32 possibleTargets[3] =
            {
                NPC_QUILBOAR_BRIARPATCH_GREKOG,
                NPC_OGRE_RUINS_TORGOK,
                NPC_HRUNS_BARROW_HRUN
            };

            for (uint32 targetEntry : possibleTargets)
                if (Creature* creature = GetCaster()->FindNearestCreatureWithOptions(100.f, { .CreatureId = targetEntry, .IgnorePhases = true }))
                {
                    target = creature;
                    break;
                }
        }

        void Register() override
        {
            OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_necrotic_ritual_325406_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_NEARBY_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_necrotic_ritual_325406_SpellScript();
    }
};

// ID - 321690 Ritual Scene - Main Beam(DNT)
// ID - 321691 Ritual Scene - Harpy Beam (DNT)
// ID - 321692 Ritual Scene - Hrun Beam (DNT)
// ID - 321693 Ritual Scene - Ogre Citadel (DNT)
// ID - 312605 Ambience (DNT)
class PlaySceneWithDelay : public BasicEvent
{
public:
    PlaySceneWithDelay(Player* player, uint32 sceneId) : _player(player), _sceneId(sceneId) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        _player->GetSceneMgr().PlayScene(_sceneId, _player);

        return true;
    }

private:
    Player* _player;
    uint32 _sceneId;
};

class spell_exiles_reach_scene_spells : public SpellScriptLoader
{
public:
    spell_exiles_reach_scene_spells() : SpellScriptLoader("spell_exiles_reach_scene_spells") {}

    class spell_exiles_reach_scene_spells_AuraScript : public AuraScript
    {
        void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Player* player = caster->ToPlayer();
            if (!player)
                return;

            PreventDefaultAction();

            // This is the fix for black screen on login and login to scene window without ui and no cursor bugs.
            // (Should add scenes with delay after login (scene system issue))
            player->m_Events.AddEventAtOffset(new PlaySceneWithDelay(player, aurEff->GetMiscValue()), 5s);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_exiles_reach_scene_spells_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PLAY_SCENE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_exiles_reach_scene_spells_AuraScript();
    }
};

class DelayedTeleportToCapital : public BasicEvent
{
public:
    DelayedTeleportToCapital(Player* player, bool alliance) : _player(player), _alliance(alliance) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        if (_alliance)
        {
            _player->TeleportTo(0, -9062.75f, 436.2f, 93.06f, 0.66f, TELE_TO_TRANSPORT_TELEPORT);

        }
        else
        {
            _player->TeleportTo(1, 1458.66f, -4420.39f, 25.45f, 0.09f, TELE_TO_TRANSPORT_TELEPORT);
        }

        return true;
    }

private:
    Player* _player;
    bool _alliance;
};

// ID - 299565 Stormwind Teleport (from exile's reach)
class spell_exiles_reach_stormwind_teleport_299565 : public SpellScriptLoader
{
public:
    spell_exiles_reach_stormwind_teleport_299565() : SpellScriptLoader("spell_exiles_reach_stormwind_teleport_299565") {}

    class spell_exiles_reach_stormwind_teleport_299565_SpellScript : public SpellScript
    {
        void HandleOnCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Player* player = caster->ToPlayer();
            if (!player)
                return;

            player->m_Events.AddEventAtOffset(new DelayedTeleportToCapital(player, true), 3s);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_exiles_reach_stormwind_teleport_299565_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_exiles_reach_stormwind_teleport_299565_SpellScript();
    }
};

// ID - 326311 Orgrimmar Teleport (from exile's reach)
class spell_exiles_reach_orgrimmar_teleport_326311 : public SpellScriptLoader
{
public:
    spell_exiles_reach_orgrimmar_teleport_326311() : SpellScriptLoader("spell_exiles_reach_orgrimmar_teleport_326311") {}

    class spell_exiles_reach_orgrimmar_teleport_326311_SpellScript : public SpellScript
    {
        void HandleOnCast()
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            Player* player = caster->ToPlayer();
            if (!player)
                return;

            player->m_Events.AddEventAtOffset(new DelayedTeleportToCapital(player, false), 3s);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_exiles_reach_orgrimmar_teleport_326311_SpellScript::HandleOnCast);
        }

    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_exiles_reach_orgrimmar_teleport_326311_SpellScript();
    }
};

void AddSC_exiles_reach_general()
{
    // Player Scripts
    new exiles_reach_general_playerscript();
    // WorldMap Scripts
    new map_exiles_reach();
    // Spell Scripts
    new spell_necrotic_ritual_325406();
    new spell_exiles_reach_scene_spells();
    new spell_exiles_reach_stormwind_teleport_299565();
    new spell_exiles_reach_orgrimmar_teleport_326311();
}
