#include "Header.h"

int main()
{
	FieldRender Field;
	FieldManager Matrix;
	   	
	Matrix.FixField();

	//Window settings
	RenderWindow window;
	window.create(VideoMode(1280, 768), "Gems");
	window.setFramerateLimit(FPS);

	while (window.isOpen())
	{
		//Main Menu
		MainMenu(&Matrix, &Field, &window);

		//main game
		GameProcess(&Matrix, &Field, &window);
		
		//End
		DeathScreen(&Matrix, &Field, &window);
	}
		
	return 0;

}