	.include	"data_def.s"

	.data
	
	gfx_data	_imp1_gfx,"imp1.rgb"
	gfx_data	_imp2_gfx,"imp2.rgb"
	gfx_data	_imp3_gfx,"imp3.rgb"
	gfx_data	_imp4_gfx,"imp4.rgb"
	gfx_data	_imp5_gfx,"imp5.rgb"
	gfx_data	_imp6_gfx,"imp6.rgb"
	gfx_data	_imp7_gfx,"imp7.rgb"

	animation_begin	_imp_animation
	animation_chunk	6,_imp1_gfx
	animation_chunk	6,_imp2_gfx
	animation_chunk	6,_imp3_gfx
	animation_chunk	6,_imp4_gfx
	animation_chunk	6,_imp5_gfx
	animation_chunk	6,_imp6_gfx
	animation_chunk	6,_imp7_gfx
	animation_end


