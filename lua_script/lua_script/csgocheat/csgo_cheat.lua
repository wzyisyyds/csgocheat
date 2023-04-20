CheatConfig = {
    screen_x=1440,--游戏分辨率
    screen_y=1080,--
    aimbot_vk_code=162,--vk_code，按下自瞄
    exitpossprocess_vk_code=0xA1,--vk_code，退出程序
    move=20000 --范围1000-2000000
}

dll_init = package.loadlib ("csgocheat", "lua_dll_init")
pcall(dll_init,1,0,0)

os.execute("pause")
--安装驱动，会重启电脑
--InstallDriver()

--运行aimbot
--Aimbot_run()


--卸载驱动，会重启电脑

--UnInstallDriver()

os.execute("pause")