#include <allegro.h>
#include <winalleg.h>
#include <time.h>
#include <stdio.h>
#include <fstream>

#define W 1024
#define H 750
#define TOWER_DAMAGE 50
#define ENTER 0x0D
//using namespace std;

typedef enum
{
	FIN = 0,
	PAUSE,
	INICIO
}enumEstado;

typedef struct
{
public:
	BITMAP* Tower_sprite;
	BITMAP* Gun_sprite;
	BITMAP* Boom;
	fixed angle;
	int hp;
	int Pos_X;
	int Pos_Y;
	float Life_gainer;

} TOWER;

typedef struct
{
public:
	BITMAP* Shot_sprite;
	BITMAP* Explotion_sprite;
	int VelocitY;
	int VelocitX;
	int Pos_X;
	int Pos_Y;
	fixed angle;
	bool shoot;
	SAMPLE* Shot;
	SAMPLE* Explotion;

} TSHOT;

typedef struct
{
public:
	int Pos_x;
	int Pos_y;
	int Hp;
	int Damage;
	int Range;
	int Current_frame;
	int Explotion_frame;
	int Death_Pos_X;
	time_t Tiempo_ataque;

} TENEMIES;

typedef struct
{
public:
	BITMAP* buffer;
	BITMAP* Grass;
	BITMAP* Logo;
	SAMPLE* Shot;
	SAMPLE* Complain;
	SAMPLE* Background_music;
	time_t Tiempo_nuevo_enemigo;
	enumEstado Estado;
	int Gravity;
	int Cantidad_enemigos;
	int colision;
	int wave;
	int Multiplier;
	int alive_enemies;

} TJUEGO;

typedef struct  
{
public:
	BITMAP* Sprite[4];
	SAMPLE* Shot[4];
	int Range[4];

} TARMAS;

typedef struct
{
public:
	BITMAP* Sprite;
	int Pos_x;
	int Pos_y;

} TCLOUDS;

typedef struct
{
public:
	char Nombre[50];
	int Kills;
	int Waves;
	int Score;
	int Max_Kills;
	int Max_Waves;
	int Max_Score;
	int Max_Multiplier;
}USUARIO;

extern TJUEGO Juego;
extern TOWER Torre;
extern TENEMIES Enemigos[10];
extern TCLOUDS Clouds;
extern TSHOT Proyectil;
extern TARMAS Weapons;
extern USUARIO* User;
extern USUARIO* data;

void GameInit();
void GameDeinit();
void GameRender();
void GameUpdate();
void Shoot();
void Check_colision();
void Delete_shot();
void Enemy_Update();
void Map_Render();
void Update_HP();
void Enemies_init();
void Menu();
void HUD();
void highscore();
void crear_perfil();
void guardar_en_archivo();
void cargar_perfil();
int Get_String(int);
void Button_UP(int x, int y, int x2, int y2, BITMAP* );
void Button_DOWN(int x, int y, int x2, int y2, BITMAP*);
void view_highscores();