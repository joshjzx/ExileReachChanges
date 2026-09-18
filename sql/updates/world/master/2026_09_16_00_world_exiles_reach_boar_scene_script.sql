-- Exiles Reach: bind the Giant Boar ride scene (SceneId 2338, package 2712) to its SceneScript.
-- During the boar ride ("Ride of the Scientifically Enhanced Boar" / "The Redeather") the client
-- fires "Trampling Time", "Big Kill Credit" and "Knockback" trigger events; core only dispatches
-- those to a bound SceneScript via scene_template.ScriptName. The script itself lives in
-- src/server/scripts/ExilesReach/exiles_reach.cpp (scene_exiles_reach_boar_ride).
UPDATE `scene_template` SET `ScriptName` = 'scene_exiles_reach_boar_ride' WHERE `SceneId` = 2338;
