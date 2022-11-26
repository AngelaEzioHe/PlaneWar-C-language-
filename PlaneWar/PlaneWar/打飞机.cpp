//头文件
#include<stdio.h>
#include<graphics.h>    //图形库，帮助小白快速入门：easyX
#include<conio.h>
#include<time.h>

//得分
int grade = 0;

//定义窗口大小
enum My
{
	WIDTH=480,//窗口宽度
	HEIGHT=852,//窗口高度
	BULLET_NUM=15,//玩家子弹数量
	ENEMY_NUM=10,//敌机数量
	BIG,
	SMALL
};

//飞机结构（坐标及生存状况）
struct Plane
{
	int x;
	int y;
	bool live;//是否存活
	int width;
	int height;
	int hp;
	int type;//敌机类型
}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

IMAGE bk;//保存背景的变量
IMAGE img_role[2];//保存玩家图片的变量  player
IMAGE img_bullet[2];//保存子弹的变量
IMAGE img_enemy[2][2];//保存敌机的变量

//加载背景、飞机的图片（将文件夹里的图片储存到变量里
void loadImg()
{

	//加载背景图片
	loadimage(&bk, "./picture/background.png");//分辨率：480*852
	//可能会报错
	//参数有误，字符集不对，不应是"unicode"，而是"多字节字符集"（项目->属性->高级）
	
	//加载玩家图片
	loadimage(&img_role[0], "./picture/hero.png");
	loadimage(&img_role[1], "./picture/hero.png");

	//加载子弹图片
	loadimage(&img_bullet[0], "./picture/bullet1.png");
	loadimage(&img_bullet[1], "./picture/bullet1.png");

	//加载敌机图片
	loadimage(&img_enemy[0][0], "./picture/enemy1.png");
	loadimage(&img_enemy[0][1], "./picture/enemy1.png");
	loadimage(&img_enemy[1][0], "./picture/enemy1_down1.png");
	loadimage(&img_enemy[1][1], "./picture/enemy1_down1.png");
}

//定义敌机的出现概率。血量、大小
void enemyHp(int i)
{
	int flag = rand() % 10;
	if (flag >= 0 && flag <=3)//未损坏的飞机
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
	else//部分损坏的飞机
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
}

//玩家飞机的坐标及生存状态
void gameInit()
{
	loadImg();

	//初始化玩家飞机位置
	player.x = WIDTH/2;
	player.y = HEIGHT-120;
	
	//初始化子弹
	player.live = true;
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].x = 0;//初始化子弹位置
		bullet[i].y = 0;//初始化子弹位置
		bullet[i].live = false;
	}

	//初始化敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
		enemyHp(i);
	}
	
}

//游戏的图片绘制函数
void gameDraw()
{
	//把背景图片贴到窗口上
	putimage(0, 0, &bk);

	//绘制玩家飞机
	putimage(player.x , player.y , &img_role[0]);
	putimage(player.x , player.y , &img_role[1]);
	
	//绘制子弹
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			putimage(bullet[i].x, bullet[i].y, &img_bullet[0]);
			putimage(bullet[i].x, bullet[i].y, &img_bullet[1]);
		}
	}

	//绘制敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == BIG)
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0]);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1]);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0]);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1]);
			}
		}
	}
}

//生成子弹
void createBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = (player.x+51);//实际发射子弹时根据玩家飞机位置而确定
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}
	}
}

//子弹移动函数
void BulletMove(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].y -= speed;
			if (bullet[i].y < 0)
			{
				bullet[i].live = false;
			}
		}
	}
}

//定义定时器
bool Timer(int ms, int id)
{
	static DWORD t[10];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}

	return false;
}

//角色移动，获取键盘消息，上，下，左，右（注意边界处理）
void playerMove(int speed)
{
	#if 0
	//检测是否有按键按下，如果有键按下，则返回真，否则返回假
	if (_kbhit())
	{
		//有两种方式
		//1. _getch() 阻塞函数，和scanf一样，如果没有输入，就会卡主程序，一直等待输入（不是C语言标准函数，须包含头文件<conio.h>）		
		char key = _getch();
		
		switch (key)
		{
		case 'w':
		case 'W':
			player.y -= speed;
			break;
		case 's':
		case 'S':
			player.y += speed;
			break;
		case 'a':
		case 'A':
			player.x -= speed;
			break;
		case 'd':
		case 'D':
			player.x += speed;
			break;
		}
	}
	#elif 1
	//2.使用Windows函数获取键盘输入 非阻塞函数，会非常流畅
	//VK_UP是一个宏
	//如果要检测字母按键，必须用大写，这样大写小写都可以检测到；如果用小写，那么大写小写都检测不到
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))     
	{
		if (player.y > 0)//到上边缘停住
		{
			player.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (player.y < (HEIGHT-126))//到下边缘停住
		{
			player.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x > -51)//为了让半个飞机出左窗口，使得飞机可以打到窗口边缘的敌人
		{
		player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x < (WIDTH-51))//为了让半个飞机出右窗口，使得飞机可以打到窗口边缘的敌人
		{
			player.x += speed;
		}
	}
	#endif
	if (GetAsyncKeyState(VK_SPACE) && Timer(100,1))//定时器函数，隔200毫秒才可发射下一个子弹（防止子弹过于密集）
	{
		//创建一个子弹
		createBullet();
	}
}

//产生敌机
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH-57);
			enemy[i].y = 0;
			enemyHp(i);//重置敌机的血量
			break;
		}
	}
}

//敌机的移动
void EnemyMove(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT)
			{
				enemy[i].live = false;
			}
		}
	}
}

//敌机死亡判断
void playPlane()
{
	
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			continue;
		}
		for (int k = 0; k < BULLET_NUM; k++)
		{
			if (!bullet[k].live)
			{
				continue;
			}
			//如果子弹碰到敌机
			if (bullet[k].x > enemy[i].x && bullet[k].x < enemy[i].x + enemy[i].width 
			 && bullet[k].y > enemy[i].y && bullet[k].y < enemy[i].y + enemy[i].height)
			{
				bullet[k].live = false;
				enemy[i].hp --;
			}
		}
		if (enemy[i].hp <= 0)
		{
			enemy[i].live = false;
			grade++;
			printf("得分：%d\n", grade);
		}
	}
}

//主函数
int main()
{
	
	//创建一个窗口 控制台窗口是自动创建的，图形窗口需要开发者手动创建
	initgraph(WIDTH, HEIGHT,SHOWCONSOLE);//创建一个图形窗口（即游戏界面）长：640 宽：480
	gameInit();
	
	//双缓冲绘图
	BeginBatchDraw();	
	while (1)
	{
		gameDraw();	
		FlushBatchDraw();
		playerMove(1);
		BulletMove(2);

		if (Timer(500,0))//定时器（每过500毫秒产生一个敌机）
		{
			createEnemy();
		}
		if (Timer(10, 2))
		{
			EnemyMove(1);
		}
		playPlane();
	}
	EndBatchDraw();
	
	return 0;
}