#ifndef RENDER_FUNCS_H
#define RENDER_FUNCS_H

#include <iostream>
#include <MinHook/MinHook.h>
#include "common.h"
#include "RenderWare.h"
#include <audio/bass.h>
#include <audio/bass_fx.h>

using RpLightSetColor_t = RpLight*(__cdecl*)(RpLight* light, const RwColorFloat* color);

RpWorld* __cdecl SetLightColor_HK(RpLight* light, const RwColorFloat* color);
void world_add_light_hook(void);
void change_color_value(RwColorFloat newValue);
BOOL music_worker(const char *url);
BOOL bass_free();

#endif // RENDER_FUNCS_H