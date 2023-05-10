#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<stdio.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
enum My
{
	//���ô��ڴ�С
	WIDTH = 480,
	HEIGTH = 700,
	//�ӵ�����
	BULLELT_NUM = 30,
	ENEMY_NUM = 20,//�л�����

	BIG,//��ɻ�
	SMALL//С�ɻ�
};

//�ɻ����ӵ��ṹ��
struct Plane
{
	int x;
	int y;
	int frame;//��ǰ֡
	bool live;//�ж��Ƿ���
	int width;//���
	int height;//�߶�
	int HP;//Ѫ��
	int type;//�ɻ�����
	bool destroy;
}plane, bull[BULLELT_NUM], enemy[ENEMY_NUM], enemybull[BULLELT_NUM];
//����
int score = 0;//��ǰ�÷�
int max_score=0;//��߷�
//ת��Ϊ�ַ���
char planeScore[10];
char planeHP[3];
char maxScore[10];

//���ص�ͼ
IMAGE img_Map;
IMAGE img_gameover[2];
//�������ͼƬ
IMAGE img_Movre[2][2];
//�����ӵ�
IMAGE img_bullelt[2];
//����л�
IMAGE img_enemy[4][4];

void drawImg(int x, int y, IMAGE* src)
{
	// ������ʼ��
	DWORD* pwin = GetImageBuffer();			//���ڻ�����ָ��
	DWORD* psrc = GetImageBuffer(src);		//ͼƬ������ָ��
	int win_w = getwidth();				//���ڿ��
	int win_h = getheight();
	int src_w = src->getwidth();				//ͼƬ���
	int src_h = src->getheight();

	// ������ͼ��ʵ�ʳ���
	int real_w = (x + src_w > win_w) ? win_w - x : src_w;			// �������ұ߽�
	int real_h = (y + src_h > win_h) ? win_h - y : src_h;			// �������±߽�
	if (x < 0) { psrc += -x;			real_w -= -x;	x = 0; }	// ��������߽�
	if (y < 0) { psrc += (src_w * -y);	real_h -= -y;	y = 0; }	// �������ϱ߽�


	// ������ͼ��ʼλ��
	pwin += (win_w * y + x);

	// ʵ��͸����ͼ
	for (int iy = 0; iy < real_h; iy++)
	{
		for (int ix = 0; ix < real_w; ix++)
		{
			byte a = (byte)(psrc[ix] >> 24);//����͸��ͨ����ֵ[0,256) 0Ϊ��ȫ͸�� 255Ϊ��ȫ��͸��
			if (a > 100)
			{
				pwin[ix] = psrc[ix];
			}
		}
		//������һ��
		pwin += win_w;
		psrc += src_w;
	}
}

//��ʼ���л�Ѫ��
void EnemyHP(int i)
{
	int flag = rand() % 10;
	if (flag <= 2 && flag >= 0)
	{
		//��л�
		enemy[i].type = BIG;
		enemy[i].HP = 4;
		enemy[i].width = 84;
		enemy[i].height = 128;
	}
	else
	{
		//С�л�
		enemy[i].type = SMALL;
		enemy[i].HP = 2;
		enemy[i].width = 57;
		enemy[i].height = 43;
	}
}

//��ʱ������
bool Timer(int id, DWORD ms)
{
	static DWORD t[100];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}
//����ͼƬ
void LoadImage()
{
	//���ص�ͼͼƬ
	loadimage(&img_Map, "background.png");

	loadimage(&img_gameover[0], "gameover.png");
	loadimage(&img_gameover[1], "gameover.png");

	//�������ͼƬ
	loadimage(&img_Movre[0][0], "me1.png");
	loadimage(&img_Movre[0][1], "me2.png");
	//���ر�����ͼƬ
	loadimage(&img_Movre[1][0], "me_destroy_1.png");
	loadimage(&img_Movre[1][1], "me_destroy_2.png");


	//�����ӵ�ͼƬ
	loadimage(&img_bullelt[0], "bullet2.png");
	loadimage(&img_bullelt[1], "bullet1.png");

	//���صл�ͼƬ
	//С�л�
	loadimage(&img_enemy[0][0], "enemy1.png");
	loadimage(&img_enemy[0][1], "enemy2.png");
	//��л�
	loadimage(&img_enemy[1][0], "enemy3_n2.png");
	loadimage(&img_enemy[1][1], "enemy3_n1.png");

	//С�л�������
	loadimage(&img_enemy[2][0], "enemy1_down1.png");
	loadimage(&img_enemy[2][1], "enemy1_down2.png");

	//��л�������
	loadimage(&img_enemy[3][0], "enemy3_down3.png");
	loadimage(&img_enemy[3][1], "enemy3_down4.png");
}

//��ʼ����Ϸ����
void GameInit()
{
	//����ͼƬ
	LoadImage();
	//��ҿ�������
	plane.x = WIDTH / 2 - 40;
	plane.y = HEIGTH - 120;
	plane.HP = 5;//���Ѫ��
	plane.width = 60;
	plane.height = 74;
	//����״̬
	plane.live = true;//���

	for (int i = 0; i < BULLELT_NUM; i++)
	{
		//��ʼ������ӵ�
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}

	for (int i = 0; i < BULLELT_NUM; i++)
	{
		//��ʼ���л��ӵ�
		enemybull[i].x = 0;
		enemybull[i].y = 0;
		enemybull[i].live = false;
	}

	//��ʼ���л�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].live = false;
		enemy[i].destroy = false;
		EnemyHP(i);
	}

}

//����ͼƬ
void GameDraw()
{
	//������ͼ
	putimage(0, 0, &img_Map);
	//�������
	if (plane.live)
	{
		//���
		drawImg(plane.x, plane.y, &img_Movre[0][plane.frame]);
		plane.frame = (plane.frame + 1) % 2;
	}
	else
	{
		//������
		drawImg(plane.x, plane.y, &img_Movre[1][1]);
	}
	//�����ӵ�
	for (int i = 0; i < BULLELT_NUM; i++)
	{
		if (bull[i].live)
		{
			drawImg(bull[i].x, bull[i].y, &img_bullelt[0]);
		}
	}

	//���Ƶл��ӵ�
	for (int i = 0; i < BULLELT_NUM; i++)
	{
		if (enemybull[i].live)
		{
			drawImg(enemybull[i].x, enemybull[i].y, &img_bullelt[0]);
		}
	}

	//���Ƶл�
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == BIG)
			{
				//���ƴ�ɻ�
				drawImg(enemy[i].x, enemy[i].y, &img_enemy[1][0]);
			}
			else
			{
				//����С�ɻ�
				drawImg(enemy[i].x, enemy[i].y, &img_enemy[0][0]);
			}
		}
		//������
		if (enemy[i].destroy)
		{
			if (enemy[i].type == SMALL)
			{
				//С�л�
				drawImg(enemy[i].x, enemy[i].y, &img_enemy[2][0]);
			}
			else
			{
				//��л�
				drawImg(enemy[i].x, enemy[i].y, &img_enemy[3][0]);
			}
		}
	}

	//�������
	if (!plane.live)
	{
		drawImg(getwidth()/5, getheight()/2, &img_gameover[0]);
	}
}

//����ӵ�����λ��
void CreateBullelt()
{
	for (int i = 0; i < BULLELT_NUM; i++)
	{
		if (!bull[i].live)
		{
			bull[i].x = plane.x + 27;
			bull[i].y = plane.y;
			bull[i].live = true;
			break;
		}
	}
}

//����ӵ��������ƶ�
void Bullelt_Move(int speed)
{
	//�����ӵ�
	//��Ҵ��ڿɴ���
	if (plane.live)
	{
		if (GetAsyncKeyState(VK_SPACE) && Timer(3, 100))//�ո�
		{
			CreateBullelt();
		}
		for (int i = 0; i < BULLELT_NUM; i++)
		{
			if (bull[i].live)
			{
				bull[i].y -= speed;
				if (bull[i].y < 0)
				{
					bull[i].live = false;
				}
			}
		}
	}
}

//�л��ӵ�����λ��
void Create_Bullelt_ENEMY()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].live)
		{
			if (!enemybull[i].live)
			{
				enemybull[i].live = true;
				//С�ɻ�
				if (enemy[i].type == SMALL)
				{
					enemybull[i].x = enemy[i].x + 25;
					enemybull[i].y = enemy[i].y + 43;
				}
				//��ɻ�����λ��
				else
				{
					enemybull[i].x = enemy[i].x + 42;
					enemybull[i].y = enemy[i].y + 120;
				}
			}
		}
	}
}

//�л��ӵ��ƶ�
void Bullelt_Enemy(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemybull[i].live)
		{
			enemybull[i].y += speed;
			//�ӵ��ɳ���ͼ
			if (enemybull[i].y > HEIGTH)
			{
				enemybull[i].live = false;
			}
		}
	}
}

//��ȡ���̿�������ƶ�
void PlayeMove(int speed)
{
	//ʹ��windows������ȡ�������룬ʮ������
	if (plane.live)
	{
		if (GetAsyncKeyState('W'))//��
		{
			//����߽�
			if (plane.y > 0)
				plane.y -= speed;
		}
		if (GetAsyncKeyState('S'))//��
		{
			//��ȥ�ɻ��ĳ���
			if (plane.y < HEIGTH - 100)
				plane.y += speed;
		}
		if (GetAsyncKeyState('A'))//��
		{
			if (plane.x + 50 > 0)
				plane.x -= speed;
		}
		if (GetAsyncKeyState('D'))//��
		{
			if (plane.x < WIDTH - 30)
				plane.x += speed;
		}
	}
}

//�л��ƶ�
void Enemy_Mocve(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//�л����
		if (enemy[i].live)
		{
			//����ͼ����
			enemy[i].y += speed;

			if (enemy[i].y > HEIGTH)
			{
				enemy[i].live = false;
			}
		}
	}
}

//���ɵл�
void CreateEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			enemy[i].destroy = false;
			//����������
			enemy[i].x = rand() % (WIDTH - (enemy[i].width / 2));
			enemy[i].y = -40;
			EnemyHP(i);
			break;
		}
	}
}

//�������ɻ�
void playPlance()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		//�л�������ʲôҲ����
		if (!enemy[i].live)
			continue;
		for (int j = 0; j < BULLELT_NUM; j++)
		{
			//�ӵ�������ʲôҲ����
			if (!bull[j].live)
				continue;
			//�ӵ�����
			if ((bull[j].x > enemy[i].x) && (bull[j].x < enemy[i].x + enemy[i].width)
				&& (bull[j].y > enemy[i].y) && (bull[j].y < enemy[i].y + enemy[i].height))
			{
				//�ӵ���ʧ
				bull[j].live = false;
				//��Ѫ
				enemy[i].HP--;
			}
		}
		if (enemy[i].HP <= 0)
		{
			enemy[i].live = false;
			enemy[i].destroy = true;
			//�Ʒ�
			if (enemy[i].type == BIG)
				score += 5;
			else
				score += 2;
		}
	}
}
void change(int score, char str[]);
int record();
//�л��ӵ��������
void playEnmey()
{
	//��Ҵ���
	if (plane.live)
	{
		for (int i = 0; i < BULLELT_NUM; i++)
		{
			//�ӵ�����
			if (enemybull[i].live)
			{
				//�ӵ��������
				if ((enemybull[i].x > plane.x) && (enemybull[i].x < plane.x + plane.width)
					&& (enemybull[i].y > plane.y) && (enemybull[i].y < plane.y + plane.height))
				{
					enemybull[i].live = false;
					plane.HP--;
				}
			}
		}
		//��ұ�����
		if (plane.HP <= 0)
		{
			plane.live = false;
		}
		//////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		GameDraw();
	}
}

//�л������ײ
void collision()
{
	//��Ҵ��
	if (plane.live)
	{
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			//�л�Ҳ���
			if (enemy[i].live)
			{
				//�л������ײ
				if ((enemy[i].x > plane.x) && (enemy[i].x < plane.x + plane.width)
					&& (enemy[i].y > plane.y) && (enemy[i].y < plane.y + plane.height)
					|| (plane.x > enemy[i].x + 10) && (plane.x < enemy[i].x + enemy[i].width - 10)
					&& (plane.y > enemy[i].y) && (plane.y < enemy[i].y + enemy[i].height))
				{
					//ײС�ɻ�
					if (enemy[i].type == SMALL)
					{
						enemy[i].destroy = true;
						enemy[i].live = false;
						plane.HP -= 3;
					}
					else
					{
						//ײ��ɻ�
						enemy[i].destroy = true;
						enemy[i].live = false;
						plane.HP -= 10;
					}
					if (plane.HP <= 0)
					{
						plane.live = false;
						////////////////////////////////////////////////
					}
				}
			}
		}
	}
}

//����ת��Ϊ�ַ���
void change(int score,char str[]) {
	int cnt = 0;
	int x = score;
	while (x)
	{
		str[cnt++] = x % 10 + '0';
		x = x / 10;
	}
	for (int i = 0; i < cnt/2; i++)
	{
		char op = str[i];
		str[i] = str[cnt - 1 - i];
		str[cnt - 1 - i] = op;
	}
	str[cnt] = '\0';
}

//�ļ�����-��¼��߷�
int record()
{
	FILE* f;
	//��û���ļ����ȴ���һ������Ե�һ�Σ�
	if ((f = fopen("D:\\�ɻ���սscore.txt", "r")) == NULL)
	{
		//����
		f = fopen("D:\\�ɻ���սscore.txt", "w");
		//д�����
		fwrite(&score, sizeof(int), 1, f);
		fclose(f);
		return score;
	}
	else
	{
		fclose(f);
		//����֮��ķ���
		f = fopen("D:\\�ɻ���սscore.txt", "r");
		int sc;
		fread(&sc, sizeof(int), 1, f);//��ȡ��߷�
		fclose(f);
		//����߷ֱȽ�
		if (score > sc)
		{
			sc = score;
			f = fopen("D:\\�ɻ���սscore.txt", "w");
			fwrite(&score, sizeof(int), 1, f);//д����߷�
			fclose(f);
		}
		fclose(f);
		return sc;
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	//�������ڴ�С
	initgraph(WIDTH, HEIGTH);
	GameInit();
	GameDraw();

	//˫�����ͼ
	BeginBatchDraw();
	max_score = record();
	while (true)
		{
			GameDraw();
			//������ת��Ϊ�ַ���
			outtextxy(0, 0, "�÷֣�");
			change(score, planeScore);
			outtextxy(40, 0, planeScore);
			outtextxy(0, 30, "HP��");
			change(plane.HP, planeHP);
			outtextxy(35, 30, planeHP);
			outtextxy(0, 60, "��߷֣�");
			record();
			change(max_score, maxScore);
			outtextxy(50, 60, maxScore);

			//ˢ��
			FlushBatchDraw();
			//����ƶ�
			if (Timer(0, 2))
				PlayeMove(1);
			//�ӵ��ƶ�
			Bullelt_Move(1);
			//�л�����
			if (Timer(1, 1500))
				CreateEnemy();

			//�л��ƶ�
			if (Timer(2, 50))
				Enemy_Mocve(1);
			//���˷����ӵ�
			if (Timer(4, 500))
				Create_Bullelt_ENEMY();
			if (Timer(5, 50))
				Bullelt_Enemy(12);

			//�ӵ��Ƿ����зɻ�
			playPlance();
			playEnmey();
			//�ɻ���ײ
			collision();
		}
	EndBatchDraw();
	return 0;
}