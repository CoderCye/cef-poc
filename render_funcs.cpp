#include "render_funcs.h"

RpLightSetColor_t origSetLightColor = nullptr;

RwColorFloat c{ 255.0, 255.0, 255.0, 0.0 };

RpWorld* __cdecl SetLightColor_HK(RpLight* light, const RwColorFloat* color)
{
	static bool first = false;
	if (!first)
	{
		std::cout << "call AddLightWorld_HK() AT " << __TIME__ << std::endl;
		first = true;
	}
	return origSetLightColor(light, &c);
}

void world_add_light_hook(void)
{
	std::cout << "call world_add_light_hook() AT " << __TIME__ << std::endl;
	MH_CreateAndEnableHook(reinterpret_cast<LPVOID>(0x00751A90), &SetLightColor_HK, reinterpret_cast<LPVOID*>(&origSetLightColor));
}

void change_color_value(RwColorFloat newValue)
{
	c.a = newValue.a;
	c.r = newValue.r;
	c.g = newValue.g;
	c.g = newValue.b;
}

BOOL music_worker(const char* url)
{

	std::string_view url_t = url;
	
	HSTREAM stream = BASS_StreamCreateURL(url_t.data(), NULL, 0, NULL, NULL);
	if (stream == NULL)
	{
		int err = BASS_ErrorGetCode();
		std::cout << "bass.dll error code[0]: " << err << std::endl;
	}
	
	BASS_ChannelSetAttribute(stream, BASS_ATTRIB_VOL, 0.6f);
	//BASS_ChannelSetFX(stream, BASS_FX_BFX_FREEVERB, 0);
	//BASS_FX_TempoCreate(stream, NULL);
	
	if (HIWORD(BASS_FX_GetVersion()) != BASSVERSION)
	{
		std::cout << "incorrect version bass_fx loaded" << std::endl;
	}

	HFX hFx = BASS_ChannelSetFX(stream, BASS_FX_BFX_ECHO, 1);
	if (hFx == NULL)
	{
		int err = BASS_ErrorGetCode();
		std::cout << "bass.dll error code[1]: " << err << std::endl;
	}
	
	// test
	BASS_BFX_ECHO bfx_echo_params;
	
	bfx_echo_params.fLevel = 1;
	bfx_echo_params.lDelay = 10;
	

	if (!BASS_FXSetParameters(hFx, &bfx_echo_params))
	{
		int err = BASS_ErrorGetCode();
		std::cout << "bass.dll error code[2]: " << err << std::endl;
	}
	BASS_ChannelPlay(stream, false);

	return TRUE;
}

BOOL bass_free()
{
	if (BASS_Free())
		return TRUE;
	return FALSE;
}
