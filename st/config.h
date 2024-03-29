
#define FONT "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*"
#define BOLDFONT "-*-terminus-bold-r-*-*-12-*-*-*-*-*-*-*"

/* Space in pixels around the terminal buffer */
#define BORDER 0

/* Default shell to use if SHELL is not set in the env */
#define SHELL "/bin/sh"

/* Terminal colors (16 first used in escape sequence) */
static const char *colorname[] = {
	/* 8 normal colors */
	"#000000",
	"#ff6565",
	"#93d44f",
	"#eab93d",
	"#204a87",
	"#963c59",
	"#89b6e2",
	"#cccccc",

	/* 8 bright colors */
	"#555753",
	"#ff8d8d",
	"#c8e7a8",
	"#ffc123",
	"#3465a4",
	"#cf9ebe",
	"#46a4ff",
	"#ffffff",

	[255] = 0,

	/* more colors can be added after 255 to use with DefaultXX */
	"#cccccc",
	"#333333",
};

/* Default colors (colorname index)
   foreground, background, cursor, unfocused cursor */
#define DefaultFG  7
#define DefaultBG  0
#define DefaultCS  256
#define DefaultUCS 257

/* Special keys (change & recompile st.info accordingly)
   Keep in mind that kpress() in st.c hardcodes some keys.

   Mask value:
   * Use XK_ANY_MOD to match the key no matter modifiers state
   * Use XK_NO_MOD to match the key alone (no modifiers)

      key,        mask,  output */
static Key key[] = {
	{ XK_BackSpace, XK_NO_MOD, "\177" },
   	{ XK_Insert,    XK_NO_MOD, "\033[2~" },
	{ XK_Delete,    XK_NO_MOD, "\033[3~" },
	{ XK_Home,      XK_NO_MOD, "\033[1~" },
	{ XK_End,       XK_NO_MOD, "\033[4~" },
	{ XK_Prior,     XK_NO_MOD, "\033[5~" },
	{ XK_Next,      XK_NO_MOD, "\033[6~" },
	{ XK_F1,        XK_NO_MOD, "\033OP"   },
	{ XK_F2,        XK_NO_MOD, "\033OQ"   },
	{ XK_F3,        XK_NO_MOD, "\033OR"   },
	{ XK_F4,        XK_NO_MOD, "\033OS"   },
	{ XK_F5,        XK_NO_MOD, "\033[15~" },
	{ XK_F6,        XK_NO_MOD, "\033[17~" },
	{ XK_F7,        XK_NO_MOD, "\033[18~" },
	{ XK_F8,        XK_NO_MOD, "\033[19~" },
	{ XK_F9,        XK_NO_MOD, "\033[20~" },
	{ XK_F10,       XK_NO_MOD, "\033[21~" },
	{ XK_F11,       XK_NO_MOD, "\033[23~" },
	{ XK_F12,       XK_NO_MOD, "\033[24~" },
};

/* Set TERM to this */
#define TNAME "st-256color"

/* Line drawing characters (sometime specific to each font...) */
static char gfx[] = {
	['f'] = 'o',
	['g'] = '+',
	['i'] = '#',
	[255] = 0,
};

/* double-click timeout (in milliseconds) between clicks for selection */
#define DOUBLECLICK_TIMEOUT 300
#define TRIPLECLICK_TIMEOUT (2*DOUBLECLICK_TIMEOUT)

#define TAB 8
