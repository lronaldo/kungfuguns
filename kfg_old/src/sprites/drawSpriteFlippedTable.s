;;-----------------------------LICENSE NOTICE------------------------------------
;;  This file is part of CPCtelera: An Amstrad CPC Game Engine 
;;  Copyright (C) 2015-2016 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
;;
;;  This program is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU Lesser General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  (at your option) any later version.
;;
;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;  GNU Lesser General Public License for more details.
;;
;;  You should have received a copy of the GNU Lesser General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;-------------------------------------------------------------------------------
.module cpct_sprites

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Function: drawSpriteFlipAlphaTable
;;
;;    Draws a sprite to video memory (or to a screen buffer), making use of a 
;; dual table for flipping and alpha masking. Table must be placed at 0x100
;;
;; C Definition:
;;    void <drawSpriteFlipAlphaTable> (const void* *psprite*, void* *pvideomem*, 
;;                                     <u8> *width*, <u8> *height*, <u8> flipped) __z88dk_callee;
;;
;; Input Parameters (6 bytes):
;;  (2B  BC) psprite    - Source Sprite Pointer
;;  (2B  DE) pvideomem  - Destination video memory pointer
;;  (1B IXL) width      - Sprite Width in *bytes* (>0) (Beware, *not* in pixels!)
;;  (1B IXH) height     - Sprite Height in bytes (>0)
;;  (2B  HL) pmasktable - Pointer to the aligned mask table used to create transparency
;;
;; Assembly call (Input parameters on registers):
;;    > call cpct_drawSpriteMaskedAlignedTable_asm
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Macros for undocumented opcodes
;; 
.macro ld__a_ixl
   .dw #0x7DDD  ;; Opcode for ld a, ixl
.endm

.macro dec__ixl
   .dw #0x2DDD  ;; Opcode for dec ixl
.endm

.macro dec__ixh
   .dw #0x25DD  ;; Opcode for dec ixh
.endm

.macro ld__ixl    Value 
   .db #0xDD, #0x2E, Value  ;; Opcode for ld ixl, Value
.endm

ld__l__pHL          = 0x6E  ;; Opcode for ld l, (hl)
dec__de             = 0x1B  ;; Opcode for dec de
inc__de             = 0x13  ;; Opcode for inc de
dec_inc_de_xor_bits = 0x08  ;; Difference bits between dec de and inc de
jump_no_reverse_off = dms_sprite_height_loop - dms_reversion_jump - 1


;;
;; C Definition:
;;    void <switchFlipForDrawSprite> ()
;;
_switchFlipForDrawSprite::
   ld    hl, #dms_byte_reversion ;; [3] HL points to reversion opcode
   ld     a, (hl)                ;; [2] A = reversion opcode
   xor    #ld__l__pHL            ;; [2] Switches between 0x6E and 0x00 (ld l, (hl) / NOP)
   ld  (hl), a                   ;; [2] Stores switched opcode into memory

   ld    hl, #dms_reversion_jump ;; [3] HL points to reversion jump offset
   ld     a, (hl)                ;; [2] A = reversion jump offset
   xor    #jump_no_reverse_off   ;; [2] Switches between 0x00 and offset (jump or not jump)
   ld  (hl), a                   ;; [2] Stores switched offset into memory

   ld    hl, #dms_byte_advance   ;; [3] HL points to byte advancing/retroceding opcode
   ld     a, (hl)                ;; [2] A = opcode for advancing/retroceding
   xor    #dec_inc_de_xor_bits   ;; [2] Switches between 0x1B and 0x13 (dec de / inc de)
   ld  (hl), a                   ;; [2] Stores switched opcode into memory

   ret                           ;; [3] ret

;;
;; C Definition:
;;    void <setFlipForDrawSprite> ();
;; 
;;
;_setFlipForDrawSprite::
;   ld     a, #ld__l__pHL       ;; [2] A = 0x6E (Opcode for ld l, (hl))
;   ld  (dms_byte_reversion), a ;; [5] Set reverse operation to ld l, (hl) (reverse ON)
;   xor    a                    ;; [1] A = 0
;   ld  (dms_reversion_jump), a ;; [5] Set reversion jump to jr 0 (reverse ON)
;   ld     a, #dec__de          ;; [2] A = 0x1B (Opcode for dec DE)
;   ld  (dms_byte_advance), a   ;; [5] Set reverse traversing video memory (reverse ON)
;   ret                         ;; [3] ret
;
;;;
;;; C Definition:
;;;    void <setNoFlipForDrawSprite> ();
;;; 
;;;
;_setNoFlipForDrawSprite::
;   xor    a                       ;; [2] A = 0x00 (Opcode for NOP)
;   ld  (dms_byte_reversion), a    ;; [5] Set reverse operation to NOP (reverse OFF)
;   ld     a, #jump_no_reverse_off ;; [1] A = Offset to jump over addition to point to the end of the line
;   ld  (dms_reversion_jump), a    ;; [5] Set jump to prevent reversing traversal (reverse OFF)
;   ld     a, #inc__de             ;; [2] A = 0x1B (Opcode for dec DE)
;   ld  (dms_byte_advance), a      ;; [5] Set reversion jump to jr 0 (reverse ON)
;   ret                            ;; [3] Set reverse traversing video memory (reverse ON)

;;
;; C bindings for <drawSpriteFlipAlphaTable>
;;
_drawSpriteFlipAlphaTable::

   ld (dms_restore_ix + 2), ix  ;; [6] Save IX to restore it before returning
   pop   ix         ;; [5] HL = Return Address
   pop   bc         ;; [3] BC = Pointer to the Sprite data
   pop   de         ;; [3] DE = Pointer to the place in video memory where sprite will be drawn
   ex   (sp), ix    ;; [7] IX = width (IXL) and height (IXH) of the sprite in bytes, 
   ld    hl, #0x100 ;; [3] HL = Reversion + transparency table

   ;; Save Width in a placeholder for easy recovering it later
   ld__a_ixl               ;; [2] A = IXL = width of the sprite
   ld (restore_ixl + 2), a ;; [4] Save IXL (widht of the sprite) in a placeholder for recovering it later

dms_reversion_jump = .+1
   jr dms_sprite_height_loop ;; [3] Jump no to draw the sprite backwards

   ;; Point to the end of the sprite line that must be draw on the screen
   dec    a        ;; [1] A-- (width - 1)
   add    e        ;; [1] DE += A 
   ld     e, a     ;; [1]    DE points to the last byte of the screen line
   adc    d        ;; [1]    where this sprite line is being drawn (to draw
   sub    e        ;; [1]    it backwards)
   ld     d, a     ;; [1]

dms_sprite_height_loop:
   push de         ;; [4] Save DE for later use (jump to next screen line)

dms_sprite_width_loop:
   ld    a, (bc)   ;; [2] Get next byte from the sprite
   ld    l, a      ;; [1] Access reversion table element
dms_byte_reversion:
   nop             ;; [2] Reverse byte (requires ld l, (hl) to revert)
   inc   h         ;; [1] Point to mask table
   ld    a, (de)   ;; [2] Get the value of the byte of the screen where we are going to draw
   and (hl)        ;; [2] Erase background part that is to be overwritten (Mask step 1)
   or    l         ;; [1] Add up background and sprite information in one byte (Mask step 2)
   ld  (de), a     ;; [2] Save modified background + sprite data information into memory
   dec   h         ;; [1] HL points again to inversion table
dms_byte_advance:
   inc  de         ;; [2] Next bytes (sprite and memory)
   inc  bc         ;; [2] Next byte from the sprite 

   dec__ixl        ;; [2] IXL holds sprite width, we decrease it to count pixels in this line.
   jr   nz,dms_sprite_width_loop;; [2/3] While not 0, we are still painting this sprite line 
                                ;;      - When 0, we have to jump to next pixel line

   pop  de         ;; [3] Recover DE from stack. We use it to calculate start of next pixel line on screen
                   ;;  ... It must be restored before exiting the function, or it will be taken as return address

   dec__ixh        ;; [2] IXH holds sprite height. We decrease it to count another pixel line finished
   jr    z,dms_sprite_copy_ended;; [2/3] If 0, we have finished the last sprite line.
                                ;;      - If not 0, we have to move pointers to the next pixel line
restore_ixl:
   ld__ixl #00     ;; [3] IXL = Restore IXL to the width of the sprite (00 is a placeholder)

   ld    a, d      ;; [1] Start of next pixel line normally is 0x0800 bytes away.
   add   #0x08     ;; [2]    so we add it to DE (just by adding 0x08 to D)
   ld    d, a      ;; [1]
   and   #0x38     ;; [2] We check if we have crossed memory boundary (every 8 pixel lines)
   jr   nz, dms_sprite_height_loop ;; [2/3]  by checking the 4 bits that identify present memory line. 
                                   ;; ....  If 0, we have crossed boundaries

dms_sprite_8bit_boundary_crossed:
   ld    a, e      ;; [1] DE = DE + 0xC050h
   add   #0x50     ;; [2] -- Relocate DE pointer to the start of the next pixel line:
   ld    e, a      ;; [1] -- DE is moved forward 3 memory banks plus 50 bytes (4000h * 3) 
   ld    a, d      ;; [1] -- which effectively is the same as moving it 1 bank backwards and then
   adc   #0xC0     ;; [2] -- 50 bytes forwards (which is what we want to move it to the next pixel line)
   ld    d, a      ;; [1] -- Calculations are made with 8 bit maths as it is faster than other alternatives here

   jr  dms_sprite_height_loop ;; [3] Jump to continue with next pixel line

dms_sprite_copy_ended:

dms_restore_ix:
   ld   ix, #0000  ;; [4] Restore IX before returning
   ret             ;; [3] Return to caller
