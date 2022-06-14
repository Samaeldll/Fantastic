-- Add Missing Black Blood of Draenor Spawns to db
SET @GUID := 209032;
DELETE FROM `creature` WHERE `guid` BETWEEN @GUID AND @GUID+36;
INSERT IGNORE INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(@GUID+0,23286,530,1,1,0,0,-4989.68945,533.267,-6.23201227,2.588225,120,5,0,1,0,0,1),
(@GUID+1,23286,530,1,1,0,0,-5008.07764,445.799774,-7.687496,4.848184,120,5,0,1,0,0,1),
(@GUID+2,23286,530,1,1,0,0,-5012.0127,514.3434,-5.042469,5.13465834,120,5,0,1,0,0,1),
(@GUID+3,23286,530,1,1,0,0,-5012.997,549.9415,-4.40564346,1.67378473,120,5,0,1,0,0,1),
(@GUID+4,23286,530,1,1,0,0,-5016.02148,490.971039,-7.866439,4.838478,120,5,0,1,0,0,1),
(@GUID+5,23286,530,1,1,0,0,-5021.433,469.1069,-8.175169,1.2226696,120,5,0,1,0,0,1),
(@GUID+6,23286,530,1,1,0,0,-5024.51563,426.561432,-10.20804,1.11687481,120,5,0,1,0,0,1),
(@GUID+7,23286,530,1,1,0,0,-5025.797,397.633545,-13.0280495,4.00145245,120,5,0,1,0,0,1),
(@GUID+8,23286,530,1,1,0,0,-5036.446,597.1265,18.5822525,0.0929955542,120,5,0,1,0,0,1),
(@GUID+9,23286,530,1,1,0,0,-5048.92041,450.347321,-12.185751,2.29650974,120,5,0,1,0,0,1),
(@GUID+10,23286,530,1,1,0,0,-5051.3457,399.3506,-12.2878532,5.29644632,120,5,0,1,0,0,1),
(@GUID+11,23286,530,1,1,0,0,-5053.088,628.9155,24.1695518,2.795165,120,5,0,1,0,0,1),
(@GUID+12,23286,530,1,1,0,0,-5056.857,296.117645,-7.93896675,3.48118663,120,5,0,1,0,0,1),
(@GUID+13,23286,530,1,1,0,0,-5069.628,423.853058,-11.0079794,6.07293034,120,5,0,1,0,0,1),
(@GUID+14,23286,530,1,1,0,0,-5069.8833,636.1971,30.2196884,5.05358,120,5,0,1,0,0,1),
(@GUID+15,23286,530,1,1,0,0,-5080.326,676.015869,32.9786034,3.00858331,120,5,0,1,0,0,1),
(@GUID+16,23286,530,1,1,0,0,-5085.002,454.966553,-7.321316,1.203854,120,5,0,1,0,0,1),
(@GUID+17,23286,530,1,1,0,0,-5087.409,344.1611,4.08592224,5.54356575,120,5,0,1,0,0,1),
(@GUID+18,23286,530,1,1,0,0,-5087.988,507.385925,-11.2343569,1.80733728,120,5,0,1,0,0,1),
(@GUID+19,23286,530,1,1,0,0,-5088.39746,475.1501,-7.82405758,5.05217171,120,5,0,1,0,0,1),
(@GUID+20,23286,530,1,1,0,0,-5088.81445,645.055237,32.7489166,2.61234975,120,5,0,1,0,0,1),
(@GUID+21,23286,530,1,1,0,0,-5094.403,647.8368,33.02874,5.85462,120,5,0,1,0,0,1),
(@GUID+22,23286,530,1,1,0,0,-5099.644,408.555359,-12.73991,2.38603544,120,5,0,1,0,0,1),
(@GUID+23,23286,530,1,1,0,0,-5102.78174,685.7555,33.9081421,5.558793,120,5,0,1,0,0,1),
(@GUID+24,23286,530,1,1,0,0,-5102.902,443.3057,-7.44998455,0.574597657,120,5,0,1,0,0,1),
(@GUID+25,23286,530,1,1,0,0,-5105.38037,536.0138,-10.526392,5.009346,120,5,0,1,0,0,1),
(@GUID+26,23286,530,1,1,0,0,-5119.125,387.557648,-12.1417027,3.153311,120,5,0,1,0,0,1),
(@GUID+27,23286,530,1,1,0,0,-5131.08838,361.671021,-17.5902081,4.802402,120,5,0,1,0,0,1),
(@GUID+28,23286,530,1,1,0,0,-5133.446,398.5927,-11.155489,3.7525115,120,5,0,1,0,0,1),
(@GUID+29,23286,530,1,1,0,0,-5134.523,424.9907,-10.7020779,2.93014455,120,5,0,1,0,0,1),
(@GUID+30,23286,530,1,1,0,0,-5136.922,466.687683,-13.7564421,2.79966116,120,5,0,1,0,0,1),
(@GUID+31,23286,530,1,1,0,0,-5149.55469,351.741577,-19.33655,0.996007144,120,5,0,1,0,0,1),
(@GUID+32,23286,530,1,1,0,0,-5153.18555,487.3614,-12.2186108,4.57836151,120,5,0,1,0,0,1),
(@GUID+33,23286,530,1,1,0,0,-5170.654,422.92,-10.6833916,0.485873938,120,5,0,1,0,0,1),
(@GUID+34,23286,530,1,1,0,0,-5179.337,369.596924,-20.0646782,1.51201284,120,5,0,1,0,0,1),
(@GUID+35,23286,530,1,1,0,0,-5181.72559,345.638947,-21.1928349,5.528716,120,5,0,1,0,0,1),
(@GUID+36,23286,530,1,1,0,0,-5212.74658,326.250763,-21.8211174,4.56194735,120,5,0,1,0,0,1);
