/*
Graficos por Computadora
Nombre: Práctica 02
Fecha:  06-12-2022
Programador: Rogelio Manríquez Cobián
Objetivo: Utilizando el Lenguaje C, el estándar grafico OpenGL y 
el kit de herramientas de GLUT, implementar la animación dela colisión 
de un grupo de figuras circulares,de la siguiente manera: Iniciar con 10 círculos 
ubicados de forma aleatoria dentro de un plano cuadrado, moverlos los círculos 
en direcciones aleatoriasy a diferentes velocidades, cuando dos círculos colisionen 
entre sí, estos deben de desaparecer, el programa debe terminar hasta que no quede 
ningún círculo activo.
Compilar:
gcc -c collisioncircles.c
Enlazar:
gcc collisioncircles.o -o prog  -l glut32 -l opengl32
*/

#include<gl\glut.h>
#include<math.h>
#include<stdio.h>
#include<windows.h>
#define numC 10

//Estructuras para manejar figuras circulares
typedef struct{ 
    float x;
    float y;
}PUNTO;

typedef struct{ 
    PUNTO c;
    float r;
    char on;
}CIRCULO;

//Prototipos
void display(void);
void Circulo(CIRCULO ci);
void Cuadrantes();
void Animacion(void);

//Constantes para el circulo
const float pi=3.14159;
const float radio = 0.5; //Cambiar tamaño del radio

//Circulos en pantalla y manejo de cada uno de ellas en los cuadrantes
CIRCULO circulos[numC];
float posX[numC];
float posY[numC];

//Numero de Circulos activos
char cOn = numC;
char ini = 0; //Indicamos que el programa aun no inicia
unsigned int speed = 0; //Inicializamos la velocidad de los circulos

int main(int argc, char **argv){
    Cuadrantes(); //Inicializar los circulos en ventana
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Collision of Circles");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,1.0,-1.0); 
    glMatrixMode(GL_MODELVIEW);
    glutDisplayFunc(display);
    glutIdleFunc(Animacion);
    glutMainLoop();
    return 0;
}
  
void display(void){
    unsigned int i;
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    
    // Creacion de circulos en pantalla
    for(i=0; i < numC; i++)
        if(circulos[i].on == 1)
            Circulo(circulos[i]);
    glFlush();

    //Tiempo de espera antes de iniciar la ejecución
    if(ini == 0){
        sleep(3);
        ini = 1;
    }
}

void Circulo(CIRCULO ci){
    float x,y,theta;
    glBegin(GL_LINE_LOOP);
    for(theta=0; theta<6.28; theta+=0.1){
        x=ci.c.x+ci.r*cos(theta);
        y=ci.c.y+ci.r*sin(theta);
        glVertex2f(x,y);
    }
  glEnd(); 
}

void Cuadrantes(){
    unsigned int i;    
    float numsX[numC], numsY[numC];
    //Posiciones aleatorias "puntos"
    for(i=0; i<numC; i++){
        numsX[i] = (rand() % 101)*0.01;
        numsY[i] = (rand() % 101)*0.01;        
    }
    //Asignación aleatoria de puntos, después de la creación de los circulos
    for(i=0; i<numC; i++){
        circulos[i].on = 1;       //Circulo presente-activo       
        circulos[i].r = radio;    //Radio del circulo que aparecerá  
        
        //Creación de la division de cuatro cuadrantes en el plano
        //1 cuadrante

        if(i<numC/4){
            //Creación aleatoria en la posición (x,y) en el plano
            circulos[i].c.x = (rand() % 8);
            circulos[i].c.y = (rand() % 8);
            posX[i] = numsX[i];
            posY[i] = numsY[i];
        }

        //2 cuadrante
        else if(i<numC/2){
            //Creación aleatoria en la posición (x,y) en el plano
            circulos[i].c.x = (rand() % 8)*-1;
            circulos[i].c.y = (rand() % 8);
            posX[i] = numsX[i]*-1;
            posY[i] = numsY[i];
        }

        //3 cuadrante
        else if(i<(numC*3/4)){
            //Creación aleatoria en la posición (x,y) en el plano
            circulos[i].c.x = (rand() % 8)*-1;
            circulos[i].c.y = (rand() % 8)*-1;
            posX[i] = numsX[i]*-1;
            posY[i] = numsY[i]*-1;
        }
        //4 cuadrante
        else{
            //Creación aleatoria en la posición (x,y) en el plano
            circulos[i].c.x = (rand() % 8);
            circulos[i].c.y = (rand() % 8)*-1;
            posX[i] = numsX[i]*1;
            posY[i] = numsY[i]*-1;
        }
    }
}

void Animacion(void){    
    unsigned int i, j;

    //Primer velocidad con las que se maneja los circulos
    if((speed%150)==0){
        for(i=0; i<numC/2; i++){
            circulos[i].c.x += posX[i];
            circulos[i].c.y += posY[i];
            if(circulos[i].c.x + radio > 9.5)
                posX[i]*= -1;
            else if(circulos[i].c.x + radio < - 9.5)
                posX[i]*= -1;
            if(circulos[i].c.y + radio > 9.5)
                posY[i]*= -1;
            else if(circulos[i].c.y + radio < - 9.5)
                posY[i]*= -1;
            for(j=0; j<numC; j++){
                if(j!=i)
                    if(circulos[j].on == 1 && circulos[i].on == 1)
                        if((circulos[i].c.x < circulos[j].c.x + radio) && (circulos[i].c.x > circulos[j].c.x - radio)){
                            if((circulos[i].c.y < circulos[j].c.y + radio) && (circulos[i].c.y > circulos[j].c.y - radio)){
                                circulos[i].on = 0;
                                circulos[j].on = 0;
                                cOn -=2;                            
                            }
                }                  
            }            
        }                
    }

    //Segu velocidad con las que se maneja los circulos
    if((speed%300)==0){
        for(i=numC/2; i<numC; i++){            
            circulos[i].c.x += posX[i];
            circulos[i].c.y += posY[i];
            if(circulos[i].c.x + radio > 9.5)
                posX[i]*= -1;
            else if(circulos[i].c.x + radio < - 9.5)
                posX[i]*= -1;
            if(circulos[i].c.y + radio > 9.5)
                posY[i]*= -1;
            else if(circulos[i].c.y + radio < - 9.5)
                posY[i]*= -1;
            for(j=0; j<numC; j++){
                if(j!=i)
                    if(circulos[j].on == 1 && circulos[i].on == 1)
                        if((circulos[i].c.x < circulos[j].c.x + radio) && (circulos[i].c.x > circulos[j].c.x - radio)){
                            if((circulos[i].c.y < circulos[j].c.y + radio) && (circulos[i].c.y > circulos[j].c.y - radio)){
                                if(cOn == 2)
                                    circulos[i].on = 0;
                                circulos[i].on = 0;
                                circulos[j].on = 0;
                                cOn -=2;                            
                            }
                }                  
            }            
        }                
    }
    speed++;
    if(speed>1000000000)
        speed=0;

    //Ulitmos 2 circulos    
    if(cOn<2){
        sleep(0);
        exit(0); //Termina ejecución de programa cuando los 2 ultimos chocan
    }
    glutPostRedisplay();    
}
