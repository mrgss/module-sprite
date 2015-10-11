/* 
 * File:   mrgss_sprite.h
 * Author: manuel
 *
 * Created on 9 de octubre de 2015, 00:18
 */

#ifndef MRGSS_SPRITE_H
#define	MRGSS_SPRITE_H
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <mruby.h>
#include <SDL2/SDL.h>
#include <kazmath/kazmath.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    void mrgss_sprite_init(mrb_state *mrb);
    typedef struct sprite {
        GLfloat vertex_buff[8];
        GLfloat texture_buff[8];
        GLuint vao;
        GLuint vbo;
        mrb_int x, y, z;
        SDL_Rect *src_rect;
        
    } mrgss_sprite;



#ifdef	__cplusplus
}
#endif

#endif	/* MRGSS_SPRITE_H */

