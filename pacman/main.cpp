#include <allegro.h>
#include <cstdlib>
#define MAXFILAS 20 //pARA EL EJE X
#define MAXCOLS 31  //PARA EL EJE Y 
BITMAP *buffer;  //creamos  mapas de bits 
BITMAP *roca;
BITMAP *pacbmp;
BITMAP *pacman;
BITMAP *comida;
MIDI *musica1;
SAMPLE *bolita;
SAMPLE *caminando;
SAMPLE *muerte;

BITMAP *muertebmp;
int dir=4;
int px=30*14 ,py=30*17;
int fdir=0;

int anteriorpx,anteriorpy;


char mapa [MAXFILAS][MAXCOLS] = {
     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  //PAREDES SUPERIORES DE LA MATRIZ 
     "X|ooo| ooo |XXXXX| ooo |ooo|X",  //LA CUAL DIBUJARA EL MAPA 
     "X XXX XXXXX XXXXX XXXXX XXX X",
     "X XXXoXXXXX XXXXX XXXXXoXXX X",
     "X ooo   oooo ooo oooo   ooo X",
     "XoXXXoXX XXXXXXXXXXX XX XXXoX",
     "Xo   oXX     XXX     XXo   oX",
     "XoXXXoXXXXXX XXX XXXXXXoXXXoX",
     "XoXXXoXXo   | o |   oXXoXXXoX",
     " o   oXX XXXXXXXXXXX XXo   o ",
     "X XXX XX XXXXXXXXXXX XX XXX X",
     "X XXX XXo o | o | o oXX XXX X",
     "XoXXX XXXXXX XXX XXXXXX XXX X",
     "X| o |XX| o oXXXo o |XX| o |X",
     "XoXXX XX XXXXXXXXXXX XX XXX X",                     
     "X XXX|  o | oooooo | o |XXX X",
     "X XXX XXXX XXXXXXXX XXX XXX X",
     "X XXX XXXX          XXX XXX X",
     "Xo oo o  ooXXXXXXXXoo   oo oX",
     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",  
     };
     
void dibujar_mapa(){
     int row,col ;
     //CICLO QUE RECORRE LAS POSICIONES LA MATRIX 
     for (row =0 ; row < MAXFILAS ; row++){
         for(col =0 ;col < MAXCOLS ;col++){
             if(mapa[row][col]=='X'){
             draw_sprite(buffer, roca , col*30 , row*30 );// AJUSTA EL TAMAÑO DE LA IMAGEN EN CADA X 
             }//Esta funcion me posiciona la comida y me elimina la comida obtenida
             else if (mapa[row][col]=='o'){
                  draw_sprite(buffer, comida , col*30 , row*30 );
                  if (py/30 == row && px/30 ==col)
                     play_sample(bolita,300,150,1000,0);
                     mapa[row][col]=' ' ;
                  }
         }
         }
         }
void pantalla(){
     blit(buffer, screen , 0,0,0,0,880,600); //tamaño en pantalla
     }
void dibujar_personaje(){
     blit(pacbmp,pacman,dir*33,0,0,0,33,33);
     draw_sprite(buffer,pacman,px,py);
     }
bool terminar(){
     //Esta funcion me busca en cada una de las posiciones de la matriz
     //Buscando la comida
     //Cuando no encuentre mas se dejara de ejecutar
      int row,col ;
     
     for (row =0 ; row < MAXFILAS ; row++){
         for(col =0 ;col < MAXCOLS ;col++){
             if(mapa[row][col]=='o')return true;
             }
         }
     return false;
     }
class fantasma{//Esta clase facilita la creacion de los fantasmas
     BITMAP *enemigobmp;
     BITMAP *enemigo;
     int fdir;
     int _x,_y; 
public:
       fantasma(int x,int y );//constructor
       void dibujar_fantasma() const;
       void mover_fantasma();
       void choque_pacman();
       };
fantasma::fantasma(int x,int y ){
       _x = x;
       _y = y;
       fdir = rand()%4;
       enemigo=create_bitmap(30,30);
       enemigobmp=load_bitmap("enemigo.bmp",NULL);                
                       
}

void fantasma::dibujar_fantasma() const{
     
      blit(enemigobmp,enemigo,0,0,0,0,30,30);
     draw_sprite(buffer,enemigo,_x,_y);
     
     
     }
 void fantasma::choque_pacman(){
      //Me ejecuta los movimientos de el choque de el pacman
      //cuando mueren
      if (py == _y && px ==_x || _y==anteriorpy && _x ==anteriorpx){
      play_sample(muerte,100,150,1000,0);
        for(int j=0;j <=5 ;j++){
         clear(pacman);
         clear(buffer);
         dibujar_mapa();
         blit(muertebmp,pacman,j*33,0,0,0,33,33);
         draw_sprite(buffer,pacman,px,py);
         pantalla();
         rest(80);
         }
         }
     px = 30*14;
     py = 30*17;
     dir =4;
      }    
      //Basado en el patron strategy 
      //Se clasifica como patrón de 
      //comportamiento porque determina 
      //cómo se debe realizar el movimiento 
      //de los objetos fantasmas
      
void fantasma::mover_fantasma(){
    dibujar_fantasma();
     
     choque_pacman();
      if(mapa[_y/30][(_x-30)/30] != '|'){
      fdir=rand()%4;
     if(fdir == 0){
         if(mapa[_y/30][(_x-30)/30] != 'X')_x-=30;
         else fdir =rand()%4; //valor al azar para los movimientos del enemigo
     
     }
     if(fdir == 1){
         if(mapa[_y/30][(_x+30)/30] != 'X')_x+=30;
         else fdir =rand()%4;
         }
       if(fdir == 2){
         if(mapa[(_y-30)/30][_x/30] != 'X')_y-=30;
         else fdir =rand()%4;
         }  
       if(fdir == 3){
         if(mapa[(_y+30)/30][_x/30] != 'X')_y+=30;
         else fdir =rand()%4;
         } 
        if(_x <= -30) _x = 870;
           else if( _x>=870)_x= -30;  
         } 
    }
int main() {
	allegro_init();
	install_keyboard();
		set_color_depth(32);
        set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 600, 0, 0);
        //Abro la libreria de sonidos 
   if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return 1;
    }
set_volume(70, 70);// Musica de fondo durante la ejecucion de el programa
//Cargamos las imagenes en formato BMP
musica1=load_midi("mario.mid");
bolita=load_wav("coin.wav");
caminando=load_wav("jump.wav");
muerte=load_wav("Muerte.wav");
         buffer = create_bitmap(880, 600);
         roca= load_bitmap("roca.bmp",NULL); //cargamos la imagen guardada que sera reemplazada por las X 
         pacbmp=load_bitmap("pacman.bmp",NULL);
         pacman=create_bitmap(33,33);
         comida=load_bitmap("Comida.bmp",NULL);
        
         muertebmp=load_bitmap("muerte.bmp",NULL);
         //Me crea los fantasmas con las posiciones 
         //que ocuparan en el mapa
fantasma A(30*2,30*3);
fantasma B(30*10,30*13);
fantasma C(30*4,30*2);
fantasma D(30*1,30*16);
fantasma E(30*11,30*14);
fantasma F(30*5,30*11);
fantasma G(30*6,30*1);
fantasma H(30*14,30*8);

play_midi(musica1,1);
  while (!key[KEY_ESC]&&terminar()){
        if(dir!= 4) play_sample(caminando,100,150,1000,0);
        anteriorpx =px;
        anteriorpy=py;                      
        if(key[KEY_RIGHT])dir =1; //DIRECCIONES Y MOVIMIENTOS DE PACMAN
        else if(key[KEY_LEFT])dir =0;
        else if(key[KEY_UP])dir =2;
        else if(key[KEY_DOWN])dir =3;
        //Movimientos pacman
        //condicion if utilizada para delimitar los muros
        //Evaluando las X que se encuentran en el mapa 
        if (dir == 0){
           if(mapa[py/30][(px-30)/30] != 'X')
                 px -=30;
           else dir=4;
                 }
        if (dir == 1){
            if(mapa[py/30][(px+30)/30] != 'X' )
                 px +=30;
        else dir = 4;
        }
        
        if (dir == 2){
           if(mapa[(py-30)/30][(px)/30] != 'X')
                 py -=30;
           else dir=4;
                 }
        if (dir == 3){
                if(mapa[(py+30)/30][(px+30)/30] != 'X')
                 py +=30;
            else dir=4;     
                 }
        //Atajos
        if(px <= -30) px = 870;
           else if( px>=870)px= -30;
                 
        clear(buffer); //ESt funcion me elimina las posiciones anteriormente recorridas
        //Me agrega la cantidad de fantasmas necesarios
        //con la clase implementada anteriormente 
         dibujar_mapa();  
         dibujar_personaje();
         A.mover_fantasma ();
         B.mover_fantasma ();
         C.mover_fantasma ();
         D.mover_fantasma ();    
         E.mover_fantasma (); 
         F.mover_fantasma (); 
         G.mover_fantasma (); 
         H.mover_fantasma ();        
         pantalla(); 
         rest(70); //Me demora el proceso de ejecucion del movimiento
      clear(pacman);
      blit(pacbmp,pacman,4*33,0,0,0,33,33);
     draw_sprite(buffer,pacman,px,py);
     pantalla();
     rest(90);
}}
END_OF_MAIN();

