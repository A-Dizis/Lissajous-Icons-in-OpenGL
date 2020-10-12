#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <unistd.h>

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

#include <stdarg.h>
#include <math.h>

int main()
{
    double t = 0;
    double x, y;
    FILE *fp;

    fp = fopen("Lissajous.txt", "w");

    for(t=0; t<200; t=t+0.01)
	{
	    x = 0.5+0.5/*exp(-0.20*t)*/*sin(11*t + 0);
	    y = 0.5+0.5/*exp(-0.25*t)*/*sin(13*t + 3.14159/2);

	    fprintf(fp, "%lf\t%lf\n", x, y);
	}
    
    fclose(fp);

    int p = 0, flag = 1;

    fp = fopen("Lissajous.txt", "r");
    //Create GLFW window
    GLFWwindow *window;

    if(!glfwInit())
	{
	    return -1;
	}

    //initialize window
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MY WINDOW", NULL, NULL);

    //check if window is opened
    if(!window)
	{
	    glfwTerminate();
	    return -1;
	}

    //Make window current
    glfwMakeContextCurrent(window);
    
    //Coordinates Change from -1->1 to new System with Pixels
    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);//Set bounds from 0->640, 0->480
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();//EveryTime we set glMatrixMode
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);//New coordinates
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat matrixir[20002];
    GLfloat matrixi[20002];
    
     for(p=0; p<10001; p++)
	 {
	     fscanf(fp, "%f\t%f\t", &matrixir[2*p], &matrixir[2*p+1]);
	     matrixi[2*p] = 0;
	     matrixi[2*p+1] = 0;
	 }
     for(p=0; p<10001; p++)
	 {
	     matrixir[2*p] = matrixir[2*p]*SCREEN_WIDTH;
	     matrixir[2*p+1] = matrixir[2*p+1]*SCREEN_HEIGHT;///2;//+0.5*SCREEN_HEIGHT;
	 }
     int k = 0;
    //If windows is not closed by the user, do..
    while(!glfwWindowShouldClose(window))
	{
	    glClear(GL_COLOR_BUFFER_BIT);//circle points not square

	    for(p=0; p<k; p++)
		{
		    matrixi[2*p] = matrixir[2*p];
		    matrixi[2*p+1] = matrixir[2*p+1];
		}
	    if(k<10000)
		k++;
	    else
		return 0;
	   
            //Here goes the code for OpenGL

	    glEnable( GL_POINT_SMOOTH );//supposed to make circle points
	    
	    glEnableClientState(GL_VERTEX_ARRAY);
	    glPointSize(1.3);
	    glVertexPointer(2, GL_FLOAT, 0, matrixi);
	    glDrawArrays(GL_LINE_STRIP, 0, k);
	    glDisableClientState(GL_VERTEX_ARRAY);

	    
	    glDisable(GL_POINT_SMOOTH);
	       
	    //Swap front and back buffers
	    glfwSwapBuffers(window);

	    //Pool for and process events
	    glfwPollEvents();

	    if(flag == 1)
	      {
		usleep(15000);
		flag = 0;
	      }
	    
	    usleep(15000);

	}
    //If finished terminate the window
    glfwTerminate();
    
    return 0;
}
