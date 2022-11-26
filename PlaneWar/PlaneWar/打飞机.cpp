//ͷ�ļ�
#include<stdio.h>
#include<graphics.h>    //ͼ�ο⣬����С�׿������ţ�easyX
#include<conio.h>
#include<time.h>

//�÷�
int grade = 0;

//���崰�ڴ�С
enum My
{
	WIDTH=480,//���ڿ��
	HEIGHT=852,//���ڸ߶�
	BULLET_NUM=15,//����ӵ�����
	ENEMY_NUM=10,//�л�����
	BIG,
	SMALL
};

//�ɻ��ṹ�����꼰����״����
struct Plane
{
	int x;
	int y;
	bool live;//�Ƿ���
	int width;
	int height;
	int hp;
	int type;//�л�����
}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

IMAGE bk;//���汳���ı���
IMAGE img_role[2];//�������ͼƬ�ı���  player
IMAGE img_bullet[2];//�����ӵ��ı���
IMAGE img_enemy[2][2];//����л��ı���

//���ر������ɻ���ͼƬ�����ļ������ͼƬ���浽������
void loadImg()
{

	//���ر���ͼƬ
	loadimage(&bk, "./picture/background.png");//�ֱ��ʣ�480*852
	//���ܻᱨ��
	//���������ַ������ԣ���Ӧ��"unicode"������"���ֽ��ַ���"����Ŀ->����->�߼���
	
	//�������ͼƬ
	loadimage(&img_role[0], "./picture/hero.png");
	loadimage(&img_role[1], "./picture/hero.png");

	//�����ӵ�ͼƬ
	loadimage(&img_bullet[0], "./picture/bullet1.png");
	loadimage(&img_bullet[1], "./picture/bullet1.png");

	//���صл�ͼƬ
	loadimage(&img_enemy[0][0], "./picture/enemy1.png");
	loadimage(&img_enemy[0][1], "./picture/enemy1.png");
	loadimage(&img_enemy[1][0], "./picture/enemy1_down1.png");
	loadimage(&img_enemy[1][1], "./picture/enemy1_down1.png");
}

//����л��ĳ��ָ��ʡ�Ѫ������С
void enemyHp(int i)
{
	int flag = rand() % 10;
	if (flag >= 0 && flag <=3)//δ�𻵵ķɻ�
	{
		enemy[i].type = BIG;
		enemy[i].hp = 3;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
	else//�����𻵵ķɻ�
	{
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
}

//��ҷɻ������꼰����״̬
void gameInit()
{
	loadImg();

	//��ʼ����ҷɻ�λ��
	player.x = WIDTH/2;
	player.y = HEIGHT-120;
	
	//��ʼ���ӵ�
	player.live = true;
	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].x = 0;//��ʼ���ӵ�λ��
		bullet[i].y = 0;//��ʼ���ӵ�λ��
		bullet[i].live = false;
	}

	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
		enemyHp(i);
	}
	
}

//��Ϸ��ͼƬ���ƺ���
void gameDraw()
{
	//�ѱ���ͼƬ����������
	putimage(0, 0, &bk);

	//������ҷɻ�
	putimage(player.x , player.y , &img_role[0]);
	putimage(player.x , player.y , &img_role[1]);
	
	//�����ӵ�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].live)
		{
			putimage(bullet[i].x, bullet[i].y, &img_bullet[0]);
			putimage(bullet[i].x, bullet[i].y, &img_bullet[1]);
		}
	}

	//���Ƶл�
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

//�����ӵ�
void createBullet()
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x = (player.x+51);//ʵ�ʷ����ӵ�ʱ������ҷɻ�λ�ö�ȷ��
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}
	}
}

//�ӵ��ƶ�����
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

//���嶨ʱ��
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

//��ɫ�ƶ�����ȡ������Ϣ���ϣ��£����ң�ע��߽紦��
void playerMove(int speed)
{
	#if 0
	//����Ƿ��а������£�����м����£��򷵻��棬���򷵻ؼ�
	if (_kbhit())
	{
		//�����ַ�ʽ
		//1. _getch() ������������scanfһ�������û�����룬�ͻῨ������һֱ�ȴ����루����C���Ա�׼�����������ͷ�ļ�<conio.h>��		
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
	//2.ʹ��Windows������ȡ�������� ��������������ǳ�����
	//VK_UP��һ����
	//���Ҫ�����ĸ�����������ô�д��������дСд�����Լ�⵽�������Сд����ô��дСд����ⲻ��
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))     
	{
		if (player.y > 0)//���ϱ�Եͣס
		{
			player.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if (player.y < (HEIGHT-126))//���±�Եͣס
		{
			player.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if (player.x > -51)//Ϊ���ð���ɻ����󴰿ڣ�ʹ�÷ɻ����Դ򵽴��ڱ�Ե�ĵ���
		{
		player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if (player.x < (WIDTH-51))//Ϊ���ð���ɻ����Ҵ��ڣ�ʹ�÷ɻ����Դ򵽴��ڱ�Ե�ĵ���
		{
			player.x += speed;
		}
	}
	#endif
	if (GetAsyncKeyState(VK_SPACE) && Timer(100,1))//��ʱ����������200����ſɷ�����һ���ӵ�����ֹ�ӵ������ܼ���
	{
		//����һ���ӵ�
		createBullet();
	}
}

//�����л�
void createEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH-57);
			enemy[i].y = 0;
			enemyHp(i);//���õл���Ѫ��
			break;
		}
	}
}

//�л����ƶ�
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

//�л������ж�
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
			//����ӵ������л�
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
			printf("�÷֣�%d\n", grade);
		}
	}
}

//������
int main()
{
	
	//����һ������ ����̨�������Զ������ģ�ͼ�δ�����Ҫ�������ֶ�����
	initgraph(WIDTH, HEIGHT,SHOWCONSOLE);//����һ��ͼ�δ��ڣ�����Ϸ���棩����640 ��480
	gameInit();
	
	//˫�����ͼ
	BeginBatchDraw();	
	while (1)
	{
		gameDraw();	
		FlushBatchDraw();
		playerMove(1);
		BulletMove(2);

		if (Timer(500,0))//��ʱ����ÿ��500�������һ���л���
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