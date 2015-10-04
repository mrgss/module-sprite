#include <mruby.h>
#include <mrgss.h>
#include <mrgss/mrgss-texture.h>
#include <mrgss/mrgss-viewport.h>

/**
 * Sprite Object Constructor
 * @param mrb 
 * @param self
 * @return 
 */
static mrb_value
initialize(mrb_state *mrb, mrb_value self) {
    mrb_int x, y;
    mrb_value tex;
    mrb_value view;
    mrb_get_args(mrb, "iiooo", &x, &y, &tex, &view);
    if (mrgss_object_is_a(mrb, tex, "Texture") && mrgss_object_is_a(mrb, view, "Viewport")) {
        mrgss_iv_create(mrb, self, "@x", mrb_fixnum_value(x));
        mrgss_iv_create(mrb, self, "@y", mrb_fixnum_value(y));
        mrgss_iv_create(mrb, self, "@texture", tex);
        mrgss_iv_create(mrb, self, "@viewport", view);
    } else {
        mrgss_raise(mrb, E_ARGUMENT_ERROR, "wrong type of arguments");
        return mrb_nil_value();
    }

    return self;
}

/**
 * x getter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value get_x(mrb_state *mrb, mrb_value self) {
    return mrgss_iv_get(mrb, self, "@x");
}

/**
 * y getter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value get_y(mrb_state *mrb, mrb_value self) {
    return mrgss_iv_get(mrb, self, "@y");
}

/**
 * texture getter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value get_texture(mrb_state *mrb, mrb_value self) {
    return mrgss_iv_get(mrb, self, "@texture");
}

/**
 * viewport getter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value get_viewport(mrb_state *mrb, mrb_value self) {
    return mrgss_iv_get(mrb, self, "@viewport");
}

/**
 * rect getter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value get_rect(mrb_state *mrb, mrb_value self) {
    return mrgss_iv_get(mrb, self, "@src_rect");
}

/**
 * x setter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value set_x(mrb_state *mrb, mrb_value self) {
    mrb_int val;
    mrb_get_args(mrb, "i", &val);
    mrgss_iv_create(mrb, self, "@x", mrb_fixnum_value(val));
    return self;
}

/**
 * y setter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value set_y(mrb_state *mrb, mrb_value self) {
    mrb_int val;
    mrb_get_args(mrb, "i", &val);
    mrgss_iv_create(mrb, self, "@y", mrb_fixnum_value(val));
    return self;
}

/**
 * texture setter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value set_texture(mrb_state *mrb, mrb_value self) {
    mrb_value val;
    mrb_get_args(mrb, "o", &val);
    if (mrgss_object_is_a(mrb, val, "Texture")) {
        mrgss_iv_create(mrb, self, "@texture", val);
        return self;
    } else {
        mrgss_raise(mrb, E_ARGUMENT_ERROR, "Expecting a MRGSS::Texture");
        return mrb_nil_value();
    }

}

/**
 * viewport setter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value set_viewport(mrb_state *mrb, mrb_value self) {
    mrb_value val;
    mrb_get_args(mrb, "o", &val);
    if (mrgss_object_is_a(mrb, val, "Viewport")) {
        mrgss_iv_create(mrb, self, "@viewport", val);
        return self;
    } else {
        mrgss_raise(mrb, E_ARGUMENT_ERROR, "Expecting a MRGSS::Viewport");
        return mrb_nil_value();
    }

}

/**
 * rect setter
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value set_rect(mrb_state *mrb, mrb_value self) {
    mrb_value val;
    mrb_get_args(mrb, "o", &val);
    if (mrgss_object_is_a(mrb, val, "Rect")) {
        mrgss_iv_create(mrb, self, "@src_rect", val);
        return self;
    } else {
        mrgss_raise(mrb, E_ARGUMENT_ERROR, "Expecting a MRGSS::Rect");
        return mrb_nil_value();
    }

}

/**
 * Sprite initializer
 * @param mrb
 */
void mrgss_init_sprite(mrb_state *mrb) {
    struct RClass *type = mrgss_create_class(mrb, "Sprite");
    mrb_define_method(mrb, type, "initialize", initialize, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, type, "x", get_x, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "x=", set_x, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, type, "y", get_y, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "y=", set_y, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, type, "texture", get_texture, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "texture=", set_texture, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, type, "viewport", get_viewport, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "viewport=", set_viewport, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, type, "src_rect", get_rect, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "src_rect=", set_rect, MRB_ARGS_REQ(1));

}
