// **********************************************************************
// PUCRS/Escola Politécnica
// COMPUTAÇÃO GRÁFICA
//
// Programa básico para criar aplicacoes 3D em OpenGL
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
//  void DesenhaCubo()
//
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
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posição da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

 glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de iluminação
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz número Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentraçãoo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado será o brilho. (Valores válidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}


// **********************************************************************
//  void init(void)
//		Inicializa os par‚metros globais de OpenGL
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
// **********************************************************************
//  void PosicUser()
//
//
// **********************************************************************

float obsX = 0, obsY = 1, obsZ = 0;
float alvoX = obsX+1, alvoY = obsY, alvoZ = obsZ+1;
float angObsX = 0;
float angObsY = 0;

void MovimentaObservador(int direcao)
{
    switch(direcao)
    {
        case 1: //frente
            glPushMatrix();
                obsX = obsX+0.7*((cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)));
                obsZ = obsZ+0.7*((-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)));
                alvoX = alvoX+0.7*((cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)));
                alvoZ = alvoZ+0.7*((-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)));
            glPopMatrix();
            break;

        case 2: //tr·s
            glPushMatrix();
                obsX = obsX-0.7*((cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)));
                obsZ = obsZ-0.7*((-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)));
                alvoX = alvoX-0.7*((cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)));
                alvoZ = alvoZ-0.7*((-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)));
            glPopMatrix();
            break;

        case 4: //esquerda
            angObsX += 6;
            alvoX = (cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)) + obsX;
            alvoZ = (-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)) + obsZ;
            break;

        case 3: //direita
            angObsX -= 6;
            alvoX = (cos((angObsX*3.14)/180) + sin((angObsX*3.14)/180)) + obsX;
            alvoZ = (-(sin((angObsX*3.14)/180)) + cos((angObsX*3.14)/180)) + obsZ;
//            alvoX = ((alvoX - obsX)*cos((-6*3.14)/180) + (alvoZ - obsZ)*sin((-6*3.14)/180)) + obsX;
//            alvoZ = (-((alvoX - obsX)*sin((-6*3.14)/180)) + (alvoZ - obsZ)*cos((-6*3.14)/180)) + obsZ;
            break;

        case 5:
            angObsY -= 6;
            alvoY = (cos((angObsY*3.14)/180) + sin((angObsY*3.14)/180)) + obsY;
            alvoZ = (-(sin((angObsY*3.14)/180)) + cos((angObsY*3.14)/180)) + obsZ;
            break;

        case 6:
            angObsY += 6;
            alvoY = (cos((angObsY*3.14)/180) + sin((angObsY*3.14)/180)) + obsY;
            alvoZ = (-(sin((angObsY*3.14)/180)) + cos((angObsY*3.14)/180)) + obsZ;
            break;
    }
}

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

typedef struct  // Struct para armazenar um ponto
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


typedef struct // Struct para armazenar um tri‚ngulo
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

void PosicUser()
{
	// Define os parâmetros da projeção Perspectiva
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90,AspectRatio,0.01,200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsX, obsY, obsZ,   // Posição do Observador
              alvoX,alvoY,alvoZ,     // Posição do Alvo
			  0.0f,1.0f,0.0f);

}

Objeto3D arvore("treeNOVO");
Objeto3D casa("casaNOVO");

void DesenhaItem(int item)
{
    switch(item)
    {
    case 1: //rua
        {
            DesenhaParalelepipedo();
            break;
        }

    case 3: //arvore
        {
            glPushMatrix();
                glScalef(0.4,0.4,0.4);
                arvore.ExibeObjeto();
            glPopMatrix();
            break;
        }

    case 4: //casa
        {
            glPushMatrix();
                glScalef(1.5,1,1.5); //mudar escala
                casa.ExibeObjeto();
            glPopMatrix();
            break;
        }
    //case 5: //sentinel tower
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

    for(int j = 0; j < z; j+=2)
    {
        glPushMatrix();
            glTranslatef(0,0,j);
            for(int i = 0; i < z; i+=2)
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
            for(int i = 0; i < z; i+=2)
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

	// Evita divisão por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a relação entre largura e altura para evitar distorção na imagem.
    // Veja função "PosicUser".
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
Objeto3D testezera("house");
Objeto3D testezera2("ferrariNOVO");

void display( void )
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();

	glMatrixMode(GL_MODELVIEW);

//	glPushMatrix();
//		glTranslatef ( 2.0f, 0.0f, 1.0f );
//        glRotatef(AngY,0,1,0);
//		glColor3f(0.5f,0.0f,0.0f); // Vermelho
//		DesenhaCubo();
//	glPopMatrix();
//
//	glPushMatrix();
//		glTranslatef ( -2.0f, 2.0f, -1.0f );
//		glRotatef(AngY,0,1,0);
//		glColor3f(0.0f,0.6f,0.0f); // Verde
//		DesenhaCubo();
//	glPopMatrix();

	glPushMatrix();
        glScalef(0.4,0.4,0.4);
        testezera2.ExibeObjeto();
	glPopMatrix();

    DesenhaCenario();

//    glPushMatrix();
//        glScalef(0.4,0.4,0.4);
//        glTranslatef(10,0,10);
//        arvore.ExibeObjeto();
//	glPopMatrix();

    cout << "x: " << obsX << " z: " << obsZ << endl;
    cout << angObsX << " | " << angObsY << endl;

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
    AccumTime +=dt;
    if (AccumTime >=3) // imprime o FPS a cada 3 segundos
    {
        cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }
    //cout << "AccumTime: " << AccumTime << endl;
    // Anima cubos
    AngY = AngY + 1;
    // Sa;va o tempo para o próximo ciclo de rendering
    last_idle_time = time_now;

        //if  (GetAsyncKeyState(32) & 0x8000) != 0)
          //  cout << "Espaco Pressionado" << endl;

    // Redesenha
    glutPostRedisplay();
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
    case 'w':
        MovimentaObservador(1);
        break;
    case 'a':
        MovimentaObservador(4);
        break;
    case 's':
        MovimentaObservador(2);
        break;
    case 'd':
        MovimentaObservador(3);
        break;

    case 'q':
        MovimentaObservador(5);
        break;

    case 'e':
        MovimentaObservador(6);
        break;

    case 'f':
        if(obsY == 18)
        {
            obsX = 0;
            obsY = 1;
            obsZ = 0;
            angObsY = 0;
        }
        else
        {
            obsX = 3.2;
            obsY = 18;
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

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
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
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );
	return 0;
}
