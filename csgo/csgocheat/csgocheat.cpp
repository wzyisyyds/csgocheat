#include"header.h"
#include<queue>



int player_size = 0;

EntityList players[32];


std::vector<playerinfo> emeny;

playerinfo me_play;

EXTERN_C bool aimbotflag = false;
EXTERN_C bool exitpossprocess = false;
EXTERN_C HHOOK keyboardHook = 0;

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
    float screen_x = 1920 * 0.5f;
    float screen_y = 1080 * 0.5f;

    screen_x = (0.5 * out.x * 1920) + out.x ;
    screen_y = -(0.5 * out.y * 1080) + out.y ;
    out.x = screen_x;
    out.y = screen_y;

    if ((out.x* out.x+ out.y* out.y)>1000)
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
        Sleep(1);
        if (exitpossprocess)
        {
            break;
        }
    }

    
    ExitThread(0);
}




void run() 
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
            int len = 10000000;
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





int main()
{
    int chiose = 0;
    std::cout << "输入\n0：启动程序\n1：安装驱动\n2：卸载驱动\n输入选择：" << std::endl;
    std::cin >> chiose;
    switch (chiose)
    {
    case 0:
        run();
        break;
    case 1:
        InstallDriver("yexi", "yexi.sys");
        break;
    case 2:
        UnInstallDriver("yexi");
        break;
    default:
        break;
    }
    system("pause");
    system("Cls");

    return 0;
}

