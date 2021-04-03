// credits: Liv Javier

#if defined _menu_included
    #endinput
#endif

#define _menu_included

#include <a_samp>
#include <YSI_Data/y_iterate>
#include <YSI_Coding/y_hooks>

const 
    MENU_MAX_ITEMS = 500,
    MENU_ITEM_COLOR = 168430310,
    MENU_MAIN_COLOR = -1523963137,
    MENU_MAX_ITEM_PER_PAGE = 12; // 

static
    count_tick[MAX_PLAYERS],
    player_menu_id[MAX_PLAYERS],
    Iterator:player_items[MAX_PLAYERS]<MENU_MAX_ITEMS>,
    player_item_list[MAX_PLAYERS][MENU_MAX_ITEMS][26],
    player_item_color[MAX_PLAYERS] = {MENU_MAIN_COLOR, ...},
    PlayerText:_@m_ptd[MAX_PLAYERS][5] = {PlayerText:-1, ...},
    PlayerText:_@m_box[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...},
    PlayerText:_@m_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...};
    //PlayerText:_@m_sub_item[MAX_PLAYERS][MENU_MAX_ITEM_PER_PAGE] = {PlayerText:-1, ...}; ko lam nua

forward OnMenuResponse(playerid, menuid, response, listitem);

stock Menu_Add(playerid, const item[26])
{
    new 
        index = Iter_Free(player_items[playerid]);

    if(-1 < index < MENU_MAX_ITEMS)
    {
        strcat((player_item_list[playerid][index][0] = EOS, player_item_list[playerid][index]), item);
        Iter_Add(player_items[playerid], index);
        return 1;
    }
    return 0;
}

stock Menu_Show(playerid, menuid, const tittle[] = "Menu", const header[] = "Interaction")
{
    if(menuid <= 0) 
        return 0;
    
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][1], tittle);
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][3], header);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawShow(playerid, _@m_ptd[playerid][i]);
    }
    Menu_ShowPage(playerid, 0);
    Menu_SelectIndex(playerid, 0);
    player_menu_id[playerid] = menuid;
    return 1;
}

stock Menu_Hide(playerid) 
{
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawHide(playerid, _@m_ptd[playerid][i]);
    }
    for(new count = 0; count < MENU_MAX_ITEM_PER_PAGE; count ++) 
    {
        if(_@m_item[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_item[playerid][count]);
            _@m_item[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
        }               
        if(_@m_box[playerid][count] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_box[playerid][count]);
            _@m_box[playerid][count] = PlayerText:INVALID_TEXT_DRAW;
        }               
    }
    Iter_Clear(player_items[playerid]);
    SetPVarInt(playerid, "Menu.SelectIndex", -1);
    return 1;
}

stock Menu_SetColor(playerid, color)
{
    player_item_color[playerid] = color;
    return 1;
}

static Menu_SelectIndex(playerid, index, cur_index = -1, bool:show = true) 
{
    if(cur_index != -1) 
    {
        if(_@m_box[playerid][cur_index] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawColor(playerid, _@m_box[playerid][cur_index], MENU_ITEM_COLOR);
            if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][cur_index]);
        }   
    }
    if(_@m_box[playerid][index] != PlayerText:INVALID_TEXT_DRAW) 
    {
        SetPVarInt(playerid, "Menu.SelectIndex", index);
        PlayerTextDrawColor(playerid, _@m_box[playerid][index], player_item_color[playerid]);
        if(show) PlayerTextDrawShow(playerid, _@m_box[playerid][index]);
    }

    new 
        str[22];

    count_tick[playerid] = GetTickCount() + 150;
    format(str, sizeof(str), "%d/%d", Menu_CurrentItem(playerid) + 1, Iter_Count(player_items[playerid]));
    PlayerTextDrawSetString(playerid, _@m_ptd[playerid][4], str);
    PlayerTextDrawShow(playerid, _@m_ptd[playerid][4]);     
    return 1;   
}

static Menu_ShowPage(playerid, page = 0) 
{
    for(new j = 0, i = page * MENU_MAX_ITEM_PER_PAGE; i < MENU_MAX_ITEM_PER_PAGE * (page + 1); i ++, j ++) 
    {
        if(Iter_Contains(player_items[playerid], i)) 
        {
            Menu_Create_TD(playerid, j, player_item_list[playerid][i]);
        }
        else 
        {
            if(_@m_item[playerid][j] != PlayerText:-1) 
            {
                PlayerTextDrawDestroy(playerid, _@m_item[playerid][j]);
                _@m_item[playerid][j] = PlayerText:-1;
            }           
            if(_@m_box[playerid][j] != PlayerText:-1) 
            {
                PlayerTextDrawDestroy(playerid, _@m_box[playerid][j]);
                _@m_box[playerid][j] = PlayerText:-1;
            }   
        }
    }
    SetPVarInt(playerid, "Menu.curPage", page);
    return 1;
}

static Menu_CurrentItem(playerid) 
{
    return MENU_MAX_ITEM_PER_PAGE * GetPVarInt(playerid, "Menu.curPage") + GetPVarInt(playerid, "Menu.SelectIndex");
}

stock Menu_GetMaxItemPerPage(playerid, cur_page) 
{
    new 
        count = 0;
    
    for(new i = MENU_MAX_ITEM_PER_PAGE * cur_page; i < MENU_MAX_ITEM_PER_PAGE * (cur_page + 1); i ++) 
    {
        if(Iter_Contains(player_items[playerid], i))
        {
            count++;
        }
    } 
    return count;
}

static Menu_Create_TD(playerid, index, const item[26]) 
{
    if(_@m_item[playerid][index] != PlayerText:-1) 
    {
        PlayerTextDrawDestroy(playerid, _@m_item[playerid][index]);
    }
    if(_@m_box[playerid][index] != PlayerText:-1) 
    {
        PlayerTextDrawDestroy(playerid, _@m_box[playerid][index]);
    }

    new 
        Float:Y = 204.548095 + (index * 11.300745);

    _@m_box[playerid][index] = CreatePlayerTextDraw(playerid, 498.966522, Y - 1.70 + index * 0.4, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, _@m_box[playerid][index], 110.030433, 11.699993);
    PlayerTextDrawAlignment(playerid, _@m_box[playerid][index], 1);
    PlayerTextDrawColor(playerid, _@m_box[playerid][index], MENU_ITEM_COLOR);
    PlayerTextDrawSetShadow(playerid, _@m_box[playerid][index], 0);
    PlayerTextDrawFont(playerid, _@m_box[playerid][index], 4);
    PlayerTextDrawShow(playerid, _@m_box[playerid][index]);

    _@m_item[playerid][index] = CreatePlayerTextDraw(playerid, 500.366638 + 0.0, Y + 0.0 + index * 0.4, item);
    PlayerTextDrawLetterSize(playerid, _@m_item[playerid][index], 0.184666, 0.886074);
    PlayerTextDrawSetProportional(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawAlignment(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawSetShadow(playerid, _@m_item[playerid][index], 0);
    PlayerTextDrawColor(playerid, _@m_item[playerid][index], -1);
    PlayerTextDrawFont(playerid, _@m_item[playerid][index], 1);
    PlayerTextDrawShow(playerid, _@m_item[playerid][index]);
    return 1;
}

stock Menu_Showing(playerid) 
{
    return GetPVarInt(playerid, "Menu.SelectIndex") == -1 ? 0 : 1;
}

stock Menu_GetMaxPage(playerid) 
{
    new 
        items = Iter_Count(player_items[playerid]);

    return floatround((items % MENU_MAX_ITEM_PER_PAGE == 0) ? items / MENU_MAX_ITEM_PER_PAGE : items / MENU_MAX_ITEM_PER_PAGE + 1);
}

hook OnPlayerConnect(playerid)
{
    _@m_ptd[playerid][0] = CreatePlayerTextDraw(playerid, 498.966491, 140.937042, "LD_SPAC:white"); // Main BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][0], 110.000000, 51.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][0], player_item_color[playerid]);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][0], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][0], 4);

    _@m_ptd[playerid][1] = CreatePlayerTextDraw(playerid, 554.366882, 159.129791, "_"); // Main Tittle
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][1], 0.400000, 1.600000);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][1], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][1], 2);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][1], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][1], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][1], 1);

    _@m_ptd[playerid][2] = CreatePlayerTextDraw(playerid, 498.966491, 191.840148, "LD_SPAC:white"); // Header BG
    PlayerTextDrawTextSize(playerid, _@m_ptd[playerid][2], 110.000000, 11.000000);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][2], 16843208);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][2], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][2], 0);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][2], 4);

    _@m_ptd[playerid][3] = CreatePlayerTextDraw(playerid, 501.000061, 193.359375, "Interaction"); // Header 
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][3], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][3], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][3], 0);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][3], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][3], 2);

    _@m_ptd[playerid][4] = CreatePlayerTextDraw(playerid, 607.598144, 193.359298, #); // Page
    PlayerTextDrawLetterSize(playerid, _@m_ptd[playerid][4], 0.123999, 0.766222);
    PlayerTextDrawSetProportional(playerid, _@m_ptd[playerid][4], 1);
    PlayerTextDrawSetShadow(playerid, _@m_ptd[playerid][4], 0);
    PlayerTextDrawAlignment(playerid, _@m_ptd[playerid][4], 3);
    PlayerTextDrawColor(playerid, _@m_ptd[playerid][4], -1);
    PlayerTextDrawFont(playerid, _@m_ptd[playerid][4], 2);
    
    player_menu_id[playerid] = -1;
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        _@m_item[playerid][i] = PlayerText:-1;
        _@m_box[playerid][i] = PlayerText:-1;
    }

    SetPVarInt(playerid, "Menu.SelectIndex", -1);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    Iter_Clear(player_items[playerid]);
    for(new i = 0; i < 5; i ++) 
    {
        PlayerTextDrawDestroy(playerid, _@m_ptd[playerid][i]);
    }
    for(new i = 0; i < MENU_MAX_ITEM_PER_PAGE; i ++) 
    {
        if(_@m_item[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_item[playerid][i]);
            _@m_item[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }               
        if(_@m_box[playerid][i] != PlayerText:INVALID_TEXT_DRAW) 
        {
            PlayerTextDrawDestroy(playerid, _@m_box[playerid][i]);
            _@m_box[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }   
    }   
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(Menu_Showing(playerid) && count_tick[playerid] - GetTickCount() <= 0)
    {
        new 
            nextIndex, Keys, ud, lr,
            curPage = GetPVarInt(playerid, "Menu.curPage"),
            oldIndex = GetPVarInt(playerid, "Menu.SelectIndex");

       // nextIndex = oldIndex;
        GetPlayerKeys(playerid, Keys, ud, lr);
        #pragma unused Keys, lr
        switch(ud)
        {
            case KEY_DOWN:
            {
                if(oldIndex + 1 >= MENU_MAX_ITEM_PER_PAGE) 
                {
                    nextIndex = 0;
                    Menu_ShowPage(playerid, ++curPage >= Menu_GetMaxPage(playerid) ? 0 : curPage);
                }
                else 
                {
                    nextIndex = oldIndex + 1;
                    if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                    {
                        nextIndex = 0;
                        Menu_ShowPage(playerid, 0);
                    }
                }
                Menu_SelectIndex(playerid, nextIndex, oldIndex);    
            }
            case KEY_UP:
            {
                if(oldIndex - 1 <= -1) 
                {
                    new 
                        page;
                    
                    Menu_ShowPage(playerid, --curPage <= -1 ? Menu_GetMaxPage(playerid) - 1: curPage);
                    page = (curPage == -1) ? Menu_GetMaxPage(playerid) - 1 : GetPVarInt(playerid, "Menu.curPage");
                    nextIndex = Menu_GetMaxItemPerPage(playerid, page) - 1;
                //  printf("<A> nextIndex=%d; curPage=%d;", nextIndex, curPage);
                }
                else 
                {
                    nextIndex = oldIndex - 1;
                    if((MENU_MAX_ITEM_PER_PAGE * curPage + nextIndex) >= Iter_Count(player_items[playerid])) 
                    {
                        nextIndex = 0;
                        Menu_ShowPage(playerid, 0);
                    }
                //  printf("<B> nextIndex=%d; curPage=%d;", nextIndex, curPage);
                }
                if(nextIndex <= -1)
                {
                    new 
                        page = Menu_GetMaxPage(playerid) - 1;

                    Menu_ShowPage(playerid, page);
                    nextIndex = Menu_GetMaxItemPerPage(playerid, page - 1);
                //  printf("Page=%d; nextIndex=%d;", page, nextIndex);
                }
                Menu_SelectIndex(playerid, nextIndex, oldIndex);    
            }
        }
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(Menu_Showing(playerid))
    {
        switch(newkeys)
        {
            case KEY_SECONDARY_ATTACK, KEY_SPRINT: 
            { 
                new 
                    response = (newkeys & KEY_SECONDARY_ATTACK) ? 1 : 0;
                
                if(player_menu_id[playerid] >= 0) 
                {
                    CallLocalFunction("OnMenuResponse", "dddd", playerid, player_menu_id[playerid], response, Menu_CurrentItem(playerid));
                }
            }           
        }
    }
    return 1;
}

hook OnGameModeInit()
{
    Iter_Init(player_items);
}
