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
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

class exiles_reach_darkmaul_plains_playerscript : public PlayerScript
{
public:
    exiles_reach_darkmaul_plains_playerscript() : PlayerScript("exiles_reach_darkmaul_plains_playerscript") {}

    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_DARKMAUL_PLAINS:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_MOBS }, true);
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_LIGHTSPAWN_AND_GLOBES_CORRUPTED }, !QuestCompletedOrRewarded(player, QUEST_FREEING_THE_LIGHT));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_LIGHTSPAWN_AND_GLOBES_SAVED }, QuestCompletedOrRewarded(player, QUEST_FREEING_THE_LIGHT));
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_QUILBOAR_BRIARPATCH }, QuestCompletedOrRewarded(player, QUEST_H_THE_CHOPPY_BOOSTER_MK5) && !player->HasAura(SPELL_RIDING_SCOUT_O_MATIC));

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, !player->HasAura(SPELL_RESIZER_SLAUGHTER) && !player->GetVehicle()  && !player->GetQuestStatus(QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_OGRE_RUINS_TORGOK }, !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_MITHDRAN }, QuestCompletedOrRewarded(player, QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_BREKA }, QuestCompletedOrRewarded(player, QUEST_H_DOWN_WITH_THE_QUILBOAR));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_FIZZLEPOP }, QuestCompletedOrRewarded(player, QUEST_H_DOWN_WITH_THE_QUILBOAR) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_FIZZLEPOP) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_FIZZLEPOP_RESIZING_SUMMON));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_15355 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_HERBERT_RITUAL }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_HARPY_ROOST_HERBERT_AFTER_RITUAL }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_OGRE_RUINS_SHUJA_RITUAL }, QuestsRewarded(player, { QUEST_H_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_H_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_15319 }, QuestAccepted(player, QUEST_H_THE_CHOPPY_BOOSTER_MK5) && !QuestAccepted(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_FRIENDS }, QuestCompletedOrRewarded(player, QUEST_H_THE_CHOPPY_BOOSTER_MK5));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_DARKMAUL_PLAINS_15331 }, QuestAccepted(player, QUEST_H_THE_REDEATHER));
               }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_ZOMBIES_AND_CADAVERS }, QuestsRewarded(player, { QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR }) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_OGRE_RUINS_TORGOK }, !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_HARPY_ROOST_MEREDY_RITUAL }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_15276, PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_16917 }, player->GetQuestStatus(QUEST_DOWN_WITH_THE_QUILBOAR) != QUEST_STATUS_REWARDED);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_SPRINGSTOCK }, QuestCompletedOrRewarded(player, QUEST_DOWN_WITH_THE_QUILBOAR) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_SPRINGSTOCK) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_SPRINGSTOCK_RESIZING_SUMMON));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_GARRICK }, QuestCompletedOrRewarded(player, QUEST_DOWN_WITH_THE_QUILBOAR) && !QuestRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_AUSTIN }, QuestCompletedOrRewarded(player, QUEST_DOWN_WITH_THE_QUILBOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_13394 }, QuestCompleted(player, QUEST_DOWN_WITH_THE_QUILBOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_13780 }, QuestCompleted(player, QUEST_DOWN_WITH_THE_QUILBOAR));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_13443 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_15356 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_OGRE_RUINS_HENRY_RITUAL }, QuestsRewarded(player, {QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR}) && !QuestCompletedOrRewarded(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR));
                    WowCommunity::PhaseShift(player, { PHASE_GENERAL_DARKMAUL_PLAINS_13784 }, QuestsRewarded(player, {QUEST_FORBIDDEN_QUILBOAR_NECROMANCY, QUEST_DOWN_WITH_THE_QUILBOAR}) && !QuestCompletedOrRewarded(player, QUEST_H_THE_REDEATHER));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_13747 }, player->HasAura(SPELL_RIDING_SCOUT_O_MATIC));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_FRIENDS }, QuestCompletedOrRewarded(player, QUEST_THE_SCOUT_O_MATIC_5000));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_DARKMAUL_PLAINS_BIGBOAR }, QuestCompletedOrRewarded(player, QUEST_RE_SIZING_THE_SITUATION) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_WANDERING_BOAR_SUMMON) && !player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_GIANT_BOAR_VEHICLE) && !player->GetVehicle());
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
                case QUEST_H_THE_CHOPPY_BOOSTER_MK5:
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_SUMMONER);
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_OBSERVER);
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_FIZZLEPOP);
                    break;
                case QUEST_THE_SCOUT_O_MATIC_5000:
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_SCOUTOMATIC_SUMMONER);
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_SCOUTOMATIC_OBSERVER);
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_SPRINGSTOCK);
                    break;
                case QUEST_RE_SIZING_THE_SITUATION:
                    activeSummons.push_back(NPC_DARKMAUL_PLAINS_WANDERING_BOAR_SUMMON);
                    break;
                }

                for (uint32 entry : activeSummons)
                    if (player->GetSummonedCreatureByEntry(entry))
                        player->UnsummonCreatureByEntry(entry);

                if (questId == QUEST_RE_SIZING_THE_SITUATION)
                    player->CastSpell(player, SPELL_LINDIE_DESUMMON, true);

                if (questId == QUEST_H_RE_SIZING_THE_SITUATION)
                    player->RemoveAura(SPELL_H_RESIZING_BACKPACK);
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
// Cork Fizzlepop, Lindie Springstock (Darkmaul Plains)
struct npc_exiles_reach_darkmaul_plains_167019_149899 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_plains_167019_149899(Creature* creature) : ScriptedAI(creature)
    {
        ResetTalk();
    }

    void ResetTalk()
    {
        talk1 = false;
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        me->CastSpell(player, SPELL_UPDATE_PHASESHIFT);

        if (quest->GetQuestId() == QUEST_THE_SCOUT_O_MATIC_5000)
        {
            if (TempSummon* tempSpringstock = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                if (Creature* scoutOmatic = player->SummonCreature(NPC_DARKMAUL_PLAINS_SCOUTOMATIC_SUMMONER, CopterPositions[0], TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                {
                    scoutOmatic->SetObjectScale(0.2f);
                    scoutOmatic->SetUninteractible(true);

                    tempSpringstock->AI()->Talk(2, player); // I brought along a miniaturized Scout-o-Matic 5000! Let's just unshrink it...

                    player->GetScheduler().Schedule(7s, [tempSpringstock, scoutOmatic, player](TaskContext /*context*/)
                        { tempSpringstock->CastSpell(scoutOmatic, SPELL_RE_SIZING); });
                }
            }
        }

        if (quest->GetQuestId() == QUEST_H_THE_CHOPPY_BOOSTER_MK5)
        {
            if (TempSummon* tempFizzlepop = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                if (Creature* choppyBooster = player->SummonCreature(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_SUMMONER, CopterPositions[0], TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
                {
                    choppyBooster->SetObjectScale(0.2f);
                    choppyBooster->SetUninteractible(true);

                    tempFizzlepop->AI()->Talk(2, player); // I brought along a miniaturized Scout-o-Matic 5000! Let's just unshrink it...

                    player->GetScheduler().Schedule(7s, [tempFizzlepop, choppyBooster, player](TaskContext /*context*/)
                        { tempFizzlepop->CastSpell(choppyBooster, SPELL_RE_SIZING); });
                }
            }
        }

        if (quest->GetQuestId() == QUEST_H_RE_SIZING_THE_SITUATION)
        {
            player->CastSpell(player, SPELL_H_RESIZING_BACKPACK);
        }

        if (quest->GetQuestId() == QUEST_RE_SIZING_THE_SITUATION)
        {
            player->CastSpell(player, SPELL_A_RESIZING_BACKPACK);
        }

        if (quest->GetQuestId() == QUEST_H_THE_REDEATHER)
        {
            me->AI()->Talk(4);
            player->SummonCreature(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_SLAUGHTER, CopterPositions[0], TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID());
        }
    }

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        switch (me->GetEntry())
        {
        case NPC_DARKMAUL_PLAINS_FIZZLEPOP:
            if (!talk1 && player->GetQuestStatus(QUEST_THE_SCOUT_O_MATIC_5000) == QUEST_STATUS_NONE && !me->IsSummon())
            {
                me->AI()->Talk(8, player); // You must be Warlord Breka! Shuja's mother! She always said you'd save our hides.

                if (Creature* breka = me->FindNearestCreature(NPC_DARKMAUL_PLAINS_BREKA, 10.f))
                    player->GetScheduler().Schedule(7s, [breka, player](TaskContext /*context*/)
                        {
                            breka->AI()->Talk(0, player); // We're not safe just yet. Where are the others?
                        })
                    .Schedule(14s, [this, player](TaskContext /*context*/)
                        {
                            me->AI()->Talk(9, player); // No idea... but I've got a way to find out!
                        });

                        talk1 = true;
            }
            break;
        case NPC_DARKMAUL_PLAINS_SPRINGSTOCK:
            if (!talk1 && player->GetQuestStatus(QUEST_H_THE_CHOPPY_BOOSTER_MK5) == QUEST_STATUS_NONE && !me->IsSummon())
            {
                me->AI()->Talk(0, player); // You're Captain Garrick! Henry's mother! He always said you'd come for us!

                if (Creature* garrick = me->FindNearestCreatureWithOptions(10.f, { .CreatureId = NPC_DARKMAUL_PLAINS_GARRICK, .IgnorePhases = true }))
                    player->GetScheduler().Schedule(7s, [garrick, player](TaskContext /*context*/)
                        {
                            garrick->AI()->Talk(5, player); // Well, he was right. Now, where's the rest of the expedition?
                        })
                    .Schedule(14s, [this, player](TaskContext /*context*/)
                        {
                            me->AI()->Talk(1, player); // I'm not sure, but I know how we can find out.
                        });

                        talk1 = true;
            }
            break;
        }
    }

private:
    bool talk1;
};

// Choppy Booster, Scout-O-Matic (Darkmaul Plains) (for observing zombies)
struct npc_exiles_reach_darkmaul_plains_167905_156526 : public VehicleAI
{
    npc_exiles_reach_darkmaul_plains_167905_156526(Creature* creature) : VehicleAI(creature)
    {
        timer = 1000;
        scenePlayed = false;
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
            {
                if (Player* player = passenger->ToPlayer())
                {
                    if (!player->HasAura(SPELL_ROPED))
                        me->CastSpell(me, SPELL_ROPED);

                    // We arrived to the ritual
                    if (me->GetPosition().IsInDist(CopterPositions[1], 1.f) && !scenePlayed)
                    {

                        player->CastSpell(player, SPELL_GORGOTH_SCENE_KILL_CREDIT);

                        if (player->GetTeam() == ALLIANCE)
                            player->CastSpell(player, SPELL_GORGROTH_SCENE_A);
                        else
                            player->CastSpell(player, SPELL_GORGROTH_SCENE_H);

                        scenePlayed = true;
                    }

                    // We going back from the ritual
                    if (me->GetAreaId() == AREA_OGRE_RUINS && !player->HasAura(SPELL_GORGROTH_SCENE_H) && !player->HasAura(SPELL_GORGROTH_SCENE_A) && scenePlayed)
                    {
                        me->GetMotionMaster()->MovePoint(1, CopterPositions[0], true);

                        if (player->GetTeam() == ALLIANCE)
                            player->PlayConversation(12078);
                        else
                            player->PlayConversation(14520);
                    }

                    // We are at the start point again
                    if (me->GetPosition().IsInDist(CopterPositions[0], 1.f))
                    {
                        player->RemoveAura(SPELL_RIDING_SCOUT_O_MATIC);
                        player->ExitVehicle();
                        // next line will avoid player to leave vehicle under map
                        player->TeleportTo(player->GetMapId(), CopterPositions[2].GetPositionX(), CopterPositions[2].GetPositionY(), CopterPositions[2].GetPositionZ(), player->GetOrientation());
                    }
                }
            }

            timer = 1000;
        }
        else
            timer -= diff;
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            if (Creature* scoutOmatic = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_SCOUTOMATIC_SUMMONER))
                scoutOmatic->DespawnOrUnsummon();

            if (Creature* choppyBooster = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_CHOPPYBOOSTER_SUMMONER))
                choppyBooster->DespawnOrUnsummon();

            if (me->GetAreaId() == AREA_DARKMAUL_PLAINS)
            {
                player->CastSpell(me, SPELL_ENTER_VEHICLE, true);
                me->GetMotionMaster()->MovePoint(1, CopterPositions[1], true);

                if (player->GetTeam() == ALLIANCE)
                    player->PlayConversation(12083);
                else
                    player->PlayConversation(14517);
            }
        }
    }

private:
    uint32 timer;
    bool scenePlayed;
};

// Giant Boar (summoned vehicle) (Darkmaul Plains)
struct npc_exiles_reach_darkmaul_plains_156267 : public VehicleAI
{
    npc_exiles_reach_darkmaul_plains_156267(Creature* creature) : VehicleAI(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Player* player = summoner->ToPlayer())
        {
            if (me->GetAreaId() == AREA_DARKMAUL_PLAINS)
            {
                if (Creature* garrick = player->GetSummonedCreatureByEntry(NPC_OGRE_RUINS_TORGOK_FIGHTER_A))
                {
                    garrick->EnterVehicle(me, 1);
                }
            }
        }
    }
};

// Cork Fizzlepop, Lindie Springstock (Darkmaul Plains) (Re-Sizing the Situation)
struct npc_exiles_reach_darkmaul_plains_167915_156749 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_plains_167915_156749(Creature* creature) : ScriptedAI(creature)
    {
        me->SetFaction(35); // just in case
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        player->PlayConversation(12086);
        me->FollowTarget(player);
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (me->GetPosition().IsInDist(ResizingTheSituationPositions[1], 1.f))
                me->CastSpell(me->GetCharmerOrOwnerPlayerOrPlayerItself(), SPELL_LINDIE_DESUMMON, true);
        }
        else
            timer -= diff;
    }

private:
    uint32 timer = 1000;
};

// Wandering Boar (Darkmaul Plains) (Re-Sizing the Situation)
struct npc_exiles_reach_darkmaul_plains_156736_156716 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_plains_156736_156716(Creature* creature) : ScriptedAI(creature) {}

    void Reset() override
    {
        _events.Reset();
        _events.ScheduleEvent(1, 3s);
    }

    void JustEngagedWith(Unit* who) override
    {
        me->CastSpell(who, SPELL_WANDERING_BOAR_MOB_CHARGE);
    }

    // creature shouldn't die at resizing the situation quest otherwise killcredit doubled
    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        if (Player* player = attacker->ToPlayer())
        {
            if (damage > me->GetHealth() && (player->GetQuestStatus(QUEST_RE_SIZING_THE_SITUATION) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_H_RE_SIZING_THE_SITUATION) == QUEST_STATUS_INCOMPLETE))
            {
                damage = 0;
                me->DespawnOrUnsummon();
            }
        }
    }

    void IsSummonedBy(WorldObject* summoner) override
    {
        Player* player = summoner->ToPlayer();
        if (!player)
            return;

        me->CastSpell(me, SPELL_GROW_1);
        me->GetMotionMaster()->MovePoint(0, ResizingTheSituationPositions[0].GetPositionX(), ResizingTheSituationPositions[0].GetPositionY(), ResizingTheSituationPositions[0].GetPositionZ(), true, 0.7f);
        player->KilledMonsterCredit(156722);

        if (Creature* springstock = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_SPRINGSTOCK_RESIZING_SUMMON))
            springstock->GetMotionMaster()->MovePoint(0, ResizingTheSituationPositions[1].GetPositionX(), ResizingTheSituationPositions[1].GetPositionY(), ResizingTheSituationPositions[1].GetPositionZ(), true, 4.f);

        if (Creature* fizzlepop = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_FIZZLEPOP_RESIZING_SUMMON))
            fizzlepop->GetMotionMaster()->MovePoint(0, ResizingTheSituationPositions[1].GetPositionX(), ResizingTheSituationPositions[1].GetPositionY(), ResizingTheSituationPositions[1].GetPositionZ(), true, 4.f);
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (me->GetPosition().IsInDist(ResizingTheSituationPositions[0], 1.f))
                me->DespawnOrUnsummon();
        }
        else
            timer -= diff;

        if (!UpdateVictim())
            return;

        _events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case 1:
                DoCastVictim(SPELL_WANDERING_BOAR_MOB_GORE);
                _events.Repeat(10s);
                break;
            default:
                break;
            }
        }
    }

private:
    EventMap _events;
    uint32 timer = 1000;
};

// Lightspawn (Darkmaul Plains)
struct npc_exiles_reach_darkmaul_plains_157114 : public ScriptedAI
{
    npc_exiles_reach_darkmaul_plains_157114(Creature* creature) : ScriptedAI(creature)
    {
        if (me->GetPhaseShift().HasPhase(PHASE_GENERAL_DARKMAUL_PLAINS_LIGHTSPAWN_AND_GLOBES_SAVED))
            me->RemoveAura(SPELL_LIGHTSPAWN_BOUND);
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_FREEING_THE_LIGHT)
        {
            // Dispel the foul magic that binds me here. 
            me->Say(179206);
            me->PlayDirectSound(153065, player, 179206);

            player->CastSpell(player, SPELL_LIGHTSPAWN_LIGHTSBOON, true);
        }
    }

    void OnQuestReward(Player* player, Quest const* quest, LootItemType /*type*/, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_FREEING_THE_LIGHT)
        {
            // Accept my blessing, and end the necromancy that plagues this island. 
            me->Say(179243);
            me->PlayDirectSound(153071, player, 179243);

            if (GameObject* pedestal1 = me->FindNearestGameObject(326716, 50.0f))
                pedestal1->AI()->Reset();

            player->RemoveAura(SPELL_LIGHTSPAWN_LIGHTSBOON);
        }
    }

};

/* ## GAMEOBJECT SCRIPTS > ## */

// Ritual Predestal (Darkmaul Plains)
struct go_exiles_reach_darkmaul_plains_326716_326717_326718_326719 : public GameObjectAI
{
    go_exiles_reach_darkmaul_plains_326716_326717_326718_326719(GameObject* go) : GameObjectAI(go) {}

    void Reset() override
    {
        if (Creature* InvisBunny_OnPedestal = me->FindNearestCreature(NPC_LIGHTSPAWN_PEDESTAL_INVISBUNNY, 2.0f))
        {
            InvisBunny_OnPedestal->RemoveAllAuras();
            InvisBunny_OnPedestal->CastSpell(InvisBunny_OnPedestal, SPELL_LIGHTSPAWN_NECROTICBALL, true);
        }
    }

    bool OnGossipHello(Player* player) override
    {
        if (!player->IsActiveQuest(QUEST_FREEING_THE_LIGHT))
            return false;

        player->KillCreditGO(me->GetEntry());

        if (Creature* InvisBunny_OnPedestal = player->FindNearestCreature(NPC_LIGHTSPAWN_PEDESTAL_INVISBUNNY, 10.0f))
        {
            InvisBunny_OnPedestal->RemoveAllAuras();
            InvisBunny_OnPedestal->CastSpell(InvisBunny_OnPedestal, SPELL_LIGHTSPAWN_LIGHTBALL, true);

            InvisBunny_OnPedestal->DespawnOrUnsummon(120s, 5s); // despawn after 2min and respawn after 5s
        }

        if (Creature* LightSpawn = player->FindNearestCreature(157114, 50.0f))
        {
            if (player->GetQuestObjectiveProgress(QUEST_FREEING_THE_LIGHT, 1) == 1)
            {
                // I attempted to bring an end to these rituals... 
                LightSpawn->Say(179202);
                LightSpawn->PlayDirectSound(153066, player, 179202);
            }
            if (player->GetQuestObjectiveProgress(QUEST_FREEING_THE_LIGHT, 2) == 1)
            {
                // But the ogres ensnared me... drained my power... 
                LightSpawn->Say(179203);
                LightSpawn->PlayDirectSound(153067, player, 179203);
            }
            if (player->GetQuestObjectiveProgress(QUEST_FREEING_THE_LIGHT, 3) == 1)
            {
                // Free me, so I might recover... and pass some of my power to you.  
                LightSpawn->Say(179204);
                LightSpawn->PlayDirectSound(153068, player, 179204);
            }
            if (player->GetQuestObjectiveProgress(QUEST_FREEING_THE_LIGHT, 4) == 1)
            {
                player->CastSpell(player, SPELL_LIGHTSPAWN_FREED, true);
                // The spell is weakening... the Light returns to me!  
                LightSpawn->Say(179205);
                LightSpawn->PlayDirectSound(153069, player, 179205);
            }
        }

        return true;
    }
};

/* ## SPELL SCRIPTS > ## */

// ID - 313269 Re-Sizing
class spell_re_sizing_313269 : public SpellScriptLoader
{
public:
    spell_re_sizing_313269() : SpellScriptLoader("spell_re_sizing_313269") {}

    class spell_re_sizing_313269_AuraScript : public AuraScript
    {
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            Creature* creature = caster->ToCreature();
            if (!caster || !target)
                return;

            Creature* smallCopter = target->ToCreature();
            if (!smallCopter)
                return;

            tickCount = 0;

            smallCopter->CastSpell(smallCopter, SPELL_GROW_1);
            smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_1, smallCopter->GetGUID());

            if (creature)
            {
                if (creature->GetEntry() == NPC_DARKMAUL_PLAINS_SPRINGSTOCK)
                {
                    creature->AI()->Talk(3); // Whoops, too big! A slight adjustment, and...
                }
                if (creature->GetEntry() == NPC_DARKMAUL_PLAINS_FIZZLEPOP)
                {
                    creature->AI()->Talk(1); // Okay, might have overdone it a bit. Hold on...
                }
            }
        }

        void PeriodicTick(AuraEffect const* /*aurEff*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            if (!caster || !target)
                return;

            Creature* smallCopter = target->ToCreature();
            if (!smallCopter)
                return;

            switch (tickCount)
            {
            case 0:
                // 3s delay
                break;
            case 1:
                for (int8 i = 0; i != 7; i++)
                {
                    smallCopter->GetScheduler().Schedule(i * 600ms, [smallCopter, i](TaskContext /*context*/)
                        {
                            switch (i) // 600ms counter in the 3sec periodic effect
                            {
                            case 0:
                                smallCopter->RemoveAura(SPELL_GROW_1);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            case 1:
                                smallCopter->CastSpell(smallCopter, SPELL_GROW_2, true);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            case 2:
                                smallCopter->RemoveAura(SPELL_GROW_2);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            case 3:
                                smallCopter->CastSpell(smallCopter, SPELL_GROW_3, true);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                smallCopter->SetObjectScale(1.f);
                                break;
                            case 4:
                                smallCopter->RemoveAura(SPELL_GROW_3);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            case 5:
                                smallCopter->CastSpell(smallCopter, SPELL_GROW_1, true);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            case 6:
                                smallCopter->RemoveAura(SPELL_GROW_1);
                                smallCopter->PlayObjectSound(SOUND_COPTER_SIZING_0, smallCopter->GetGUID());
                                break;
                            } });
                }
                break;
            case 2:
                // 3s delay
                break;
            case 3:
                smallCopter->RemoveAura(GetSpellInfo()->Id);
                smallCopter->SetUninteractible(false);
                break;
            }

            tickCount++;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (Creature* creature = caster->ToCreature())
            {
                if (creature->GetEntry() == NPC_DARKMAUL_PLAINS_SPRINGSTOCK)
                {
                    creature->AI()->Talk(4); // There! Just right. Give it a try!
                }
                if (creature->GetEntry() == NPC_DARKMAUL_PLAINS_FIZZLEPOP)
                {
                    creature->AI()->Talk(2); // Perfect! It ain't pretty, but it gets the job done.
                }
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_re_sizing_313269_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_re_sizing_313269_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            OnEffectRemove += AuraEffectRemoveFn(spell_re_sizing_313269_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }

    private:
        int8 tickCount;
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_re_sizing_313269_AuraScript();
    }
};

// ID - 305716 Re-Sizing
class spell_re_sizing_305716 : public SpellScriptLoader
{
public:
    spell_re_sizing_305716() : SpellScriptLoader("spell_re_sizing_305716") {}

    class spell_re_sizing_305716_AuraScript : public AuraScript
    {
        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            Creature* creature = caster->ToCreature();
            if (!caster || !target || !target->IsCreature())
                return;

            if (creature->GetEntry() != NPC_DARKMAUL_PLAINS_WANDERING_BOAR)
                return;
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetUnitOwner();
            Player* player = caster->ToPlayer();
            Creature* creature = target->ToCreature();
            if (!caster || !target || !player || !creature)
                return;

            caster->ClearChannelObjects();
            caster->SetChannelSpellId(0);
            caster->SetChannelVisual({});

            uint32 hitVariants[3] = { SPELL_RESIZER_HIT_TOSMALL, SPELL_RESIZER_HIT_EXPLODE, SPELL_RESIZER_OVERCHARGE };
            uint32 selectedFailedSpell = hitVariants[rand() % 3];

            if (player->GetQuestStatus(QUEST_RE_SIZING_THE_SITUATION) || player->GetQuestStatus(QUEST_H_RE_SIZING_THE_SITUATION))
            {
                if (player->GetQuestObjectiveProgress(QUEST_RE_SIZING_THE_SITUATION, 0) < 2)
                {
                    player->CastSpell(target, selectedFailedSpell, true);
                }
                else
                {
                    player->CastSpell(target, SPELL_RESIZER_SPARKS_AND_BOAR_SUMMON, true);
                    creature->DespawnOrUnsummon();
                }
            }
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_re_sizing_305716_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_re_sizing_305716_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_re_sizing_305716_AuraScript();
    }

    class spell_re_sizing_305716_SpellScript : public SpellScript
    {
        void HandleCast()
        {
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            Creature* creature = target->ToCreature();
            if (!caster || !target || !target->IsCreature())
                return;

            if (creature->GetEntry() != NPC_DARKMAUL_PLAINS_WANDERING_BOAR)
                return;

            GetSpell()->SendChannelStart(3000);
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_re_sizing_305716_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_re_sizing_305716_SpellScript();
    }
};

// ID - 305742 Resizer Hit
class spell_resizer_hit_305742 : public SpellScriptLoader
{
public:
    spell_resizer_hit_305742() : SpellScriptLoader("spell_resizer_hit_305742") {}

    class spell_resizer_hit_305742_SpellScript : public SpellScript
    {
        void HandleSummon(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            if (!caster)
                return;

            if (caster->GetSummonedCreatureByEntry(GetSpellInfo()->GetEffect(effIndex).MiscValue))
                PreventHitDefaultEffect(effIndex);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_resizer_hit_305742_SpellScript::HandleSummon, EFFECT_0, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_resizer_hit_305742_SpellScript();
    }
};

// ID - 325346 Re-Sizing
class spell_re_sizing_325346 : public SpellScriptLoader
{
public:
    spell_re_sizing_325346() : SpellScriptLoader("spell_re_sizing_325346") {}

    class spell_re_sizing_325346_SpellScript : public SpellScript
    {
        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            Unit* target = GetExplTargetUnit();
            if (!caster || !target || !target->IsCreature())
                return;

            caster->CastSpell(target, SPELL_RE_SIZING_2, true);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_re_sizing_325346_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_re_sizing_325346_SpellScript();
    }
};

// ID - 305068 Riding Giant Boar
class spell_riding_giant_boar_305068 : public SpellScriptLoader
{
public:
    spell_riding_giant_boar_305068() : SpellScriptLoader("spell_riding_giant_boar_305068") {}

    class spell_riding_giant_boar_305068_AuraScript : public AuraScript
    {
        void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            if (!owner)
                return;

            // Riding the giant boar: the undead chew on the boar, not the rider.
            // Block NPC attacks on the player (the trample/charge already pull
            // threat onto the boar, so remaining direct hits shouldn't reach us).
            owner->SetUnitFlag(UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* owner = GetUnitOwner();
            Player* player = owner->ToPlayer();
            if (!owner || !player)
                return;

            owner->RemoveUnitFlag(UNIT_FLAG_IMMUNE_TO_NPC);
            player->RemoveAura(SPELL_RIDING_GIANT_BOAR2);
            player->CastSpell(player, SPELL_KNOCKBACK_FOR_HACKFIX_GIANT_BOAR_LEAVE, true);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_riding_giant_boar_305068_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
            OnEffectApply += AuraEffectApplyFn(spell_riding_giant_boar_305068_AuraScript::HandleApply, EFFECT_1, SPELL_AURA_LINKED_SUMMON, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(spell_riding_giant_boar_305068_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_LINKED_SUMMON, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_riding_giant_boar_305068_AuraScript();
    }
};

// ID - 173426 Ride Vehicle
class spell_ride_vehicle_173426 : public SpellScriptLoader
{
public:
    spell_ride_vehicle_173426() : SpellScriptLoader("spell_ride_vehicle_173426") {}

    class spell_ride_vehicle_173426_AuraScript : public AuraScript
    {
        void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* caster = GetCaster();
            Player* player = caster->ToPlayer();
            if (!caster || !player)
                return;

            // This spell is universal so i made my changes to Exile's Reach Only
            if (player->GetMapId() == MAP_NPE)
            {
                // Scheduler failsafe solution to avoid crash - scheduler should stop before despawn
                if (Creature* giantBoar = player->GetSummonedCreatureByEntry(NPC_DARKMAUL_PLAINS_WANDERING_BOAR_SUMMON))
                    giantBoar->GetScheduler().CancelAll();

                player->RemoveAura(SPELL_RIDING_GIANT_BOAR);
                player->RemoveAura(SPELL_RIDING_GIANT_BOAR2);
                player->CastSpell(player, SPELL_KNOCKBACK_FOR_HACKFIX_GIANT_BOAR_LEAVE, true);
            }
        }

        void Register() override
        {
            OnEffectRemove += AuraEffectRemoveFn(spell_ride_vehicle_173426_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_ride_vehicle_173426_AuraScript();
    }
};

// ID - 321627 Charge
class spell_giant_boar_charge_321627 : public SpellScriptLoader
{
public:
    spell_giant_boar_charge_321627() : SpellScriptLoader("spell_giant_boar_charge_321627") {}

    class spell_giant_boar_charge_321627_SpellScript : public SpellScript
    {
        void HandleCharge(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            WorldLocation const* loc = GetExplTargetDest();
            Unit* caster = GetCaster();
            if (!caster)
                return;

            caster->GetMotionMaster()->MoveCharge(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ());
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_giant_boar_charge_321627_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE_DEST);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_giant_boar_charge_321627_SpellScript();
    }
};

// Giant Boar Trample (305557) — mows down the undead army / Monstrous Cadavers
// (157091) around the boar. The spell's Effect 1 is a DUMMY (radius 13) that
// targets the caster; the actual damage to nearby enemies is scripted here.
class spell_giant_boar_trample_305557 : public SpellScriptLoader
{
public:
    spell_giant_boar_trample_305557() : SpellScriptLoader("spell_giant_boar_trample_305557") {}

    class spell_giant_boar_trample_305557_SpellScript : public SpellScript
    {
        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster || !caster->IsInWorld())
                return;

            // Damage comes from the boar vehicle so the undead aggro the boar,
            // not the rider (a player-cast trample would pull threat onto the
            // player and the mobs would chew through the rider's health).
            Unit* source = caster;
            if (Player* player = caster->ToPlayer())
                if (Unit* boar = player->GetVehicleBase())
                    source = boar;

            float radius = GetEffectInfo().CalcRadius(source).Max;

            std::list<Creature*> targets;
            Trinity::AnyUnitInObjectRangeCheck u_check(source, radius);
            Trinity::CreatureListSearcher<Trinity::AnyUnitInObjectRangeCheck> searcher(source, targets, u_check);
            Cell::VisitGridObjects(source, searcher, radius);

            for (Creature* target : targets)
            {
                if (!source->IsHostileTo(target))
                    continue;
                // Mow the undead down: despawn them outright so the trampled
                // army vanishes (the scene's "Big Kill Credit" grants the
                // quest credit, so the mobs don't need to die through the
                // normal damage/corpse path).
                target->DespawnOrUnsummon(0s, 1h);
            }
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_giant_boar_trample_305557_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_giant_boar_trample_305557_SpellScript();
    }
};

class LeaveGiantBoarEvent : public BasicEvent
{
public:
    LeaveGiantBoarEvent(Player* player) : _player(player) {}

    bool Execute(uint64 /*e_time*/, uint32 /*p_time*/) override
    {
        if (Vehicle* giantBoar = _player->GetVehicle())
            giantBoar->RemoveAllPassengers();

        return true;
    }

private:
    Player* _player;
};

// ID - 305559 Ping Vehicle (DNT)
class spell_giant_boar_ping_vehicle_305559 : public SpellScriptLoader
{
public:
    spell_giant_boar_ping_vehicle_305559() : SpellScriptLoader("spell_giant_boar_ping_vehicle_305559") {}

    class spell_giant_boar_ping_vehicle_305559_SpellScript : public SpellScript
    {
        void HandleOnCast()
        {
            Unit* caster = GetCaster();
            Player* player = caster->ToPlayer();
            Creature* giantBoar = player->GetVehicleCreatureBase();
            if (!caster || !player || !giantBoar)
                return;

            player->m_Events.AddEventAtOffset(new LeaveGiantBoarEvent(player), 5s);

            for (int8 i = 0; i != 6; i++)
            {
                giantBoar->GetScheduler().Schedule(i * 600ms, [giantBoar, i](TaskContext /*context*/)
                    {
                        switch (i) // 600ms counter in the 3sec periodic effect
                        {
                        case 0:
                            giantBoar->CastSpell(giantBoar, SPELL_GROW_2, true);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            break;
                        case 1:
                            giantBoar->RemoveAura(SPELL_GROW_2);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            break;
                        case 2:
                            giantBoar->CastSpell(giantBoar, SPELL_GROW_2, true);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            break;
                        case 3:
                            giantBoar->RemoveAura(SPELL_GROW_2);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            break;
                        case 4:
                            giantBoar->CastSpell(giantBoar, SPELL_GROW_3, true);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            break;
                        case 5:
                            giantBoar->RemoveAura(SPELL_GROW_3);
                            giantBoar->PlayObjectSound(SOUND_BOAR_SIZING, giantBoar->GetGUID());
                            giantBoar->CastSpell(giantBoar, SPELL_RESIZER_HIT_TOSMALL, true);
                            break;
                        } });
            }
        }

        void Register() override
        {
            OnCast += SpellCastFn(spell_giant_boar_ping_vehicle_305559_SpellScript::HandleOnCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_giant_boar_ping_vehicle_305559_SpellScript();
    }
};

void AddSC_exiles_reach_darkmaul_plains()
{
    // Player Scripts
    new exiles_reach_darkmaul_plains_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_167019_149899);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_167905_156526);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_167915_156749);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_156736_156716);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_156267);
    RegisterCreatureAI(npc_exiles_reach_darkmaul_plains_157114);
    // GameObject Scripts
    RegisterGameObjectAI(go_exiles_reach_darkmaul_plains_326716_326717_326718_326719);
    // Spell Scripts
    new spell_re_sizing_313269();
    new spell_re_sizing_305716();
    new spell_re_sizing_325346();
    new spell_resizer_hit_305742();
    new spell_riding_giant_boar_305068();
    new spell_ride_vehicle_173426();
    new spell_giant_boar_charge_321627();
    new spell_giant_boar_ping_vehicle_305559();
    new spell_giant_boar_trample_305557();
}
