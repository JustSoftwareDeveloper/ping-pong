#include <iostream>
#include <raylib.h> 
#include <cmath>

//цвета
const Color Green = Color{38, 185, 154, 255};
const Color Dark_Green = Color{20, 160, 133, 255};
const Color Light_Green = Color{129, 204, 184, 255};
const Color Yellow = Color{243, 213, 91, 255 };

//счЄтчики очков
int player_score = 0;
int cpu_score = 0;

//класс дл€ м€ча
class Ball {
public:
	float x, y;
	int speed_x, speed_y, radius;

	//отрисовываем м€ч
	void Draw()
	{
		DrawCircle(x, y, radius, Yellow);
	}

	//обновл€ем положение м€ча
	void Update()
	{
		x += speed_x;
		y += speed_y;

		//мен€ем направление по y, если м€ч коснулс€ борта
		if (y + radius >= GetScreenHeight() || y - radius <= 0) 
			speed_y *= -1;

		//мен€ем направление по x и прибовл€ем счЄт, если м€ч коснулс€ борта
		if (x + radius >= GetScreenWidth())
		{
			speed_x = -abs(speed_x);
			player_score++;
		}

		if (x - radius <= 0)
		{
			speed_x = abs(speed_x);
			cpu_score++;
		}
	}
};

//класс дл€ платформы
class Paddle {
private:
	
	//ограничиваем движение платформы, чтобы не выходила за экран
	void LimitMovement()
	{
		if (y < 0)
			y = 0;

		if (y + height > GetScreenHeight())
			y = GetScreenHeight() - height;
	}

public:
	float x, y, width, height;
	int speed;

	//отрисовываем платформу
	void Draw()
	{
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
	}

	//обновл€ем передвижение игрока
	void Player_Update()
	{
		//мен€ем положение игрока по нажатию стрелочек
		if (IsKeyDown(KEY_UP))
			y -= speed;

		if (IsKeyDown(KEY_DOWN))
			y += speed;

		LimitMovement(); //ограничиваем движение
	}

	//обновл€ем движение компьютера
	void Cpu_Update(float ball_x, float ball_y, int ball_speed)
	{
		if (ball_speed == abs(ball_speed) || ball_x >= GetScreenWidth() - 110)
		{
			//мен€ем положение компьютера относительно положени€ м€ча
			if (y + height / 2 > ball_y)
				y -= speed;

			if (y + height / 2 <= ball_y)
				y += speed;

			LimitMovement(); //ограничиваем движение
		}
	}
};

int main()
{
	const int screen_width = 1280; //ширина экрана
	const int screen_height = 800; //высота экрана
	InitWindow(screen_width, screen_height, "Ping-Pong Game"); //создание окна
	SetTargetFPS(60); //устанавливаем максимальное количество кадров в секунду

	Ball ball; //создаЄм м€ч
	Paddle player; //создаЄм игрока
	Paddle cpu; //создаЄм компьютер

	//характеристики м€ча
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.radius = 20;
	ball.speed_x = 7;
	ball.speed_y = 7;

	//характеристики игрока
	player.width = 25;
	player.height = 120;
	player.x = 10;
	player.y = screen_height / 2 - player.height / 2;
	player.speed = 6;

	//характеристики компьютера
	cpu.width = 25;
	cpu.height = 120;
	cpu.x = screen_width - cpu.width - 10;
	cpu.y = screen_height / 2 - cpu.height / 2;
	cpu.speed = 6;

	//игровой цикл, который закончитьс€ при закрытии окна
	while (!WindowShouldClose())
	{
		BeginDrawing(); //настраиваем холст

		ball.Update(); //обновл€ем м€ч
		player.Player_Update(); //обновл€ем игрока
		cpu.Cpu_Update(ball.x, ball.y, ball.speed_x); //обновл€ем компьютер

		//проверка столкновени€ м€ча с платформой
		if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { player.x, player.y, player.width, player.height }))
			ball.speed_x = abs(ball.speed_x);

		if (CheckCollisionCircleRec({ ball.x, ball.y }, ball.radius, { cpu.x, cpu.y, cpu.width, cpu.height }))
			ball.speed_x = -abs(ball.speed_x);

		//отрисовываем графику
		ClearBackground(Dark_Green);
		DrawRectangle(0, 0, screen_width / 2, screen_height, Green);
		DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
		DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
		ball.Draw();
		player.Draw();
		cpu.Draw();
		DrawText(TextFormat("%i", player_score), screen_width / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", cpu_score), screen_width * 3 / 4 - 20, 20, 80, WHITE);

		EndDrawing(); //заканчиваем рисование
	}

	CloseWindow(); //закрываем окно
	return 0;
}