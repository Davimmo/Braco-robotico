#include <GL/glut.h>
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
float baseX = 0.0f;
float angulo1 = 0.0f;
float angulo2 = 0.0f;
float angulo3 = 0.0f;
float aberturaGarra = 15.0f;
int segmentoSelecionado = 1;

float cameraX = 0.0f, cameraY = 0.0f;
float zoom = 1.0f;
int lastMouseX = 0, lastMouseY = 0;
bool arrastandoDireita = false;

void desenhaRetangulo(float largura, float altura) {
    glBegin(GL_POLYGON);
    glVertex2f(-largura / 2.0f, 0.0f);
    glVertex2f(largura / 2.0f, 0.0f);
    glVertex2f(largura / 2.0f, altura);
    glVertex2f(-largura / 2.0f, altura);
    glEnd();
}

void desenhaCirculo(float raio, int segmentos = 30) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segmentos; ++i) {
        float ang = 2.0f * M_PI * i / segmentos;
        glVertex2f(cos(ang) * raio, sin(ang) * raio);
    }
    glEnd();
}

void desenhaGarra(float abertura) {
    // Base da garra (parte fixa)
    glColor3f(0.3f, 0.3f, 0.3f);
    desenhaRetangulo(10.0f, 10.0f);

    // Dedo esquerdo
    glPushMatrix();
        glTranslatef(-5.0f - abertura, 10.0f, 0.0f);  // Movimento horizontal para esquerda
        glTranslatef(0.0f, 20.0f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.7f);
        desenhaRetangulo(5.0f, 40.0f);
    glPopMatrix();

    // Dedo direito
    glPushMatrix();
        glTranslatef(5.0f + abertura, 10.0f, 0.0f);  // Movimento horizontal para direita
        glTranslatef(0.0f, 20.0f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.7f);
        desenhaRetangulo(5.0f, 40.0f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glScalef(zoom, zoom, 1.0f);
    glTranslatef(-cameraX, -cameraY, 0.0f);

    glTranslatef(baseX, 0.0f, 0.0f);

    glColor3f(0.2f, 0.2f, 0.8f);
    desenhaRetangulo(40.0f, 20.0f);

    glTranslatef(0.0f, 20.0f, 0.0f);

    // Junta circular entre base e braço
    glColor3f(0.8f, 0.5f, 0.2f);
    desenhaCirculo(6.0f);

    glPushMatrix();
    glRotatef(angulo1, 0, 0, 1);
    glColor3f(0.6f, 0.6f, 0.6f);
    desenhaRetangulo(20.0f, 60.0f);

    glTranslatef(0.0f, 60.0f, 0.0f);
    glColor3f(0.8f, 0.8f, 0.2f);
    desenhaCirculo(6.0f);

    glPushMatrix();
    glRotatef(angulo2, 0, 0, 1);
    glColor3f(0.3f, 0.6f, 0.3f);
    desenhaRetangulo(15.0f, 50.0f);

    glTranslatef(0.0f, 50.0f, 0.0f);
    glColor3f(0.6f, 0.3f, 0.3f);
    desenhaCirculo(5.0f);

    glPushMatrix();
    glRotatef(angulo3, 0, 0, 1);
    glColor3f(0.9f, 0.7f, 0.2f);
    desenhaRetangulo(10.0f, 40.0f);
    desenhaGarra(aberturaGarra);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    // Instruções
    glLoadIdentity();
    glColor3f(1, 1, 1);
    const char* instrucoes[] = {
        "Teclas:",
        "[1-3] Seleciona segmento manualmente",
        "<-  -> Rotaciona segmento selecionado",
        "A / D Move base horizontalmente",
        "W / S Abre ou fecha a garra",
        "ESC Fecha o programa",
        "",
        "Mouse:",
        "Botao Esquerdo: Seleciona junta clicando sobre ela",
        "Botao Direito: Arrasta a cena (camera)",
        "Scroll: Zoom in / Zoom out"
    };

    int y = 200;
    for (int i = 0; i < sizeof(instrucoes) / sizeof(instrucoes[0]); ++i) {
        glRasterPos2f(-290, y);
        for (const char* p = instrucoes[i]; *p; ++p)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
        y -= 15;
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-300, 300, -50, 250);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
        case '1': segmentoSelecionado = 1; break;
        case '2': segmentoSelecionado = 2; break;
        case '3': segmentoSelecionado = 3; break;
        case 'a': case 'A': baseX -= 10.0f; break;
        case 'd': case 'D': baseX += 10.0f; break;
        case 'w': case 'W':
            aberturaGarra += 2.0f;
            if (aberturaGarra > 20.0f) aberturaGarra = 20.0f;  // Máximo de abertura
            break;
        case 's': case 'S':
            aberturaGarra -= 2.0f;
            if (aberturaGarra < 0.0f) aberturaGarra = 0.0f;  // Mínimo (dedos encostados)
            break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void teclasEspeciais(int tecla, int x, int y) {
    switch (tecla) {
        case GLUT_KEY_LEFT:
            if (segmentoSelecionado == 1) angulo1 += 5.0f;
            else if (segmentoSelecionado == 2) angulo2 += 5.0f;
            else if (segmentoSelecionado == 3) angulo3 += 5.0f;
            break;
        case GLUT_KEY_RIGHT:
            if (segmentoSelecionado == 1) angulo1 -= 5.0f;
            else if (segmentoSelecionado == 2) angulo2 -= 5.0f;
            else if (segmentoSelecionado == 3) angulo3 -= 5.0f;
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            arrastandoDireita = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            arrastandoDireita = false;
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        float worldWidth = 600.0f;
        float worldHeight = 300.0f;
        
        float mx = (x / (float)viewport[2]) * worldWidth - worldWidth/2.0f;
        mx = mx / zoom + cameraX;
        
        float my = ((viewport[3] - y) / (float)viewport[3]) * worldHeight - 50.0f;
        my = my / zoom + cameraY;

        float joint1X = baseX;
        float joint1Y = 20.0f;
        
        if (hypotf(mx - joint1X, my - joint1Y) < 10.0f) {
            segmentoSelecionado = 1;
            glutPostRedisplay();
            return;
        }

        float ang1_rad = angulo1 * M_PI / 180.0f;
        float joint2X = joint1X + sin(ang1_rad) * 60.0f;
        float joint2Y = joint1Y + cos(ang1_rad) * 60.0f;

        if (hypotf(mx - joint2X, my - joint2Y) < 10.0f) {
            segmentoSelecionado = 2;
            glutPostRedisplay();
            return;
        }

        float ang2_rad = (angulo1 + angulo2) * M_PI / 180.0f;
        float joint3X = joint2X + sin(ang2_rad) * 50.0f;
        float joint3Y = joint2Y + cos(ang2_rad) * 50.0f;

        if (hypotf(mx - joint3X, my - joint3Y) < 8.0f) {
            segmentoSelecionado = 3;
            glutPostRedisplay();
            return;
        }
    } else if (button == 3) {
        zoom *= 1.1f;
        glutPostRedisplay();
    } else if (button == 4) {
        zoom /= 1.1f;
        glutPostRedisplay();
    }
}

void motion(int x, int y) {
    if (arrastandoDireita) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        cameraX -= dx / zoom;
        cameraY += dy / zoom;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braco Robótico 2D");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}