# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
    #Bosses
    Custom/boss/arhitel.cpp
    Custom/boss/azog_conqueror.cpp
    Custom/boss/uforius_frost.cpp
    Custom/boss/infernal_crusher.cpp
    Custom/boss/archangel_michael.cpp
    #Title Master
    Custom/TitleMaster/TitleEngine.cpp
    Custom/TitleMaster/TitleEngine.h
    Custom/TitleMaster/TitleHandler.cpp
    Custom/TitleMaster/TitleHandler.h
    Custom/TitleMaster/TitleMgr.cpp
    Custom/TitleMaster/TitleMgr.h
    Custom/TitleMaster/xMessage.cpp
    Custom/TitleMaster/xMessage.h
    #Transmogrification
    Custom/Transmogrification.h
    Custom/Transmogrification.cpp
    Custom/Transmogrifier.cpp
    #Arena1x1
    Custom/npc_arena1v1.cpp
    Custom/npc_arena1v1.h
    #WorldChat
    Custom/WorldChat/worldchat.cpp
    #Other
    Custom/world_announer.cpp
    Custom/Announce_NewPlayer.cpp
    Custom/custom_item_scripts.cpp
    Custom/Reforging.cpp
    Custom/Custom_PlayerScripts.cpp
    Custom/buffs.cpp
    Custom/killsystem.cpp
    Custom/change.cpp
    Custom/ArenaFastStart.cpp
    Custom/Arena_Ranks.cpp
    Custom/channel_factions.cpp
    Custom/duel_reset.cpp
    Custom/telenpc.cpp
    Custom/teleto80lvl.cpp
    Custom/trainer.cpp
    Custom/on_script.cpp
    Custom/language.cpp
    Custom/teleporter.cpp
    Custom/buffer.cpp
    Custom/remove_aura.cpp
    Custom/vendors.cpp
    Custom/npc_money.cpp
)

message("  -> Prepared: Custom")
