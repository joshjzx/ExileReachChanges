/*
 * Copyright (c) WowCommunity - 2023
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

class exiles_reach_ogre_ruins_playerscript : public PlayerScript
{
public:
    exiles_reach_ogre_ruins_playerscript() : PlayerScript("exiles_reach_ogre_ruins_playerscript") {}

    void AfterScene(Player* player, uint32 sceneInstanceID)
    {
        SceneTemplate const* sceneTemplate = player->GetSceneMgr().GetSceneTemplateFromInstanceId(sceneInstanceID);
        if (!sceneTemplate)
            return;

        // tele to the scene position after reunion scene
        if (sceneTemplate->SceneId == SCENE_REUNION)
        {
            player->CastSpell(player, SPELL_TELEPORT_AFTER_REUNION_SCENE, true);
        }
    }

    void OnSceneCancel(Player* player, uint32 sceneInstanceID) override
    {
        AfterScene(player, sceneInstanceID);
    }

    void OnSceneComplete(Player* player, uint32 sceneInstanceID) override
    {
        AfterScene(player, sceneInstanceID);
    }
    
        
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            TC_LOG_ERROR("playerbot.v2", "[ExilesReach] phase check area={} rideRewarded={} toCitadel={}",
                player->GetAreaId(),
                QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR),
                QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL));
            switch (player->GetAreaId())
            {
            case AREA_OGRE_RUINS:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_MOBS }, true);
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_OGRE_RUINS_TORGOK }, !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER) && player->GetQuestObjectiveProgress(QUEST_H_THE_REDEATHER, 1) == 0 && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_13784 }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_HERBERT_AFTER_RITUAL }, true);
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_LIGHTSPAWN_AND_GLOBES_CORRUPTED }, !QuestCompletedOrRewarded(player, QUEST_FREEING_THE_LIGHT));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_LIGHTSPAWN_AND_GLOBES_SAVED, PHASE_GENERAL_DARKMAUL_PLAINS_13388 }, QuestCompletedOrRewarded(player, QUEST_FREEING_THE_LIGHT));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_HRUNS_BARROW_HRUN }, !QuestRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT) && !QuestRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_HRUNS_BARROW_CRENNA_RALIA_FREED_AT_THE_CAVE_PSUMMONS }, QuestRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT) || QuestRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, !player->HasAura(SPELL_RESIZER_SLAUGHTER) && !player->HasAura(SPELL_RIDING_GIANT_BOAR) && !player->GetQuestStatus(QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_RITUAL }, QuestsRewarded(player, { QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_H_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_QUEST_REWARDER }, QuestAcceptedOrCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_CAMP_OBJECTS }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HRUNS_BARROW_CRENNA_RITUAL }, QuestCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_15330 }, QuestCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_AT_HARPIES_WAY }, QuestRewarded(player, QUEST_H_THE_REDEATHER) && !QuestAcceptedOrCompleted(player, QUEST_H_THE_HARPY_PROBLEM) && !QuestRewarded(player, QUEST_H_THE_HARPY_PROBLEM));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_LANA_WONSA_STANDING_NEAR_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER) && !player->GetQuestStatus(QUEST_H_WESTWARD_BOUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_LANA_WONSA_RUN_TO_CAVE }, !QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HRUNS_BARROW_LANA_WONSA_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_CRENNA_CAMP }, QuestRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT) && !QuestAcceptedOrCompleted(player, QUEST_H_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SUB_ACTORS }, QuestCompletedOrRewarded(player, QUEST_H_NORTHBOUND) && !QuestAccepted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_TOTEMS }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_HERBERT_CAMP }, player->GetQuestStatus(QUEST_H_MESSAGE_TO_BASE) && !QuestAcceptedOrCompleted(player, QUEST_H_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_BO_CAMP }, player->GetQuestStatus(QUEST_H_MESSAGE_TO_BASE) && !QuestAcceptedOrCompleted(player, QUEST_H_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_BREKA_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER) && (!QuestRewarded(player, QUEST_H_MESSAGE_TO_BASE) || !QuestRewarded(player, QUEST_H_WESTWARD_BOUND)));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_BREKA_BEFORE_DARKMAUL_CITADEL }, QuestRewarded(player, QUEST_H_MESSAGE_TO_BASE) && QuestRewarded(player, QUEST_H_WESTWARD_BOUND) && !QuestAcceptedOrCompleted(player, QUEST_H_TO_DARKMAUL_CITADEL));
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_RITUAL }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HRUNS_BARROW_RALIA_RITUAL }, QuestCompleted(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13395 }, QuestCompleted(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_PERSONAL_ZOMBIE_PHASE }, player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_GIANT_BOAR_VEHICLE));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_QUEST_GIVER_AFTER_RITUAL }, QuestCompleted(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_CAMP_SUB_ACTORS, PHASE_ALLIANCE_OGRE_RUINS_CAMP_OBJECTS }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13794 }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_KEELA_HENRY_AT_HARPIES_WAY }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) && !QuestAcceptedOrCompleted(player, QUEST_THE_HARPY_PROBLEM) && !QuestRewarded(player, QUEST_THE_HARPY_PROBLEM));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_BJRON_ALARIA_STANDING_NEAR_CAMP }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) && !player->GetQuestStatus(QUEST_WESTWARD_BOUND) && !player->GetQuestStatus(QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_CAMP }, player->GetQuestStatus(QUEST_MESSAGE_TO_BASE) && !QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_15011 }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_MEREDY_KEELA_CAMP }, player->GetQuestStatus(QUEST_MESSAGE_TO_BASE) && !QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13826 }, QuestRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_RALIA_CAMP }, QuestCompletedOrRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT) && !QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_ALARIA_BJRON_AFTER_PIT }, QuestCompletedOrRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_TOTEMS }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13836 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13815 }, true);                    
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_GARRICK_CAMP }, (QuestAccepted(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) || QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR)) && (!QuestRewarded(player, QUEST_MESSAGE_TO_BASE) || !QuestRewarded(player, QUEST_WESTWARD_BOUND)));
                    TC_LOG_ERROR("playerbot.v2", "[ExilesReach] GARRICK_15017 add={}", (QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) && !QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL)));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_GARRICK_BEFORE_DARKMAUL_CITADEL }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) && !QuestAcceptedOrCompleted(player, QUEST_TO_DARKMAUL_CITADEL));
                }
            }
            break;
            }
        }
    }

    void OnQuestStatusChange(Player* player, uint32 questId) override
    {
        // Remove summons if player abandons or rewards a quest
        if (player->GetMapId() == MAP_NPE)
        {
            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE || player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
            {
                std::vector<uint32> activeSummons;
                switch (questId)
                {
                case QUEST_H_THE_REDEATHER:
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_SLAUGHTER);
                    activeSummons.push_back(NPC_OGRE_RUINS_TORGOK_FIGHTER_H);
                    break;
                case QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR:
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_GIANT_BOAR_VEHICLE);
                    activeSummons.push_back(NPC_OGRE_RUINS_TORGOK_FIGHTER_A);
                    break;
                case QUEST_H_TO_DARKMAUL_CITADEL:
                    activeSummons.push_back(NPC_OGRE_RUINS_BREKA_TO_DARKMAUL_CITADEL);
                    activeSummons.push_back(NPC_HARPY_ROOST_HERBERT_AFTER_RITUAL);
                    activeSummons.push_back(NPC_OGRE_RUINS_CRENNA_CAMP);
                    activeSummons.push_back(NPC_OGRE_RUINS_SHUJA_CAMP);  
                    break;
                }

                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);
            }
        }
    }

    void OnCriteriaUpdate(Player* player, Criteria const* criteria, CriteriaProgress const* /*progress*/) override
    {
        if (QuestAccepted(player, QUEST_STOCKING_UP_ON_SUPPLIES))
            if (criteria->Entry->ID == 47940 || criteria->Entry->ID == 48009) // item sold to vendor
                player->ForceCompleteQuest(QUEST_STOCKING_UP_ON_SUPPLIES);

        if (QuestAccepted(player, QUEST_H_STOCKING_UP_ON_SUPPLIES))
            if (criteria->Entry->ID == 47940 || criteria->Entry->ID == 48009) // item sold to vendor
                player->ForceCompleteQuest(QUEST_H_STOCKING_UP_ON_SUPPLIES);
    }

    // Phase Update in every 1sec
    void OnPlayerWorldObjectUpdate(Player* player, uint32 diff) override
    {
        if (timer <= diff)
        {
            CheckExilesReachPhaseShift(player);

            timer = 1000;
        }
        else
            timer -= diff;
    }

private:
    uint32 timer = 1000;
};

/* ## NPC SCRIPTS > ## */

// Breka Grimaxe, Captain Garrick (Ogre Ruins) (Fighting with Torgok) (good attacker ai must be used for all follower)
struct npc_exiles_reach_ogre_ruins_167145_156807 : public ScriptedAI
{
    npc_exiles_reach_ogre_ruins_167145_156807(Creature* creature) : ScriptedAI(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        if (QuestAccepted(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) || QuestAccepted(player, QUEST_H_THE_REDEATHER))
        {
            me->FollowTarget(summoner->ToUnit());
            me->SetOwnerGUID(summoner->GetGUID());
            me->SetFaction(FACTION_ESCORTEE_H_NEUTRAL_ACTIVE);
            me->SetBaseWeaponDamage(BASE_ATTACK, WeaponDamageRange(MAXDAMAGE), 0.1f);
            me->setAttackTimer(BASE_ATTACK, 2000);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!player)
                return;

            PhasingHandler::InheritPhaseShift(me, player);

            if (player->IsInCombat())
            {
                Unit* target = player->GetCombatManager().GetAnyTarget();
                if (!target)
                    return;

                if (target->IsAlive() && !me->IsInCombat() && player->IsInCombatWith(target) && me->AI()->CanAIAttack(target))
                    me->SetInCombatWith(target);
            }

            // @todo: if player completes quest the redeather go to shuja and convo

            timer = 1000;
        }
        else
            timer -= diff;

        if (!UpdateVictim())
            return;
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        // At alliance side the actors are fading after a few sec on retail.
        if (quest->GetQuestId() == QUEST_TO_DARKMAUL_CITADEL)
        {
            Talk(0, player);
            // Recruit, you're with us. The rest of you stay here while we head for the citadel.

            std::vector<uint32> summons = {me->GetEntry(), NPC_OGRE_RUINS_MEREDY_CAMP, NPC_OGRE_RUINS_RALIA_CAMP, NPC_OGRE_RUINS_HENRY_CAMP};

            me->m_Events.AddEventAtOffset(new DelayedMoveEventWithSummons(player, AlliPointToFadeToDarkMaulCitadelPath, summons, 10s), 10ms);
        }
    }

    void MovementInform(uint32 type, uint32 /*id*/) override
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (me->IsSummon() && me->GetWaypointPathId())
            me->GetMotionMaster()->MovePath(me->GetWaypointPathId(), false);
    }

private:
    uint32 timer = 1000;
};

// Torgok (Ogre Ruins)
struct npc_exiles_reach_ogre_ruins_162817 : public ScriptedAI
{
    npc_exiles_reach_ogre_ruins_162817(Creature* creature) : ScriptedAI(creature)
    {
        me->SetEmoteState(EMOTE_STATE_READY_SPELL_OMNI);
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_SOUL_GRASP, 2s);
        _events.ScheduleEvent(EVENT_SPIRIT_BOLT, 5s);
    }

    void JustEngagedWith(Unit* attacker) override // attack_started
    {
        Player* player = attacker->ToPlayer();
        if (!player)
            return;

        me->SetEmoteState(EMOTE_ONESHOT_NONE);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_SOUL_GRASP:
                if (me->GetVictim()->IsWithinCombatRange(me, 10.f))
                {
                    DoCast(me, SPELL_SOUL_GRASP);
                    _events.RescheduleEvent(EVENT_SOUL_GRASP, 20s);
                }
                else
                    _events.RescheduleEvent(EVENT_SOUL_GRASP, 3s);
                break;
            case EVENT_SPIRIT_BOLT:
                DoCastVictim(SPELL_SPIRIT_BOLT);
                _events.RescheduleEvent(EVENT_SPIRIT_BOLT, 10s);
                break;
            default:
                break;
            }
        }

    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        // Kill by player only
        if (damage > me->GetHealth() && attacker->IsCreature())
            damage = 0;
    }

    void JustDied(Unit* killer) override
    {
        Player* player = killer->ToPlayer();
        if (!player)
            return;

        // safeguard if the player don't meet the next quest level requirement
        if (player->GetLevel() < 6)
            player->GiveXP(player->GetXPForNextLevel(), player, 1);

        Talk(0);
    }

private:
    EventMap _events;
    enum
    {
        EVENT_SOUL_GRASP = 1,
        EVENT_SPIRIT_BOLT
    };
};

// Shuja Grimaxe (ritual variant & quest rewarder variant), Henry Garrick (ritual variant & quest rewarder variant)
struct npc_exiles_reach_ogre_ruins_167126_167128_156799 : public ScriptedAI
{
    npc_exiles_reach_ogre_ruins_167126_167128_156799(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (player->GetQuestObjectiveProgress(QUEST_H_THE_REDEATHER, 2) == 8 && player->GetQuestObjectiveProgress(QUEST_H_THE_REDEATHER, 1) == 0 && !player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_TORGOK_FIGHTER_H))
        {
            player->SummonCreature(NPC_OGRE_RUINS_TORGOK_FIGHTER_H, FindSummonPosition(player, NPC_OGRE_RUINS_TORGOK_FIGHTER_H), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
        }
    }
};

/* ## ACHIEVEMENT SCRIPTS > ## */

// Buy from vendor
class Exiles_Reach_AchievementScript : public AchievementScript
{
public:
    Exiles_Reach_AchievementScript() : AchievementScript("Exiles_Reach_AchievementScript") {}

    // @todo make a hook for criteria update and use that

    void OnCompleted(Player* player, AchievementEntry const* achievement) override
    {
        if (achievement->ID == ACHIEVEMENT_NPE_SOLD_ITEM)
        {
            if (QuestAccepted(player, QUEST_STOCKING_UP_ON_SUPPLIES))
                player->ForceCompleteQuest(QUEST_STOCKING_UP_ON_SUPPLIES);

            if (QuestAccepted(player, QUEST_H_STOCKING_UP_ON_SUPPLIES))
                player->ForceCompleteQuest(QUEST_H_STOCKING_UP_ON_SUPPLIES);
        }
    }
};

void AddSC_exiles_reach_ogre_ruins()
{
    // Player Scripts
    new exiles_reach_ogre_ruins_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_ogre_ruins_167145_156807);
    RegisterCreatureAI(npc_exiles_reach_ogre_ruins_162817);
    RegisterCreatureAI(npc_exiles_reach_ogre_ruins_167126_167128_156799);
    // Achievement Scripts
    new Exiles_Reach_AchievementScript();
}
