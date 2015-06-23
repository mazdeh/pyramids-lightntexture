/*
 *  Vahid M. Mazdeh
 *  06/20/2015
 *  The Great Pyramids of Giza, Egypt. with Lighting and Texture
 *
 *  Key bindings:
 *  l/L        Toggle lighting on/off
 *  t/T        Toggle textures on/off
 *  p/P        Toggle projection between orthogonal/perspective
 *  b/B        Toggle display of quads
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode on/off
 *  F3         Toggle two sided mode on/off
 *  'i'        Toggle light at infinity
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  []         Decrease/increase light elevation
 *  {}         Decrease/increase spot cutoff
 *  1/!        Decrease/increase constant attenuation
 *  2/@        Decrease/increase linear attenuation
 *  3/#        Decrease/increase quadratic attenuation
 *  x/X        Decrease/increase light X-position
 *  y/Y        Decrease/increase light Y-position
 *  z/Z        Decrease/increase light Z-position
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int side=0;       //  Two sided mode
int ntex=1;       //  Texture mode
int th=0,ph=0;    //  View angles
int Th=0,Ph=30;   //  Light angles
float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
int at0=100;      //  Constant  attenuation %
int at1=0;        //  Linear    attenuation %
int at2=0;        //  Quadratic attenuation %
int fov=53;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=1200;     //  Size of world
// Light values
int inf       =   0;  // Infinite distance light
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =   0;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[]={1}; // Shininess (value)
float X       = 0;    // Light X position
float Y       = 500;    // Light Y position
float Z       = 1;    // Light Z position

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Yellow ball
   glColor3f(1,1,0);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,0);
   glScaled(dx,dy,dz);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   // glBindTexture(GL_TEXTURE_2D,texture[0]);

   //  Front
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3d( 0, 0, dz);
   glTexCoord2f(0,0);   glVertex3f(-1,0, 1);
   glTexCoord2f(1,0);   glVertex3f(+1,0, 1);
   glTexCoord2f(1,1);   glVertex3f(+1,+2, 1);
   glTexCoord2f(0,1);   glVertex3f(-1,+2, 1);
   glEnd();
   //  Back
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3d( 0, 0,-dz);
   glTexCoord2f(0,0);   glVertex3f(+1,0,-1);
   glTexCoord2f(1,0);   glVertex3f(-1,0,-1);
   glTexCoord2f(1,1);   glVertex3f(-1,+2,-1);
   glTexCoord2f(0,1);   glVertex3f(+1,+2,-1);
   glEnd();
   //  Right
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3d(+dx, 0, 0);
   glTexCoord2f(0,0);   glVertex3f(+1,0,+1);
   glTexCoord2f(1,0);   glVertex3f(+1,0,-1);
   glTexCoord2f(1,1);   glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1);   glVertex3f(+1,+2,+1);
   glEnd();
   //  Left
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3d(-dx, 0, 0);
   glTexCoord2f(0,0);   glVertex3f(-1,0,-1);
   glTexCoord2f(1,0);   glVertex3f(-1,0,+1);
   glTexCoord2f(1,1);   glVertex3f(-1,+2,+1);
   glTexCoord2f(0,1);   glVertex3f(-1,+2,-1);
   glEnd();
   //  Top
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3d( 0,+dy, 0);
   glTexCoord2f(0,0);   glVertex3f(-1,+2,+1);
   glTexCoord2f(1,0);   glVertex3f(+1,+2,+1);
   glTexCoord2f(1,1);   glVertex3f(+1,+2,-1);
   glTexCoord2f(0,1);   glVertex3f(-1,+2,-1);
   glEnd();
   //  Bottom
   glColor3f(0.730, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-dy, 0);
   glTexCoord2f(0,0);   glVertex3f(-1,0,-1);
   glTexCoord2f(1,0);   glVertex3f(+1,0,-1);
   glTexCoord2f(1,1);   glVertex3f(+1,0,+1);
   glTexCoord2f(0,1);   glVertex3f(-1,0,+1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}



// Render a pyramid consisting of 4 triangles
static void pyramid(double x, double y, double z,
                     double dx, double dy, double dz,
                     double th) {
   
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glPushMatrix();                  // Reset the model-view matrix
   
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

    //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   // glBindTexture(GL_TEXTURE_2D,texture[0]);
 
   // glBegin(GL_TRIANGLES);
   // Front
   glColor3f(1, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   
      glNormal3d( 0, 0, dz);
   glBegin(GL_TRIANGLES);
      glTexCoord2f(0,0);   glVertex3f( 0.0f, 2.0f, 0.0f);
      glTexCoord2f(1,0);   glVertex3f(-1.0f, 0.0f, 1.0f);
      glTexCoord2f(0,1);   glVertex3f(1.0f, 0.0f, 1.0f);
   glEnd();

   // Right
   glColor3f(1, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   
      glNormal3d( dx, 0, 0);
   glBegin(GL_TRIANGLES);
      glTexCoord2f(0,0);   glVertex3f(0.0f, 2.0f, 0.0f);
      glTexCoord2f(1,0);   glVertex3f(1.0f, 0.0f, 1.0f);
      glTexCoord2f(0,1);   glVertex3f(1.0f, 0.0f, -1.0f);
   glEnd();

   // Back
   glColor3f(1, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   
      glNormal3d( 0, 0, -dz);
   glBegin(GL_TRIANGLES);
      glTexCoord2f(0,0);   glVertex3f(0.0f, 2.0f, 0.0f);
      glTexCoord2f(1,0);   glVertex3f(1.0f, 0.0f, -1.0f);
      glTexCoord2f(0,1);   glVertex3f(-1.0f, 0.0f, -1.0f);
   glEnd();

   // Left
   glColor3f(1, 0.55, 0.34);
   // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   
      glNormal3d( -dx, 0, 0);
   glBegin(GL_TRIANGLES);
      glTexCoord2f(0,0);   glVertex3f( 0.0f, 2.0f, 0.0f);
      glTexCoord2f(1,0);   glVertex3f(-1.0f,0.0f,-1.0f);
      glTexCoord2f(0,1);   glVertex3f(-1.0f,0.0f, 1.0f);
   glEnd();
   

      // Bottom
      glColor3f(0.730, 0.55, 0.34);
      // if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
      glBegin(GL_QUADS);
         glNormal3d( 0, -1, 0);
         glTexCoord2f(0,0);  glVertex3f(-1.0f,0.0f, 1.0f);
         glTexCoord2f(1,0);   glVertex3f(-1.0f, 0.0f, -1.0f);
         glTexCoord2f(1,1);   glVertex3f(1.0f, 0.0f, -1.0f);
         glTexCoord2f(0,1);   glVertex3f(1.0f, 0.0f, 1.0f);
      glEnd();
 
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}



/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2000;  //  Length of axes

   float Position[] = {X+700*Cos(Th)-400,Y+50*Sin(Th),300+Z+1200*Sin(Th),1-inf};
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   glShadeModel(smooth?GL_SMOOTH:GL_FLAT);
   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Spotlight color and direction
      float yellow[] = {1.0,1.0,0.0,1.0};
      float Direction[] = {Cos(Th)*Sin(Ph),Sin(Th)*Sin(Ph),-Cos(Ph),0};
      //  Draw light position as ball (still no lighting here)
      ball(Position[0],Position[1],Position[2] , 10);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  Two sided mode
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Set specular colors
      glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
      glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      //  Set spotlight parameters
      glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
      glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
      glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
      //  Set attenuation
      glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);
   }
   else
      glDisable(GL_LIGHTING);
   //  Enable textures
   if (ntex)
      glEnable(GL_TEXTURE_2D);
   else
      glDisable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   


   // drawing the 3 great pyramids of Egypt
      pyramid(0,0,0,      230.124,138.684,230.124, 0);
       
      pyramid(300,0,115,  30,30,30, 0);
      pyramid(300,0,20,  30,30,30, 0);
      pyramid(300,0,-75,  30,30,30, 0);

      pyramid(-500,0,600, 230,136,230, 0);

      cube(-100, 0, 600, 115, 30, 50, 0);
      // sphere2(-20, 90, 600, 30);

      pyramid(-900,0,990, 103.327,60.96,103.327, 0);
      
      // drawig the queens' pyramids
      pyramid(-900,0, 1200, 30, 30, 30, 0);
      pyramid(-1000,0, 1200, 30, 30, 30, 0);
      pyramid(-800,0, 1200, 30, 30, 30, 0);
   


   glDisable(GL_TEXTURE_2D);
   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s Light=%s",
     th,ph,dim,mode?"Orthogonal":"Perpective",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,65);
      Print("Cutoff=%.0f Exponent=%.1f Direction=%d,%d Attenuation=%.2f,%.2f,%.2f",sco,Exp,Th,Ph,at0/100.0,at1/100.0,at2/100.0);
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s TwoSided=%s Position=%.1f,%.1f,%.1f,%.1f",smooth?"Smooth":"Flat",local?"On":"Off",side?"On":"Off",Position[0],Position[1],Position[2],Position[3]);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   Th = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Increase/decrease asimuth
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Increase/decrease elevation
   else if (key == GLUT_KEY_UP)
      ph += 5;
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   //  Two sided mode
   else if (key == GLUT_KEY_F3)
      side = 1-side;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  increase dim
   else if (ch == '>')
      dim += 50;
   //  PageDown key - decrease dim
   else if (ch == '<')
      dim -= 50;
   //  Reset view angle
   else if (ch == '0')
      X = Y = th = ph = 0;
   //  Toggle axes
   else if (ch == 'b' || ch == 'B')
      axes = 1-axes;
   //  Toggle textures
   else if (ch == 't' || ch == 'T')
      ntex = 1-ntex;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Toggle infinity
   else if (ch == 'i' || ch == 'I')
      inf = 1-inf;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Increase/decrease spot azimuth
   else if (ch == '[')
      Ph -= 5;
   else if (ch == ']')
      Ph += 5;
   //  Increase/decrease spot cutoff angle
   else if (ch == '{' && sco>5)
      sco = sco==180 ? 90 : sco-5;
   else if (ch == '}' && sco<180)
      sco = sco<90 ? sco+5 : 180;
   //  Change constant attenuation
   else if (ch == '1' && at0>0)
      at0--;
   else if (ch == '!')
      at0++;
   //  Change linear attenuation
   else if (ch == '2' && at1>0)
      at1--;
   else if (ch == '@')
      at1++;
   //  Change quadratic attenuation
   else if (ch == '3' && at2>0)
      at2--;
   else if (ch == '#')
      at2++;
   //  Light position
   else if (ch=='x')
      X -= 50;
   else if (ch=='X')
      X += 50;
   else if (ch=='y')
      Y -= 50;
   else if (ch=='Y')
      Y += 50;
   else if (ch=='z')
      Z -= 50;
   else if (ch=='Z')
      Z += 50;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Wrap angles
   Th = Th%360;
   Ph = Ph%360;
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(800,800);
   glutCreateWindow("Vahid Mazdeh - hw3 - Egyptian Pyramids w Lighting + Texture");
   // background color
   glClearColor(0.95, 0.810, 0.4, 0.0);
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   LoadTexBMP("textures/pyramid.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
