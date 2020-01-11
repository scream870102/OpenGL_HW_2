// Gouraud shading with a single light source at (4, 4, 0);
// �P Exmaple4 �ۦP, Light ���ѼƧ令�H���c���覡�ǻ�
// �P�ɹ�{ Per Pixel Lighting�BNonphotorealistic Shading  �P Silhouette
//
// ����e���ǳƤu�@
// ���� CShape.h ���� #define LIGHTING_WITHCPU 
// �}�� CShape.h ���� #define LIGHTING_WITHGPU 
// ���� CShape.h ���� #define PERVERTEX_LIGHTING 
//

#include "header/Angel.h"
#include "HW/Utils/Input.h"
#include "HW/Scene.h"
#define SCREEN_SIZE 800
Scene* scene = nullptr;
//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();

void init( void )
{
	
	scene = new Scene();
	scene->Init();
}

//----------------------------------------------------------------------------
void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window
	scene->Draw();
	glutSwapBuffers();	// �洫 Frame Buffer
}


void onFrameMove(float delta)
{
	scene->Update(delta);
	Input::GetInstance()->Update();
	GL_Display();
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
	Input::GetInstance()->KeyPressed(key);
	if (key == 27) {
		Input::GetInstance()->Destroy();
		delete scene;
		exit(EXIT_SUCCESS);
	}
}

//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	Input::GetInstance()->MousePressed(button, state, x, y);
}
//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	Input::GetInstance()->KeyPressed(key);
}

//----------------------------------------------------------------------------
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {
	Input::GetInstance()->MouseMove(x, y);
}

// The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
void Win_MouseMotion(int x, int y) {
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Shading Example 3");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutSpecialFunc(Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);
	glutMainLoop();
	return 0;
}