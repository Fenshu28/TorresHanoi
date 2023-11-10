#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <gtk/gtk.h>
#include "Pila.h"
#include "MyFunc.h"


/*Función para mostrar una caja de mensaje con un determinado texto.
 * Recibe 1 argumento por referencia: el mensaje a mostrar, 
 * "*aviso" de tipo apuntador a char, que  realmente es un vector 
 * de caracteres. No retorna nada porque la función es vacía.*/
void MsgBox(GtkWindow *win,const gchar *format, ...){
	//char formatted_str[128];
	char formatted_str[MChar];
	GtkWidget *FrmDialog;
	
	va_list arglist;
    va_start(arglist, format);

    int length = vsnprintf(formatted_str, sizeof(formatted_str), format, arglist);
    va_end(arglist);
	
	FrmDialog = gtk_message_dialog_new(win,
					GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"%s",
					formatted_str);
					
	gtk_dialog_run(GTK_DIALOG(FrmDialog));
	gtk_widget_destroy(FrmDialog);
	
}

int IsInteger(char *c,int len){
	for(int i = 0; i < len; i++){
		//printf("%d\n",c[i]);
		if((c[i] < 48 || c[i] > 57) && c[i] != 46 && c[i] != 0){ // no es un numero o un punto.
			return 0;
		}
	}
	return 1; // si termina y no encontró un error debuelve true
}

// Esta función solo recorre un string y la limpia.
void CleanString(char *c,int len){
	for(int i = 0; i < len; i++){
		c[i] = '\0';
	}
}

void Destruir() { gtk_main_quit(); exit(0);}

void LoadCSS(char *file){
	GtkCssProvider *cssProvider;
	cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, file, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
											GTK_STYLE_PROVIDER(cssProvider),
											GTK_STYLE_PROVIDER_PRIORITY_USER);
}

//~ void widg_set_trans(GtkWidget *w){
	//~ GdkScreen *screen;
	//~ GdkVisual *visual;
	//~ gtk_widget_set_app_paintable(w, TRUE);
	//~ screen = gdk_screen_get_default();
	//~ visual = gdk_screen_get_rgba_visual(screen);
	//~ if (visual != NULL && gdk_screen_is_composited(screen)) {
		//~ gtk_widget_set_visual(w, visual);
	//~ }
//~ }

void color_back(cairo_t *cr){
  cairo_set_source_rgba(cr, 0.135, 0.206, 0.235, 0.8);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data){
  color_back(cr);
  return FALSE;
}

 //~ GtkWidget *PushBackground(char *file,int w, int h){
	//~ GtkWidget *picture;
	//~ GdkPixbuf *pixbuf;
	
	//~ /* Cargamos la imagen */
	//~ pixbuf = gdk_pixbuf_new_from_file_at_scale(file, /* nombre de fichero */
							 //~ w, /* ancho de la pantalla */
							 //~ h, /* alto de la pantalla */
							 //~ FALSE, /* No respetar aspecto */
							 //~ NULL);
	//~ /* Guardamos la imagen en un widget */
	//~ picture = gtk_image_new_from_pixbuf(pixbuf);
	//~ //gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(picture));
	//~ return picture;
//~ }

GtkWidget *PushImg(int w, int h,char *File){
	 GdkPixbuf *pixbuf; 
	 GtkWidget *picture;
	/* Cargamos la imagen */
	pixbuf = gdk_pixbuf_new_from_file_at_scale(File, /* nombre de fichero */
							 w, /* ancho de la imagen */
							 h, /* alto de la imagen */
							 FALSE, /* No respetar aspecto */
							 NULL);
	/* Guardamos la imagen en un widget */
	picture = gtk_image_new_from_pixbuf(pixbuf);
	//gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(picture));
	return picture;
}

GtkWidget *ButtonImg(char *name,char *ImgFile,char *Tooltip,int wImg,int hImg,GCallback handler){
	GtkWidget *btn = gtk_button_new();
	
	if(strcmp(name,"")){
		gtk_widget_set_name(btn,name);
	}
	
	if(strcmp(ImgFile,"")){
		GtkWidget *Img = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_size(ImgFile,wImg,hImg,NULL));
		gtk_button_set_image(GTK_BUTTON(btn),Img);
		gtk_button_set_image_position(GTK_BUTTON(btn),GTK_POS_TOP);		
	}
	
	if(strcmp(Tooltip,"")){
		gtk_widget_set_tooltip_text(btn,Tooltip);
	}
	
	if(handler){
		g_signal_connect(btn,"clicked",handler,NULL);
	}
	
	gtk_style_context_add_class(gtk_widget_get_style_context(btn), "background-trans");
	//gtk_style_context_add_class(gtk_widget_get_style_context(btn), "black-bg");
	
	return btn;
}

GtkWidget *ButtonIco(char *name,char *Ico,char *Tooltip,GCallback handler){
	GtkWidget *btn = gtk_button_new();
	
	if(strcmp(Ico,"")){
		btn = gtk_button_new_from_icon_name(Ico,GTK_ICON_SIZE_MENU);
	}
	
	if(handler){
		g_signal_connect(btn,"clicked",handler,NULL);
	}	
	
	if(strcmp(Tooltip,"")){
		gtk_widget_set_tooltip_text(btn,Tooltip);
	}
	
	if(strcmp(name,"")){
		gtk_widget_set_name(btn,name);
	}
	
	//gtk_style_context_add_class(gtk_widget_get_style_context(btn), "background-trans");
	gtk_style_context_add_class(gtk_widget_get_style_context(btn), "black-bg");
	
	return btn;
}

GtkWidget *MakeLabels(char *Text){
	GtkWidget *w = gtk_label_new(Text);
	gtk_style_context_add_class(gtk_widget_get_style_context(w), "black-bg");
	return w;
}

GtkWidget *MakeSeparator(GtkOrientation orientacion){
	GtkWidget *sep = gtk_separator_new(orientacion);
	gtk_style_context_add_class(gtk_widget_get_style_context(sep), "separator");
	
	return sep;
}

GtkWidget *MakeEntry(char *Name, char *Tooltip, GCallback Activate, GCallback Change){
	/* Create a buffer */
    //GtkEntryBuffer *buffer = gtk_entry_buffer_new (NULL, 0);

    /* Create our first entry */
	GtkWidget *txt = gtk_entry_new();
	
	if(strcmp(Tooltip,"")){
		gtk_widget_set_tooltip_text(txt,Tooltip);
	}
	
	if(strcmp(Name,"")){
		gtk_widget_set_name(txt,Name);
	}
	
	if(Activate != NULL){
		g_signal_connect(txt,"activate",Activate,NULL);
	}
	
	if(Change !=NULL){
		g_signal_connect(txt,"changed",Change,NULL);
	}
	
	//gtk_style_context_add_class(gtk_widget_get_style_context(txt), "background-trans");
	gtk_style_context_add_class(gtk_widget_get_style_context(txt), "Textos");
	
	return txt;
}

GtkWidget *MakeTextView(char *Name, char *Tooltip,int h, int w){
	/* Create a buffer */
    //GtkEntryBuffer *buffer = gtk_entry_buffer_new (NULL, 0);
    GtkCssProvider *provider = gtk_css_provider_new ();

    /* Create our first entry */
	GtkWidget *txt = gtk_text_view_new(),*box = gtk_scrolled_window_new(NULL,NULL);
	
	
	if(strcmp(Tooltip,"")){
		gtk_widget_set_tooltip_text(txt,Tooltip);
	}
	
	if(strcmp(Name,"")){
		gtk_widget_set_name(txt,Name);
	}
	
	
	gtk_css_provider_load_from_data(provider,
			"*{background-image: url(\"./Img/txtVert2.png\");background-repeat: no-repeat;background-position: center;background-size: 500% auto;color: black;font-size: 20pt;	border-style: none;	border-top-style: none;	color: rgba(0.5,0.4,0.6,1);} selection{background-color: blue;  color: white;}",
			 -1, NULL);	
	gtk_style_context_add_provider (gtk_widget_get_style_context (txt), GTK_STYLE_PROVIDER (provider), G_MAXUINT);
	
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW(txt), GTK_WRAP_WORD_CHAR);
	gtk_text_view_set_left_margin (GTK_TEXT_VIEW (txt), 20);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW (txt), 20);
    
	gtk_widget_set_size_request(box,w,h);
	gtk_container_add(GTK_CONTAINER(box),txt);
	
	//~ gtk_style_context_add_class(gtk_widget_get_style_context(txt), "background-trans");
	//~ gtk_style_context_add_class(gtk_widget_get_style_context(txt), "white-bg");
	
	return box;
}

GtkWidget *MakeCombo(const gchar *c[], GCallback handler, int len){
	// Crea el combo box de tipos.
    GtkWidget *cmb = gtk_combo_box_text_new();
    //GString texts;
    for (int i = 0; i < len; i++){
		//texts  = g_string_new(c[0][i]);
		gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (cmb), c[i]);
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (cmb), 0);
	
	if(handler != NULL){
		g_signal_connect(cmb,"changed",handler,NULL);
	}
	
	//~ GtkCssProvider *provider = gtk_css_provider_new ();
	//~ gtk_css_provider_load_from_data(provider,
			//~ "*{background-image: url(\"./Img/txtVert2.png\");background-repeat: no-repeat;background-position: center;background-size: 500% auto;color: black;font-size: 20pt;	border-style: none;	border-top-style: none;	color: rgba(0.5,0.4,0.6,1);} selection{background-color: blue;  color: white;}",
			 //~ -1, NULL);	
	//~ gtk_style_context_add_provider (gtk_widget_get_style_context (cmb), GTK_STYLE_PROVIDER (provider), G_MAXUINT);
	
	return cmb;
}

int CheckSyntax(GString *s){
	Pila p;
	int c,cent;
	PilaVacia(&p);
	
	// ( = 40, ) = 41, [ = 91, ] = 93, { = 123, } = 125.
	// 40,90,123 - 41,93,125
	
	for(int i = 0; i < s->len; i++){
		c = s->str[i];
		if(c  == 40 || c == 91 || c == 123){
			Push(c,&p);
		}else if (c == 41 || c == 93 || c == 125){
			switch (c){
				case 41:
					if(Cima(&p) == 40){
						Pop(&p);
					}else{
						return 1;
					}
					break;
				case 93:
					if(Cima(&p) == 91){
						Pop(&p);
					}else{
						return 1;
					}
					break;
				case 125:
					if(Cima(&p) == 123){
						Pop(&p);
					}else{
						return 1;
					}
					break;	
				default:
					
			}
		}else if ((Cima(&p)  == 40 || Cima(&p) == 91 || Cima(&p) == 123) && i == (s->len - 1)){
			printf("paso aqui\n");
			return 1;
		}
		//printf("i = %d,c = %c - %d, Cima = %d - %c\n",i,c,c,Cima(&p),Cima(&p));
	}
	
	
	return 0;
}

double StrToFlo(char *str){
	char c[MStr] = {};
	int j = 0, i = 0;;
	CleanString(c,MStr);

	while(str[i] != '\0'){
		if((str[i] > 47 && str[i] < 58) || str[i] == 46){
			c[j] = str[i];
			j++;
		}
		i++;
	}
	return atof(c);
}

int EsNum(char c){
	if((c > 47 && c < 58) || c == 46){
		return 1;
	}else{
		return 0;
	}
}

int GetFormat(GString *D,GString **S){
	int i = 0,j = 0, N = 0;
	char cad[MStr]={}; 
	//Datos = g_string_new((const gchar *)d);
	//printf("%ld\n",Datos->len);
	do{
		//printf("%d\n",i);
		if((D->str[i] != 44) && (D->str[i] != 32) && 
			(D->str[i] != 40) && (D->str[i] != 41) && 
			(D->str[i] != 123) && i != D->len && 
			(D->str[i] != '\n')){ // Verifica si el caracter no es ',' o ' '.
			if(D->str[i] != 47){
				cad[j] = D->str[i];
			}else{
				cad[j] = ' ';
			}
			j ++;
			//printf("agrega a cad\n");
		}else if(D->str[i] == 44 || // Si encuentra una coma, inserta
				 D->str[i] == 45 || // Si encuentra un guion, inserta
				 D->str[i] == 59 || // Si encuentra un punto y coma, inserta
				 i >= D->len){		// Si es el ultimo caracter tambien inserta
			if(strcmp(cad," ") && strcmp(cad,"")){
				S[N] = g_string_new(cad); 
				//printf("%s\n",Vertices[Nvert]->str);
				// limpiamos la cadena auxiliar de los enteros(const gchar *)
			}else{
				S[N] = g_string_new("-9999"); 
			}
			CleanString(cad,MStr);
			j = 0;
			N ++;
		}
		
		i ++;
	}while(i <= D->len);
	
	return N;
}

gboolean CheckVert(GString *obj, GString **v,int len){
	int  i;
	if(len > 0){
		for(i = 0; i< len;i++){
			if(!strcmp(obj->str,v[i]->str)){
				return TRUE;
			}
		}
	}	
	
	return FALSE;
}


void HasConnect(GString *m[MChar][MChar],GString **c,GString **verts,int len, int *len2){
	int i, j,k=0;
	
	*len2 = 0;
	
	for(i = 0; i < len; i++){
		for(j = 0; j < len; j++){
			if(strcmp(m[i][j]->str,"-9999")){
				if(!CheckVert(verts[i],c,*len2)){
					c[*len2] = g_string_new(verts[i]->str);
					printf("%s - %d\n",c[*len2]->str,*len2);
					
					(*len2) ++;
				}

				
				
				if(!CheckVert(verts[j],c,*len2)){
					c[*len2] = g_string_new(verts[j]->str);
					printf("%s - %d\n",c[*len2]->str,*len2);
					
					(*len2) ++;
				}
				
				
				
			}
		}
	}	
}


