#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mrgss/mrgss.h>
#include <mrgss/mrgss_texture.h>
#include <mrgss/mrgss_shader.h>
#include <mrgss/mrgss_sprite.h>


static GLuint indices[6] = {0, 1, 2, 2, 3, 0};
static GLuint eab;
static mrb_bool eab_init = FALSE;

static void calculate_vertex_position(GLfloat *buffer, int x, int y, int w, int h) {
    //top left
    buffer[0] = (float) x;
    buffer[1] = (float) y;
    //top right
    buffer[2] = (float) (x + w);
    buffer[3] = (float) (y);
    // bottom left
    buffer[4] = (float) (x + w);
    buffer[5] = (float) (y + h);
    // bottom right
    buffer[6] = (float) (x);
    buffer[7] = (float) (y + h);
}

static void calculate_texture_coords(GLfloat *buffer, float rx, float ry, float rw, float rh, float mw, float mh) {
    //top left
    buffer[0] = rx / mw;
    buffer[1] = ry / mh;

    //top right
    buffer[2] = (rx + rw) / mw;
    buffer[3] = ry / mh;
    // bottom left
    buffer[4] = (rx + rw) / mw;
    buffer[5] = (ry + rh) / mh;
    // bottom right
    buffer[6] = rx / mw;
    buffer[7] = (ry + rh) / mh;

}

static void sprite_free(mrb_state *mrb, void *p) {
    if (p) {
        free(p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_sprite_data_type = {"Sprite", sprite_free};

static mrb_value initialize(mrb_state *mrb, mrb_value self) {
    mrgss_sprite *sprite;
    mrgss_texture *tex;
    mrb_value texture, viewport;
    mrb_get_args(mrb, "oo", &texture, &viewport);
    sprite = mrb_malloc(mrb, sizeof (mrgss_sprite));
    tex = DATA_PTR(texture);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@viewport"), viewport);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@texture"), texture);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@shader"), mrb_gv_get(mrb, mrb_intern_lit(mrb, "default_shader")));
    glGenVertexArrays(1, &sprite->vao);
    glGenBuffers(1, &sprite->vbo);

    sprite->z = 0;
    sprite->x = 0;
    sprite->y = 0;
    sprite->src_rect = mrb_malloc(mrb, sizeof (SDL_Rect));
    sprite->src_rect->x = 0;
    sprite->src_rect->y = 0;
    sprite->src_rect->h = tex->h;
    sprite->src_rect->w = tex->w;
    calculate_vertex_position(sprite->vertex_buff, 0, 0, tex->w, tex->h);
    calculate_texture_coords(sprite->texture_buff, 0, 0, tex->w, tex->h, tex->w, tex->h);
    glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof (sprite->vertex_buff) + sizeof (sprite->texture_buff), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, sprite->vertex_buff);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * 8, sizeof (GL_FLOAT) * 8, sprite->texture_buff);
    //mrb_ary_push(mrb, viewport, self);
    viewport_add_children(mrb, viewport, self);
    DATA_TYPE(self) = &mrbal_sprite_data_type;
    DATA_PTR(self) = sprite;
    return self;
}

static mrb_value get_x(mrb_state *mrb, mrb_value self) {
    mrgss_sprite *spr = DATA_PTR(self);
    return mrb_fixnum_value(spr->x);
}

static mrb_value set_x(mrb_state *mrb, mrb_value self) {
    mrb_int x;
    mrgss_sprite* spr;
    mrb_get_args(mrb, "i", &x);
    spr = DATA_PTR(self);
    spr->x = x;
    calculate_vertex_position(spr->vertex_buff, spr->x, spr->y, spr->src_rect->w, spr->src_rect->h);
    glBindBuffer(GL_ARRAY_BUFFER, spr->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertex_buff);
    return mrb_fixnum_value(spr->x);
}

static mrb_value get_y(mrb_state *mrb, mrb_value self) {
    mrgss_sprite *spr = DATA_PTR(self);
    return mrb_fixnum_value(spr->y);
}

static mrb_value set_y(mrb_state *mrb, mrb_value self) {
    mrb_int y;
    mrgss_sprite* spr;
    mrb_get_args(mrb, "i", &y);
    spr = DATA_PTR(self);
    spr->y = y;
    calculate_vertex_position(spr->vertex_buff, spr->x, spr->y, spr->src_rect->w, spr->src_rect->h);
    glBindBuffer(GL_ARRAY_BUFFER, spr->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertex_buff);
    return mrb_fixnum_value(spr->y);
}

static mrb_value get_rect(mrb_state *mrb, mrb_value self) {
    mrgss_sprite *spr;
    mrb_value params[4];
    struct RClass *type;
    spr = DATA_PTR(self);
    type = mrb_class_get_under(mrb, mrgss_module(), "Rect");
    params[0] = mrb_fixnum_value(spr->src_rect->x);
    params[1] = mrb_fixnum_value(spr->src_rect->y);
    params[2] = mrb_fixnum_value(spr->src_rect->w);
    params[3] = mrb_fixnum_value(spr->src_rect->h);
    return mrb_obj_new(mrb, type, 4, params);
}

static mrb_value set_rect(mrb_state *mrb, mrb_value self) {
    mrb_value rect;
    mrb_value texture;
    mrgss_texture *tex;
    mrgss_sprite *spr;
    mrb_get_args(mrb, "o", &rect);
    spr = DATA_PTR(self);
    spr->src_rect = DATA_PTR(rect);
    texture = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@texture"));
    tex = DATA_PTR(texture);
    calculate_vertex_position(spr->vertex_buff, spr->x, spr->y, spr->src_rect->w, spr->src_rect->h);
    calculate_texture_coords(spr->texture_buff, spr->src_rect->x, spr->src_rect->y, spr->src_rect->w, spr->src_rect->h, tex->w, tex->h);
    glBindBuffer(GL_ARRAY_BUFFER, spr->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertex_buff);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * 8, sizeof (GL_FLOAT) * 8, spr->texture_buff);
    return rect;
}

static mrb_value get_shader(mrb_state *mrb, mrb_value self){
    return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@shader"));
}

static mrb_value set_shader(mrb_state *mrb, mrb_value self){
    mrb_value nshader;
    mrb_get_args(mrb, "o", &nshader);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@shader"), nshader);
    return nshader;
}

static mrb_value get_texture(mrb_state *mrb, mrb_value self){
    return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@texture"));
}

static mrb_value set_texture(mrb_state *mrb, mrb_value self){
    mrb_value ntexture;
    mrb_get_args(mrb, "o", &ntexture);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@texture"), ntexture);
    return ntexture;
}

static mrb_value get_viewport(mrb_state *mrb, mrb_value self){
    return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@shader"));
}

static mrb_value set_viewport(mrb_state *mrb, mrb_value self){
    mrb_value nviewport;
    mrb_get_args(mrb, "o", &nviewport);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@viewport"), nviewport);
    return nviewport;
}

void mrgss_sprite_init(mrb_state *mrb) {
    struct RClass *sprite = mrb_define_class(mrb, "Sprite", mrb->object_class);
    mrb_define_method(mrb, sprite, "initialize", initialize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sprite, "x", get_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "x=", set_x, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "y", get_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "y=", set_y, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "src_rect", get_rect, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "src_rect=", set_rect, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "shader", get_shader, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "shader=", set_shader, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "texture", get_texture, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "texture=", set_texture, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite, "viewport", get_viewport, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite, "viewport=", set_viewport, MRB_ARGS_REQ(1));
    MRB_SET_INSTANCE_TT(sprite, MRB_TT_DATA);
}

static void bind_shader_attributes(GLuint shaderProgram, kmMat4* matrices) {
    GLint texture_coord_attribute;
    GLint model;
    GLint view;
    GLint projection;
    GLint position_attribute = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(position_attribute);
    texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
    glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof (GL_FLOAT) * 8));
    glEnableVertexAttribArray(texture_coord_attribute);
    model = glGetUniformLocation(shaderProgram, "Model");
    glUniformMatrix4fv(model, 1, GL_FALSE, matrices[0].mat);
    view = glGetUniformLocation(shaderProgram, "View");
    glUniformMatrix4fv(view, 1, GL_FALSE, matrices[1].mat);

    projection = glGetUniformLocation(shaderProgram, "Projection");
    glUniformMatrix4fv(projection, 1, GL_FALSE, matrices[2].mat);

}

void
draw_sprite(mrb_state *mrb, mrb_value sprite, kmMat4* matrices) {
    mrgss_sprite *spr;
    mrgss_texture *tex;
    mrgss_shader *sdr;
    if (!eab_init) {
        glGenBuffers(1, &eab);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);
        eab_init = TRUE;
    }
    spr = DATA_PTR(sprite);
    tex = DATA_PTR(mrb_iv_get(mrb, sprite, mrb_intern_lit(mrb, "@texture")));
    sdr = DATA_PTR(mrb_iv_get(mrb, sprite, mrb_intern_lit(mrb, "@shader")));
    glBindVertexArray(spr->vao);
    glBindBuffer(GL_ARRAY_BUFFER, spr->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
    glBindTexture(GL_TEXTURE_2D, tex->tid);
    bind_shader_attributes(sdr->sid, matrices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}