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


#include "Loot.h"
#include "exiles_reach.h"

class exiles_reach_abandoned_camp_playerscript : public PlayerScript
{
public:
    exiles_reach_abandoned_camp_playerscript() : PlayerScript("exiles_reach_abandoned_camp_playerscript") {}

	
    void CheckExilesReachPhaseShift(Player* player)
    {
        if (player->GetMapId() == MAP_EXILES_REACH)
        {
            switch (player->GetAreaId())
            {
            case AREA_ABANDONED_CAMP:
            {
                WowCommunity::PhaseShift(player, { PHASE_GENERAL_SHORE }, false);

                if (player->IsInHorde())
                {
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_SHORE_UNK }, true);;
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_ABANDONED_CAMP_15324 }, true);
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_ABANDONED_CAMP_WONSA_STARVING }, !QuestRewarded(player, QUEST_H_COOKING_MEAT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_ABANDONED_CAMP_WONSA_EATEN }, QuestRewarded(player, QUEST_H_COOKING_MEAT));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_ABANDONED_CAMP_BREKA_PEACEFUL }, QuestCompletedOrRewarded(player, QUEST_H_FINDING_THE_LOST_EXPEDITION) && !player->GetSummonedCreatureByEntry(NPC_ABANDONED_CAMP_BREKA) && !player->GetSummonedCreatureByEntry(NPC_ABANDONED_CAMP_BREKA_WALKING) && !player->GetQuestStatus(QUEST_H_NORTHBOUND));
                    WowCommunity::PhaseShift(player, { PHASE_HORDE_QUILBOAR_BRIARPATCH_MITHDRAN }, !QuestRewarded(player, QUEST_H_NORTHBOUND));
                }

                if (player->IsInAlliance())
                {
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_ABANDONED_CAMP_ALARIA_STARVING }, !QuestRewarded(player, QUEST_COOKING_MEAT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_ABANDONED_CAMP_ALARIA_EATEN }, QuestRewarded(player, QUEST_COOKING_MEAT));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_ABANDONED_CAMP_GARRICK_PEACEFUL }, QuestRewarded(player, QUEST_FINDING_THE_LOST_EXPEDITION) && !player->GetSummonedCreatureByEntry(NPC_ABANDONED_CAMP_GARRICK) && !player->GetQuestStatus(QUEST_NORTHBOUND));
                    WowCommunity::PhaseShift(player, { PHASE_ALLIANCE_QUILBOAR_BRIARPATCH_AUSTIN }, !QuestRewarded(player, QUEST_NORTHBOUND));
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
                case QUEST_NORTHBOUND:
                    activeSummons.push_back(NPC_ABANDONED_CAMP_GARRICK_WALKING);
                    break;
                case QUEST_H_NORTHBOUND:
                    activeSummons.push_back(NPC_ABANDONED_CAMP_BREKA_WALKING);
                    break;
                case QUEST_ENHANCED_COMBAT_TACTICS:
                    activeSummons.push_back(NPC_ABANDONED_CAMP_GARRICK);
                    break;
                case QUEST_H_ENHANCED_COMBAT_TACTICS:
                    activeSummons.push_back(NPC_ABANDONED_CAMP_BREKA);
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

// Won'sa, Alaria (abandoned camp) (starving)
struct npc_exiles_reach_abandoned_camp_166854_156607 : public ScriptedAI
{
    npc_exiles_reach_abandoned_camp_166854_156607(Creature* creature) : ScriptedAI(creature)
    {
        talked = false;
    }

    void MoveInLineOfSight(Unit* who) override
    {
        Player* player = who->ToPlayer();
        if (!player)
            return;

        if (player->IsInDist(me, 10.0f))
        {
            if (player->GetQuestStatus(QUEST_FINDING_THE_LOST_EXPEDITION) || player->GetQuestStatus(QUEST_H_FINDING_THE_LOST_EXPEDITION))
                player->KilledMonsterCredit(me->GetEntry());

            if (player->HasItemCount(ITEM_COOKED_MEAT, 2, false))
                if (Creature* npc = me->FindNearestCreature(NPC_ABANDONED_CAMP_BREKA, 10.0f, true))
                    if (!talked)
                    {
                        Talk(1, player);
                        // We got ambushed... by da quilboar...
                        // The quilboar... ambushed us...

                        me->AddDelayedEvent(5000, [this, player]() // time_delay
                            {
                                me->AI()->Talk(4, player); // Eat first. I won't let you starve to death.
                            });

                        talked = true;
                    }
        }

        if (!metBrekaOrGarrick)
        {
            if (Creature* breka = player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_BREKA))
            {
                breka->AI()->Talk(3, player);
                // Won'sa!

                me->AddDelayedEvent(3000, [this, player]() // time_delay
                    {
                        me->AI()->Talk(2, player); // So... hungry...
                    });

                breka->GetMotionMaster()->MovePoint(0, -249.05f, -2492.35f, 17.957f, true);
                metBrekaOrGarrick = true;
            }

            if (Creature* garrick = player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_GARRICK))
            {
                garrick->AI()->Talk(3, player);
                // Alaria!

                garrick->GetMotionMaster()->MovePoint(0, -249.05f, -2492.35f, 17.957f, true);
                metBrekaOrGarrick = true;
            }
        }

    }

    void OnQuestReward(Player* player, Quest const* quest, LootItemType /*type*/, uint32 /*opt*/) override
    {
        if (quest->GetQuestId() == QUEST_H_COOKING_MEAT && player->GetItemByEntry(ITEM_COOKED_MEAT))
            player->DestroyItemCount(ITEM_COOKED_MEAT, player->GetItemCount(ITEM_COOKED_MEAT), true, false);

        if (quest->GetQuestId() == QUEST_H_FINDING_THE_LOST_EXPEDITION)
            if (Creature* brekatemp = player->GetSummonedCreatureByEntry(NPC_MURLOC_HIDEAWAY_BREKA))
                brekatemp->DespawnOrUnsummon();

        me->CastSpell(player, SPELL_UPDATE_PHASESHIFT, true);

        me->SetStandState(UNIT_STAND_STATE_STAND);
        talked = false;
        metBrekaOrGarrick = false;
    }

private:
    bool talked, metBrekaOrGarrick;
};

// Warlord Breka Grimaxe, Captain Garrick (abandoned camp)
struct npc_exiles_reach_abandoned_camp_166906_156651 : public ScriptedAI
{
    npc_exiles_reach_abandoned_camp_166906_156651(Creature* creature) : ScriptedAI(creature)
    {
        orientation = me->GetOrientation();
    }

    void BeFriendly()
    {
        me->AttackStop();
        me->CastSpell(me, SPELL_DRINK_HEALING_POTION, true);
        me->SetFaction(35);
        me->RemoveAllAuras();
    }

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_COOKING_MEAT || quest->GetQuestId() == QUEST_H_COOKING_MEAT)
        {
            if (Creature* wonsa = me->FindNearestCreature(NPC_ABANDONED_CAMP_WONSA, 10.0f, true))
            {
                wonsa->AI()->Talk(0, player); // Shuja said... you'd be comin'...

                me->AddDelayedEvent(5000, [this, player]() // time_delay
                    {
                        me->AI()->Talk(0, player); // She's my daughter. Nothing could keep me from her.
                        player->GetScheduler().Schedule(6s, [this, player](TaskContext /*context*/)
                            {
                                me->AI()->Talk(1, player); // And as a soldier of the Horde, she knows we leave no one behind.
                            }); });
            }

            if (Creature* alaria = me->FindNearestCreature(NPC_ABANDONED_CAMP_ALARIA, 10.0f, true))
            {
                alaria->AI()->Talk(0, player); // Henry knew... you would come....

                me->AddDelayedEvent(5000, [this, player]() // time_delay
                    {
                        me->AI()->Talk(0, player);
                        // She's my daughter. Nothing could keep me from her.
                        // Of course I did. He's my son.
                        player->GetScheduler().Schedule(6s, [this, player](TaskContext /*context*/)
                            {
                                me->AI()->Talk(1, player);
                                // And as a soldier of the Horde, she knows we leave no one behind.
                                // Eat first, Alaria. You need your strength.
                            }); });
            }
        }

        if (quest->GetQuestId() == QUEST_H_ENHANCED_COMBAT_TACTICS || quest->GetQuestId() == QUEST_ENHANCED_COMBAT_TACTICS)
        {
            if (Creature* clone = player->SummonCreature(me->GetEntry(), me->GetPosition(), TEMPSUMMON_MANUAL_DESPAWN, 0s, 0U, 0U, player->GetGUID()))
            {
                clone->AI()->Talk(2, player);
                // Recruit. Quilboar pose a far greater threat than murlocs. You must be ready to face them in battle.
                // Recruit! Quilboar are far deadlier than murlocs. You need to be ready for the dangers ahead.

                player->GetScheduler().Schedule(2s, [clone](TaskContext /*context*/)
                    { clone->GetMotionMaster()->MovePoint(1, EnhancedCombatTacticFightingPositions[1], true); })
                    .Schedule(4s, [clone, player](TaskContext /*context*/)
                        {
                            if (rand() % 2 + 1 == 1)
                                clone->GetMotionMaster()->MovePoint(2, EnhancedCombatTacticFightingPositions[2], true);
                            else
                                clone->GetMotionMaster()->MovePoint(3, EnhancedCombatTacticFightingPositions[3], true); })
                    .Schedule(12s, [clone, player](TaskContext /*context*/)
                        {
                            clone->AI()->Talk(3, player);
                            // Your next lesson begins now!
                            // Alright, let's see what you know!
                            clone->SetFaction(14);
                            clone->SetLevel(player->GetLevel());
                            clone->SetTarget(player->GetGUID());
                            clone->Attack(player, true); });
            }
        }

        me->CastSpell(player, SPELL_UPDATE_PHASESHIFT, true);
    }

    void SpellHit(WorldObject* caster, SpellInfo const* /*spell*/) override
    {
        // @TODO : make it class specific
        Player* player = caster->ToPlayer();
        if (!player)
            return;

        hitCounter++;

        if (hitCounter == 3)
        {
            if (player->GetQuestStatus(QUEST_ENHANCED_COMBAT_TACTICS) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_H_ENHANCED_COMBAT_TACTICS) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(164577);
            }

            if (player->GetQuestStatus(QUEST_ENHANCED_COMBAT_TACTICS) == QUEST_STATUS_COMPLETE ||
                player->GetQuestStatus(QUEST_H_ENHANCED_COMBAT_TACTICS) == QUEST_STATUS_COMPLETE)
            {
                BeFriendly();
                trainingQuestDone = true;
            }

            hitCounter = 0;
        }
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage, DamageEffectType /*damageType*/, SpellInfo const* /*spellInfo = nullptr*/) override
    {

        if ((me->GetHealth() <= damage) || (me->GetHealth() <= me->GetHealth() * 0.2f))
        {
            damage = 0;
            me->CastSpell(me, SPELL_DRINK_HEALING_POTION, true);
        }
    }

    void UpdateAI(uint32 diff) override
    {
        if (timer <= diff)
        {
            if (trainingQuestDone)
            {
                if (me->GetPosition().IsInDist(EnhancedCombatTacticFightingPositions[2], 1.f) || me->GetPosition().IsInDist(EnhancedCombatTacticFightingPositions[3], 1.f))
                    me->GetMotionMaster()->MovePoint(1, EnhancedCombatTacticFightingPositions[1], true);
                if (me->GetPosition().IsInDist(EnhancedCombatTacticFightingPositions[1], 1.f))
                    me->GetMotionMaster()->MovePoint(0, EnhancedCombatTacticFightingPositions[0], true);
                if (me->GetPosition().IsInDist(EnhancedCombatTacticFightingPositions[0], 1.f) && me->GetOrientation() != orientation)
                {
                    if (Creature* wonsa = me->FindNearestCreature(NPC_ABANDONED_CAMP_WONSA, 20.0f, true))
                        me->SetFacingTo(me->GetAbsoluteAngle(wonsa), true);

                    if (Creature* alaria = me->FindNearestCreature(NPC_ABANDONED_CAMP_ALARIA_2, 20.0f, true))
                        me->SetFacingTo(me->GetAbsoluteAngle(alaria), true);

                    if (me->IsSummon())
                        me->DespawnOrUnsummon(1s, 0s);
                }
            }

            timer = 1000;
        }
        else
            timer -= diff;
    }

private:
    float orientation = 0.f;
    uint32 timer = 1000;
    int8 hitCounter = 0;
    bool trainingQuestDone = false;
};

// Won'sa, Alaria (abandoned camp) (eaten)
struct npc_exiles_reach_abandoned_camp_175030_175031 : public ScriptedAI
{
    npc_exiles_reach_abandoned_camp_175030_175031(Creature* creature) : ScriptedAI(creature) {}

    void OnQuestAccept(Player* player, Quest const* quest) override
    {
        if (quest->GetQuestId() == (QUEST_H_NORTHBOUND))
        {
            if (TempSummon* tempBreka = player->SummonCreature(NPC_ABANDONED_CAMP_BREKA_WALKING, Position(-249.05f, -2492.35f, 17.957f, 0.466517f), TEMPSUMMON_TIMED_DESPAWN, 5min, 0U, 0U, player->GetGUID()))
            {
                tempBreka->AI()->Talk(0, player); // We'll head north to see what the others have found. Won'sa, rest and regain your strength before joining us.
                tempBreka->FollowTarget(player);
                tempBreka->SetSpeed(MOVE_RUN, player->GetSpeed(MOVE_RUN));
                player->GetScheduler().Schedule(Milliseconds(10000), [this, player](TaskContext /*context*/)
                    {
                        me->AI()->Talk(0, player); // I be seein' ya soon, den. Ya daughter be right ta count on ya.
                    })
                    .Schedule(Milliseconds(16000), [this, tempBreka, player](TaskContext /*context*/)
                        {
                            tempBreka->AI()->Talk(1, player); // The Horde never abandons its soldiers. I will not abandon her.
                        });
            }
        }

        if (quest->GetQuestId() == (QUEST_NORTHBOUND))
        {
            if (TempSummon* tempGarrick = player->SummonCreature(NPC_ABANDONED_CAMP_GARRICK_WALKING, Position(-249.05f, -2492.35f, 17.957f, 0.466517f), TEMPSUMMON_TIMED_DESPAWN, 5min, 0U, 0U, player->GetGUID()))
            {
                tempGarrick->AI()->Talk(4, player); // We'll head north and rendezvous with the others. Alaria, recover your stamina before pushing forward.
                tempGarrick->FollowTarget(player);
                tempGarrick->SetSpeed(MOVE_RUN, player->GetSpeed(MOVE_RUN));
                player->GetScheduler().Schedule(Milliseconds(10000), [tempGarrick, player](TaskContext /*context*/)
                    {
                        //  I've never given up on any member of the Alliance. I'm not about to start by failing my son. 
                        tempGarrick->Say(178620, player);
                        tempGarrick->PlayDirectSound(152746, player, 178620); });
            }
        }
    }
};

/* ## MOB SCRIPTS > ## */

// Jrokgar (jrokgar's coast)
class npc_exiles_reach_jrokgar_161265 : public VehicleScript
{
public:
    npc_exiles_reach_jrokgar_161265() : VehicleScript("npc_exiles_reach_jrokgar_161265") {}

    void OnAddPassenger(Vehicle* vehicle, Unit* /*passenger*/, int8 /*seatId*/) override
    {
        vehicle->GetBase()->GetMotionMaster()->MovePoint(0, Position(-88.93f, -2288.38f, 10.8f), true); // throwing position
    }

    struct npc_exiles_reach_jrokgar_161265_AI : public ScriptedAI
    {
        npc_exiles_reach_jrokgar_161265_AI(Creature* creature) : ScriptedAI(creature)
        {
            actionOngoing = false;
            me->SetWalk(true);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
            {
                // Jrokgar like throw. No like fight. Only throw!
                if (!talked)
                {
                    talked = true;
                    me->Say(185017, player);
                    me->PlayDirectSound(149409, player, 185017);
                }
            }
        }

        bool OnGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) override
        {
            // Okay. Jrokgar throw you far!
            Talk(0, player);
            me->CastSpell(me, SPELL_RIDE_JROKGAR, true);
            player->CastSpell(me, SPELL_RIDE_VEHICLE, true);
            me->StopMoving();
            me->SetWalk(false);
            me->GetMotionMaster()->MovePoint(0, throwingPos);
            actionOngoing = true;

            return true;
        }

        void UpdateAI(uint32 diff) override
        {
            if (timer <= diff)
            {
                if (me->GetPosition().IsInDist(throwingPos, 10.f))
                {
                    if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                    {
                        passenger->ExitVehicle();
                        passenger->CastSpell(passenger, SPELL_LAUNCHED, true);
                        me->RemoveAllAuras();
                        me->SetWalk(true);
                        timer = 10000;
                    }
                    else
                    {
                        me->GetMotionMaster()->MovePoint(0, me->GetHomePosition());
                        actionOngoing = false;
                        talked = false;
                    }
                }
                timer = 1000;
            }
            else
                timer -= diff;
        }

        void MovementInform(uint32 type, uint32 /*id*/) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            me->GetMotionMaster()->MoveRandom(me->GetWanderDistance());
        }

    private:
        uint32 timer = 1000;
        bool actionOngoing, talked;
        Position throwingPos = Position(-88.93f, -2288.38f, 10.8f);
    };
};

/* ## GAMEOBJECT SCRIPTS > ## */

// Campfire (Abandoned Camp)
struct go_campfire_339769 : public GameObjectAI
{
    go_campfire_339769(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {

        if (player->hasQuest(QUEST_COOKING_MEAT) || player->hasQuest(QUEST_H_COOKING_MEAT))
            player->ForceCompleteQuest(QUEST_COOKING_MEAT);

        player->AddItem(174074, 1);

        me->DespawnOrUnsummon(0s);

        return true;
    }    
};

void AddSC_exiles_reach_abandoned_camp()
{
    // Player Scripts
    new exiles_reach_abandoned_camp_playerscript();
    // NPC Scripts
    RegisterCreatureAI(npc_exiles_reach_abandoned_camp_166854_156607);
    RegisterCreatureAI(npc_exiles_reach_abandoned_camp_166906_156651);
    RegisterCreatureAI(npc_exiles_reach_abandoned_camp_175030_175031);
    // Mob Scripts
    new npc_exiles_reach_jrokgar_161265();
    // Gameobject Scripts
    RegisterGameObjectAI(go_campfire_339769);
}
