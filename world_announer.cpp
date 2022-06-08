#include "ScriptPCH.h"
struct BroadcastData { const char* text; uint32 time; };

struct BroadcastData Broadcasts[] =
{
    {"|cffff0000 Добро Пожаловать на игровой сервер Fantastic-WoW(3.3.5)", 280000},
	{"|cffff0000 По поводу Доната, обращаться напрямую вк", 630000},
	{"|cffff0000[ВАЖНО]|r|cc33cc333  Почаще заглядывайте в группу вконтакте, ведь там проходят все интересующие для вас вопросы", 980000},
	{"|cffff0000[ВАЖНО]|r|cc33cc333 GM не воскрешает вас, не пишет в ПМ, а так-же не телепортирует вас куда либо просто так. Если у вас возникли вопросы, то обратитесь к GM через систему тикетов.", 1830000},
    {"|cffff0000  Так же на сервере присутствует команда .buff ,.in un all,.max", 2970000},
	{"|cffff0000[ВАЖНО]|r|cc33cc333 Для того,чтобы играть на нашем сервере, вам необходимо скачать патчи для данного реалма, которые есть как в группе сервера", 3660000},
	};


#define BROADCAST_COUNT  sizeof Broadcasts/sizeof(*Broadcasts)
class CustomAnnouncer : public WorldScript
{
public:
    CustomAnnouncer() : WorldScript("CustomAnnouncer") { }

    void OnStartup()
    {
        for(uint32 i = 0; i < BROADCAST_COUNT; i++)
            Events.ScheduleEvent(i+1, Broadcasts[i].time);
    }

    void OnUpdate(uint32 diff)
    {
        Events.Update(diff);
        while (uint32 eventId = Events.ExecuteEvent())
        {
            if(eventId <= BROADCAST_COUNT)
            {
                sWorld->SendServerMessage(SERVER_MSG_STRING, Broadcasts[eventId-1].text);
                Events.ScheduleEvent(eventId, Broadcasts[eventId-1].time);
            }
        }
    }

private:
    EventMap Events;
};

void AddSC_CustomAnnouncer()
{
    new CustomAnnouncer();
}