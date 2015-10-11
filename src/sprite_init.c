#include <mruby.h>
#include <mrgss/mrgss_sprite.h>

void
mrb_mrgss_sprite_gem_init(mrb_state *mrb) {
	mrgss_sprite_init(mrb);
}

void
mrb_mrgss_sprite_gem_final(mrb_state* mrb) {
    /* finalizer */
}
