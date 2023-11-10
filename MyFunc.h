#include <stdio.h>

#define MDig 5
#define MChar 1000
#define MStr 50
#define SizeImg 500

static GtkTextBuffer *Buff;

void MsgBox (GtkWindow *win,const gchar *format,...) __attribute__((__format__ (gnu_printf, 2, 3)));

// Esta función solo recorre un string y la limpia.
void CleanString(char *c,int len);

void Destruir();

int IsInteger(char *c,int len);

int CheckSyntax(GString *s);

double StrToFlo(char *str);

int EsNum(char c);

void LoadCSS(char *file);

//~ void widg_set_trans(GtkWidget *w);

static void color_back(cairo_t *cr);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);

int GetFormat(GString *D,GString **S);

gboolean CheckVert(GString *obj, GString **v,int len);

void HasConnect(GString *m[MChar][MChar],GString **c,GString **verts,int len, int *len2);

//~ GtkWidget *PushBackground(char *file,int w, int h);

GtkWidget *PushImg(int w, int h,char *File);

GtkWidget *ButtonImg(char *name,char *ImgFile,char *Tooltip,int wImg,int hImg,GCallback handler);

GtkWidget *ButtonIco(char *name,char *Ico,char *Tooltip,GCallback handler);

GtkWidget *MakeLabels(char *Text);

GtkWidget *MakeSeparator(GtkOrientation orientacion);

GtkWidget *MakeEntry(char *Name, char *Tooltip, GCallback Activate, GCallback Change);

GtkWidget *MakeTextView(char *Name, char *Tooltip,int h, int w);

GtkWidget *MakeCombo(const gchar *c[], GCallback handler, int len);
