///DULAPUL DIN NARNIA 

#include <stdlib.h>///PENTRU EROAREA EXIT(0)
#include <glut.h>///INCLUDE GLU.H SI GL.H             
#include <glaux.h>///PENTRU TEXTURA SI OBIECTE
#include <windows.h> ///PENTRU SUNET
#include <mmsystem.h> ///PENTRU SUNET
#include <math.h>///FUNCTIILE DE UMBRA

#define INIT_VIEW_X 0.0    
#define INIT_VIEW_Y 0.0
#define INIT_VIEW_Z -4.5
#define VIEW_LEFT -2.0
#define VIEW_RIGHT 2.0
#define VIEW_BOTTOM -2.0
#define VIEW_TOP 2.0
#define VIEW_NEAR 1.0
#define VIEW_FAR 350.0


///ILUMINARE
GLfloat AmbientLight[]={0.3,0.3,0.3,1.0};                  
GLfloat DiffuseLight[] ={0.8,0.8,0.8,1.0};
GLfloat SpecularLight[] ={0.5,0.5,0.5,1.0};
GLfloat SpecRef[] = {0.3,0.3,0.3,1.0};
GLfloat pozSursa[] = {-20.0,50.0,50.0,1.0};
GLint Shine =78;

///MISCARE SAGETI
GLint walkX=0,walkY=0;

///MISCARE MOUSE
GLint world=1,lookX=0,lookY=0,oldX=-1,oldY=-1;

///VARIABILE UMBRA
int check;

///VARIABILE MISCARE USA STANGA JOS
GLfloat xST=50;
GLfloat xManerS=37;

///VARIABILE MISCARE USA DREAPTA JOS
GLfloat xDR=-50;
GLfloat xManerD=-37;

///VARIABILE MISCARE USA DREAPTA SUS
GLfloat alfa=0;

///VARIABILE MISCARE USA STANGA SUS
GLfloat beta=0;

///GHIVECIUL(facut din suprafata bezier)
GLfloat ctrlpoints[4][4][3] = {
    {{-1.5, -1.5, 0.0}, {-0.5, -2, 0.0}, 
    {0.5, -2, 0.0}, {1.5, -1.5, 0.0}}, 

    {{-2.5, -0.5, 0.0}, {-4.5, -2.5, 7.0}, 
    {4.5, -2.5, 7.0}, {2.5, -0.5, 0.0}},
 
    {{-2.5, 0.5, 0.0}, {-4.5, 2.5, 7.0}, 
    {4.5, 2.5, 7.0}, {2.5, 0.5, 0.0}}, 

    {{-1.5, 1.5, 0.0}, {-0.5, 2, 0.0}, 
    {0.5, 2, 0.0}, {1.5, 1.5, 0.0}}
};

///PROTOTIP TEXTURA
void incarca_textura(const char* s);

///VARIABILE TEXTURA
GLuint IDtextura, ID1, ID2;
const char* sir;


///FUNCTII UMBRE
void calcCoeficientiPlan(float P[3][3], float coef[4])
{
	float v1[3], v2[3];
	float length;
	static const int x=0;
	static const int y=1;
	static const int z=2;

//calculeaza doi vectori din trei puncte
	v1[x]=P[0][x]-P[1][x];
	v1[y]=P[0][y]-P[1][y];
	v1[z]=P[0][z]-P[1][z];

	v2[x]=P[1][x]-P[2][x];
	v2[y]=P[1][y]-P[2][y];
	v2[z]=P[1][z]-P[2][z];


//se calculeaza produsul vectorial al celor doi vectori
//care reprezinta un al treilea vector perpendicular pe plan
//ale carui componente sunt chiar coeficientii A,B,C din ecuatia planului

	coef[x]=v1[y]*v2[z]-v1[z]*v2[y];
	coef[y]=v1[z]*v2[x]-v1[x]*v2[z];
	coef[z]=v1[x]*v2[y]-v1[y]*v2[x];

//normalizeaza vectorul
	length=(float)sqrt((coef[x]*coef[x])+(coef[y]*coef[y])+(coef[z]*coef[z]));
	coef[x]/=length;
	coef[y]/=length;
	coef[z]/=length;
}

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4],GLfloat mat[4][4])
{
	GLfloat coefPlan[4];
	GLfloat temp;

	//determina coeficientii planului
	calcCoeficientiPlan(puncte,coefPlan);

	//determina si pe D
	coefPlan[3]=-((coefPlan[0]*puncte[2][0])+(coefPlan[1]*puncte[2][1])+(coefPlan[2]*puncte[2][2]));

	//temp=A*xL+B*yL+C*zL+D*w
	temp=coefPlan[0]*pozSursa[0]+coefPlan[1]*pozSursa[1]+coefPlan[2]*pozSursa[2]+coefPlan[3]*pozSursa[3];

	//prima coloana
	mat[0][0]=temp-pozSursa[0]*coefPlan[0];
	mat[1][0]=0.0f-pozSursa[0]*coefPlan[1];
	mat[2][0]=0.0f-pozSursa[0]*coefPlan[2];
	mat[3][0]=0.0f-pozSursa[0]*coefPlan[3];
	//a doua coloana
	mat[0][1]=0.0f-pozSursa[1]*coefPlan[0];
	mat[1][1]=temp-pozSursa[1]*coefPlan[1];
	mat[2][1]=0.0f-pozSursa[1]*coefPlan[2];
	mat[3][1]=0.0f-pozSursa[1]*coefPlan[3];
	//a treia coloana
	mat[0][2]=0.0f-pozSursa[2]*coefPlan[0];
	mat[1][2]=0.0f-pozSursa[2]*coefPlan[1];
	mat[2][2]=temp-pozSursa[2]*coefPlan[2];
	mat[3][2]=0.0f-pozSursa[2]*coefPlan[3];
	//a patra coloana
	mat[0][3]=0.0f-pozSursa[3]*coefPlan[0];
	mat[1][3]=0.0f-pozSursa[3]*coefPlan[1];
	mat[2][3]=0.0f-pozSursa[3]*coefPlan[2];
	mat[3][3]=temp-pozSursa[3]*coefPlan[3];
}


///FUNCTIE TEXTURA
void incarca_textura(const char* s)
{
	AUX_RGBImageRec *pImagineTextura = auxDIBImageLoad(s);

	if (pImagineTextura != NULL)
	{
		glGenTextures(1, &IDtextura);

		glBindTexture(GL_TEXTURE_2D, IDtextura);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}
	if (pImagineTextura)
	{
		if (pImagineTextura->data)
			free(pImagineTextura->data);

		free(pImagineTextura);
	}
}


//////FUNCTII OBIECTE

///FUNCTIE DULAP
void dulap()
{
	glPushMatrix();



	/////SPATE

	glPushMatrix();
	//fata fata
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(0.7,0.7,0.7);
	else
		glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, 10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, 10);
	glTexCoord2f(1.0, 40.0);
	glVertex3f(-200, 100, 10);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	//fata spate
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//fata stanga
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200, -100, 10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, 100, 10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//fata dreapta
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, 10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, -10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//fata sus
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 100, 10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 100, 10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	//fata jos
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, 10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, -100, -10);
	glEnd();
	glPopMatrix();



	////LATERAL STANGA

	//fata spate
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(0.7,0.7,0.7);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	//fata fata
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-220, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-220, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-220, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-220, 100, -10);
	glEnd();
	glPopMatrix();

	//fata stanga
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-220, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200, -100, -10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-220, 100, -10);
	glEnd();
	glPopMatrix();

	//fata dreapta
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-220, -100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-220, 100, 100);
	glEnd();
	glPopMatrix();

	//fata sus
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-220, 100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-220, 100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	//fata jos
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-220, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-220, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-200, -100, 100);
	glTexCoord2f(10.0, 0.0);
	glVertex3f(-200, -100, -10);
	glEnd();
	glPopMatrix();


	///LATERAL DREAPTA

	//fata spate
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(0.7,0.7,0.7);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, -10);
	glEnd();
	glPopMatrix();

	//fata fata
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(220, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(220, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(220, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(220, 100, -10);
	glEnd();
	glPopMatrix();


	//fata stanga
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(220, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(220, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, 100);
	glEnd();
	glPopMatrix();


	//fata dreapta
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else
		glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100,-10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(220, -100, -10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(220, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, -10);
	glEnd();

	//fata sus
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, 100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(220, 100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(220, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, -10);
	glEnd();
	glPopMatrix();

	//fata jos
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(220, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(220, -100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, -100, -10);
	glEnd();
	glPopMatrix();

	///SEPARATOR ORIZONTALA
	//fata spate
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(-120,0,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(200, 100, -10);
	glEnd();
	glPopMatrix();



	///FATA

	///FATA SUS PENTRU CHILOTI DREAPTA
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(140,-50,1);
	glRotatef(alfa,1,0,0);
	//fata spate
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-60, -50, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(60, -50, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(60, 50, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-60, 50, 100);
	glEnd();
	glPopMatrix();


	///FATA SUS PENTRU CHILOTI STANGA
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(140,50,1);
	glRotatef(beta,1,0,0);
	//fata spate
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-60, -50, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(60, -50, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(60, 50, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-60, 50, 100);
	glEnd();
	glPopMatrix();



	///FATA JOS PENTRU TANGA STANGA
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(-60,xST,1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-140, -50, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(140, -50, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(140, 50, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-140, 50, 100);
	glEnd();
	glPopMatrix();


	///FATA JOS PENTRU TANGA DREAPTA
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(-60,xDR,1.1);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-140, -50, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(140, -50, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(140, 50, 100);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-140, 50, 100);
	glEnd();
	glPopMatrix();



	///SUS

	//fata sus
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -10);
	glEnd();
	glPopMatrix();

	//fata jos
	glPushMatrix();
	glBegin(GL_QUADS);
	if(check==0)
	glColor3f(0.8,0.8,0.8);
	else glColor3f(0,0,0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 90, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 90, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 90, -10);
	glTexCoord2f(10.0, 0.0);
	glVertex3f(-200, 90, -10);
	glEnd();
	glPopMatrix();

	///SEPARATOR VERTICALA
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glTranslatef(0,-90,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 90, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 90, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 90, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 90, -10);
	glEnd();
	glPopMatrix();

	//fata sus(asta am adaugat eu)
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glRotatef(90,1,0,0);
	glTranslatef(0,0,-95);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 100, 5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 100, 5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, -5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -5);
	glEnd();
	glPopMatrix();



	///JOS

	//fata sus
	glPushMatrix();
    if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, -100, -10);
	glEnd();
	glPopMatrix();

	//fata jos
	glPushMatrix();
	if(check==0)
	glColor3f(0.8,0.8,0.8);
	else glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -100, 100);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, -100, 100);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, -100, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, -100, -10);
	glEnd();
	glPopMatrix();

	//fata sus(ce am modificat eu)
	glPushMatrix();
	if(check==0)
	glColor3f(1,1,1);
	else glColor3f(0,0,0);
	glRotatef(90,1,0,0);
	glTranslatef(0,0,95);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, 100, 5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(200, 100, 5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(200, 100, -5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-200, 100, -5);
	glEnd();
	glPopMatrix();


///MANER STANGA SUS
	glPushMatrix();
	if(check==0)
	glColor3f(0,0.0,0.0);
	else glColor3f(0,0,0);
	glScalef(2.4,2.4,2.4);
	glTranslatef(50,4,43);
	glBegin(GL_LINES);
	{
		glVertex2f(1,0);
		glVertex2f(10,0);
	}glEnd();
	glPopMatrix();

	glPopMatrix();


	///MANER DREAPTA SUS
	glPushMatrix();
	if(check==0)
	glColor3f(0,0.0,0.0);
	else glColor3f(0,0,0);
	glScalef(2.4,2.4,2.4);
	glTranslatef(50,-4,43);
	glBegin(GL_LINES);
	{
		glVertex2f(1,0);
		glVertex2f(10,0);
	}glEnd();
	glPopMatrix();


	///MANER STANGA JOS
	glPushMatrix();
	if(check==0)
	glColor3f(0,0.0,0.0);
	else glColor3f(0,0,0);
	glScalef(2.4,2.4,2.4);
	glTranslatef(0,xManerS,43);
	glBegin(GL_LINES);
	{
		glVertex2f(1,0);
		glVertex2f(15,0);
	}glEnd();
	glPopMatrix();

	///MANER DREAPTA JOS
	glPushMatrix();
	if(check==0)
	glColor3f(0,0.0,0.0);
	else glColor3f(0,0,0);
	glScalef(2.4,2.4,2.4);
	glTranslatef(0,xManerD,43);
	glBegin(GL_LINES);
	{
		glVertex2f(1,0);
		glVertex2f(15,0);
	}glEnd();
	glPopMatrix();



	glPopMatrix();
}


///FUNCTIE MASA1 (CEA CU CEAINIC)
void masa1()
{
	glPushMatrix();

	///SCAUN STANGA
	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-45,-12,3);
	glScalef(0.5,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-48,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-42,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-48,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-42,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	///SPATARUL LUI DAN
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-49,-6.5,3);
	glRotatef(90,0,0,1);
	glScalef(0.6,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();


	///SCAUN DREAPTA
	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-15,-12,3);
	glScalef(0.5,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-18,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-12,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-18,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-12,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	///SPATARUL LUI DAN
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-11,-6.5,3);
	glRotatef(90,0,0,1);
	glScalef(0.6,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();

	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-30,-10,3);
	glScalef(1,0.1,1);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-35,-14,-1);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-25,-14,-1);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-35,-14,10);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-25,-14,10);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	///CEAINIC
	glPushMatrix();
	if(check==0)
	glColor3f(0,0,1);
	else
		glColor3f(0,0,0);
	glTranslatef(-30,-7.5,5);
	auxSolidTeapot(2);
	glPopMatrix();

	glPopMatrix();
}


///FUNCTIE FLOARE
void floare()
{

	///TULPINA
	glPushMatrix();
	if(check==0)
	glColor3f(0.419608,0.556863,0.137255);
	else
		glColor3f(0,0,0);
	glTranslatef(0,1.0,0);
	glRotatef(45,0,1,0);
	auxSolidCylinder(0.08,4);
	glPopMatrix();

	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0.9,0);
	else glColor3f(0,0,0);
	glTranslatef(0,1.5,0.2);
	auxSolidSphere(0.2);
	glPopMatrix();


	///PETALE
	///CEA DE SUS
glPushMatrix();

glTranslatef(0,0,-0.8);
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else
		glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glTranslatef(0,2.1,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///DREAPTA SUS
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else
		glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(45,0,0,1);
	glTranslatef(1.1,1.6,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///DREAPTA 
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(90,0,0,1);
	glTranslatef(1.6,0.5,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///DREAPTA JOS 
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(-45,0,0,1);
	glTranslatef(-1.1,0.6,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///JOS
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glTranslatef(0.0,1,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///STANGA JOS
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(45,0,0,1);
	glTranslatef(1.1,0.6,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///STANGA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(-90,0,0,1);
	glTranslatef(-1.6,0.5,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

	///STANGA SUS
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,1);
	else glColor3f(0,0,0);
	glRotatef(-180,0,1,0);
	glRotatef(-45,0,0,1);
	glTranslatef(-1.13,1.6,-1);
	glScalef(0.2,0.4,0.4);
	gluDisk(gluNewQuadric(),0,1,100,10);
	glPopMatrix();

glPopMatrix();
}


///FUNCTIE MASA2 (CEA CU GHIVECI)
void masa2()
{
   glPushMatrix();

	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-30,-10,3);
	glScalef(1,0.1,1);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-35,-14,-1);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-25,-14,-1);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-35,-14,9);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-25,-14,9);
	glScalef(0.1,4.0,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();


	///SCAUN STANGA
	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-45,-12,3);
	glScalef(0.5,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-48,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-42,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-48,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-42,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	///SPATARUL LUI DAN
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-49,-6.5,3);
	glRotatef(90,0,0,1);
	glScalef(0.6,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();


	///SCAUN DREAPTA
	///SFERA
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-15,-12,3);
	glScalef(0.5,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();

	///CRACII
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-18,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-12,-15,0.5);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-18,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else
		glColor3f(0,0,0);
	glTranslatef(-12,-15.2,6.2);
	glScalef(0.1,3.5,0.1);
	auxSolidCylinder(3,3);
	glPopMatrix();

	///SPATARUL LUI DAN
	glPushMatrix();
	if(check==0)
	glColor3f(1,0,0);
	else glColor3f(0,0,0);
	glTranslatef(-11,-6.5,3);
	glRotatef(90,0,0,1);
	glScalef(0.6,0.1,0.5);
	auxSolidSphere(10);
	glPopMatrix();


	///GHIVECI
	///MAPARE
    glEnable(GL_MAP2_VERTEX_3); 
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

    glPushMatrix();
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glRotatef(-10,1,0,0);
	if(check==0)
	glColor3f(0,0,1);
	else
		glColor3f(0,0,0);
	glTranslatef(-30,-7.1,2.0);
    glRotatef(100.0, 1.0, 0.0, 0.0);
	glScalef(0.5,0.6,0.6);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20); 
    glPopMatrix();

	///FLOARE
	glPushMatrix();
	glTranslatef(-30,-5,3);;
	glScalef(1.8,1.8,1.8);
	floare();
	glPopMatrix();

	glPopMatrix();
}


///FUNCTIE PARCHET
void floor()
{  
   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);
   sir=".\\parchet.bmp";
   incarca_textura(sir);
   ID1=IDtextura;
   glEnable(GL_TEXTURE_2D);
   glColor3f(1,1,1);
   glTranslatef(0,-22.45,0);
   glRotatef(90,1,0,0);
   glBindTexture( GL_TEXTURE_2D, ID1);

   glPushMatrix();
    glBegin(GL_QUAD_STRIP);
	    glTexCoord2f(0.0, 0.0);
		glVertex2f(-1200,-1200);
		glTexCoord2f(0.0, 100.0);
		glVertex3f(-1200, 2000, -1);
		glTexCoord2f(100.0, 100.0);
		glVertex2f(1200, -1200);
		glTexCoord2f(100.0, 0.0);
		glVertex3f(1200, 2000, -1);
	glEnd();
	glPopMatrix();
}


////DISPLAY
void Display()
{
	///PARTEA CU UMBRELE IN DISPLAY
    GLfloat matUmbra[4][4];
    GLfloat puncte[3][3]={ { -100.0f, -22.0f, 50.0f },{ 100.0f, -22.0f, -20.0f },{ 50.0f, -22.0f, -20.0f }};


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Clear the window

	///FORMAREA UMBREI
    MatriceUmbra(puncte,pozSursa,matUmbra);

	///CONDITIE PENTRU MISCARE MOUSE + SAGETI
    glPushMatrix();
    if(world==1)
	{
       	      glTranslatef(walkX,-1,walkY);
                  glRotatef(lookY,0,1,0);
                  glRotatef(lookX,1,0,0);
	}
	glPushMatrix();

	glTranslatef(-4,2,-75);


//////UMBRE
   glPushMatrix();
   glPushAttrib(GL_LIGHTING_BIT);
   glDisable(GL_LIGHTING);

	glPushMatrix();
		glMultMatrixf((GLfloat*)matUmbra);
		check = 1;

    

	///dulap central
	glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,-5,-15);
	glScalef(0.1,0.1,0.1);
	dulap();
	glPopMatrix();

	///dulap dreapta
	glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,-95,-30);
	glScalef(0.1,0.1,0.1);
	dulap();
	glPopMatrix();

	///dulap stanga
	glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,95,-30);
	glScalef(0.1,0.1,0.1);
	dulap();
	glPopMatrix();

	///masa1
	glPushMatrix();
	glTranslatef(-10,0,3);
	masa1();
	glPopMatrix();


	///masa2
	glPushMatrix();
	glTranslatef(80,0,3);
	masa2();
	glPopMatrix();

	glPopMatrix();

	glPopAttrib();
	glPopMatrix();

    glEnable(GL_LIGHTING);


///MASA1
	glPushMatrix();
	glTranslatef(-10,0,3);
	check=0;
	masa1();
	glPopMatrix();


///MASA2
	glPushMatrix();
	glTranslatef(80,0,3);
	check=0;
	masa2();
	glPopMatrix();

///DULAP
	glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    sir=".\\wood.bmp";
    incarca_textura(sir);
    ID2=IDtextura;
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    glBindTexture( GL_TEXTURE_2D, ID2);

	///CENTRAL
    glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,-5,-15);
	glScalef(0.1,0.1,0.1);
	check=0;
	dulap();
	glPopMatrix();

	///DREAPTA
	glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,-95,-30);
	glScalef(0.1,0.1,0.1);
	check=0;
	dulap();
	glPopMatrix();

	///STANGA
	glPushMatrix();
	glRotatef(90,0,0,1);
    glTranslatef(0,95,-30);
	glScalef(0.1,0.1,0.1);
	check=0;
	dulap();
	glPopMatrix();

	///DOAR DULAPUL NU ARE ILUMINARE
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);




///PODEA
	glPushMatrix();
	floor();
	glPopMatrix();

///DOAR PODEAUA NU ARE ILUMINARE
   glEnable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);


	glPopMatrix();
    glPopMatrix();                                     
    glutSwapBuffers();                            
}


///MUTAREA USILOR DE JOS ALE DULAPULUI + TASTA ESC + SCHIMBARE FUNDAL
void keyboard ( unsigned char key, int x, int y ) 
{
	GLfloat colors[][3] = { { 0.858824f , 0.439216f, 0.576471f},{0.0f,  0.498039f, 1.0f } };
    static int back;


    switch ( key ) {
    case 27:
       exit ( 0 ); 
       break;
	case '1':xST=xST+10;
		xManerS=xManerS+4.2;
		      dulap();
			  glutPostRedisplay();
		     break;

	case '2':xST=xST-10;
		xManerS=xManerS-4.2;
		     dulap();
			 glutPostRedisplay();
		     break;
 
	case '3':xDR=xDR-10;
		xManerD=xManerD-4.2;
		      dulap();
			  glutPostRedisplay();
		     break;

	case '4':xDR=xDR+10;
		xManerD=xManerD+4.2;
		     dulap();
			 glutPostRedisplay();
		     break;

	case '5':alfa=alfa+5;
		     dulap();
			 glutPostRedisplay();
		     break;

	case '6':alfa=alfa-5;
		     dulap();
			 glutPostRedisplay();
		     break;

	case '7':beta=beta-5;
		     dulap();
			 glutPostRedisplay();
		     break;

	case '8':beta=beta+5;
		     dulap();
			 glutPostRedisplay();
		     break;

///schimbare fundal din taste
    default: 
	   back ^= 1;
       glClearColor(colors[back][0], colors[back][1], colors[back][2], 1.0f);
       glutPostRedisplay();
                    }
}


///INITIALIZARI
void SetupRend()
{
    glClearColor(0.858824 , 0.439216, 0.576471,1.0);
    glEnable(GL_DEPTH_TEST);         //Enable depth testing
    glEnable(GL_LIGHTING);             //Enable lighting
    glLightfv(GL_LIGHT0,GL_AMBIENT,AmbientLight);//Set up and enable light zero
    glLightfv(GL_LIGHT0,GL_DIFFUSE,DiffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,SpecularLight);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);                   //Enable color tracking
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);//Set material to follow
    glMaterialfv(GL_FRONT,GL_SPECULAR,SpecRef);//Set specular reflectivity and shine
    glMateriali(GL_FRONT,GL_SHININESS,Shine);



}


///MISCAREA DIN SAGETI
void walk(int key,int x,int y)                                      //change positions using arrow keys
{
    if(key==GLUT_KEY_UP)    walkY+=1;
    if(key==GLUT_KEY_DOWN)  walkY-=1;
    if(key==GLUT_KEY_RIGHT) walkX-=1;
    if(key==GLUT_KEY_LEFT)  walkX+=1;
}


///FUNCTIE MOUSE
void gaze(int x,int y)
{
    if((oldX<0) || (oldY<0))
	{

      oldX=x;
                  oldY=y;
	}
    lookX+=y-oldY;lookY+=x-oldX;oldX=x;oldY=y;
}


///MYRESHAPE
void myReshape(int w, int h)
{
    GLfloat Ratio;
    glViewport(0,0,w,h);
    Ratio=1.0*w/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0,Ratio,VIEW_NEAR,VIEW_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(INIT_VIEW_X,INIT_VIEW_Y,INIT_VIEW_Z);
	glLightfv(GL_LIGHT0, GL_POSITION, pozSursa);
}


///MAIN
void main(int argc, char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800,600);
    glutCreateWindow("Welcome to Narnia!");
	PlaySound("swing.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
    glutReshapeFunc(myReshape);
	glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutIdleFunc(Display);
    glutSpecialFunc(walk);
    glutPassiveMotionFunc(gaze);
    SetupRend();
    glEnable(GL_NORMALIZE);
    glutMainLoop();
}