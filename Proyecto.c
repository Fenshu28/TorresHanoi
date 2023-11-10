#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <allegro.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "Pila.h"
#include "MyFunc.h"

#define MaxDisc 8
#define Errx 20
#define Erry 60
#define Alto 600
#define Ancho 800

// Kivy aplicaciones moviles en python

static void activate_level();
void Make_Menu();

// Variables tipo Pila
Pila Torre1,Torre2,Torre3,*Torres[3] = {&Torre1,&Torre2,&Torre3};

//Definicion de la estructura datos
typedef struct{
	char nombre[5];
	int codigo;
	float tiempo;
	float puntaje;
	int nivel;
	int estrellas;
	char Pasw[10];
}Usuario;

// Widgets de la interface y contenedores.
GtkWidget *fixdGame,*fixdMenu,*windowMenu,*FrmDialog, *dlg, *btnTorre1,*btnTorre2,*btnTorre3,
*lbCrono,*picture,*boxOpc,*LayoutOpc,*boxContinuar,*boxScore,*boxHistoria,*boxCreditos,
*txtNombre,*txtPasw,*btnVisible,*ErrDialog;

// Variable de imagen
GdkPixbuf *pixbuf;

// Medidas de referencia botones menú 
int AnBtn=120,AlBtn=50,xBtn,yBtn=175,yInc;

// Variables de archivos
char Bckgrnd[20][30] = {
	{"img/menu.png"},// 0
	{"img/nivel1.png"},// 1
	{"img/nivel2.png"},// 2
	{"img/nivel3.png"},// 3
	{"img/nivel4.png"},// 4
	{"img/nivel5.png"},// 5
	{"img/nivel6.png"},// 6
	{"img/creditos.png"},// 7
	{"img/historia.png"},// 8
	{"img/Blur.png"},// 9
	{"img/menu/pause.png"},// 10
	{"img/menu/usuarioyC.png"},// 11
	{"img/menu/continuar.png"},// 12
	{"img/menu/puntajes.png"},// 13
	{"img/menu/pergamino.png"},// 14
	{"img/menu/contraseña.png"}// 15
};
char StarsImg[5][30] = {
	{"img/menu/full-0.png"},// 0
	{"img/menu/full-1.png"},// 1
	{"img/menu/full-2.png"},// 2
	{"img/menu/full-3.png"} // 3
};
char ImgBtns[15][30] = {
	{"img/menu/opciones.png"},
	{"img/menu/aceptar.png"},
	{"img/menu/regresar.png"},
	{"img/menu/1st.png"},
	{"img/menu/2nd.png"},
	{"img/menu/3rd.png"},
	{"img/menu/btnpause.png"},
	{"img/menu/movimientos.png"}
};

char ImgDsc[15][30] = {
	{"img/Base.png"},
	{"dscbrk/1.png"}
};

char ImgDsc2[15][30] = {
	
};

char BkndMusic[15][30] = {
	{"./audio/Music/Credits.wav"},
	{"./audio/Music/1.wav"},
	{"./audio/Music/2.wav"},
	{"./audio/Music/3.wav"},
	{"./audio/Music/4.wav"},
	{"./audio/Music/5.wav"},
	{"./audio/Music/6.wav"},
	{"./audio/Music/7.wav"},
	{"./audio/Music/End.wav"}
};

char SFX[15][30]={
	{"./audio/SFX/Push.wav"},
	{"./audio/SFX/Pop.wav"},
	{"./audio/SFX/Click.wav"},
	{"./audio/SFX/Explosión1.wav"},
	{"./audio/SFX/Explosión2.wav"},
	{"./audio/SFX/ExplosiónBola.wav"}
};

// Posición base de las torres en el fixed
int ATrr = 300, ABtn = 50,LBtn = 195,Torre1x = 79,Torre2x = 295,Torre3x = 517,Torrey = 500,Dsm=20;

// Variables para almacenar valores x,y del puntero en la pantalla
int offsetx, offsety, px, py, maxx, maxy; 

// Variables contadoras y centinelas.
float Tiempo=0;
int Movimientos=0, Intentos = 0, Play, Disc = 3, level,hilo;
int levels[7] = {0,3,4,5,6,7,8};

// Variables de Datos.
Usuario UserAct;
float Score,ScoreFinal;
int Stars;

// Variables de hilos
pthread_t h1;

// Variable musica de fondo;
SAMPLE *sonido;

// Variable para el css

// Parametros para el ratón
const gint Sensitivity = 1, EvMask = GDK_BUTTON_PRESS_MASK | GDK_BUTTON1_MOTION_MASK;

// Strings para almacenar datos
GString *NumDsc, *NumBoton,*NumTorre,*TorrDest,*Minutes,*txtDatos;

inline static int Min(const int a, const int b) { return b < a ? b : a; }
inline static int Max(const int a, const int b) { return b > a ? b : a; }
inline static int RoundDownToMultiple(const int i, const int m){ return i/m*m; }
inline static int RoundToNearestMultiple(const int i, const int m)
{
	if (i % m > (double)m / 2.0d)
		return (i/m+1)*m;
	return i/m*m;
}

 
 //GtkWidget *widget, gpointer   data 
static void destroy( ) { allegro_exit(); gtk_main_quit (); exit(1); destroy_sample(sonido); }

void *Cronometro(void *arg){
	//const gchar *T;
	float T;
	while(hilo){
		if(Play){
			sleep(1);
			Tiempo ++;
			T = Tiempo / 60.0;
			
			gtk_label_set_text(GTK_LABEL(lbCrono),g_strdup_printf("\t%d \n\n\n%.2f min",Movimientos,T));
			//g_print("Minutos: %.2f, Movimientos: %d\n",T,Movimientos);
		}
	}
}


GtkWidget *PushBackground(int n,int w, int h){
	/* Cargamos la imagen */
	pixbuf = gdk_pixbuf_new_from_file_at_scale(Bckgrnd[n], /* nombre de fichero */
							 w, /* ancho de la pantalla */
							 h, /* alto de la pantalla */
							 FALSE, /* No respetar aspecto */
							 NULL);
	/* Guardamos la imagen en un widget */
	picture = gtk_image_new_from_pixbuf(pixbuf);
	//gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(picture));
	return picture;
}


GtkWidget *BtnImg(int w,int h,char *Img,gchar *btnName,gchar *txt,GCallback funci){
	GtkWidget *fixdBtn = gtk_fixed_new(),
	*btnAux = gtk_button_new(),
	*lbTexto = gtk_label_new(txt);
	
	gtk_widget_set_opacity(btnAux,0);
	gtk_widget_set_size_request(btnAux,w,h);
	gtk_widget_set_size_request(fixdBtn,w,h);
	
	gtk_widget_set_name(btnAux,btnName);
	
	g_signal_connect(btnAux,"clicked",funci,NULL);
	
	gtk_fixed_put(GTK_FIXED(fixdBtn),PushImg(w,h,Img),0,0);
	gtk_fixed_put(GTK_FIXED(fixdBtn),btnAux,0,0);
	gtk_fixed_put(GTK_FIXED(fixdBtn),lbTexto,20,(h/2)-(10));
	
	return fixdBtn;
}

void OnClose(){
	FrmDialog = gtk_message_dialog_new(GTK_WINDOW(windowMenu),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_YES_NO,"¿Estas seguro que quieres salir?");
	int resp = gtk_dialog_run(GTK_DIALOG(FrmDialog));
	g_print("resp: %d\n",resp);
	gtk_widget_destroy(FrmDialog);	
}

void widg_set_trans(GtkWidget *w){
	GdkScreen *screen;
	GdkVisual *visual;
	gtk_widget_set_app_paintable(w, TRUE);
	screen = gdk_screen_get_default();
	visual = gdk_screen_get_rgba_visual(screen);
	if (visual != NULL && gdk_screen_is_composited(screen)) {
		gtk_widget_set_visual(w, visual);
	}
}

int Rep_SFX(char *archivo,int loop,int Vol){
	SAMPLE *sfx;
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0){
		allegro_message("Error inicializando el sistema de sonido.\n%s\n",allegro_error);
		return 1;
	}
	sfx = load_sample(archivo);
	play_sample(sfx,Vol,0,1000,loop);
}

int Rep_Musica(char *archivo,int loop,int Vol){
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) != 0){
		allegro_message("Error inicializando el sistema de sonido.\n%s\n",allegro_error);
		return 1;
	}
	sonido = load_sample(archivo);
	play_sample(sonido,Vol,0,1000,loop);
}

void ToUpperStr(char *cad,int lon){
	for(int i = 0;i<lon;i++){
		cad[i] = toupper(cad[i]);
	}
	cad[lon-1] = '\0';
}

void Guardar(Usuario Us){
	FILE *archivo;
	archivo = fopen("datos.dat","ab");
	if (archivo == NULL)
		exit(1);
	Usuario user = Us;
	//~ strcpy(user.nombre,name);
	//~ user.codigo = cod;
	//~ user.tiempo = tiem;
	//~ user.puntaje = punt;
	//~ user.nivel = niv;
	//~ user.estrellas = est;
	
	fwrite(&user, sizeof(Usuario),1,archivo);
	fclose(archivo);
}

int listaUser(Usuario *listUsers){
	FILE *archivo;
	int i=0;
	archivo = fopen("datos.dat","rb");
	
	if(archivo == NULL)
		return -9999;

	do{
		fread(&listUsers[i], sizeof(Usuario), 1, archivo);
		i++;
	}while(!feof(archivo));
	
	fclose(archivo);
}

//Funcion para consultar un solo usuario dependiendo de su codigo
int consulta(Usuario *usr,int cod){
	FILE *archivo;
	Usuario inArch;
	archivo =fopen("datos.dat","rb");
	if(archivo == NULL)
		return -9999;
		//exit(1);
	
	int existe =0;
	
	do{
		fread(&inArch,sizeof(Usuario), 1,archivo);
		if(cod == inArch.codigo){
			g_print("\t%i  \t%s  \t%i  \t%.2f  \t%0.2f  \t%i\n",inArch.codigo,inArch.nombre,inArch.nivel,inArch.tiempo,inArch.puntaje,inArch.estrellas);
			*usr = inArch;
			existe=1;
			break;
		}
	}while(!feof(archivo));
	
	if(existe==0)
		g_print("No existe un usuario con ese codigo\n");
	
	fclose(archivo);
	
	return existe;
}

//funcion que actualiza los datos del usuario
void actualizar(int cod, float tiem, float punt, int niv, int est){
	FILE *archivo;
	archivo=fopen("datos.dat","r+b");
	if(archivo==NULL)
		exit(1);

	Usuario usuarios;
	int existe=0;
	
	fread(&usuarios, sizeof(Usuario), 1,archivo);
	while(!feof(archivo))
	{
		if(cod==usuarios.codigo)
		{
			usuarios.nivel = niv;
			usuarios.tiempo = tiem;
			usuarios.puntaje = punt;
			usuarios.estrellas = est;
			
			int pos=ftell(archivo)-sizeof(Usuario);
			fseek(archivo,pos,SEEK_SET);
			
			fwrite(&usuarios, sizeof(Usuario), 1,archivo);
			g_print("Se modifico los datos del usuario.\n");
			existe=1;
			break;
		}
		fread(&usuarios, sizeof(Usuario), 1,archivo);
	}
	
	if (existe==0)
		g_print("No existe un usuario con ese codigo.\n");
	fclose(archivo);
	//continuar();
}

void VisiblePasw(){
	if(gtk_entry_get_visibility(GTK_ENTRY(txtPasw))){
		gtk_entry_set_visibility(GTK_ENTRY(txtPasw),FALSE);
	}else{
		gtk_entry_set_visibility(GTK_ENTRY(txtPasw),TRUE);
	}
}

// Volver al menú
void BackMenu(){
	Rep_SFX(SFX[2],0,200);
	
	//~ if( sonido){
		//~ destroy_sample(sonido);
	//~ }	
	
	gtk_widget_destroy(fixdMenu);
	Make_Menu();
}

void SetLevel(int lev){
	level = lev;
	hilo = 1;
	Disc = levels[level];// Asigna los discos dependiendo del nivel.
	pthread_create(&h1,NULL,Cronometro,NULL);// Crea el hilo del juego.
	
	//~ if(sonido != NULL){
		//~ destroy_sample(sonido); // Elimina la musica actual
	//~ }
	
	Rep_Musica(BkndMusic[level],1,50);// Reproduce el sonido de fondo.
}

void GoLevel(GtkWidget *w){
	//~ VisiblePasw();
	GString *psw = g_string_new(gtk_entry_get_text(GTK_ENTRY(txtPasw)));
	Usuario auxus;
	consulta(&auxus,UserAct.codigo);
	
	if(!strcmp(psw->str, auxus.Pasw)){
		UserAct = auxus;
		SetLevel(UserAct.nivel);
		g_print("Iniciando nivel de : %s, en nivel: %d\n",UserAct.nombre,UserAct.nivel);
		activate_level();		
	}else {
		MsgBox(GTK_WINDOW(windowMenu),"Contraseña no valida.");
	}
}

void LayoutPasw(GtkWidget *w){
	GtkWidget *btnAsek = gtk_button_new_with_label("Asekto");
	txtPasw = gtk_entry_new();
	btnVisible = BtnImg(30,30,ImgBtns[2],"","",G_CALLBACK(VisiblePasw));
	
	//destroy_sample(sonido);
	Rep_SFX(SFX[2],0,200); // Sonido de click
	
	// Reinicia el box que simula una ventana
	gtk_widget_destroy (LayoutOpc);
	LayoutOpc = gtk_fixed_new();
	
	// Colocamos el fondo
	gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
	gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(400,300,Bckgrnd[15]),(Ancho/2)-(400/2),(Alto/2)-(300/2));
	
	// Propiedades de la caja de texto para contraseña.
	gtk_style_context_add_class(gtk_widget_get_style_context(txtPasw),"entryname");
	gtk_entry_set_width_chars(GTK_ENTRY(txtPasw),15);
	gtk_entry_set_visibility(GTK_ENTRY(txtPasw),FALSE);
	gtk_entry_set_text(GTK_ENTRY(txtPasw),"2109");
	
	gtk_widget_set_name(btnAsek,"Nuevo");
	gtk_widget_set_opacity(btnAsek,0);
	g_signal_connect(btnAsek,"clicked",G_CALLBACK(GoLevel),NULL);
	
	// Colocamor el textbox para la contraseña y botones.
	gtk_fixed_put(GTK_FIXED(LayoutOpc),BtnImg(AnBtn,AlBtn,ImgBtns[2],"","",G_CALLBACK(BackMenu)),10,10);
	gtk_fixed_put(GTK_FIXED(LayoutOpc),btnVisible,530,284);
	gtk_fixed_put(GTK_FIXED(fixdMenu),LayoutOpc,0,0);
	gtk_fixed_put(GTK_FIXED(LayoutOpc),txtPasw,(Ancho/2)-30,284);
	gtk_fixed_put(GTK_FIXED(LayoutOpc),btnAsek,(Ancho/2)-40,325);
	
	// Mostrar todos los componentes
    gtk_widget_show_all(fixdMenu);
    
    UserAct.codigo = atoi(gtk_widget_get_name(w));
    
    //~ destroy_sample(sonido);
}

void SaveGame( GtkWidget *w){
	Rep_SFX(SFX[2],0,200); // Sonido de click
	int i;
	char Op[10]={}, AuxName[5]={},AuxPasw[10]={};
	
	if(!strcmp(gtk_widget_get_name(w),"Nuevo")){
		//~ destroy_sample(sonido);
		strcpy(AuxName, gtk_entry_get_text(GTK_ENTRY(txtNombre)));
		strcpy(AuxPasw, gtk_entry_get_text(GTK_ENTRY(txtPasw)));
		strcpy(txtDatos->str, gtk_entry_get_text(GTK_ENTRY(txtNombre)));
		
		if(strcmp(AuxName,"") && strcmp(AuxPasw,"")){
			Usuario AuxUs;
			for(i = 0;i<200;i++){
				g_print("i: %d\n",i);
				int auxcons = consulta(&AuxUs,i);
				if(auxcons==0){
					g_print("Consultó\n");
					//i ++;
					break;
				}else if (auxcons == -9999){
					i = 0;
					break;
				}
			}
			g_print("i: %d\n",i);
			ToUpperStr(AuxName,5);
			
			//~ char *name,int cod, float tiem, float punt, int niv, int est
			strcpy(AuxUs.nombre,AuxName);
			strcpy(AuxUs.Pasw,AuxPasw);
			AuxUs.codigo = i;
			AuxUs.estrellas = 0;
			AuxUs.nivel = 1;
			AuxUs.puntaje = 0;
			AuxUs.tiempo = 0.0;
			
			Guardar(AuxUs);
			
			UserAct.codigo = i;
			g_print("Nueva partida guardada\n");
			SetLevel(1);
			activate_level();
		}else{
			MsgBox(GTK_WINDOW(windowMenu),"Ingresa un nombre :)");
		}		
	}else{
		destroy_sample(sonido);
		actualizar(UserAct.codigo,Tiempo/60,ScoreFinal,level+1,Stars);
		g_print("Score actualizado\n");
		SetLevel(level + 1);
		activate_level();
		
	}
}

void DestroyGame(){
	if(fixdGame != NULL){
		//destroy_sample(sonido);
		gtk_widget_destroy(dlg);
		gtk_widget_destroy(fixdGame);
		g_print("Eliminado\n");
	}
}

int Winner(){
	if(NumElem(&Torre3) == Disc){
		g_print("Ultima llena.\n");
		int Min = pow(2,Disc)-1,aux;
		
		if(Movimientos == (Min)){
			Stars = 3;
			aux = 1;
		}else{
			Stars = ((Movimientos > Min) && (Movimientos < Min+3) ? 2 :( (Movimientos > Min) && (Movimientos < Min+6) ? 1 : 0));
			aux = 0;
		}
		
		Score = (Movimientos / (Tiempo/60)) * (Stars == 0 ? 0.5 : Stars);
		ScoreFinal += Score;
		
		return aux;
	}else{
		return -9999;
	}
}

void QuitPause(){
	gtk_widget_destroy(dlg);
}

void GoMenu(GtkWidget *w){
	destroy_sample(sonido);
	if(!strcmp(gtk_widget_get_name(w),"MenuFull")){
		actualizar(UserAct.codigo,Tiempo/60,ScoreFinal,level+1,Stars);
		g_print("Score actualizado\n");
	}
	
	hilo = Play = ScoreFinal = 0 ;
	DestroyGame();
	Make_Menu();
}

void NextGame(GtkWidget *w){
	DestroyGame();
	hilo = Play = 0;
	SaveGame(w);
}

void Repeat(){
	activate_level();
}

void SetLayoutDialog(int tipo,GtkWidget *cont){
	int anbtn = 120, albtn = 40,xb = ((400/2)-(anbtn/2)),yb = 95,dist = 10;

	if(tipo == 1){ // Pause
		// Colocamos el fondo
		gtk_fixed_put(GTK_FIXED(cont), PushBackground(10,400,300),0,0);
		for(int i = 0; i<3;i++){
			if(i == 0){
				gtk_fixed_put(GTK_FIXED(cont),BtnImg(anbtn,albtn,"","Reiniciar","",G_CALLBACK(Repeat)),xb,yb);
			}else if(i == 1){
				gtk_fixed_put(GTK_FIXED(cont),BtnImg(anbtn,albtn,"","Reanudar","",G_CALLBACK(QuitPause)),xb,yb + (i*(albtn+dist)));
			}else if(i == 2){
				gtk_fixed_put(GTK_FIXED(cont),BtnImg(anbtn,albtn,"","MenuPause","",G_CALLBACK(GoMenu)),xb,yb + (i*(albtn+dist)));
			}
		}
		
	}else if(tipo == 2){ // Nivel full
		GString *aux1 = g_string_new("");
		GtkWidget *lbScr,*lbTim;
		// Colocamos el fondo
		gtk_fixed_put(GTK_FIXED(cont), PushImg(400,300,StarsImg[Stars]),0,0);
		// Colocamos los botones.
		gtk_fixed_put(GTK_FIXED(cont),BtnImg(40,40,"","MenuFull","",G_CALLBACK(GoMenu)),((400/2)-(40/2)),200);
		gtk_fixed_put(GTK_FIXED(cont),BtnImg(40,40,"","Reiniciar","",G_CALLBACK(Repeat)),((400/2)-(40/2)) - 70,205);
		gtk_fixed_put(GTK_FIXED(cont),BtnImg(40,40,"","Next","",G_CALLBACK(NextGame)),((400/2)-(40/2)) + 70 ,205);
		
		sprintf(aux1->str,"%0.2f",Score);
		lbScr = gtk_label_new(aux1->str);
		gtk_style_context_add_class(gtk_widget_get_style_context(lbScr),"whiteback");
		gtk_fixed_put(GTK_FIXED(cont),lbScr,70,115);
		strcpy(aux1->str,"");
		
		sprintf(aux1->str,"%0.2f m",Tiempo/60.0);
		lbTim = gtk_label_new(aux1->str);
		gtk_style_context_add_class(gtk_widget_get_style_context(lbTim),"whiteback");
		gtk_fixed_put(GTK_FIXED(cont),lbTim,70,170);
		
		g_print("Mov = %d, Tiempo = %f, Score = %0.2f, Stars = %d",Movimientos,Tiempo/60,Score,Stars);	
	}
}

// Funcón de cuadro de dialogo (pausa y ganar)
void WinDialog(int tipo){
	dlg = gtk_dialog_new();
	GtkWidget *fxdDlg = gtk_fixed_new(),
	*ContentArea = gtk_dialog_get_content_area(GTK_DIALOG(dlg));
	
	// Propiedades del dialog
	gtk_window_set_decorated(GTK_WINDOW(dlg),FALSE);
	gtk_window_set_default_size(GTK_WINDOW(dlg),400,300);
	gtk_window_set_position(GTK_WINDOW(dlg),GTK_WIN_POS_CENTER_ON_PARENT);
	widg_set_trans(dlg);
	
	// Añadimo componentes al fixed segun el tipo de ventana
	SetLayoutDialog(tipo,fxdDlg);
	
	// Agregamos el fixed al dialog
	gtk_container_add(GTK_CONTAINER(ContentArea),fxdDlg);
	
	gtk_widget_show_all(dlg);
	gtk_dialog_run(GTK_DIALOG(dlg));
	gtk_widget_destroy(dlg);
}

// Función que encuentra la direccón de memoria de una torre.
int findTorre(Pila *torr){
	for(int i = 0; i<3; i++){
		if(torr == Torres[i]){
			return i;
		}
	}
}

// Función que obtiene la torre destino.
int GetTorrDest(){
	//TorrDest = g_string_new(gtk_widget_get_name(widget));
	//g_print("TorrDest: %s\n",TorrDest->str);
	
	if((px >= Torre1x) && (px <= (Torre1x + LBtn)) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
		return 0;
	}else if((px >= Torre2x) && (px <= (Torre2x + LBtn )) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
		return 1;
	}else if((px >= Torre3x) && (px <= (Torre3x + LBtn)) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
		return 2;
	}else {
		return -9999;
	}
}

// Función que cambia la imagen del disco desde un archivo
GtkWidget *ChangeImgDisc(char *Imagen,int w, int h){
	GdkPixbuf *pixb = gdk_pixbuf_new_from_file_at_scale(Imagen,w,h,FALSE,NULL);
	
	return gtk_image_new_from_pixbuf(pixb);
}

// Función de preseionar el Disco.
static gboolean button_press_event( GtkWidget *w, GdkEventButton *event )
{
	NumBoton = g_string_new(gtk_widget_get_name(w));
	NumBoton->str[NumBoton->len -1] = '\0';
	NumTorre = g_string_new(gtk_widget_get_name(w));
	NumTorre->str[0] = '0';
	
	if (event->button == 1 && (atoi(NumBoton->str) == Cima(Torres[atoi(NumTorre->str)]))) {
		GtkWidget* p = gtk_widget_get_parent(w);
		char fileimag[40] = {};
		strcat(fileimag,"./img/dscfull/");
		strcat(fileimag,NumBoton->str);
		strcat(fileimag,"f.png");
		gtk_button_set_image(GTK_BUTTON(w),ChangeImgDisc(fileimag,LBtn,ABtn));
		
		// Reproduce efecto pop
		Rep_SFX(SFX[1],0,255);
		// offset == distance of parent widget from edge of screen ...
		gdk_window_get_position(gtk_widget_get_window(p), &offsetx, &offsety);
		// plus distance from pointer to edge of widget
		offsetx += (int)event->x;
		offsety += (int)event->y;
		// maxx, maxy both relative to the parent
		// note that we're rounding down now so that these max values don't get
		// rounded upward later and push the widget off the edge of its parent.
		maxx = RoundDownToMultiple(gtk_widget_get_allocated_width(p) - gtk_widget_get_allocated_width(w), Sensitivity);
		maxy = RoundDownToMultiple(gtk_widget_get_allocated_height(p) - gtk_widget_get_allocated_height(w), Sensitivity);
	}
	return TRUE;
}

// Fución que comprueba que estamos arrastrando el disco
static gboolean motion_notify_event( GtkWidget *widget, GdkEventMotion *event )
{
	NumBoton = g_string_new(gtk_widget_get_name(widget));
	NumBoton->str[NumBoton->len -1] = '\0';
	NumTorre = g_string_new(gtk_widget_get_name(widget));
	NumTorre->str[0] = '0';
	
	if(atoi(NumBoton->str) == Cima(Torres[atoi(NumTorre->str)])){
		int x = (int)(event->x_root - (offsetx));
		int y = (int)(event->y_root - (offsety));
		x = RoundToNearestMultiple(Max(Min(x, maxx), 0), Sensitivity);
		y = RoundToNearestMultiple(Max(Min(y, maxy), 0), Sensitivity);
		if (x != px || y != py) {
			px = x;
			py = y;
			
			gtk_fixed_move(GTK_FIXED(fixdGame), widget, x, y);
			//g_print("x: %d, y: %d\n",px,py);
		}
	}
	return TRUE;
}

void closeErr(GtkWidget *p){
	//~ gtk_widget_destroy(gtk_widget_get_parent(p));
	gtk_widget_destroy(ErrDialog);
}

void MakeWinErr(){
	int s_h = 600,s_w = 800;
	GtkWidget *boxdlg,*btnAcp;
			
	// Creamos el cuadro de dialogo de export.
	ErrDialog = gtk_dialog_new();
	//~ gtk_window_set_title(GTK_WINDOW(ExportDialog),".");
	gtk_window_set_default_size(GTK_WINDOW(ErrDialog),s_w,s_h);
	gtk_window_set_resizable(GTK_WINDOW(ErrDialog),FALSE);
	gtk_window_set_decorated(GTK_WINDOW(ErrDialog),FALSE);
	gtk_window_set_modal(GTK_WINDOW(ErrDialog),TRUE);
	
	btnAcp = gtk_button_new();
	gtk_widget_set_name(ErrDialog,"ErrWin");
	g_signal_connect(btnAcp,"clicked",G_CALLBACK(closeErr),NULL);
	gtk_widget_set_opacity(btnAcp,0);
	
	// Obtenemos el box del dialog
	boxdlg = gtk_dialog_get_content_area(GTK_DIALOG(ErrDialog));
	
	// Agregamos el fixed al box
	gtk_box_pack_end(GTK_BOX(boxdlg),btnAcp,TRUE,TRUE,0);
	
	// Mostramos todo
	gtk_widget_show_all(boxdlg);
    
    // lo abrimos
    gtk_dialog_run(GTK_DIALOG(ErrDialog));
    
}

// Funcón que mueve un disco de una torre origen a una torre destino
void MovDisc(GtkWidget *widget){
	// Extrae el número del botón y el número de torre origen, almacenado en el nombre de widget disco
	NumBoton = g_string_new(gtk_widget_get_name(widget));
	NumBoton->str[NumBoton->len -1] = '\0';
	NumTorre = g_string_new(gtk_widget_get_name(widget));
	NumTorre->str[0] = '0';
	
	if(atoi(NumBoton->str) == Cima(Torres[atoi(NumTorre->str)]) ){ // evalua si el disco tomado es el que esta en la cima para poder moverlo.
		int Td = GetTorrDest() != -9999  ? Cima(Torres[GetTorrDest()]) : 0;
		int Ev = GetTorrDest() != -9999  ? EsVacia(Torres[GetTorrDest()]) : 1;
		
		//g_print("Cima Td: %d, Cima To: %d, Td Ev: %d\n",Td,Cima(Torres[atoi(NumTorre->str)]),Ev);
		
		if((Td < Cima(Torres[atoi(NumTorre->str)])) && (!Ev) && (!EsVacia(Torres[atoi(NumTorre->str)])) ){ // Evalua si el disco del origen es masgrande que el disco de la torre destino, tambien si la torre destino y la torre origen no esta vacía
			//~ MsgBox(GTK_WINDOW(windowMenu),"Movimiento invalido.");
			MakeWinErr();
			gtk_fixed_move(GTK_FIXED(fixdGame), widget,(atoi(NumTorre->str) == 0 ? Torre1x : (atoi(NumTorre->str) == 1 ? Torre2x : Torre3x))+ 5, Torrey - NumElem(Torres[atoi(NumTorre->str)])*(ABtn-Dsm));
			Push(Pop(Torres[atoi(NumTorre->str)]),Torres[atoi(NumTorre->str)]);
		}else{
			// Evalua el lugar donde estamos soltando el disco y si esta en el rango de alguna torre lo suelta ahí
			if((px >= Torre1x) && (px <= (Torre1x + LBtn)) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
				Push(Pop(Torres[atoi(NumTorre->str)]),&Torre1);
				Movimientos ++;
				gtk_fixed_move(GTK_FIXED(fixdGame), widget, Torre1x+ 5, Torrey - NumElem(&Torre1)*(ABtn-Dsm)); // (NumElem(&Torre1) * ABtn)
				
				NumDsc = g_string_new("");
				g_string_append_printf(NumDsc,"%d%d",atoi(NumBoton->str),findTorre(&Torre1));
				gtk_widget_set_name(widget,NumDsc->str);
				
			}else if((px >= Torre2x) && (px <= (Torre2x + LBtn)) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
				Push(Pop(Torres[atoi(NumTorre->str)]),&Torre2);
				Movimientos ++;
				gtk_fixed_move(GTK_FIXED(fixdGame), widget, Torre2x+ 5, Torrey - NumElem(&Torre2)*(ABtn-Dsm));
				
				NumDsc = g_string_new("");
				g_string_append_printf(NumDsc,"%d%d",atoi(NumBoton->str),findTorre(&Torre2));
				gtk_widget_set_name(widget,NumDsc->str);
			}else if((px >= Torre3x) && (px <= (Torre3x + LBtn)) && (py <= Torrey) && (py >= (Torrey - ATrr)) ){
				Push(Pop(Torres[atoi(NumTorre->str)]),&Torre3);
				Movimientos ++;
				gtk_fixed_move(GTK_FIXED(fixdGame), widget, Torre3x+ 5, Torrey - NumElem(&Torre3)*(ABtn-Dsm));
				
				NumDsc = g_string_new("");
				g_string_append_printf(NumDsc,"%d%d",atoi(NumBoton->str),findTorre(&Torre3));
				gtk_widget_set_name(widget,NumDsc->str);
			}else{ // Si no soltaste el disco en una torre lo regresa a su origen
				
				gtk_fixed_move(GTK_FIXED(fixdGame), widget,(atoi(NumTorre->str) == 0 ? Torre1x : (atoi(NumTorre->str) == 1 ? Torre2x : Torre3x))+ 5, Torrey - NumElem(Torres[atoi(NumTorre->str)])*(ABtn-Dsm));
			}
			
		}
		// Reproduce efecto push
		Rep_SFX(SFX[0],0,255);
		// Cambia la imagen del disco.
		char fileimag[40] = {};
		strcat(fileimag,"./img/dscbrk/");
		strcat(fileimag,NumBoton->str);
		strcat(fileimag,".png");
		gtk_button_set_image(GTK_BUTTON(widget),ChangeImgDisc(fileimag,LBtn,ABtn));
		
		// Evalua si ya puede comprobar sí ganó
		if(Movimientos >= (pow(2,Disc)-1)){
			g_print("Verificando...\n");
			if(Winner() == 1){
				Play = 0;
				//MsgBox("Ganaste!");
				WinDialog(2);
				g_print("Ganaste\n");
			}else if (Winner() == 0){
				Play = 0;
				WinDialog(2);
				//MsgBox("Perdiste :(");
			}
		}
	}
	
	//g_print("Cima 1: %d, Cima 2: %d, Cima 3: %d, E1: %d, E2: %d, E3: %d\n",Cima(&Torre1),Cima(&Torre2),Cima(&Torre3),NumElem(&Torre1),NumElem(&Torre2),NumElem(&Torre3));
}

// Fución que contruye un disco y lo retorna
GtkWidget *make_Disc(gchar* text,gchar *Imagen)
{
	GtkWidget* b = gtk_button_new(); // Creamos el boton, con el label text
	gtk_style_context_add_class(gtk_widget_get_style_context(b),"Disco");
	
	gtk_button_set_image(GTK_BUTTON(b),ChangeImgDisc(Imagen,LBtn,ABtn));
	
	gtk_widget_set_size_request(b,LBtn,ABtn);
	gtk_widget_set_name(b,text);
	gtk_widget_add_events(b, EvMask); // Asignamos las Mask como eventos al boton.
	
	// Enlazamos las funciones para cuando se precione el mouse.
	g_signal_connect(b, "button_press_event", G_CALLBACK(button_press_event), NULL);
	g_signal_connect(b, "button_release_event", G_CALLBACK(MovDisc), NULL);
	g_signal_connect(b, "motion_notify_event",G_CALLBACK(motion_notify_event), NULL);
	gtk_widget_show(b);
	return b;
}

/* Funcín que instancía los primeros discos en la torre 1 */
void TorreInicial(Pila *Torr,int Disc){	
	int Base = Torrey;
	char imag[20]={"img/dscbrk/"},aunx[20] = {};
	
	for(int i = Disc-1; i>=0; i--){
		NumDsc = g_string_new("");
		strcpy(imag,"./img/dscbrk/");
		
		Push(i+1,Torr); // Agrega los elementos a la pila
		Base -= ABtn-Dsm; // Disminuye la x para ir acomodando hacia arriba los botones
		
		sprintf(aunx,"%d.png",i+1);
		strcat(imag,aunx);
		g_string_append_printf(NumDsc,"%d%d",i+1,findTorre(Torr)); // Concatena el número de disco y el número de torre donde se encuentra para el nombre del botón
		gtk_fixed_put(GTK_FIXED(fixdGame), make_Disc(NumDsc->str,imag), Torre1x + 5, Base); // Crea el boton con sus propiedades Drag.
	}	
}

// Función para reiniciar el nivel.
void Reboot(){
	// Inicializa una pila vacía
	PilaVacia(&Torre1);
	PilaVacia(&Torre2);
	PilaVacia(&Torre3);
	
	// Crea los discos de la torre 1
	TorreInicial(&Torre1,Disc);
	
	// Reicia el contador de movimientos, aumenta los intentos y el timer
	Movimientos = Tiempo = Stars = Score = 0;
	Intentos ++;
}

// Parametros para pausar el juego.
void Pause(){
	Play = 0;
	WinDialog(1);
	Play = 1;
}

// Contructor de la ventana para jugar
static void activate_level(){	
	int AnTrrs = 750,AlTrrs=375;
	
	// Destruye los anteriores por si existen
	DestroyGame();
	// creando boton reinciar
	GtkWidget *btnPause;
	btnPause = gtk_button_new_with_label("Pausa");
	
	// Crea el fixed
	fixdGame = gtk_fixed_new();
	// Creando las torres
	btnTorre1 = gtk_button_new_with_label("T1");
	btnTorre2 = gtk_button_new_with_label("T2");
	btnTorre3 = gtk_button_new_with_label("T3");
	
	gtk_widget_set_size_request(btnTorre1,LBtn+10,ATrr);
	gtk_widget_set_size_request(btnTorre2,LBtn+10,ATrr);
	gtk_widget_set_size_request(btnTorre3,LBtn+10,ATrr);
	
	gtk_widget_set_opacity(btnTorre1,0);
	gtk_widget_set_opacity(btnTorre2,0);
	gtk_widget_set_opacity(btnTorre3,0);
	
	gtk_widget_set_name(btnTorre1,"0");
	gtk_widget_set_name(btnTorre2,"1");
	gtk_widget_set_name(btnTorre3,"2");
	
	// Creando el label cronometro
	lbCrono = gtk_label_new("");
	gtk_style_context_add_class(gtk_widget_get_style_context(lbCrono),"blackback");
	
	// Añade los componentes al fixed
	gtk_fixed_put(GTK_FIXED(fixdGame),PushBackground(level,Ancho,Alto),0,0);
	gtk_fixed_put(GTK_FIXED(fixdGame),BtnImg(60,60,ImgBtns[6],"Pause","",G_CALLBACK(Pause)),20,20);
	gtk_fixed_put(GTK_FIXED(fixdGame),PushImg(200,155,ImgBtns[7]),580,20);
	gtk_fixed_put(GTK_FIXED(fixdGame),PushImg(AnTrrs,AlTrrs,ImgDsc[0]),(Ancho/2)-(AnTrrs/2),Alto-AlTrrs-20);
	gtk_fixed_put(GTK_FIXED(fixdGame),lbCrono,650,70);
	gtk_fixed_put(GTK_FIXED(fixdGame),btnTorre1,Torre1x,Torrey-ATrr);
	gtk_fixed_put(GTK_FIXED(fixdGame),btnTorre2,Torre2x,Torrey-ATrr);
	gtk_fixed_put(GTK_FIXED(fixdGame),btnTorre3,Torre3x,Torrey-ATrr);
	
    // inicia el nivel
	Reboot();
    
    // Añade el contenedor fixed a la ventana
	if(gtk_widget_is_ancestor(fixdMenu,GTK_WIDGET(windowMenu))){
		//gtk_container_remove(GTK_CONTAINER(windowMenu), fixdMenu);
		gtk_widget_destroy(fixdMenu);
	}
	gtk_container_add(GTK_CONTAINER(windowMenu), fixdGame);
    
    // Mostrar todos los componentes
    gtk_widget_show_all(windowMenu);
    //gtk_widget_set_visible(fixdMenu,FALSE);
    Play = 1;
}

// Funcion que ordena de mayor a menor los usuarios de ranking.
void OrdenarUsr(Usuario *arrUs,int lon){
	Usuario aux;
	
	for(int i = 0;i<lon;i++){
		for(int j = i + 1;j<lon;j++){
			if((arrUs[i].puntaje < arrUs[j].puntaje) && (arrUs[i].puntaje > 0.0) && (arrUs[j].puntaje > 0.0)){
				g_print("ipunt: %f, jpunt: %f\n",arrUs[i].puntaje,arrUs[j].puntaje);
				aux = arrUs[j];
				arrUs[j] = arrUs[i];
				arrUs[i]=aux;
			}
		}
	}
}

// Función que administra los layouts del menú
void ChangeLayoutMenu(GtkWidget *widget, gpointer   data ){
	int NumBot = atoi(gtk_button_get_label(GTK_BUTTON(widget))),lon;
	Usuario UsAux[100] = {};
	GString *aux1 = g_string_new(""),*aux2 = g_string_new("");
	GtkWidget *ScrllWn,*Grid,*viewp,*labelUs = gtk_label_new("");
	gtk_style_context_add_class(gtk_widget_get_style_context(labelUs),"entryname");
	
	// Reproduce sonido de click
	Rep_SFX(SFX[2],0,200);
	gtk_widget_destroy(fixdMenu);
	
	fixdMenu = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(fixdMenu),PushBackground(0,Ancho,Alto),0,0);
	
	switch (NumBot)
	{
		case 1: // Nuevo juego
			// Reinicia el box que simula una ventana
			LayoutOpc = gtk_fixed_new();
			
			// Crenado los widget para nuevo juego
			GtkWidget *btnAsek = gtk_button_new_with_label("Asekto"), *btnVisible = gtk_button_new();
			
			txtNombre = gtk_entry_new();
			txtPasw = gtk_entry_new();
			btnVisible = BtnImg(30,30,ImgBtns[2],"","",G_CALLBACK(VisiblePasw));
			
			gtk_style_context_add_class(gtk_widget_get_style_context(txtNombre),"entryname");
			gtk_style_context_add_class(gtk_widget_get_style_context(txtPasw),"entryname");
			
			gtk_entry_set_width_chars(GTK_ENTRY(txtNombre),15);
			gtk_entry_set_width_chars(GTK_ENTRY(txtPasw),15);
			
			gtk_entry_set_visibility(GTK_ENTRY(txtPasw),FALSE);
			
			gtk_widget_set_name(btnAsek,"Nuevo");
			gtk_widget_set_opacity(btnAsek,0);
			g_signal_connect(btnAsek,"clicked",G_CALLBACK(SaveGame),NULL);
			g_signal_connect(btnVisible,"clicked",G_CALLBACK(VisiblePasw),NULL);
			
			// Damos tamaño y agregamos los widgets al box
			gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(400,300,Bckgrnd[11]),(Ancho/2)-(400/2),(Alto/2)-(300/2));
			gtk_fixed_put(GTK_FIXED(LayoutOpc),txtNombre,440,230);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),txtPasw,440,300);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),btnAsek,(Ancho/2)-50,360);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),btnVisible,395,310);
			
			break;
		case 2:	// Continuar juego
			// Reinicia el box que simula una ventana
			LayoutOpc = gtk_fixed_new();

			// Colocamos el fondo
			gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(400,300,Bckgrnd[12]),(Ancho/2)-(400/2),(Alto/2)-(300/2));
			
			// Creamos el contenedor scroll
			ScrllWn = gtk_scrolled_window_new(NULL,NULL);
			Grid = gtk_grid_new();
			
			if(listaUser(UsAux) == -9999){
				labelUs = gtk_label_new("\n\t\t\t\tNo hay registros aun");
				gtk_grid_attach(GTK_GRID(Grid),labelUs,0,0,1,1);
			}else{
				lon = sizeof(UsAux) / sizeof(UsAux[0]);
				//g_print("size tot: %ld, size fir: %ld, long: %d\n",sizeof(UsAux),sizeof(UsAux[0]),lon);
				//OrdenarUsr(UsAux,lon);		
				
				for(int i = 0;i<lon;i++){
					if(UsAux[i].nivel > 0){
						GtkWidget *Usua;
						
						strcpy(aux1->str,UsAux[i].nombre);
						strcat(aux1->str,"\t\t\t\t\t\t\t\t\t");
						sprintf(aux2->str,"%d",UsAux[i].nivel);
						strcat(aux1->str,aux2->str);
						sprintf(aux2->str,"%d",UsAux[i].codigo);
						Usua = BtnImg(200,30,"",aux2->str,aux1->str,G_CALLBACK(LayoutPasw));
						gtk_widget_set_name(Usua,"blackback");
												
						gtk_grid_attach(GTK_GRID(Grid),Usua,0,i,1,1);			
					}					
				}
			}
			
			gtk_container_add(GTK_CONTAINER(ScrllWn),Grid);
			gtk_widget_set_size_request(ScrllWn,320,150);
			
			gtk_fixed_put(GTK_FIXED(LayoutOpc),ScrllWn,(Ancho/2)-(320/2),250);
			break;
		case 3: // Mejor puntaje
			
			// Reinicia el box que simula una ventana
			LayoutOpc = gtk_fixed_new();
			
			// Colocamos el fondo
			gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(400,300,Bckgrnd[13]),(Ancho/2)-(400/2),(Alto/2)-(300/2));
			
			// Creamos el contenedor scroll
			ScrllWn = gtk_scrolled_window_new(NULL,NULL);
			Grid = gtk_grid_new();
			
			if(listaUser(UsAux) == -9999){
				labelUs = gtk_label_new("\n\t\t\t\tNo hay registros aun");
				gtk_grid_attach(GTK_GRID(Grid),labelUs,0,0,1,1);
			}else{
				lon = sizeof(UsAux) / sizeof(UsAux[0]);
				g_print("size tot: %ld, size fir: %ld, long: %d\n",sizeof(UsAux),sizeof(UsAux[0]),lon);
				OrdenarUsr(UsAux,lon);				
				
				for(int i = 0;i<lon;i++){
					if(UsAux[i].puntaje > 0.0){
						GtkWidget *Usua,*Sco;
						
						strcpy(aux1->str," ");
						strcat(aux1->str,UsAux[i].nombre);
						strcat(aux1->str,"\t\t\t\t\t\t\t\t");
						Usua = gtk_label_new(aux1->str);
						strcpy(aux1->str,"");
						
						sprintf(aux1->str,"%0.2f",UsAux[i].puntaje);
						Sco = gtk_label_new(aux1->str);
						strcpy(aux1->str,"");
						
						if(i == 0){
							gtk_grid_attach(GTK_GRID(Grid),PushImg(30,30,ImgBtns[3]),0,i,1,1);
						}else if(i == 1){
							gtk_grid_attach(GTK_GRID(Grid),PushImg(30,30,ImgBtns[4]),0,i,1,1);
						}else if(i == 2){
							gtk_grid_attach(GTK_GRID(Grid),PushImg(30,30,ImgBtns[5]),0,i,1,1);
						}else{
							sprintf(aux1->str,"\t%d\t",i+1);
							gtk_grid_attach(GTK_GRID(Grid),gtk_label_new(aux1->str),0,i,1,1);
						}
						gtk_grid_attach(GTK_GRID(Grid),Usua,1,i,1,1);
						gtk_grid_attach(GTK_GRID(Grid),Sco,2,i,1,1);						
					}
					
				}
			}
			
			gtk_container_add(GTK_CONTAINER(ScrllWn),Grid);
			gtk_widget_set_size_request(ScrllWn,330,150);
			
			gtk_fixed_put(GTK_FIXED(LayoutOpc),ScrllWn,(Ancho/2)-(330/2),270);
			
			break;
		case 4: // Historia
			GtkWidget *TextMul;
			aux1 = g_string_new("");
			aux2 = g_string_new("");
			int chars = 0;
			//GtkTextBuffer *buff = gtk_text_buffer_new(NULL); 
			//gtk_text_buffer_set_text(buff,"La leyenda del origen de este rompecabezas que cuenta que hace mucho tiempo, los monjes de un monasterio ubicado en la región de Hanói, Vietnam, se encontraban rezando a sus tres dioses: Brahma (el dios creador), Vishnu (el dios conservador) y Shiva (el dios destructor), ya que querían saber cuándo iba a ser destruido el mundo. En respuesta a sus plegarias, se apareció ante ellos el dios Brahma y les entregó una base con tres postes, los cuales eran de diamante y del grosor del aguijón de un avispón, en uno de ellos se encontraban apilados 64 discos de oro puro de diferentes tamaños, siendo el disco más pequeño el que se encontraba en la punta de la torre, después un disco más grande, y, así sucesivamente, hasta llegar a la base de la torre donde se encontraba el disco de mayor tamaño. Una vez que el dios Brahma se retiró, los monjes se preguntaban qué debían hacer con los discos, en ese momento se manifestó el dios Vishnu y les explicó las reglas que describimos al inicio. Los monjes se alegraron de saber lo que tenían que hacer y pensaron que si movían todos los discos de un poste al otro cumpliendo las reglas que les había sido explicadas, complacerían a sus dioses y estos les dirían cuándo se terminaría el mundo. Cuando estaban a punto de comenzar a mover los discos, hizo su aparición el dios Shiva y les dijo: ‘’cuando terminen de mover los 64 discos, en ese momento el mundo habrá terminado’’. Los monjes se llenaron de miedo, pues si no movían los discos, sus dioses se enfadarían con ellos y los castigarían, pero si terminaban de mover todos los discos, entonces el mundo llegaría a su fin. ¡Qué difícil situación en la que se habían metido los monjes! Antes de comenzar a mover los discos, los monjes querían saber el tiempo exacto que le quedaba al mundo, así que pensaron que si conocían el número exacto de movimientos mínimos necesarios para mover todos los discos, entonces podrían calcular con certeza el tiempo que restaba para que el mundo terminara. Así, su gran pregunta era, ¿cuántos movimientos como mínimo se necesitan para mover todos los discos?.\0",2096);
			
			char strng[3000] = {
				"La leyenda del origen de este rompecabezas que cuenta que hace mucho tiempo, los monjes de un monasterio ubicado en la región de Hanói, Vietnam, se encontraban rezando a sus tres dioses: Brahma (el dios creador), Vishnu (el dios conservador) y Shiva (el dios destructor), ya que querían saber cuándo iba a ser destruido el mundo. En respuesta a sus plegarias, se apareció ante ellos el dios Brahma y les entregó una base con tres postes, los cuales eran de diamante y del grosor del aguijón de un avispón, en uno de ellos se encontraban apilados 64 discos de oro puro de diferentes tamaños, siendo el disco más pequeño el que se encontraba en la punta de la torre, después un disco más grande, y, así sucesivamente, hasta llegar a la base de la torre donde se encontraba el disco de mayor tamaño. Una vez que el dios Brahma se retiró, los monjes se preguntaban qué debían hacer con los discos, en ese momento se manifestó el dios Vishnu y les explicó las reglas que describimos al inicio. Los monjes se alegraron de saber lo que tenían que hacer y pensaron que si movían todos los discos de un poste al otro cumpliendo las reglas que les había sido explicadas, complacerían a sus dioses y estos les dirían cuándo se terminaría el mundo. Cuando estaban a punto de comenzar a mover los discos, hizo su aparición el dios Shiva y les dijo: ‘’cuando terminen de mover los 64 discos, en ese momento el mundo habrá terminado’’. Los monjes se llenaron de miedo, pues si no movían los discos, sus dioses se enfadarían con ellos y los castigarían, pero si terminaban de mover todos los discos, entonces el mundo llegaría a su fin. ¡Qué difícil situación en la que se habían metido los monjes! Antes de comenzar a mover los discos, los monjes querían saber el tiempo exacto que le quedaba al mundo, así que pensaron que si conocían el número exacto de movimientos mínimos necesarios para mover todos los discos, entonces podrían calcular con certeza el tiempo que restaba para que el mundo terminara. Así, su gran pregunta era, ¿cuántos movimientos como mínimo se necesitan para mover todos los discos?.\0"
			};
			
			// Reinicia el box que simula una ventana
			LayoutOpc = gtk_fixed_new();
			// Colocamos el fondo
			gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(450,550,Bckgrnd[14]),(Ancho/2)-(450/2),(Alto/2)-(550/2));
			
			// Creamos el contenedor scroll
			ScrllWn = gtk_scrolled_window_new(NULL,NULL);
			Grid = gtk_grid_new();
			
			for(int i = 0;i<3000;i++){
				g_string_append_c(aux1,strng[i]);
				chars ++;
				
				if(strng[i] == 32 && chars > 40){
					chars = 0;
					g_string_append(aux1,"\n");
				}
			}
			
			TextMul = gtk_label_new(aux1->str);
			gtk_style_context_add_class(gtk_widget_get_style_context(TextMul), "whiteback");
			
			//strcpy(aux2->str,"<span foreground= '#1c1c1c'> ");
			//strcat(aux2->str,aux1->str);
			//strcat(aux2->str," </span>");
			//gtk_label_set_markup(GTK_LABEL(TextMul),aux2->str);
			
			gtk_container_add(GTK_CONTAINER(ScrllWn),TextMul);
			gtk_widget_set_size_request(ScrllWn,400,400);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),ScrllWn,(Ancho/2)-(400/2),120);			
			break;
		case 5: // Creditos
			destroy_sample(sonido);
			// Reinicia el box que simula una ventana
			LayoutOpc = gtk_fixed_new();
			// Colocamos el fondo
			gtk_widget_set_size_request(LayoutOpc,Ancho,Alto);
			gtk_fixed_put(GTK_FIXED(LayoutOpc),PushImg(Ancho,Alto,Bckgrnd[7]),0,0);
			
			//~ Rep_Musica(BkndMusic[0],1,255);// Reproduce el sonido de fondo.			
			break;
		default:
			//gtk_window_close(GTK_WINDOW(windowMenu));
			destroy();
	}
	
	gtk_fixed_put(GTK_FIXED(LayoutOpc),BtnImg(AnBtn,AlBtn,ImgBtns[2],"","",G_CALLBACK(BackMenu)),10,10);	
	gtk_fixed_put(GTK_FIXED(fixdMenu),LayoutOpc,0,0);
	
	// Añade el contenedor fixed a la ventana
	gtk_container_add(GTK_CONTAINER(windowMenu), fixdMenu);
    // Mostrar todos los componentes
    gtk_widget_show_all(windowMenu); 
}

// Funcion contructora, de la venta principal
void Make_Menu(){
	xBtn=((Ancho/2)-(AnBtn/2));
	yInc=AlBtn + 25;
	
	GtkWidget *btn1 = gtk_button_new_with_label("1"),
	*btn2= gtk_button_new_with_label("2"),
	*btn3= gtk_button_new_with_label("3"),
	*btn4= gtk_button_new_with_label("4"),
	*btn5= gtk_button_new_with_label("5"),
	*btn6= gtk_button_new_with_label("6");
	
	fixdMenu = gtk_fixed_new(); 
	boxOpc = gtk_box_new(TRUE,22);
	gtk_widget_set_size_request(boxOpc,AnBtn,AlBtn);
	gtk_widget_set_opacity(boxOpc,0);
	
	//~ g_print("Entro\n");
	
	g_signal_connect(btn1,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	g_signal_connect(btn2,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	g_signal_connect(btn3,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	g_signal_connect(btn4,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	g_signal_connect(btn5,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	g_signal_connect(btn6,"clicked",G_CALLBACK(ChangeLayoutMenu),NULL);
	
	gtk_box_pack_start(GTK_BOX(boxOpc),btn1,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boxOpc),btn2,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boxOpc),btn3,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boxOpc),btn4,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boxOpc),btn5,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boxOpc),btn6,TRUE,FALSE,0);
	
	//~ g_print("Entro\n");
	// Agregamos la imagen de fondo y la imagen del menu
	gtk_fixed_put(GTK_FIXED(fixdMenu),PushBackground(0,Ancho,Alto),0,0);
	gtk_fixed_put(GTK_FIXED(fixdMenu),PushImg(363,456,ImgBtns[0]),((Ancho/2)-(363/2)),120);
	gtk_fixed_put(GTK_FIXED(fixdMenu),boxOpc,xBtn,yBtn);
	
	// Añade el contenedor fixed a la ventana
	gtk_container_add(GTK_CONTAINER(windowMenu), fixdMenu);
    
    // Mostrar todos los componentes
    gtk_widget_show_all(windowMenu);
    
    //~ Rep_Musica(BkndMusic[8],1,255);
}

void VentanaMenu(GtkApplication *app, gpointer user_data){
	
	// Creando la ventana
	windowMenu = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(windowMenu), "Torres");
    gtk_window_set_default_size(GTK_WINDOW(windowMenu), Ancho, Alto);
    gtk_window_set_position(GTK_WINDOW(windowMenu), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(windowMenu), FALSE);
    
    LoadCSS("./estilos.css");
	Make_Menu();
	
    //~ Rep_Musica(BkndMusic[8],1,255);// Reproduce el sonido de fondo.
}

// Funcón principal
int main(int argc, char **argv){
	GtkApplication *app;
    int status;
    txtDatos = g_string_new("");
	
	// Iniciamos allegro.
	allegro_init();
	set_volume(230, 200);

    // Inicializa la ventana
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(VentanaMenu), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
}
END_OF_MAIN();
