/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <sstream>
#include <utility>
#include "game_map.h"
#include "input.h"
#include "text.h"
#include "window_settings.h"
#include "game_config.h"
#include "input_buttons.h"
#include "keys.h"
#include "output.h"
#include "baseui.h"
#include "bitmap.h"
#include "player.h"
#include "system.h"
#include "audio.h"


Window_Settings::Window_Settings(int ix, int iy, int iwidth, int iheight) :
	Window_Selectable(ix, iy, iwidth, iheight) {
	column_max = 1;
}

void Window_Settings::UpdateMode() {
	auto& frame = GetFrame();
	auto mode = frame.uimode;
}

void Window_Settings::DrawOption(int index) {
	Rect rect = GetItemRect(index);
	contents->ClearRect(rect);

	auto& option = options[index];

	bool enabled = bool(option.action);
	Font::SystemColor color = enabled ? Font::ColorDefault : Font::ColorDisabled;

	contents->TextDraw(rect, color, option.text);
	contents->TextDraw(rect, color, option.value_text, Text::AlignRight);
}

Window_Settings::StackFrame& Window_Settings::GetFrame(int n) {
	auto i = stack_index - n;
	assert(i >= 0 && i < static_cast<int>(stack.size()));
	return stack[i];
}

const Window_Settings::StackFrame& Window_Settings::GetFrame(int n) const {
	auto i = stack_index - n;
	assert(i >= 0 && i < static_cast<int>(stack.size()));
	return stack[i];
}

int Window_Settings::GetStackSize() const {
	return stack_index + 1;
}

void Window_Settings::Push(UiMode ui, int arg) {
	SavePosition();

	++stack_index;
	assert(stack_index < static_cast<int>(stack.size()));
	stack[stack_index] = { ui, arg, 0, 0};

	Refresh();
	RestorePosition();
}

void Window_Settings::Pop() {
	SavePosition();
	--stack_index;
	assert(stack_index >= 0);

	Refresh();
	RestorePosition();
}

void Window_Settings::SavePosition() {
	auto mode = GetFrame().uimode;
	if (mode != eNone) {
		auto& mem = memory[mode - 1];
		mem.index = index;
		mem.top_row = GetTopRow();
	}
}

void Window_Settings::RestorePosition() {
	auto mode = GetFrame().uimode;
	if (mode != eNone) {
		auto& mem = memory[mode - 1];
		index = mem.index;
		SetTopRow(mem.top_row);
	}
}

Window_Settings::UiMode Window_Settings::GetMode() const {
	return GetFrame().uimode;
}

void Window_Settings::Refresh() {
	options.clear();

	switch (GetFrame().uimode) {
		case eNone:
		case eMain:
			break;
		case eInput:
			RefreshInput();
			break;
		case eVideo:
			RefreshVideo();
			break;
		case eAudio:
			RefreshAudio();
			break;
		case eLicense:
			RefreshLicense();
			break;
		case eInputListButtons:
			RefreshInputMapping();
			break;
	}

	item_max = options.size();

	CreateContents();

	if (GetFrame().uimode == eNone || options.empty()) {
		SetIndex(-1);
	} else {
		SetIndex(index);
	}

	contents->Clear();

	for (int i = 0; i < item_max; ++i) {
		DrawOption(i);
	}
}

void Window_Settings::UpdateHelp() {
	if (index >= 0 && index < static_cast<int>(options.size())) {
		help_window->SetText(options[index].help);
	} else {
		help_window->SetText("");
	}
}

template <typename Param, typename Action>
void Window_Settings::AddOption(const Param& param,
	Action&& action)
{
	if (!param.IsOptionVisible()) {
		return;
	}
	Option opt;
	opt.text = ToString(param.GetName());
	opt.help = ToString(param.GetDescription());
	opt.value_text = param.ValueToString();
	opt.mode = eOptionNone;
	if (!param.IsLocked()) {
		opt.action = std::forward<Action>(action);
	}
	options.push_back(std::move(opt));
}

template <typename T, typename Action>
void Window_Settings::AddOption(const RangeConfigParam<T>& param,
		Action&& action
	) {
	if (!param.IsOptionVisible()) {
		return;
	}
	Option opt;
	opt.text = ToString(param.GetName());
	opt.help = ToString(param.GetDescription());
	opt.value_text = param.ValueToString();
	opt.mode = eOptionRangeInput;
	opt.current_value = static_cast<int>(param.Get());
	opt.original_value = opt.current_value;
	opt.min_value = param.GetMin();
	opt.max_value = param.GetMax();
	if (!param.IsLocked()) {
		opt.action = std::forward<Action>(action);
	}
	options.push_back(std::move(opt));
}

template <typename T, typename Action, size_t S>
void Window_Settings::AddOption(const EnumConfigParam<T, S>& param,
			Action&& action
	) {
	if (!param.IsOptionVisible()) {
		return;
	}
	Option opt;
	opt.text = ToString(param.GetName());
	opt.help = ToString(param.GetDescription());
	opt.value_text = param.ValueToString();
	opt.mode = eOptionPicker;
	opt.current_value = static_cast<int>(param.Get());
	opt.original_value = opt.current_value;
	int idx = 0;
	for (auto& s: param.GetValues()) {
		if (param.IsValid(static_cast<T>(idx))) {
			opt.options_text.push_back(ToString(s));
			opt.options_index.push_back(idx);
		}
		++idx;
	}

	idx = 0;
	for (auto& s: param.GetDescriptions()) {
		if (param.IsValid(static_cast<T>(idx))) {
			opt.options_help.push_back(ToString(s));
		}
		++idx;
	}
	if (!param.IsLocked()) {
		opt.action = std::forward<Action>(action);
	}
	options.push_back(std::move(opt));
}

void Window_Settings::RefreshVideo() {
	auto cfg = DisplayUi->GetConfig();

	AddOption(cfg.renderer,	[](){});
	AddOption(cfg.fullscreen, [](){ DisplayUi->ToggleFullscreen(); });
	AddOption(cfg.window_zoom, [](){ DisplayUi->ToggleZoom(); });
	AddOption(cfg.vsync, [](){ DisplayUi->ToggleVsync(); });
	AddOption(cfg.fps_limit, [this](){ DisplayUi->SetFrameLimit(GetCurrentOption().current_value); });
	AddOption(cfg.show_fps, [](){ DisplayUi->ToggleShowFps(); });
	AddOption(cfg.fps_render_window, [](){ DisplayUi->ToggleShowFpsOnTitle(); });
	AddOption(cfg.stretch, []() { DisplayUi->ToggleStretch(); });
	AddOption(cfg.scaling_mode, [this](){ DisplayUi->SetScalingMode(static_cast<ScalingMode>(GetCurrentOption().current_value)); });
	AddOption(cfg.touch_ui, [](){ DisplayUi->ToggleTouchUi(); });
}

void Window_Settings::RefreshAudio() {
	auto cfg = DisplayUi->GetAudio().GetConfig();

	AddOption(cfg.music_volume, [this](){ DisplayUi->GetAudio().BGM_SetGlobalVolume(GetCurrentOption().current_value); });
	AddOption(cfg.sound_volume, [this](){ DisplayUi->GetAudio().SE_SetGlobalVolume(GetCurrentOption().current_value); });
	/*AddOption("Midi Backend", LockedConfigParam<std::string>("Unknown"), "",
			[](){},
			"Which MIDI backend to use");
	AddOption("Midi Soundfont", LockedConfigParam<std::string>("Default"), "",
			[](){},
			"Which MIDI soundfont to use");*/
}

void Window_Settings::RefreshLicense() {
	AddOption(ConfigParam<std::string>("EasyRPG Player", "The engine you are using :)", "GPLv3+"), [](){});
	AddOption(ConfigParam<std::string>("liblcf", "Handles RPG Maker 2000/2003 and EasyRPG projects", "MIT"), [](){});
	AddOption(ConfigParam<std::string>("libpng", "For reading and writing PNG image files", "zlib"), [](){});
	AddOption(ConfigParam<std::string>("zlib", "Implements deflate used in ZIP archives and PNG images", "zlib"), [](){});
	AddOption(ConfigParam<std::string>("Pixman", "Pixel-manipulation library", "MIT"), [](){});
	AddOption(ConfigParam<std::string>("fmtlib", "Text formatting library", "BSD"), [](){});
	// No way to detect them - Used by liblcf
	AddOption(ConfigParam<std::string>("expat", "XML parser", "MIT"), [](){});
	AddOption(ConfigParam<std::string>("ICU", "Unicode library", "ICU"), [](){});
#if USE_SDL == 1
	AddOption(ConfigParam<std::string>("SDL", "Abstraction layer for graphic, audio, input and more", "LGPLv2.1+"), [](){});
#endif
#if USE_SDL == 2
	AddOption(ConfigParam<std::string>("SDL2", "Abstraction layer for graphic, audio, input and more", "zlib"), [](){});
#endif
#ifdef HAVE_FREETYPE
	AddOption(ConfigParam<std::string>("Freetype", "Font parsing and rasterization library", "Freetype"), [](){});
#endif
#ifdef HAVE_HARFBUZZ
	AddOption(ConfigParam<std::string>("Harfbuzz", "Text shaping engine", "MIT"), [](){});
#endif
#ifdef SUPPORT_AUDIO
	// Always shown because the Midi synth is compiled in
	AddOption(ConfigParam<std::string>("FmMidi", "MIDI file parser and Yamaha YM2608 FM synthesizer", "BSD"), [](){});
#ifdef HAVE_LIBMPG123
	AddOption(ConfigParam<std::string>("mpg123", "Decodes MPEG Audio Layer 1, 2 and 3", "LGPLv2.1+"), [](){});
#endif
#ifdef HAVE_LIBSNDFILE
	AddOption(ConfigParam<std::string>("libsndfile", "Decodes sampled audio data (WAV)", "LGPLv2.1+"), [](){});
#endif
#ifdef HAVE_OGGVORBIS
	AddOption(ConfigParam<std::string>("ogg", "Ogg container format library", "BSD"), [](){});
	AddOption(ConfigParam<std::string>("vorbis", "Decodes the free Ogg Vorbis audio codec", "BSD"), [](){});
#endif
#ifdef HAVE_TREMOR
	AddOption(ConfigParam<std::string>("tremor", "Decodes the free Ogg Vorbis audio format", "BSD"), [](){});
#endif
#ifdef HAVE_OPUS
	AddOption(ConfigParam<std::string>("opus", "Decodes the free OPUS audio codec", "BSD"), [](){});
#endif
#ifdef HAVE_WILDMIDI
	AddOption(ConfigParam<std::string>("WildMidi", "MIDI synthesizer", "LGPLv3+"), [](){});
#endif
#ifdef HAVE_FLUIDSYNTH
	AddOption(ConfigParam<std::string>("FluidSynth", "MIDI synthesizer supporting SoundFont 2", "LGPLv2.1+"), [](){});
#endif
#ifdef HAVE_FLUIDLITE
	AddOption(ConfigParam<std::string>("FluidLite", "MIDI synthesizer supporting SoundFont 2 (lite version)", "LGPLv2.1+"), [](){});
#endif
#ifdef HAVE_XMP
	AddOption(ConfigParam<std::string>("xmp-lite", "Module (MOD, S3M, XM and IT) synthesizer", "MIT"), [](){});
#endif
#ifdef HAVE_LIBSPEEXDSP
	AddOption(ConfigParam<std::string>("speexdsp", "Audio resampler", "BSD"), [](){});
#endif
#ifdef HAVE_LIBSAMPLERATE
	AddOption(ConfigParam<std::string>("samplerate", "Audio resampler", "BSD"), [](){});
#endif
#ifdef WANT_DRWAV
	AddOption(ConfigParam<std::string>("dr_wav", "Decodes sampled audio data (WAV)", "MIT-0"), [](){});
#endif
#ifdef HAVE_ALSA
	AddOption(ConfigParam<std::string>("ALSA", "Linux sound support (used for MIDI playback)", "LGPL2.1+"), [](){});
#endif
#endif
	AddOption(ConfigParam<std::string>("rang", "Colors the terminal output", "Unlicense"), [](){});
#ifdef _WIN32
	AddOption(ConfigParam<std::string>("dirent", "Dirent interface for Microsoft Visual Studio", "MIT"), [](){});
#endif
	AddOption(ConfigParam<std::string>("Baekmuk", "Korean font family", "Baekmuk"), [](){});
	AddOption(ConfigParam<std::string>("Shinonome", "Japanese font family", "Public Domain"), [](){});
	AddOption(ConfigParam<std::string>("ttyp0", "ttyp0 font family", "ttyp0"), [](){});
	AddOption(ConfigParam<std::string>("WenQuanYi", "WenQuanYi font family (CJK)", "GPLv2+ with FE"), [](){});
#ifdef EMSCRIPTEN
	AddOption(ConfigParam<std::string>("PicoJSON", "JSON parser/serializer", "BSD"), [](){});
	AddOption(ConfigParam<std::string>("Teenyicons", "Tiny minimal 1px icons", "MIT"), [](){});
#endif
}

void Window_Settings::RefreshInput() {
	Game_ConfigInput& cfg = Input::GetInputSource()->GetConfig();

	AddOption(ConfigParam<std::string>("Key/Button mapping", "Change the keybindings", ""),
		[this]() { Push(eInputListButtons); });
	AddOption(cfg.gamepad_swap_ab_and_xy, [&cfg](){ cfg.gamepad_swap_ab_and_xy.Toggle(); });
	AddOption(cfg.gamepad_swap_analog, [&cfg](){ cfg.gamepad_swap_analog.Toggle(); });
	AddOption(cfg.gamepad_swap_dpad_with_buttons, [&cfg](){ cfg.gamepad_swap_dpad_with_buttons.Toggle(); });
}

void Window_Settings::RefreshInputMapping() {
	auto& mappings = Input::GetInputSource()->GetButtonMappings();
	auto custom_names = Input::GetInputKeyNames();

	for (int i = 0; i < Input::BUTTON_COUNT; ++i) {
		auto button = static_cast<Input::InputButton>(i);

		std::string name = Input::kButtonNames.tag(button);

		auto help = Input::kButtonHelp.tag(button);
		std::string value = "";
		auto ki = mappings.LowerBound(button);
		if (ki != mappings.end() && ki->first == button) {
			auto custom_name = std::find_if(custom_names.begin(), custom_names.end(), [&](auto& key_pair) {
				return key_pair.first == ki->second;
			});

			if (custom_name != custom_names.end()) {
				value = custom_name->second;
			} else {
				value = Input::Keys::kNames.tag(ki->second);
			}
		}

		bool first_letter = true;
		for (size_t i = 0; i < name.size(); ++i) {
			auto& ch = name[i];
			if (ch >= 'A' && ch <= 'Z') {
				if (!first_letter) {
					ch += 32;
				}
				first_letter = false;
			} else if (ch == '_') {
				ch = ' ';
				first_letter = true;
			}
		}

		auto param = ConfigParam<std::string>(name, help, value);
		AddOption(param,
				[this, button](){
				Push(eInputButtonOption, static_cast<int>(button));
			});
	}
}
