// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa b�sico para criar aplicacoes 3D em OpenGL
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include "gl\glut.h"
    static DWORD last_idle_time;
#else
    #include <sys/time.h>
    static struct timeval last_idle_time;
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

GLfloat AspectRatio, AngY=0;

// **********************************************************************
//  void DefineLuz(void)
//
//
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}


// **********************************************************************
//  void init(void)
//		Inicializa os par�metros globais de OpenGL
//
// **********************************************************************
void init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo de tela preto

	glShadeModel(GL_SMOOTH);
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_DEPTH_TEST);
	glEnable ( GL_CULL_FACE );

    // Obtem o tempo inicial
#ifdef WIN32
    last_idle_time = GetTickCount();
#else
    gettimeofday (&last_idle_time, NULL);
#endif

}


float obsX = 0, obsY = 1, obsZ = 0;
float alvoX = obsX+1, alvoY = obsY, alvoZ = obsZ+1;
float angObsX = 0;
float angObsY = 0;

//Mouse Look AT



//Driving Variables
float steeringSpeed = 0.005;
float carX = 1.0f, carZ = -1.0f; // Dire��o do carro
float deltaAngleSteeringHorizontal = 0.0f;
float steeringAngle = 0.0f;
float deltaMoveHorizontal = 0.0f;

//Camera Variables
float angleHorizontal = 0.0f;
float angleVertical = 0.0f;
float cameraVerticalSpeed = 0.005;
float cameraHorizontalSpeed = 0.005;
float mouseX = 0.0f, mouseZ = 5.0f; //Posi��o do carro/c�mera
float mouseLookX = 0.0f, mouseLookY = 0.5f, mouseLookZ= -1.0f; //Direa��o da c�mera
float deltaAngleCameraHorizontal =0.0f;
float deltaAngleVertical = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

void CarController(int action)
{
    switch(action)
    {
    case 0://ignition
        deltaMoveHorizontal = 2.0f;
        break;
    default:

        break;
    }
}

void carMovement(float deltaMoveHorizontal)
{
    steeringAngle += deltaAngleSteeringHorizontal;
	carX = sin(steeringAngle);
	carZ = -cos(steeringAngle);

    mouseX += deltaMoveHorizontal * carX * 0.1f;
	mouseZ += deltaMoveHorizontal * carZ * 0.1f;
}


void PosicUser()
{
	// Define os par�metros da proje��o Perspectiva
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(mouseX, 1.0f, mouseZ,   // Posi��o do Observador
               mouseX + mouseLookX, 1.0f + mouseLookY , mouseZ +mouseLookZ,     // Posi��o do Alvo
			  0.0f,1.0f,0.0f);

}


//Fun��o que transforma c�digo hexadecimal em uma cor de openGL
vector<int> hexToColor(string Entrada)
{
    string aux;
    int i;
    vector<int> resp;

    aux = Entrada.substr(2,2);
    i = (int)strtol(aux.c_str(), NULL,16);
    resp.push_back(i);

    aux = Entrada.substr(4,2);
    i = (int)strtol(aux.c_str(), NULL,16);
    resp.push_back(i);

    aux = Entrada.substr(6,2);
    i = (int)strtol(aux.c_str(), NULL,16);
    resp.push_back(i);

    return resp;
}

// Struct para armazenar um ponto X,Y,Z
typedef struct
{
    float X,Y,Z;
    void Set(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
    void Imprime()
    {
        cout << "X: " << X << " Y: " << Y << " Z: " << Z;
    }
} TPoint;

// Struct para armazenar um tri�ngulo
typedef struct
{
    TPoint P1, P2, P3;
    vector<int> cor;
    void imprime()
    {
        cout << "P1 ";  P1.Imprime(); cout << endl;
        cout << "P2 ";  P2.Imprime(); cout << endl;
        cout << "P3 ";  P3.Imprime(); cout << endl;
    }
} TTriangle;

// Classe para armazenar um objeto 3D
class Objeto3D
{
    vector<TTriangle> faces; // vetor de faces
    //unsigned int nFaces; // Variavel que armazena o numero de faces do objeto
public:
    Objeto3D(string nomearq)
    {
        ifstream inFile;
        inFile.open(nomearq + ".tri");
        float x;
        string color;
        inFile >> x; //joga fora o numero de pontos do txt
        //esse while sempre vai tirar a primeira cordenada
        //TODO: fazer com que "X" seja a cor
        while(inFile >> color)
        {

            //passar o "color" em um conversor de hex -> rgb (tem no moodle)
            vector<TPoint> aa;
            for(int i = 0; i < 3; i++)
            {
                TPoint aux;
                float a,b,c;
                inFile >> a;
                inFile >> b;
                inFile >> c;
                //cout << a << endl;
                aux.Set(a,b,c);
                aa.push_back(aux);
            }

            TTriangle fig;
            fig.P1 = aa[0];
            fig.P2 = aa[1];
            fig.P3 = aa[2];
            fig.cor = hexToColor(color);
            faces.push_back(fig);

            //cout << "oi" << endl;
        }
        inFile.close();
        //verificar se os pontos estao corretos quando o arquivo .tri estiver pronto para teste
    }

    unsigned int getNFaces()
    {
        return faces.size();
    }

    void ExibeObjeto()
    {
        for(int i = 0; i < faces.size(); i++)
        {
            glPushMatrix();
            glColor3f(1,0,1); //colocar cor do triangulo
                glBegin(GL_TRIANGLES);
                    glVertex3f(faces[i].P1.X,faces[i].P1.Y,faces[i].P1.Z);
                    glVertex3f(faces[i].P2.X,faces[i].P2.Y,faces[i].P2.Z);
                    glVertex3f(faces[i].P3.X,faces[i].P3.Y,faces[i].P3.Z);
                glEnd();
            glPopMatrix();
        }
    }
};

//Declara��o de Objetos3D
Objeto3D arvore("treeNOVO");
Objeto3D casa("casaNOVO");
Objeto3D testezera("house");
Objeto3D testezera2("ferrariNOVO");






// **********************************************************************
//  Fun��es de Desenho
//
// **********************************************************************

void DesenhaCubo()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}



void DesenhaParalelepipedo()
{
    glPushMatrix();
        glColor3ub(130,117,95);
        glTranslatef(0,-0.95,0);
        glScalef(1,1,1);
        DesenhaCubo();
    glPopMatrix();
}

void DesenhaPiso()
{
    glBegin ( GL_QUADS );
    glNormal3f(0,1,0);
    glVertex3f(-1.0f,  0.0f, -1.0f);
    glVertex3f(-1.0f,  0.0f,  1.0f);
    glVertex3f( 1.0f,  0.0f,  1.0f);
    glVertex3f( 1.0f,  0.0f, -1.0f);
    glEnd();
}




void DesenhaItem(int item)
{
    switch(item)
    {
    case 1: //Rua
        {
            DesenhaParalelepipedo();
            break;
        }

    case 3: //�rvore
        {
            glPushMatrix();
                glScalef(0.4,0.4,0.4);
                arvore.ExibeObjeto();
            glPopMatrix();
            break;
        }

    case 4: //Casa vertical
        {
            glPushMatrix();
                glRotatef(90,0,1,0);
                glScalef(1.5,1,1.5); //mudar escala
                casa.ExibeObjeto();
            glPopMatrix();
            break;
        }
    //case 5: //Sentinel tower
    }
}

void DesenhaCenario()
{
    ifstream inFile;
    inFile.open("mapa2.txt");
    int x,z;
    inFile >> z;
    glPushMatrix();
        glScalef(z,1,z);
        glColor3ub(59,143,20);
        DesenhaPiso();
    glPopMatrix();

    for(int j = z; j > 0; j-=2)
    {
        glPushMatrix();
            glTranslatef(0,0,j);
            for(int i = z; i > 0; i-=2)
            {
                glPushMatrix();
                    glTranslatef(i,0,0);
                    inFile >> x;
                    DesenhaItem(x);
                glPopMatrix();
            }

            for(int i = 0; i < z; i+=2)
            {
                glPushMatrix();
                    glTranslatef(i*-1,0,0);
                    inFile >> x;
                    DesenhaItem(x);
                glPopMatrix();
            }
        glPopMatrix();
    }

    for(int j = 0; j < z; j+=2)
    {
        glPushMatrix();
            glTranslatef(0,0,j*-1);
            for(int i = z; i > 0; i-=2)
            {
                glPushMatrix();
                    glTranslatef(i,0,0);
                    inFile >> x;
                    DesenhaItem(x);
                glPopMatrix();
            }

            for(int i = 0; i < z; i+=2)
            {
                glPushMatrix();
                    glTranslatef(i*-1,0,0);
                    inFile >> x;
                    DesenhaItem(x);
                glPopMatrix();
            }
        glPopMatrix();
    }

    inFile.close();
}


// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}

// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	glMatrixMode(GL_MODELVIEW);

	/*
	glPushMatrix();
        glScalef(0.4,0.4,0.4);
        testezera2.ExibeObjeto();
	glPopMatrix();
*/
    DesenhaCenario();


	glutSwapBuffers();
}

// **********************************************************************
//  void animate ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void animate()
{
    static float dt;
    static float AccumTime=0;

#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    // Figure out time elapsed since last call to idle function
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif

    if(deltaMoveHorizontal)
        carMovement(deltaMoveHorizontal);


    PosicUser();

    AccumTime +=dt;

    if (AccumTime >=3) // imprime o FPS a cada 3 segundos
    {
        //cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }

    // Sa;va o tempo para o pr�ximo ciclo de rendering
    last_idle_time = time_now;


    // Redesenha
    glutPostRedisplay();
}

void Debug()
{
    cout << "Angle Horizontal: " << angleHorizontal << " Angle Vertical: " << angleVertical<< endl;
    cout << "mouseX: " << mouseX << " mouseZ: " << mouseZ<< endl;
    cout << "mouseLookX: " << mouseLookX << " mouseLookY: " << mouseLookY<< " mouseLookZ: " << mouseLookZ<< endl;
    cout << "deltaAngleCameraHorizontal: " << deltaAngleCameraHorizontal << " deltaAngleVertical: " << deltaAngleVertical<< endl;
    cout << "xOrigin: " << xOrigin << " yOrigin: " << yOrigin<< endl<< endl;
}

// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key )
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;
    case 32:
        CarController(0); //Ignicao
        break;
    case 'g':
        Debug();
        break;
    case 'f':
        if(obsY == 40)
        {
            obsX = 0;
            obsY = 1;
            obsZ = 0;
            angObsY = 0;
        }
        else
        {
            obsX = 3.2;
            obsY = 40;
            obsZ = 2.6;
            angObsX = 180;
            angObsY  = -90;
        }
        break;

    default:
            cout << key;
      break;
  }
}


void mouseMove(int x, int y) {

         // this will only be true when the left button is down
         if (xOrigin >= 0) {

            // update deltaAngle
            deltaAngleCameraHorizontal = (x - xOrigin) * cameraHorizontalSpeed;
            deltaAngleVertical = (y - yOrigin) * cameraVerticalSpeed;

            // update camera's direction
            mouseLookX = sin(angleHorizontal + deltaAngleCameraHorizontal);
            mouseLookY = -cos(angleVertical + deltaAngleVertical);
            mouseLookZ = -cos(angleHorizontal + deltaAngleCameraHorizontal);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
//  cout << endl << "--DELTA ANGLE AT CHANGE = " << deltaAngleCameraHorizontal << "--" << endl;
			angleHorizontal += deltaAngleCameraHorizontal;
			angleVertical += deltaAngleVertical;
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
		}
	}
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void pressionaTecla ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_F11:
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
        case GLUT_KEY_LEFT:
            deltaAngleSteeringHorizontal = -0.1f;
            break;
        case GLUT_KEY_RIGHT:
            deltaAngleSteeringHorizontal = 0.1f;
            break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
	}
}

void soltaTecla(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngleSteeringHorizontal = 0;break;
		case GLUT_KEY_RIGHT : deltaAngleSteeringHorizontal = 0;break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )
{
	glutInit            ( &argc, argv );
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 500 );
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" );

	init ();
    //system("pwd");

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( pressionaTecla );
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(soltaTecla);

	glutIdleFunc ( animate );

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop ( );
	return 0;
}

