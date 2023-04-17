## Default values
#$(eval $(call IMG2SP, SET_MODE        , 0                  ))  { 0, 1, 2 }
#$(eval $(call IMG2SP, SET_MASK        , none               ))  { interlaced, none }
#$(eval $(call IMG2SP, SET_EXTRAPAR    ,                    ))
#$(eval $(call IMG2SP, SET_IMG_FORMAT  , sprites            ))	{ sprites, zgtiles, screen }
#$(eval $(call IMG2SP, SET_OUTPUT      , c                  ))  { bin, c }

PALETTE=8 0 18 22 1 2 3 20 6 17 24 15 9 4 13 26
$(eval $(call IMG2SP, SET_PALETTE_FW  , $(PALETTE) ))
$(eval $(call IMG2SP, SET_FOLDER      , src/assets))
$(eval $(call IMG2SP, CONVERT_PALETTE , $(PALETTE), g_palette ))
$(eval $(call IMG2SP, CONVERT         , assets/princess.png, 12, 28, sp_princess))
$(eval $(call IMG2SP, CONVERT         , assets/agent1.png, 12, 28, sp_enemy_china))

