#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define pi 3.141592

int maksimalnaDubina = 5;
int tkoJeNaPotezu = 1;
int A[6][7];
int wg, hg;
int p = 0;
int tipIgre = 1;
int potezUPartiji = 1;

const double Xmin = 0.0, Xmax = 7.0;
const double Ymin = 0.0, Ymax = 6.0;

struct stablo
{
	int matrica[6][7];
	double vrijednost;
	int potez;
	int zadnji;
	int djecaKojaPostoje[7];
	struct stablo *roditelj;
	struct stablo *dijete[7];
};

int pobjeda (int [6][7], int *, int *, int *);
int dobra(int, int);
void identificiraj (int [6][7], int [6][7]);
int odigraj(int [6][7], int, int);
int kraj (int [6][7]);
int krajZaHeuristiku (int [6][7], int);
double heuristika (int [6][7], int);
void ispis(int [6][7]);
void generiraj (struct stablo *, int);
void inicijaliziraj (int [6][7]);
void postavi (int [6][7]);
double maxi (double, double);
double mini (double, double);
double maxv(struct stablo *, int);
double minv(struct stablo *, int);
double minimax (struct stablo *, int);
void kompIgraPrvi();
void jaIgramPrvi();
void samProtivSebe();

void kruzic(float x, float y)
{
	int i;
	
	glBegin(GL_POLYGON);
	for (i=0; i<51; i += 2)
	{ 
		glVertex2f(x + 0.48*cos(2*pi/51*i+pi/2), y + 0.48*sin(2*pi/51*i+pi/2));
		glColor3f( 0.0, 0.0, 0.0 ); 
		glVertex2f(x + 0.48*cos(2*pi/51*(i+1)+pi/2), y + 0.48*sin(2*pi/51*(i+1)+pi/2));
	}
	glEnd();

}

void drawScene(void)
{
	int i, j, x=-1, y=-1, k=-1;
	float xSrediste, ySrediste;
	glClearColor(0.6, 0.6, 0.6, 0.6);
	glClear(GL_COLOR_BUFFER_BIT );
	glColor3f( 1.0, 1.0, 1.0 );	
	glLineWidth(1);
	glShadeModel(GL_SMOOTH);
	glBegin( GL_LINES);
	for (i=0; i<8; i++) {
		glVertex2f( i, Ymin);
		glVertex2f( i, Ymax);
	}	
	for (j=0; j<7; j++) {	
		glVertex2f( Xmin, j);
		glVertex2f( Xmax, j);
	}
	glEnd();	
	for (i=0; i<6; i++)
		for (j=0; j<7; j++)
			if (A[i][j]>=0) {
				if (A[i][j]==1) 
					glColor3f(1.0, 0.0, 0.0);
				else 
					glColor3f(1.0, 1.0, 0.0);	
				xSrediste = (float)j+0.5;
				ySrediste = (6-(float)i)-0.5;
				kruzic(xSrediste, ySrediste);
				if (pobjeda(A, &x, &y, &k))
				{
					glLineWidth(5);
					glBegin( GL_LINES);
					glColor3f(1.0, 1.0, 1.0);
					glVertex2f( y + 0.5, 6 - x - 0.5 );
					switch (k)
					{
						case 1:
							glVertex2f( y + 0.5, 6 - x + 2 + 0.5 );
							break;
						case 2:
							glVertex2f( y + 3 + 0.5, 6 - x - 0.5 );
							break;	
						case 3:
							glVertex2f( y + 3 + 0.5, 6 - x + 2 + 0.5 );
							break;					
						case 4:
							glVertex2f( y - 3 + 0.5, 6 - x + 2 + 0.5 );
							break;				
					}
					glEnd();
				}
			}
	if (tipIgre == 0 && potezUPartiji == 1){
		int i;
		double score;
		struct stablo *game;
		tkoJeNaPotezu = 0;
		game = (struct stablo *) malloc(sizeof(struct stablo));
		game -> roditelj = NULL;
		game -> potez = 1;
		identificiraj(game->matrica, A);
		generiraj(game, 0);
		score = minimax(game, maksimalnaDubina);
		for (i = 0; i < 7; i++) {
			if (game -> dijete[i] -> vrijednost == score) {
				game = game -> dijete[i];
				break;
			}
		}
		identificiraj(A, game->matrica);
		potezUPartiji = 0;
		glutPostRedisplay();
	}
	glFlush();
}
void tipkovnica(unsigned char key, int x, int y){
	switch(key){
		case 27:
			exit(0);
			break;
		case 'j': 
			tipIgre = 2;
			break;

	}
}

void mis(int button, int state, int x, int y) 
{
	int i;
	double score;
	struct stablo *game;
	if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		switch (tipIgre){
			case 0:
					if(!odigraj(A, x/100, 0))
						return;
					game = (struct stablo *) malloc(sizeof(struct stablo));
					game -> roditelj = NULL;
					game -> potez = 1;
					identificiraj(game->matrica, A);
					generiraj(game, 0);
					score = minimax(game, maksimalnaDubina);
					for (i = 0; i < 7; i++) {
						if (game -> dijete[i] -> vrijednost == score) {
						game = game -> dijete[i];
						break;
						}
					}
					identificiraj(A, game->matrica);
					glutPostRedisplay();
				break;
			case 1:
				tkoJeNaPotezu = 1;
				game = (struct stablo *) malloc(sizeof(struct stablo));
				game -> roditelj = NULL;
				game -> potez = 0;

				if(!odigraj(A, x/100, 1))
					return;

				identificiraj(game->matrica, A);

				generiraj(game, 0);
				score = minimax(game, maksimalnaDubina);
				for (i = 0; i < 7; i++) {
					if (game -> dijete[i] -> vrijednost == score) {
						game = game -> dijete[i];
						break;
					}
				}
				identificiraj(A, game->matrica);
				glutPostRedisplay();
				break;
			case 2:
				tkoJeNaPotezu = (tkoJeNaPotezu + 1) % 2;
				game = (struct stablo *) malloc(sizeof(struct stablo));
				game -> roditelj = NULL;
				game -> potez = 1 - tkoJeNaPotezu;
				identificiraj(game->matrica, A);
				generiraj(game, 0);
				score = minimax(game, maksimalnaDubina);
				for (i = 0; i < 7; i++) {
					if (game -> dijete[i] -> vrijednost == score) {
						game = game -> dijete[i];
						break;
					}
				}
				identificiraj(A, game->matrica);
				glutPostRedisplay();
				break;
			case 3:
				if(!odigraj(A, x/100, tkoJeNaPotezu))
					return;
				tkoJeNaPotezu = (tkoJeNaPotezu+1)%2;
				glutPostRedisplay();
				break;
		}
	}
}

void resizeWindow( int w, int h )
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;
	glViewport( 0, 0, w, h );
	wg = w = (w==0) ? 1 : w;
	hg = h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );
}

int main(int argc, char* argv[])
{
	inicijaliziraj(A);
	if (argc >= 2)
		tipIgre = atoi(argv[1]);

	if (argc == 3)
		maksimalnaDubina = atoi(argv[2]);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 600);
	glutCreateWindow("Spoji 4");
	glutKeyboardFunc(tipkovnica);
	glutMouseFunc(mis);	
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resizeWindow);
	glutMainLoop();
	
	return 0;
}

int kraj (int a[6][7])
{
	int i, j;
	for (j = 0; j < 7; j++)
		for (i = 5; i > 2 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i - 1][j] && a[i][j] == a[i-2][j] && a[i][j] == a[i-3][j]) return 1;
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 0 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i][j+1] && a[i][j] == a[i][j+2] && a[i][j] == a[i][j+3]) return 1;
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j+1] && a[i][j] == a[i-2][j+2] && a[i][j] == a[i-3][j+3]) return 1;
		}
	for (j = 3; j < 7; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j-1] && a[i][j] == a[i-2][j-2] && a[i][j] == a[i-3][j-3]) return 1;
		}
	return 0;
}
int pobjeda (int a[6][7], int *x1, int *y1, int *k)
{
	int i, j;
	for (j = 0; j < 7; j++)
		for (i = 5; i > 2 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i - 1][j] && a[i][j] == a[i-2][j] && a[i][j] == a[i-3][j])
			{	
				*x1 = i;
				*y1 = j;
				*k = 1;
				return 1;
			}
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 0 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i][j+1] && a[i][j] == a[i][j+2] && a[i][j] == a[i][j+3])
			{	
				*x1 = i;
				*y1 = j;
				*k = 2;
				return 1;
			}
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j+1] && a[i][j] == a[i-2][j+2] && a[i][j] == a[i-3][j+3]) 	
			{	
				*x1 = i;
				*y1 = j;
				*k = 3;
				return 1;
			}
		}
	for (j = 3; j < 7; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j-1] && a[i][j] == a[i-2][j-2] && a[i][j] == a[i-3][j-3]) 
			{	
				*x1 = i;
				*y1 = j;
				*k = 4;
				return 1;
			}
		}
	return 0;
}


int krajZaHeuristiku (int a[6][7], int p)
{
	int i, j;
	for (j = 0; j < 7; j++)
		for (i = 5; i > 2; i--) {	
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i - 1][j] && a[i][j] == a[i-2][j] && a[i][j] == a[i-3][j] && a[i][j] == p) return 1;
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 0 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i][j+1] && a[i][j] == a[i][j+2] && a[i][j] == a[i][j+3] && a[i][j] == p) return 1;
		}
	for (j = 0; j < 4; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j+1] && a[i][j] == a[i-2][j+2] && a[i][j] == a[i-3][j+3] && a[i][j] == p) return 1;
		}
	for (j = 3; j < 7; j++)
		for (i = 5; i >= 3 ; i--) {
			if (a[i][j] == -1) break;
			if (a[i][j] == a[i-1][j-1] && a[i][j] == a[i-2][j-2] && a[i][j] == a[i-3][j-3] && a[i][j] == p) return 1;
		}
	return 0;
}

int dobra(int i, int j)
{
	if (i >= 6 || i < 0 || j < 0 || j >= 7)
		return 0;
	return 1;

}

double heuristika (int a[6][7], int p)
{
	int i, j, k, br1 = 0, br2 = 0, br3 = 0, br4 = 0, br1ZaX = 0, br2ZaX = 0, br3ZaX = 0, br4ZaX = 0;
	int pomocnaMatrica[6][7];

	double sumaZaX = 0;
	double sumaZaO = 0;

	identificiraj(pomocnaMatrica, a);

	if (krajZaHeuristiku(a, (tkoJeNaPotezu + 1)%2)) 
		return 10000;
	if (krajZaHeuristiku(a, tkoJeNaPotezu))
		return -10000;
	
	p = (tkoJeNaPotezu + 1)%2;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			for (k = -3; k <= 3; k++) {
				if (a[i][j]==p) {
					if (dobra(i + k, j + k) && a[i + k][j + k] == p)
						br1ZaX += 2;
					if (dobra(i, j + k) && (a[i][j + k] == p))
						br2ZaX += 2;
					if (dobra(i + k, j) && (a[i + k][j] == p))
						br3ZaX += 2;
					if (dobra(i + k, j - k) && (a[i + k][j - k] == p))
						br4ZaX += 2;			
					if (dobra(i + k, j + k) && (a[i + k][j + k] == p || a[i + k][j + k] == -1))
						br1++;
					if (dobra(i, j + k) && (a[i][j + k] == p || a[i][j + k] == -1))
						br2++;
					if (dobra(i + k, j) && (a[i + k][j] == p || a[i + k][j] == -1))
						br3++;
					if (dobra(i + k, j - k) && (a[i + k][j - k] == p || a[i + k][j - k] == -1))
						br4++;
				}
			}
			if (br1 >= 4) 
				sumaZaX = (sumaZaX + 1 + br1%4)+br1ZaX;
			if (br2 >= 4) 
				sumaZaX = (sumaZaX + 1 + br2%4)+br2ZaX;
			if (br3 >= 4) 
				sumaZaX = (sumaZaX + 1 + br3%4)+br3ZaX;
			if (br4 >= 4) 
				sumaZaX = (sumaZaX + 1 + br4%4)+br4ZaX;
			br1=br2=br3=br4=br1ZaX=br2ZaX=br3ZaX=br4ZaX=0;
		}
	}
	p = tkoJeNaPotezu;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			for (k = -3; k <= 3; k++) {
				if (a[i][j]==p) {
					if (dobra(i + k, j + k) && (a[i + k][j + k] == p))
						br1ZaX+=2;
					if (dobra(i, j + k) && (a[i][j + k] == p))
						br2ZaX+=2;
					if (dobra(i + k, j) && (a[i + k][j] == p))
						br3ZaX+=2;
					if (dobra(i + k, j - k) && (a[i + k][j - k] == p))
						br4ZaX+=2;	
					if (dobra(i + k, j + k) && (a[i + k][j + k] == p || a[i + k][j + k] == -1))
						br1++;
					if (dobra(i, j + k) && (a[i][j + k] == p || a[i][j + k] == -1))
						br2++;
					if (dobra(i + k, j) && (a[i + k][j] == p || a[i + k][j] == -1))
						br3++;
					if (dobra(i + k, j - k) && (a[i + k][j - k] == p || a[i + k][j - k] == -1))
						br4++;
				}
			}
			if (br1 >= 4) 
				sumaZaO = (sumaZaO + 1 + br1%4)+br1ZaX;
			if (br2 >= 4) 
				sumaZaO = (sumaZaO + 1 + br2%4)+br2ZaX;
			if (br3 >= 4) 
				sumaZaO = (sumaZaO + 1 + br3%4)+br3ZaX;
			if (br4 >= 4) 
				sumaZaO = (sumaZaO + 1 + br4%4)+br4ZaX;
			br1=br2=br3=br4=br1ZaX=br2ZaX=br3ZaX=br4ZaX=0;
		}
	}
	return sumaZaX - sumaZaO;
}

int odigraj(int polje[6][7], int brojStupcaUKojiStavlja, int oznakaIgraca)
{
	int i;
	if (kraj(polje)) return 0;
	for (i = 5; i >= 0; i--)
		if (polje[i][brojStupcaUKojiStavlja] == -1) { 
			polje[i][brojStupcaUKojiStavlja] = oznakaIgraca;
			return 1;
		}
	return 0;
}

void ispis(int polje[6][7])
{
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			if (polje[i][j] == -1)
				printf(". ");
			else if (polje[i][j] == 1)
				printf("X ");
			else if (polje[i][j] == 0)
				printf("O ");
		}
		printf("\n");
	}
	for (i = 1; i < 8; i++)
		printf("%d ", i);
	printf("\n");
}

void identificiraj (int a[6][7], int b[6][7])
{
	int i, j;
	for (i = 0; i < 6; i++)
		for (j = 0; j < 7; j++)
			a[i][j] = b[i][j];
}

void generiraj (struct stablo *a, int d)
{
	int i;
	if (d < maksimalnaDubina){
		for (i = 0; i < 7; i++){
			a -> dijete[i] = (struct stablo *) malloc(sizeof(struct stablo));
			identificiraj(a -> dijete[i] -> matrica, a -> matrica);
			a -> dijete[i] -> roditelj = a;
			if(odigraj(a -> dijete[i] -> matrica, i, a -> potez)){
				a -> dijete[i] -> potez = (a -> potez + 1) % 2;
				generiraj(a -> dijete[i], d + 1);
				a -> djecaKojaPostoje[i] = 1;
			}
			else if (kraj(a -> dijete[i] -> matrica)){
				a->djecaKojaPostoje[i] = 1;
				a->dijete[i]->zadnji = 1;
			}
			else {
				a->djecaKojaPostoje[i] = 0;
			}
		}
	}
	return;
}

void inicijaliziraj (int mat[6][7])
{
	int i, j;
	for (i = 0; i < 6; i++)
		for (j = 0; j < 7; j++)
			mat[i][j] = -1;
}

double maxi(double a, double b)
{
	if (a > b) return a;
	else return b;

}
double mini(double a, double b)
{
	if (a > b) return b;
	else return a;
}

double minv(struct stablo *cvor, int d)
{
	int i;
	double v = 100000;
	double val_1;
	if (!d || cvor->zadnji==1) {
		val_1 = heuristika(cvor -> matrica, 1);
		cvor -> vrijednost = val_1;
		return val_1;
	}
	
	for (i = 0; i < 7; i++) {
		if (cvor->djecaKojaPostoje[i]){
			v = mini(v, maxv(cvor -> dijete[i], d - 1));
			cvor -> vrijednost = v;
		}
	}
	return v;
}
double maxv(struct stablo *cvor, int d)
{
	int i;
	double  val_1;
	double v = -100000;
	if (!d || cvor -> zadnji == 1) {
		val_1 = heuristika(cvor -> matrica, 1);
		cvor -> vrijednost = val_1;
		return val_1;
	}
	
	for (i = 0; i < 7; i++) {
		if (cvor->djecaKojaPostoje[i]){
			v = maxi(v, minv(cvor -> dijete[i], d - 1));
			cvor -> vrijednost = v;
		}
	}
	return v;
}

double minimax (struct stablo *cvor, int d)
{
	return maxv(cvor, d);
}
