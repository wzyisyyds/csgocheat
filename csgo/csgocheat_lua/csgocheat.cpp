#include"header.h"
#include<queue>

#include"lua.hpp"



EXTERN_C_START


_declspec(dllexport) void WINAPI csgo_aimbot_run();

_declspec(dllexport) void WINAPI csgo_InstallDriver();

_declspec(dllexport) void WINAPI csgo_UnInstallDriver();

_declspec(dllexport) int lua_dll_init(lua_State* L);

EXTERN_C_END





int player_size = 0;

EntityList players[32];


std::vector<playerinfo> emeny;

playerinfo me_play;

EXTERN_C bool aimbotflag = false;
EXTERN_C bool exitpossprocess = false;
EXTERN_C HHOOK keyboardHook = 0;




EXTERN_C int aimbot_vk_code = 0xA0;
EXTERN_C int exitpossprocess_vk_code = 0xA1;

int screen_x = 1920;
int screen_y = 1080;

int move_len = 1000;




bool move_XY(playerinfo& player, float ViewMatrix[4][4])
{
    player.moveflag = false;
    if (!read_playerinfo(player))
    {
        player.moveflag = false;
        return false;
    }

    if (player.health < 1)
    {
        player.moveflag = false;
        return false;
    }

    vec3 temperror;
    temperror.x = player.head.x - player.vec.x;
    temperror.y = player.head.y - player.vec.y;
    float wucha = temperror.x * temperror.x + temperror.y * temperror.y;
    //std::cout << "误差分析:" << wucha << std::endl;
    if (wucha > 500.f)
    {
        player.moveflag = false;
        return false;
    }
    if (wucha>130.f)
    {
        player.head.x = player.vec.x;
        player.head.y = player.vec.y;
        player.head.z = player.vec.z + 60.f;
    }
    vec3& out = player.move;

    out.x = ViewMatrix[0][0] * player.head.x + ViewMatrix[0][1] * player.head.y + ViewMatrix[0][2] * player.head.z + ViewMatrix[0][3];
    out.y = ViewMatrix[1][0] * player.head.x + ViewMatrix[1][1] * player.head.y + ViewMatrix[1][2] * player.head.z + ViewMatrix[1][3];
    out.z = ViewMatrix[2][0] * player.head.x + ViewMatrix[2][1] * player.head.y + ViewMatrix[2][2] * player.head.z + ViewMatrix[2][3];
    float w = ViewMatrix[3][0] * player.head.x + ViewMatrix[3][1] * player.head.y + ViewMatrix[3][2] * player.head.z + ViewMatrix[3][3];
    

    float inv_w = 1.f / w;
    out.x *= inv_w;
    out.y *= inv_w;
    out.z *= inv_w;
    float _x = screen_x * 0.5f;
    float _y = screen_y * 0.5f;

    _x = (0.5 * out.x * screen_x) + out.x ;
    _y = -(0.5 * out.y * screen_y) + out.y ;
    out.x = _x;
    out.y = _y;

    if ((out.x* out.x+ out.y* out.y)> move_len)
    {
        player.moveflag = false;
        return false;
    }
    player.moveflag = true;
    return true;
}



DWORD WINAPI aimbot(_In_ LPVOID lpParameter)
{
    playerinfo* emenytem = (playerinfo*)lpParameter;
    
    float ViewMatrix[4][4];


    while (true)
    {
        if (aimbotflag)
        {
            vec3 vec;
            bool moveflag = false;
			if (read_ViewMatrix(ViewMatrix))
			{
				if (move_XY(*emenytem, ViewMatrix))
				{
                    Sleep(1);
				}
			}
        }
        else
        {
            Sleep(50);
        }
        Sleep(1);
        if (exitpossprocess)
        {
            break;
        }
    }

    
    ExitThread(0);
}




void WINAPI csgo_aimbot_run()
{
    auto tempthread = CreateThread(NULL, 0, hookmain, 0, 0,0);

    if (!(dll_init(&me_play)&&Init_SendInput()))
    {
        return;
    }

    if (!read_EntityList(players))
    {
        return;
    }

    if (!read_playerinfo(me_play))
    {
        return;
    }


    while (true)
    {

        if (players[player_size].addr!=NULL)
        {
            playerinfo pos;
            pos.addr = (PVOID)players[player_size++].addr;
            if (!read_playerinfo(pos))
            {
                return;
            }
            if (pos.team!=me_play.team)
            {
                emeny.push_back(pos);
            }
        }
        else
        {
            break;
        }
        
    }

    for (size_t i = 0; i < emeny.size(); i++)
    {
        CreateThread(NULL, 0, aimbot, &emeny[i], 0, 0);
    }


    while (true)
    {
        if (aimbotflag)
        {
            int x=1000, y=1000;
            int len = 2000000;
            bool aim = false;
            for (size_t i = 0; i < emeny.size(); i++)
            {
                if (emeny[i].moveflag)
                {
                    aim = true;
                    int tem = emeny[i].move.x * emeny[i].move.x + emeny[i].move.y * emeny[i].move.y;
                    if (tem< len)
                    {
                        len = tem;
                        x = emeny[i].move.x;
                        y = emeny[i].move.y;
                    }
                }
            }
            if (aim)
            Send_Move(x, y);
        }
        if (exitpossprocess)
        {
            break;
        }
        Sleep(3);
    }

    UnhookWindowsHookEx(keyboardHook);
}


void WINAPI csgo_InstallDriver()
{
    InstallDriver("yexi", "yexi.sys");
}


void WINAPI csgo_UnInstallDriver()
{
    UnInstallDriver("yexi");
}


int csgo_aimbot_run_lua (lua_State* L)
{
    csgo_aimbot_run();
    return 0;
}

int csgo_InstallDriver_lua (lua_State* L)
{
    csgo_InstallDriver();
    return 0;
}

int csgo_UnInstallDriver_lua (lua_State* L)
{
    csgo_UnInstallDriver();
    return 0;
}

int lua_dll_init ( lua_State *L )
{
    lua_getglobal(L, "CheatConfig");
    if (!lua_istable(L, -1))//判断是否是table
    {
        printf("table error");
        system("pause");
        return 0;
    }
    lua_getfield(L, -1, "screen_x");
    if (lua_isinteger(L, -1))
    {
        screen_x = lua_tointeger(L, -1);
        printf("screen_x %d\n", screen_x);
        lua_pop(L, 1);
    }
    else
    {
        printf("screen_x error");
    }

    lua_getfield(L, -1, "screen_y");
    if (lua_isinteger(L, -1))
    {
        screen_y = lua_tointeger(L, -1);
        printf("screen_y %d\n", screen_y);
        lua_pop(L, 1);
    }
    else
    {
        printf("screen_y error");
    }

    lua_getfield(L, -1, "aimbot_vk_code");
    if (lua_isinteger(L, -1))
    {
        aimbot_vk_code = lua_tointeger(L, -1);
        printf("aimbot_vk_code %d\n", aimbot_vk_code);
        lua_pop(L, 1);
    }
    else
    {
        printf("aimbot_vk_code error");
    }

    lua_getfield(L, -1, "exitpossprocess_vk_code");
    if (lua_isinteger(L, -1))
    {
        exitpossprocess_vk_code = lua_tointeger(L, -1);
        printf("exitpossprocess_vk_code %d\n", exitpossprocess_vk_code);
        lua_pop(L, 1);
    }
    else
    {
        printf("exitpossprocess_vk_code error");
    }

    lua_getfield(L, -1, "move");
    if (lua_isinteger(L, -1))
    {
        move_len = lua_tointeger(L, -1);
        printf("move %d\n", move_len);
        lua_pop(L, 1);
    }
    else
    {
        printf("move error");
    }


    printf("C++");
    system("pause");
    lua_register(L, "Aimbot_run", csgo_aimbot_run_lua);
    lua_register(L, "InstallDriver", csgo_InstallDriver_lua);
    lua_register(L, "UnInstallDriver", csgo_UnInstallDriver_lua);
    printf("C++");
    return 0;
}