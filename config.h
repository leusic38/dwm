/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 1;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 1;       /* vert inner gap between windows */
static const unsigned int gappoh    = 1;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 1;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=12:antialias=true:autohint=true", "Inconsolata:size=10", "Symbola:size=10", "Twitter Color Emoji:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#004400";
static char selbgcolor[]            = "#004400";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm", "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     	 	instance    title       tags mask     isfloating   monitor */
	{ "Gimp",    		NULL,       NULL,       0,            1,           -1 },
	{ "Qalculate-gtk",   	NULL,       NULL,       0,            1,           -1 },
	{ "Alsamixer",     	NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  		NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Spotify",  		NULL,       NULL,       1 << 8,       0,           0 },
};

/* layout(s) */
static const float mfact     = 0.70707070707070; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layoutsgaps.c"
#include "swiftview.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "|M|",      centeredmaster },

 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
	{ "[D]",      deck },

	{ ">M>",      centeredfloatingmaster },
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        		function        argument */
	/* {MODKEY,			XK_Escape, 		spawn,		{} }*/
	/***********           grave use with stack actions         ***************/	
	TAGKEYS(                        XK_1,                   		0)
	TAGKEYS(                        XK_2,		                	1)
	TAGKEYS(                        XK_3,		                	2)
	TAGKEYS(                        XK_4,		    	        	3)
	TAGKEYS(                        XK_5,           			4)
	TAGKEYS(                        XK_6,                			5)
	TAGKEYS(                        XK_7,                			6)
	TAGKEYS(                        XK_8,                			7)
	TAGKEYS(                        XK_9,                			8)
	{ MODKEY,                       XK_0,      		view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      		tag,            {.ui = ~0 } },
	{ MODKEY|Mod1Mask,              XK_0,			togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,			defaultgaps,    {0} },
	{ MODKEY,		        XK_minus,		incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_minus,  		incrogaps,      {.i = -1 } },
	{ MODKEY|ShiftMask|ControlMask,	XK_minus,      		incrigaps,      {.i = -1 } },
	{ MODKEY,              		XK_equal,		incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_equal,		incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask|ControlMask, XK_equal,	      	incrigaps,      {.i = +1 } },
	{ MODKEY,			XK_BackSpace,  		killclient,	{0} },
	{ MODKEY|ShiftMask,		XK_BackSpace,  		quit,		{0} },
	{ MODKEY|Mod1Mask,		XK_BackSpace,  		spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Shutdown computer?\")\" = Yes ] && sudo -A shutdown -h now") },
	{ MODKEY|ControlMask,		XK_BackSpace,  		spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Reboot computer?\")\" = Yes ] && sudo -A reboot") },

	{ MODKEY,			XK_Tab,  		view,       	{0} },
	/* {MODKEY|ShiftMask,		XK_Tab,  		spawn,		{} }*/
	/***********           q use with stack actions 	        ***************/	
	{ MODKEY,			XK_w,			spawn,		SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,		XK_w,			spawn,		SHCMD("$BROWSER_2") },
	{ MODKEY|ControlMask|ShiftMask,	XK_w,			spawn,		SHCMD("$BROWSER_3") },
	{ MODKEY,			XK_e,			spawn,		SHCMD("$EDI") },
	{ MODKEY|ShiftMask,		XK_e,			spawn,		SHCMD("code") },
	/*{ MODKEY|ControlMask|ShiftMask,	XK_e,			spawn,		SHCMD("postman") },*/
	{ MODKEY,			XK_r,			spawn,		SHCMD("st -e lf") },
	{ MODKEY|ShiftMask,		XK_r,			spawn,		SHCMD("st -e lf /hdd") },
	{ MODKEY|ControlMask,			XK_r,			spawn,		SHCMD("st -e lf /development") },
	{ MODKEY|ControlMask|ShiftMask,		XK_r,			spawn,		SHCMD("st -e sudo lf /") },
	{ MODKEY,                       XK_t,      		setlayout,     	{.v = &layouts[0]} },
	{ MODKEY|ShiftMask,		XK_t,      		setlayout,     	{.v = &layouts[1]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_t,      		setlayout,     	{.v = &layouts[2]} },
	{ MODKEY,                       XK_y,      		setlayout,     	{.v = &layouts[3]}},
	{ MODKEY|ShiftMask,             XK_y,      		setlayout,     	{.v = &layouts[4]}},
	{ MODKEY|ControlMask|ShiftMask, XK_y,      		setlayout,     	{.v = &layouts[5]}},
	{ MODKEY,                       XK_u,      		setlayout,     	{.v = &layouts[6]} },
	{ MODKEY|ShiftMask,		XK_u,      		setlayout,     	{.v = &layouts[7]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_u,      		setlayout,     	{.v = &layouts[8]} },
	{ MODKEY,                       XK_i,			spawn,     	SHCMD("libreoffice") },
	{ MODKEY|ShiftMask,		XK_i,			spawn,		SHCMD("st -e sudo nmtui") },
	{ MODKEY,			XK_o,			spawn,     	SHCMD("st -e lf") },
	{ MODKEY|ShiftMask,		XK_o,			spawn,     	SHCMD("st -e lf /hdd") },
	{ MODKEY|ControlMask|ShiftMask, XK_o,			spawn,     	SHCMD("st -e lf /development") },
	{ MODKEY,			XK_p,			spawn,		SHCMD("postman") },
	{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("st -e cdpj") },
	/*{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("mpc seek -10") }, */
	/*{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("mpc seek -120") },*/
	/*{ MODKEY,			XK_bracketright,	spawn,		SHCMD("mpc seek +10") },*/
	/*{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("mpc seek +120") },*/
	{ MODKEY,			XK_backslash,		incnmaster,	{.i = +1} },
	{ MODKEY|ShiftMask,		XK_backslash,		incnmaster,	{.i = -1} },
	
	/***********           a use with stack actions         ***************/	
	{ MODKEY,			XK_s,			togglesticky,	{0} },
	{ MODKEY|ShiftMask,		XK_s,			togglescratch,  {.ui = 0 } },
	{ MODKEY|ShiftMask|ControlMask,		XK_s,			togglescratch,  {.ui = 1 } },
	{ MODKEY|ShiftMask|ALTKEY,		XK_s,			togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_d,      		spawn,         	SHCMD("dbeaver") },
	{ MODKEY|ShiftMask,		XK_d,      		spawn,         	{.v = dmenucmd } },
	{ MODKEY,			XK_f,			togglefullscr,	{0} },
	/* { MODKEY|ShiftMask,                       XK_f,      		setlayout,     	{.v = &layouts[1]} },*/
	{ MODKEY,			XK_g,			shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_g,			shifttag,	{.i = -1} },
	{ MODKEY,                       XK_h,      		setmfact,      	{.f = -0.05} },
	{ MODKEY|ShiftMask,		XK_h,      		setmfact,      	{.f = -0.15} },
	/***********           j and k use with stack actions         ***************/	
	{ MODKEY,                       XK_l,      		setmfact,      	{.f = +0.05} },
	{ MODKEY|ShiftMask,		XK_l,      		setmfact,      	{.f = +0.15} },
	/* { MODKEY,			XK_semicolon,		spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_semicolon,		spawn,		SHCMD("") }, */
	{ MODKEY,			XK_apostrophe,		spawn,		SHCMD("st -e man dwm") },
	/* { MODKEY|ShiftMask,		XK_apostrophe,		spawn,		SHCMD("") }, */
	{ MODKEY,		        XK_Return, 		spawn,         	{.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, 		spawn,         	{.v = termcmd } },

	/***********           z and x use with stack actions         ***************/	
	{ MODKEY,			XK_c,      		spawn	,    	SHCMD("discord") },
	{ MODKEY|ShiftMask,             XK_c,      		spawn,    	SHCMD("slack") },
	{ MODKEY|ControlMask,			XK_c,      		spawn	,    	SHCMD("teams") },
	{ MODKEY|ShiftMask|ControlMask,             XK_c,      		spawn,    	SHCMD("skype") },
	{ MODKEY|ALTKEY,			XK_c,      		spawn	,    	SHCMD("pkill -9 Discord") },
	{ MODKEY|ALTKEY|ShiftMask,             XK_c,      		spawn,    	SHCMD("pkill -9 slack") },
	{ MODKEY|ALTKEY|ControlMask,			XK_c,      		spawn	,    	SHCMD("pkill -9 teams") },
	{ MODKEY|ALTKEY|ShiftMask|ControlMask,             XK_c,      		spawn,    	SHCMD("pkill -9 skype") },
	{ MODKEY,			XK_v,			spawn,		SHCMD("vlc") },
	{ MODKEY|ShiftMask,		XK_v,			spawn,		SHCMD("twitch") },
	{ MODKEY,                       XK_b,      		togglebar,     	{0} },
	{ MODKEY|ShiftMask,		XK_b,      		togglebar,     	{0} },
	{ MODKEY,			XK_n,			spawn,		SHCMD("st -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY|ShiftMask,		XK_n,			spawn,		SHCMD("teams") },
	{ MODKEY|ControlMask|ShiftMask,	XK_n,			spawn,		SHCMD("pkill -9 teams") },
	{ MODKEY,                       XK_m,      		spawn,     	SHCMD("st -e man dwm") },
	{ MODKEY|ShiftMask,		XK_m,      		spawn,		SHCMD("spotify") },
	{ MODKEY,                       XK_comma,  		focusmon,      	{.i = -1 } },
	{ MODKEY,                       XK_period, 		focusmon,      	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  		tagmon,        	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 		tagmon,        	{.i = +1 } },
	{ MODKEY,			XK_slash,			spawn,		SHCMD("st -e sudo openvpn /home/manu/openVpnConfig/mably-connect-efarcis-config.ovpn") },
	{ MODKEY|ShiftMask,		XK_slash,			spawn,		SHCMD("st -e /usr/NX/bin/nxplayer") },
	{ MODKEY,			XK_space,		zoom,		{0} },
	{ MODKEY|ShiftMask,             XK_space,  		togglefloating,	{0} },
	{ MODKEY,			XK_Page_Up,		shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_Page_Up,		shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,		shiftview,	{ .i = 1 } },
	{ MODKEY|ShiftMask,		XK_Page_Down,		shifttag,	{ .i = 1 } },
	{ MODKEY,			XK_Insert,		spawn,		SHCMD("notify-send \"📋 Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },

	{ MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,	tagmon,		{.i = +1 } },

	/*{ MODKEY,			XK_F1,			spawn,		SHCMD("") },*/
	{ MODKEY,			XK_F2,			quit,		{0} },
	{ MODKEY,			XK_F3,			spawn,		SHCMD("displayselect") },
	{ MODKEY,			XK_F4,			spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Hibernate computer?\")\" = Yes ] && sudo -A zzz") },
	{ MODKEY,			XK_F5,			xrdb,		{.v = NULL } },
/*	{ MODKEY,			XK_F6,			spawn,		SHCMD("torwrap") },
	{ MODKEY,			XK_F7,			spawn,		SHCMD("td-toggle") },
	{ MODKEY,			XK_F8,			spawn,		SHCMD("mailsync") },
	{ MODKEY,			XK_F9,			spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F10,			spawn,		SHCMD("dmenuumount") },*/
	/* { MODKEY,			XK_F11,			spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_F12,			spawn,		SHCMD("") }, */

	{ 0,				XK_Print,		spawn,		SHCMD("maim /hdd/Images/screenshots/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			XK_Print,		spawn,		SHCMD("printscript") },
	/*{ MODKEY,			XK_Print,		spawn,		SHCMD("") },
	{ MODKEY|ShiftMask,		XK_Print,		spawn,		SHCMD("") },
	{ MODKEY,			XK_Delete,		spawn,		SHCMD("") },
	{ MODKEY,			XK_Scroll_Lock,		spawn,		SHCMD("killall screenkey || screenkey &") },*/

	{ 0,				XF86XK_AudioMute,		spawn,		SHCMD("amixer sset Master toggle ; pkill -RTMIN+10 dwmblocks") },
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,		SHCMD("amixer sset Master 5%+ ; pkill -RTMIN+10 dwmblocks") },
	{ 0,				XF86XK_AudioLowerVolume,	spawn,		SHCMD("amixer sset Master 5%- ; pkill -RTMIN+10 dwmblocks") },
	{ 0,				XF86XK_AudioPrev,		spawn,		SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Previous") },
	{ 0,				XF86XK_AudioNext,		spawn,		SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Next") },
	/*{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },*/
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.PlayPause") },
	/*{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD("st -e ncmpcpp") },
	{ 0, XF86XK_PowerOff,		spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Shutdown computer?\")\" = Yes ] && sudo -A shutdown -h now") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD("st -e bc -l") },
	{ 0, XF86XK_Sleep,		spawn,		SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Hibernate computer?\")\" = Yes ] && sudo -A zzz") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,		spawn,		SHCMD("st") },
	{ 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD("st -e htop") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD("st -e lf /") },*/
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	/*{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },*/
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              1,	        setlayout,      {0} },
	{ ClkLtSymbol,          0,              3,     		setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              2,        	zoom,           {0} },
	{ ClkStatusText,        0,              1,        	sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              2,	        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              3,	        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              4,	        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              5,	        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        0,              8,	        sigdwmblocks,   {.i = 8} },
	{ ClkStatusText,        0,              9,	        sigdwmblocks,   {.i = 9} },
	{ ClkClientWin,         MODKEY,         1,	        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         2,	        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         3,	        resizemouse,    {0} },
	{ ClkTagBar,            0,              1,	        view,           {0} },
	{ ClkTagBar,            0,              3,	        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         1,	        tag,            {0} },
	{ ClkTagBar,            MODKEY,         3,	        toggletag,      {0} },
};

