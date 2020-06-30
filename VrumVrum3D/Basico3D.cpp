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
  GLfloat LuzAmbiente[]   = {0.6, 0.6, 0.5f } ;
  GLfloat LuzDifusa[]   = {0.8, 0.8, 0.8};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 5.0f, 0.0f };
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



float obsX = 0, obsY = 1, obsZ = 0;
float alvoX = obsX+1, alvoY = obsY, alvoZ = obsZ+1;
float angObsX = 0;
float angObsY = 0;


//Driving Variables
float steeringSpeed = 0.005;
float carRotationAngle = 90;
float carPositionX = 0.0f, carPositionZ = 0.0f;
float carDirectionX = 0.0f, carDirectionZ = 0.0f; // Dire��o do carro
float steeringAngle = 0.0f;
float deltaMoveHorizontal = 0.0f;
bool carroLigado = false;


//Camera Variables
bool overhead = false;
bool cameraMovementToogle = true;
float angleHorizontal = 0.0f;
float angleVertical = 0.0f;
float cameraVerticalSpeed = 0.005;
float cameraHorizontalSpeed = 0.005;
float cameraObserverX = 0.0f, cameraObserverY = 0.0f, cameraObserverZ = -1.0f;
float cameraPositionX = 0.0f, cameraPositionY = 1.0f, cameraPositionZ = 0.0f; //Posi��o da c�mera
float cameraLookX = 0.0f, cameraLookY = 0.5f, cameraLookZ= 0.0f; //Direa��o da c�mera
float deltaAngleCameraHorizontal =0.0f;
float deltaAngleVertical = 0.0f;
int xOrigin = -1;
int yOrigin = -1;







void controleCarro(int action)
{
    switch(action)
    {
    case 0://ignition
        if(!carroLigado)
        {
            deltaMoveHorizontal = 2.0f;
            carroLigado = true;
        }
        else
        {
            deltaMoveHorizontal = 0.0f;
            carroLigado = false;
        }
        break;
    default:
    case 1: //Curva Direita
        steeringAngle += 90;
        break;
    case 2: //Curva Esquerda
        steeringAngle -= 90;
        break;
    }
}



void cameraMovement()
{
    carRotationAngle = 90 - (steeringAngle);
	carDirectionX = sin(steeringAngle*3.14159/180);
	carDirectionZ = -cos(steeringAngle*3.14159/180);


    if(cameraMovementToogle)
    {
        cameraPositionX += deltaMoveHorizontal * carDirectionX * 0.1f;
        cameraPositionZ += deltaMoveHorizontal * carDirectionZ * 0.1f;

        cameraObserverX = cameraPositionX + cameraLookX + carDirectionX;
        cameraObserverY = cameraPositionY + cameraLookY;
        cameraObserverZ = cameraPositionZ + cameraLookZ + carDirectionZ;
    }
    else
    {
        cameraObserverX = carPositionX;
        cameraObserverZ = carPositionZ;

    }
    carPositionX += deltaMoveHorizontal * carDirectionX *0.1f;
    carPositionZ += deltaMoveHorizontal * carDirectionZ *0.1f;


}



void PosicUser()
{
	gluLookAt(cameraPositionX, cameraPositionY, cameraPositionZ,   // Posi��o do Observador
               cameraObserverX, cameraObserverY, cameraObserverZ,     // Posi��o do Alvo
			  0.0f,1.0f,0.0f);

}

void TrocaPerspectiva()
{
    if(overhead)
    {
        cameraMovementToogle = true;
        cameraPositionX = carPositionX;
        cameraPositionY = 1.0f;
        cameraPositionZ = carPositionZ;

    }
    else
    {
        cameraMovementToogle = false;
        cameraPositionX = 0.0f;
        cameraPositionY = 10.0f;
        cameraPositionZ = 0.0f;
    }


    overhead = !overhead;
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

TPoint combusA ,combusB, combusC;
vector<TPoint> combustiveisMapa;
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
            vector<TPoint> aa;
            for(int i = 0; i < 3; i++)
            {
                TPoint aux;
                float a,b,c;
                inFile >> a;
                inFile >> b;
                inFile >> c;

                aux.Set(a,b,c);
                aa.push_back(aux);
            }

            TTriangle fig;
            fig.P1 = aa[0];
            fig.P2 = aa[1];
            fig.P3 = aa[2];
            fig.cor = hexToColor(color);
            faces.push_back(fig);

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
            glColor3f(faces[i].cor[0] ,faces[i].cor[1],faces[i].cor[2]);
            glBegin(GL_TRIANGLES);
                glVertex3f(faces[i].P1.X,faces[i].P1.Y,faces[i].P1.Z);
                glVertex3f(faces[i].P2.X,faces[i].P2.Y,faces[i].P2.Z);
                glVertex3f(faces[i].P3.X,faces[i].P3.Y,faces[i].P3.Z);
            glEnd();
        }
    }
};

//Declara��o de Objetos3D
Objeto3D arvore("treeNOVO");
Objeto3D casa("casaNOVO");
Objeto3D modeloCarro("ferrariNOVO");
Objeto3D dogao("dogNOVO");
Objeto3D cactus("cactus");

// **********************************************************************
//  Fun��es de Desenho
//
// **********************************************************************

void DesenhaQuadrado()
{

}

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
                glScalef(0.4,1,0.4);
                arvore.ExibeObjeto();
            glPopMatrix();
            break;
        }

    case 4: //Casa vertical
        {
            glPushMatrix();
                glRotatef(90,0,1,0);
                glScalef(2,4,2); //mudar escala
                casa.ExibeObjeto();
            glPopMatrix();
            break;
        }
    //case 5: //Sentinel tower
    }
}

vector<TPoint> PreencheMatrix()
{
    ifstream inFile;
    inFile.open("mapa2.txt");
    int x,z;
    inFile >> z;
    vector<TPoint> resp;

    for(int j = z; j > 0; j-=2)
    {
        glPushMatrix();
            glTranslatef(0,0,j);
            for(int i = z; i > 0; i-=2)
            {
                glPushMatrix();
                    glTranslatef(i,0,0);
                    inFile >> x;
                    TPoint a;
                    a.Set(i,x,j);
                    resp.push_back(a);
                glPopMatrix();
            }

            for(int i = 0; i < z; i+=2)
            {
                glPushMatrix();
                    glTranslatef(i*-1,0,0);
                    inFile >> x;
                    TPoint a;
                    a.Set(-i,x,j);
                    resp.push_back(a);
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
                    TPoint a;
                    a.Set(i,x,-j);
                    resp.push_back(a);
                glPopMatrix();
            }

            for(int i = 0; i < z; i+=2)
            {
                glPushMatrix();
                    glTranslatef(i*-1,0,0);
                    inFile >> x;
                    TPoint a;
                    a.Set(-i,x,-j);
                    resp.push_back(a);
                glPopMatrix();
            }
        glPopMatrix();
    }

    inFile.close();
    return resp;
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

//************************************************************************
float CombusTempo = 20;

vector<TPoint> MatrixCenario = PreencheMatrix();
//[0] = posicao | [1] = direcao
typedef struct{
    int pos,dir;
    float cordX, cordY;

    void Set(int p, int d, float x, float y)
    {
        pos = p;
        dir = d;
        cordX = x;
        cordY = y;
    }

} Inimigo;

Inimigo makeEnemy(int p, int d, float x, float y)
{
    Inimigo resp;
    resp.Set(p,d,x,y);
    return resp;
}

vector<Inimigo> makePosEnemys()
{
    vector<Inimigo> resp;
    Inimigo e1 = makeEnemy(1401,1001,0.0,0.0);
    resp.push_back(e1);
    return resp;
}

vector<Inimigo> posEnemys = makePosEnemys();

int hh = 0;
int direcao = 1001;
int a = 740;

//************************************************************************




void inicializaCombustivel()
{
    combusA.Set(37,1515,35);
    combusB.Set(5,202,2);
    combusC.Set(18,735,15);

    combustiveisMapa.push_back(combusA);
    combustiveisMapa.push_back(combusB);
    combustiveisMapa.push_back(combusC);
}

void DesenhaBarrilCombustivel()
{
    for (int i = 0; i < combustiveisMapa.size(); i++)
    {
        glPushMatrix();
            glTranslatef(combustiveisMapa[i].X,0,combustiveisMapa[i].Z);
            glScalef(0.3,0.3,0.3);
            cactus.ExibeObjeto();
        glPopMatrix();
    }
}

void DesenhaBarraCombustivel()
{
    int restoCombus = ceil(CombusTempo);
    if(restoCombus == 0)
        deltaMoveHorizontal = 0.0f;

    glPushMatrix();
        glColor3ub(255,0,0);

        glTranslatef(0.2,0.2,0);

        for (int i = 0; i < restoCombus ; i++)
        {

            glTranslatef(0,0.5,0);
            glPushMatrix();
                glBegin(GL_QUADS);
                    glVertex2f(0.0,0.0);
                    glVertex2f(0.0,0.2);
                    glVertex2f(0.2,0.2);
                    glVertex2f(0.2,0.0);
                glEnd();
            glPopMatrix();
        }


    glPopMatrix();

}

void DesenhaCarro()
{
    glPushMatrix();
        glTranslatef(carPositionX,0.0f ,carPositionZ);
        glPushMatrix();
            glRotatef(carRotationAngle ,0.0f,1.0f,0.0f);
            modeloCarro.ExibeObjeto();
        glPopMatrix();
    glPopMatrix();
}


void colisaoCarro()
{
    for(int i = 0; i < combustiveisMapa.size(); i++)
    {
        if(combustiveisMapa[i].Y == a)
        {
            CombusTempo = 20;
            //combustiveisMapa[i].Set(100,100,100);
        }
    }

}

void display3d()
{
    DefineLuz();

    PosicUser();

    DesenhaCenario();

    DesenhaBarrilCombustivel();

    DesenhaCarro();

    colisaoCarro();

}

void display2d()
{

  DesenhaBarraCombustivel();

}

void SyncMatrixJogador()
{
    //cout << "frente: " << a << endl;
    if(carroLigado)
    {
        int caso = direcao%4;
        //cout << "direcao: " << direcao << endl;
        //cout << "posicao: " << a << endl;

        switch(caso)
        {
        case 0:
            if(MatrixCenario[a+1].Y != 1)
                controleCarro(0);
                //ingicao = 0

            //if(ignicao != 0)
            if(carroLigado)
                a++;
            break;

        case 1:
            if(MatrixCenario[a+40].Y != 1)
                controleCarro(0);
                //ignicao = 0

            //if(ignicao != 0)
            if(carroLigado)
                a += 40;
            break;

        case 2:
            if(MatrixCenario[a-1].Y != 1)
                controleCarro(0);
                //ignicao = 0

            //if(ignicao != 0)
            if(carroLigado)
                a--;
            break;

        case 3:
            if(MatrixCenario[a-40].Y != 1)
                controleCarro(0);
                //ignicao = 0

            //if(ignicao != 0)
            if(carroLigado)
                a -=40;
            break;
        }
    }
}

void SyncMatrixEnemy(int id)
{
    int caso = posEnemys[id].dir%4;
    switch(caso)
    {
    case 0:
        if(MatrixCenario[posEnemys[id].pos+1].Y != 1)
            //ingicao = 0

        posEnemys[id].pos++;
        break;

    case 1:
        if(MatrixCenario[posEnemys[id].pos+40].Y != 1)
            //ignicao = 0

        posEnemys[id].pos += 40;
        break;

    case 2:
        if(MatrixCenario[posEnemys[id].pos-1].Y != 1)
            //ignicao = 0

        posEnemys[id].pos--;
        break;

    case 3:
        if(MatrixCenario[posEnemys[id].pos-40].Y != 1)
            //ignicao = 0

        posEnemys[id].pos -=40;
        break;
    }
}

void MovimentaEnemys()
{
    for(int i = 0; i < posEnemys.size(); i++)
    {
       dogao.ExibeObjeto();
       SyncMatrixEnemy(i,posEnemys[i].pos);
    }
}

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,AspectRatio,0.01,200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    display3d();

    //20 == 2/0.1 | 2 = distancia , 0.1 = velocidade
    if(hh%10 == 0)
    {
        SyncMatrixJogador();
        //movimenta enemys
        //cout << "teste flood" << endl;
    }

    hh++;

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 10, 0, 10, 0.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    display2d();

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

//AQui
    if(deltaMoveHorizontal)
    {
        cameraMovement();
    }



//AQUI

    AccumTime +=dt;
    if(carroLigado)
        CombusTempo -= dt;


    if (AccumTime >=10) // imprime o FPS a cada 3 segundos
    {
        cout << 1.0/dt << " FPS"<< endl;
        AccumTime = 0;
    }

    // Sa;va o tempo para o pr�ximo ciclo de rendering
    last_idle_time = time_now;

    glutPostRedisplay();
}

void Debug()
{
    cout << "Car direction X: " << carDirectionX << " Car direction Z: " << carDirectionZ << endl;
    cout << "Car Position X: " << carPositionX << " Car Position Z: " << carPositionZ << endl;
    cout << "Camera Direction X: " << cameraPositionX + cameraLookX<< " Camera Direction Y: " << cameraPositionY + cameraLookY<< " Camera Direction Z: " << cameraPositionZ + cameraLookZ<< endl;
    cout << "Steering Angle: " << steeringAngle << " Rotation Angle: " << carRotationAngle << endl;
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
        controleCarro(0); //Ignicao
        break;
    case 'a':
        controleCarro(2); //Movimento Esquerda
        direcao--;
        break;
    case 'd':
        controleCarro(1); //Movimento Direita
        direcao++;
        break;
    case 'g':
        Debug();
        break;
    case 'f': //Muda a vis�o
        TrocaPerspectiva();
        break;

    default:
            cout << key;
      break;
  }
}


void mouseMove(int x, int y) {

         if (xOrigin >= 0) {

            deltaAngleCameraHorizontal = ((x - xOrigin)) * cameraHorizontalSpeed;
            deltaAngleVertical = (y - yOrigin) * cameraVerticalSpeed;

            cameraLookX = sin(angleHorizontal + deltaAngleCameraHorizontal);
            cameraLookY = -cos(angleVertical + deltaAngleVertical);
            cameraLookZ = -cos(angleHorizontal + deltaAngleCameraHorizontal);


	}
}

void mouseButton(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			angleHorizontal += deltaAngleCameraHorizontal;
			angleVertical += deltaAngleVertical;
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {
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
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 );
			break;
		default:
			break;
	}
}

void init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Fundo de tela preto

	glShadeModel(GL_SMOOTH);
	glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_DEPTH_TEST);
	glEnable ( GL_CULL_FACE );


    inicializaCombustivel();




    // Obtem o tempo inicial
#ifdef WIN32
    last_idle_time = GetTickCount();
#else
    gettimeofday (&last_idle_time, NULL);
#endif

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
	glutCreateWindow    ( "Racing 3D" );

	init ();
    //system("pwd");

	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( pressionaTecla );

	glutIdleFunc ( animate );
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop ( );
	return 0;
}

// **********************************************************************
//  void init(void)
//		Inicializa os par�metros globais de OpenGL
//
// **********************************************************************




