/* See LICENSE file for copyright and license details. */


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 0;        /* snap pixel */
static const unsigned int tag_space = 8;        /* spacing between tags */
static const unsigned int status_space = 8;     /* spacing between status items */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 0;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "DejaVu Sans:size=10", "Fira Code:size=10.5", "Symbols Nerd Font:size=10.7" };
static const char dmenufont[]       = "Fira Code:size=10";
static const char col_gray1[]       = "#a6aab2";
static const char col_gray2[]       = "#434c5e";
static const char col_gray3[]       = "#3b4252";
static const char col_white[]       = "#e5e9f0";
static const char col_red[]         = "#bf6161";
static const char col_orange[]      = "#d08770";
static const char col_yellow[]      = "#ebcb8b";
static const char col_green[]       = "#a3be8c";
static const char col_magenta[]     = "#b48ead";
static const char col_blue1[]       = "#5e81ac";
static const char col_blue2[]       = "#81a1c1";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]    = { col_white, col_gray3, col_gray3 },
	[SchemeSel]     = { col_white, col_red  , col_red   },
	[SchemeHid]     = { col_red,   col_gray3, col_gray3 },
	[SchemeLayout]  = { col_blue1, col_gray3, col_gray3 },
    [TagSchemeNorm] = { col_gray1, col_gray3, col_gray3 },
};

static const char *tag_colors[][3]  = {
	/* fg          bg         border   */
    { col_red,     col_gray3, col_gray3 },
    { col_orange,  col_gray3, col_gray3 },
    { col_yellow,  col_gray3, col_gray3 },
    { col_green,   col_gray3, col_gray3 },
    { col_magenta, col_gray3, col_gray3 },
    { col_red,     col_gray3, col_gray3 },
    { col_orange,  col_gray3, col_gray3 },
    { col_yellow,  col_gray3, col_gray3 },
    { col_green,   col_gray3, col_gray3 },
    { col_magenta, col_gray3, col_gray3 }
};

static const char *status_colors[][3]  = {
	/* fg          bg         border   */
    { col_red,     col_gray3, col_gray3 },
    { col_orange,  col_gray3, col_gray3 },
    { col_yellow,  col_gray3, col_gray3 },
    { col_green,   col_gray3, col_gray3 },
    { col_magenta, col_gray3, col_gray3 },
    { col_red,     col_gray3, col_gray3 },
    { col_orange,  col_gray3, col_gray3 },
    { col_yellow,  col_gray3, col_gray3 },
    { col_green,   col_gray3, col_gray3 },
    { col_magenta, col_gray3, col_gray3 }
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { NULL,      NULL,     NULL,           0,         0,          0,           1,        -1 }, /* by default no swallowing */
	{ "Gimp",    NULL,     NULL,           1 << 7,         0,          0,           0,        -1 },
	{ "firefox", NULL,     NULL,           1 << 2,    0,          0,          -1,        -1 },
	{ "st-256color", NULL, NULL,           0,         0,          1,           0,        -1 },
    /* swallow */
    { "mpv",     NULL,     NULL,           0,         0,          0,           0,        -1 },
    { "Sxiv",    NULL,     NULL,           0,         0,          0,           0,        -1 },
    { "pcmanfm", NULL,     NULL,        0,         0,          0,           0,        -1 },
    { "Zathura", NULL,     NULL,        0,         0,          0,           0,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
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
#define NOSHCMD(...) { .v = (const char*[]){ __VA_ARGS__, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-F", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray2, "-nf", col_white, "-sb", col_red, "-sf", col_white, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstackvis,  {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstackvis,  {.i = -1 } },
	{ MODKEY|ControlMask,           XK_j,      focusstackhid,  {.i = +1 } },
	{ MODKEY|ControlMask,           XK_k,      focusstackhid,  {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
    { MODKEY,                       XK_x,      spawn,          NOSHCMD("xkill") },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_n,      show,           {0} },
	{ MODKEY,                       XK_n,      hide,           {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 

    //Applications
    { MODKEY,                       XK_w,      spawn,          NOSHCMD("firefox") },
    { 0,         XF86XK_AudioLowerVolume,      spawn,          NOSHCMD("volnotify", "-5%") },
    { 0,         XF86XK_AudioRaiseVolume,      spawn,          NOSHCMD("volnotify", "+5%") },
    { 0,                XF86XK_AudioMute,      spawn,          NOSHCMD("volnotify", "mute") },
    { 0,                XF86XK_AudioPlay,      spawn,          NOSHCMD("playerctl", "play-pause") },
    { 0,                XF86XK_AudioPrev,      spawn,          NOSHCMD("playerctl", "previous") },
    { 0,                XF86XK_AudioNext,      spawn,          NOSHCMD("playerctl", "next") }
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

