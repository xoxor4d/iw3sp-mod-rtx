#ifndef	INFO_BOX_ORIENT
	#define INFO_BOX_TITLE_HEIGHT		20
	#define INFO_BOX_WIDTH				(CHOICE_SIZE_X-(INFO_BOX_TITLE_HEIGHT/4))
	#define INFO_BOX_HEIGHT				126
	#define INFO_BOX_X					2
	#define INFO_BOX_Y					270
	#define ORIGIN_INFO_BOX_TITLE		INFO_BOX_X INFO_BOX_Y
#endif

#define INFO_BOX_WIDTH2				(INFO_BOX_WIDTH+5)
#define ORIGIN_INFO_BOX				INFO_BOX_X (INFO_BOX_Y+5)
#define ORIGIN_INFO_BOX_FRAME		INFO_BOX_X INFO_BOX_Y

#ifndef	INFO_TEXT 				
#define INFO_TEXT				dvarString( "motd" )
#endif

#ifndef INFO_TITLE
#define INFO_TITLE				"Message of the Day"
#endif

#ifndef INFO_TITLE_COLOR
#define INFO_TITLE_COLOR		1 1 1 0.7
#endif

#ifndef INFO_TEXT_COLOR
#define INFO_TEXT_COLOR			1 1 1 0.5
#endif

#undef PREPROC_TEXT_DRAW_ALIGNED_EXP
#define PREPROC_TEXT_DRAW_ALIGNED_EXP( prect, porigin, ptext, psize, palignx, paligny, palign, pcolor ) \
itemDef { \
	rect			prect \
	origin 			porigin \
	textalignx		palignx textaligny paligny \
	textfont 		UI_FONT_BIG \
	textalign		palign \
	textscale		psize \
	forecolor		pcolor \
	exp				text( ptext ); \
	visible			1 decoration autowrapped }

#undef PREPROC_SHADER_DRAW_ALIGNED_VIS
#define PREPROC_SHADER_DRAW_ALIGNED_VIS( prect, porigin, pshader, pcolor, pborder, pbordersize, pbordercolor, visArg ) \
itemDef { \
	style			WINDOW_STYLE_SHADER \
	rect			prect \
	origin 			porigin \
	forecolor		pcolor \
	exp				material( pshader ); \
	border			pborder \
	bordersize		pbordersize \
	bordercolor		pbordercolor \
	ownerdraw		198 \
	visible			visArg decoration }	

// size version 1: 248 126

//#define PREPROC_SHADER_DRAW_ALIGNED( prect, porigin, pshader, pcolor, pborder, pbordersize, pbordercolor )

PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME  INFO_BOX_WIDTH2 (INFO_BOX_HEIGHT-5) HORIZONTAL_ALIGN_CENTER VERTICAL_ALIGN_TOP, 0 -20, "white", 0 0 0 0.75, 1.5, 1, 0.5 0.5 0.5 0.5 )
//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH2 (INFO_BOX_HEIGHT-20) CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 0, "white", 0 0 0 0.3, 0, 0, 0 0 0 0 )
//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH 20 CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 -20, "gradient_fadein", 0.9 0.95 1 0.4, 0, 0, 0 0 0 0 )

//UI_EDGE( ORIGIN_INFO_BOX_FRAME, CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, INFO_BOX_WIDTH, -20, 0 0 0 0.3 )
//UI_EDGE( ORIGIN_INFO_BOX_FRAME, CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, INFO_BOX_WIDTH, -20, 0.9 0.95 1 0.4 )

//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH 1 CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 -20, "gradient_fadein", 0.9 0.95 1 0.5, 0, 0, 0 0 0 0 )
//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH2 6 CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 100, "gradient_fadein",0.9 0.95 1 0.35, 0, 0, 0 0 0 0 )
//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH 1 CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 100, "gradient_fadein",0.9 0.95 1 0.5, 0, 0, 0 0 0 0 )
//PREPROC_SHADER_DRAW_ALIGNED( ORIGIN_INFO_BOX_FRAME 80 20 CHOICE_HORIZONTAL_ALIGN VERTICAL_ALIGN_TOP, 0 -20, "ui_perforation", 1 1 1 1, 0, 0, 0 0 0 0 )


PREPROC_TEXT_DRAW_ALIGNED_EXP( ORIGIN_INFO_BOX (INFO_BOX_WIDTH-4) 100 HORIZONTAL_ALIGN_CENTER VERTICAL_ALIGN_TOP, 5 10, INFO_TEXT, 0.4, 0, -14, ITEM_ALIGN_LEFT, INFO_TEXT_COLOR )
//PREPROC_TEXT_DRAW_ALIGNED_EXP( ORIGIN_INFO_BOX_FRAME INFO_BOX_WIDTH 0 HORIZONTAL_ALIGN_CENTER VERTICAL_ALIGN_TOP, -6 -3, INFO_TITLE, 0.4, 0, 0, ITEM_ALIGN_RIGHT, INFO_TITLE_COLOR )
