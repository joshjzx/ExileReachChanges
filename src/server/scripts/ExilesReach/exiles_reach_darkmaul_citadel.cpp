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
#include "Item.h"

class exiles_reach_darkmaul_citadel_playerscript : public PlayerScript
{
public:
    exiles_reach_darkmaul_citadel_playerscript() : PlayerScript("exiles_reach_darkmaul_citadel_playerscript") {}

    // for dancing in the ogre cooking area
    void OnTextEmote(Player* player, uint32 /*textEmote*/, uint32 emoteNum, ObjectGuid /*guid*/) override
    {
        if (player->GetMapId() == MAP_NPE && emoteNum == TEXT_EMOTE_DANCE 
        && (player->GetQuestStatus(QUEST_H_RIGHT_BENEATH_THEIR_EYES) || player->GetQuestStatus(QUEST_RIGHT_BENEATH_THEIR_EYES)))
        {
            AddCreditForQuestObjective(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_DANCING_AT_COOKING_PIT_H);
            AddCreditForQuestObjective(player, QUEST_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_DANCING_AT_COOKING_PIT_A);

            if (Creature* grunk = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_COOKING_AREA_GRUNK))
                grunk->SetEmoteState(EMOTE_STATE_DANCE);

            if (Creature* wug = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_COOKING_AREA_WUG))
                wug->SetEmoteState(EMOTE_STATE_DANCE);

            if (Creature* jugnug = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_COOKING_AREA_JUGNUG))
                jugnug->SetEmoteState(EMOTE_STATE_DANCE);
        }
    }
    
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_DARKMAUL_CITADEL:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_CITADEL_GORGROTH_AND_FRIENDS }, player->HasAura(SPELL_OGRE_TRANSFORM));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_CITADEL_OGRE_ADMIRALS }, !player->HasAura(SPELL_OGRE_TRANSFORM));

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_BRIDGE_HERBERT_CRENNA_SHUJA_BREKA_STANDING }, !player->HasAura(SPELL_SUMMON_PRISONERS_H) && player->GetQuestStatus(QUEST_H_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_BREKA_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_BREKA));
                    
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_MULGRIN_RITUAL }, !QuestCompletedOrRewarded(player, QUEST_H_CONTROLLING_THEIR_STONES));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_OGRE_RUNESTONES }, !QuestRewarded(player, QUEST_H_CONTROLLING_THEIR_STONES));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_GORGROTH_AND_FRIENDS_COOKING_AREA }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_HERBERT_CRENNA_SHUJA_BREKA_STANDING_AT_THE_ENTRANCE }, !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_BREKA_SUMMON) && !QuestCompletedOrRewarded(player, QUEST_H_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_MULGRIN_AFTER_SAVE }, QuestCompletedOrRewarded(player, QUEST_H_CONTROLLING_THEIR_STONES) && !QuestCompletedOrRewarded(player, QUEST_H_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_WYVERNS }, QuestCompletedOrRewarded(player, QUEST_H_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_FINAL_ACTORS }, QuestCompletedOrRewarded(player, QUEST_H_DUNGEON_DARKMAUL_CITADEL));
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_BRIDGE_GARRICK_HENRY_MEREDY_RALIA_STANDING }, !player->HasAura(SPELL_SUMMON_PRISONERS_A) && player->GetQuestStatus(QUEST_TO_DARKMAUL_CITADEL) && !player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_TORGOK_FIGHTER_A) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_BRIDGE_GARRICK));
                    
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_KEIRA_RITUAL }, !QuestCompletedOrRewarded(player, QUEST_CONTROLLING_THEIR_STONES));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_OGRE_RUNESTONES }, !QuestRewarded(player, QUEST_CONTROLLING_THEIR_STONES));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_13557 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_GORGROTH_AND_FRIENDS_SUMMON }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_GARRICK_HENRY_MEREDY_RALIA_STANDING_AT_THE_ENTRANCE }, !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_GARRICK_SUMMON) && !QuestCompletedOrRewarded(player, QUEST_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_KEIRA_AFTER_SAVE }, QuestCompletedOrRewarded(player, QUEST_CONTROLLING_THEIR_STONES) && !QuestCompletedOrRewarded(player, QUEST_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_GRYPHONS }, QuestCompletedOrRewarded(player, QUEST_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_FINAL_ACTORS }, QuestCompletedOrRewarded(player, QUEST_DUNGEON_DARKMAUL_CITADEL));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_15612 }, true);
                }
            }
            break;
            }
        }
    }

    void OnQuestStatusChange(Player* player, uint32 questId) override
    {
        CheckExilesReachPhaseShift(player);

        // Remove summons if player abandons or rewards a quest
        if (player->GetMapId() == MAP_NPE)
        {
            if (player->GetQuestStatus(questId) == QUEST_STATUS_NONE || player->GetQuestStatus(questId) == QUEST_STATUS_REWARDED)
            {
                std::vector<uint32> activeSummons;
                switch (questId)
                {
                case QUEST_H_RIGHT_BENEATH_THEIR_EYES:
                case QUEST_RIGHT_BENEATH_THEIR_EYES:
                    activeSummons.push_back(NPC_DARKMAUL_CITADEL_COOKING_AREA_GORGROTH);
                    activeSummons.push_back(NPC_DARKMAUL_CITADEL_COOKING_AREA_GRUNK);
                    activeSummons.push_back(NPC_DARKMAUL_CITADEL_COOKING_AREA_WUG);
                    activeSummons.push_back(NPC_DARKMAUL_CITADEL_COOKING_AREA_JUGNUG);
                    break;
                }

                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);
            }
        }
    }

    void OnSendNewItem(Player* player, Item* item) override
    {
        if (item->GetEntry() == ITEM_WARD_STONE_1 || item->GetEntry() == ITEM_WARD_STONE_2 || item->GetEntry() == ITEM_WARD_STONE_3)
            player->CastSpell(player, SPELL_OGRE_CREDIT, true);
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

// Ogre Brute, Ogre Shadowcaster, Ogre Chef (Darkmaul Citadel)
struct npc_exiles_reach_darkmaul_citadel_153239_153242_154103 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_citadel_153239_153242_154103(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        // One ogre has to be near DarkMaulCitadelStairsTriggerPosition
        if (player->IsInDist(DarkMaulCitadelStairsTriggerPosition, 30.0f))
        {
            if (!player->HasAura(SPELL_AROUND_FRONT_DOOR))
            {
                player->CastSpell(player, SPELL_AROUND_FRONT_DOOR, true);

                if (me->GetEntry() == NPC_DARKMAUL_CITADEL_OGRE_SHADOWCASTER && me->IsInDist(DarkMaulCitadelStairsTriggerPosition, 10.0f))
                    if (Creature* garrick = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_CITADEL_GARRICK_SUMMON))
                        StartPrivateConversation(player, CONVERSATION_REACHED_DARKMAUL_CITADEL_ENTRANCE, { me, garrick });
            }
        }

        // One ogre has to be near DarkMaulCitadelCookingAreaPosition
        if (player->IsInDist(DarkMaulCitadelCookingAreaPosition, 50.0f))
        {
            if (!player->HasAura(SPELL_AROUND_COOKING_AREA))
                player->CastSpell(player, SPELL_AROUND_COOKING_AREA, true);
            
            if (player->HasAura(SPELL_AROUND_FRONT_DOOR))
                player->RemoveAura(SPELL_AROUND_FRONT_DOOR);
        }
        else
        {
            if (player->HasAura(SPELL_AROUND_COOKING_AREA))
                player->RemoveAura(SPELL_AROUND_COOKING_AREA);
        }
    }

    void Reset() override
    {
        _events.Reset();

        _events.ScheduleEvent(EVENT_SPELL_CAST, 4s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;           
        
        if (me->HasAura(SPELL_COSMETIC_SLEEP))
            me->RemoveAura(SPELL_COSMETIC_SLEEP);
        
        if (me->HasAura(SPELL_COSMETIC_SLEEP2))
            me->RemoveAura(SPELL_COSMETIC_SLEEP2);

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_SPELL_CAST:
            {
                switch (me->GetEntry())
                {
                    case NPC_DARKMAUL_CITADEL_OGRE_CHEF:
                    case NPC_DARKMAUL_CITADEL_OGRE_SHADOWCASTER:
                        DoCastVictim(SPELL_OGRE_SHADOWBOLT);
                        break;
                    case NPC_DARKMAUL_CITADEL_OGRE_BRUTE:
                        DoCastVictim(SPELL_OGRE_SMASH);
                        break;
                }
                _events.RescheduleEvent(EVENT_SPELL_CAST, 8s);
            }
                break;
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    enum
    {
        EVENT_SPELL_CAST = 1,
    };
};

// Grunk, Wug, Jugnug (Darkmaul Citadel)
struct npc_exiles_reach_darkmaul_citadel_153581_153582_153583 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_citadel_153581_153582_153583(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();

        _events.ScheduleEvent(EVENT_SPELL_CAST, 3s);
        _events.ScheduleEvent(EVENT_SPELL_CAST_2, 3s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;
        
        if (me->HasAura(SPELL_COSMETIC_SLEEP))
            me->RemoveAura(SPELL_COSMETIC_SLEEP);
            
        if (me->HasAura(SPELL_COSMETIC_SLEEP2))
            me->RemoveAura(SPELL_COSMETIC_SLEEP2);

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_SPELL_CAST:
                if (me->GetEntry() == NPC_DARKMAUL_CITADEL_GRUNK)
                    DoCastVictim(SPELL_OGRE_SHADOWBOLT);
                _events.RescheduleEvent(EVENT_SPELL_CAST_2, 6s);
                break;
            case EVENT_SPELL_CAST_2:
            {
                switch (me->GetEntry())
                {
                    case NPC_DARKMAUL_CITADEL_GRUNK:
                        DoCastVictim(SPELL_GRUNK_DESECRATE);
                        break;
                    case NPC_DARKMAUL_CITADEL_WUG:
                        DoCastSelf(SPELL_WUG_ENRAGE);
                        break;
                    case NPC_DARKMAUL_CITADEL_JUGNUG:
                        DoCastVictim(SPELL_JUGNUG_DECAPITATE);
                        break;
                }
               _events.RescheduleEvent(EVENT_SPELL_CAST_2, 6s);
            }
                break;
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    enum
    {
        EVENT_SPELL_CAST = 1,
        EVENT_SPELL_CAST_2 = 2,
    };
};

// Gor'Groth (Darkmaul Citadel)
class OgresWalkOutEvent : public BasicEvent
{
public:
    OgresWalkOutEvent(Creature* creature) : _creature(creature) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        _creature->CastSpell(_creature, SPELL_LIGHTNING_DISSOLVE_OUT, true);

        _creature->DespawnOrUnsummon(15s);

        if (Creature* grunk = _creature->FindNearestCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_GRUNK, 50.f))
        {
            grunk->SetWalk(true);
            grunk->GetMotionMaster()->MovePoint(0, DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionX(), DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionY(), DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionZ());
            grunk->DespawnOrUnsummon(15s);
        }

        if (Creature* wug = _creature->FindNearestCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_WUG, 50.f))
        {
            wug->SetWalk(true);
            wug->GetMotionMaster()->MovePoint(0, DarkMaulCitadelCookingAreaWugWalkOutPosition.GetPositionX(), DarkMaulCitadelCookingAreaWugWalkOutPosition.GetPositionY(), DarkMaulCitadelCookingAreaWugWalkOutPosition.GetPositionZ());
            wug->DespawnOrUnsummon(15s);
        }

        if (Creature* jugnug = _creature->FindNearestCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_JUGNUG, 50.f))
        {
            jugnug->SetWalk(true);
            jugnug->GetMotionMaster()->MovePoint(0, DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionX(), DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionY(), DarkMaulCitadelCookingAreaJugnugGrunkWalkOutPosition.GetPositionZ());
            jugnug->DespawnOrUnsummon(15s);
        }

        return true;
    }

private:
    Creature* _creature;
};

struct npc_exiles_reach_darkmaul_citadel_153580 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_citadel_153580(Creature* creature) : ScriptedAI(creature) {}

    void ReceiveEmote(Player* player, uint32 emoteId) override
    {
        if (emoteId == TEXT_EMOTE_WAVE && (player->GetQuestStatus(QUEST_H_RIGHT_BENEATH_THEIR_EYES) || player->GetQuestStatus(QUEST_RIGHT_BENEATH_THEIR_EYES)))
        {
            StartPrivateConversation(player, CONVERSATION_WAVED_TO_GORGROTH, { me });
            AddCreditForQuestObjective(player, QUEST_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_WAVED_A);
            AddCreditForQuestObjective(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_WAVED_H);

            me->m_Events.AddEventAtOffset(new OgresWalkOutEvent(me), 20s);
        }
    }
};

// Invisible Stalker (Darkmaul Citadel)
struct npc_exiles_reach_darkmaul_citadel_161306 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_citadel_161306(Creature* creature) : ScriptedAI(creature) {}

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (!me->GetChannelSpellId())
            {
                me->SetChannelSpellId(SPELL_NECROTIC_RITUAL_CHANNEL);

                if (Creature* kelra = me->FindNearestCreature(NPC_DARKMAUL_CITADEL_KELRA, 100.f))
                    if (kelra->HasAura(SPELL_NECROTIC_RITUAL))
                        me->AddChannelObject(kelra->GetGUID());

                if (Creature* mulgrin = me->FindNearestCreature(NPC_DARKMAUL_CITADEL_MULGRIN, 100.f))
                    if (mulgrin->HasAura(SPELL_NECROTIC_RITUAL))
                        me->AddChannelObject(mulgrin->GetGUID());
            }

            timer = 1000;
        }
        else
            timer -= diff;
    }

private:
    uint32 timer = 1000;
};

// Captain Kelra, Warlord Mulgrin Thunderwalker (Darkmaul Citadel)
class ReachCitadelEntranceEvent : public BasicEvent
{
public:
    ReachCitadelEntranceEvent(Player* player) : _player(player) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        uint32 prisoners[8] = 
        {
            NPC_DARKMAUL_CITADEL_GARRICK_SUMMON, NPC_DARKMAUL_CITADEL_HENRY_SUMMON, NPC_DARKMAUL_CITADEL_RALIA_SUMMON, NPC_DARKMAUL_CITADEL_MEREDY_SUMMON,
            NPC_DARKMAUL_CITADEL_BREKA_SUMMON, NPC_DARKMAUL_CITADEL_SHUJA_SUMMON, NPC_DARKMAUL_CITADEL_HERBERT_SUMMON, NPC_DARKMAUL_CITADEL_CRENNA_SUMMON
        };

        _player->CastSpell(_player, SPELL_CANCEL_CHAINED, true);
        _player->CastSpell(_player, SPELL_CANCEL_MAINTAIN_SUMMONS_A, true); // only have effect on alliance

        if (_player->HasAura(SPELL_SUMMON_PRISONERS_H))
            _player->RemoveAura(SPELL_SUMMON_PRISONERS_H);

        if (_player->HasAura(SPELL_SUMMON_PRISONERS_A))
            _player->RemoveAura(SPELL_SUMMON_PRISONERS_A);

        if (_player->HasAura(SPELL_OGRE_DISGUISE))
            _player->RemoveAura(SPELL_OGRE_DISGUISE);

        _player->SetObjectScale(1.f);

        for (uint32 entry : prisoners)
        {
            // @todo better positioning
            if (Creature* summon = _player->GetSummonedCreatureByEntry(entry))
                summon->m_Events.AddEventAtOffset(new DelayedMoveEvent(_player, summon, summon->GetFirstCollisionPosition(50.f, 0.f), 0, false, true), 2s);
        }

        return true;
    }

private:
    Player* _player;
};

struct npc_exiles_reach_darkmaul_citadel_156954_167646 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_citadel_156954_167646(Creature* creature) : ScriptedAI(creature) {}

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (player->IsInDist(me, 50.f) && !QuestCompleted(player, QUEST_RIGHT_BENEATH_THEIR_EYES) && !QuestCompleted(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES) )
        {
            if (QuestAccepted(player, QUEST_RIGHT_BENEATH_THEIR_EYES))
            {
                //AddCreditForQuestObjective(player, QUEST_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACH_CITADEL);
                player->ForceCompleteQuest(QUEST_RIGHT_BENEATH_THEIR_EYES);
            }

            if (QuestAccepted(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES))
            {
                //AddCreditForQuestObjective(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACH_CITADEL);
                player->ForceCompleteQuest(QUEST_H_RIGHT_BENEATH_THEIR_EYES);
            }

            player->m_Events.AddEventAtOffset(new ReachCitadelEntranceEvent(player), 2s);
        }
    }
};

/* ## GAMEOBJECT SCRIPTS > ## */

// Catapult (Darkmaul Citadel)
class CatapultBoomEvent : public BasicEvent
{
public:
    CatapultBoomEvent(GameObject* go, int8 spellVisualPhase) : _go(go), _spellVisualPhase(spellVisualPhase) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        switch (_spellVisualPhase)
        {
            case 0:
            _go->SetSpellVisualId(100772);
                break;
            case 1:
            _go->SetSpellVisualId(100773);
                break;
            case 2:
            _go->SetSpellVisualId(86956);
                break;
        }

        return true;
    }

private:
    GameObject* _go;
    int8 _spellVisualPhase;
};
struct go_exiles_reach_catapult_351477_326651 : public GameObjectAI
{
    go_exiles_reach_catapult_351477_326651(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        player->KillCreditGO(351477);
        player->KillCreditGO(326651);
        me->SetGoState(GOState::GO_STATE_DESTROYED);
        me->SetWorldEffectID(2654);
        me->m_Events.AddEventAtOffset(new CatapultBoomEvent(me, 0), 1s);
        me->m_Events.AddEventAtOffset(new CatapultBoomEvent(me, 1), 3s);
        me->m_Events.AddEventAtOffset(new CatapultBoomEvent(me, 2), 4s);
        //me->SetSpawnTrackingStateAnimID(1776);
        me->DespawnOrUnsummon(1min, 1s);
        
        return true;
    }
};

// Ogre Runestones (Darkmaul Citadel)
struct go_exiles_reach_ogre_runestone_351476_339865 : public GameObjectAI
{
    go_exiles_reach_ogre_runestone_351476_339865(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        if ((QuestAccepted(player, QUEST_CONTROLLING_THEIR_STONES) || QuestAccepted(player, QUEST_H_CONTROLLING_THEIR_STONES))
            && player->HasItemCount(168599) && player->HasItemCount(168600) && player->HasItemCount(168601))
        {
            if (Creature* invisibleStalker = me->FindNearestCreature(NPC_DARKMAUL_CITADEL_INVISIBLE_STALKER, 5.f))
            {
                player->KillCreditGO(me->GetEntry());
                invisibleStalker->DespawnOrUnsummon(200ms, 60s);

                if (player->GetQuestObjectiveProgress(QUEST_CONTROLLING_THEIR_STONES, 4) == 3)
                {
                    player->ForceCompleteQuest(QUEST_CONTROLLING_THEIR_STONES);
                    player->RemoveAura(SPELL_RITUAL_SCENE_OGRE_CITADEL);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_CITADEL_MULGRIN_RITUAL }, false);
                }

                if (player->GetQuestObjectiveProgress(QUEST_H_CONTROLLING_THEIR_STONES, 4) == 3)
                {
                    player->ForceCompleteQuest(QUEST_H_CONTROLLING_THEIR_STONES);
                    player->RemoveAura(SPELL_RITUAL_SCENE_OGRE_CITADEL);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_CITADEL_KEIRA_RITUAL }, false);
                }

                return true;
            }
        }
        
        return false;
    }
};

/* ## SPELL SCRIPTS > ## */
// ID - 299483 Around Front Door (DNT)
class spell_around_front_door_299483 : public SpellScriptLoader
{
public:
    spell_around_front_door_299483() : SpellScriptLoader("spell_around_front_door_299483") {}

    class spell_around_front_door_299483_AuraScript : public AuraScript
    {
        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Player* player = caster->ToPlayer();
            if (!caster || !player)
                return;

            // Player entered darkmaul citadel entrance area (stairs)
            player->SummonCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_GORGROTH, DarkMaulCitadelCookingAreaGorgrothPosition, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            player->SummonCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_GRUNK, DarkMaulCitadelCookingAreaGrunkPosition, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            player->SummonCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_WUG, DarkMaulCitadelCookingAreaWugPosition, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
            player->SummonCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_JUGNUG, DarkMaulCitadelCookingAreaJugnugPosition, TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());

            AddCreditForQuestObjective(player, QUEST_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACHED_DARKMAUL_CITADEL_ENTRANCE);
            AddCreditForQuestObjective(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACHED_DARKMAUL_CITADEL_ENTRANCE);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_around_front_door_299483_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_around_front_door_299483_AuraScript();
    }
};

// ID - 299486 Around Cooking Area (DNT)
class spell_around_cooking_area_299486 : public SpellScriptLoader
{
public:
    spell_around_cooking_area_299486() : SpellScriptLoader("spell_around_cooking_area_299486") {}

    class spell_around_cooking_area_299486_AuraScript : public AuraScript
    {
        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Player* player = caster->ToPlayer();
            if (!caster || !player)
                return;

            // Player entered darkmaul citadel cooking area
            if (Creature* gorgroth = player->FindNearestCreature(NPC_DARKMAUL_CITADEL_COOKING_AREA_GORGROTH, 50.f))
                StartPrivateConversation(player, CONVERSATION_REACHED_COOKING_PIT, { gorgroth });

            AddCreditForQuestObjective(player, QUEST_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACHED_COOKING_PIT);
            AddCreditForQuestObjective(player, QUEST_H_RIGHT_BENEATH_THEIR_EYES, QUEST_OBJECTIVE_RIGHT_BENEATH_THEIR_EYES_REACHED_COOKING_PIT);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_around_cooking_area_299486_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_around_cooking_area_299486_AuraScript();
    }
};

void AddSC_exiles_reach_darkmaul_citadel()
{
    // Player Scripts
    new exiles_reach_darkmaul_citadel_playerscript();

    // Npc Scripts    
    RegisterCreatureAI(npc_exiles_reach_darkmaul_citadel_153239_153242_154103);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_citadel_153581_153582_153583);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_citadel_153580);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_citadel_156954_167646);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_citadel_161306);

    // Gameobject Scripts    
    RegisterGameObjectAI(go_exiles_reach_catapult_351477_326651);
    RegisterGameObjectAI(go_exiles_reach_ogre_runestone_351476_339865);

    // Spell Scripts
    new spell_around_front_door_299483();
    new spell_around_cooking_area_299486();
}
