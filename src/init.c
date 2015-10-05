#include <mruby.h>
#include <mrgss/mrgss-sprite.h>

void
mrb_mrgss_sprite_gem_init(mrb_state *mrb) {
	mrgss_init_sprite(mrb);
}

void
mrb_mrgss_sprite_gem_final(mrb_state* mrb) {
    /* finalizer */
}