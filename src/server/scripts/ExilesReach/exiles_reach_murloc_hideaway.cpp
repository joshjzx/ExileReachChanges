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

class exiles_reach_murloc_hideaway_playerscript : public PlayerScript
{
public:
    exiles_reach_murloc_hideaway_playerscript() : PlayerScript("exiles_reach_murloc_hideaway_playerscript") {}

    // after the ship crash movie play the tiny murloc scene
    void OnMovieComplete(Player* player, uint32 movieId) override
    {
        if (movieId == 931 || movieId == 895)
        {
            player->GetSceneMgr().PlaySceneByPackageId(SCENE_ALI_HORDE_CRASHED_ON_ISLAND, SceneFlag::NotCancelable);
            if (player->HasAura(SPELL_HEAVY_FOG_AND_RAIN))
                player->RemoveAura(SPELL_HEAVY_FOG_AND_RAIN);
            player->CastSpell(player, SPELL_MEDIUM_FOG_AND_RAIN, true);
            player->CastSpell(player, SPELL_KNOCKED_DOWN, true);
        }
    }   

    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_MURLOC_HIDEAWAY:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_SHORE }, true);

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_SHORE_BREKA_AND_FRIENDS_INJURED }, !player->GetQuestStatus(QUEST_H_EMERGENCY_FIRST_AID));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_SHORE_BREKA_AND_FRIENDS_HEALED }, QuestRewarded(player, QUEST_H_EMERGENCY_FIRST_AID) && !QuestAccepted(player, QUEST_H_FINDING_THE_LOST_EXPEDITION) && !player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_BREKA));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_ABANDONED_CAMP_WONSA_STARVING }, !player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_BREKA));
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHORE_GARRICK_AND_FRIENDS_INJURED }, !player->GetQuestStatus(QUEST_EMERGENCY_FIRST_AID));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_13619 }, !QuestAccepted(player, QUEST_MURLOC_MANIA));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHORE_GARRICK_AND_FRIENDS_HEALED }, QuestRewarded(player, QUEST_EMERGENCY_FIRST_AID) && !QuestAccepted(player, QUEST_FINDING_THE_LOST_EXPEDITION) && !player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_GARRICK));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHORE_12940, PHASE_ALLIANCE_SHORE_13157, PHASE_ALLIANCE_SHORE_13503 }, QuestRewarded(player, QUEST_MURLOC_MANIA));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_ABANDONED_CAMP_ALARIA_STARVING }, !player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_GARRICK));

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
            case QUEST_EMERGENCY_FIRST_AID:
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_GARRICK);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_KEELA);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_BJORN);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_AUSTIN);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_COLE);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_RICHTER);
                break;
            case QUEST_H_EMERGENCY_FIRST_AID:
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_BREKA);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_BO);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_MITHDRAN);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_LANA);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_THROG);
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_JINHAKE);
                break;
            case QUEST_FINDING_THE_LOST_EXPEDITION:
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_GARRICK);
                break;
            case QUEST_H_FINDING_THE_LOST_EXPEDITION:
                activeSummons.push_back(NPC_MURLOC_HIDEAWAY_BREKA);
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

// Warlord Breka Grimaxe, Captain Garrick (murloc hideaway)
class FirstAidEvent : public BasicEvent
{
public:
    FirstAidEvent(Creature* creature, Creature* target) : _creature(creature), _target(target) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        _creature->CastSpell(_target, SPELL_FIRST_AID);

        return true;
    }

private:
    Creature* _creature;
    Creature* _target;
};

struct npc_exiles_reach_shore_166782_156626 : public ScriptedAI
{
    npc_exiles_reach_shore_166782_156626(Creature* creature) : ScriptedAI(creature)
    {
        greetPlayer = false;
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        switch (quest->GetQuestId())
        {
        case QUEST_MURLOC_MANIA:
        case QUEST_H_MURLOC_MANIA:
            Talk(1);
            // Recover our supplies, but don't fight too many murlocs at once. They're dangerous in packs.
            // Murlocs are dangerous in large numbers.Take back our supplies, but avoid fighting too many of them at once.
            break;
        case QUEST_H_EMERGENCY_FIRST_AID:
        case QUEST_EMERGENCY_FIRST_AID:
        {        
            std::list<Creature*> nearbyCreatures;
            player->GetCreatureListInGrid(nearbyCreatures, 200.f);
            
            if (Creature* captainClone = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                for (Creature* subactor : nearbyCreatures)
                {
                    switch (subactor->GetEntry())
                    {
                    case NPC_MURLOC_HIDEAWAY_BO:
                    case NPC_MURLOC_HIDEAWAY_MITHDRAN:
                    case NPC_MURLOC_HIDEAWAY_LANA:
                    case NPC_MURLOC_HIDEAWAY_KEELA:
                    case NPC_MURLOC_HIDEAWAY_AUSTIN:
                    case NPC_MURLOC_HIDEAWAY_BJORN:
                    case NPC_MURLOC_HIDEAWAY_THROG:
                    case NPC_MURLOC_HIDEAWAY_JINHAKE:
                    case NPC_MURLOC_HIDEAWAY_COLE:
                    case NPC_MURLOC_HIDEAWAY_RICHTER:
                        if (Creature* clone = player->SummonCreature(subactor->GetEntry(), subactor->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                        {
                            PhasingHandler::InheritPhaseShift(clone, player);

                            if (clone->GetEntry() == NPC_MURLOC_HIDEAWAY_COLE || clone->GetEntry() == NPC_MURLOC_HIDEAWAY_THROG)
                                captainClone->m_Events.AddEventAtOffset(new FirstAidEvent(captainClone, clone), 2s);

                            if (clone->GetEntry() == NPC_MURLOC_HIDEAWAY_JINHAKE || clone->GetEntry() == NPC_MURLOC_HIDEAWAY_RICHTER)
                                captainClone->m_Events.AddEventAtOffset(new FirstAidEvent(captainClone, clone), 8s);
                        }
                        break;
                    }
                }

                captainClone->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, captainClone, EmergencyFirstAidPositions[0], 0, false, false), 5s);
            }
        }
            break;
        case QUEST_FINDING_THE_LOST_EXPEDITION:
        case QUEST_H_FINDING_THE_LOST_EXPEDITION:
        {
            std::list<Creature*> nearbyCreatures;
            me->GetCreatureListInGrid(nearbyCreatures, 50.f);

            for (Creature* buddy : nearbyCreatures)
            {
                switch (buddy->GetEntry())
                {
                case NPC_MURLOC_HIDEAWAY_BO:
                case NPC_MURLOC_HIDEAWAY_MITHDRAN:
                case NPC_MURLOC_HIDEAWAY_LANA:
                case NPC_MURLOC_HIDEAWAY_KEELA_2:
                case NPC_MURLOC_HIDEAWAY_AUSTIN_2:
                case NPC_MURLOC_HIDEAWAY_BJORN_2:
                    if (Creature* clone = player->SummonCreature(buddy->GetEntry(), buddy->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                    {
                        clone->SetStandState(UNIT_STAND_STATE_STAND);
                        clone->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, clone, Position(-385.65f, -2594.42f, 3.2135f), 0, false, true), 3s);
                    }
                    break;
                }
            }
            if (Creature* clone = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                clone->CastSpell(player, SPELL_UPDATE_PHASESHIFT);
                PhasingHandler::InheritPhaseShift(clone, player);

                clone->FollowTarget(player);
                clone->AI()->Talk(2, player);
                // You three, search for more survivors. $n, you and I will push forward.
                // You three, scour the island for any sign of the expedition. $N, you and I will scout ahead.
            }
        }
        break;
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (!greetPlayer && (player->GetQuestStatus(QUEST_MURLOC_MANIA) == QUEST_STATUS_NONE && player->GetQuestStatus(QUEST_H_MURLOC_MANIA) == QUEST_STATUS_NONE))
        {
            Talk(0, player);
            // $n! You survived! We need to regroup at once.
            // $N!Thank the Light!Are you injured ?
            greetPlayer = true;
        }
    }

    void MovementInform(uint32 type, uint32 id) override
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 0) // reached the injured friend
        {
            me->m_Events.AddEventAtOffset(new DelayedMoveEvent(nullptr, me, EmergencyFirstAidPositions[1], 1, false, false), 5s);
        }
        else // reached back to start pos
        {
            me->SetNpcFlag(UNIT_NPC_FLAG_QUESTGIVER);
            me->SetFacingTo(me->GetAbsoluteAngle(EmergencyFirstAidPositions[0]), true);
        }
    }

private:
    bool greetPlayer;
};

// Bo 166786, Mithdran 166791, Lana 166796, Kee'la 156612, Austin 156610, Bjorn 156609 (murloc hideaway)
struct npc_exiles_reach_shore_injured_buddie : public ScriptedAI
{
    npc_exiles_reach_shore_injured_buddie(Creature* creature) : ScriptedAI(creature)
    {
        if (me->GetPhaseShift().HasPhase(PHASE_ALLIANCE_SHORE_GARRICK_AND_FRIENDS_INJURED) || me->GetPhaseShift().HasPhase(PHASE_HORDE_SHORE_BREKA_AND_FRIENDS_INJURED))
            me->SetEmoteState(EMOTE_ONESHOT_SLEEP);
    }

    void SpellHit(WorldObject* caster, SpellInfo const* spell) override
    {
        Player* player = caster->ToPlayer();
        if (!player)
            return;

        if (spell->Id != SPELL_FIRST_AID)
            return;

        Position pos;

        switch (me->GetEntry())
        {
        case NPC_MURLOC_HIDEAWAY_BO:
        case NPC_MURLOC_HIDEAWAY_KEELA:
            pos = EmergencyFirstAidPositions[2];
            break;
        case NPC_MURLOC_HIDEAWAY_MITHDRAN:
        case NPC_MURLOC_HIDEAWAY_AUSTIN:
            pos = EmergencyFirstAidPositions[3];
            break;
        case NPC_MURLOC_HIDEAWAY_LANA:
        case NPC_MURLOC_HIDEAWAY_BJORN:
            pos = EmergencyFirstAidPositions[4];
            break;
        }

        me->SetEmoteState(EMOTE_STATE_STAND);
        player->KilledMonsterCredit(me->GetEntry());        
        Talk(0, player); // various text e.g: My thanks. We'll find the expedition yet.
        me->m_Events.AddEventAtOffset(new DelayedMoveEvent(player, me, pos, 0, false, false), 3s);
    }
};

// Throg 166784, Jin'Hake 166800, Cole 149917, Richter 156622 (murloc hideaway)
struct npc_exiles_reach_shore_other_two_injured : public ScriptedAI
{
    npc_exiles_reach_shore_other_two_injured(Creature* creature) : ScriptedAI(creature) {}

    void SpellHit(WorldObject* /*caster*/, SpellInfo const* spell) override
    {
        if (spell->Id != SPELL_FIRST_AID)
            return;

        me->SetStandState(UNIT_STAND_STATE_STAND);
        me->m_Events.AddEventAtOffset(new DelayedMoveEvent(nullptr, me, EmergencyFirstAidPositions[5], 0, false, true), 3s);
    }
};

/* ## ITEM SCRIPTS > ## */

// First Aid Kit
class item_exiles_reach_first_aid_kit_168410 : public ItemScript
{
public:
    item_exiles_reach_first_aid_kit_168410() : ItemScript("item_first_aid_kit") {}

    bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& targets, ObjectGuid /*castId*/) override
    {
        Unit* target = targets.GetUnitTarget();
        if (!target)
            return false;

        std::vector<uint32> possibleTargetEntries = { 156609, 156612, 156610, 166786, 166791, 166796 };

        for (uint32 targetEntry : possibleTargetEntries)
        {
            if (target->GetEntry() == targetEntry)
                player->CastSpell(target, SPELL_FIRST_AID);
        }

        return true;
    }
};

void AddSC_exiles_reach_murloc_hideaway()
{
    // Player Scripts
    new exiles_reach_murloc_hideaway_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_shore_166782_156626);
    RegisterCreatureAI(npc_exiles_reach_shore_injured_buddie);
    RegisterCreatureAI(npc_exiles_reach_shore_other_two_injured);
    // Item Scripts
    new item_exiles_reach_first_aid_kit_168410();
}
