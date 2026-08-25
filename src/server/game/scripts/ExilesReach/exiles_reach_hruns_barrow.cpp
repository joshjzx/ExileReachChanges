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

class exiles_reach_hruns_barrow_playerscript : public PlayerScript
{
public:
    exiles_reach_hruns_barrow_playerscript() : PlayerScript("exiles_reach_hruns_barrow_playerscript") {}

    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_HRUNS_BARROW:
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
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, !player->HasAura(SPELL_RESIZER_SLAUGHTER) && !player->GetVehicle() && !player->GetQuestStatus(QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_RITUAL }, QuestsRewarded(player, { QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_H_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_QUEST_REWARDER }, QuestCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_CAMP_OBJECTS }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HRUNS_BARROW_CRENNA_RITUAL }, QuestCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_15330 }, QuestCompleted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_AT_HARPIES_WAY }, QuestRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_BREKA_BEFORE_DARKMAUL_CITADEL }, QuestRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_LANA_WONSA_STANDING_NEAR_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER) && QuestAcceptedOrCompleted(player, QUEST_H_WESTWARD_BOUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_LANA_WONSA_RUN_TO_CAVE }, !QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HRUNS_BARROW_LANA_WONSA_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_CRENNA_CAMP }, QuestRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SUB_ACTORS }, QuestCompletedOrRewarded(player, QUEST_H_NORTHBOUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_TOTEMS }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_HERBERT_CAMP }, QuestRewarded(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_BO_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_THE_RESCUE_OF_HERBERT_GLOOMBURST) && QuestCompletedOrRewarded(player, QUEST_H_NORTHBOUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_BREKA_CAMP }, QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));

                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_LANA_WONSA_RUN_TO_CAVE }, !QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_HRUNS_BARROW_HRUN }, !QuestRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));                    
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HRUNS_BARROW_CRENNA_RITUAL }, !QuestCompleted(player, QUEST_WHO_LURKS_IN_THE_PIT));                    
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_RITUAL }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HRUNS_BARROW_RALIA_RITUAL }, QuestCompleted(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13395 }, QuestCompleted(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_GARRICK_CAMP }, QuestCompleted(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_PERSONAL_ZOMBIE_PHASE }, player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_GIANT_BOAR_VEHICLE));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_QUEST_GIVER_AFTER_RITUAL }, QuestCompleted(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_GARRICK_CAMP }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_CAMP_SUB_ACTORS, PHASE_ALLIANCE_OGRE_RUINS_CAMP_OBJECTS }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13794 }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_KEELA_HENRY_AT_HARPIES_WAY }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_BJRON_ALARIA_STANDING_NEAR_CAMP }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR) && !QuestCompleted(player, QUEST_WESTWARD_BOUND) && !player->GetQuestStatus(QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_CAMP }, QuestRewarded(player, QUEST_MESSAGE_TO_BASE));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_15011 }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_GARRICK_BEFORE_DARKMAUL_CITADEL }, QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_MEREDY_KEELA_CAMP }, player->GetQuestStatus(QUEST_MESSAGE_TO_BASE));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13826 }, QuestRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_RALIA_CAMP }, QuestCompletedOrRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_ALARIA_BJRON_AFTER_PIT }, QuestCompletedOrRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_TOTEMS }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13836 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_13815 }, true);

                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_BJORN_ALARIA_RUNNING_TO_THE_CAVE }, QuestCompletedOrRewarded(player, QUEST_WESTWARD_BOUND) && !QuestCompletedOrRewarded(player, QUEST_H_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_HRUNS_BARROW_HRUN }, !QuestRewarded(player, QUEST_WHO_LURKS_IN_THE_PIT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HRUNS_BARROW_RALIA_RITUAL }, !QuestCompleted(player, QUEST_WHO_LURKS_IN_THE_PIT));                    
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
                case QUEST_H_WHO_LURKS_IN_THE_PIT:
                    activeSummons.push_back(NPC_HRUNS_BARROW_CRENNA_TRAVEL_FORM);
                    break;
                case QUEST_WHO_LURKS_IN_THE_PIT:
                    activeSummons.push_back(NPC_HRUNS_BARROW_RALIA_TRAVEL_FORM);
                    break;
                }

                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);
            }
        }
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
// Hrun the Exiled (Hruns Barrow)
class HrunsDeathEvent : public BasicEvent
{
public:
    HrunsDeathEvent(Creature* creature) : _creature(creature) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        // summon personal freed npc for all attackers @todo add for grekog
        for (auto const& itr : _creature->GetThreatManager().GetUnsortedThreatList())
        {
            Player* player = itr->GetVictim()->ToPlayer();
            if (!player)
                continue;

            if (player->GetQuestStatus(QUEST_WHO_LURKS_IN_THE_PIT))
                player->SummonCreature(NPC_HRUNS_BARROW_RALIA_TRAVEL_FORM, RescuedFromHrun, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());

            if (player->GetQuestStatus(QUEST_H_WHO_LURKS_IN_THE_PIT))
                player->SummonCreature(NPC_HRUNS_BARROW_CRENNA_TRAVEL_FORM, RescuedFromHrun, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());

            if (player->HasAura(SPELL_RITUAL_SCENE_HRUN_BEAM))
                player->RemoveAura(SPELL_RITUAL_SCENE_HRUN_BEAM);

            player->KilledMonsterCredit(NPC_HRUNS_BARROW_HRUN);
        }

        if (Creature* ralia = _creature->FindNearestCreatureWithOptions(100.f, { .CreatureId = NPC_HRUNS_BARROW_RALIA, .IgnorePhases = true }))
            ralia->DespawnOrUnsummon(0s, 2min);

        if (Creature* crenna = _creature->FindNearestCreatureWithOptions(100.f, { .CreatureId = NPC_HRUNS_BARROW_CRENNA, .IgnorePhases = true }))
            crenna->DespawnOrUnsummon(0s, 2min);

        _creature->DespawnOrUnsummon(10s, 2min);

        return true;
    }

private:
    Creature* _creature;
};

struct npc_exiles_reach_hruns_barrow_156900 : public ScriptedAI
{
    npc_exiles_reach_hruns_barrow_156900(Creature* creature) : ScriptedAI(creature)
    {
        me->SetEmoteState(EMOTE_STATE_READY_SPELL_OMNI);
        talked = false;
    }

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(EVENT_SPIRIT_BOLT, 2s);
        _events.ScheduleEvent(EVENT_DRAIN_SPIRIT, 6s);
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        me->SetEmoteState(EMOTE_ONESHOT_NONE);
        //Talk(0); // You will be kindling for the flame! @todo fix creature_text
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (me->HasAura(SPELL_FINAL_SACRIFICE_FIRE))
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_SPIRIT_BOLT:
                DoCastVictim(SPELL_SPIRIT_BOLT);
                _events.RescheduleEvent(EVENT_SPIRIT_BOLT, 4s);
                break;
            case EVENT_DRAIN_SPIRIT:
                DoCastVictim(SPELL_DRAIN_SPIRIT);
                _events.RescheduleEvent(EVENT_DRAIN_SPIRIT, 14s);
                break;
            default:
                break;
            }
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        Player* player = attacker->ToPlayer();

        if (damage > me->GetHealth())
        {
            if (player)
            {
                damage = 0;
                me->AttackStop();
                me->StopMoving();
                me->SetUninteractible(true);
                me->CastSpell(me, SPELL_FINAL_SACRIFICE_FIRE);

                me->m_Events.AddEventAtOffset(new HrunsDeathEvent(me), 6s);

                Talk(1); // If... I die... so... do you!
            }
            else
            {
                damage = 0;
            }
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (talked)
            return;

        if (QuestAccepted(player, QUEST_WHO_LURKS_IN_THE_PIT))
            player->PlayConversation(12120);

        if (QuestAccepted(player, QUEST_H_WHO_LURKS_IN_THE_PIT))
            player->PlayConversation(14540);

        talked = true;
    }

private:
    bool talked;
    EventMap _events;
    enum
    {
        EVENT_SPIRIT_BOLT = 1,
        EVENT_DRAIN_SPIRIT
    };
};

// Freed Expedition Member (Hruns Barrow)
struct npc_exiles_reach_hruns_barrow_167244_160964 : public ScriptedAI
{
    npc_exiles_reach_hruns_barrow_167244_160964(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        if (GameObject* cocoon = me->FindNearestGameObject(GAMEOBJECT_THICK_COCOON_A, 10.f))
            me->Relocate(cocoon);

        me->SetFacingToObject(player);
        me->AI()->Talk(urand(0, 3));

        // run to the entrance
        me->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, me, me->GetFirstCollisionPosition(50.f, me->GetAbsoluteAngle(Position(74.f, -2232.f, 67.f))), 0, false, true), 2s);
    }
};

// Ralia Dreamchaser, Crenna Earth-Daughter (After Ritual) (Hruns Barrow)
struct npc_exiles_reach_hruns_barrow_156929_167254 : public ScriptedAI
{
    npc_exiles_reach_hruns_barrow_156929_167254(Creature* creature) : ScriptedAI(creature) { }

    void IsSummonedBy(WorldObject* /*summoner*/) override
    {
        me->AI()->Talk(0);
        // You saved my life! Allow me to return the favor. Hop on!
        // Thank you, friend. I will bear us to safety. Hop on!
    }

    void OnSpellClick(Unit* clicker, bool /*result*/) override
    {
        Player* player = clicker->ToPlayer();
        if (!player)
            return;

        player->CastSpell(player, SPELL_TELEPORT_AFTER_HRUN, true);
        me->DespawnOrUnsummon();
    }
};

/* ## GAMEOBJECT SCRIPTS > ## */

// Thick Cocoon (Hruns Barrow)
struct go_exiles_reach_hruns_barrow_339568_350796 : public GameObjectAI
{
    go_exiles_reach_hruns_barrow_339568_350796(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        // horde
        if (player->GetQuestStatus(QUEST_H_WHO_LURKS_IN_THE_PIT))
        {
            player->KillCreditGO(GAMEOBJECT_THICK_COCOON_H);
            player->CastSpell(player, SPELL_FREED_EXPEDITION_MEMBER_H, true);
            me->DestroyForPlayer(player);
            return true;
        }

        // alli
        if (player->GetQuestStatus(QUEST_WHO_LURKS_IN_THE_PIT))
        {
            player->KillCreditGO(GAMEOBJECT_THICK_COCOON_A);
            player->CastSpell(player, SPELL_FREED_EXPEDITION_MEMBER_A, true);
            me->DestroyForPlayer(player);
            return true;
        }

        return false;
    }
};

void AddSC_exiles_reach_hruns_barrow()
{
    // Player Scripts
    new exiles_reach_hruns_barrow_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_hruns_barrow_156900);
    RegisterCreatureAI(npc_exiles_reach_hruns_barrow_167244_160964);
    RegisterCreatureAI(npc_exiles_reach_hruns_barrow_156929_167254);
    // GameObject Scripts
    RegisterGameObjectAI(go_exiles_reach_hruns_barrow_339568_350796);
}
