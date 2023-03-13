
#include"ReadMemory.h";

static HINSTANCE hdll = 0;

static GetModuleBaseAddr dllGetModuleBaseAddr = NULL;

static ReadMemory dllReadMemory = NULL;

static ULONG_PTR csgo_client_dll_base = NULL;

static HANDLE csgoid;
//fun

static ULONG_PTR GetclientBaseAddr(HANDLE hProcessID);



bool dll_init(PVOID addr)
{
    hdll = LoadLibraryW(L"yexi.dll");
    if (hdll == NULL)
    {
        return false;
    }



    dllGetModuleBaseAddr = (GetModuleBaseAddr)GetProcAddress(hdll, "GetModuleBaseAddr");
    if (dllGetModuleBaseAddr == NULL)
    {
        return false;
    }
    dllReadMemory = (ReadMemory)GetProcAddress(hdll, "ReadMemory");
    if (dllReadMemory == NULL)
    {
        return false;
    }

    HWND hWnd = FindWindowA(nullptr, "Counter-Strike: Global Offensive - Direct3D 9");
    DWORD dwPid = -1;
    GetWindowThreadProcessId(hWnd, &dwPid);
    csgoid = (HANDLE)dwPid;
    csgo_client_dll_base = GetclientBaseAddr(csgoid);

    PCHAR out_buffer = new CHAR[0x5000];

    if (!dllReadMemory(csgoid, out_buffer, (PVOID)(csgo_client_dll_base+hazedumper::signatures::dwLocalPlayer), sizeof(CHAR) * 0x120))
    {
        delete[] out_buffer;
        return false;
    }

    ((playerinfo*)(addr))->addr = (PVOID)( * ((UINT32*)out_buffer));

    delete[] out_buffer;
    std::cout << "csgo_client_dll_base:" << std::hex << csgo_client_dll_base << std::endl;

    return true;
}

bool read_EntityList(PVOID out_buffer)
{
    if (dllReadMemory == NULL)
    {
        return false;
    }

    if (!dllReadMemory(csgoid,out_buffer, (PVOID)(csgo_client_dll_base+ hazedumper::signatures::dwEntityList) ,sizeof(EntityList)*32))
    {
        return false;
    }

    return true;
}

bool read_playerinfo(playerinfo& player_info)
{
    if (dllReadMemory == NULL)
    {
        return false;
    }

    PCHAR out_buffer = new CHAR[0x5000];

    if (!dllReadMemory(csgoid, out_buffer, player_info.addr, sizeof(CHAR)*0x120))
    {
        delete[] out_buffer;
        return false;
    }

    player_info.health = *((int*)(out_buffer + hazedumper::netvars::m_iHealth));
    player_info.team= *((int*)(out_buffer + hazedumper::netvars::m_iTeamNum));
    player_info.dormant = out_buffer + hazedumper::signatures::m_bDormant;
    player_info.vec.x = *((float*)(out_buffer + 0xa0));
    player_info.vec.y = *((float*)(out_buffer + 0xa4));
    player_info.vec.z = *((float*)(out_buffer + 0xa8));

    
    if (!dllReadMemory(csgoid, out_buffer, (PVOID)((UINT32)player_info.addr +hazedumper::netvars::m_dwBoneMatrix), sizeof(char) * 16))
    {
        delete[] out_buffer;
        return false;
    }
    if (!dllReadMemory(csgoid, out_buffer, (PVOID)(( * ((UINT32*)out_buffer))+ 0x180), sizeof(float) * 16))
    {
        delete[] out_buffer;
        return false;
    }

    float* temp= (float*)out_buffer;


    player_info.head.x = temp[3];
    player_info.head.y = temp[7];
    player_info.head.z = temp[11];

    delete[] out_buffer;
    return true;
}

bool read_ViewMatrix(PVOID buffer)
{
    if (dllReadMemory == NULL)
    {
        return false;
    }

    if (!dllReadMemory(csgoid, buffer, (PVOID)(csgo_client_dll_base + hazedumper::signatures::dwViewMatrix), sizeof(float) * 16))
    {
        return false;
    }

    return true;
}



ULONG_PTR GetclientBaseAddr(HANDLE hProcessID)
{
    ULONG_PTR ret = NULL;
    if (dllGetModuleBaseAddr !=NULL)
    {
        char name[16] = "client.dll";
        ret = dllGetModuleBaseAddr(hProcessID, name, 16);
    }
    return ret;
}

