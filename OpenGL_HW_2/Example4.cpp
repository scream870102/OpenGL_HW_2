// Gouraud shading with a single light source at (4, 4, 0);
// 與 Exmaple4 相同, Light 的參數改成以結構的方式傳遞
// 同時實現 Per Pixel Lighting、Nonphotorealistic Shading  與 Silhouette
//
// 執行前的準備工作
// 關閉 CShape.h 中的 #define LIGHTING_WITHCPU 
// 開啟 CShape.h 中的 #define LIGHTING_WITHGPU 
// 關閉 CShape.h 中的 #define PERVERTEX_LIGHTING 
//

#include "header/Angel.h"
#include "HW/Utils/Input.h"
#include "HW/Scene.h"
#define SCREEN_SIZE 800
Scene* scene = nullptr;
//----------------------------------------------------------------------------
// 函式的原型宣告
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
	glutSwapBuffers();	// 交換 Frame Buffer
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
	glutKeyboardFunc(Win_Keyboard);	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc(Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);
	glutMainLoop();
	return 0;
}