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

class exiles_reach_north_sea_playerscript : public PlayerScript
{
public:
    exiles_reach_north_sea_playerscript() : PlayerScript("exiles_reach_north_sea_playerscript") {}

	
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_NORTH_SEA_H:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_SHIP, PHASE_HORDE_SHIP_15514 }, player->GetQuestStatus(QUEST_H_BRACE_FOR_IMPACT) || player->GetQuestStatus(QUEST_H_STAND_YOUR_GROUND));
                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_COLE, PHASE_ALLIANCE_SHIP_GARRICK_1, PHASE_ALLIANCE_SHIP_CREW_OUTSIDE }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_SHIP_THROG }, !QuestAccepted(player, QUEST_H_STAND_YOUR_GROUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_SHIP_15287, PHASE_HORDE_SHIP_15516 }, player->GetQuestStatus(QUEST_H_BRACE_FOR_IMPACT));
                }
            }
            break;
            case AREA_NORTH_SEA_A:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_SHIP }, true);                    
                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_COLE }, !QuestAccepted(player, QUEST_STAND_YOUR_GROUND) && !QuestCompleted(player, QUEST_BRACE_FOR_IMPACT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_13619 }, QuestRewarded(player, QUEST_WARMING_UP) && !QuestAccepted(player, QUEST_STAND_YOUR_GROUND));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_14350 }, QuestRewarded(player, QUEST_WARMING_UP) && !QuestCompleted(player, QUEST_BRACE_FOR_IMPACT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_GARRICK_1 }, !QuestRewarded(player, QUEST_WARMING_UP));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_CREW_OUTSIDE }, !QuestCompleted(player, QUEST_BRACE_FOR_IMPACT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_SHIP_GARRICK_2, PHASE_ALLIANCE_SHIP_CREW_INSIDE }, QuestCompleted(player, QUEST_BRACE_FOR_IMPACT));
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
                case QUEST_STAND_YOUR_GROUND:
                    activeSummons.push_back(NPC_SHIP_COLE);
                    break;
                case QUEST_H_STAND_YOUR_GROUND:
                    activeSummons.push_back(NPC_SHIP_THROG);
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

// Warlord Breka Grimaxe (ship),  Captain Garrick (ship)
struct npc_exiles_reach_ship_166573_156280 : public ScriptedAI
{
    npc_exiles_reach_ship_166573_156280(Creature* creature) : ScriptedAI(creature) {}

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        switch (quest->GetQuestId())
        {
        case QUEST_WARMING_UP:
            // I need everyone in top form if we're to complete this mission. Show me what you can do!
            Talk(1, player);
            break;
        case QUEST_H_WARMING_UP:
            // We need to be ready for anything. Show me your skill in combat.
            Talk(0, player);
            break;
        case QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR:
            // Ride the boar alone — no follower (156807 reuses Jaina's model,
            // which read as a duplicate Jaina tagging along). The quest-giver
            // stays in place.
            break;
        }
    }

    void OnQuestReward(Player* player, Quest const* quest, LootItemType /*type*/, uint32 /*opt*/) override
    {
        switch (quest->GetQuestId())
        {
        case QUEST_WARMING_UP:
            // @TODO: walk inside and spawn it inside too and make phaseshift for it
            Talk(3, player); // Private Cole will run you through the rest of the drills. I need to discuss this storm with the helmsman.
            break;
        case QUEST_H_WARMING_UP:
            // @TODO: walk inside and spawn it inside too and make phaseshift for it
            Talk(1, player); // Throg will spar with you for now. I must speak to the crew about the rain.
            break;
        case QUEST_BRACE_FOR_IMPACT:
            player->TeleportTo(2175, -462.722f, -2620.544f, 0.472f, 0.760f);
            player->CastSpell(player, SPELL_A_SHIP_CRASH);
            break;
        case QUEST_H_BRACE_FOR_IMPACT:
            player->TeleportTo(2175, -462.722f, -2620.544f, 0.472f, 0.760f);
            player->CastSpell(player, SPELL_H_SHIP_CRASH);
            player->RemoveAllAuras();
            break;
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (me->GetAreaId() == AREA_OGRE_RUINS && me->IsSummon() && !me->GetVehicle())
            {
                if (Player* player = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    if (QuestAccepted(player, QUEST_RIDE_OF_THE_SCIENTIFICALLY_ENHANCED_BOAR))
                    {
                        me->DespawnOrUnsummon();
                    }
                }
            }
        }
        else
            timer -= diff;
    }

private:
    uint32 timer = 1000;
};

// Training Dummy - 160737,
struct npc_exiles_reach_ship_training_dummy : public ScriptedAI
{
    npc_exiles_reach_ship_training_dummy(Creature* creature) : ScriptedAI(creature) {}

    void JustDied(Unit* killer /*killer*/) override
    {
        Player* player = killer->ToPlayer();
        if (!player)
            return;

        if (player->GetQuestStatus(QUEST_H_WARMING_UP))
        {
            if (Creature* breka = me->FindNearestCreature(NPC_SHIP_BREKA, 20.f, true))
                breka->AI()->Talk(3, player); // Fine work. Next, we... rain? Our shaman said the skies would be clear.

            player->CastSpell(player, SPELL_MEDIUM_FOG_AND_RAIN, true);
        }

        if (player->GetQuestStatus(QUEST_WARMING_UP))
        {
            if (Creature* garrick = me->FindNearestCreature(NPC_SHIP_GARRICK, 20.f, true))
                garrick->AI()->Talk(2, player); // Good! Next... hm, that's odd. We weren't expecting rain...

            player->CastSpell(player, SPELL_MEDIUM_FOG_AND_RAIN, true);
        }
    }
};

// Grunt Throg (ship), Private Cole (ship)
struct npc_exiles_reach_ship_166583_160664 : public ScriptedAI
{
    npc_exiles_reach_ship_166583_160664(Creature* creature) : ScriptedAI(creature) {}

    void BeFriendly()
    {
        me->AttackStop();
        me->CastSpell(me, SPELL_DRINK_HEALING_POTION, true);
        me->SetFaction(35);
        me->RemoveAllAuras();
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        switch (quest->GetQuestId())
        {
        case QUEST_STAND_YOUR_GROUND:
        case QUEST_H_STAND_YOUR_GROUND:
            if (Creature* clone = me->SummonPersonalClone(me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player))
            {
                clone->SetHealth(player->GetHealth());
                clone->AI()->Talk(0, player);
                // I'll take my position. Strike first, and strike hard!
                // Let's move into sparring positions. I'll let you have the first strike.

                player->GetScheduler().Schedule(Milliseconds(3000), [clone, player](TaskContext /*context*/)
                    {
                        clone->CastSpell(player, SPELL_JUMP_BEHIND, true);
                        clone->SetFaction(14);
                        clone->SetReactState(REACT_AGGRESSIVE);
                        clone->AI()->Talk(1, player);
                        // Never run from your foe! Victory or death!
                        // Never run from your opponent.Stand your ground and fight until the end!
                    })
                    .Schedule(Milliseconds(5000), [clone, player](TaskContext /*context*/)
                        {
                            if (clone->GetEntry() == 166583)  // @TODO: add this text to cole too
                                clone->AI()->Talk(3, player); // Always face your enemy!
                        });
            }
            break;
        case QUEST_BRACE_FOR_IMPACT:
            player->CastSpell(player, SPELL_HEAVY_FOG_AND_RAIN, true);

            Talk(3, player); // Captain! We can't weather this storm for long!

            if (Creature* garrick = player->FindNearestCreature(NPC_SHIP_GARRICK, 20.0f, true))
            {
                player->GetScheduler().Schedule(Milliseconds(4000), [garrick, player](TaskContext /*context*/)
                    {
                        garrick->AI()->Talk(4, player); // Everyone below decks! Now
                    });
            }
            break;
        case QUEST_H_BRACE_FOR_IMPACT:
            player->CastSpell(player, SPELL_HEAVY_FOG_AND_RAIN, true);

            Talk(2, player); // Warlord! This storm will soon overwhelm us!

            if (Creature* breka = player->FindNearestCreature(NPC_SHIP_BREKA, 20.0f, true))
            {
                player->GetScheduler().Schedule(Milliseconds(4000), [breka, player](TaskContext /*context*/)
                    {
                        breka->AI()->Talk(4, player); // Soldiers, brace yourselves!
                    });
            }
            break;
        }
    }

    void DamageTaken(Unit* attacker, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {
        Player* player = attacker->ToPlayer();
        if (!player)
            return;

        if (damage > me->GetHealth())
        {
            damage = 0;
            me->SetFullHealth();
            me->SetFaction(35);
            me->AttackStop();
            player->KilledMonsterCredit(155607);

            player->GetScheduler().Schedule(Milliseconds(2000), [this, player](TaskContext /*context*/)
                {
                    me->AI()->Talk(4, player);
                    BeFriendly();
                    // I concede! Your strength will see our mission through.
                    me->DespawnOrUnsummon(); 
                });
        }
        else
            me->CastSpell(player, SPELL_JUMP_BEHIND, true);
    }
};

void AddSC_exiles_reach_north_sea()
{
    // Player Scripts
    new exiles_reach_north_sea_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_ship_166573_156280);
    RegisterCreatureAI(npc_exiles_reach_ship_training_dummy);
    RegisterCreatureAI(npc_exiles_reach_ship_166583_160664);
}
