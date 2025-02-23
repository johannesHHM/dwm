/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx     = 3;   /* border pixel of windows */
static unsigned int gappx        = 0;   /* gaps between windows */
static unsigned int barheight    = 7;   /* gaps between windows */
static unsigned int snap         = 0;  /* snap pixel */
static unsigned int ulinepad     = 0;	/* horizontal padding between the underline and tag */
static unsigned int ulinestroke	 = 4;	/* thickness / height of the underline */
static unsigned int ulinevoffset = 0;	/* how far above the bottom of the bar the line should appear */
static int ulineall 		 = 0;	/* 1 to show underline on all tags, 0 for just the active ones */
static int swallowfloating       = 0;      /* 1 means swallow floating windows by default */
static int showbar               = 1;      /* 0 means no bar */
static int topbar                = 1;      /* 0 means bottom bar */
static int noborder              = 0;      /* 0 means bottom bar */
static char font[1024]           = "FiraCode Nerd Font Mono:size=18:antialias=true:autohint=true";
static char font2[1024]          = "Symbols Nerd Font:size=18:antialias=true:autohint=true";
static char *fonts[]             = { font, font2 };
static char dmenufont[1024]      = "FiraCode Nerd Font Mono:size=18:antialias=true:autohint=true";
static char col_background[]     = "#282828";
static char col_altbackground[]  = "#3c3836";
static char col_foreground[]     = "#ebdbb2";
static char col_altforeground[]  = "#ebdbb2";
static char col_highlight[]      = "#689d6a";
static char *colors[][3]         = {
        /*                foreground         background         border   */
        [SchemeNorm]  = { col_foreground,    col_background,    col_altbackground },
        [SchemeSel]   = { col_altforeground, col_altbackground, col_highlight },
        [SchemeUline] = { col_highlight,     col_altbackground, col_highlight },
};
static char termcol0[] = "#000000"; /* black   */
static char termcol1[] = "#ff0000"; /* red     */
static char termcol2[] = "#33ff00"; /* green   */
static char termcol3[] = "#ff0099"; /* yellow  */
static char termcol4[] = "#0066ff"; /* blue    */
static char termcol5[] = "#cc00ff"; /* magenta */
static char termcol6[] = "#00ffff"; /* cyan    */
static char termcol7[] = "#d0d0d0"; /* white   */
static char termcol8[]  = "#808080"; /* black   */
static char termcol9[]  = "#ff0000"; /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char *termcolor[] = {
	termcol0,
	termcol1,
	termcol2,
	termcol3,
	termcol4,
	termcol5,
	termcol6,
	termcol7,
	termcol8,
	termcol9,
	termcol10,
	termcol11,
	termcol12,
	termcol13,
	termcol14,
	termcol15,
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance  title           tags mask iscentered  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",     NULL,     NULL,           0,        1,          1,          0,           0,        -1 },
	{ "Firefox",  NULL,     NULL,           1 << 8,   1,          0,          0,          -1,        -1 },
	{ "st",       NULL,     NULL,           0,        1,          0,          1,           0,        -1 },
	{ "st-float", NULL,     NULL,           0,        1,          1,          1,           1,        -1 },
	{ NULL,       NULL,     "Event Tester", 0,        1,          0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmsbar"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_background, "-nf", col_foreground, "-sb", col_altbackground, "-sf", col_foreground, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *webcmd[]   = { "firefox", NULL };
static const char *volume_raise[]   = { "volume", "raise", NULL };
static const char *volume_lower[]   = { "volume", "lower", NULL };
static const char *volume_mute[]   = { "volume", "mute", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,			XK_q,      spawn,          {.v = termcmd } },
	{ MODKEY,			XK_w,      spawn,          {.v = webcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_o,      setnmaster,     {.i = 1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,			XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_g,	   setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_g,      setgaps,        {.i = -5 } },
	{ MODKEY|ControlMask,		XK_g,      setgaps,        {.i = 0  } },
	{ MODKEY,			XK_a,      xrdbreload,     { 0 } },
        { MODKEY,                       XK_s,      spawn,	   SHCMD("snap") },
        { MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("snap -s") },
        { MODKEY|ControlMask,           XK_v,      spawn,          {.v = volume_mute } },
        { MODKEY,                       XK_v,      spawn,	   {.v = volume_raise } },
        { MODKEY|ShiftMask,             XK_v,      spawn,          {.v = volume_lower } },
        { MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("xkill") },
        { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("dunstctl set-paused toggle") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_m,      quit,           {0} },
	{ 0, XF86XK_AudioMute,                     spawn,          {.v = volume_mute } },
        { 0, XF86XK_AudioRaiseVolume,              spawn,          {.v = volume_raise } },
        { 0, XF86XK_AudioLowerVolume,              spawn,          {.v = volume_lower } },
};

ResourcePref resources[] = {
	{"font",          STRING,   &font},
	{"font2",         STRING,   &font2},
	{"font",          STRING,   &dmenufont},
	{"borderpx",      INTEGER,  &borderpx},
	{"barheight",     INTEGER,  &barheight},
	{"gappx",         INTEGER,  &gappx},
	{"ulineall",      INTEGER,  &ulineall},
	{"ulinepx",       INTEGER,  &ulinestroke},
	{"topbar",        INTEGER,  &topbar},
	{"noborder",      INTEGER,  &noborder},
	{"background",    STRING,   &col_background},
	{"altbackground", STRING,   &col_altbackground},
	{"foreground",    STRING,   &col_foreground},
	{"altforeground", STRING,   &col_altforeground},
	{"highlight",     STRING,   &col_highlight},
	{"color0",        STRING,   &termcol0},
	{"color1",        STRING,   &termcol1},
	{"color2",        STRING,   &termcol2},
	{"color3",        STRING,   &termcol3},
	{"color4",        STRING,   &termcol4},
	{"color5",        STRING,   &termcol5},
	{"color6",        STRING,   &termcol6},
	{"color7",        STRING,   &termcol7},
	{"color8",        STRING,   &termcol8},
	{"color9",        STRING,   &termcol9},
	{"color10",       STRING,   &termcol10},
	{"color11",       STRING,   &termcol11},
	{"color12",       STRING,   &termcol12},
	{"color13",       STRING,   &termcol13},
	{"color14",       STRING,   &termcol14},
	{"color15",       STRING,   &termcol15},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
#define LeftClick Button1
#define RightClick Button3
#define MiddleClick Button2
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              LeftClick,      setlayout,      {0} },
	{ ClkLtSymbol,          0,              RightClick,     setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              MiddleClick,    zoom,           {0} },
	{ ClkStatusText,        0,              LeftClick,      sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              MiddleClick,    sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              RightClick,     sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         LeftClick,      movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         MiddleClick,    togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         RightClick,     resizemouse,    {0} },
	{ ClkTagBar,            0,              LeftClick,      view,           {0} },
	{ ClkTagBar,            0,              RightClick,     toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         LeftClick,      tag,            {0} },
	{ ClkTagBar,            MODKEY,         RightClick,     toggletag,      {0} },
};

