#ifndef _CUSTOMITEM_H
#define _CUSTOMITEM_H

#include <string>
#include <iostream>
#include <algorithm>

std::string to_string(int param);

class CustomItem
{
    public:
    void SendCustomBuyError(Player* player, Creature* creature, VendorItem const* crItem); 
    void HandleCustomItemDescription(ItemTemplate const* pProto, std::string &Description);
};

#define sCustomItem ACE_Singleton<CustomItem, ACE_Null_Mutex>::instance()
#endif