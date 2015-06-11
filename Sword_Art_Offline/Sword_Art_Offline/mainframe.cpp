#include "mainframe.h"
#include "player.h"
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include "Vfw.h"
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "Vfw32.lib")

#define INIT_ORIGINX 0

MAINFRAME::MAINFRAME()
{
	originx = INIT_ORIGINX;
}

MAINFRAME::~MAINFRAME() {}

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000?1:0)  //检测按键和鼠标点击

#define maxx 1072       //初始化屏幕宽高
#define maxy 600
#define inter 10        //初始化Sleep的间隔

void main()
{
	MAINFRAME mainFrame;
	mainFrame.welcomeInit();
}

void headLine(char *SAO, char *ETS,char *blank,bool flagstart,bool blink)
{
	settextstyle(112, 30, _T("SAO UI"));	// 设置大标题字体
	settextcolor(WHITE);
	outtextxy(200, 180, SAO);

	if (blink == 1 || flagstart == 1){
		if (flagstart == 1)settextcolor(YELLOW);
		settextstyle(50, 13, _T("SAO UI"));   //设置ETS字体
		outtextxy(411, 400, ETS);
	}
	else{
		settextstyle(50, 13, _T("SAO UI"));
		outtextxy(411, 400, blank);
	}                                      //实现ETS的闪烁
	settextcolor(YELLOW);
	setlinecolor(BLACK);
	settextstyle(30, 8, _T("SAO UI"));	// 重新设置为背景字体
}

void clearline(int i)                    //实现背景空白行扫描
{
	line(i, 0, i, 599);
	line(i + 536, 0, i + 536, 599);
	line(i - 536, 0, i - 536, 599);
}

void M_clear(POINT pt, IMAGE *bk, IMAGE pic)//pt上一个动作图片的输出坐标，bk背景图片
{
	IMAGE clear;
	SetWorkingImage(bk);//设定当前的绘图设备为背景图片
	getimage(&clear, pt.x, pt.y, pic.getwidth(), pic.getheight());//获取图像
	SetWorkingImage();//设定回默认绘图窗口
	putimage(pt.x, pt.y, &clear);//输出
}

void MAINFRAME::stillput(bool dir,int x,int y,int ox, IMAGE *player,int type)
{
	switch (type){
	case 0:{
		switch (dir){
		case 0:{
			loadimage(player, "pic/lstill.jpg",180,185,true);
			mainFrame::M_putimg(x, y, player, WHITE, 100, ox);
		}break;
		case 1:{
			loadimage(player, "pic/rstill.jpg", 180, 185, true);
			mainFrame::M_putimg(x, y, player, WHITE, 100, ox);
		}break;
		}
	}break;
	case 1:{
		switch (dir){
		case 0:{
			loadimage(player, "pic/les.jpg", 180, 185, true);
			mainFrame::M_putimg(x, y,player, WHITE, 100, ox);
		}break;
		case 1:{
			loadimage(player, "pic/res.jpg", 180, 185, true);
			mainFrame::M_putimg(x, y, player, WHITE, 100, ox);
		}break;
		}
	}break;

	}
}

void MAINFRAME::copy_img(IMAGE* img1, IMAGE* img2)
{
	//copy img2 to img1
	IMAGE* now_working = GetWorkingImage();
	Resize(img1, img2->getwidth(), img2->getheight());
	SetWorkingImage(img2);
	getimage(img1, 0, 0, img1->getwidth(), img1->getheight());
	SetWorkingImage(now_working);
}

int MAINFRAME::getOriginx()
{
	return originx;
}

void MAINFRAME::setOriginx(int num)
{
	originx = num;
}

void MAINFRAME::screenMove(int x, int spd)
{
	if (x > (700 - originx)){              //右移动边界
		originx -= abs(spd);              //屏幕区域位置改变
	}
	if (x < (200 - originx)){              //左移动边界
		originx +=abs(spd);
	}

	if (originx > 0)originx = 0;           //限制originx>=0
	if (originx < -2144)originx = -2144;     //限制originx<=2144
	setorigin(originx, 0);                 //重设原点
}

void MAINFRAME::welcomeInit()
{
	bool flagstart = false;
	bool flagsound = true;
	bool blink = true;
	int tic = 0;
	char blank[47] = "                                              ";
	HWND hw = GetHWnd();
	
	// 设置随机函数种子
	srand((unsigned)time(NULL));

	// 初始化图形模式
	initgraph(maxx, maxy);
	POINT pt;								 //鼠标位置
	int  x, y;
	char c;								     //随机字母
	char headline[] = "s w o r d    a r t    o f f l i n e";
	char ETS[] = "PRESS ENTER TO START";
	
	//bgm(0);                                  //播放crossing_field.mp3
	
	settextstyle(30, 8, _T("SAO UI"));       //初始化背景字体和字母颜色
	settextcolor(YELLOW);
	setlinecolor(BLACK);

	for (int i = 0; i <= maxx - 1; i++)     /***********主循环开始***********/
	{
		hw = GetHWnd();
		//计时用变量
		tic++;
		// 在随机位置显示三个随机字母
		for (int j = 0; j < 3; j++)
		{
			x = (rand() % 134) * 8;//随机横坐标
			y = (rand() % 20) * 30;//随机纵坐标
			c = (rand() % 26) + 65;//随机字母
			outtextxy(x, y, c);
		}

		// 画线擦掉像素行
		clearline(i);
		//ETS闪烁
		if (tic == 50){
			blink = !blink;
			tic = 0;
		}

		//获取鼠标位置
		GetCursorPos(&pt);
		//将获取到的鼠标屏幕位置转换为窗口内位置
		ScreenToClient(hw, &pt);
		//ETS的鼠标响应判断
		if (pt.x >= 411 && pt.y >= 400 && pt.x <= 679 && pt.y <= 450){
			flagstart = 1;
			if (flagsound == 1){
				sound(1);
				flagsound = 0;
			}
			if (KEY_DOWN(VK_LBUTTON)){
				sound(0);
				bgm(9);
				unlimitedMode();
			}

		}
		else{
			flagstart = 0;
			flagsound = 1;
		}
		
		headLine(headline, ETS,blank,flagstart,blink);//输出标题和ETS

		Sleep(10);	                        // 延时(控制FPS为100）

		if (i >= maxx - 1)	i = -1;		    // 循环处理

		if (KEY_DOWN(VK_RETURN)){           //ENTER键开始新游戏（暂定）
			sound(0);
			bgm(9);
		    unlimitedMode();
		}
		if (KEY_DOWN(VK_ESCAPE)){           //按ESC键退出
			bgm(9);
			gameExit();
		}
	}/******************主循环结束*******************/

	// 关闭图形模式
	closegraph();
}

int MAINFRAME::unlimitedMode()
{
	IMAGE background, wbackground, welcome, player, skillpic250, skillpic300, enemyplayer, hpUI;
	POINT pt;    //定义清理图像指针
	pt.x = 0;   //清理图像指针赋值
	pt.y = 0;
	
	//link start!
	//HWND hwnd = MCIWndCreate(GetHWnd(), NULL, WS_CHILD | WS_VISIBLE | MCIWNDF_NOMENU | MCIWNDF_NOPLAYBAR, NULL);
	//MCIWndOpen(hwnd, "avi\\loadgame.wmv", NULL);
	//MCIWndPlay(hwnd);
	//Sleep(17000);

	settextstyle(30, 8, _T("SAO UI"));       //初始化背景字体和字母颜色
	settextcolor(YELLOW);
	setlinecolor(BLACK);

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

	setorigin(originx, 0);//设置初始原点

	// 加载通用图片
	loadimage(&background, "pic/blank.jpg");	// 请确保该图片是 1072*600 像素
	loadimage(&wbackground, "pic/whitebk.jpg");

	// 随机BGM
	int m = ((rand() % 6) + 2);
	bgm(m);

	//计算时间用参数
	int tik = 0;
	int comboclear = 0;
	int mobRefresh = 0;

	//复活吧，我的勇士
	PLAYER kirito;
	//为你而战，我的女士
	PLAYER enemy;
	int newx = (rand() % 3016); //随机新敌人的X
	enemy.teleport(newx, 400);
	int newdir = (rand() % 2);  //随机新敌人的朝向
	enemy.setDir(newdir);
	enemy.setHp(100);
	//世界筑造
		
	//开始批量绘图
	BeginBatchDraw();  

	while (1){      /**********************************游戏主循环******************************/

		//背景图
		putimage(0, 0, &background);

		//死亡判定
		if (kirito.getHp() <= 0)kirito.teleport(0,-200);  
		if (enemy.getHp() <= 0&&mobRefresh==0){
			enemy.teleport(0, -200);
			mobRefresh = 100;
		}
		//怪物刷新判定
		if (mobRefresh > 0){
			mobRefresh--;
			if (mobRefresh == 1){
				int newx = (rand() % 3016);
				enemy.teleport(newx, 400);
				int newdir = (rand() % 2);
				enemy.setDir(newdir);
				enemy.setHp(100);
			}
		}
		//摩擦力(Kirito和enemy)
		if(kirito.getMovespd()>0&&!kirito.jumpJudge())
			kirito.setSpd(kirito.getMovespd() - 1);
		if (kirito.getMovespd()<0&&!kirito.jumpJudge())
			kirito.setSpd(kirito.getMovespd() + 1);
		if (enemy.getMovespd()>0 && !enemy.jumpJudge())
			enemy.setSpd(enemy.getMovespd() - 1);
		if (enemy.getMovespd()<0 && !enemy.jumpJudge())
			enemy.setSpd(enemy.getMovespd() + 1);
		//HPUI
		int khp = kirito.getHp(), kmhp = kirito.getMaxHp();
		mainFrame::hpUI(&khp, &kmhp);
		int ehp = enemy.getHp(), emhp = enemy.getMaxHp(), ex = enemy.getX(),ey=enemy.getY();
		enemyHpUI(&ehp, &emhp, &ex,&ey);
		//kirito移动
		kirito.moveX();
		enemy.moveX();
		//检测人物位置，移动屏幕
		screenMove(kirito.getX(), kirito.getMovespd());

		//静止敌人图片
		if (enemy.stillJudge()){
			stillput(enemy.getDir(), enemy.getX(), enemy.getY(), originx, &enemyplayer, 1); 
		}

		//当有键盘输入时执行
		if (_kbhit()){

			if (KEY_DOWN('J') && kirito.stillJudge()&&(!kirito.coolingJudge())){ 
				int ex = enemy.getX(), eh = enemy.getHp();																	//普通攻击"J"
				kirito.meleeAttack(&ex, &eh, &enemy, &player, &enemyplayer, &skillpic250, &skillpic300, originx);
				sound(2);
			}

			if (KEY_DOWN('K') && kirito.stillJudge() && (!kirito.coolingJudge())){
				int ex = enemy.getX(), eh = enemy.getHp();																	//音速冲击"K"
				kirito.sonicLeap(&ex, &eh, &enemy, &player, &enemyplayer, &skillpic250, &skillpic300, originx);
				sound(2);
			}
			else{
				if (KEY_DOWN(VK_SPACE) && (!kirito.jumpJudge())){        //按Space跳跃
					kirito.startJump();
				}
	                     
				if (KEY_DOWN('A')){                                       //按A向左移动
					kirito.setDir(0);         //改变方向
					kirito.setStill(-5);         //开始跑动
					kirito.setRunState(1);	 //使runState为1
					kirito.setSpd(-10);         //使kirito获得速度
					if (KEY_DOWN(VK_SHIFT))     //配合shift食用更佳
						kirito.setSpd(-15);
					}
				if (KEY_DOWN('D')){                                       //按D向右移动
					kirito.setDir(1);         //改变方向
					kirito.setStill(-5);         //开始跑动
					kirito.setRunState(1);	 //使runState为1
					kirito.setSpd(10);
					if (KEY_DOWN(VK_SHIFT))
						kirito.setSpd(15);
					}
					
				if (KEY_DOWN(VK_ESCAPE)){
					bgm(10);
					return 0;
				}//ESC退出
				}
		}

		//若kirito处于跳跃状态，进行Y值改变和绘图
		if (kirito.jumpJudge()){    
			kirito.jump();
			switch (kirito.getDir()){
			case 0:loadimage(&player, "pic/ll4.jpg"); break;
			case 1:loadimage(&player, "pic/rr4.jpg"); break;
			}
			M_putimg(kirito.getX(), kirito.getY(), &player, WHITE, 100, originx);
			
		}
		//若kirito处于跑动状态，进行跑步绘图
		if (kirito.runJudge() && !kirito.jumpJudge() && !kirito.stillJudge()){
			kirito.running(&player, originx);
		}
		//技能释放时的姿势和特效绘制
		kirito.skillEffect(&skillpic250,&skillpic300,&player,originx);
	
		//between
		if (!kirito.stillJudge()){
			kirito.setStill(kirito.getStill() + 1);
			M_putimg(kirito.getX(), kirito.getY(), &player, WHITE, 100, originx);
		}
		//静止时的putimg
		if (kirito.stillJudge() && !(kirito.getSkillState()) && !kirito.jumpJudge()){
			kirito.setRunState(1);
			kirito.setSkillType(0);
			kirito.setRun(0);
			comboclear++;
			if (comboclear == 20){
				comboclear = 0;
				kirito.setCombo(0);
			}
			stillput(kirito.getDir(),kirito.getX(),kirito.getY(),originx, &player,0);   //静止角色图片
		}
		
		

		//冷却时间减少
		if (kirito.coolingJudge())
			kirito.coolingDown();
		if (kirito.getSkillState() > 0)
			kirito.skillStateMove(); 

        FlushBatchDraw();      //绘制结果输出
		Sleep(inter);          //控制帧率
		//M_clear(pt, &background,background);    //清空画面
		cleardevice();

	}//***************************************主循环结束*******************************************

	closegraph();
	
}

void MAINFRAME::bgm(int song)
{
	switch (song){
	case 0:{
		mciSendString(TEXT("open bgm/crossing_field.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 1:{
		mciSendString(TEXT("open bgm/Swordland.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 2:{
		mciSendString(TEXT("open bgm/The_First_Town.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 3:{
		mciSendString(TEXT("open bgm/Survive_The_Swordland.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 4:{
		mciSendString(TEXT("open bgm/Luminous_Sword.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 5:{
		mciSendString(TEXT("open bgm/light_your_sword.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 6:{
		mciSendString(TEXT("open bgm/Everyday_Life.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;
	case 7:{
		mciSendString(TEXT("open bgm/She_has_to_overcome_her_fear.mp3 alias MySong"), NULL, 0, NULL);
		mciSendString(TEXT("play MySong"), NULL, 0, NULL);
	}break;

	case 10:{
		mciSendString(TEXT("close MySong"), NULL, 0, NULL);
	}
	case 9:{
		mciSendString(TEXT("stop MySong"), NULL, 0, NULL);
	}
	default:break;
	}
}

void MAINFRAME::sound(int soundtype)
{
	switch (soundtype){
	case 0:{
		PlaySound("sound/boot.wav", NULL, SND_FILENAME | SND_ASYNC);
	}break;
	case 1:{
		PlaySound("sound/press.wav", NULL, SND_FILENAME | SND_ASYNC);
	}break;
	case 2:{
		PlaySound("sound/melee.wav", NULL, SND_FILENAME | SND_ASYNC);
	}break;
	case 3:{
		PlaySound("sound/welcome.wav", NULL, SND_FILENAME | SND_ASYNC);
	}break;
	
	default:break;
	}
}

void MAINFRAME::gameExit()
{
	bgm(9);
	closegraph();
}

void MAINFRAME::hpUI(int *hp, int *maxHp)
{
	IMAGE hpTIAO, hpCAO;
	float hpRatio = (1.0*(*hp)) / (1.0*(*maxHp));
	int hpPix = hpRatio * 258;

	if (hpRatio > 0.66)								//根据血量加载适应颜色的hpTIAO
		loadimage(&hpTIAO, "pic/hp_green.jpg",hpPix,24,true);
	if (hpRatio > 0.33&&hpRatio <= 0.66)
		loadimage(&hpTIAO, "pic/hp_yellow.jpg", hpPix, 24, true);
	if (hpRatio <= 0.33)
		loadimage(&hpTIAO, "pic/hp_red.jpg", hpPix, 24, true);
	loadimage(&hpCAO, "pic/hp_bar.jpg");			//加载hpCAO
	//图像输出
	M_putimg(75-originx, 12, &hpTIAO, WHITE , 100 , originx);
	M_putimg(0 - originx, 0, &hpCAO, WHITE, 80, originx);
}

void MAINFRAME::enemyHpUI(int *hp, int *maxHp,int *x,int *y)
{
	IMAGE hpTIAO;
	float hpRatio = (1.0*(*hp)) / (1.0*(*maxHp));
	int hpPix = hpRatio * 180;
	if (hpRatio > 0.66)								//根据血量加载适应颜色的hpTIAO
		loadimage(&hpTIAO, "pic/hp_enemy_green.jpg", hpPix, 24, true);
	if (hpRatio > 0.33&&hpRatio <= 0.66)
		loadimage(&hpTIAO, "pic/hp_enemy_yellow.jpg", hpPix, 24, true);
	if (hpRatio <= 0.33)
		loadimage(&hpTIAO, "pic/hp_enemy_red.jpg", hpPix, 24, true);
	//图像输出
	M_putimg(*x, *y-20, &hpTIAO, WHITE, 100, originx);
}

void MAINFRAME::M_putimg(int dstX, int dstY, IMAGE *pimg, int avoid_color, int tp, int originx)
{
	//排除颜色avoid_color,容差为deviation；透明度tp(transparency)从0到100
	setorigin(originx, 0);
	int x, y, num;
	int deviation = 100;
	int R, G, B;//记录贴图某点色彩
	//记录排除颜色色彩
	int avoid_r = GetRValue(avoid_color);
	int avoid_g = GetGValue(avoid_color);
	int avoid_b = GetBValue(avoid_color);
	IMAGE pSrcImg;//背景图
	IMAGE tempimg;//临时贴图
	mainFrame::copy_img(&tempimg, pimg);//保护原图
	SetWorkingImage(NULL);//对默认绘图窗口的绘图操作
	getimage(&pSrcImg, dstX, dstY, pimg->getwidth(), pimg->getheight());

	//透明度容错
	if (tp<0)
	{
		tp = 0;
	}
	else if (tp>100)
	{
		tp = 100;
	}

	// 获取背景指向显存的指针
	DWORD* bk_pMem = GetImageBuffer(&pSrcImg);

	//贴图指向显存的指针
	DWORD* pMem = GetImageBuffer(&tempimg);

	for (y = 0; y<pimg->getheight(); y++)
	{
		for (x = 0; x<pimg->getwidth(); x++)
		{
			num = y*pimg->getwidth() + x;
			R = GetRValue(pMem[num]);
			G = GetGValue(pMem[num]);
			B = GetBValue(pMem[num]);
			if ((abs(R - avoid_r) <= deviation) && (abs(G - avoid_g) <= deviation) && (abs(B - avoid_b) <= deviation))
			{
				pMem[num] = bk_pMem[num];
			}
			else
			{
				pMem[num] = RGB((R*tp + GetRValue(bk_pMem[num])*(100 - tp)) / 100, (G*tp + GetGValue(bk_pMem[num])*(100 - tp)) / 100, (B*tp + GetBValue(bk_pMem[num])*(100 - tp)) / 100);
			}
		}
	}
	putimage(dstX, dstY, &tempimg);
}
