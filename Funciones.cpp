#include "Game.h"

void GameInit()
{
	allegro_init();
	install_keyboard();
	install_mouse();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT, W, H,0,0);// W = 1024, H = 768
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
	set_window_title("Tower Of Defense........A Durakuzz Production.");

	Juego.Estado = INICIO;
	Juego.buffer = create_bitmap(W,H);
	clear_to_color(Juego.buffer,makecol(0,255,255));
	Juego.Background_music = load_wav("Music & Effects/Background_Music.wav");
	Juego.Grass = load_bitmap("Sprites/Grass.bmp",NULL);
	Juego.Gravity = 2;
	Juego.colision = 0;
	Juego.Cantidad_enemigos = 1;
	Juego.Complain = load_wav("Music & Effects/Complain.wav");
	Juego.Logo = load_bitmap("Sprites/Defense_Tower_Logo.bmp",NULL);
	Juego.wave = 0;
	Juego.Multiplier = 0;
	
	char s[15] = { };
	char w[30] = { };
	
	for (int i = 0; i < 4;i++)
	{
		sprintf(s,"Sprites/%d.bmp",i);
		sprintf(w,"Music & Effects/%d.wav",i);
		Weapons.Sprite[i] = load_bitmap(s,NULL);
		Weapons.Shot[i] = load_wav(w);
	}

	Clouds.Sprite = load_bitmap("Sprites/Clouds.bmp",NULL);
	Clouds.Pos_x = 0;
	Clouds.Pos_y = 200;

	Torre.Tower_sprite = load_bitmap("Sprites/Towers.bmp", NULL);
	Torre.Gun_sprite = load_bitmap("Sprites/Gun.bmp",NULL);
	Torre.Pos_X = 937;
	Torre.Pos_Y = 410;
	Torre.hp = 10000;
	Torre.Life_gainer = 100;
	Torre.Boom = load_bitmap("Sprites/boom.bmp",NULL);

	Proyectil.Pos_X = 972;
	Proyectil.Pos_Y = 463;
	Proyectil.VelocitY = 15;
	Proyectil.VelocitX = 31;
	Proyectil.shoot = false;
	Proyectil.Shot = load_wav("Music & Effects/Tower_Shot.wav");
	Proyectil.Explotion = load_wav("Music & Effects/Explotion.wav");
	Proyectil.Shot_sprite = load_bitmap("Sprites/Shot_Sprite.bmp",NULL);
	Proyectil.Explotion_sprite = load_bitmap("Sprites/Explotion.bmp",NULL);

	Menu();

	Enemies_init();
	play_sample(Juego.Background_music,200,124,1000,0);

	User->Kills = 0;
	User->Max_Multiplier = 0;
	User->Score = 0;
	User->Waves = 1;

	//guardar_en_archivo();
}

void Menu()
{
	clear_to_color(Juego.buffer,makecol(0,0,0));
	bool clicked = false;

	while(clicked != true)
	{
		draw_sprite(Juego.buffer, Juego.Logo,0,0);
		Button_UP(300,475, 450, 525, Juego.buffer);//150x,50y es la diferencia
		textprintf_ex(Juego.buffer, font,320,500,makecol(255,0,0),-1,"Load Profile");

		Button_UP(600,475, 750, 525, Juego.buffer);
		textprintf_ex(Juego.buffer, font,620,500,makecol(255,0,0),-1,"Create Profile");

		Button_UP(450,575, 600, 625, Juego.buffer);
		textprintf_ex(Juego.buffer, font,465,600,makecol(255,0,0),-1,"EXIT GAME");

		if(mouse_b & 1 && (mouse_x >=300 && mouse_x <= 450) && (mouse_y >= 475 && mouse_y <=525) )// Cargar Perfil
		{
			Button_DOWN(300,475, 450, 525, Juego.buffer);
			blit(Juego.buffer, screen,0,0,0,0,W,H);
			rest(60);
			clicked = true;
			cargar_perfil();
		}

		if(mouse_b & 1 && (mouse_x >=600 && mouse_x <= 750) && (mouse_y >= 475 && mouse_y <=525) )// Crear Perfil
		{
			Button_DOWN(600,475, 750, 525, Juego.buffer);
			blit(Juego.buffer, screen,0,0,0,0,W,H);
			rest(60);
			clicked = true;
			crear_perfil();
		}

		if(mouse_b & 1 && (mouse_x >=450 && mouse_x <= 600) && (mouse_y >= 575 && mouse_y <=625) )// View Highscores
		{
			Button_DOWN(450,575, 600, 625, Juego.buffer);
			blit(Juego.buffer, screen,0,0,0,0,W,H);
			rest(60);
			clicked = true;
			//view_highscores();
			GameDeinit();
		}
		show_mouse(Juego.buffer);
		blit(Juego.buffer, screen,0,0,0,0,W,H);
		rest(60);
		clear_bitmap(Juego.buffer);

		if(mouse_b & 2)
			allegro_exit();
	}

	//readkey();
}

/*Esta funcion se encarga de simular un Boton sin presionar.
*/
void Button_UP(int x, int y, int x2, int y2, BITMAP * buffer)
{
	rectfill(buffer, x-2, y-2, x2+2, y2 +2, makecol(0,0,0));
	rectfill(buffer, x, y, x2, y2, makecol(128,128,128));
	rectfill(buffer, x-1, y, x, y2, makecol(255,255,255));
	rectfill(buffer, x-1, y-1, x2, y, makecol(255,255,255));
}

/*Esta funcion se encarga de simular un Boton presionado
*/
void Button_DOWN(int x, int y, int x2, int y2, BITMAP * buffer)
{
	rectfill(buffer, x - 1, y, x, y2, makecol(0,0,0));
	rectfill(buffer, x - 1, y - 1, x2, y, makecol(0,0,0));
	rectfill(buffer, x - 1, y2, x2, y2 + 1, makecol(255,255,255));
	rectfill(buffer, x2, y, x2 + 1, y2, makecol(255,255,255));
}

void GameDeinit()
{
	destroy_sample(Proyectil.Shot);
	destroy_sample(Proyectil.Explotion);
	destroy_sample(Juego.Complain);
	destroy_sample(Juego.Background_music);

	for (int i = 0; i < 4;i++)
		destroy_bitmap(Weapons.Sprite[i]);

	destroy_bitmap(Proyectil.Explotion_sprite);
	destroy_bitmap(Proyectil.Shot_sprite);
	destroy_bitmap(Torre.Boom);
	destroy_bitmap(Torre.Gun_sprite);
	destroy_bitmap(Torre.Tower_sprite);
	destroy_bitmap(Clouds.Sprite);
	destroy_bitmap(Juego.Grass);
	destroy_bitmap(Juego.buffer);
	remove_keyboard();
	remove_mouse();
	remove_sound();
	allegro_exit();
}

void GameRender()
{

	acquire_screen();

	draw_sprite(screen, Juego.buffer, 0, 0);

	release_screen();
}

void GameUpdate()
{
	clear_keybuf();
	if(Torre.hp <= 0)
	{
		Juego.Estado = FIN;
		guardar_en_archivo();
		highscore();
		return;
	}
	if(Juego.Estado != PAUSE)
	{
		show_mouse(Juego.buffer);

		Torre.angle = 64 - (((double)(mouse_y) / 768) * 64);

		if(key[KEY_P])
		{
			//Juego.Estado = FIN;
			//GameDeinit();
			guardar_en_archivo();
			highscore();
			GameDeinit();
		}

		if(key[KEY_ENTER] || (mouse_b & 1))
			if(Proyectil.shoot == false)
				Shoot();

		Juego.alive_enemies = 0;
		clear_to_color(Juego.buffer,makecol(0,185,255));
		Map_Render();
		Enemy_Update();

		if(Proyectil.shoot == true)
		{
			Shoot();
			Check_colision();
		}
	
		if(clock()/CLOCKS_PER_SEC >= Juego.Tiempo_nuevo_enemigo)
		{
			if(Juego.Cantidad_enemigos < 10)
				Juego.Cantidad_enemigos++;

			Juego.Tiempo_nuevo_enemigo = ((clock()/CLOCKS_PER_SEC) + (rand() % 3 + 1));
		}

		if(Juego.alive_enemies == 0 && Juego.Cantidad_enemigos ==10)
		{
			if(User->Waves < 4)		// Si son las primeras 4 rondas va en orden.
				Juego.wave++;
			
			else					// Si pasaron las primeras 4 rondas las demas son aleatorias.
				Juego.wave = rand() % 4;

			User->Waves++;
			if(User->Waves > User->Max_Waves)//Si ha llegado a una wave mayor que la anterior se guarda la nueva wave mayor.
				User->Max_Waves = User->Waves;
			
			Enemies_init();
			Juego.Cantidad_enemigos = 1;
		
			Torre.hp = Torre.hp + (Torre.hp * (Torre.Life_gainer/100));
		
			if(Torre.Life_gainer > 20)
				Torre.Life_gainer -= 20;
			else
				Torre.Life_gainer = 10;

			if(Torre.hp > 10000)
				Torre.hp = 10000;

			textprintf_ex(Juego.buffer,font,500,350,makecol(255,255,0),-1,"Wave # %d", User->Waves);
			Juego.Estado = PAUSE;
		}
	}
	HUD();
	if(key[KEY_ESC] && Juego.Estado == PAUSE)
	{
		Juego.Estado = INICIO;
	}
}

void Shoot()
{
	if(Proyectil.shoot == false)
	{
		if(Torre.angle > 32)
			Proyectil.VelocitY = -42;
		else
			Proyectil.VelocitY = Torre.angle * -1;

		if(Torre.angle > 32 && Torre.angle <= 64)
			Proyectil.VelocitX = (32-(Torre.angle - 32));
		else
			Proyectil.VelocitX = 32;


		play_sample(Proyectil.Shot,254,124,1000,0);
		Proyectil.shoot = true;
		rotate_sprite(Juego.buffer, Proyectil.Shot_sprite, Proyectil.Pos_X, Proyectil.Pos_Y, itofix(Proyectil.angle));
	}

	Proyectil.Pos_X -= Proyectil.VelocitX;
	Proyectil.VelocitY += Juego.Gravity;
	Proyectil.Pos_Y += Proyectil.VelocitY;
	Proyectil.angle = -Proyectil.VelocitY;

	rotate_sprite(Juego.buffer, Proyectil.Shot_sprite, Proyectil.Pos_X, Proyectil.Pos_Y, itofix(Proyectil.angle));
}

void Check_colision()
{
	Juego.colision= 0;
	for(int i = 0; i < Juego.Cantidad_enemigos; i++)
	{
		if((Proyectil.Pos_X + 20 >= Enemigos[i].Pos_x && Proyectil.Pos_X <= Enemigos[i].Pos_x + 43) && Proyectil.Pos_Y >= Enemigos[i].Pos_y) // Si se colisiono con un enemigo.
		{
			Juego.colision = 1;

			Juego.Multiplier++;
			if(Juego.Multiplier > User->Max_Multiplier)
				User->Max_Multiplier = Juego.Multiplier;

			if(Enemigos[i].Hp > 0)
				Enemigos[i].Hp -= TOWER_DAMAGE;

			play_sample(Proyectil.Explotion, 195, 125, 1000, 0);
			masked_blit(Proyectil.Explotion_sprite, Juego.buffer, 0, 0, Enemigos[i].Pos_x, Enemigos[i].Pos_y, 200, 150);
		}
		
		else if(Proyectil.Pos_Y >= 725)
		{
			Juego.Multiplier = 0;
			User->Score -= 20;
			Delete_shot();
		}
	}
	if(Juego.colision == 1)
		Delete_shot();
}

void Delete_shot()
{
	Proyectil.shoot = false;
	play_sample(Proyectil.Explotion, 195, 125, 1000, 0);
	
	if(Juego.colision == 0)
		masked_blit(Proyectil.Explotion_sprite, Juego.buffer, 0, 0, Proyectil.Pos_X - 50, Proyectil.Pos_Y - 80, 200, 150);

	Proyectil.Pos_X = 972;
	Proyectil.Pos_Y = 463;
	Proyectil.VelocitY = 0;
}

void Enemy_Update()
{
	for(int i = 0; i < Juego.Cantidad_enemigos; i++)
	{
		if(Enemigos[i].Hp > 0)
		{
			Juego.alive_enemies++;

			if (Enemigos[i].Pos_x < Torre.Pos_X - Enemigos[i].Range) // Enemy walking.
			{
				Enemigos[i].Pos_x += 2;

				Enemigos[i].Current_frame++;

				if(Enemigos[i].Current_frame > 8)
					Enemigos[i].Current_frame = 0;
				
				if(Enemigos[i].Pos_x >= (Torre.Pos_X - Enemigos[i].Range) - 2)
					Enemigos[i].Tiempo_ataque = clock()/CLOCKS_PER_SEC + 1;
			}

			else // Enemy Atacking.
			{
				if(clock()/CLOCKS_PER_SEC >= Enemigos[i].Tiempo_ataque) 
				{
					Enemigos[i].Tiempo_ataque = clock() / CLOCKS_PER_SEC + 1;
					play_sample(Weapons.Shot[Juego.wave], 255,124,1000,0);
				}

				if(Enemigos[i].Current_frame < 9)
					Enemigos[i].Current_frame = 9;

				else
					Enemigos[i].Current_frame++;
				
				if(Enemigos[i].Current_frame > 10)
					Enemigos[i].Current_frame = 0;

				masked_blit(Torre.Boom, Juego.buffer, 0, 0,((rand() % 1000) + 945),((rand() % 700) + 470), 32, 26);

				if(Torre.hp > 0)
					Torre.hp -= Enemigos[i].Damage;
				/*else if(Torre.hp < 0 || Torre.hp == 0)
					Juego.Estado = FIN;*/
			
			}

			masked_blit(Weapons.Sprite[Juego.wave], Juego.buffer, 0, Enemigos[i].Current_frame * 55, Enemigos[i].Pos_x, Enemigos[i].Pos_y, 200, 55);
			Update_HP();
		}
	
		else
		{
			if(Enemigos[i].Hp > -1)
			{
				User->Kills++;
				if(User->Kills > User->Max_Kills)//si ya ha matado mas que su record anterior se copia la nueva cantidad maxima.
					User->Max_Kills = User->Kills;

				User->Score += (Juego.Multiplier * 10);
				if(User->Score > User->Max_Score)//Si ya ha alcanzado un puntaje mas alto a su record anterior se copia el nuevo puntaje maximo.
					User->Max_Score = User->Score;

				Enemigos[i].Hp = -1;
			}

			if(Enemigos[i].Explotion_frame < 3)
			{
				if(Enemigos[i].Explotion_frame == 0)
				{
					play_sample(Juego.Complain, 195, 125, 1000,0);
					play_sample(Proyectil.Explotion, 195, 125, 1000, 0);
				}
			
				masked_blit(Proyectil.Explotion_sprite, Juego.buffer, 0, Enemigos[i].Explotion_frame * 150, Enemigos[i].Pos_x - 70, Enemigos[i].Pos_y - 50, 200, 150);
				Enemigos[i].Explotion_frame++;
			}
			else
			{
				if(Enemigos[i].Pos_x < W)
					Enemigos[i].Death_Pos_X = Enemigos[i].Pos_x;

				masked_blit(Weapons.Sprite[Juego.wave], Juego.buffer, 0, 11 * 55, Enemigos[i].Death_Pos_X, Enemigos[i].Pos_y + 15,200, 55);
				Enemigos[i].Pos_x = W;
			}
		}
	}
}

void Map_Render()
{
	for(int i = 0;i < 4;i++)
	{
		Clouds.Pos_x --;

		if(Clouds.Pos_x < -520)
			Clouds.Pos_x = 0;
		draw_sprite(Juego.buffer, Clouds.Sprite, Clouds.Pos_x + i * 260 ,Clouds.Pos_y);
		draw_sprite(Juego.buffer, Clouds.Sprite, Clouds.Pos_x + (i + 4) * 260 ,Clouds.Pos_y);
	}

    Update_HP();

	for(int i = 0;i < 924;i += 171)
		draw_sprite(Juego.buffer, Juego.Grass, i, 730);

	if(Torre.hp > 0)
		rotate_sprite(Juego.buffer, Torre.Gun_sprite, 970, 460, itofix(Torre.angle));

	masked_blit(Torre.Tower_sprite, Juego.buffer, (4 - (fixceil(ftofix((double)Torre.hp / 2500)))) * 127, 0, Torre.Pos_X, Torre.Pos_Y, 127, 500);
}

void Update_HP()
{
	int up = 10;

	//Se imprime el rectangulo de la HP de la torre.// bajara de 10 en 10 (100 total hp de torre visible)
	rect(Juego.buffer,850,10, 952, 20,makecol(0,0,0));

	if(Torre.hp >= 7500)//Green
		rectfill(Juego.buffer, 851, 11, 851 + Torre.hp/100, 19,makecol(0,255,0) );
	else if(Torre.hp >= 5000 && Torre.hp < 7500)//Yellow
		rectfill(Juego.buffer, 851, 11, 851 + Torre.hp/100, 19,makecol(255,255,0));
	else if(Torre.hp >= 2500 && Torre.hp < 5000)//Orange
		rectfill(Juego.buffer, 851, 11, 851 + Torre.hp/100, 19,makecol(255,165,0));
	else//Red
		rectfill(Juego.buffer, 851, 11, 851 + Torre.hp/100, 19,makecol(255,0,0));

	//Aki se imprimira la HP del enemigo
	for(int i = 0; i < Juego.Cantidad_enemigos;i++)
	{
		if(Enemigos[i].Hp > 0 && i == 0)
		{
			rect(Juego.buffer,Enemigos[i].Pos_x, Enemigos[i].Pos_y - 15, Enemigos[i].Pos_x + 26,  Enemigos[i].Pos_y - 10, makecol(0,0,0));//15-10
			if(Enemigos[i].Hp == 100)//Green
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11,makecol(0,255,0));//14-11
			else if(Enemigos[i].Hp <= 50)//Red
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11,makecol(255,0,0));
		}

		else if(Enemigos[i].Hp > 0 && Enemigos[i].Pos_x + 43 < Enemigos[i-1].Pos_x)// cuando hp es mayor que cero y no esta colisionando con ningun otro enemigo
		{
			rect(Juego.buffer,Enemigos[i].Pos_x, Enemigos[i].Pos_y - 15, Enemigos[i].Pos_x + 26,  Enemigos[i].Pos_y - 10, makecol(0,0,0));//15-10
			if(Enemigos[i].Hp == 100)//Green
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11,makecol(0,255,0));//14-11
			else if(Enemigos[i].Hp <= 50)//Red
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11,makecol(255,0,0));
		}
		//cuando su hp es mayor que cero y se esta colisionando con el enemigo anterior
		else if(Enemigos[i].Hp > 0 && (Enemigos[i].Pos_x + 43 >= Enemigos[i-1].Pos_x) && Enemigos[i].Pos_x <= Enemigos[i-1].Pos_x + 43)
		{
			rect(Juego.buffer,Enemigos[i].Pos_x, Enemigos[i].Pos_y - 15 - up, Enemigos[i].Pos_x + 26,  Enemigos[i].Pos_y - 10 - up, makecol(0,0,0));//15-10
			if(Enemigos[i].Hp == 100)//Green
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14 - up, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11 - up,makecol(0,255,0));//14-11
			else if(Enemigos[i].Hp <= 50)//Red
				rectfill(Juego.buffer,Enemigos[i].Pos_x + 1, Enemigos[i].Pos_y - 14 - up, Enemigos[i].Pos_x + Enemigos[i].Hp/4, Enemigos[i].Pos_y - 11 - up ,makecol(255,0,0));
		}

		up += 10;
	}
}	

void Enemies_init()
{
	Juego.Tiempo_nuevo_enemigo =  ((clock()/CLOCKS_PER_SEC) + (rand() % 3 + 1));
	
	for(int i = 0;i < 10; i++)
	{
		Enemigos[i].Hp = 100;
		Enemigos[i].Pos_y = 678;
		Enemigos[i].Pos_x = 0;
		Enemigos[i].Current_frame = 0;
		Enemigos[i].Range = ((Juego.wave + 1) * 200) + (rand() % 100 + 20);
		Enemigos[i].Damage = Juego.wave + 1;
		Enemigos[i].Explotion_frame = 0;	
	}
}

void HUD()
{
	textprintf_ex(Juego.buffer,font,0,0,makecol(255,255,0),-1,"Angulo de la torreta %d", Torre.angle);

	textprintf_ex(Juego.buffer,font,0,15,User->Score > 0? makecol(0,255,0): makecol(255,0,0),-1,"Score %d", User->Score);

	textprintf_ex(Juego.buffer,font,0,30,makecol(255,255,0),-1,"Kills: %d", User->Kills);

	textprintf_ex(Juego.buffer,font,0,45,makecol(255,255,0),-1,"Wave: %d", User->Waves);

	textprintf_ex(Juego.buffer,font,0,60,makecol(255,255,0),-1,"Max KillStreak: %d", User->Max_Multiplier);

	textprintf_ex(Juego.buffer,font,0,75,makecol(255,255,0),-1,"KillStreak: %d", Juego.Multiplier);

	textprintf_ex(Juego.buffer,font,0,90,makecol(255,255,0),-1,"%s", User->Nombre);
}

void view_highscores()
{
	char c;
	int x = 50, y = 200;
	std::ifstream openfile;
	openfile.open("Highscore.txt");
	if(openfile.is_open())
	{
		while(!openfile.eof())
		{
			c = openfile.get();
			if(!openfile.eof())
			{
				if(c = '\n')
				{
					x = 50;
					y+=10;
				}
				textout_ex(screen, font,(const char*) (c), x,y,makecol(255,255,255),-1);
				x++;
			}
		}
	}
	openfile.close();
}

int Get_String(int tipo)
{
	int  i = 0, press = 0, k;
	char lt;
	clear_keybuf();
	if(tipo == 1)// Si se va a querer digitar un nombre.
	{
		draw_sprite(Juego.buffer,Juego.Logo,0,0);
		textprintf_ex(Juego.buffer,font,300 + 12*i,250,makecol(255,255,0),-1,"Digite el nombre del nuevo perfil: ");
		draw_sprite(screen,Juego.buffer,0,0);

		do
		{
			lt = readkey();
			if( lt >= 'a' && lt <= 'z' || lt >= 'A' && lt <= 'Z' )
			{
				textprintf_ex(screen,font,300 + 12*i,275,makecol(255,69,0),-1,"%c",lt);
				User->Nombre[i] = lt;
				User->Nombre[i+1] = '\0';
				i++;
			}

			if(lt == 8)// por alguna razon el jodio backspace del teclado trabaja con 8.
			{
				i--;
				User->Nombre[i] = ' ';
				lt = ' ';
				textprintf(screen,font,300 + 12 * i,275,makecol(0,0,0),"%c",lt);
			}

		}while(lt != ENTER && i <= 50);
	}

	else if(tipo == 2)//Cuando se quiere pedir el numero de usuario registrado en el archivo.
	{
		do
		{
			readkey();
		}while(!key[KEY_ENTER]);

		int k = (int)((char)(readkey() - 48));
		return k;
	}
	return 0;
}

/* Esta funcion se encarga de calcular los nuevos highscores y escribirlos en los respectivos archivos de texto. no recibe ningun parametro.
 *VALOR QUE RETORNA- ninguno
*/
void highscore()
{
    int n,i,j,compare,tmp_nvl,num_sec, lim, x;

    USUARIO *user, *tmp;
    std::fstream archi, arch, reg_highscore, tmp_file;
	user = new USUARIO();
	tmp = new USUARIO();

	archi.open("perfiles.DAT",std::ios::binary |std::ios::in);//abrimos el archivo de perfiles en modo lectura.
	arch.open("highmod.DAT",std::ios::binary | std::ios::out);
    
	if(archi.good() )
		x = 1;
	if(arch.good() )
		x = 2;
	if(archi.is_open())
		x = 3;
	if(arch.is_open())
		x = 4;
// se estructura el archivo del highscore.
   
    reg_highscore.open("Highscore.txt",std::ios::out);
	if(reg_highscore.is_open())//Si esta abierto escribimos lo sgt...
	{
		reg_highscore << "_____________________________________________________________________________\n";
		reg_highscore << "\t\tTabla de Altos Puntajes (HIGHSCORE)\n";
		reg_highscore << "_____________________________________________________________________________\n";
		reg_highscore << "#\tNombre\tWave\tPuntaje\t\n";
	}
	reg_highscore.close();
    
//----------------------------------------------------------------------------------------------------------------------------------------------------
	archi.seekg(0, std::ios::end);
	n = archi.tellg() / sizeof(USUARIO);// la cantidad de usuarios registrados...
	archi.seekg(std::ios::beg);

	for(i = 0; i < n; i++)
	{
		archi.read(reinterpret_cast<char*>(tmp + i), sizeof(USUARIO));
		arch.write(reinterpret_cast<char*>(tmp + i), sizeof(USUARIO));//escribo lo que esta en perfiles.dat en mi archivo highmod.DAT
	}

    archi.close();
	arch.close();

    num_sec = 0;
	if(n < 10)
		lim = n;
	else if(n > 10)
		lim = 10;
    for(j=1; j<lim; j++)//1-10, ya que queremos los primero diez solamente.
    {
        ++num_sec;
        arch.open("highmod.DAT",std::ios::binary| std::ios::in);

		arch.seekg(0, std::ios::end);
		n = arch.tellg() / sizeof(USUARIO);// la cantidad de usuarios registrados...
		arch.seekg(std::ios::beg);

        for(i= 0; i<n; i++)
        {
			arch.read(reinterpret_cast<char*>(user + i), sizeof(USUARIO));// leo ese profile

            if(i == 0)//en la primera iteracion copio todo del primer usuario en ese tmp
            {
                strcpy(tmp->Nombre, (user+i)->Nombre);
                tmp->Max_Waves = (user+i)->Max_Waves;
                tmp->Max_Score = (user+i)->Max_Score;
            }
            if((user+i)->Max_Waves > tmp->Max_Waves)//si el ese usuario a llegado a una wave mayor que el otro se copia todo su contenido
            {
                    strcpy(tmp->Nombre, (user+i)->Nombre);
                    tmp->Max_Waves = (user+i)->Max_Waves;
                    tmp->Max_Score = (user+i)->Max_Score;
            }

            if((user+i)->Max_Waves == tmp->Max_Waves)//Si lo niveles son iguales se comparan sus puntajes.
            {
                tmp_nvl = tmp->Max_Waves;
                if((user+i)->Max_Score > tmp->Max_Score)
                {
                        strcpy(tmp->Nombre, (user+i)->Nombre);
                        tmp->Max_Waves = (user+i)->Max_Waves;
                        tmp->Max_Score = (user+i)->Max_Score;
                }
            }
        }
		arch.close();
        //ahora que ya tengo el mayor escribo la nueva data en mi archivo highmod.DAT, escribo todos los usuarios menos el mayor.
        arch.open("highmod.DAT",std::ios::binary | std::ios::in);//si no existe lo crea, asi le añado al final
        tmp_file.open("tmp_hs.DAT",std::ios::binary |std::ios::out );
		for(i=0; i<n; i++)
        {
			arch.read(reinterpret_cast<char*>(user+i), sizeof(USUARIO));

            compare = strcmp(tmp->Nombre, (user+i)->Nombre );
            if(compare == 0)//si son iguales no lo escribo en el nuevo archivo.
                continue;
            else
				tmp_file.write(reinterpret_cast<char*>(user+i), sizeof(USUARIO));
        }
		arch.close();
		tmp_file.close();

		arch.open("highmod.DAT",std::ios::binary |std::ios::out);
		tmp_file.open("tmp_hs.DAT",std::ios::binary | std::ios::in);

		tmp_file.seekg(0, std::ios::end);//se le la nueva cantidad de usuarios
		n = tmp_file.tellg() / sizeof(USUARIO);
        tmp_file.seekg(std::ios::beg);

		for(i = 0; i < n; i++)//escribo todos los usuarios menore que el mayor en mi archivo de highmod.DAT
		{
			tmp_file.read(reinterpret_cast<char*>(user+i), sizeof(USUARIO));
			arch.write(reinterpret_cast<char*>(user+i), sizeof(USUARIO));
		}
		arch.close();
		tmp_file.close();

        //cuando llego aki significa que ya escribi todos los usuarios menores que el mayor en mi archivo highmod.DAT(menos el mayor).
        tmp_nvl = tmp->Max_Waves;

		reg_highscore.open("Highscore.txt",std::ios::out | std::ios::app);
		reg_highscore << num_sec;
		reg_highscore << "\t";
		reg_highscore << tmp->Nombre;
		reg_highscore << "\t";
		reg_highscore << tmp->Max_Waves;
		reg_highscore << "\t";
		reg_highscore << tmp->Max_Score;
		reg_highscore << "\n";
		reg_highscore.close();
            
    }
	
}

/* Esta funcion se encarga de crear los perfiles. no recibe ningun parametro
 *VALOR QUE RETORNA
    esta funcion retorna el usuario creado
*/
void crear_perfil()
{
	clear_to_color(Juego.buffer, makecol(0,0,0));//se limpia el buffer a negro para que se pueda ver el texto.
	//GameRender();
    //Ahora se le inicializan todos sus valores a 0(los que se le pueden)
	Get_String(1);// se le manda uno para que sepa que es el nombre que va a leer
	User->Waves = 1;//todo usuario empieza con el primer wave desbloqueado
	User->Score = 0;
     
    std::fstream archi;
	archi.open ("perfiles.DAT", std::ios::out | std::ios::binary | std::ios::app);//se abre para añadir al final y tambien lee(si no existe lo crea).
    if(archi.bad())// No se leyo bien
        allegro_message("No se pudo cargar el archivo: perfiles.DAT");

	archi.write(reinterpret_cast<char*>(User), sizeof(USUARIO));// se escribe el user en el archivo de Perfiles.DAT

	archi.close();
}

/* Esta funcion se encarga de sobreescribir el historial del usuario actual en el archivo de perfiles.
 *PARAMETROS QUE RECIBE
    recibe el usuario
*/
void guardar_en_archivo()
{
    int n=0,i=0,k=0,j=0, compare, x;
    std::fstream archi, arch;
	//USUARIO *data;

	archi.open("perfiles.DAT", std::ios::binary | std::ios::in);//se abre para lectura
    arch.open("Perfiles_Mod.DAT", std::ios::binary| std::ios::out);//crea el archivo o lo reemplaza.

	if(archi.good() )
		x = 1;
	if(arch.good() )
		x = 2;
	if(archi.is_open())
		x = 3;
	if(arch.is_open())
		x = 4;
	
	archi.seekg(0, std::ios::end);
	n = archi.tellg() / sizeof(USUARIO);// la cantidad de usuarios registrados...
	archi.seekg(std::ios::beg);//para poner el cursor del archivo en el principio

	//data = new USUARIO();
	for(j = 0; j < n; j++)//Para recorrer los usuarios listados en perfiles.DAT
	{
		archi.read(reinterpret_cast<char*>(data + j), sizeof(USUARIO));//leo lo que ahi en el archivo original de perfiles

		compare = strcmp(User->Nombre,(data + j)->Nombre);// (*(data+i)).Nombre  //se comparan los nombre del archivo actual y el current de la lista.
		if(compare == 0)// Si fueron iguales
		{
			(data+j)->Max_Waves = User->Max_Waves;
			(data+j)->Max_Score = User->Max_Score;
			(data+j)->Max_Kills = User->Max_Kills;
		}
		arch.write(reinterpret_cast<char*>(data+j), sizeof(USUARIO));// se escribe el valor nuevo en el archivo.
	}
	arch.close();
	archi.close();

	archi.open ("perfiles.DAT", std::ios::binary | std::ios::out);
	arch.open("Perfiles_Mod.DAT", std::ios::binary| std::ios::in);//para leer solamente

	if(archi.good() )
		x = 1;
	if(arch.good() )
		x = 2;
	if(archi.is_open())
		x = 3;
	if(arch.is_open())
		x = 4;

	for(j = 0; j < n; j++)//Se escribe nuevamente el archivo de los perfiles con la nueva data.
	{
		arch.read(reinterpret_cast<char*>(data + j), sizeof(USUARIO));
		archi.write(reinterpret_cast<char*>(data + j), sizeof(USUARIO));//sobreescribo lo de perfile_Mod.DAT en perfiles.DAT
	}
	//delete(data);
	arch.close();
	archi.close();
}

/* Esta funcion se encarga de cargar los perfiles ya existentes. no recibe ningun parametro
 *VALOR QUE RETORNA
    esta funcion retorna el usuario seleccionado
*/
void cargar_perfil()
{
    int i=0,k=-1,format = 130;
	long int n;
	
    std::fstream archi;
	archi.open ("perfiles.DAT",std::ios::binary | std::ios::in);//se abre para lectura solamente
    if(archi.is_open())
        k = -2;

	archi.seekg(0, std::ios::end);
	n =  archi.tellg() / sizeof(USUARIO);
	archi.seekg(std::ios::beg);//para poner el cursor del archivo en el principio

	clear_to_color(Juego.buffer, makecol(0,0,0));//se limpia el buffer a negro para que se pueda ver el texto.
	GameRender();
	textprintf_ex(screen, font,200,100,makecol(255,255,255),-1,"Leyendo %ld usuario/s del archivo de perfiles...", n);

	textprintf_ex(screen, font,200,110,makecol(255,255,255),-1,"Se han encontrado los siguientes perfiles:");
    for(i= 0; i < n; i++)
	{
		archi.read(reinterpret_cast<char*>(User+i), sizeof(USUARIO) );
		textprintf_ex(screen, font,200,format,makecol(255,255,255),-1,"Perfil %d: %s",i+1, (User+i)->Nombre);
		format+=10;
	}

	textprintf_ex(screen, font,200,format + 20,makecol(255,255,255),-1,"Presione ENTER y luego Digite el # del Usuario: ");
	k = Get_String(2); //para que digite el # usuario.
	archi.close();

    if(k==1)//Si k==1, se eligio el perfil 1, por lo tanto se manda data solamente.
		*User = (*(User+0));
	else
	{
		k--;
		*User = (*(User+k));
	}
}
