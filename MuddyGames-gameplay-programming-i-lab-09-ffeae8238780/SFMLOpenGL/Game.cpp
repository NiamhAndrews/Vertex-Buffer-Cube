#include <Game.h>

static bool flip;

Game::Game() : window(sf::VideoMode(800, 600), "OpenGL Cube VBO")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	sf::Event event;

	while (isRunning) {

		std::cout << "Game running..." << std::endl;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[3];
} Vertex;

Vertex vertex[8];
GLubyte triangles[36];

/* Variable to hold the VBO identifier */
GLuint vbo[1];
GLuint index;

void Game::initialize()
{
	isRunning = true;

	glewInit();

	/* Vertices counter-clockwise winding */

	vertex[0].coordinate[0] = -1.0f;
	vertex[0].coordinate[1] = -1.0f;
	vertex[0].coordinate[2] = 1.0f;

	vertex[1].coordinate[0] = 1.0f;
	vertex[1].coordinate[1] = -1.0f;
	vertex[1].coordinate[2] = 1.0f;

	vertex[2].coordinate[0] = 1.0f;
	vertex[2].coordinate[1] = 1.0f;
	vertex[2].coordinate[2] = 1.0f;

	vertex[3].coordinate[0] = -1.0f; 
	vertex[3].coordinate[1] = 1.0f;  
	vertex[3].coordinate[2] = 1.0f;

	vertex[4].coordinate[0] = -1.0f; 
	vertex[4].coordinate[1] = -1.0f;  
	vertex[4].coordinate[2] = -1.0f;

	vertex[5].coordinate[0] = 1.0f; 
	vertex[5].coordinate[1] = -1.0f;  
	vertex[5].coordinate[2] = -1.0f;

	vertex[6].coordinate[0] = 1.0f;
	vertex[6].coordinate[1] = 1.0f;
	vertex[6].coordinate[2] = -1.0f;

	vertex[7].coordinate[0] = -1.0f;
	vertex[7].coordinate[1] = 1.0f;
	vertex[7].coordinate[2] = -1.0f;




	vertex[0].color[0] = 1.0f;
	vertex[0].color[1] = 0.3f;
	vertex[0].color[2] = 0.0f;

	vertex[1].color[0] = 0.7f;
	vertex[1].color[1] = 0.0f;
	vertex[1].color[2] = 0.5f;

	vertex[2].color[0] = 0.0f;
	vertex[2].color[1] = 0.5f;
	vertex[2].color[2] = 0.5f;

	vertex[3].color[0] = 0.4f;
	vertex[3].color[1] = 1.0f;
	vertex[3].color[2] = 0.0f;

	vertex[4].color[0] = 0.5f;
	vertex[4].color[1] = 0.5f;
	vertex[4].color[2] = 0.0f;

	vertex[5].color[0] = 0.2f;
	vertex[5].color[1] = 0.0f;
	vertex[5].color[2] = 0.6f;


	triangles[0] = 0;   triangles[1] = 1;	triangles[2] = 1;
	triangles[3] = 2;   triangles[4] = 3;	triangles[5] = 0;
	triangles[6] = 3;	triangles[7] = 7;	triangles[8] = 4;
	triangles[9] = 4;   triangles[10] = 0;	triangles[11] = 3;
	triangles[12] = 4;   triangles[13] = 7;	triangles[14] = 6;
	triangles[15] = 6;	triangles[16] = 5;	triangles[17] = 4;
	triangles[18] = 6;   triangles[19] = 2;	triangles[20] = 1;
	triangles[21] = 1;	triangles[22] = 5;	triangles[23] = 6;
	triangles[24] = 6;	triangles[25] = 7;	triangles[26] = 3;
	triangles[27] = 3;	triangles[28] = 2;	triangles[29] = 6;
	triangles[30] = 0;	triangles[31] = 4;	triangles[32] = 5;
	triangles[33] = 5;	triangles[34] = 1;	triangles[35] = 0;



	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 6, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();

		if (!flip)
		{
			flip = true;
		}
		else
			flip = false;
	}

	if (flip)
	{
		rotationAngle += 0.005f;

		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}

	//Change vertex data
	vertex[0].coordinate[0] += -0.0001f;
	vertex[0].coordinate[1] += -0.0001f;
	vertex[0].coordinate[2] += -0.0001f;

	std::cout << "Update up" << std::endl;
}

void Game::render()
{
	std::cout << "Drawing" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);


	glColorPointer(3, GL_FLOAT, sizeof(Vertex), (char*)NULL + 12);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain 
		model compoents that are and are not to be drawn )	*/
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (char*)NULL + 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	window.display();

}

void Game::unload()
{
	std::cout << "Cleaning up" << std::endl;

	glDeleteBuffers(1, vbo);
}

