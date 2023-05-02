/***************************************************************************
 *   Copyright (C) 2009 by Al Sabawi   *
 *   sabawi@sabawi1   *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/
#ifndef _SDL_UTILS_H 
#define _SDL_UTILS_H    1

double GetTime(void);
SDL_Cursor *init_system_cursor(const char *image[]);
		
bool init_SDL(void);
void SDL_clean_up();
bool init_mySDL(void);
bool set_background(std::string);
bool load_background(void);
bool load_sound(void);

bool Load_bg_music(std::string );
bool play_bg_music(void);
void stop_bg_music(void);

SDL_Surface * load_image( std::string  );
void apply_surface( int, int, SDL_Surface* , SDL_Surface*);
bool load_files(std::string ,SDL_Surface **  );

void ConvertSphereGeomToScreen(dReal , dReal , int *, int * , int );
bool is_close(int ,int ,int ,int ,int );
dGeomID DrawToClosest(dGeomID , int );
dGeomID DrawToClosestExcluding(dGeomID , int , dGeomID );
void DrawTo2Closest(dGeomID , int , dGeomID &o1, dGeomID &o2);
dGeomID DrawToClosestAny(dGeomID mygeom,int nDrawto,int WithInDistance,dGeomID (&CloseGeom)[5] );
void TraceString (char * , ...);
int m2sx(int mx);
int m2sy(int my);
double s2ox(int sx);
double s2oy(int sy);
int o2sx(double ox);
int o2sy(double oy);
void RotateLine(double sx1, double sy1,double ex1, double ey1, double theta);
void RotateLine2 (double center[2], double (&line)[4],double , Uint32);
void MatMult (const dReal * pos, const dReal * R,  dReal (&res)[4]);
void RotateBox (const dReal * center, double w, double h, const dReal * R, double (&res)[4][4]);
//void sDrawLine(int x1, int y1, int x2, int y2, Uint32  );
void sDrawLine2(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
bool IsWithinDistance(const dReal *, const dReal *, double );
bool is_file_extension(const char *, const char * );
void rotate_body_x(dBodyID body, double phi);
void get_euler(const dReal * matrix,dReal &kx,dReal &ky,dReal &kz);
void GetRelDirectionToMouse(int x, int y,int *rel_x,int *rel_y);
void drawText(char* string,
              int size,
              int x, int y,
              int fR, int fG, int fB,
              int bR, int bG, int bB);
void drawText2(char* string,int size,int x, int y,Uint32 Color);
void CheckAllJointsForces(bool bDisplay, double Fkill, double Tkill);
void UpdateBodyJointForces(PTBlist pB, int joint);

void reset_YZaccess_drift(dBodyID body);
bool IntersectLines(	double , double ,
						double , double ,
						double , double ,
						double , double ,
						double *, double *) ;
char * GetTimeDiffString(double t1, double t2, char (&str)[100]);
char * GetTimeDiffString2(double t1, double t2, char (&str)[100]);
bool show_overscreen(SDL_Surface* screen, double dTime);
void close_audio(void);
bool init_audio(void);
double GetAngleOfLine(double fX1, double fY1, double fX2, double fY2);
void DrawImageFromLinePoints(SDL_Surface *image, int x1, int y1, int x2, int y2);
#endif // _DSL_UTILS_H
